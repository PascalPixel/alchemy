#include "types.h"

/*
 * resource_39f owner at 0x02001b84, 176 bytes: code 0x02001b84-0x02001c2d, an
 * alignment halfword at 0x02001c2e and the pool word 0xfffe0000 at 0x02001c30.
 *
 * The twin of 0x02001520 for slot 13 and the parking tile (45, 6): it marks the
 * actor's own tile 0xff, clears the four orthogonally adjacent tiles, and when
 * the actor stands on that tile clears the record's mode byte and writes
 * -2.0 (0xfffe0000 in 16.16) into the words at +12 and +20.
 *
 * See resource_39f_c_02000030.c for the link base and the `bl` encoding rule,
 * and resource_39f_c_02001520.c for the byte-exact Func_02000244 interface the
 * five painting calls use.
 *
 * The epilogue is `add sp, #8 / pop {r3} / ... / pop {r0} / bx r0`, so the
 * owner is void.
 */

void Func_02004932();
s32 *Func_02004950();
s32 *Func_02004958();
/* Byte-exact-shaped: (layer, x, z, width, height, value). Each call site
 * keeps its own raw overlay_show target per HANDOVER's exact-reconstruction
 * transcription rule -- overlay_call_targets.ts resolves all five to the
 * established Func_02000244 interface, but the assembler encodes each site
 * independently. */
s32 Func_02001dfa();
s32 Func_02001e0e();
s32 Func_02001e20();
s32 Func_02001e32();
s32 Func_02001e44();
s32 *Func_020049ca();
void Func_020049cc();

void Func_02001b84(void)
{
    s32 x;
    s32 z;

    /* No argument register is written before this branch. */
    Func_02004932();

    x = Func_02004950(13)[2] >> 20;
    z = Func_02004958(13)[4] >> 20;

    Func_02001dfa(2, x, z, 1, 1, 0xff);
    Func_02001e0e(2, x + 1, z, 1, 1, 0);
    Func_02001e20(2, x - 1, z, 1, 1, 0);
    Func_02001e32(2, x, z + 1, 1, 1, 0);
    Func_02001e44(2, x, z - 1, 1, 1, 0);

    if (x == 45 && z == 6) {
        u8 *record = (u8 *)Func_020049ca(13);

        record[85] = 0;
        *(s32 *)(record + 20) = (s32)0xfffe0000;
        *(s32 *)(record + 12) = (s32)0xfffe0000;
    }

    /* Common exit; no argument registers are set. */
    Func_020049cc();
}
