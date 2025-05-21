#include "oauth-handler.h"


int oauth_handler(ogs_sbi_message_t message, ogs_sbi_stream_t *stream){
    // char*  error_uri= "https://www.etsi.org/deliver/etsi_ts/129500_129599/129510/18.06.00_60/ts_129510v180600p.pdf";
    ogs_sbi_response_t *response = NULL;
    ogs_sbi_message_t sendmsg;
    OpenAPI_access_token_rsp_t token_response;
    char* current_scope = NULL;
    int n = 0;

    memset(&sendmsg, 0, sizeof(sendmsg));
    memset(&token_response, 0, sizeof(token_response));

    if (message.AccessTokenRequest->nf_instance_id == NULL || message.AccessTokenRequest->scope == NULL){
        char* error_description = NULL;
        if (message.AccessTokenRequest->nf_instance_id == NULL){
            error_description = ogs_strdup("The request is missing a required parameter: nfInstanceId.");
        } else if (message.AccessTokenRequest->scope == NULL){
            error_description = ogs_strdup("The request is missing a required parameter: scope.");
        }
        
        ogs_error("error_description: [%s]", error_description);
        sendmsg.AccessTokenError = OpenAPI_access_token_err_create(OpenAPI_access_token_err_ERROR_invalid_request, error_description, NULL);
        response = ogs_sbi_build_response(&sendmsg, OGS_SBI_HTTP_STATUS_BAD_REQUEST);
        ogs_assert(response);
        ogs_assert(true == ogs_sbi_server_send_response(stream, response));
        return false;
    }


    if (message.AccessTokenRequest->grant_type != OpenAPI_access_token_req_GRANTTYPE_client_credentials) {
        ogs_error("Unkown Grant-Type [%s]", OpenAPI_access_token_req_grant_type_ToString(message.AccessTokenRequest->grant_type));

        // OpenAPI_access_token_err_t access_token_error;
        // memset(&access_token_error, 0, sizeof(access_token_error));
        
        // access_token_error.error = OpenAPI_access_token_err_ERROR_unsupported_grant_type;

        sendmsg.AccessTokenError = OpenAPI_access_token_err_create(OpenAPI_access_token_err_ERROR_unsupported_grant_type,(char*)"The provided grant type is not supported by this authorization server.", NULL);
        


        response = ogs_sbi_build_response(&sendmsg, OGS_SBI_HTTP_STATUS_BAD_REQUEST);
        ogs_assert(response);
        ogs_assert(true == ogs_sbi_server_send_response(stream, response));
        return false;
    }

    // According to TS29.510 Table 6.3.5.2.2-1, the `nfInstanceId` is mandatory
    ogs_sbi_nf_instance_t* sub_nf_instance = ogs_sbi_nf_instance_find(message.AccessTokenRequest->nf_instance_id);
    if (sub_nf_instance == NULL) {
        ogs_error("no subject NF Instance found [%s]", message.AccessTokenRequest->nf_instance_id);

        sendmsg.AccessTokenError = OpenAPI_access_token_err_create(OpenAPI_access_token_err_ERROR_invalid_client, NULL, NULL);

        response = ogs_sbi_build_response(&sendmsg, OGS_SBI_HTTP_STATUS_UNAUTHORIZED);
        ogs_assert(response);
        ogs_assert(true == ogs_sbi_server_send_response(stream, response));
        return false;
    }

    // According to TS29.510 Table 6.3.5.2.2-1, the `targetNfInstanceId` is NOT mandatory
    // TODO: The choices here should be made based on the list below:
    //      1) if targetNfInstanceId is present, it should be checked but the NFType should be infered and the NFType should be used
    //      2) if targetNfInstanceId is not present, we should fall-back to targetNfType
    //      3) if targetNfInstanceId and targetNfType are not present, we should infer NFType from scope
    //          3-1) infer the NFType from the first scope
    //          3-2) if the the second or other scopes are from different NFTypes, we should return error
    OpenAPI_nf_type_e target_nf_type = OpenAPI_nf_type_NULL;
    
    if (message.AccessTokenRequest->target_nf_type != OpenAPI_nf_type_NULL) {
        target_nf_type = message.AccessTokenRequest->target_nf_type;
        // if (target_nf_type == OpenAPI_nf_type_NULL){
        //     ogs_error("Not found the NF type for access token [%s]", OpenAPI_nf_type_ToString(message.AccessTokenRequest->target_nf_type));
        //     ogs_assert(true ==
        //         ogs_sbi_server_send_error(stream,
        //             OGS_SBI_HTTP_STATUS_NOT_FOUND, &message,
        //             "No such NF type found", "",
        //             NULL));
        //     return false;
        // }
    }
    
    if (message.AccessTokenRequest->target_nf_instance_id){
        ogs_sbi_nf_instance_t* target_nf_instance = ogs_sbi_nf_instance_find(message.AccessTokenRequest->target_nf_instance_id);
        if (target_nf_instance == NULL) {
            ogs_error("Not found [%s] for access token", message.AccessTokenRequest->nf_instance_id);
            ogs_assert(true ==
                ogs_sbi_server_send_error(stream,
                    OGS_SBI_HTTP_STATUS_NOT_FOUND, &message,
                    "No such target NF found", "",
                    NULL));
            return false;
        }

        if (target_nf_type == OpenAPI_nf_type_NULL){
            target_nf_type = target_nf_instance->nf_type;
        } else {
            // OpenAPI_nf_type_ToString
            if (target_nf_type != target_nf_instance->nf_type){
                ogs_error("No [%s] exists with [%s] id.", OpenAPI_nf_type_ToString(target_nf_type), target_nf_instance->id);
                ogs_assert(true ==
                    ogs_sbi_server_send_error(stream,
                        OGS_SBI_HTTP_STATUS_NOT_FOUND, &message,
                        "The target NF type dose no match with instance id requested for access token", "",
                        NULL));
                return false;                
            }
        }
        
    }
    char* scope = message.AccessTokenRequest->scope;
    char** splitted_scopes = ogs_sbi_split_str(scope, ' ', &n);
    if (splitted_scopes) {
        if (target_nf_type == OpenAPI_nf_type_NULL){
            ogs_sbi_service_type_e first_scope = ogs_sbi_service_type_from_name(splitted_scopes[0]);
            if (first_scope == OGS_SBI_SERVICE_TYPE_NULL) {
                ogs_error("Do not have this [%s] service name", ogs_sbi_service_type_to_name(first_scope));
                OpenAPI_access_token_err_t access_token_error;
                memset(&access_token_error, 0, sizeof(access_token_error));
                access_token_error.error = OpenAPI_access_token_err_ERROR_invalid_scope;

                sendmsg.AccessTokenError = &access_token_error;

                response = ogs_sbi_build_response(&sendmsg, OGS_SBI_HTTP_STATUS_BAD_REQUEST);
                ogs_assert(response);
                ogs_assert(true == ogs_sbi_server_send_response(stream, response));
                return false;
            }

            target_nf_type = ogs_sbi_service_type_to_nf_type(first_scope);
            if (target_nf_type == OpenAPI_nf_type_NULL){
                ogs_error("No NF type can be inferred from scope [%s].", splitted_scopes[0]);
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

        int i;
        for(i = 0; i < n; i++) {
            current_scope = splitted_scopes[i];
            if (current_scope) {
                ogs_sbi_service_type_e current_service = ogs_sbi_service_type_from_name(current_scope);
                if (current_service == OGS_SBI_SERVICE_TYPE_NULL) {
                    ogs_error("can not infer service type from scope [%s]", current_scope);
                    OpenAPI_access_token_err_t access_token_error;
                    memset(&access_token_error, 0, sizeof(access_token_error));
                    access_token_error.error = OpenAPI_access_token_err_ERROR_invalid_scope;

                    sendmsg.AccessTokenError = &access_token_error;

                    response = ogs_sbi_build_response(&sendmsg, OGS_SBI_HTTP_STATUS_BAD_REQUEST);
                    ogs_assert(response);
                    ogs_assert(true == ogs_sbi_server_send_response(stream, response));
                    return false;
                }

                ogs_sbi_nf_instance_t* scope_target = ogs_sbi_nf_instance_find_by_service_type(current_service, sub_nf_instance->nf_type);
                if (!scope_target) {
                    ogs_error("NF [%s] of type [%s], requested access to service [%s] for NF type of [%s] which is not permitted", 
                        sub_nf_instance->id, OpenAPI_nf_type_ToString(sub_nf_instance->nf_type),
                        current_scope, OpenAPI_nf_type_ToString(target_nf_type)
                    );
                    OpenAPI_access_token_err_t access_token_error;
                    memset(&access_token_error, 0, sizeof(access_token_error));
                    access_token_error.error = OpenAPI_access_token_err_ERROR_unauthorized_client;
            
                    sendmsg.AccessTokenError = &access_token_error;
            
                    response = ogs_sbi_build_response(&sendmsg, OGS_SBI_HTTP_STATUS_BAD_REQUEST);
                    ogs_assert(response);
                    ogs_assert(true == ogs_sbi_server_send_response(stream, response));
                    return false;
                }
                if (target_nf_type != scope_target->nf_type) {
                    ogs_error("NF of type [%s], does not support service [%s]",  OpenAPI_nf_type_ToString(target_nf_type), current_scope);
                    OpenAPI_access_token_err_t access_token_error;
                    memset(&access_token_error, 0, sizeof(access_token_error));
                    access_token_error.error = OpenAPI_access_token_err_ERROR_invalid_scope;
                
                    sendmsg.AccessTokenError = &access_token_error;
                
                    response = ogs_sbi_build_response(&sendmsg, OGS_SBI_HTTP_STATUS_BAD_REQUEST);
                    ogs_assert(response);
                    ogs_assert(true == ogs_sbi_server_send_response(stream, response));
                    return false;
                }
                // if (current_service) {
                //     bool permitted = false;
                //     int j = 0;
                //     for (j = 0; j < current_service->num_of_allowed_nf_type; j++) {
                //         if (current_service->allowed_nf_type[j] == sub_nf_instance->nf_type) {
                //             permitted = true;
                //             break;
                //         }
                //     }
                //     if (!permitted) {
                //         ogs_error("NF [%s] of type [%s], requested access to service [%s] for NF type of [%s] which is not permitted", 
                //             sub_nf_instance->id, OpenAPI_nf_type_ToString(sub_nf_instance->nf_type),
                //             current_scope, OpenAPI_nf_type_ToString(target_nf_instance->nf_type)
                //         );
                //         OpenAPI_access_token_err_t access_token_error;
                //         memset(&access_token_error, 0, sizeof(access_token_error));
                //         access_token_error.error = OpenAPI_access_token_err_ERROR_unauthorized_client;
                //
                //         sendmsg.AccessTokenError = &access_token_error;
                //
                //         response = ogs_sbi_build_response(&sendmsg, OGS_SBI_HTTP_STATUS_BAD_REQUEST);
                //         ogs_assert(response);
                //         ogs_assert(true == ogs_sbi_server_send_response(stream, response));
                //         return false;
                //     } else {
                //         ogs_info("NF [%s] request for a token for service [%s] at NF [%s]", sub_nf_instance->id, current_service->name, target_nf_instance->id);
                //     }
                // } else {
                //     ogs_error("NF [%s] of type [%s], does not support service [%s]", target_nf_instance->id, OpenAPI_nf_type_ToString(target_nf_instance->nf_type), current_scope);
                //     OpenAPI_access_token_err_t access_token_error;
                //     memset(&access_token_error, 0, sizeof(access_token_error));
                //     access_token_error.error = OpenAPI_access_token_err_ERROR_invalid_scope;
                //
                //     sendmsg.AccessTokenError = &access_token_error;
                //
                //     response = ogs_sbi_build_response(&sendmsg, OGS_SBI_HTTP_STATUS_BAD_REQUEST);
                //     ogs_assert(response);
                //     ogs_assert(true == ogs_sbi_server_send_response(stream, response));
                //     return false;
                // }
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
        for (i = 0; i < n; i++)
            if (splitted_scopes[i])
                ogs_free(splitted_scopes[i]);
        ogs_free(splitted_scopes);
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
        OpenAPI_nf_type_ToString(target_nf_type),
        message.AccessTokenRequest->scope,
        OGS_OAUTH_TOKEN_ALG, &expires_in
    );

    if (token) {
        token_response.access_token = token;
        token_response.expires_in = expires_in;
        token_response.is_expires_in = true;
        // ogs_info("the end check of toke:[%d]", check_token(token));
        // message.h.service.name;
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
    if (token)
        ogs_free(token);
    return true;
}