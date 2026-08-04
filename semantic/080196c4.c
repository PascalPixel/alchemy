#include "types.h"

/*
 * __call_via_rN veneer site, resolved per-site against the ROM.
 *
 * All four `bl Func_08007304` sites (0x0801973a, 0x08019746, 0x0801975a,
 * 0x08019772) are `__call_via_r8` -- 0x08007304 is index 8 of the 0x080072e4
 * bank. The callee is not a function at 0x08007304.
 *
 * r8 is written ONCE, at 0x0801970c, from `ldr r3, [r2, #0]` at 0x08019704
 * where r2 is 0x03001e50 + 200. 200 is 0x32 * 4, so that word is
 * Func_080048b0's allocation slot for id 0x32 -- the same slot table read at
 * 0x08012388, 0x08021be0 and 0x080f02b0, checked again here from this file's
 * own pool word at 0x080197a0. The lazy-init block above allocates it and
 * DMAs 0x140 bytes from 0x08015430 into it when it is still null.
 *
 * So the callee is the relocated parser routine, and it is LATCHED into r8
 * before the loop rather than re-read per iteration.
 *
 * ARITY: one argument. Every site does `adds r0, r5, #0` (r5 = sp, the
 * 12-byte parser workspace) immediately before the branch, and nothing else
 * is set. r8 is outside the r0-r3 argument registers, so the register-index
 * regularity does not apply and there is no hidden callee-in-an-argument-slot
 * here. The result comes back in r0 and is stored as a halfword token.
 */
typedef u32 (*TextTokenReader)(void *parser);

void *Func_080048b0(s32, s32);
s32 Func_08002dd8(s32);
void Func_08019bac(void *, s32);

/*
 * Decode a text stream into the engine's halfword token buffer. Most tokens
 * occupy one slot; the extended control forms consume an additional one or
 * two payload bytes. A temporary text workspace is installed when the shared
 * workspace is not already active.
 */
s32 Func_080196c4(s32 text_id, u16 *output, s32 slots_left) {
    u8 parser[12];
    volatile u32 *dma3;
    void *workspace;
    u32 token;
    s32 owns_workspace;
    TextTokenReader readToken;

    owns_workspace = (*(s32 *)0x03001F18 == 0);
    if (owns_workspace) {
        workspace = Func_080048b0(0x32, 0x140);
        dma3 = (volatile u32 *)0x040000D4;
        dma3[0] = 0x08015430;
        dma3[1] = (u32)workspace;
        dma3[2] = 0x84000050;
    }

    /* Latched once, before the loop, exactly as r8 is at 0x0801970c. */
    readToken = *(TextTokenReader *)0x03001F18;

    Func_08019bac(parser, text_id);
    for (;;) {
        token = readToken(parser);
        if (token == 0) {
            break;
        }

        if (token == 0xE) {
            slots_left -= 3;
            if (slots_left <= 0) {
                break;
            }
            *output++ = (u16)token;
            *output++ = (u16)(readToken(parser) - 1);
            *output++ = (u16)(readToken(parser) - 1);
        } else if (token == 0xF || (token >= 8 && token <= 0xC)) {
            slots_left -= 1;
            if (slots_left <= 0) {
                break;
            }
            *output++ = (u16)token;
            *output++ = (u16)(readToken(parser) - 1);
        } else {
            slots_left -= 1;
            if (slots_left <= 0) {
                break;
            }
            *output++ = (u16)token;
        }
    }

    *output = 0;
    if (owns_workspace) {
        return Func_08002dd8(0x32);
    }
    return (s32)token;
}
