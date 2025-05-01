#ifndef OAUTH_HANDLER_H
#define OAUTH_HANDLER_H

#include "event.h"
#include "context.h"

#ifdef __cplusplus
extern "C" {
#endif

int ogs_num_delimeter(const char* s, const char deli);
char** ogs_split_str(const char* s, const char deli, int* num);
int oauth_handler(ogs_sbi_message_t message, ogs_sbi_stream_t *stream);

#ifdef __cplusplus
}
#endif

#endif /* OAUTH_HANDLER_H */
