/* Near miss: differ 2 of 67 (one reordered row). The native loads gPartyState's address before it
 * starts the owner offset (ldr r3, =gPartyState; movs r2, #133); the draft starts the offset first.
 * The address in a local with an empty asm barrier, a "+l" barrier or a one-pass block orders that pair
 * but swaps the second message call's arguments (adds r0, r5, #0; movs r1, #1), differ 2 again.
 * Message 3599 ("... found ...", English numbering) has no name in the catalogs yet: MsgFoundItem here
 * is undefined, and the other editions number it differently. */
#include "types.h"
#include "party_state.h"
#include "object_runtime.h"

extern u8 MsgFoundItem[];

struct ObjectRuntime *Object_GetById(s32 id);
s32 PartyInventory_FindRoomFar(s32 item);
void Func_080cf424(s32 item, s32 mode);
void UiText_DrawQuantity(s32 value, s32 slot);
void UiText_ShowPositionedMessageAndWaitFar(s32 message, s32 mode);
void ObjectMotion_SetAngleToward(s32 actor, s32 target, s32 frames);
void EventRuntime_Wait(s32 frames);
void ObjectMotion_SetHorizontalPositionWithTerrain(s32 actor, s32 x, s32 z);
s32 PartyInventory_GiveItem(s32 item, s32 unused);
void GameFlag_SetBit(s32 flag);

s32 Func_080d296c(s32 actor, s32 item, s32 flag)
{
    if (PartyInventory_FindRoomFar(item) < 0) {
        Object_GetById(actor)->scale_x = 0;
        Func_080cf424(item, 3);
        UiText_DrawQuantity(item, 2);
        UiText_ShowPositionedMessageAndWaitFar((s32)MsgFoundItem, 1);
        UiText_ShowPositionedMessageAndWaitFar((s32)MsgFoundItem + 4, 1);
        Object_GetById(actor)->scale_x = 0x10000;
        ObjectMotion_SetAngleToward(gPartyState.current_owner, actor, 0);
        EventRuntime_Wait(20);
        return -1;
    }
    ObjectMotion_SetHorizontalPositionWithTerrain(actor, 0, 0);
    Func_080cf424(item, 3);
    PartyInventory_GiveItem(item, 0);
    if (flag != -1)
        GameFlag_SetBit(flag);
    return 0;
}
