#include "battle.h"
#include "field.h"
#include "game.h"
#include "graphics.h"
#include "lib.h"
#include "menu.h"
#include "sound.h"
#include "system.h"
#include "types.h"
#include "input.h"
#include "party_state.h"
#include "scene_ids.h"
#include "serial_runtime.h"
#include "text/msg_ids.h"

s32 Func_02000f6c(s32 id);
s32 Func_02000fbc(s32 actor);
void Func_0200191c(void);
void Func_0200197c(void);
void Func_02001984(s32 srcX, s32 srcY, s32 destX, s32 destY, s32 width,
                   s32 height);
void Func_020019e4(s32 value, s32 digits);
void Func_020019ec(void);
void Func_02001a14(void);
void Func_02001a1c(void);
void Func_02001a64(s32 message);
void Func_02001a74(s32 actor, s32 flags);
void Func_02001a7c(s32 actor, s32 animation, s32 flags);
s32 Func_02001a84(s32 scene, s32 entrance);
void Func_02001ab4(s32 skip);
void ObjectMotion_SetAngleToward(s32 actor, s32 target, s32 frames);
void UiText_ShowPositionedMessageAndWait(const u8 *message, s32 flags);

static __inline__ s32 Flag_Test(s32 flag) {
  /* FAKEMATCH: forwarded inline; see scene_call.h. */
  return GameFlag_Test(flag);
}

s32 Func_02001130(s32 actor) {
  s32 base = (s32)MsgItSOur;
  s32 lead = Func_02000f6c(4);
  s32 own = Func_02000f6c(actor);

  Func_02001a14();
  Func_02001ab4(0);
  ObjectMotion_SetAngleToward(actor, gPartyState.currentOwner, 0);
  if (GameFlag_Test(0x304)) {
    Flag_Test(0x2f4);
    base = GameFlag_Test(actor + 0x2f0);
    if (GameFlag_Test(0x305)) {
      if (base)
        base = (s32)MsgEverybodyDeservesA;
      else
        base = (s32)MsgEverybodyDidGreat;
    } else {
      if (base)
        base = (s32)MsgDonTBlame;
      else
        base = (s32)MsgYouWinSome;
    }
  } else if (lead != 0) {
    if (own == 0)
      base = (s32)MsgOkWeRe;
  } else {
    base = (s32)MsgILlFight;
  }
  Func_02001a64(base + actor);
  Func_02001a74(actor, 0);
  if (base == (s32)MsgILlFight && actor == 5) {
    Func_02001a7c(5, 263, 0);
    FieldEvent_WaitTicks(10);
  }
  Func_02001a1c();
}

s32 Func_02001210(void) {
  Func_0200191c();
  Sound_LoadPresetParameters(2);
  /* FAKEMATCH: the do/while loads the scene number before the 1. */
  do {
    return Func_02001a84((s32)&SceneId_Ov64a, 1);
  } while (0);
}

s32 Func_0200122c(s32 actor) {
  s32 message;

  if (actor == 14 && (gInput.held & 0x300)) {
    Func_02000fbc(14);
    return;
  }
  Func_02001a14();
  Func_02001ab4(0);
  if (GameFlag_Test(0x206)) {
    switch (actor) {
    case 13:
      message = (s32)MsgILlBe;
      break;
    case 14:
      message = (s32)MsgILikeMy;
      break;
    default:
      message = (s32)MsgSometimesYourClass;
      break;
    }
  } else {
    switch (actor) {
    case 13:
      message = (s32)MsgYouLookA;
      break;
    case 14:
      message = (s32)MsgILikeMy;
      break;
    default:
      message = (s32)MsgSometimesYourClass;
      break;
    }
    GameFlag_SetBit(0x206);
  }
  Func_02001a64(message);
  UiText_OpenMessageAtObject(actor, 0);
  FieldEvent_WaitTicks(1);
  if (message == (s32)MsgYouLookA) {
    ObjectMotion_SetVariantCallback(14, 1);
    Object_SetModeById(14, 4);
    FieldEvent_WaitTicks(15);
    ObjectMotion_SetAngleToward(14, gPartyState.currentOwner, 0);
    FieldEvent_WaitTicks(5);
    UiText_OpenMessageAtObject(14, 0);
  }
  Func_02001a1c();
}

s32 Func_02001304(s32 actor) {
  s32 message;

  Func_02001a14();
  Func_02001ab4(0);
  ObjectMotion_SetAngleToward(actor, gPartyState.currentOwner, 0);
  if (!Flag_Test(0x204)) {
    if (Party_CountActiveOwners() == 8)
      message = (s32)MsgUpToThree;
    else if (Party_CountActiveOwners() <= 3)
      message = (s32)MsgUpToThreeAllies;
    else
      message = (s32)MsgUpToThreeAlliesCan;
    GameFlag_SetBit(0x204);
  } else {
    message = (s32)MsgYouCanChange;
    GameFlag_ClearBit(0x204);
  }
  Func_02001a64(message);
  UiText_OpenMessageAtObject(actor, 0);
  Func_02001a1c();
}

s32 Func_02001388(s32 id) {
  struct PartyState *gs;

  Func_02001a14();
  Func_02001ab4(0);
  gs = &gPartyState;
  ObjectMotion_SetAngleToward(id, gs->currentOwner, 0);
  if (gs->linkTallies[2] != 0) {
    Func_020019e4(gs->linkTallies[2], 5);
    Func_02001a64((s32)MsgYouVeWon);
  } else {
    Func_02001a64((s32)MsgICanTWaitTo);
  }
  UiText_OpenMessageAtObject(id, 0);
  Func_02001a1c();
}
