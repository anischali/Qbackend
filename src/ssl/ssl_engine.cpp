#include "ssl_engine.hpp"
#include <iostream>
#include <stdint.h>
#include <stdint.h>
#include <string.h>


/**
 * @brief Encodes a string in base 64. 
 * 
 * @param __buffer clear string
 * @param __size the size of clear string.
 * @param __res a pointer to null distination buffer.
 * @return size_t the final size of the encodded string.
 */
size_t ssl_engine::b64_encode(const uint8_t *__buffer, size_t __size, uint8_t **__res) {
    BIO *b64, *bmem;
    BUF_MEM *ptr;
    size_t __length = __size;

    b64 = BIO_new(BIO_f_base64());
    BIO_set_flags(b64, BIO_FLAGS_BASE64_NO_NL);
    bmem = BIO_new(BIO_s_mem());
    bmem = BIO_push(b64, bmem);

    BIO_write(bmem, __buffer, __size);
    BIO_flush(bmem);
    BIO_get_mem_ptr(bmem, &ptr);
    __length = (*ptr).length;
    *__res = (uint8_t *) malloc(__length);
    memcpy(*__res, (uint8_t *)(*ptr).data, __length);
    
    BIO_free_all(bmem);
    return __length;
}


/**
 * @brief Decodes a base 64 string
 * 
 * @param __b64_msg encoded base 64 string.
 * @param __b64_size the size of the encoded string.
 * @param __res a pointer to null distination buffer.
 * @return size_t the final size of the decoded string.
 */
size_t ssl_engine::b64_decode(uint8_t *__b64_msg, size_t __b64_size, uint8_t **__res) {
     BIO *b64, *bmem;
    BUF_MEM *ptr;
    uint8_t *p = &__b64_msg[__b64_size - 1];
    int padding =  (*p == '=') ? ((p-- && *p == '=') ? 2 : 1) : 0;
    size_t __length = (__b64_size * 3) / 4 - padding;

    *__res = (uint8_t *) malloc((__length * sizeof(uint8_t)) + 1);
    memset(*__res, 0, __length + 1);

    bmem = BIO_new_mem_buf(__b64_msg, -1);
    b64 = BIO_new(BIO_f_base64());
    BIO_set_flags(b64, BIO_FLAGS_BASE64_NO_NL);
    bmem = BIO_push(b64, bmem);


    __length = BIO_read(bmem, *__res, __b64_size);
    BIO_free_all(bmem);
    
    return __length;
}

/**
 * @brief 
 * 
 * @param __filename 
 * @param __res 
 * @param __base64 
 * @return size_t 
 */
size_t ssl_engine::calculate_sha256_hash(const char *__filename, uint8_t **__res, bool __base64) {
    
    size_t ret = 0, __length = SHA256_DIGEST_LENGTH * sizeof(uint8_t);
    SHA256_CTX ctx;
    uint8_t *sha256_hash = (uint8_t *) malloc(__length);
    int fd = 0;
    uint8_t tmp[SHA256_DIGEST_LENGTH];

    fd = open(__filename, O_RDONLY, 0);
    if (fd <= 0)
        return -1;

    SHA256_Init(&ctx);
    
    do {
        ret = read(fd, tmp, SHA256_DIGEST_LENGTH);
        SHA256_Update(&ctx, tmp, ret);
    } while (ret > 0);

    close(fd);

    SHA256_Final(sha256_hash, &ctx);
    *__res = sha256_hash;
    if (__base64) {
        __length = b64_encode(sha256_hash, __length, __res);
        free(sha256_hash);
    }
    return __length;
}



/**
 * @brief 
 * 
 * @param buffer 
 * @param __size 
 * @param __res 
 * @param __base64 
 * @return size_t 
 */
size_t ssl_engine::calculate_sha256_hash_buff(const uint8_t *buffer, size_t __size, uint8_t **__res, bool __base64) {
    
    size_t __length = SHA256_DIGEST_LENGTH * sizeof(uint8_t);
    SHA256_CTX ctx;
    uint8_t *sha256_hash = (uint8_t *) malloc(__length);
    const uint8_t *ptr = &buffer[0];
    
    SHA256_Init(&ctx);
    
    for (int i = 0; i < __size; i += SHA256_DIGEST_LENGTH) {
        size_t s = std::min(SHA256_DIGEST_LENGTH, (int)(__size - i));
        SHA256_Update(&ctx, &buffer[i], s);
    }
    
    SHA256_Final(sha256_hash, &ctx);
    *__res = sha256_hash;
    if (__base64) {
        __length = b64_encode(sha256_hash, __length, __res);
        free(sha256_hash);
    }
    return __length;
}


/**
 * @brief 
 * 
 * @param __rsa 
 * @param __digest 
 * @param __signed_msg 
 * @param __smsg_size 
 * @param __msg 
 * @param __msg_size 
 * @return true 
 * @return false 
 */
