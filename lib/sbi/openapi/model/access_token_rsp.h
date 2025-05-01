/*
 * access_token_rsp.h
 *
 * Contains information related to the access token response
 */

#ifndef _OpenAPI_access_token_rsp_H_
#define _OpenAPI_access_token_rsp_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_access_token_rsp_s OpenAPI_access_token_rsp_t;
typedef enum { OpenAPI_access_token_rsp_TOKENTYPE_NULL = 0, OpenAPI_access_token_rsp_TOKENTYPE_Bearer } OpenAPI_access_token_rsp_token_type_e;

char* OpenAPI_access_token_rsp_token_type_ToString(OpenAPI_access_token_rsp_token_type_e token_type);

OpenAPI_access_token_rsp_token_type_e OpenAPI_access_token_rsp_token_type_FromString(char* token_type);
typedef struct OpenAPI_access_token_rsp_s {
    char *access_token;
    OpenAPI_access_token_rsp_token_type_e token_type;
    bool is_expires_in;
    int expires_in;
    char *scope;
} OpenAPI_access_token_rsp_t;

OpenAPI_access_token_rsp_t *OpenAPI_access_token_rsp_create(
    char *access_token,
    OpenAPI_access_token_rsp_token_type_e token_type,
    bool is_expires_in,
    int expires_in,
    char *scope
);
void OpenAPI_access_token_rsp_free(OpenAPI_access_token_rsp_t *access_token_rsp);
OpenAPI_access_token_rsp_t *OpenAPI_access_token_rsp_parseFromJSON(cJSON *access_token_rspJSON);
cJSON *OpenAPI_access_token_rsp_convertToJSON(OpenAPI_access_token_rsp_t *access_token_rsp);
OpenAPI_access_token_rsp_t *OpenAPI_access_token_rsp_copy(OpenAPI_access_token_rsp_t *dst, OpenAPI_access_token_rsp_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_access_token_rsp_H_ */

