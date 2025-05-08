
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "access_token_rsp.h"

char *OpenAPI_access_token_rsp_token_type_ToString(OpenAPI_access_token_rsp_token_type_e token_type)
{
    const char *token_typeArray[] =  { "NULL", "Bearer" };
    size_t sizeofArray = sizeof(token_typeArray) / sizeof(token_typeArray[0]);
    if (token_type < sizeofArray)
        return (char *)token_typeArray[token_type];
    else
        return (char *)"Unknown";
}

OpenAPI_access_token_rsp_token_type_e OpenAPI_access_token_rsp_token_type_FromString(char* token_type)
{
    int stringToReturn = 0;
    const char *token_typeArray[] =  { "NULL", "Bearer" };
    size_t sizeofArray = sizeof(token_typeArray) / sizeof(token_typeArray[0]);
    while (stringToReturn < sizeofArray) {
        if (strcmp(token_type, token_typeArray[stringToReturn]) == 0) {
            return stringToReturn;
        }
        stringToReturn++;
    }
    return 0;
}
OpenAPI_access_token_rsp_t *OpenAPI_access_token_rsp_create(
    char *access_token,
    OpenAPI_access_token_rsp_token_type_e token_type,
    bool is_expires_in,
    int expires_in,
    char *scope
)
{
    OpenAPI_access_token_rsp_t *access_token_rsp_local_var = ogs_malloc(sizeof(OpenAPI_access_token_rsp_t));
    ogs_assert(access_token_rsp_local_var);

    access_token_rsp_local_var->access_token = access_token;
    access_token_rsp_local_var->token_type = token_type;
    access_token_rsp_local_var->is_expires_in = is_expires_in;
    access_token_rsp_local_var->expires_in = expires_in;
    access_token_rsp_local_var->scope = scope;

    return access_token_rsp_local_var;
}

void OpenAPI_access_token_rsp_free(OpenAPI_access_token_rsp_t *access_token_rsp)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == access_token_rsp) {
        return;
    }
    if (access_token_rsp->access_token) {
        ogs_free(access_token_rsp->access_token);
        access_token_rsp->access_token = NULL;
    }
    if (access_token_rsp->scope) {
        ogs_free(access_token_rsp->scope);
        access_token_rsp->scope = NULL;
    }
    ogs_free(access_token_rsp);
}

cJSON *OpenAPI_access_token_rsp_convertToJSON(OpenAPI_access_token_rsp_t *access_token_rsp)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (access_token_rsp == NULL) {
        ogs_error("OpenAPI_access_token_rsp_convertToJSON() failed [AccessTokenRsp]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (!access_token_rsp->access_token) {
        ogs_error("OpenAPI_access_token_rsp_convertToJSON() failed [access_token]");
        return NULL;
    }
    if (cJSON_AddStringToObject(item, "access_token", access_token_rsp->access_token) == NULL) {
        ogs_error("OpenAPI_access_token_rsp_convertToJSON() failed [access_token]");
        goto end;
    }

    if (access_token_rsp->token_type == OpenAPI_access_token_rsp_TOKENTYPE_NULL) {
        ogs_error("OpenAPI_access_token_rsp_convertToJSON() failed [token_type]");
        return NULL;
    }
    if (cJSON_AddStringToObject(item, "token_type", OpenAPI_access_token_rsp_token_type_ToString(access_token_rsp->token_type)) == NULL) {
        ogs_error("OpenAPI_access_token_rsp_convertToJSON() failed [token_type]");
        goto end;
    }

    if (access_token_rsp->is_expires_in) {
    if (cJSON_AddNumberToObject(item, "expires_in", access_token_rsp->expires_in) == NULL) {
        ogs_error("OpenAPI_access_token_rsp_convertToJSON() failed [expires_in]");
        goto end;
    }
    }

    if (access_token_rsp->scope) {
    if (cJSON_AddStringToObject(item, "scope", access_token_rsp->scope) == NULL) {
        ogs_error("OpenAPI_access_token_rsp_convertToJSON() failed [scope]");
        goto end;
    }
    }

end:
    return item;
}

OpenAPI_access_token_rsp_t *OpenAPI_access_token_rsp_parseFromJSON(cJSON *access_token_rspJSON)
{
    OpenAPI_access_token_rsp_t *access_token_rsp_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *access_token = NULL;
    cJSON *token_type = NULL;
    OpenAPI_access_token_rsp_token_type_e token_typeVariable = 0;
    cJSON *expires_in = NULL;
    cJSON *scope = NULL;
    access_token = cJSON_GetObjectItemCaseSensitive(access_token_rspJSON, "access_token");
    if (!access_token) {
        ogs_error("OpenAPI_access_token_rsp_parseFromJSON() failed [access_token]");
        goto end;
    }
    if (!cJSON_IsString(access_token)) {
        ogs_error("OpenAPI_access_token_rsp_parseFromJSON() failed [access_token]");
        goto end;
    }

    token_type = cJSON_GetObjectItemCaseSensitive(access_token_rspJSON, "token_type");
    if (!token_type) {
        ogs_error("OpenAPI_access_token_rsp_parseFromJSON() failed [token_type]");
        goto end;
    }
    if (!cJSON_IsString(token_type)) {
        ogs_error("OpenAPI_access_token_rsp_parseFromJSON() failed [token_type]");
        goto end;
    }
    token_typeVariable = OpenAPI_access_token_rsp_token_type_FromString(token_type->valuestring);

    expires_in = cJSON_GetObjectItemCaseSensitive(access_token_rspJSON, "expires_in");
    if (expires_in) {
    if (!cJSON_IsNumber(expires_in)) {
        ogs_error("OpenAPI_access_token_rsp_parseFromJSON() failed [expires_in]");
        goto end;
    }
    }

    scope = cJSON_GetObjectItemCaseSensitive(access_token_rspJSON, "scope");
    if (scope) {
    if (!cJSON_IsString(scope) && !cJSON_IsNull(scope)) {
        ogs_error("OpenAPI_access_token_rsp_parseFromJSON() failed [scope]");
        goto end;
    }
    }

    access_token_rsp_local_var = OpenAPI_access_token_rsp_create (
        ogs_strdup(access_token->valuestring),
        token_typeVariable,
        expires_in ? true : false,
        expires_in ? expires_in->valuedouble : 0,
        scope && !cJSON_IsNull(scope) ? ogs_strdup(scope->valuestring) : NULL
    );

    return access_token_rsp_local_var;
end:
    return NULL;
}

OpenAPI_access_token_rsp_t *OpenAPI_access_token_rsp_copy(OpenAPI_access_token_rsp_t *dst, OpenAPI_access_token_rsp_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_access_token_rsp_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_access_token_rsp_convertToJSON() failed");
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

    OpenAPI_access_token_rsp_free(dst);
    dst = OpenAPI_access_token_rsp_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

