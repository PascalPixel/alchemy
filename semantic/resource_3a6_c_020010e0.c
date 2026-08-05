#include "types.h"

u8 *Func_0808a080();
s32 Func_080091d8();
void Func_080000c0();
void Func_08009080();
void Func_080091e0();
void Func_0808a018();
void Func_0808a020();
void Func_0808a0c0();
void Func_080f9010();

/*
 * resource_3a6 owner at 0x020010e0, 192 bytes including its mask pool word:
 * place scene record zero on the center of its current metatile, attempt the
 * presentation, and configure the successful record while preserving flags.
 */
s32 Func_020010e0(void)
{
    s32 position[3];
    u8 *record = Func_0808a080(0);
    u8 saved_flags = record[0x55];

    position[1] = *(s32 *)(record + 12);
    position[0] = (*(s32 *)(record + 8) & 0xfff00000) + 0x00080000;
    position[2] = (*(s32 *)(record + 16) & 0xfff00000) + 0x02800000;

    if (Func_080091d8(record, position) != 0)
        return 0;

    Func_0808a018();
    record[0x55] &= 0x7e;
    Func_08009080(record, 6);
    Func_080000c0(6);
    Func_080f9010(152);
    Func_08009080(record, 7);
    *(s32 *)(record + 52) = 0x00020000;
    *(s32 *)(record + 40) = 0x00040000;
    *(s32 *)(record + 48) = 0x00030000;
    Func_080091e0(record, 0);
    Func_0808a0c0(0, *(s16 *)((u8 *)position + 2),
                        *(s16 *)((u8 *)position + 10));
    Func_08009080(record, 6);
    Func_080091e0(record, 1);
    Func_0808a020();
    record[0x55] = saved_flags;
    return 1;
}