bool ssl_engine::rsa_verify_signature(RSA *__rsa, const char *__digest,
                        const uint8_t *__signed_msg, size_t __smsg_size, 
                        const uint8_t *__msg, size_t __msg_size) {
                            
    EVP_PKEY *puk = EVP_PKEY_new();
    EVP_PKEY_assign_RSA(puk, __rsa); 
    const EVP_MD *evp_md = EVP_get_digestbyname(__digest);
    EVP_MD_CTX *context = EVP_MD_CTX_new();
    

    if (!evp_md || !context || !puk)
        return false;

    EVP_VerifyInit(context, evp_md); 
    
    EVP_VerifyUpdate(context, __msg, __msg_size);

    int status = EVP_VerifyFinal(context, __signed_msg, __smsg_size, puk);
    
    EVP_MD_CTX_free(context);
    EVP_PKEY_free(puk);
    return status == 1;
}

bool ssl_engine::x509_verify_signature(X509 *__x509, const char *__digest,
                        const uint8_t *__signed_msg, size_t __smsg_size, 
                        const uint8_t *__msg, size_t __msg_size) {
                            
    EVP_PKEY *puk = X509_get_pubkey(__x509); 
    const EVP_MD *evp_md = EVP_get_digestbyname(__digest);
    EVP_MD_CTX *context = EVP_MD_CTX_new();

    EVP_VerifyInit(context, evp_md); 
    
    EVP_VerifyUpdate(context, __msg, __msg_size);

    int status = EVP_VerifyFinal(context, __signed_msg, __smsg_size, puk);
    
    EVP_MD_CTX_free(context);
    EVP_PKEY_free(puk);
    return status == 1;
}

size_t ssl_engine::rsa_sign_message(RSA *__rsa,  const char *__digest,
                    const uint8_t *__msg, size_t __msg_size, 
                    uint8_t **__signed_message, bool __base64) {
                            
    size_t __length = 0;
    unsigned int __len = 0;
    EVP_PKEY *puk = EVP_PKEY_new();
    EVP_PKEY_assign_RSA(puk, __rsa); 
    const EVP_MD *evp_md = EVP_get_digestbyname(__digest);
    EVP_MD_CTX *context = EVP_MD_CTX_new();
    uint8_t *tmp;
    
    EVP_SignInit(context, evp_md); 
    
    EVP_VerifyUpdate(context, __msg, __msg_size);

    EVP_SignFinal(context, NULL, &__len, puk);
    tmp = (uint8_t *) malloc(sizeof(uint8_t) * __len);
    EVP_SignFinal(context, tmp, &__len, puk);
    
    *__signed_message = tmp;
    __length = __len;
    if (__base64) {
        *__signed_message = NULL;
        __length = b64_encode(tmp, __length, __signed_message);
        free(tmp);
    }

    EVP_MD_CTX_free(context);
    EVP_PKEY_free(puk);
    return __length;
}

bool ssl_engine::x509_verify_cert(X509 *__cert) {
    int ret = 0;
    EVP_PKEY *pkey = X509_get_pubkey(__cert);

    ret = X509_verify(__cert, pkey);
    EVP_PKEY_free(pkey);

    return ret == 1;
}

bool ssl_engine::x509_verify_certs_chain(X509 *__cert, X509 *__cacert) {
    int ret;
    X509_STORE *store;
    X509_STORE_CTX *ctx;

    store = X509_STORE_new();
    X509_STORE_add_cert(store, __cacert);

    ctx = X509_STORE_CTX_new();
    X509_STORE_CTX_init(ctx, store, __cert, NULL);

    ret = X509_verify_cert(ctx);

    X509_STORE_CTX_free(ctx);
    X509_STORE_free(store);
    
    return ret == 1;
}

/**
 * @brief 
 * 
 * @param __chain 
 * @param __size 
 * @return true 
 * @return false 
 */
bool ssl_engine::x509_mem_verify_chain(uint8_t *__chain, size_t __size) {
    int ret = -1, i = 0;
    BIO *bio;
    X509 *cert = NULL, *scert;
    X509_STORE *store;
    X509_STORE_CTX *ctx;


    store = X509_STORE_new();
    if (!store)
        return false;

    bio = BIO_new_mem_buf(__chain, __size);
    if(!bio)
       goto err1;

    do {
        cert = PEM_read_bio_X509(bio, NULL, 0, NULL);
        if (cert && !X509_STORE_add_cert(store, cert)) {   
            goto err2;
        }
        X509_free(cert);
    } while(cert != NULL && (scert = cert));

    ctx = X509_STORE_CTX_new();
    if (!ctx) 
        goto err2;
    
    X509_STORE_CTX_init(ctx, store, scert, NULL);
    
    ret = X509_verify_cert(ctx);
    if (!ret) {
        /*If a full check doesn't work try a partial check*/
        X509_STORE_CTX_free(ctx);
        ctx = NULL;
        ctx = X509_STORE_CTX_new();
        if (!ctx) 
            goto err2;

        X509_STORE_CTX_init(ctx, store, scert, NULL);
        X509_STORE_CTX_set_flags(ctx, X509_V_FLAG_PARTIAL_CHAIN);
        ret = X509_verify_cert(ctx);
    }
    
    X509_STORE_CTX_free(ctx);
    X509_STORE_free(store); 
    BIO_free(bio);

    return ret == 1;

err2:
    BIO_free(bio);
err1:
    X509_STORE_free(store);  
    return false;
}
