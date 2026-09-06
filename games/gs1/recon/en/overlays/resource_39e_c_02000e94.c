#include "types.h"

#define FieldScene_RunPrimarySequence Func_02000e94

void Func_02001040();
void Func_02001058();
void Func_0200107a();
void Func_02001096();
void Func_020051f8();
void *Func_020052a6();
void Func_02005462();

void FieldScene_RunPrimarySequence(void)
{
    void *scene;
    s32 actor;
    s32 base;
    s32 slot;

    scene = Func_020052a6(19);
    actor = 0;
    slot = 8;
    do {
        Func_020051f8(slot);
        *(s32 *)(scene + 16) += 0x10000;
        *(s32 *)(scene + 64) = (s32)0x80000000;
        actor++;
        slot -= 2;
    } while (actor <= 3);

    *(u16 *)(*(u8 **)(scene + 80) + 30) = 0;
    *(s32 *)(scene + 16) += 0x180000;
    *(s32 *)(scene + 64) = (s32)0x80000000;
    Func_02005462(227);
    base = 0x3333;

    Func_02001040(
        *(s32 *)(scene + 8),
        *(s32 *)(scene + 12),
        *(s32 *)(scene + 16) + 0xc0000,
        (s32)0xffff3334,
        0,
        base,
        0,
        0);
    Func_02001058(
        *(s32 *)(scene + 8),
        *(s32 *)(scene + 12),
        *(s32 *)(scene + 16) + 0xc0000,
        0xcccc,
        0,
        base,
        0,
        0);
    Func_0200107a(
        *(s32 *)(scene + 8) - 0x60000,
        *(s32 *)(scene + 12),
        *(s32 *)(scene + 16) - 0x80000,
        base,
        0,
        0x10000,
        0,
        0);
    Func_02001096(
        *(s32 *)(scene + 8) + 0x60000,
        *(s32 *)(scene + 12),
        *(s32 *)(scene + 16) - 0x80000,
        base,
        0,
        0x10000,
        0,
        0);
}
