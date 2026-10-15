#include "types.h"
#include "ram_buffer.h"
#include "party_state.h"
#include "text/msg_ids.h"

void Func_020002b8(void);
void Func_020002c0(void);
void Func_02000300(s32 message);
void Func_02000310(s32 actor, s32 a, s32 b);
void Func_02000318(s32 actor, s32 a);
void Func_02000338(void);
void Func_02000340(s32 skip);
s32 UiText_OpenMessageAtObject(s32 speaker, s32 flags);
s32 Inventory_PromptAndSetObjectMode(s32 actor, s32 flags);
void Motion_SetModeAndWaitAnimation(s32 actor, s32 mode);
void Motion_CamBounds(s32 x, s32 y, s32 z, s32 mode);
void GameFlag_SetBit(s32 flag);

void Func_02000054(void) {
  Func_020002b8();
  Func_02000340(0);
  Func_02000300((s32)MsgMadraGoingToOsenia);
  UiText_OpenMessageAtObject(8, 0);
  if (Inventory_PromptAndSetObjectMode(gPartyState.currentOwner, 0) == 0) {
    Motion_SetModeAndWaitAnimation(8, 3);
    Func_02000310(8, 0, 2);
  } else {
    (*(u16 *)((u8 *)Ram_HeapSlots->eventWork + 0x1c4))++;
    Func_02000310(8, 0, 2);
  }
  Func_020002c0();
}
