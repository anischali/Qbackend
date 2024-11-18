#ifndef __SSL_ENGINE_HPP__
#define __SSL_ENGINE_HPP__


#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <fcntl.h>
#include <openssl/evp.h>
#include <openssl/objects.h>
#include <openssl/x509.h>
#include <openssl/x509_vfy.h>
#include <openssl/err.h>
#include <openssl/pem.h>
#include <openssl/rsa.h>
#include <openssl/ssl.h>

class ssl_engine
{
private:
    /* data */
public:

    static size_t b64_encode(const uint8_t *__buffer, size_t __size, uint8_t **__res);
    static size_t b64_decode(uint8_t *__b64_msg, size_t __b64_size, uint8_t **__res);
    static size_t calculate_sha256_hash(const char *__filename, uint8_t **__res, bool __base64);
    static size_t calculate_sha256_hash_buff(const uint8_t *buffer, size_t __size, uint8_t **__res, bool __base64);
    static bool rsa_verify_signature(RSA *__rsa, const char *__digest,
                        const uint8_t *__signed_msg, size_t __smsg_size, 
                        const uint8_t *__msg, size_t __msg_size);
    static size_t rsa_sign_message(RSA *__rsa,  const char *__digest,
                    const uint8_t *__msg, size_t __msg_size, 
                    uint8_t **__signed_message, bool __base64);
    static bool x509_verify_signature(X509 *__x509, const char *__digest,
                        const uint8_t *__signed_msg, size_t __smsg_size, 
                        const uint8_t *__msg, size_t __msg_size);

    static bool x509_verify_cert(X509 *__cert);
    static bool x509_verify_certs_chain(X509 *__cert, X509 *__cacert);
    static bool x509_mem_verify_chain(uint8_t *__chain, size_t __size);
};

#endif