/**
 * Sign an image with a private key.
 * The public kay and signature is added to image head so that the image can be validated by secure boot
 *
 * Image file
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
 * sign_image <image_file> <public_key_file> <private_key_file>
 *
 * Output is a file: <image_file>.signed
 */

#include <stdio.h>     /* for fopen fwrite fclose */
#include <stdlib.h>    /* for malloc */
#include <assert.h>    /* for assert */
#include <string.h>    /* for strnlen */
#include <arpa/inet.h> /* for htonl */

#include <openssl/pem.h> /* PEM_read_PUBKEY */
#include <openssl/evp.h> /* EVP_PKEY */
#include <openssl/err.h> /* ERR_print_errors_fp */

#define SB_MAX_FILENAME_LEN (255)
#define SB_BUF_LEN (1024)

#define SB_OK (0)
#define SB_ERR (-1)

#define SB_MAGIC_NUM (0x53494D47)
#define SB_SIGNED_EXT ".signed"
#define SB_SIGNED_EXT_LEN (7)

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
 * Copy a string in to the buffer if there is capacity
 *
 * @param[out] buf the buffer to contain the string
 * @param[in]  src the source string
 *
 * @return 0 - success
 *         -1 - insufficient space to copy the string
 */
int str_buf_copy(str_buf_t *buf, const str_t src)
{
    assert(buf->ptr != NULL);
    if (buf->capacity < src.len)
    {
        return SB_ERR;
    }
    strncpy(buf->ptr, src.ptr, src.len);
    buf->len = src.len;
    return SB_OK;
}

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
 * Add a file extention to a file name.
 *
 * @param[in] str_buf the buffer to free
 * @param[in] str_ext the extention to add
 *
 * @return 0 - success
 *         -1 - insufficient space to add the extention
 */
int str_buf_add_ext(str_buf_t *str_buf, const str_t str_ext)
{
    assert(str_buf->ptr != NULL);

    if ((str_buf->len + str_ext.len) > str_buf->capacity)
    {
        return SB_ERR;
    }
    strncpy(str_buf->ptr + str_buf->len, str_ext.ptr, str_ext.len);
    str_buf->len = str_buf->len + str_ext.len;
    return SB_OK;
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
        str_buf->ptr = NULL;
    }
    str_buf->len = 0;
    str_buf->capacity = 0;
}

/**
 * Generate the signature for the image
 *
 * @param[in]  image_filename Image file name
 * @param[in]  pkey           The private key
 * @param[out] signature      The generated signature
 *
 * @note signature is allocated and must be freed with str_buf_free()
 *
 * @return -1 - error
 *          0 - OK
 */
int generate_signature(const str_t image_filename, EVP_PKEY *pkey, str_buf_t *signature)
{
    assert(signature->ptr == NULL);

    FILE *fp = NULL;
    EVP_MD_CTX *md_ctx = NULL;

    // Initialize OpenSSL
    OpenSSL_add_all_digests();
    ERR_load_crypto_strings();

    // Create and initialize the signing context
    md_ctx = EVP_MD_CTX_new();
    if (md_ctx == NULL)
    {
        goto error;
    }

    // Initialize the signing operation with SHA256
    if (!EVP_SignInit(md_ctx, EVP_sha256()))
    {
        goto error;
    }

    // Read the file and update the context
    fp = fopen(image_filename.ptr, "rb");
    if (fp == NULL)
    {
        goto error;
    }

    unsigned char buffer[SB_BUF_LEN];
    size_t bytes_read;
    while ((bytes_read = fread(buffer, 1, sizeof(buffer), fp)) > 0)
    {
        if (!EVP_SignUpdate(md_ctx, buffer, bytes_read))
        {
            goto error;
        }
    }
    fclose(fp);
    fp = NULL;

    unsigned int sig_len = EVP_PKEY_size(pkey);

    // Allocate buffer for the signature
    str_buf_alloc(signature, (size_t)sig_len);

    // Finalize the signature
    if (signature->capacity == 0)
    {
        goto error;
    }

    if (!EVP_SignFinal(md_ctx, signature->ptr, &sig_len, pkey))
    {
        goto error;
    }
    signature->len = sig_len;

    EVP_MD_CTX_free(md_ctx);
    return SB_OK;

error:
    if (fp != NULL)
    {
        fclose(fp);
    }
    str_buf_free(signature);
    if (md_ctx != NULL)
    {
        EVP_MD_CTX_free(md_ctx);
    }
    return SB_ERR;
}

/**
 * Read red data from the file
 *
 * @param[in]  data_filename The name foe the data file
 * @param[out] buf            The buffer with the data
 *
 * NOTE: The buffer is allocated and must freed using str_buf_free
 *
 * @return -1 - error
 *          0 - OK
 */
int read_data(const str_t data_filename, str_buf_t *buf)
{
    assert(buf->ptr == NULL);

    // Read data from file
    FILE *fp = fopen(data_filename.ptr, "rb");
    if (!fp)
    {
        fprintf(stderr, "Error out of memory.\n");
        return SB_ERR;
    }
    fseek(fp, 0, SEEK_END);

    str_buf_alloc(buf, (size_t)ftell(fp));
    if (buf->capacity == 0)
    {
        fclose(fp);
        fprintf(stderr, "Error opening data file.\n");
        return SB_ERR;
    }
    fseek(fp, 0, SEEK_SET);
    fread(buf->ptr, 1, buf->capacity, fp);
    buf->len = buf->capacity;
    fclose(fp);

    return SB_OK;
}

