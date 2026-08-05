#include "types.h"

extern u8 *Func_0808a080(s32 id);
extern void Func_08000128(s32 distance, s32 heading, s32 *position);
extern s32 Func_080091d8(u8 *record, s32 *position);
extern void Func_080770d0(s32 id);
extern void Func_02000ad0(void);
extern void Func_08009080(u8 *record, s32 mode);
extern void Func_080000c0(s32 frames);
extern void Func_080f9010(s32 id);
extern void Func_080091e0(u8 *record, s32 mode);
extern void Func_0808a0c0(s32 id, s32 x, s32 z);

void Func_02000928(void)
{
    u8 *record = Func_0808a080(0);
    s32 position[3];
    u8 saved_mode;
    s32 heading;

    heading = (*(u16 *)(record + 6) + 0x1000) & 0xe000;
    saved_mode = record[0x55];
    position[0] = (*(s32 *)(record + 8) & (s32)0xfff00000) + 0x100000;
    position[1] = *(s32 *)(record + 12);
    position[2] = (*(s32 *)(record + 16) & (s32)0xfff00000) + 0x100000;

    Func_08000128(0x200000, heading, position);
    if (Func_080091d8(record, position) != 0)
        return;

    Func_080770d0(0x250);
    Func_02000ad0();
    Func_08009080(record, 6);
    Func_080000c0(6);
    Func_080f9010(152);
    Func_08009080(record, 7);

    *(s32 *)(record + 0x30) = 0x30000;
    *(s32 *)(record + 0x34) = 0x20000;
    *(s32 *)(record + 0x28) = 0x40000;
    record[0x55] &= 0x7e;

    record[0x55] = saved_mode;
    Func_080091e0(record, 0);
    Func_0808a0c0(0, *(s16 *)((u8 *)position + 2),
                     *(s16 *)((u8 *)position + 10));
    Func_08009080(record, 6);
    Func_080091e0(record, 1);
}
