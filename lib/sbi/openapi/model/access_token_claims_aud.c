
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "access_token_claims_aud.h"

OpenAPI_access_token_claims_aud_t *OpenAPI_access_token_claims_aud_create(
)
{
    OpenAPI_access_token_claims_aud_t *access_token_claims_aud_local_var = ogs_malloc(sizeof(OpenAPI_access_token_claims_aud_t));
    ogs_assert(access_token_claims_aud_local_var);


    return access_token_claims_aud_local_var;
}

void OpenAPI_access_token_claims_aud_free(OpenAPI_access_token_claims_aud_t *access_token_claims_aud)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == access_token_claims_aud) {
        return;
    }
    ogs_free(access_token_claims_aud);
}

cJSON *OpenAPI_access_token_claims_aud_convertToJSON(OpenAPI_access_token_claims_aud_t *access_token_claims_aud)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (access_token_claims_aud == NULL) {
        ogs_error("OpenAPI_access_token_claims_aud_convertToJSON() failed [AccessTokenClaims_aud]");
        return NULL;
    }

    item = cJSON_CreateObject();
end:
    return item;
}

OpenAPI_access_token_claims_aud_t *OpenAPI_access_token_claims_aud_parseFromJSON(cJSON *access_token_claims_audJSON)
{
    OpenAPI_access_token_claims_aud_t *access_token_claims_aud_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    access_token_claims_aud_local_var = OpenAPI_access_token_claims_aud_create (
    );

    return access_token_claims_aud_local_var;
end:
    return NULL;
}

OpenAPI_access_token_claims_aud_t *OpenAPI_access_token_claims_aud_copy(OpenAPI_access_token_claims_aud_t *dst, OpenAPI_access_token_claims_aud_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_access_token_claims_aud_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_access_token_claims_aud_convertToJSON() failed");
        return NULL;
    }

    content = cJSON_Print(item);
    cJSON_Delete(item);

    if (!content) {
        ogs_error("cJSON_Print() failed");
        return NULL;
    }

    item = cJSON_Parse(content);
    ogs_free(content);
    if (!item) {
        ogs_error("cJSON_Parse() failed");
        return NULL;
    }

    OpenAPI_access_token_claims_aud_free(dst);
    dst = OpenAPI_access_token_claims_aud_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

