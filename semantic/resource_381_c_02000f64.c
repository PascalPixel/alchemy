#include "types.h"

/*
 * resource_381 sixteen-actor formation owner at 0x02000f64, complete 288-byte
 * span through its three-word pool. The apparent returns at 0x0ffa/0x100a are
 * two calls through the relocated IWRAM helper, not function boundaries.
 */

typedef s32 (*ProjectionHelper)(s32 angle, s32 scale);
extern s32 Func_08000118(s32 angle);
extern s32 Func_08000120(s32 angle);
extern void Func_08009080(u8 *actor, s32 mode);
extern void Func_08009150(u8 *actor, s32 x, s32 y, s32 z);
extern void Func_080091e0(u8 *actor, s32 mode);
extern void Func_0808a010(s32 frames);
extern u8 *Func_0808a080(s32 actor);
extern void Func_0808a0a8(s32 actor);
extern void Func_0808a0e8(s32 actor);
extern void Func_080f9010(s32 sound);

void Func_02000f64(void)
{
    ProjectionHelper project = (ProjectionHelper)0x03000118;
    s32 actor;
    s32 index;

    for (actor = 16; actor <= 31; actor++) {
        u8 *record = Func_0808a080(actor);
        u8 *display;

        Func_0808a0a8(actor);
        Func_080091e0(record, 0);
        Func_08009080(record, 2);
        display = *(u8 **)(record + 80);
        display[9] &= (u8)~12;
        record[85] = 0;
        *(s32 *)(record + 48) = 0x80000;
        *(s32 *)(record + 52) = 0xc000;
        *(s32 *)(record + 24) = 0x1cccc;
        *(s32 *)(record + 28) = 0x1cccc;
        *(s32 *)(record + 8) = 0x00e80000;
        *(s32 *)(record + 12) = 0x00140000;
        *(s32 *)(record + 16) = 0x00840000;
    }

    Func_080f9010(0x91);

    for (index = 0; index <= 15; index++) {
        u8 *record = Func_0808a080(index + 16);
        s32 angle = index << 12;
        s32 x_offset;
        s32 z_offset;

        *(u16 *)(*(u8 **)(record + 80) + 30) =
            (u16)(angle - 0x4000);
        x_offset = project(Func_08000120(angle), 0x01000000);
        z_offset = project(Func_08000118(angle), 0x01000000);
        Func_08009150(record,
                      *(s32 *)(record + 8) + x_offset,
                      *(s32 *)(record + 12),
                      *(s32 *)(record + 16) + z_offset);
    }

    Func_0808a010(20);
    Func_0808a0e8(16);

    for (actor = 16; actor <= 31; actor++) {
        u8 *record = Func_0808a080(actor);

        Func_0808a0a8(actor);
        *(s32 *)(record + 24) = 0x10000;
        *(s32 *)(record + 28) = 0x10000;
        *(s32 *)(record + 8) = 0;
        *(s32 *)(record + 12) = 0;
        *(s32 *)(record + 16) = 0;
        *(s32 *)(record + 36) = 0;
        *(s32 *)(record + 40) = 0;
        *(s32 *)(record + 44) = 0;
        *(s32 *)(record + 56) = (s32)0x80000000;
        *(s32 *)(record + 60) = (s32)0x80000000;
        *(s32 *)(record + 64) = (s32)0x80000000;
    }
}
