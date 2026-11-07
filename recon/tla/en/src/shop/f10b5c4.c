#include "types.h"
#include "ram_buffer.h"
#include "render_input.h"

struct ShopCursor {
  struct WindowSprite *anchor;
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
struct FieldObject *Actor_GetByIdFar(s32 objectId);
void *Func_080380f8(s32 portrait, s32 a, s32 b, s32 c);
void Func_08108aa8(struct ShopCursor *cursor, s32 x, s32 y);
void Func_081084f4(s32 message);
s32 Func_0810b6bc(void);
void Window_CloseFar(void *window, s32 mode);

s32 Func_0810b5c4(s32 objectId) {
  struct ShopRuntime *shop;
  struct WindowSprite *icon;
  void *window;
  s32 message;
  s32 result;

  Func_08108148();
  shop = Ram_HeapSlots->menuRuntime;
  shop->open = 1;
  shop->partyAction = 4;
  shop->portrait = *Actor_GetByIdFar(objectId)->sprite->portrait;
  window = Func_080380f8(shop->portrait, 0, 0, 0);
  icon = WindowSprite_CreateFar(shop->cursorIcon, 0x40000000, window, 0, 0);
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
  Window_CloseFar(window, 2);
  Func_0810824c();
  return 0;
}
