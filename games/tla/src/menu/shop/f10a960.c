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

s32 BattleFx_GetResourceIdFar(u16 id);
void UiWork_FinalizePendingCoreFar(void);
void UiText_OpenMessageWindowFar(s32 message, s32 a, s32 b, s32 c);
s32 UiWork_IsCompleteFar(void);
void WaitFrames(s32 frames);

// Shows the service message, its wording for the shop's mode, with the
// keeper's portrait, and waits until it has been read. The Broken Seal's
// UiMessage_ShowResolvedAndWait.
void Func_0810a9ac(s32 value) {
  u8 *shop;
  s32 portrait;
  /* FAKEMATCH: the barrier keeps the shop work's load ahead of the keeper's
   * offset 0x4fa, and the message pinned to r6 keeps its copy from the
   * parameter below the barrier, as native; without both the offset starts
   * first (differ 2), and the barrier alone lifts the copy to the top (differ
   * 2). */
  register s32 message asm("r6");

  shop = Ram_HeapSlots->menuRuntime;
  asm volatile("");
  message = value;
  portrait = BattleFx_GetResourceIdFar(*(u16 *)(shop + 0x4fa));
  UiWork_FinalizePendingCoreFar();
  message = Shop_MsgByMode(message);
  UiText_OpenMessageWindowFar(message, 5, 0, (portrait << 16) | 0x22);
  while (UiWork_IsCompleteFar() == 0)
    WaitFrames(1);
  WaitFrames(1);
}

struct ShopRuntime {
  u8 unk000[0x4dc];
  u8 *cursorAnchor;
  u8 unk4e0[0x1a];
  u16 keeperResource;
};

// As Func_0810a9ac, with the cursor's anchor object set to state 13 while the
// message shows and restored after. The Broken Seal's
// UiMessage_ShowResolvedAndRestoreState.
void Func_0810a9fc(s32 message) {
  struct ShopRuntime *shop;
  u8 **anchor;
  s32 portrait;
  u8 saved;

  shop = Ram_HeapSlots->menuRuntime;
  anchor = &shop->cursorAnchor;
  saved = (*anchor)[5];
  portrait = BattleFx_GetResourceIdFar(shop->keeperResource);
  message = Shop_MsgByMode(message);
  (*anchor)[5] = 13;
  UiWork_FinalizePendingCoreFar();
  UiText_OpenMessageWindowFar(message, 5, 0, (portrait << 16) | 0x22);
  while (UiWork_IsCompleteFar() == 0)
    WaitFrames(1);
  WaitFrames(1);
  shop->cursorAnchor[5] = saved;
}
