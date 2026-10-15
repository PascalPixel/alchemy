/* Draft, differ 2 of 45 (2026-10-15), ported from The Broken Seal's Shop_DrawSelectMsg: the native
 * loads the menu work before it starts the 0x507 offset (ldr r3, [r3]; movs r2, #129); here the
 * offset starts first. A one-pass block or a barrier after the load also keeps the two parameter
 * copies above it (differ 6), where the native moves them below the party_action load. */
#include "types.h"
#include "ram_buffer.h"
#include "text/msg_ids.h"

struct ShopRuntime {
    u8 unknown_000[0x507];
    s8 party_action;
};

s32 Shop_ServicePrice(s32 selection, s32 variant);
s32 Shop_CanServe(s32 entry_id, s32 kind);
s32 Shop_MsgByMode(s32 value);
void RenderOutput_PrepareForRedrawFar(s32 window);
void UiText_DrawQuantity(s32 value, s32 slot);
void UiText_DrawResourceFar(s32 message, s32 window, s32 x, s32 y);

void Func_0810b04c(s32 target, s32 selection)
{
    s32 variant;
    s32 message;

    variant = ((struct ShopRuntime *)Ram_HeapSlots->menu_runtime)->party_action;
    message = Shop_ServicePrice(selection, variant);
    if (target != 0) {
        RenderOutput_PrepareForRedrawFar(target);
        if (Shop_CanServe(selection, variant) != 0)
            variant = (s32)MsgReviveCost;
        else
            variant = (s32)MsgNoHealingNeeded;
        variant = Shop_MsgByMode(variant);
        UiText_DrawQuantity(message, 5);
        UiText_DrawResourceFar(variant, target, 0, 0);
    }
}
