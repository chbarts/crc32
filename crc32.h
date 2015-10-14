#ifndef CRC32_H
#define CRC32_H

#include <stdint.h>
#include <stdio.h>

typedef struct crcctx crcctx;

crcctx *crc32_new(void);
int crc32_add(crcctx * ctx, uint8_t * buf, size_t len);
uint32_t crc32_run(crcctx * ctx);
int crc32_reset(crcctx * ctx);
void crc32_free(crcctx * ctx);

#endif
