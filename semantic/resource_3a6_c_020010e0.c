#include "types.h"











/*
 * resource_3a6 owner at 0x020010e0, 192 bytes including its mask pool word:
 * place scene record zero on the center of its current metatile, attempt the
 * presentation, and configure the successful record while preserving flags.
 */
extern u8 * Func_02002da8();
extern s32 Func_02002d90();
extern void Func_02002dd0();
extern void Func_02002d60();
extern void Func_02002d4e();
extern void Func_02002e94();
extern void Func_02002d74();
extern void Func_02002dde();
extern void Func_02002e3c();
extern void Func_02002dac();
extern void Func_02002dfc();
s32 Func_020010e0(void)
{
    s32 position[3];
    u8 *record = Func_02002da8(0);
    u8 saved_flags = record[0x55];

    position[1] = *(s32 *)(record + 12);
    position[0] = (*(s32 *)(record + 8) & 0xfff00000) + 0x00080000;
    position[2] = (*(s32 *)(record + 16) & 0xfff00000) + 0x02800000;

    if (Func_02002d90(record, position) != 0)
        return 0;

    Func_02002dd0();
    record[0x55] &= 0x7e;
    Func_02002d60(record, 6);
    Func_02002d4e(6);
    Func_02002e94(152);
    Func_02002d74(record, 7);
    *(s32 *)(record + 52) = 0x00020000;
    *(s32 *)(record + 40) = 0x00040000;
    *(s32 *)(record + 48) = 0x00030000;
    Func_02002dde(record, 0);
    Func_02002e3c(0, *(s16 *)((u8 *)position + 2),
                        *(s16 *)((u8 *)position + 10));
    Func_02002dac(record, 6);
    Func_02002dfc(record, 1);
    Func_02002e3c();
    record[0x55] = saved_flags;
    return 1;
}
