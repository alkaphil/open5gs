#ifndef OAUTH_HANDLER_H
#define OAUTH_HANDLER_H

#include "event.h"
#include "context.h"

#ifdef __cplusplus
extern "C" {
#endif

int oauth_handler(ogs_sbi_message_t message, ogs_sbi_stream_t *stream);

#ifdef __cplusplus
}
#endif

#endif /* OAUTH_HANDLER_H */
