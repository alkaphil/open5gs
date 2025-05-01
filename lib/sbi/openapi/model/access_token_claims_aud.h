/*
 * access_token_claims_aud.h
 *
 * 
 */

#ifndef _OpenAPI_access_token_claims_aud_H_
#define _OpenAPI_access_token_claims_aud_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
#include "nf_type.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_access_token_claims_aud_s OpenAPI_access_token_claims_aud_t;
typedef struct OpenAPI_access_token_claims_aud_s {
} OpenAPI_access_token_claims_aud_t;

OpenAPI_access_token_claims_aud_t *OpenAPI_access_token_claims_aud_create(
);
void OpenAPI_access_token_claims_aud_free(OpenAPI_access_token_claims_aud_t *access_token_claims_aud);
OpenAPI_access_token_claims_aud_t *OpenAPI_access_token_claims_aud_parseFromJSON(cJSON *access_token_claims_audJSON);
cJSON *OpenAPI_access_token_claims_aud_convertToJSON(OpenAPI_access_token_claims_aud_t *access_token_claims_aud);
OpenAPI_access_token_claims_aud_t *OpenAPI_access_token_claims_aud_copy(OpenAPI_access_token_claims_aud_t *dst, OpenAPI_access_token_claims_aud_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_access_token_claims_aud_H_ */

