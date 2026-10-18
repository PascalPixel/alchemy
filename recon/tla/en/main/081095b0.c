// First draft of Func_081095b0: shows how many of an item the member already
// carries ("You have"/none) and draws the member's inventory icons through
// Func_0810bf98 at (8, 8); the twin of ☀️'s Shop_DrawUnitItem
// (games/tbs/src/shop/shop.c), whose icon loop ⚓️ moved into that helper.
// The messages take the twin's names. Differ 0 of 47 with the twin's
// quantityOffset local (10 with the offset inside the read).
#include "types.h"
#include "render_input.h"

extern u8 MsgYouHave[], MsgShopNoneInStock[];

void *UnitGet(s32 unit);
void RenderOutput_PrepareForRedrawFar(struct RenderInput *window);
s32 Inventory_FindFar(s32 unit, s32 item);
void UiText_DrawQuantity(s32 value, s32 digits);
void UiText_DrawCharacterAtOffsetFar(s32 message, struct RenderInput *window,
                                     s32 x, s32 y);
void Func_0810bf98(struct RenderInput *window, s32 x, s32 y, s32 unit);

void Func_081095b0(struct RenderInput *window, s32 unitId, s32 itemId) {
  u8 *unitBytes;
  s32 slot;
  s32 quantityOffset;

  unitBytes = (u8 *)UnitGet(unitId);
  if (window != 0) {
    RenderOutput_PrepareForRedrawFar(window);
    slot = Inventory_FindFar(unitId, itemId);
    if (slot != -1) {
      quantityOffset = slot * 2 + 216;
      UiText_DrawQuantity((*(u16 *)(unitBytes + quantityOffset) >> 11) + 1, 5);
      UiText_DrawCharacterAtOffsetFar((s32)MsgYouHave, window, 0, 0);
    } else {
      UiText_DrawCharacterAtOffsetFar((s32)MsgShopNoneInStock, window, 0, 0);
    }
    Func_0810bf98(window, 8, 8, unitId);
  }
}
