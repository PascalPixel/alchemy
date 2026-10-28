#include "types.h"
#include "ram_buffer.h"

extern u8 MsgReviveService[];
extern u8 MsgCurePoisonService[];
extern u8 MsgExorcismService[];
extern u8 MsgRemoveCurseService[];

s32 Shop_MsgByMode(s32 value) {
  s8 *shop;
  s8 mode;

  /* FAKEMATCH: the one-pass block ends the scheduling block: the native loads
   * the shop work before it builds the offset of the service kind (one
   * reordered row, differ 2 without). */
  do {
    shop = Ram_HeapSlots->menuRuntime;
  } while (0);
  mode = shop[0x507];
  if (mode == 1) {
    value += (u32)MsgCurePoisonService - (u32)MsgReviveService;
  }
  if (mode == 2) {
    value += (u32)MsgExorcismService - (u32)MsgReviveService;
  }
  if (mode == 3) {
    value += (u32)MsgRemoveCurseService - (u32)MsgReviveService;
  }
  return value;
}

s32 BattleEffect_GetResourceIdFar(u16 id);
void Message_CloseFinishedWindowsFar(void);
void Message_OpenWindowFar(s32 message, s32 a, s32 b, s32 c);
s32 Message_AllTypedFar(void);
void WaitFrames(s32 frames);

/// Shows the service message, its wording for the shop's mode, with the
/// keeper's portrait, and waits until it has been read.
// The Broken Seal's Message_ShowResolvedAndWait.
void Func_0810a9ac(s32 value) {
  u8 *shop;
  s32 portrait;
  /* FAKEMATCH: the barrier loads the shop work before the keeper's offset, and
   * the message pinned to r6 is copied below it; without both, 2 instructions
   * differ in tla-en. */
  register s32 message asm("r6");

  shop = Ram_HeapSlots->menuRuntime;
  asm volatile("");
  message = value;
  portrait = BattleEffect_GetResourceIdFar(*(u16 *)(shop + 0x4fa));
  Message_CloseFinishedWindowsFar();
  message = Shop_MsgByMode(message);
  Message_OpenWindowFar(message, 5, 0, (portrait << 16) | 0x22);
  while (Message_AllTypedFar() == 0)
    WaitFrames(1);
  WaitFrames(1);
}

struct ShopRuntime {
  u8 unk000[0x4dc];
  u8 *cursorAnchor;
  u8 unk4e0[0x1a];
  u16 keeperResource;
};

/// As Func_0810a9ac, with the cursor's anchor object set to state 13 while the
/// message shows and restored after.
// The Broken Seal's Message_ShowResolvedAndRestoreState.
void Func_0810a9fc(s32 message) {
  struct ShopRuntime *shop;
  u8 **anchor;
  s32 portrait;
  u8 saved;

  shop = Ram_HeapSlots->menuRuntime;
  anchor = &shop->cursorAnchor;
  saved = (*anchor)[5];
  portrait = BattleEffect_GetResourceIdFar(shop->keeperResource);
  message = Shop_MsgByMode(message);
  (*anchor)[5] = 13;
  Message_CloseFinishedWindowsFar();
  Message_OpenWindowFar(message, 5, 0, (portrait << 16) | 0x22);
  while (Message_AllTypedFar() == 0)
    WaitFrames(1);
  WaitFrames(1);
  shop->cursorAnchor[5] = saved;
}
