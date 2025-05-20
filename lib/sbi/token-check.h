#ifndef TOKEN_CHECK_H
#define TOKEN_CHECK_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <jwt.h>
#include <openssl/x509.h>
#include <openssl/pem.h>
#include <openssl/bio.h>
#include <openssl/evp.h>
#include "ogs-sbi.h"

#ifdef __cplusplus
extern "C" {
#endif
int ogs_num_delimeter2(const char* s, const char deli);
int ogs_find_string(const char* s,const char deli, const char* service_name);
char *load_public_key_from_cert_file(const char *cert_file, size_t *len);
int verify_token_producer(char* token, OpenAPI_nf_type_e nf_type, char* service_name);
int verify_token_consumer(char* token);
#ifdef __cplusplus
}
#endif

#endif /* TOKEN_CHECK_H */
