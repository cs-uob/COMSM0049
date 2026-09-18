/* *******************************************************************************
 * This is a simpple version of the heartbleaad vulnerability to demonstrate the *
 * information leak (buffer overread) vulnerability.                             *
 *
 * ******************************************************************************/


#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>


#define n2s(c,s)    ((s=(((unsigned int)(c[0]))<< 8)| \
                            (((unsigned int)(c[1]))    )),c+=2) //it extract the 1st 2 bytes from c 
                                                        // and put them in s (lenth of the message as is read from the input file) 
#define s2n(s,c)    ((c[0]=(unsigned char)(((s)>> 8)&0xff), c[1]=(unsigned char)(((s)    )&0xff)),c+=2)


typedef struct ssl3_record_st
{
    /*r */  int type;               /* type of record NOT USED */
    /*rw*/  unsigned int length;    /* How many bytes available NOT uSED*/
    /*r */  unsigned int off;       /* read/write offset into 'buf' NOT USED*/
    /*rw*/  unsigned char *data;    /* pointer to the record data. it contains the whole input */
} SSL;

int process_heartbeet(SSL *s) {
    unsigned char *p = &s->data[0], *pl; //points to the message filed of the input
    unsigned short hbtype;
    unsigned int payload; // for keeping the lenth of the message from the input
    unsigned int padding = 16; /* Use minimum padding. Added just to have some resemblance with the real code. */
    unsigned int size_buf;

    /* Read type and payload length first */
    hbtype = *p++; // reads the 1st byte from the input
    n2s(p, payload); // payload has the lenth now
    pl = p;//pl points to the start of the message (3rd byte) from the input file
    // printf("payload - %d, message - %s\n", payload, pl);
    unsigned char *buffer, *bp, *pr;
    int r, out;

    /* Allocate memory for the response, size is 1 byte
     *  * message type, plus 2 bytes payload length, plus
     *   * payload, plus padding
     *   */
    size_buf=1 + 2 + payload + padding;
    buffer = malloc(size_buf);//note we are allocating the size which includes lenth field extracted from the input
    bp = buffer;
    pr= buffer;

    s2n(payload, bp);
    memcpy(bp, pl, payload); // copying the contain from pl to bp. 
    /* The assumption is: payload = len(pl). However, payload is also extracted from the input file. If we put a large number there,
     * that large number will be used to read memory starting from &pl. In normal condition, payload is indeed equal to the length of
     * the message field. Overread happens when we provide a large payload number. Normal fuzzing process will not detect it, becuase????
     */
    
    //lets print the buffer
    printf("[*] HeartBeat:\n");
    for (out=0; out<size_buf; out++)
        printf("%02x",pr[out]);
    printf("Done. \n");

    free(buffer);
}

int main(int argc, char** argv) {
    /**************************************************************
     * the main function takes one input-- a binary file. the format of the input is:
     * type: 1 byte
     * len: 2 byte
     * message: heartbeat message of length len
     * 0----1----2----3-------------- <-byte offsets
     * |type|  len    |  message....
     * ------------------------------
     */
    const char *fname = argv[1];
    printf("%s\n", fname);
    FILE *fp = fopen(fname, "r");
    fseek(fp, 0L, SEEK_END);
    unsigned long fsize = ftell(fp);
    rewind(fp);
    SSL ssl_struct;
    unsigned char *data = malloc(fsize+1); // this is the place where overread happens (see the function above)
    data[0]=fgetc(fp); // Random byte for type field.

    data[1]=fgetc(fp);// 1 byte of len
    data[2]=fgetc(fp);//2nd byte of len

    int record_length=3;
    int c;
    //printf("%c %c %c %c\n", data[0], data[1], data[2], c);
    while((c=fgetc(fp))!=EOF) {
       // printf("%c\n", c);
        data[record_length++]=(unsigned char)c;//rest of the message of len size
    }
    //data[record_length++]='\0'; // ending the string with null character
   
    ssl_struct.data = data;
    process_heartbeet(&ssl_struct);
    free(data);
    fclose(fp);
    return 0;
}
