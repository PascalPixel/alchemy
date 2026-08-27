#include "types.h"
#include "global_cells.h"
#include "runtime_mem.h"

extern u8 *Data_03001f2c;
void *Runtime_GetObject(s32 owner);
void Func_080a3ef0(s32 owner, s32 arg1, s32 arg2);
s32 Func_08077050(s32 owner, s32 item);
void UiText_DrawAt(s32 message, s32 window, s32 x, s32 y);
void Func_08015068(s32 window, s32 a, s32 b, s32 c, s32 d);
void Audio_PlayCue(s32);
s32 Modulo(s32, s32);
void WaitFrames(s32);
s32 GameFlag_IsSet(s32);
void Func_080a1ac0(s32, s32);
void Func_080a1a40(s32, s32);
s32 BattleUnit_Recalculate(s32);
s32 Func_080772c0(s32);

s32 Func_080a5388(void)
{
    u8 *menu;
    s32 selection;
    s32 changed;
    u8 owner;
    void *owner_state;
    u16 field176;
    u32 record;
    s32 window;
    s32 mode;

    menu = Data_03001f2c;
    owner = menu[0x21B];
    owner_state = Runtime_GetObject(owner);
    field176 = *(u16 *)(menu + 0x176);
    owner = menu[0x21B];
    Func_080a3ef0(owner, field176, 0);
    record = Runtime_BumpAllocate(0x14C);
    Mem_Copy((void *)record, owner_state, 0x14C, (void *)0x03001388);
    window = *(s32 *)(menu + 0x10C);
    owner = menu[0x21B];
    field176 = *(u16 *)(menu + 0x176);
    mode = Func_08077050(owner, field176);
    mode += 2;

    if ((u32)mode > 1) {
        UiText_DrawAt(0xB2C, window, 24, 24);
        UiText_DrawAt(0xB2D, window, 72, 24);
        Func_08015068(window, 16, 16, 96, 24);
        UiText_DrawAt(0xAD6, window, 0, 16);
        Func_080a1ac0(110, 32);

        selection = 0;
        changed = 1;
        for (;;) {
            Func_080a1a40(selection * 16 + selection * 32 + 110, 32);
            if (*(volatile u32 *)ADDR_03001B04 & 0x20) {
                changed = 1;
                selection -= 1;
                Audio_PlayCue(111);
            }
            if (*(volatile u32 *)ADDR_03001B04 & 0x10) {
                changed = 1;
                selection += 1;
                Audio_PlayCue(111);
            }
            WaitFrames(1);
            if (GameFlag_IsSet(0x150)) {
                break;
            }
            if (changed) {
                changed = 0;
                selection = Modulo(selection + 2, 2);
            }
            if (*(volatile u32 *)ADDR_03001C94 & 1) {
                break;
            }
            if (*(volatile u32 *)ADDR_03001C94 & 2) {
                Audio_PlayCue(113);
                selection = 1;
                break;
            }
        }
    } else {
        Audio_PlayCue(175);
        selection = 1;
    }

    if (GameFlag_IsSet(0x150)) {
        selection = 1;
    }
    if (selection == 1) {
        owner = menu[0x21B];
        Mem_Copy(owner_state, (void *)record, 0x14C, (void *)0x03001388);
    }

    Sys_Free((void *)record);
    owner = menu[0x21B];
    BattleUnit_Recalculate(owner);
    owner = menu[0x21B];
    Func_080772c0(owner);
    return selection;
}
