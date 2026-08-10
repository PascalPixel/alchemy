#include "types.h"

/* Resource 371: spawn and configure the two halves of a paired attachment. */

extern u8 *Data_03001f30;
extern u16 Data_03001b10[];

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
        record = *(u8 **)(piece + 0x50);
        piece[0x55] = 0;
        *(u16 *)(piece + 0x64) = 0;
        *(u8 **)(piece + 0x68) = parent;
        if (record == 0) {
            continue;
        }

        Func_020082ae(record, 0);
        record[0x26] = 0;
        Func_020082a4(record[0x1c]);

        record[0x1c] = (u8)*(u16 *)(state + 70);
        record[0x1d] |= 1;
        *(u16 *)(record + 8) = (u16)
            ((*(u16 *)(record + 8) & 0xfc00) |
             ((Data_03001b10[record[0x1c] * 2 + 1] >> 5) & 0x3ff));
        record[5] = (u8)((record[5] & ~0x20 & 0x3f) | 0x40);
        record[7] = (u8)((record[7] & 0x3f) | 0x80);
        *(u8 *)(*(u8 **)(record + 0x28) + 0x16) = 0;
    }

    record = *(u8 **)(pieces[0] + 0x50);
    *(void (**)(void *))(pieces[0] + 0x6c) = Func_02004004;
    record[9] = (u8)((record[9] & ~0x0c) | 0x04);

    record = *(u8 **)(pieces[1] + 0x50);
    *(void (**)(void *))(pieces[1] + 0x6c) = Func_02003fb4;
    record[9] = (u8)((record[9] & ~0x0c) | 0x04);
    pieces[1][0x23] = 2;
}
