
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "access_token_claims.h"

OpenAPI_access_token_claims_t *OpenAPI_access_token_claims_create(
    char *iss,
    char *sub,
    OpenAPI_access_token_claims_aud_t *aud,
    char *scope,
    int exp,
    OpenAPI_plmn_id_t *consumer_plmn_id,
    OpenAPI_plmn_id_nid_t *consumer_snpn_id,
    OpenAPI_plmn_id_t *producer_plmn_id,
    OpenAPI_plmn_id_nid_t *producer_snpn_id,
    OpenAPI_list_t *producer_snssai_list,
    OpenAPI_list_t *producer_nsi_list,
    char *producer_nf_set_id,
    char *producer_nf_service_set_id,
    char *source_nf_instance_id
)
{
    OpenAPI_access_token_claims_t *access_token_claims_local_var = ogs_malloc(sizeof(OpenAPI_access_token_claims_t));
    ogs_assert(access_token_claims_local_var);

    access_token_claims_local_var->iss = iss;
    access_token_claims_local_var->sub = sub;
    access_token_claims_local_var->aud = aud;
    access_token_claims_local_var->scope = scope;
    access_token_claims_local_var->exp = exp;
    access_token_claims_local_var->consumer_plmn_id = consumer_plmn_id;
    access_token_claims_local_var->consumer_snpn_id = consumer_snpn_id;
    access_token_claims_local_var->producer_plmn_id = producer_plmn_id;
    access_token_claims_local_var->producer_snpn_id = producer_snpn_id;
    access_token_claims_local_var->producer_snssai_list = producer_snssai_list;
    access_token_claims_local_var->producer_nsi_list = producer_nsi_list;
    access_token_claims_local_var->producer_nf_set_id = producer_nf_set_id;
    access_token_claims_local_var->producer_nf_service_set_id = producer_nf_service_set_id;
    access_token_claims_local_var->source_nf_instance_id = source_nf_instance_id;

    return access_token_claims_local_var;
}

void OpenAPI_access_token_claims_free(OpenAPI_access_token_claims_t *access_token_claims)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == access_token_claims) {
        return;
    }
    if (access_token_claims->iss) {
        ogs_free(access_token_claims->iss);
        access_token_claims->iss = NULL;
    }
    if (access_token_claims->sub) {
        ogs_free(access_token_claims->sub);
        access_token_claims->sub = NULL;
    }
    if (access_token_claims->aud) {
        OpenAPI_access_token_claims_aud_free(access_token_claims->aud);
        access_token_claims->aud = NULL;
    }
    if (access_token_claims->scope) {
        ogs_free(access_token_claims->scope);
        access_token_claims->scope = NULL;
    }
    if (access_token_claims->consumer_plmn_id) {
        OpenAPI_plmn_id_free(access_token_claims->consumer_plmn_id);
        access_token_claims->consumer_plmn_id = NULL;
    }
    if (access_token_claims->consumer_snpn_id) {
        OpenAPI_plmn_id_nid_free(access_token_claims->consumer_snpn_id);
        access_token_claims->consumer_snpn_id = NULL;
    }
    if (access_token_claims->producer_plmn_id) {
        OpenAPI_plmn_id_free(access_token_claims->producer_plmn_id);
        access_token_claims->producer_plmn_id = NULL;
    }
    if (access_token_claims->producer_snpn_id) {
        OpenAPI_plmn_id_nid_free(access_token_claims->producer_snpn_id);
        access_token_claims->producer_snpn_id = NULL;
    }
    if (access_token_claims->producer_snssai_list) {
        OpenAPI_list_for_each(access_token_claims->producer_snssai_list, node) {
            OpenAPI_snssai_free(node->data);
        }
        OpenAPI_list_free(access_token_claims->producer_snssai_list);
        access_token_claims->producer_snssai_list = NULL;
    }
    if (access_token_claims->producer_nsi_list) {
        OpenAPI_list_for_each(access_token_claims->producer_nsi_list, node) {
            ogs_free(node->data);
        }
        OpenAPI_list_free(access_token_claims->producer_nsi_list);
        access_token_claims->producer_nsi_list = NULL;
    }
    if (access_token_claims->producer_nf_set_id) {
        ogs_free(access_token_claims->producer_nf_set_id);
        access_token_claims->producer_nf_set_id = NULL;
    }
    if (access_token_claims->producer_nf_service_set_id) {
        ogs_free(access_token_claims->producer_nf_service_set_id);
        access_token_claims->producer_nf_service_set_id = NULL;
    }
    if (access_token_claims->source_nf_instance_id) {
        ogs_free(access_token_claims->source_nf_instance_id);
        access_token_claims->source_nf_instance_id = NULL;
    }
    ogs_free(access_token_claims);
}

