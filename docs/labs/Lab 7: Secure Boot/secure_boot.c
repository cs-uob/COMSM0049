/**
 * Run an image, after validating the signature
 *
 * Signed image file
 *  --------------------------------
 *  |  0- 3 | Magic number         | - To identify the file type
 *  |  4- 7 | Length of public key | - The length of the public key use to validate the Image
 *  |  8-11 | Length of Signature  |
 *  | 12-17 | Length of Image      |
 *  --------------------------------
 *  | 12-   | public key           |
 *  --------------------------------
 *  | 12 + length of public key    |
 *  |       | Signature            |
 *  --------------------------------
 *  | 12 + length of public key    |
 *  |    + Length of Signature     |
 *  |       | Image                |
 *  --------------------------------
 *
 *
 * usage:
 * secure_boot <signed_image>
 *
 * Output is a file: <image_file>.signed
 */

#include <stdio.h>     /* fopen, fread, fwrite, fclose */
#include <stdlib.h>    /* for malloc */
#include <assert.h>    /* for assert */
#include <string.h>    /* for strnlen */
#include <arpa/inet.h> /* for ntohl */

#include <openssl/sha.h> /*  */
#include <openssl/pem.h> /* PEM_read_PUBKEY */
#include <openssl/evp.h> /* EVP_PKEY */
#include <openssl/err.h> /* ERR_print_errors_fp */

#define SB_MAX_FILENAME_LEN (255)
#define SB_MAGIC_NUM (0x53494D47)

#define SB_OK (0)
#define SB_VALID (1)
#define SB_ERR (-1)

/**
 * The fingerprint of the public key used to validate the key
 */
static unsigned char s_public_key_fingerprint[] = {
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
};
/**
 * A string pointer with length
 */
typedef struct
{
    char *ptr;
    size_t len;
} str_t;

/**
 * Make a string that with with a length and terminate the sting with 0
 * If the length is max_len
 *
 * @param[in] the str to make
 * @param[in] the maximum length of the string
 *
 * @return a str with a len and a terminator added if needed
 */
str_t str_make(char *str, size_t max_len)
{
    size_t len = strnlen(str, max_len);
    if (len == max_len)
    {
        str[len] = 0;
    }
    str_t ret = {str, len};
    return ret;
}

/**
 * A string buffer, with length and buffer capacity
 */
typedef struct
{
    char *ptr;
    size_t len;
    size_t capacity;
} str_buf_t;

/**
 * Allocate a string buffer with the given capacity
 *
 * @param[out] buf      The allocated buffer with the capacity that was allocated,
 *                      If the capacity is 0 then the allocation failed.
 * @param[in]  capacity The required capacity
 */
void str_buf_alloc(str_buf_t *buf, size_t capacity)
{
    assert(buf->ptr == NULL);

    buf->ptr = malloc(capacity);
    if (buf->ptr != NULL)
    {
        buf->capacity = capacity;
    }
}

/**
 * Free a previously allocated string buffer
 *
 * @param[in] str_buf the buffer to free
 */
void str_buf_free(str_buf_t *str_buf)
{
    if (str_buf->ptr != NULL)
    {
        free(str_buf->ptr);
    }
    str_buf->len = 0;
    str_buf->capacity = 0;
}

/**
 * Validate the public key with the built in interface
 *
 * @param[in] public_key_buf
 *
 * @return 1 - the public key is valid
 *         0 - the key is not valid
 */
int verify_public_key(const str_buf_t *public_key_buf)
{
    return SB_OK;
}

/**
 * verify the boot image
 *
 * @param[in] image the image buffer
 * @param[in] signature the signature buffer
 * @param[in] public_key the public key to validate the image
 *
 * @return 0 - the Image is not valid
 *         1 - the Image is valid
 *         -1 - an error
 */
int verify_image(const str_buf_t image, const str_buf_t signature, EVP_PKEY *public_key)
{
    return SB_OK;
}

/**
 * read the header
 *
 * @param[in] fp              The signed image file, the file must be open
 * @param[out] public_key_Len The length of the public key data
 * @param[out] signature_Len  The length of the signature
 * @param[out] image_len      The length on the unsigned image
 *
 * @return  0 - success
 *         -1 - failed to write the header
 */
int read_header(FILE *fp, uint32_t *public_key_Len, uint32_t *signature_len, uint32_t *image_len)
{
    uint32_t network_value = 0;

    if (fread(&network_value, sizeof(uint32_t), 1, fp) != 1)
    {
        return SB_ERR;
    }
    if (ntohl(network_value) != SB_MAGIC_NUM)
    {
        return SB_ERR;
    }
    if (fread(&network_value, sizeof(uint32_t), 1, fp) != 1)
    {
        return SB_ERR;
    }
    *public_key_Len = ntohl(network_value);
    if (fread(&network_value, sizeof(uint32_t), 1, fp) != 1)
    {
        return SB_ERR;
    }
    *signature_len = ntohl(network_value);
    if (fread(&network_value, sizeof(uint32_t), 1, fp) != 1)
    {
        return SB_ERR;
    }
    *image_len = ntohl(network_value);
    return SB_OK;
}

