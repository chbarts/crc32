/* This file is in the public domain.
 * Chris Barts 2015 chbarts@gmail.com */

#include <stdlib.h>
#include "crc32.h"

struct crcctx {
    uint32_t crc;
};

crcctx *crc32_new(void)
{
    crcctx *res;

    if ((res = malloc(sizeof(crcctx))) == NULL)
        return NULL;

    res->crc = 0xFFFFFFFF;
    return res;
}

void crc32_free(crcctx * ctx)
{
    free(ctx);
}

int crc32_add(crcctx * ctx, uint8_t * buf, size_t len)
{
    size_t i;
    const uint32_t g0 = 0xEDB88320, g1 = g0 >> 1,
        g2 = g0 >> 2, g3 = g0 >> 3;
    uint32_t c, b;
    int j;

    if (NULL == ctx)
        return -1;

    for (i = 0; i < len; i++) {
        b = buf[i];
        ctx->crc ^= b;
        for (j = 1; j >= 0; j--) {
            switch (ctx->crc & 0xf) {
            case 0:
                c = 0;
                break;
            case 1:
                c = g3;
                break;
            case 2:
                c = g2;
                break;
            case 3:
                c = g2 ^ g3;
                break;
            case 4:
                c = g1;
                break;
            case 5:
                c = g1 ^ g3;
                break;
            case 6:
                c = g1 ^ g2;
                break;
            case 7:
                c = g1 ^ g2 ^ g3;
                break;
            case 8:
                c = g0;
                break;
            case 9:
                c = g0 ^ g3;
                break;
            case 10:
                c = g0 ^ g2;
                break;
            case 11:
                c = g0 ^ g2 ^ g3;
                break;
            case 12:
                c = g0 ^ g1;
                break;
            case 13:
                c = g0 ^ g1 ^ g3;
                break;
            case 14:
                c = g0 ^ g1 ^ g2;
                break;
            case 15:
                c = g0 ^ g1 ^ g2 ^ g3;
                break;
            }

            ctx->crc = ((ctx->crc) >> 4) ^ c;
        }
    }

    return 0;
}


uint32_t crc32_run(crcctx * ctx)
{
    if (NULL == ctx)
        return -1;

    return ~(ctx->crc);
}

int crc32_reset(crcctx * ctx)
{
    if (NULL == ctx)
        return -1;

    ctx->crc = 0xFFFFFFFF;
    return 0;
}
