// Measured 2026-10-26 under the Makefile's options (-mthumb-slow-loads), differ
// 8 of 107 in tla-en. First difference: this issues `ldr r1, [pc, #56]` earlier
// (row 40) than the native does (row 43). No single empty asm or one-pass block
// reaches 0.
// Draft, differ 8 of 107 (2026-10-19; was 10): the forge's counter: opens the
// shop menu in forge mode, shows the smith's portrait from the speaking
// object's sprite and the cursor, asks for a material (Func_0810b6bc) and says
// goodbye on cancel. No ☀️ twin. Left: instruction order only: the first
// constant's movs ahead of the menu work's load, the pooled zero (the icon
// kind, a QImode constant that survives to a register, as here) ahead of the
// portrait store, and the argument zeros, each one slot from the native; the
// pool branch then lands one row earlier. The work's barrier with an r1 clobber
// (as Shop_CountUnits's) fixes the first movs; left are the pooled zero and the
// argument zeros around the portrait's reload, as in 0810aa80 and 0810b418.
#include "types.h"
#include "ram_buffer.h"
#include "render_input.h"

struct ShopCursor {
  struct RenderOutput *anchor;
  u16 x;
  u16 y;
  s16 targetX;
  s16 targetY;
  s8 active;
  s8 kind;
};

struct ShopRuntime {
  u8 unk000[0x4dc];
  struct ShopCursor cursor;
  u16 cursorIcon;
  u8 unk4ee[0xc];
  u16 portrait;
  u8 unk4fc[0xa];
  u8 open;
  s8 partyAction;
};

struct ObjectSpriteInfo {
  u8 unk00[0x28];
  u16 *portrait;
};

struct FieldObject {
  u8 unk00[0x50];
  struct ObjectSpriteInfo *sprite;
};

extern u8 MsgRightWhenI[];

void Func_08108148(void);
void Func_0810824c(void);
struct FieldObject *Object_GetByIdFar(s32 objectId);
void *Func_080380f8(s32 portrait, s32 a, s32 b, s32 c);
void Func_08108aa8(struct ShopCursor *cursor, s32 x, s32 y);
void Func_081084f4(s32 message);
s32 Func_0810b6bc(void);
void UiWork_FinalizeFar(void *window, s32 mode);

s32 Func_0810b5c4(s32 objectId) {
  struct ShopRuntime *shop;
  struct RenderOutput *icon;
  void *window;
  s32 message;
  s32 result;

  Func_08108148();
  shop = Ram_HeapSlots->menuRuntime;
  asm("" : "+l"(shop) : : "r1");
  shop->open = 1;
  shop->partyAction = 4;
  shop->portrait = *Object_GetByIdFar(objectId)->sprite->portrait;
  window = Func_080380f8(shop->portrait, 0, 0, 0);
  icon = RenderOutput_CreateFar(shop->cursorIcon, 0x40000000, window, 0, 0);
  icon->sentinel = 255;
  icon->active = 1;
  icon->kind = 0;
  Func_08108aa8(&shop->cursor, -32, 112);
  shop->cursor.anchor = icon;
  message = (s32)MsgRightWhenI;
  Func_081084f4(message);
  Func_081084f4(message + 1);
  result = Func_0810b6bc();
  Func_08108aa8(&shop->cursor, -32, 112);
  if (result == -1)
    Func_081084f4(message + 2);
  UiWork_FinalizeFar(window, 2);
  Func_0810824c();
  return 0;
}
