#include "oauth-handler.h"

int ogs_num_delimeter(const char* s, const char deli) {
    if (s == NULL)
        return 0;
    if (strlen(s) <= 0) {
        return 0;
    }
    int n = 1;
    int i = 0;
    for (i = 0; s[i]; i++){
        if (s[i] == deli)
            n++;
    }
    return n;
}

char** ogs_split_str(const char* s, const char deli, int* num) {
    char** splited_str = NULL;

    if (s == NULL)
        return splited_str;
    
    *num = ogs_num_delimeter(s, deli);
    if (*num > 0) {
        splited_str = ogs_malloc(*num * sizeof(char*));
    }

    int j = 0;
    int last = 0;
    int len = strlen(s);
    int i = 0;
    for (i = 0; s[i]; i++){
        if (s[i] == deli) {
            int diff = i - last;
            char* temp = ogs_malloc(diff + 1);
            memset(temp, 0, diff + 1);
            memcpy(temp, &s[last], diff);
            splited_str[j++] = temp;
            last = i + 1;
        }
    }
    if (last < len) {
        int diff = len - last;
        char* temp = ogs_malloc(diff + 1);
        memset(temp, 0, diff + 1);
        memcpy(temp, &s[last], diff + 1);
        splited_str[j++] = temp;
    }

    return splited_str;
}

