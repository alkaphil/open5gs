#include "oauth-token.h"

char* get_token_from_claim(const char* iss, const char* sub, const char* aud, const char* scope, const char* algorithm, const char* type, int* expires_in){
    HCkPrivateKey privKey;
    BOOL success;
    HCkJwt jwt;
    HCkJsonObject jose;
    HCkJsonObject claims;
    int curDateTime;
    const char *token;

    // Demonstrates how to create a JWT using an RSA private key.

    // This example requires the Chilkat API to have been previously unlocked.
    // See Global Unlock Sample for sample code.

    privKey = CkPrivateKey_Create();

    if (ogs_sbi_self()->tls.server.private_key == NULL) {
        ogs_error("no nrf.tls.server.private_key has been set in the configs.");
        return NULL;
    }

    // Load an RSA private key from a PEM file.
    success = CkPrivateKey_LoadEncryptedPemFile(privKey,ogs_sbi_self()->tls.server.private_key,"");
    if (success != TRUE) {
        ogs_error("%s",CkPrivateKey_lastErrorText(privKey));
        CkPrivateKey_Dispose(privKey);
        return NULL;
    }

    jwt = CkJwt_Create();

    // Build the JOSE header
    jose = CkJsonObject_Create();
    // Use RS256.  Pass the string "RS384" or "RS512" to use RSA with SHA-384 or SHA-512.
    ogs_assert(CkJsonObject_AppendString(jose,"alg",algorithm) == TRUE);
    ogs_assert(CkJsonObject_AppendString(jose,"typ",type) == TRUE);

    // Now build the JWT claims (also known as the payload)
    claims = CkJsonObject_Create();
    ogs_assert(CkJsonObject_AppendString(claims,"iss",iss) == TRUE);
    ogs_assert(CkJsonObject_AppendString(claims,"sub",sub) == TRUE);
    ogs_assert(CkJsonObject_AppendString(claims,"aud",aud) == TRUE);
    ogs_assert(CkJsonObject_AppendString(claims,"scope",scope) == TRUE);

    // Set the timestamp of when the JWT was created to now.
    curDateTime = CkJwt_GenNumericDate(jwt,0);
    ogs_assert(CkJsonObject_AddIntAt(claims,-1,"iat",curDateTime) == TRUE);

    // Set the "not process before" timestamp to now.
    ogs_assert(CkJsonObject_AddIntAt(claims,-1,"nbf",curDateTime) == TRUE);

    // Set the timestamp defining an expiration time (end time) for the token
    // to be now + 1 hour (3600 seconds)
    *expires_in = curDateTime + 3600;
    ogs_assert(CkJsonObject_AddIntAt(claims,-1,"exp",curDateTime + 3600) == TRUE);

    // Produce the smallest possible JWT:
    CkJwt_putAutoCompact(jwt,TRUE);

    // Create the JWT token.  This is where the RSA signature is created.
    token = CkJwt_createJwtPk(jwt,CkJsonObject_emit(jose),CkJsonObject_emit(claims),privKey);
    
    char* ogs_token = ogs_calloc(1, strlen(token));
    memcpy(ogs_token, token, strlen(token));

    CkPrivateKey_Dispose(privKey);
    CkJwt_Dispose(jwt);
    CkJsonObject_Dispose(jose);
    CkJsonObject_Dispose(claims);
    return ogs_token;
}