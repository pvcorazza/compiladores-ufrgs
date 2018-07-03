#include "tac.h"
#include <inttypes.h>

typedef union {
    uint32_t uint32;
    float f;
} aliasing;

/**
 * Gera assembler
 * @param tac
 */
void assembler_generate(TAC *tac);