/**
 * Read data from the file
 *
 * @param[in]  fp       The file to read from, must be open
 * @param[in]  data_len The length of the data to read
 * @param[out] buf      The buffer with the data
 *
 * @return -1 - error
 *          0 - OK
 */
int read_buf(FILE *fp, size_t data_len, str_buf_t *buf)
{
    assert(buf->ptr != NULL);
    assert(buf->capacity >= data_len);

    if (fread(buf->ptr, 1, data_len, fp) != data_len)
    {
        return SB_ERR;
    }
    buf->len = data_len;
    return SB_OK;
}

/**
 * Read data from the file
 *
 * @param[in]  fp       The file to read from, must be open
 * @param[in]  data_len The length of the data to read
 * @param[out] buf      The buffer with the data
 * @note The buffer is allocated and must be freed with str_buf_free()
 *
 * @return -1 - error
 *          0 - OK
 */
int read_data(FILE *fp, size_t data_len, str_buf_t *buf)
{
    assert(buf->ptr == NULL);

    str_buf_alloc(buf, data_len);
    if (buf->capacity == 0)
    {
        return SB_ERR;
    }

    return read_buf(fp, data_len, buf);
}

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        fprintf(stderr, "Usage: %s <singed_image> \n", argv[0]);
        return EXIT_FAILURE;
    }
    str_t singed_image_filename = str_make(argv[1], SB_MAX_FILENAME_LEN);

    str_buf_t public_key_buf = {0};
    str_buf_t signature = {0};
    str_buf_t image = {0};
    BIO *bio = NULL;
    EVP_PKEY *public_key = NULL;
    int result = EXIT_FAILURE;

    FILE *fp = fopen(singed_image_filename.ptr, "rb");
    if (fp == NULL)
    {
        fprintf(stderr, "Error opening public key file.\n");
        goto clean_up;
    }

    u_int32_t public_key_len = 0;
    u_int32_t signature_len = 0;
    u_int32_t image_len = 0;

    if (read_header(fp, &public_key_len, &signature_len, &image_len) == SB_ERR)
    {
        fprintf(stderr, "Error filed to read the header.\n");
        goto clean_up;
    }

    if (read_data(fp, public_key_len, &public_key_buf) == SB_ERR)
    {
        fprintf(stderr, "Error failed to read the public key.\n");
        goto clean_up;
    }

    if (verify_public_key(&public_key_buf) != SB_VALID)
    {
        printf("The public key is not valid.\n");
        goto clean_up;
    }

    bio = BIO_new_mem_buf((void *)public_key_buf.ptr, (int)public_key_buf.len);
    if (bio == NULL)
    {
        fprintf(stderr, "Error failed to create the private key buffer.\n");
        goto clean_up;
    }

    public_key = PEM_read_bio_PUBKEY(bio, NULL, NULL, NULL);
    if (public_key == NULL)
    {
        fprintf(stderr, "Error reading public key.\n");
        goto clean_up;
    }

    if (read_data(fp, signature_len, &signature) == SB_ERR)
    {
        fprintf(stderr, "Error failed to read the signature.\n");
        goto clean_up;
    }

    /* add a byte at the end, so that the string can be terminated for the system call*/

    str_buf_alloc(&image, image_len + 1);
    if (image.capacity == 0)
    {
        fprintf(stderr, "Error allocate memory for the image.\n");
        return SB_ERR;
    }

    if (read_buf(fp, image_len, &image) == SB_ERR)
    {
        fprintf(stderr, "Error failed to read  the image.\n");
        goto clean_up;
    }
    // add string terminator
    image.ptr[image.len] = 0;

    fclose(fp);
    fp = NULL;

    int err = verify_image(image, signature, public_key);
    if (err == SB_ERR)
    {
        fprintf(stderr, "Error in signature validation.\n");
        goto clean_up;
    }

    if (err == SB_VALID)
    {
        // very very dangerous! make certain the image is valid!!!
        system(image.ptr);
    }
    else
    {
        printf("Failed signature validation.\n");
    }
    result = EXIT_SUCCESS;

clean_up:
    str_buf_free(&image);
    str_buf_free(&signature);
    if (public_key != NULL)
    {
        EVP_PKEY_free(public_key);
    }
    if (bio != NULL)
    {
        BIO_free(bio);
    }
    str_buf_free(&public_key_buf);
    if (fp != NULL)
    {
        fclose(fp);
    }
    return result;
}