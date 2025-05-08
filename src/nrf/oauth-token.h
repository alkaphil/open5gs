#ifndef OAUTH_TOKEN_H
#define OAUTH_TOKEN_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <jwt.h>
#include "event.h"
#include "context.h"

#ifdef __cplusplus
extern "C" {
#endif

#define OGS_OAUTH_TOKEN_ALG JWT_ALG_RS256

int64_t get_expiration_time(int duration_seconds);
void jwt_free_str(char* p);
char *load_private_key_from_file(const char *filename, size_t *len);
char* get_token_from_claim(const char* iss, const char* sub, const char* aud, const char* scope, jwt_alg_t algorithm, int* expires_in);
// HCkPublicKey ExtractPublicKeyFromCert(const char *certPath);
// int check_token(char*);
#ifdef __cplusplus
}
#endif

#endif /* OAUTH_TOKEN_H */
