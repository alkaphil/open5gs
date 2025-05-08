#include "token-check.h"

char *load_public_key_from_cert_file(const char *cert_file, size_t *len) {
    FILE *fp = fopen(cert_file, "r");
    if (!fp) {
        ogs_error("Error: Cannot open certificate file \"%s\".", cert_file);
        return NULL;
    }

    // Read the certificate from the file
    X509 *cert = PEM_read_X509(fp, NULL, NULL, NULL);
    fclose(fp);

    if (!cert) {
        ogs_error("Error: Failed to read certificate.");
        return NULL;
    }

    // Extract the public key
    EVP_PKEY *pubkey = X509_get_pubkey(cert);
    if (!pubkey) {
        ogs_error("Error: Failed to extract public key.");
        X509_free(cert);
        return NULL;
    }

    // Create a BIO memory buffer to store the PEM-formatted public key
    BIO *bio = BIO_new(BIO_s_mem());
    if (!bio) {
        ogs_error("Error: Failed to create BIO buffer.");
        EVP_PKEY_free(pubkey);
        X509_free(cert);
        return NULL;
    }

    // Write the public key to the BIO buffer in PEM format
    PEM_write_bio_PUBKEY(bio, pubkey);

    // Get the length of the PEM data
    size_t key_len = BIO_pending(bio);
    char *key_str = (char *)malloc(key_len + 1);
    if (!key_str) {
        ogs_error("Error: Memory allocation failed.");
        BIO_free(bio);
        EVP_PKEY_free(pubkey);
        X509_free(cert);
        return NULL;
    }

    // Read the PEM data into the allocated buffer
    BIO_read(bio, key_str, key_len);
    key_str[key_len] = '\0';  // Null-terminate the string
    *len = key_len;

    // Cleanup
    BIO_free(bio);
    EVP_PKEY_free(pubkey);
    X509_free(cert);

    return key_str;
}



int check_token(char* token, char** scope){
    int ret = 0;
    const long leeway = 60;
    jwt_t *jwt = NULL;
    jwt_t *decoded_jwt = NULL;
    size_t cert_len = 0;
    char *cert = NULL;

    // Load the certificate from file (used for token verification)
    cert = load_public_key_from_cert_file("./build/configs/open5gs/tls/nrf.crt", &cert_len);
    if (!cert) {
        jwt_free(jwt);
        return false;
    }


    // Decode (verify) the token by providing the certificate.
    // The certificate must be a valid PEM-encoded X.509 certificate containing the public key.
    if (jwt_decode(&decoded_jwt, token, cert, cert_len) != 0) {
        ogs_error("Error: Token verification failed using the certificate.");
        return false;
    } else {
        // On successful verification, retrieve and display the issuer claim.
        long expires_in = jwt_get_grant_int(decoded_jwt, "exp");
        if (time(NULL)+leeway > expires_in) {
            ogs_error("token expired");
            return false;
        }
        *scope = ogs_strdup(jwt_get_grant(decoded_jwt, "scope"));
    }

    // Clean up all allocated memory and JWT objects
    free(cert);
    jwt_free(jwt);
    if (decoded_jwt)
        jwt_free(decoded_jwt);

    return true;
}