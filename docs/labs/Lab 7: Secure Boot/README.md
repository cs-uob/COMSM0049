# Secure boot

## Introduction

In this lab, we will simulate how Secure Boot works. We will have a simple image that can be executed.
We will sign this image using a private key. Later, the image will be "booted" by loading the image, validating it and then running the image.

### Setting up Vagrant

The rest of the labs in this course are designed to run in *virtual machines* on the lab machines set up via Vagrant. Expect to spend the first 10 minutes
of any lab re-downloading the VM images.

1. On your host machine, open a terminal in your home directory (or whatever directory you are assigned which has good memory). Make a directory `mkdir CS_vagrant`
2. `cd CS_vagrant` and then make another directory `mkdir seclabs`
3. `cd seclabs`
4. `vagrant init hashicorp/bionic64`.
5. `vagrant up`. The first time, this will download Ubuntu 18.04-64. It will take a while. Once done, we are ready to launch this VM.
6. Now ssh into our new VM. `vagrant ssh`

- You are in your new VM. This VM shares a folder with your host machine, which is the same folder from where you launched your VM, i.e. `seclabs`.
- This folder is mapped to `/vagrant` in your VM. If you do `cd /vagrant`, you can see the content of your host folder.
- We will use this folder for all our work (i.e., you can copy your files into the seclabs folder and access them from the /vagrant folder on the VM).

This VM is running a fresh Ubuntu. So, we need to install a few things. Run these commands.

1. `sudo apt update`
2. `sudo apt install build-essential -y` This will install gcc.
3. `sudo apt install openssl libssl-dev -y` This will install the OpenSSL command line and SSL/crypto libraries.
4. Now you have all the tools that will be required. To test, run these commands and check if they run properly-- `gcc --version`, `openssl version`
5. When done with your lab, you can `logout` and then, on your host machine, you can close the VM-- `vagrant halt`

### Signing and validating the image

The image is signed with the `image_sign` utility. This utility takes the private key, the public key and the image, then generates the signed image using the following steps:

A. The signature of the image is generated using the private key.
B. The signed image is then created by writing out the header, the public key, signature and the image.
C. The source for the utility is in the file `image_sign.c`

The `secure_boot` utility simulates the boot process on a device. This utility reads the signed image, validates it, and, if successful, runs it.

The image validation uses the public key. This public key is stored in the signed image file. This public key must also be validated before it can be used to validate the image. This is done in the function `verify_public_key()`
The public key is hashed to generate the fingerprint:

- The `generated fingerprint` is compared to the `device fingerprint`. In our simulation, the `device fingerprint` is compiled into the `secure_boot` utility.
- If the fingerprints are the same, then the public key can be used to validate the image

The image is validated in the function `verify_image()` by doing the following:

- The hash of the image is calculated
- This hash is then compared with the encrypted hash in the signature
- If they are the same, the image is valid

The source code for the utility is in the file `secure_boot.c`

Only images signed with the corresponding private key can be validated by the `secure_boot` utility and then run.

### The image

The image is a simple shell script that we can run. Before running the image, we need to validate that it is signed with the private key.
Below is the test script that we will be using:

```bash
#!/bin/bash
echo "Hello, World!"
```

## Signing the image

### Generating the private and public keys

Before signing any images, we need to generate a private and public key pair:

1. `openssl genrsa -out privatekey.pem 2048` This generates the new private key.
2. We can view this key using `openssl rsa -text -in privatekey.pem -noout`
3. `openssl rsa -in privatekey.pem -outform PEM -pubout -out publickey.pem` This generates the public key from the private key.
4. Again, we can look at the public key `openssl rsa -pubin publickey.pem -text -noout`

### Provision the public key fingerprint

`secure_boot` utility mimics the process that is executed when an IoT device is booting. Secure boot is often implemented in Read-Only Memory (ROM) and will read the fingerprint from specialised memory.
For this experiment, we use the static variable `s_public_key_fingerprint` to store the fingerprint.

You need to provision the public key into the `secure_boot` utility by adding the value to the source code `secure_boot.c`. You will need to copy this file from the zip file to your Vagrant MV.

The fingerprint is in the variable `s_public_key_fingerprint` below:

```c
/**
 * The fingerprint of the public key used to validate the key
 */
static  unsigned char s_public_key_fingerprint[] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                                                    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                                                    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                                                    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,};
```

Replace this value with the fingerprint of the public key we already generated.

