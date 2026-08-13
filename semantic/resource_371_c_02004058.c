#include "types.h"

/* Resource 371: spawn and configure the two halves of a paired attachment.
 *
 * Reconstruction state, measured against the reference bytes on the routed
 * path, which carries -fno-strength-reduce for this owner's stem:
 * - the entry sequence (high-register saves, sub sp #8, the hoisted 63 and
 *   state pointer) now matches the reference byte-for-byte;
 * - the spawn-slot store must be the indexed form `pieces[index] = piece`
 *   compiled with -fno-strength-reduce; the default route strength-reduces it
 *   into a pointer walk (`stmia r3!` plus a spilled cursor) that can never
 *   match the reference's `lsls r3, r7, #2 / str r0, [r3, r2]`;
 * - the piece byte/halfword clears at +0x55/+0x64 must share one s32 zero
 *   local through a `p`/`p + 15` base so they compile to one `movs`-zero and
 *   an incremented address, exactly as the reference;
 * - the record-branch clears at +0x26 and +0x16 share a second zero that the
 *   reference keeps in r8, loaded from a literal pool per iteration.
 * Residual: the reference assigns sp-base to sl and the record zero to r8;
 * this source currently receives the opposite pairing, and the remaining
 * differing bytes are that swap's encodings rippling through the body.
 */

extern u8 *Data_03001f30;
extern u8 Data_03001b10[];

u8 *Func_0200828e();
void Func_020082ae();
void Func_020082a4();
void Func_02004004(void *object);
void Func_02003fb4(void *object);

void Func_02004058(u8 *parent)
{
    u8 *state = Data_03001f30;
    u8 *pieces[2];
    u8 *piece;
    u8 *record;
    s32 index;
    u32 id;
    s32 low6;
    s32 clear;
    s32 held;

    low6 = 63;
    held = 0;
    for (index = 0; index <= 1; index++) {
        piece = Func_0200828e(26,
                              *(s32 *)(parent + 0x08),
                              *(s32 *)(parent + 0x0c),
                              *(s32 *)(parent + 0x10));
        pieces[index] = piece;
        if (piece == 0) {
            continue;
        }

        *(s32 *)(piece + 0x14) = *(s32 *)(parent + 0x14);
        {
            u8 *p = piece + 0x55;
            s32 zero = 0;

            record = *(u8 **)(piece + 0x50);
            *p = zero;
            *(u16 *)(p + 15) = zero;
        }
        *(u8 **)(piece + 0x68) = parent;
        if (record == 0) {
            continue;
        }

        Func_020082ae(record, 0);
        record[0x26] = held;
        Func_020082a4(record[0x1c]);

        record[0x1c] = (u8)*(u16 *)(state + 70);
        record[0x1d] |= 1;
        id = *(u16 *)(Data_03001b10 + record[0x1c] * 4 + 2);
        *(u16 *)(record + 8) = (u16)
            ((*(u16 *)(record + 8) & 0xfffffc00) | ((id << 17) >> 22));
        record[5] = (u8)(((record[5] & ~0x20) & low6) | 0x40);
        record[7] = (u8)((record[7] & low6) | 0x80);
        *(u8 *)(*(u8 **)(record + 0x28) + 0x16) = held;
    }

    record = *(u8 **)(pieces[0] + 0x50);
    *(void (**)(void *))(pieces[0] + 0x6c) = Func_02004004;
    clear = -13;
    record[9] = (u8)((record[9] & clear) | 4);

    record = *(u8 **)(pieces[1] + 0x50);
    *(void (**)(void *))(pieces[1] + 0x6c) = Func_02003fb4;
    record[9] = (u8)((record[9] & clear) | 4);
    pieces[1][0x23] = 2;
}
