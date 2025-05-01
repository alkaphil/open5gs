#ifndef OAUTH_TOKEN_H
#define OAUTH_TOKEN_H

#include <C_CkPrivateKey.h>
#include <C_CkJwt.h>
#include <C_CkJsonObject.h>
#include "event.h"
#include "context.h"

#ifdef __cplusplus
extern "C" {
#endif

#define OGS_OAUTH_TOKEN_ALG "RS256"
#define OGS_OAUTH_TOKEN_TYPE "JWT"

char* get_token_from_claim(const char* iss, const char* sub, const char* aud, const char* scope, const char* algorithm, const char* type, int* expires_in);

#ifdef __cplusplus
}
#endif

#endif /* OAUTH_TOKEN_H */