int oauth_handler(ogs_sbi_message_t message, ogs_sbi_stream_t *stream){
    ogs_sbi_response_t *response = NULL;
    ogs_sbi_message_t sendmsg;
    OpenAPI_access_token_rsp_t token_response;
    char* current_scope = NULL;
    ogs_sbi_nf_service_t* current_service = NULL;
    int n = 0;

    memset(&sendmsg, 0, sizeof(sendmsg));
    memset(&token_response, 0, sizeof(token_response));


    if (message.AccessTokenRequest->grant_type != OpenAPI_access_token_req_GRANTTYPE_client_credentials) {
        ogs_error("Unkown Grant-Type [%s]", OpenAPI_access_token_req_grant_type_ToString(message.AccessTokenRequest->grant_type));

        OpenAPI_access_token_err_t access_token_error;
        memset(&access_token_error, 0, sizeof(access_token_error));
        access_token_error.error = OpenAPI_access_token_err_ERROR_unsupported_grant_type;

        sendmsg.AccessTokenError = &access_token_error;

        response = ogs_sbi_build_response(&sendmsg, OGS_SBI_HTTP_STATUS_BAD_REQUEST);
        ogs_assert(response);
        ogs_assert(true == ogs_sbi_server_send_response(stream, response));
        return false;
    }

    ogs_sbi_nf_instance_t* sub_nf_instance = ogs_sbi_nf_instance_find(message.AccessTokenRequest->nf_instance_id);
    if (sub_nf_instance == NULL) {
        ogs_error("no subject NF Instance found [%s]", message.AccessTokenRequest->nf_instance_id);
        ogs_assert(true ==
            ogs_sbi_server_send_error(stream,
                OGS_SBI_HTTP_STATUS_FORBIDDEN, &message,
                "access denied", "",
                NULL));
        return false;
    }

    ogs_sbi_nf_instance_t* target_nf_instance = ogs_sbi_nf_instance_find(message.AccessTokenRequest->target_nf_instance_id);
    if (target_nf_instance == NULL) {
        ogs_error("no target NF Instance found [%s]", message.AccessTokenRequest->nf_instance_id);
        ogs_assert(true ==
            ogs_sbi_server_send_error(stream,
                OGS_SBI_HTTP_STATUS_NOT_FOUND, &message,
                "no such target NF found", "",
                NULL));
        return false;
    }

    char** splitted_scopes = ogs_split_str(message.AccessTokenRequest->scope, ' ', &n);
    if (splitted_scopes) {
        int i;
        for(i = 0; i < n; i++) {
            current_scope = splitted_scopes[i];
            if (current_scope) {
                current_service = ogs_sbi_nf_service_find_by_name(target_nf_instance, current_scope);
                if (current_service) {
                    bool permitted = false;
                    int j = 0;
                    for (j = 0; j < current_service->num_of_allowed_nf_type; j++) {
                        if (current_service->allowed_nf_type[j] == sub_nf_instance->nf_type) {
                            permitted = true;
                            break;
                        }
                    }
                    if (!permitted) {
                        ogs_error("NF [%s] of type [%s], requested access to service [%s] for NF type of [%s] which is not permitted", 
                            sub_nf_instance->id, OpenAPI_nf_type_ToString(sub_nf_instance->nf_type),
                            current_scope, OpenAPI_nf_type_ToString(target_nf_instance->nf_type)
                        );
                        OpenAPI_access_token_err_t access_token_error;
                        memset(&access_token_error, 0, sizeof(access_token_error));
                        access_token_error.error = OpenAPI_access_token_err_ERROR_unauthorized_client;

                        sendmsg.AccessTokenError = &access_token_error;

                        response = ogs_sbi_build_response(&sendmsg, OGS_SBI_HTTP_STATUS_BAD_REQUEST);
                        ogs_assert(response);
                        ogs_assert(true == ogs_sbi_server_send_response(stream, response));
                        return false;
                    } else {
                        ogs_info("NF [%s] request for a token for service [%s] at NF [%s]", sub_nf_instance->id, current_service->name, target_nf_instance->id);
                    }
                } else {
                    ogs_error("NF [%s] of type [%s], does not support service [%s]", target_nf_instance->id, OpenAPI_nf_type_ToString(target_nf_instance->nf_type), current_scope);
                    OpenAPI_access_token_err_t access_token_error;
                    memset(&access_token_error, 0, sizeof(access_token_error));
                    access_token_error.error = OpenAPI_access_token_err_ERROR_invalid_scope;

                    sendmsg.AccessTokenError = &access_token_error;

                    response = ogs_sbi_build_response(&sendmsg, OGS_SBI_HTTP_STATUS_BAD_REQUEST);
                    ogs_assert(response);
                    ogs_assert(true == ogs_sbi_server_send_response(stream, response));
                    return false;
                }
            } else {
                ogs_error("invalid scope");
                OpenAPI_access_token_err_t access_token_error;
                memset(&access_token_error, 0, sizeof(access_token_error));
                access_token_error.error = OpenAPI_access_token_err_ERROR_invalid_scope;

                sendmsg.AccessTokenError = &access_token_error;

                response = ogs_sbi_build_response(&sendmsg, OGS_SBI_HTTP_STATUS_BAD_REQUEST);
                ogs_assert(response);
                ogs_assert(true == ogs_sbi_server_send_response(stream, response));
                return false;
            }
        }
    } else {
        ogs_error("either unable to parse the scope or the scope is empty");
        OpenAPI_access_token_err_t access_token_error;
        memset(&access_token_error, 0, sizeof(access_token_error));
        access_token_error.error = OpenAPI_access_token_err_ERROR_invalid_scope;

        sendmsg.AccessTokenError = &access_token_error;

        response = ogs_sbi_build_response(&sendmsg, OGS_SBI_HTTP_STATUS_BAD_REQUEST);
        ogs_assert(response);
        ogs_assert(true == ogs_sbi_server_send_response(stream, response));
        return false;
    }

    int expires_in = 0;
    char* token = get_token_from_claim(
        ogs_sbi_self()->nf_instance->id,
        sub_nf_instance->id,
        target_nf_instance->id,
        message.AccessTokenRequest->scope,
        OGS_OAUTH_TOKEN_ALG, OGS_OAUTH_TOKEN_TYPE,
        &expires_in
    );

    if (token) {
        token_response.access_token = token;
        token_response.expires_in = expires_in;
        token_response.is_expires_in = true;
    } else {
        ogs_error("no token generated");
        ogs_assert(true ==
            ogs_sbi_server_send_error(stream,
                OGS_SBI_HTTP_STATUS_INTERNAL_SERVER_ERROR, &message,
                "no token generated", "",
                NULL));
        return false;
    }

    if (message.AccessTokenRequest->scope)
        token_response.scope = message.AccessTokenRequest->scope;
    token_response.token_type = OpenAPI_access_token_rsp_TOKENTYPE_Bearer;
    sendmsg.AccessTokenResponse = &token_response;
    response = ogs_sbi_build_response(&sendmsg, OGS_SBI_HTTP_STATUS_OK);
    ogs_assert(response);
    ogs_assert(true == ogs_sbi_server_send_response(stream, response));
    return true;
}