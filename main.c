/* This file is in the public domain.
 * Chris Barts 2015 chbarts@gmail.com */

#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include "crc32.h"
#include "handle_ferr.h"

int main(int argc, char *argv[])
{
    int i;
    size_t len;
    uint8_t buf[BUFSIZ];
    FILE *fin;
    crcctx *ctx;

    if ((ctx = crc32_new()) == NULL) {
        fprintf(stderr, "%s: failed to create context\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    if (1 == argc) {
        while ((len = fread(buf, sizeof(buf[0]), sizeof(buf), stdin)) != 0)
            crc32_add(ctx, buf, len);

        if (ferror(stdin)) {
            handle_ferr("stdin", argv[0]);
            exit(EXIT_FAILURE);
        }

        printf("%08lx\tstdin\n", (unsigned long) crc32_run(ctx));
        crc32_free(ctx);
        exit(EXIT_SUCCESS);
    }

    for (i = 1; i < argc; i++) {
        if ((fin = fopen(argv[i], "rb")) == NULL) {
            fprintf(stderr, "failed to open %s: ", argv[i]);
            handle_ferr(argv[i], argv[0]);
            continue;
        }

        while ((len = fread(buf, sizeof(buf[0]), sizeof(buf), fin)) != 0)
            crc32_add(ctx, buf, len);

        if (ferror(fin)) {
            fprintf(stderr, "error reading from %s: ", argv[i]);
            handle_ferr(argv[i], argv[0]);
            goto lbl1;
        }

        printf("%08lx\t%s\n", (unsigned long) crc32_run(ctx), argv[i]);

      lbl1:
        crc32_reset(ctx);

        if (fclose(fin) == EOF) {
            fprintf(stderr, "error closing %s: ", argv[i]);
            handle_ferr(argv[i], argv[0]);
        }
    }

    crc32_free(ctx);
    exit(EXIT_SUCCESS);
}
