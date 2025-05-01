/*
 * access_token_claims.h
 *
 * The claims data structure for the access token
 */

#ifndef _OpenAPI_access_token_claims_H_
#define _OpenAPI_access_token_claims_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
#include "access_token_claims_aud.h"
#include "plmn_id.h"
#include "plmn_id_nid.h"
#include "snssai.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_access_token_claims_s OpenAPI_access_token_claims_t;
typedef struct OpenAPI_access_token_claims_s {
    char *iss;
    char *sub;
    struct OpenAPI_access_token_claims_aud_s *aud;
    char *scope;
    int exp;
    struct OpenAPI_plmn_id_s *consumer_plmn_id;
    struct OpenAPI_plmn_id_nid_s *consumer_snpn_id;
    struct OpenAPI_plmn_id_s *producer_plmn_id;
    struct OpenAPI_plmn_id_nid_s *producer_snpn_id;
    OpenAPI_list_t *producer_snssai_list;
    OpenAPI_list_t *producer_nsi_list;
    char *producer_nf_set_id;
    char *producer_nf_service_set_id;
    char *source_nf_instance_id;
} OpenAPI_access_token_claims_t;

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
);
void OpenAPI_access_token_claims_free(OpenAPI_access_token_claims_t *access_token_claims);
OpenAPI_access_token_claims_t *OpenAPI_access_token_claims_parseFromJSON(cJSON *access_token_claimsJSON);
cJSON *OpenAPI_access_token_claims_convertToJSON(OpenAPI_access_token_claims_t *access_token_claims);
OpenAPI_access_token_claims_t *OpenAPI_access_token_claims_copy(OpenAPI_access_token_claims_t *dst, OpenAPI_access_token_claims_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_access_token_claims_H_ */