/**
 * Write out the header
 *
 * @param[in] fp             The signed image file, the file must be open
 * @param[in] public_key_Len The length of the public key data
 * @param[in] signature_Len  The length of the signature
 * @param[in] image_len      The length on the unsigned image
 *
 * @return  0 - success
 *         -1 - failed to write the header
 */
int write_header(FILE *fp, uint32_t public_key_Len, uint32_t signature_len, uint32_t image_len)
{
    uint32_t network_value = htonl(SB_MAGIC_NUM);

    if (fwrite(&network_value, sizeof(uint32_t), 1, fp) != 1)
    {
        return SB_ERR;
    }
    network_value = htonl(public_key_Len);
    if (fwrite(&network_value, sizeof(uint32_t), 1, fp) != 1)
    {
        return SB_ERR;
    }
    network_value = htonl(signature_len);
    if (fwrite(&network_value, sizeof(uint32_t), 1, fp) != 1)
    {
        return SB_ERR;
    }
    network_value = htonl(image_len);
    if (fwrite(&network_value, sizeof(uint32_t), 1, fp) != 1)
    {
        return SB_ERR;
    }
    fflush(fp);
    return SB_OK;
}

/**
 * Write data buffer to the file
 *
 * @param[in] fp the file to write the data to
 * @param[in] buf the string buffer with the data
 *
 * @return 0 -  success
 *         -1 - Failed to write the data
 */
int write_data(FILE *fp, const str_buf_t buf)
{
    if (buf.len > 0)
    {
        if (fwrite(buf.ptr, 1, buf.len, fp) != buf.len)
        {
            fprintf(stderr, "Error out of memory.\n");
            return SB_ERR;
        }
    }
    fflush(fp);
    return SB_OK;
}

/**
 * Write the signed image file out
 *
 * @param[in] image_filename  The base name of the image file
 * @param[in] public_key_data The public key
 * @param[in] signature       The generated signature
 * @param[in] image           The image
 */
int write_signed_image(const str_t image_filename, const str_buf_t public_key_data, const str_buf_t signature, const str_buf_t image)
{
    char buf[SB_MAX_FILENAME_LEN];
    str_buf_t signed_image_filename = {buf, 0, SB_MAX_FILENAME_LEN - 1};

    if (str_buf_copy(&signed_image_filename, image_filename) == SB_ERR)
    {
        return SB_ERR;
    }
    str_t str_ext = {SB_SIGNED_EXT, SB_SIGNED_EXT_LEN};
    if (str_buf_add_ext(&signed_image_filename, str_ext) == SB_ERR)
    {
        return SB_ERR;
    }
    // terminate the name for fopen
    signed_image_filename.ptr[signed_image_filename.len] = 0;
    FILE *signed_image_fp = fopen(signed_image_filename.ptr, "wb");
    if (signed_image_fp == NULL)
    {
        return SB_ERR;
    }

    int result = SB_ERR;

    // TODO:check the down cats first
    if (write_header(signed_image_fp, (uint32_t)public_key_data.len, (uint32_t)signature.len, (uint32_t)image.len) == SB_ERR)
    {
        goto clean_up;
    }

    if (write_data(signed_image_fp, public_key_data) == SB_ERR)
    {
        goto clean_up;
    }
    if (write_data(signed_image_fp, signature) == SB_ERR)
    {
        goto clean_up;
    }
    if (write_data(signed_image_fp, image) == SB_ERR)
    {
        goto clean_up;
    }

    result = SB_OK;

clean_up:
    fclose(signed_image_fp);
    return result;
}

int main(int argc, char *argv[])
{
    if (argc != 4)
    {
        fprintf(stderr, "Usage: %s <image> <public_key> <private_key>\n", argv[0]);
        return EXIT_FAILURE;
    }

    str_t image_filename = str_make(argv[1], SB_MAX_FILENAME_LEN);
    str_t public_filename = str_make(argv[2], SB_MAX_FILENAME_LEN);
    str_t private_filename = str_make(argv[3], SB_MAX_FILENAME_LEN);

    EVP_PKEY *private_key = NULL;
    FILE *fp = NULL;
    int result = EXIT_FAILURE;

    str_buf_t image = {0};
    str_buf_t public_key = {0};
    str_buf_t signature = {0};

    if (read_data(image_filename, &image) == SB_ERR)
    {
        fprintf(stderr, "failed to load the image file: %*s\n", (int)image_filename.len, image_filename.ptr);
        goto clean_up;
    }

    if (read_data(public_filename, &public_key) == SB_ERR)
    {
        fprintf(stderr, "failed to load the public key: %*s\n", (int)public_filename.len, public_filename.ptr);
        goto clean_up;
    }

    fp = fopen(private_filename.ptr, "rb");
    if (fp == NULL)
    {
        fprintf(stderr, "Error opening public key file.\n");
        goto clean_up;
    }

    private_key = PEM_read_PrivateKey(fp, NULL, NULL, NULL);
    fclose(fp);
    fp = NULL;

    if (private_key == NULL)
    {
        fprintf(stderr, "Error reading private key.\n");
        goto clean_up;
    }

    if (generate_signature(image_filename, private_key, &signature) == SB_ERR)
    {
        fprintf(stderr, "Error generating the signature.\n");
        goto clean_up;
    }

    if (write_signed_image(image_filename, public_key, signature, image) == SB_ERR)
    {
        fprintf(stderr, "Error writing the singed image.\n");
        goto clean_up;
    }
    result = EXIT_SUCCESS;

clean_up:
    if (fp != NULL)
    {
        fclose(fp);
    }
    if (private_key != NULL)
    {
        EVP_PKEY_free(private_key);
    }
    str_buf_free(&signature);
    str_buf_free(&public_key);
    str_buf_free(&image);
    return result;
}