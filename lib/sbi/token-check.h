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

char *load_public_key_from_cert_file(const char *cert_file, size_t *len);
int check_token(char*, char**);
#ifdef __cplusplus
}
#endif

#endif /* TOKEN_CHECK_H */