cJSON *OpenAPI_access_token_claims_convertToJSON(OpenAPI_access_token_claims_t *access_token_claims)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (access_token_claims == NULL) {
        ogs_error("OpenAPI_access_token_claims_convertToJSON() failed [AccessTokenClaims]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (!access_token_claims->iss) {
        ogs_error("OpenAPI_access_token_claims_convertToJSON() failed [iss]");
        return NULL;
    }
    if (cJSON_AddStringToObject(item, "iss", access_token_claims->iss) == NULL) {
        ogs_error("OpenAPI_access_token_claims_convertToJSON() failed [iss]");
        goto end;
    }

    if (!access_token_claims->sub) {
        ogs_error("OpenAPI_access_token_claims_convertToJSON() failed [sub]");
        return NULL;
    }
    if (cJSON_AddStringToObject(item, "sub", access_token_claims->sub) == NULL) {
        ogs_error("OpenAPI_access_token_claims_convertToJSON() failed [sub]");
        goto end;
    }

    if (!access_token_claims->aud) {
        ogs_error("OpenAPI_access_token_claims_convertToJSON() failed [aud]");
        return NULL;
    }
    cJSON *aud_local_JSON = OpenAPI_access_token_claims_aud_convertToJSON(access_token_claims->aud);
    if (aud_local_JSON == NULL) {
        ogs_error("OpenAPI_access_token_claims_convertToJSON() failed [aud]");
        goto end;
    }
    cJSON_AddItemToObject(item, "aud", aud_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_access_token_claims_convertToJSON() failed [aud]");
        goto end;
    }

    if (!access_token_claims->scope) {
        ogs_error("OpenAPI_access_token_claims_convertToJSON() failed [scope]");
        return NULL;
    }
    if (cJSON_AddStringToObject(item, "scope", access_token_claims->scope) == NULL) {
        ogs_error("OpenAPI_access_token_claims_convertToJSON() failed [scope]");
        goto end;
    }

    if (cJSON_AddNumberToObject(item, "exp", access_token_claims->exp) == NULL) {
        ogs_error("OpenAPI_access_token_claims_convertToJSON() failed [exp]");
        goto end;
    }

    if (access_token_claims->consumer_plmn_id) {
    cJSON *consumer_plmn_id_local_JSON = OpenAPI_plmn_id_convertToJSON(access_token_claims->consumer_plmn_id);
    if (consumer_plmn_id_local_JSON == NULL) {
        ogs_error("OpenAPI_access_token_claims_convertToJSON() failed [consumer_plmn_id]");
        goto end;
    }
    cJSON_AddItemToObject(item, "consumerPlmnId", consumer_plmn_id_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_access_token_claims_convertToJSON() failed [consumer_plmn_id]");
        goto end;
    }
    }

    if (access_token_claims->consumer_snpn_id) {
    cJSON *consumer_snpn_id_local_JSON = OpenAPI_plmn_id_nid_convertToJSON(access_token_claims->consumer_snpn_id);
    if (consumer_snpn_id_local_JSON == NULL) {
        ogs_error("OpenAPI_access_token_claims_convertToJSON() failed [consumer_snpn_id]");
        goto end;
    }
    cJSON_AddItemToObject(item, "consumerSnpnId", consumer_snpn_id_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_access_token_claims_convertToJSON() failed [consumer_snpn_id]");
        goto end;
    }
    }

    if (access_token_claims->producer_plmn_id) {
    cJSON *producer_plmn_id_local_JSON = OpenAPI_plmn_id_convertToJSON(access_token_claims->producer_plmn_id);
    if (producer_plmn_id_local_JSON == NULL) {
        ogs_error("OpenAPI_access_token_claims_convertToJSON() failed [producer_plmn_id]");
        goto end;
    }
    cJSON_AddItemToObject(item, "producerPlmnId", producer_plmn_id_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_access_token_claims_convertToJSON() failed [producer_plmn_id]");
        goto end;
    }
    }

    if (access_token_claims->producer_snpn_id) {
    cJSON *producer_snpn_id_local_JSON = OpenAPI_plmn_id_nid_convertToJSON(access_token_claims->producer_snpn_id);
    if (producer_snpn_id_local_JSON == NULL) {
        ogs_error("OpenAPI_access_token_claims_convertToJSON() failed [producer_snpn_id]");
        goto end;
    }
    cJSON_AddItemToObject(item, "producerSnpnId", producer_snpn_id_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_access_token_claims_convertToJSON() failed [producer_snpn_id]");
        goto end;
    }
    }

    if (access_token_claims->producer_snssai_list) {
    cJSON *producer_snssai_listList = cJSON_AddArrayToObject(item, "producerSnssaiList");
    if (producer_snssai_listList == NULL) {
        ogs_error("OpenAPI_access_token_claims_convertToJSON() failed [producer_snssai_list]");
        goto end;
    }
    OpenAPI_list_for_each(access_token_claims->producer_snssai_list, node) {
        cJSON *itemLocal = OpenAPI_snssai_convertToJSON(node->data);
        if (itemLocal == NULL) {
            ogs_error("OpenAPI_access_token_claims_convertToJSON() failed [producer_snssai_list]");
            goto end;
        }
        cJSON_AddItemToArray(producer_snssai_listList, itemLocal);
    }
    }

    if (access_token_claims->producer_nsi_list) {
    cJSON *producer_nsi_listList = cJSON_AddArrayToObject(item, "producerNsiList");
    if (producer_nsi_listList == NULL) {
        ogs_error("OpenAPI_access_token_claims_convertToJSON() failed [producer_nsi_list]");
        goto end;
    }
    OpenAPI_list_for_each(access_token_claims->producer_nsi_list, node) {
        if (cJSON_AddStringToObject(producer_nsi_listList, "", (char*)node->data) == NULL) {
            ogs_error("OpenAPI_access_token_claims_convertToJSON() failed [producer_nsi_list]");
            goto end;
        }
    }
    }

    if (access_token_claims->producer_nf_set_id) {
    if (cJSON_AddStringToObject(item, "producerNfSetId", access_token_claims->producer_nf_set_id) == NULL) {
        ogs_error("OpenAPI_access_token_claims_convertToJSON() failed [producer_nf_set_id]");
        goto end;
    }
    }

    if (access_token_claims->producer_nf_service_set_id) {
    if (cJSON_AddStringToObject(item, "producerNfServiceSetId", access_token_claims->producer_nf_service_set_id) == NULL) {
        ogs_error("OpenAPI_access_token_claims_convertToJSON() failed [producer_nf_service_set_id]");
        goto end;
    }
    }

    if (access_token_claims->source_nf_instance_id) {
    if (cJSON_AddStringToObject(item, "sourceNfInstanceId", access_token_claims->source_nf_instance_id) == NULL) {
        ogs_error("OpenAPI_access_token_claims_convertToJSON() failed [source_nf_instance_id]");
        goto end;
    }
    }

end:
    return item;
}

