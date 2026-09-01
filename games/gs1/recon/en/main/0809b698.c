#include "types.h"

extern void Func_080030f8(s32);
extern s32 Func_08003f3c(s32);
extern s32 Func_08003fa4(s32, s32, const void *);
extern s16 Func_08004080(void);
extern void Func_080041d8(const void *, s32);
extern void Func_08004278(const void *);
extern void Func_080091e0(void *, s32);
extern void Func_08009240(void *, s32);
extern void Func_08015040(s32, s32);
extern s32 Func_080770c0(s32);
extern void Func_080f9010(s32);

extern u8 *Data_03001f30;
extern u8 Data_02000240[];

void Func_0809b698(void)
{
    u8 *scene;
    u8 *object;
    u8 *group;
    u8 *entry;
    u32 saved;
    u16 value;
    s32 index;
    s32 active;
    s32 entry_mode;
    s32 count;

    scene = Data_03001f30;
    object = *(u8 **)(scene + 16);
    group = *(u8 **)(object + 80);
    saved = *(u16 *)(object + 6);
    entry = *(u8 **)(group + 40);
    value = Func_08004080();
    {
        s32 zero = 0;
        *(s16 *)(scene + 0x71a) = value;
        Func_08003fa4((s16)value, 0x100, (const void *)0x0809c510);
        index = 145;
        ((s32 *)Data_02000240)[index] = 0x09600000;
        index = 146;
        *(s8 *)&((s32 *)Data_02000240)[index] = Func_080770c0(0x145);
        Func_08009240(object, zero);
        *(void **)(object + 108) = (void *)0x0809b5dd;
        *(s16 *)(object + 100) = zero;
        *(s16 *)(object + 102) = zero;
    }
    Func_080f9010(0x8c);
    Func_080030f8(15);
    active = 1;
    *(s16 *)(object + 100) = active;
    Func_080030f8(10);
    entry_mode = 7;
    count = 19;
    do {
        *(s8 *)(entry + 5) = entry_mode;
        *(s8 *)(group + 37) = 1;
        Func_080030f8(2);
        *(s8 *)(group + 37) = 1;
        *(s8 *)(entry + 5) = 0;
        *(s8 *)(group + 38) = 1;
        count--;
        Func_080030f8(3);
    } while (count >= 0);
    *(void **)(object + 108) = 0;
    *(u16 *)(object + 6) = saved;
    Func_080041d8((const void *)0x0809b589, 0xc80);
    Func_080030f8(15);
    Func_080f9010(0xae);
    Func_080030f8(55);
    Func_08004278((const void *)0x0809b589);
    index = 147;
    if (((s16 *)Data_02000240)[index * 2] != 0) {
        Func_080091e0(object, 2);
    } else {
        Func_080091e0(object, 1);
    }
    Func_08009240(object, 0);
    Func_08003f3c(*(s16 *)(scene + 0x71a));
    Func_08015040(0x922, 1);
}

void RunBattleEffect16(void) __attribute__((alias("Func_0809b698")));
