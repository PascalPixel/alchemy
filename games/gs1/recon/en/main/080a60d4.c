#include "types.h"
#include "global_cells.h"
#include "runtime_mem.h"

extern u8 *Data_03001f2c;
s32 Runtime_GetObject(s32);
s32 UiWindow_UpdateOrCreate(s32 *window, s32, s32, s32, s32, s32);

void Func_080a33d4(s32, s32);
void Func_080a6384(s32);
void Func_080a112c(s32, s32, s32, s32);
void Func_080a6614(s32, s32);
void Func_080a1804(s32, s32);
void UiWindow_Commit(s32);
s32 GameFlag_IsSet(s32);
s32 Func_080770d0(s32);
void Func_080a23c0(s32);
void Func_080a1a40(s32, s32);
void Audio_PlayCue(s32);
void WaitFrames(s32);
s32 Modulo(s32, s32);
u8 PsynergyMenu_CollectActions(void *, u16 *, s32);

s32 Func_080a60d4(u16 *actions)
{
    u8 *menu;
    s8 index;
    s8 cursor_index;
    s32 window_a;
    s32 window_b;
    s32 changed;
    s32 aborted;
    s32 selected_word;
    s32 result;

    menu = Data_03001f2c;
    index = *(s8 *)(menu + 30);
    cursor_index = *(s8 *)(menu + 28);
    changed = 1;
    aborted = 0;
    menu[0x268] = (u8)0;

    selected_word = actions[cursor_index];
    Runtime_GetObject(selected_word);

    window_a = UiWindow_UpdateOrCreate((s32 *)(menu + 32), 13, 3, 17, 10, 2);
    if (window_a) {
        Func_080a33d4((s32)menu, ((s32 *)(menu + 32))[0]);
    }

    window_b = UiWindow_UpdateOrCreate((s32 *)(menu + 40), 13, 13, 17, 10, 2);
    if (window_b) {
        result = 0; /* placeholder for the object-record allocation branch */
        menu[0x21b] = (u8)result;
        *(s32 *)(menu + 44) = result;
    }

    for (;;) {
        if (changed) {
            changed = 0;
            selected_word = actions[cursor_index];
            Runtime_GetObject(selected_word);
            Func_080a6384(selected_word);
            Func_080a112c(*(s32 *)(menu + 36), selected_word, 0, 0);
            Func_080a6614(*(s32 *)(menu + 40), selected_word);
            Func_080a1804((s32)menu, selected_word);

            if (!GameFlag_IsSet(0x150) && !aborted) {
                UiWindow_Commit(*(s32 *)(menu + 44));
                UiWindow_Commit(*(s32 *)(menu + 44));
                Func_080a23c0(*(s32 *)(menu + 44));
                aborted = 1;
            } else {
                Func_080770d0(0x151);
            }
        }

        Func_080a1a40(cursor_index * 8 - 10, 16);
        WaitFrames(1);

        if (*(volatile u32 *)ADDR_03001C94 & 1) {
            Audio_PlayCue(112);
            menu[0x268] = 1;
            result = selected_word;
        } else if (*(volatile u32 *)ADDR_03001C94 & 2) {
            Audio_PlayCue(113);
            result = -1;
        } else if (*(volatile u32 *)ADDR_03001B04 & 0x20) {
            Audio_PlayCue(111);
            cursor_index -= 1;
            changed = 1;
        } else if (*(volatile u32 *)ADDR_03001B04 & 0x10) {
            Audio_PlayCue(111);
            cursor_index += 1;
            changed = 1;
        } else {
            continue;
        }
        break;
    }

    menu[28] = cursor_index;
    selected_word = actions[cursor_index];
    *(s32 *)(menu + 8) = selected_word;
    menu[0x21a] = (u8)selected_word;
    return result;
}
