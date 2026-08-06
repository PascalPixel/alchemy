#include "types.h"












extern u8 * Func_02004024(s32 id);
extern void Func_02003fca(s32 distance, s32 heading, s32 *position);
extern s32 Func_02004002(u8 *record, s32 *position);
extern void Func_02004046(s32 id);
extern void Func_0200145a(void);
extern void Func_02003ff2(u8 *record, s32 mode);
extern void Func_02003fc8(s32 frames);
extern void Func_020041de(s32 id);
extern void Func_02004006(u8 *record, s32 mode);
extern void Func_02004058(u8 *record, s32 mode);
extern void Func_020040de(s32 id, s32 x, s32 z);
extern void Func_0200403e(u8 *record, s32 mode);
extern void Func_02004076(u8 *record, s32 mode);
void Func_02000928(void)
{
    u8 *record = Func_02004024(0);
    s32 position[3];
    u8 saved_mode;
    s32 heading;

    heading = (*(u16 *)(record + 6) + 0x1000) & 0xe000;
    saved_mode = record[0x55];
    position[0] = (*(s32 *)(record + 8) & (s32)0xfff00000) + 0x100000;
    position[1] = *(s32 *)(record + 12);
    position[2] = (*(s32 *)(record + 16) & (s32)0xfff00000) + 0x100000;

    Func_02003fca(0x200000, heading, position);
    if (Func_02004002(record, position) != 0)
        return;

    Func_02004046(0x250);
    Func_0200145a();
    Func_02003ff2(record, 6);
    Func_02003fc8(6);
    Func_020041de(152);
    Func_02004006(record, 7);

    *(s32 *)(record + 0x30) = 0x30000;
    *(s32 *)(record + 0x34) = 0x20000;
    *(s32 *)(record + 0x28) = 0x40000;
    record[0x55] &= 0x7e;

    record[0x55] = saved_mode;
    Func_02004058(record, 0);
    Func_020040de(0, *(s16 *)((u8 *)position + 2),
                     *(s16 *)((u8 *)position + 10));
    Func_0200403e(record, 6);
    Func_02004076(record, 1);
}