1. `openssl dgst -sha256 -binary -out publickey.sha256.bin publickey.pem` This will create a sha256 hash of the public key
2. `xxd -p publickey.sha256.bin | tr -d '\n' | sed 's/../0x&\,/g; s/,$//'` To view the file as the file in the format 0x??,
3. Update the `s_public_key_fingerprint` in the source code `secure_boot.c`.

We have now provisioned the public key fingerprint

### Example of signing and verifying the image

The `sign_image` utility generates the signature, and then `secure_boot` uses it to validate the image.
For example, we can use the OpenSSL command line to create a signature and then use it to validate the image.

1. `openssl dgst -sha256 -sign privatekey.pem -out image.signature image.sh` This generates an image signature `image.signature`.
2. `xxd image.signature` To view the signature
3. `openssl dgst -sha256 -verify publickey.pem -signature image.signature image.sh` This will validate the

### Signing the image using `sign_image` utility

We need to build the `sign_image` utility.

1. `gcc -o sign_image sign_image.c -lssl -lcrypto` This will build the `sign_image` utility from source.
Once built, we can then use the utility to sign the image
2. `./sign_image image.sh publickey.pem privatekey.pem` This will generate the image.sh.signed file with the public key, signature and image

## Validating the secure boot image

Validating the image is done in two steps:

A. Validate the public key with the provisioned fingerprint
B. Validate the image with the signature and the public key

## Validating the public key

1. `gcc -o secure_boot secure_boot.c -lssl -lcrypto` This will build the `secure_boot` utility.
2. `./secure_boot image.sh.signed` This will validate the image and, if it is valid, then run the image.

Running the `secure_boot` utility will fail, as the function that validates the public key is not implemented.

```c
/**
 * Validate the public key with the built-in interface
 *
 * @param[in] public_key_buf
 *
 * @return 1 - the public key is valid
 *         0 - the key is not valid
 */
int verify_public_key(const str_buf_t *public_key_buf)
{
    return 0;
}
```

The ssl function `EVP_Digest((void *)key, key_len, hash, NULL, EVP_sha256(), NULL)` will generate a `sha256` of the key.
If the generated hash is the same as the `s_public_key_fingerprint`, then the function will return 1.
3. Implement the `verify_public_key()` function to validate the public key. Once the public key is validated, the image should be validated using the public key

## Validating the Image

```c
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
    return 0;
}
```

OpenSSL can validate the image using the signature and public key.
The SSL function `EVP_DigestVerifyInit(ctx, NULL, EVP_sha256(), NULL, public_key)` will set up the verification context to use the SHA-256 hash.
The function `EVP_DigestVerifyUpdate(ctx, image, image_len)` hashes the image.
The `EVP_DigestVerifyFinal(ctx, signature, signature_._len)` verifies the data in context against the signature

1. Implement the `verify_image()` function to validate the image; if the image is valid, then return 1.
2. `secure_boot` should now run the image

## Key revocation

### Provision 4 keys

In this simulation, `secure_boot.c` is taking the role of the boot ROM. The IoT would store the key in OTP memory. To simulate putting a key in OTP memory, we compile the code with the Key.

An IoT device would often have multiple keys provisioned. When the Image is validated, an algorithm determines which key to use. This could be as simple as validating the public key in the image against all provisioned fingerprints.  If the public key validates with any fingerprint, then use the public key to validate the image.

A flag normally signals that the key (fingerprint) is no longer valid, i.e., revoked. This is normally done by setting a revocation bit in OPT memory.

1. Generate 3 additional private and public keys.
2. Update the code to allow the image to be validated using all the provisioned keys that are not revoked

### Revoke a key

This process for key revocation is:

A Create a new Image, that includes both the standard software and an update process.
B. Sign the image with a valid key
C. Boot the new image; this will then mark the reliant key as revoked.

To simulate the process, do the following:

1. Create a new version of `image.sh`, named `update.sh`.
`image.sh` should have been signed with the first key. We will revoke this key.
`update.sh` should first revoke the key and then run the normal code.
To revoke the key. The `update.sh` should do the following process:
   A. Edit the `secure_boot.c` file so that the first key is flagged as revoked.
   B. Compile the code. If the compile fails, the device is bricked!
2. Sign this `update.sh` image with one of the new keys and generate a signed image `update.sh.signed`
3. Boot the image: `secure_boot update.sh.signed`. This should revoke the key and run the normal code.
If this works, `secure_boot sign_image` should fail, since it was signed with the key that is now revoked.
4. Test `secure_boot image.sh.signed`; does `image.sh.signed` run?
