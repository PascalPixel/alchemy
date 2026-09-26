/* NONMATCHING: the preview takes the owner again as its fourth argument,
 * and the first leave-menu flag exits to done, not cancel. Both are fixed.
 * Remaining: owner/item setup allocation and the confirmation exit's layout.
 * Explicit confirm/setup labels and a one-pass cue block did not recover
 * the reference's bhi, long cancel branch, then cue 175 before menu setup. */
#include "TYPES.H"
#include "SYSTEM.H"

typedef s32 (*WordCopyFn)(void *dst, const void *src, s32 size);

static __inline__ s32 CopyWords(WordCopyFn copy, void *dst, const void *src, s32 size)
{
    return copy(dst, src, size);
}

struct EquipMenu {
    u8 padding000[0x10c];
    s32 selector_window;
    u8 padding110[0x66];
    u16 item;
    u8 padding178[0xa3];
    u8 owner;
};

extern u8 Value_00000b2c;

void *Owner_GetStateFar(s32 owner);
void ItemMenu_DrawEquipPreview(s32 owner, s32 item, s32 mode, s32 target);
void *Runtime_BumpAllocate(s32 size);
void Runtime_BumpFree(void *block);
s32 Inventory_EquipFar(s32 owner, s32 item);
void UiText_DrawCharacterAtOffsetFar(s32 message, s32 window, s32 x, s32 y);
void UiWindow_ClearInteriorTilesFar(s32 window, s32 x, s32 y, s32 width, s32 height);
void UiMenu_SlideCursor(s32 x, s32 y);
void UiMenu_PositionCursor(s32 x, s32 y);
void Audio_PlayCue(s32 cue);
s32 GameFlag_TestFar(s32 flag);
s32 Math_Mod(s32 numerator, s32 denominator);
void Owner_RecalculateStatsFar(s32 owner);
void Func_080772c0(s32 owner);

s32 Func_080a5388(void)
{
    s32 selection = 0;
    s32 changed = 1;
    struct EquipMenu *menu = *(struct EquipMenu **)0x03001f2c;
    void *state = Owner_GetStateFar(menu->owner);
    void *backup;
    s32 window;

    ItemMenu_DrawEquipPreview(menu->owner, menu->item, 0, menu->owner);
    backup = Runtime_BumpAllocate(0x14c);
    CopyWords((WordCopyFn)0x03001388, backup, state, 0x14c);
    window = menu->selector_window;
    if ((u32)(Inventory_EquipFar(menu->owner, menu->item) + 2) > 1) {
        UiText_DrawCharacterAtOffsetFar((s32)&Value_00000b2c, window, 24, 24);
        UiText_DrawCharacterAtOffsetFar((s32)&Value_00000b2c + 1, window, 72, 24);
        UiWindow_ClearInteriorTilesFar(window, 16, 16, 96, 24);
        UiText_DrawCharacterAtOffsetFar(0xad6, window, 0, 16);
        UiMenu_SlideCursor(110, 32);
        while (!GameFlag_TestFar(0x150)) {
            if (changed) {
                changed = 0;
                selection = Math_Mod(selection + 2, 2);
            }
            if (*(volatile u32 *)0x03001c94 & 1) {
                Audio_PlayCue(175);
                goto done;
            }
            if (*(volatile u32 *)0x03001c94 & 2) {
                Audio_PlayCue(113);
                break;
            }
            UiMenu_PositionCursor(selection * 48 + 110, 32);
            {
                volatile u32 *repeat = (volatile u32 *)0x03001b04;

                if (*repeat & 32) {
                    selection--;
                    changed = 1;
                    Audio_PlayCue(111);
                }
                if (*repeat & 16) {
                    selection++;
                    changed = 1;
                    Audio_PlayCue(111);
                }
            }
            WaitFrames(1);
        }
        goto done;
    }
    selection = 1;
done:
    if (GameFlag_TestFar(0x150))
        selection = 1;
    if (selection == 1)
        CopyWords((WordCopyFn)0x03001388, state, backup, 0x14c);
    Runtime_BumpFree(backup);
    Owner_RecalculateStatsFar(menu->owner);
    Func_080772c0(menu->owner);
    return selection;
}