OpenAPI_access_token_claims_t *OpenAPI_access_token_claims_parseFromJSON(cJSON *access_token_claimsJSON)
{
    OpenAPI_access_token_claims_t *access_token_claims_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *iss = NULL;
    cJSON *sub = NULL;
    cJSON *aud = NULL;
    OpenAPI_access_token_claims_aud_t *aud_local_nonprim = NULL;
    cJSON *scope = NULL;
    cJSON *exp = NULL;
    cJSON *consumer_plmn_id = NULL;
    OpenAPI_plmn_id_t *consumer_plmn_id_local_nonprim = NULL;
    cJSON *consumer_snpn_id = NULL;
    OpenAPI_plmn_id_nid_t *consumer_snpn_id_local_nonprim = NULL;
    cJSON *producer_plmn_id = NULL;
    OpenAPI_plmn_id_t *producer_plmn_id_local_nonprim = NULL;
    cJSON *producer_snpn_id = NULL;
    OpenAPI_plmn_id_nid_t *producer_snpn_id_local_nonprim = NULL;
    cJSON *producer_snssai_list = NULL;
    OpenAPI_list_t *producer_snssai_listList = NULL;
    cJSON *producer_nsi_list = NULL;
    OpenAPI_list_t *producer_nsi_listList = NULL;
    cJSON *producer_nf_set_id = NULL;
    cJSON *producer_nf_service_set_id = NULL;
    cJSON *source_nf_instance_id = NULL;
    iss = cJSON_GetObjectItemCaseSensitive(access_token_claimsJSON, "iss");
    if (!iss) {
        ogs_error("OpenAPI_access_token_claims_parseFromJSON() failed [iss]");
        goto end;
    }
    if (!cJSON_IsString(iss)) {
        ogs_error("OpenAPI_access_token_claims_parseFromJSON() failed [iss]");
        goto end;
    }

    sub = cJSON_GetObjectItemCaseSensitive(access_token_claimsJSON, "sub");
    if (!sub) {
        ogs_error("OpenAPI_access_token_claims_parseFromJSON() failed [sub]");
        goto end;
    }
    if (!cJSON_IsString(sub)) {
        ogs_error("OpenAPI_access_token_claims_parseFromJSON() failed [sub]");
        goto end;
    }

    aud = cJSON_GetObjectItemCaseSensitive(access_token_claimsJSON, "aud");
    if (!aud) {
        ogs_error("OpenAPI_access_token_claims_parseFromJSON() failed [aud]");
        goto end;
    }
    aud_local_nonprim = OpenAPI_access_token_claims_aud_parseFromJSON(aud);
    if (!aud_local_nonprim) {
        ogs_error("OpenAPI_access_token_claims_aud_parseFromJSON failed [aud]");
        goto end;
    }

    scope = cJSON_GetObjectItemCaseSensitive(access_token_claimsJSON, "scope");
    if (!scope) {
        ogs_error("OpenAPI_access_token_claims_parseFromJSON() failed [scope]");
        goto end;
    }
    if (!cJSON_IsString(scope)) {
        ogs_error("OpenAPI_access_token_claims_parseFromJSON() failed [scope]");
        goto end;
    }

    exp = cJSON_GetObjectItemCaseSensitive(access_token_claimsJSON, "exp");
    if (!exp) {
        ogs_error("OpenAPI_access_token_claims_parseFromJSON() failed [exp]");
        goto end;
    }
    if (!cJSON_IsNumber(exp)) {
        ogs_error("OpenAPI_access_token_claims_parseFromJSON() failed [exp]");
        goto end;
    }

    consumer_plmn_id = cJSON_GetObjectItemCaseSensitive(access_token_claimsJSON, "consumerPlmnId");
    if (consumer_plmn_id) {
    consumer_plmn_id_local_nonprim = OpenAPI_plmn_id_parseFromJSON(consumer_plmn_id);
    if (!consumer_plmn_id_local_nonprim) {
        ogs_error("OpenAPI_plmn_id_parseFromJSON failed [consumer_plmn_id]");
        goto end;
    }
    }

    consumer_snpn_id = cJSON_GetObjectItemCaseSensitive(access_token_claimsJSON, "consumerSnpnId");
    if (consumer_snpn_id) {
    consumer_snpn_id_local_nonprim = OpenAPI_plmn_id_nid_parseFromJSON(consumer_snpn_id);
    if (!consumer_snpn_id_local_nonprim) {
        ogs_error("OpenAPI_plmn_id_nid_parseFromJSON failed [consumer_snpn_id]");
        goto end;
    }
    }

    producer_plmn_id = cJSON_GetObjectItemCaseSensitive(access_token_claimsJSON, "producerPlmnId");
    if (producer_plmn_id) {
    producer_plmn_id_local_nonprim = OpenAPI_plmn_id_parseFromJSON(producer_plmn_id);
    if (!producer_plmn_id_local_nonprim) {
        ogs_error("OpenAPI_plmn_id_parseFromJSON failed [producer_plmn_id]");
        goto end;
    }
    }

    producer_snpn_id = cJSON_GetObjectItemCaseSensitive(access_token_claimsJSON, "producerSnpnId");
    if (producer_snpn_id) {
    producer_snpn_id_local_nonprim = OpenAPI_plmn_id_nid_parseFromJSON(producer_snpn_id);
    if (!producer_snpn_id_local_nonprim) {
        ogs_error("OpenAPI_plmn_id_nid_parseFromJSON failed [producer_snpn_id]");
        goto end;
    }
    }

    producer_snssai_list = cJSON_GetObjectItemCaseSensitive(access_token_claimsJSON, "producerSnssaiList");
    if (producer_snssai_list) {
        cJSON *producer_snssai_list_local = NULL;
        if (!cJSON_IsArray(producer_snssai_list)) {
            ogs_error("OpenAPI_access_token_claims_parseFromJSON() failed [producer_snssai_list]");
            goto end;
        }

        producer_snssai_listList = OpenAPI_list_create();

        cJSON_ArrayForEach(producer_snssai_list_local, producer_snssai_list) {
            if (!cJSON_IsObject(producer_snssai_list_local)) {
                ogs_error("OpenAPI_access_token_claims_parseFromJSON() failed [producer_snssai_list]");
                goto end;
            }
            OpenAPI_snssai_t *producer_snssai_listItem = OpenAPI_snssai_parseFromJSON(producer_snssai_list_local);
            if (!producer_snssai_listItem) {
                ogs_error("No producer_snssai_listItem");
                goto end;
            }
            OpenAPI_list_add(producer_snssai_listList, producer_snssai_listItem);
        }
    }

    producer_nsi_list = cJSON_GetObjectItemCaseSensitive(access_token_claimsJSON, "producerNsiList");
    if (producer_nsi_list) {
        cJSON *producer_nsi_list_local = NULL;
        if (!cJSON_IsArray(producer_nsi_list)) {
            ogs_error("OpenAPI_access_token_claims_parseFromJSON() failed [producer_nsi_list]");
            goto end;
        }

        producer_nsi_listList = OpenAPI_list_create();

        cJSON_ArrayForEach(producer_nsi_list_local, producer_nsi_list) {
            double *localDouble = NULL;
            int *localInt = NULL;
            if (!cJSON_IsString(producer_nsi_list_local)) {
                ogs_error("OpenAPI_access_token_claims_parseFromJSON() failed [producer_nsi_list]");
                goto end;
            }
            OpenAPI_list_add(producer_nsi_listList, ogs_strdup(producer_nsi_list_local->valuestring));
        }
    }

    producer_nf_set_id = cJSON_GetObjectItemCaseSensitive(access_token_claimsJSON, "producerNfSetId");
    if (producer_nf_set_id) {
    if (!cJSON_IsString(producer_nf_set_id) && !cJSON_IsNull(producer_nf_set_id)) {
        ogs_error("OpenAPI_access_token_claims_parseFromJSON() failed [producer_nf_set_id]");
        goto end;
    }
    }

    producer_nf_service_set_id = cJSON_GetObjectItemCaseSensitive(access_token_claimsJSON, "producerNfServiceSetId");
    if (producer_nf_service_set_id) {
    if (!cJSON_IsString(producer_nf_service_set_id) && !cJSON_IsNull(producer_nf_service_set_id)) {
        ogs_error("OpenAPI_access_token_claims_parseFromJSON() failed [producer_nf_service_set_id]");
        goto end;
    }
    }

    source_nf_instance_id = cJSON_GetObjectItemCaseSensitive(access_token_claimsJSON, "sourceNfInstanceId");
    if (source_nf_instance_id) {
    if (!cJSON_IsString(source_nf_instance_id) && !cJSON_IsNull(source_nf_instance_id)) {
        ogs_error("OpenAPI_access_token_claims_parseFromJSON() failed [source_nf_instance_id]");
        goto end;
    }
    }

    access_token_claims_local_var = OpenAPI_access_token_claims_create (
        ogs_strdup(iss->valuestring),
        ogs_strdup(sub->valuestring),
        aud_local_nonprim,
        ogs_strdup(scope->valuestring),
        
        exp->valuedouble,
        consumer_plmn_id ? consumer_plmn_id_local_nonprim : NULL,
        consumer_snpn_id ? consumer_snpn_id_local_nonprim : NULL,
        producer_plmn_id ? producer_plmn_id_local_nonprim : NULL,
        producer_snpn_id ? producer_snpn_id_local_nonprim : NULL,
        producer_snssai_list ? producer_snssai_listList : NULL,
        producer_nsi_list ? producer_nsi_listList : NULL,
        producer_nf_set_id && !cJSON_IsNull(producer_nf_set_id) ? ogs_strdup(producer_nf_set_id->valuestring) : NULL,
        producer_nf_service_set_id && !cJSON_IsNull(producer_nf_service_set_id) ? ogs_strdup(producer_nf_service_set_id->valuestring) : NULL,
        source_nf_instance_id && !cJSON_IsNull(source_nf_instance_id) ? ogs_strdup(source_nf_instance_id->valuestring) : NULL
    );

    return access_token_claims_local_var;
end:
    if (aud_local_nonprim) {
        OpenAPI_access_token_claims_aud_free(aud_local_nonprim);
        aud_local_nonprim = NULL;
    }
    if (consumer_plmn_id_local_nonprim) {
        OpenAPI_plmn_id_free(consumer_plmn_id_local_nonprim);
        consumer_plmn_id_local_nonprim = NULL;
    }
    if (consumer_snpn_id_local_nonprim) {
        OpenAPI_plmn_id_nid_free(consumer_snpn_id_local_nonprim);
        consumer_snpn_id_local_nonprim = NULL;
    }
    if (producer_plmn_id_local_nonprim) {
        OpenAPI_plmn_id_free(producer_plmn_id_local_nonprim);
        producer_plmn_id_local_nonprim = NULL;
    }
    if (producer_snpn_id_local_nonprim) {
        OpenAPI_plmn_id_nid_free(producer_snpn_id_local_nonprim);
        producer_snpn_id_local_nonprim = NULL;
    }
    if (producer_snssai_listList) {
        OpenAPI_list_for_each(producer_snssai_listList, node) {
            OpenAPI_snssai_free(node->data);
        }
        OpenAPI_list_free(producer_snssai_listList);
        producer_snssai_listList = NULL;
    }
    if (producer_nsi_listList) {
        OpenAPI_list_for_each(producer_nsi_listList, node) {
            ogs_free(node->data);
        }
        OpenAPI_list_free(producer_nsi_listList);
        producer_nsi_listList = NULL;
    }
    return NULL;
}

OpenAPI_access_token_claims_t *OpenAPI_access_token_claims_copy(OpenAPI_access_token_claims_t *dst, OpenAPI_access_token_claims_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_access_token_claims_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_access_token_claims_convertToJSON() failed");
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

    OpenAPI_access_token_claims_free(dst);
    dst = OpenAPI_access_token_claims_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

