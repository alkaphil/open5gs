#include "oauth-token.h"

int64_t get_expiration_time(int duration_seconds) {
    time_t now = time(NULL);  // Get current Unix timestamp (seconds)
    return now + duration_seconds;  // Add expiration duration
}

void jwt_free_str(char* p) {
    if (p)
        free(p);
}

char *load_private_key_from_file(const char *filename, size_t *len) {
    FILE *fp = fopen(filename, "rb");
    char *buffer = NULL;
    long file_size;

    if (!fp) {
        ogs_error("Error: Cannot open file \"%s\".", filename);
        return NULL;
    }
    fseek(fp, 0, SEEK_END);
    file_size = ftell(fp);
    rewind(fp);

    buffer = (char *)malloc(file_size + 1);
    if (!buffer) {
        ogs_error("Error: Memory allocation failed for file \"%s\".", filename);
        fclose(fp);
        return NULL;
    }
    if (fread(buffer, 1, file_size, fp) != (size_t)file_size) {
        ogs_error("Error: Failed to read file \"%s\".", filename);
        fclose(fp);
        free(buffer);
        return NULL;
    }
    buffer[file_size] = '\0';
    if (len)
        *len = file_size;
    fclose(fp);
    return buffer;
}

char* get_token_from_claim(const char* iss, const char* sub, const char* aud, const char* scope, jwt_alg_t algorithm, int* expires_in){
    int64_t curDateTime = get_expiration_time(3600);
    jwt_t *jwt = NULL;
    char *token = NULL;
    size_t priv_key_len = 0;
    char *priv_key = NULL;

    if (ogs_sbi_self()->tls.server.private_key == NULL) {
        ogs_error("no nrf.tls.server.private_key has been set in the configs.");
        return NULL;
    }

    // Load an RSA private key from a PEM file.
    priv_key = load_private_key_from_file(ogs_sbi_self()->tls.server.private_key,&priv_key_len);
    if (!priv_key) {
        ogs_error("can not open private key from: %s",ogs_sbi_self()->tls.server.private_key);
        return NULL;
    }
    // Create a new JWT object for signing
    if (jwt_new(&jwt) != 0) {
        ogs_error("Error: Failed to create JWT object.");
        free(priv_key);
        return NULL;
    }

    // Set the algorithm to RS256 and pass the private key for signing.
    if (jwt_set_alg(jwt, algorithm, priv_key, priv_key_len) != 0) {
        ogs_error("Error: Failed to set JWT algorithm with the provided private key.");
        jwt_free(jwt);
        free(priv_key);
        return NULL;
    }

    // Add example claims to the token
    jwt_add_grant(jwt, "iss", iss);             // Issuer claim
    jwt_add_grant(jwt, "sub", sub);              // Subject claim
    jwt_add_grant(jwt, "aud", aud);              // Audience claim
    jwt_add_grant(jwt, "scope", scope);              // Scope claim
    jwt_add_grant_int(jwt, "exp", curDateTime);                 // Expiration time (example Unix timestamp)

    // Encode (sign) the JWT token and retrieve it as a string
    token = jwt_encode_str(jwt);
    if (!token) {
        ogs_error("Error: Failed to encode JWT token.");
        jwt_free(jwt);
        free(priv_key);
        return NULL;
    }

    *expires_in = curDateTime;
    
    char* ogs_token = ogs_malloc(strlen(token) + 1);
    memset(ogs_token, 0, strlen(token) + 1);
    memcpy(ogs_token, token, strlen(token));


    // Clean up all allocated memory and JWT objects
    free(priv_key);
    jwt_free_str(token);
    jwt_free(jwt);

    return ogs_token;
}
