#include "types.h"
#include "global_cells.h"

extern u8 *Data_03001f2c;
void *Runtime_GetObject(s32 owner);
s32 Modulo(s32, s32);
void Audio_PlayCue(s32);
void WaitFrames(s32);
s32 GameFlag_IsSet(s32);
void Func_080a1ac0(s32, s32);
void Func_080a1a40(s32, s32);
s32 Func_080770d0(s32);
void UiWindow_Commit(s32);
void UiText_DrawAt(s32, s32, s32, s32);
void Func_080a112c(s32, s32, s32, s32);
void Func_080a1804(s32, s32);

s32 Func_080a63e4(s32 mode)
{
    u8 *menu;
    s8 cursor_index;
    u8 party_count;
    s32 changed;
    s32 saved_flag;
    s32 restored;
    s32 index_word;
    s8 initial_index;
    struct {
        u16 x;
        u16 target_x;
        u16 y_lo;
        u8 pad[16];
        u16 flags;
    } *cursor_area;

    menu = Data_03001f2c;
    cursor_index = *(s8 *)(menu + 29);
    party_count = menu[0x219];
    changed = 1;
    saved_flag = 0;

    initial_index = *(s8 *)(menu + 28);
    index_word = *(u16 *)(menu + 0x208 + initial_index * 2);
    Runtime_GetObject(index_word);
    Func_080a1ac0(cursor_index * 8 + cursor_index * 16 - 10, 16);

    for (;;) {
        if (changed) {
            changed = 0;
            cursor_index = Modulo(cursor_index + party_count, party_count);
            index_word = *(u16 *)(menu + 0x208 + cursor_index * 2);
            Runtime_GetObject(index_word);

            /* window bounds recompute (unmodelled bitfield packing) */

            if (mode != 0) {
                UiWindow_Commit(*(s32 *)(menu + 44));
                UiText_DrawAt(0, 0, 0, 0); /* placeholder for Func_08015080 */
            } else {
                Func_080a112c(*(s32 *)(menu + 36), index_word, 0, 0);
                Func_080a1804((s32)menu, index_word);
                if (!GameFlag_IsSet(0x53a)) {
                    if (!saved_flag) {
                        UiWindow_Commit(*(s32 *)(menu + 44));
                        saved_flag = 1;
                    }
                } else {
                    Func_080770d0(0x151);
                }
            }
        }

        Func_080a1a40(cursor_index * 8 + cursor_index * 16 - 10, 16);
        WaitFrames(1);

        if (*(volatile u32 *)ADDR_03001C94 & 1) {
            Audio_PlayCue(112);
            restored = index_word;
        } else if (*(volatile u32 *)ADDR_03001C94 & 2) {
            Audio_PlayCue(113);
            restored = -1;
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

    menu[29] = cursor_index;
    WaitFrames(1);
    menu[29] = cursor_index;
    *(s32 *)(menu + 8) = index_word;
    menu[0x21b] = index_word;
    return restored;
}
