/* Near miss, differ 43 of 184: the twin of The Broken Seal's PsynergyMenu_SelectOwner.
   The native keeps the loop in source order (the page block, the A test, the B test,
   the cursor call, b to the top); here jump.c moves the B test and the cursor call
   above the loop's top and enters with a jump. Written with goto the order is right
   but loop.c no longer hoists the 0x204 offset and the list address (differ 73).
   The native also copies the hoisted offset (mov r6, r8) where this reuses r8.
   MsgChooseCharacter is message 4406 in English; it needs its name in the six
   catalogs before this links (ja 4403, de 4870, es/fr/it 4866 by the text of The
   Broken Seal's MsgChooseCharacter; unverified against those ROMs). */
#include "types.h"
#include "ram_buffer.h"
#include "input.h"

struct OwnerPageState {
    u8 unknown_000[8];
    s32 selected_owner;
    u8 unknown_00c[4];
    void *owner_window;
    u8 unknown_014[8];
    s8 owner_index;
    u8 unknown_01d;
    s8 owner_count;
    u8 unknown_01f[0xd1];
    void *message_window;
    u8 unknown_0f4[0xd0];
    u16 psynergies[32];
    u16 owner_ids[8];
    u8 psynergy_count;
    u8 unknown_215;
    u8 selected_ids[6];
    u16 page;
};

extern u8 MsgChooseCharacter[];

void *UnitGet(s32 owner);
void RenderOutput_RedrawSavedRectFar(void *window);
void UiText_DrawCharacterAtOffsetFar(s32 message, void *window, s32 x, s32 y);
void Func_080fee40(s32 owner, s32 page);
void PsynergyMenu_CallIconRoutineWithValue(void *menu, s32 owner);
s32 Func_080fd6f0(void *unit, u16 *list, s32 mode);
void Func_080fd6b0(u16 *list);
void Func_081005e4(s32 x, s32 y, s32 columns);
void Func_080facd8(u16 *list);
void Func_08104ef8(s32 first);
void Func_08104d5c(void *window, s32 selection, s32 count);
void Func_080f8a44(s32 x, s32 y);
s32 Func_08104c00(s32 *selection, s32 count, s32 columns);
void WaitFrames(s32 frames);
void Audio_PlayCue(s32 cue);

s32 Func_080feb58(void)
{
    struct OwnerPageState *menu = (struct OwnerPageState *)Ram_HeapSlots->menu_runtime;
    s32 selection = menu->owner_index;
    s32 count = menu->owner_count;
    s32 pending = 2;
    s32 page = menu->page;
    s32 result;

    UnitGet(menu->owner_ids[selection]);
    RenderOutput_RedrawSavedRectFar(menu->message_window);
    UiText_DrawCharacterAtOffsetFar((s32)MsgChooseCharacter, menu->message_window, 0, 0);
    UiText_DrawCharacterAtOffsetFar((s32)MsgChooseCharacter + 1, menu->message_window, 0, 16);
    for (;;) {
        /* PERM */
        if (pending) {
            selection = (selection + count) % count;
            UnitGet(menu->owner_ids[selection]);
            page = (page + 3) % 3;
            Func_080fee40(menu->owner_ids[selection], page);
            PsynergyMenu_CallIconRoutineWithValue(menu, menu->owner_ids[selection]);
            menu->psynergy_count = Func_080fd6f0(UnitGet(menu->owner_ids[selection]), menu->psynergies, 0);
            Func_080fd6b0(menu->psynergies);
            Func_081005e4(96, 96, 8);
            Func_080facd8(menu->psynergies);
            if (pending == 2) {
                Func_08104ef8(selection / 4 * 4);
                WaitFrames(1);
            }
            Func_08104d5c(menu->owner_window, selection, count);
        }
        Func_080f8a44((selection % 4) * 24 - 10, 16);
        WaitFrames(1);
        if (((volatile struct InputState *)&gInput)->pressed & 1) {
            Audio_PlayCue(112);
            result = 1;
            break;
        }
        if (((volatile struct InputState *)&gInput)->pressed & 2) {
            Audio_PlayCue(113);
            result = -1;
            break;
        }
        pending = Func_08104c00(&selection, count, 4);
    }
    menu->owner_index = selection;
    menu->selected_owner = menu->owner_ids[selection];
    menu->selected_ids[0] = menu->owner_ids[selection];
    return result;
}
