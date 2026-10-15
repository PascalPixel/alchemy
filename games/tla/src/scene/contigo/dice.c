#include "types.h"
#include "ram_buffer.h"
#include "party_state.h"
#include "scene_ids.h"
#include "text/msg_ids.h"

struct ContigoEventCount {
  u8 unk000[0x1c4];
  u16 refusals;
};

extern u8 gSceneState[];

void GameFlag_SetBit(s32 flag);
void WaitFrames(s32 frames);
s32 UiText_OpenMessageAtObject(s32 speaker, s32 flags);
s32 Inventory_PromptAndSetObjectMode(s32 actor, s32 mode);
void Field_SetDestination(s32 scene, s32 entrance);
void Party_SetFields1eeAnd1f0(s32 scene, s32 entrance);
void Func_0200403c(s32 value, s32 digits);
u32 Func_0200404c(void);
void Func_0200406c(void);
void Func_02004074(void);
void Func_0200412c(s32 message);
void Func_02004144(s32 speaker, s32 flags);
void Func_02004204(s32 skip);
void Func_0200422c(void);

// The dice game attendant: the fee is ten coins a level.
void Func_02001abc(void) {
  u32 price = Func_0200404c() * 10;

  Func_0200406c();
  Func_02004204(0);
  if ((u32)gPartyState.coins < price) {
    Func_0200412c((s32)MsgContigo03);
    UiText_OpenMessageAtObject(14, 0);
    return;
  }
  Func_0200412c((s32)MsgContigo01);
  Func_0200403c(price, 5);
  UiText_OpenMessageAtObject(14, 0);
  if (Inventory_PromptAndSetObjectMode(gPartyState.currentOwner, 0) == 0) {
    /* FAKEMATCH: the native loads the scene state's address into r3 before the
     * coins; stored plainly the two registers swap (6 differ), and with the
     * coins in a temporary the two loads come in the other order (2 differ);
     * pointer locals and one-pass blocks measured 2 or 6. */
    register u32 *state __asm__("r3") = (u32 *)gSceneState;

    *state = gPartyState.coins;
    Func_0200422c();
    Func_02004144(14, 0);
    GameFlag_SetBit(0x109);
    GameFlag_SetBit(0x9e5);
    WaitFrames(1);
    Field_SetDestination(0x1fd, 0);
    Party_SetFields1eeAnd1f0((s32)&SceneId_Ov694Room3, 19);
  } else {
    ((struct ContigoEventCount *)Ram_HeapSlots->eventWork)->refusals++;
    Func_02004144(14, 0);
  }
  Func_02004074();
}

// The Super Lucky Dice attendant: the same check without the fee shown.
void Func_02001b84(void) {
  u32 price = Func_0200404c() * 10;

  Func_0200406c();
  Func_02004204(0);
  if ((u32)gPartyState.coins < price) {
    Func_0200412c((s32)MsgContigo34);
    UiText_OpenMessageAtObject(14, 0);
    return;
  }
  Func_0200412c((s32)MsgContigo23);
  UiText_OpenMessageAtObject(8, 0);
  if (Inventory_PromptAndSetObjectMode(gPartyState.currentOwner, 0) == 0) {
    /* FAKEMATCH: the native loads the scene state's address into r3 before the
     * coins; stored plainly the two registers swap (6 differ), and with the
     * coins in a temporary the two loads come in the other order (2 differ);
     * pointer locals and one-pass blocks measured 2 or 6. */
    register u32 *state __asm__("r3") = (u32 *)gSceneState;

    *state = gPartyState.coins;
    Func_0200422c();
    Func_02004144(8, 0);
    GameFlag_SetBit(0x109);
    GameFlag_SetBit(0x9e5);
    WaitFrames(1);
    Field_SetDestination(0x1fb, 0);
    Party_SetFields1eeAnd1f0((s32)&SceneId_Ov694Room3, 18);
  } else {
    ((struct ContigoEventCount *)Ram_HeapSlots->eventWork)->refusals++;
    Func_02004144(8, 0);
  }
  Func_02004074();
}
