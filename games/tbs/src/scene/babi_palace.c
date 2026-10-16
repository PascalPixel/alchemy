#include "game_state.h"
#include "keystate.h"
#include "scene_ids.h"
#include "unit/import.h"
#include "unit/scene/babi_palace.h"
#include "unit/object/object.h"
#include "unit/object/psynergy_effects.h"
#include "unit/field/field.h"
#include "unit/game/game.h"
#include "unit/shop/shop.h"
#include "battle.h"
#include "debug.h"
#include "field.h"
#include "game.h"
#include "graphics.h"
#include "menu.h"
#include "minigame.h"
#include "sound.h"
#include "system.h"
#include "types.h"
#include "io_reg.h"
#include "field_service.h"
#include "field_scene.h"
#include "babi_palace.h"
#include "text/msg_ids.h"
#include "ability_ids.h"
#include "field_event.h"
#include "item_ids.h"
#include "objcmd.h"
#include "global_cells.h"
#include "display_transition.h"

struct OwnerState {
  u8 unk00[0x34];
  u16 maxHp;
  u16 maxPp;
  u16 hp;
  u16 pp;
};

// Restores active owners' HP/PP, rejoins the companions and restores inventory.
static __inline__ void BabiPalace_RestorePartyAndInventory(void) {
  s16 owners[8];
  struct OwnerState *state;
  s32 ownerCount;
  s32 i;

  ownerCount = FieldParty_ListActiveOwners(owners);
  for (i = 0; i < ownerCount; i++) {
    state = UnitGet(owners[i]);
    state->hp = state->maxHp;
    state->pp = state->maxPp;
    Engine_OwnerRecalculateRatios(owners[i]);
  }
  Party_AddActiveOwner(GARET);
  Party_AddActiveOwner(IVAN);
  Party_AddActiveOwner(MIA);
  Engine_InventorySnapshotRestore();
}

// Selects the palace interior's entrance table.
const struct SceneEntrance *Scene_GetEntrances(void) {
  if (gGameState.scene == (s32)&SceneId_BabiPalace2) {
    return gBabiPalaceEntrances2;
  }
  return gBabiPalaceEntrancesOther;
}

// Returns the empty region table through the legacy encoded-pointer API.
s32 BabiPalace_GetRegions(void) { return (s32)gBabiPalace_EmptyTable; }

// Returns exits through the same encoded-pointer API.
s32 BabiPalace_GetExits(void) { return (s32)gBabiPalace_MessageTable; }

// Selects the palace's occupants before, during or after Colosso.
const struct ScenePlacement *Scene_GetPlacements(void) {
  if (gGameState.scene == (s32)&SceneId_BabiPalace2) {
    return gBabiPalacePlacements2;
  }
  if (Engine_GameFlagIsSet(0x950) != 0) {
    return gBabiPalacePlacementsAfterColosso;
  }
  if (Engine_GameFlagIsSet(0x962) != 0) {
    return gBabiPalacePlacementsColosso;
  }
  return gBabiPalacePlacementsOther;
}

// Questions in these conversations store the yes/no answers beside the prompt.
static inline void BabiPalace_ShowQuestionAnswer(s32 actorId, s32 question) {
  Engine_EventSetMessage(question);
  Engine_EventOpenMessage(actorId, 0);
  if (Engine_EventChooseYesNo(0, 0) == 0)
    Engine_EventSetMessage(question + 1);
  else
    Engine_EventSetMessage(question + 2);
  Engine_EventShowMessage(actorId, 0);
}

// Selects the adjacent answer to the question about Babi's soldiers.
static void BabiPalace_AskAboutSoldiers(s32 actorId) {
  BabiPalace_ShowQuestionAnswer(actorId, MsgTolbiRunBabisSoldiers);
}

// Moves the attendant aside for the beds, or returns the attendant afterward.
static void BabiPalace_MoveBedAttendant(s32 actorId) {

  struct FieldActor *leader;
  s16 roundedFacing;
  s32 facingOrMessage;
  s32 facingBits;

  leader = Actor_Get(ACTOR_PARTY_LEADER);
  roundedFacing = (leader->facing + 0x2000) & 0xc000;
  Event_Begin();
  Battle_ResetEffectCounter();
  if (GameFlag_IsSet(512) == 0) {
    GameFlag_Set(512);
    GameFlag_Clear(0x969);
    Event_SetMessage(MsgTolbiWeHaveJustEnoughExtra);
    Event_ShowMessage(actorId, 0);
    Event_Wait(10);
    facingBits = roundedFacing << 16;
    facingOrMessage = 0x4000;
    if (facingBits == (0x4000 << 16)) {
      Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 40, 104);
      Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_EAST, 0);
    }
    Actor_SetSpeed(actorId, 0x10000, 0x8000);
    Actor_WalkByAndWait(actorId, 0, -48);
    Actor_WalkByAndWait(actorId, 64, 0);
    Actor_FaceDirection(actorId, facingOrMessage, 0);
  } else {
    GameFlag_Clear(512);
    GameFlag_Set(0x969);
    Actor_FaceDirection(actorId, FACING_SOUTH, 0);
    Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 120, 96);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 0);
    Event_Wait(20);
    facingOrMessage = MsgTolbiUseFourBeds;
    Event_SetMessage(facingOrMessage);
    Event_OpenMessage(actorId, 0);
    if (Event_ChooseYesNo(0, 0) == 0) {
      Event_SetMessage(facingOrMessage + 1);
      Event_ShowMessage(actorId, 0);
    } else {
      Event_SetMessage(facingOrMessage + 2);
      Event_ShowMessage(actorId, 0);
    }
    Event_Wait(10);
    Actor_SetAnimationAndWait(actorId, 3);
    Event_Wait(20);
    Actor_WalkByAndWait(actorId, -64, 0);
    Actor_WalkByAndWait(actorId, 0, 48);
#if defined(TBS_EDITION_ES) || defined(TBS_EDITION_FR) ||                      \
    defined(TBS_EDITION_IT)
    Actor_SetPosition(actorId, 56 << 16, 120 << 16);
#endif
  }
  Event_End();
}

// Offers the beds once, then keeps their invitation available.
static void BabiPalace_OfferRest(s32 actorId) {
  s32 question;

  Event_Begin();
  Battle_ResetEffectCounter();
  if (GameFlag_IsSet(0x966) == 0) {
    GameFlag_Set(0x966);
    GameFlag_Set(0x967);
    Actor_FaceDirection(actorId, FACING_SOUTH, 0);
    Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 120, 96);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 0);
    Event_Wait(20);
    question = MsgTolbiCameRestBefore;
    Event_SetMessage(question);
    Event_OpenMessage(actorId, 0);
    if (Event_ChooseYesNo(0, 0) == 0) {
      Event_Wait(10);
      Event_SetMessage(question + 1);
    } else {
      Event_SetMessage(question + 2);
    }
    Event_ShowMessage(actorId, 0);
    Event_Wait(10);
    Actor_SetAnimationAndWait(actorId, 3);
    Event_Wait(20);
    Actor_SetSpeed(actorId, 0x10000, 0x8000);
    Actor_WalkByAndWait(actorId, -64, 0);
    Actor_WalkByAndWait(actorId, 0, 48);
  } else {
    Event_SetMessage(MsgTolbiTiredFeelFreeRest);
    Event_OpenMessage(actorId, 0);
  }
  Event_End();
}

// Chooses the answer and emote to the question about entering Colosso.
static void BabiPalace_AskAboutColosso(s32 actorId) {
  s32 question = MsgTolbiPlanningEnterColosso;

  Engine_EventSetMessage(question);
  Engine_EventOpenMessage(actorId, 0);
  if (Engine_EventChooseYesNo(0, 0) == 0) {
    Engine_EventWait(10);
    Engine_ActorShowEmote(actorId, 0x102, 0x28);
    Engine_EventSetMessage(question + 1);
  } else {
    Engine_EventWait(10);
    Engine_ActorShowEmote(actorId, 0x105, 0x28);
    Engine_EventSetMessage(question + 2);
  }
  Engine_EventShowMessage(actorId, 0);
}

// Animates the actor's indigestion while continuing the selected dialogue.
static void BabiPalace_ShowIndigestionDialogue(s32 actorId) {
  struct FieldActor *actor;
  const s32 *indigestionScript;

  actor = Actor_Get(actorId);
  Event_Begin();
  indigestionScript = gBabiPalace_MiddleActionScript;
  Actor_SetActionScript(actorId, indigestionScript);
  Event_SetMessage(MsgTolbiCantFightBecauseLittleIndigestion);
  Event_ShowMessage(actorId, 0);
  Actor_Stop(actorId);
  actor->scaleY = 0x10000;
  actor->scaleX = 0x10000;
  Event_Wait(30);
  Actor_RunRepeatedMotion(actorId, 2);
  Event_Wait(30);
  Actor_RunRepeatedMotion(actorId, 2);
  Event_Wait(60);
  Event_ShowMessage(actorId, 0);
  Event_Wait(20);
  Actor_ShowEmote(actorId, 258, 60);
  Actor_RunRepeatedMotion(actorId, 2);
  Event_Wait(30);
  Actor_RunRepeatedMotion(actorId, 2);
  Event_Wait(30);
  Actor_RunRepeatedMotion(actorId, 2);
  Event_Wait(30);
  Actor_SetActionScript(actorId, indigestionScript);
  Event_ShowMessage(actorId, 0);
  Actor_FaceDirection(actorId, FACING_NORTHEAST, 0);
  Event_Wait(10);
  actor->scaleY = 0x10000;
  actor->scaleX = 0x10000;
  Actor_SetActionScript(actorId, indigestionScript);
  Event_End();
}

// Cancels Psynergy once for the palace-escape conversation.
static void BabiPalace_ReadEscapeThoughts(s32 unused0, s32 actorId) {
  Event_Begin();
  Event_SetMessage(MsgTolbiEvenIfEscapedBabiPalace);
  Event_ShowMessage(actorId, 0);
  if (GameFlag_IsSet(0x968) == 0) {
    GameFlag_Set(0x968);
    Psynergy_Cancel();
    Event_Wait(50);
    Actor_ShowEmote(actorId, 0x100, 70);
    Actor_FaceActor(actorId, ACTOR_PARTY_LEADER, 40);
    Event_ShowMessage(actorId, 0);
    Event_Wait(30);
    Actor_SetAnimationAndWait(actorId, 4);
    Event_Wait(20);
    Event_ShowMessage(actorId, 0);
    Actor_FaceDirection(actorId, FACING_WEST, 0);
  }
  Event_End();
}

// Selects the adjacent answer to the question about meeting Babi.
static void BabiPalace_AskAboutMeetingBabi(s32 actorId) {
  BabiPalace_ShowQuestionAnswer(actorId, MsgTolbiMeetBabi);
}

// Asks about Karagol's eastern shores and selects the matching answer.
static void BabiPalace_AskAboutEasternShores(s32 actorId) {
  BabiPalace_ShowQuestionAnswer(actorId, MsgTolbiEasternShoresKaragol);
}

// Offers the Cloak Ball, granting it and setting its chest flag on acceptance.
static void BabiPalace_OfferCloakBall(void) {
  s32 msg;

  Engine_EventBegin();
  Battle_ResetEffectCounter();
  msg = MsgTolbiFoundCloakBall;
  Engine_EventSetMessage(msg);
  Engine_EventShowMessage(-1, 0);
  Engine_EventWait(10);
  Engine_ActorRunRepeatedMotion(14, 2);
  Engine_EventWait(30);
  Engine_ActorFaceActor(ACTOR_PARTY_LEADER, 14, 30);
  Engine_EventOpenMessage(14, 0);
  if (Engine_EventChooseYesNo(0, 0) != 0) {
    Engine_EventSetMessage(msg + 2);
    Engine_EventShowMessage(14, 0);
  } else {
    Engine_EventWait(20);
    Engine_EventSetMessage(msg + 3);
    Engine_EventShowMessage(14, 0);
    Engine_EventWait(10);
    Engine_ActorSetAnimationAndWait(ACTOR_PARTY_LEADER, 3);
    Engine_EventWait(30);
    Engine_ActorFaceDirection(ACTOR_PARTY_LEADER, FACING_SOUTH, 0);
    Engine_EventWait(30);
    Engine_ActorSetPosition(16, 0, 0);
    Engine_ItemShowFound(ITEM_CLOAK_BALL, 3);
    Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, 1);
    Engine_PartyGiveItem(ITEM_CLOAK_BALL, 0);
    Engine_GameFlagSet(0xf31);
  }
}

// Wakes the rested party, handles the choice dialogue and rejoins companions.
static void BabiPalace_RunPostRestWakeUp(void) {
  struct FieldActor *leader;

  Event_Begin();
  Event_SetMessage(MsgTolbiGetUp);
  Actor_Get(ACTOR_PARTY_LEADER)->active = 0;
  Actor_Get(10)->active = 0;
  Task_Wait(1);
  REG_DISPCNT = 0x1140;
  Event_ShowMessage(-1, 0);
  REG_DISPCNT = 0x140;
  Actor_Get(ACTOR_PARTY_LEADER)->active = 1;
  Actor_Get(10)->active = 1;
  Actor_SetAnimation(ACTOR_PARTY_LEADER, 31);
  leader = Actor_Get(ACTOR_PARTY_LEADER);
  Actor_SetSpriteFlags(leader, 0);
  Actor_SetPosition(GARET, 0x780000, 0x680000);
  Actor_SetPosition(MIA, 0x680000, 0x500000);
  Actor_SetPosition(IVAN, 0x780000, 0x780000);
  Actor_FaceDirection(GARET, FACING_EAST, 0);
  Actor_FaceDirection(MIA, FACING_EAST, 0);
  Actor_FaceDirection(IVAN, FACING_NORTHEAST, 0);
  gEventWork->transitionFrames = 60;
  Engine_EventOpenScreen();
  Engine_EventWaitForScreen();
  Event_Wait(20);
  gEventWork->transitionFrames = 24;
  Actor_SetSpeed(MIA, 0x10000, 0x8000);
  Actor_WalkByAndWait(MIA, 16, 0);
  Actor_FaceDirection(MIA, FACING_SOUTHEAST, 0);
  Event_Wait(20);
  Actor_RunRepeatedMotion(MIA, 2);
  Event_Wait(30);
  Event_ShowMessage(MIA, 0);
  Event_Wait(10);
  leader = Actor_Get(ACTOR_PARTY_LEADER);
  leader->z.fixed += -0x30000;
  leader = Actor_Get(ACTOR_PARTY_LEADER);
  leader->targetZ += -0x30000;
  Actor_SetAnimation(ACTOR_PARTY_LEADER, 32);
  Event_Wait(40);
  Actor_SetAnimationAndWait(ACTOR_PARTY_LEADER, 34);
  Event_Wait(30);
  Actor_SetAnimation(ACTOR_PARTY_LEADER, 33);
  Event_Wait(50);
  Actor_RunRepeatedMotion(GARET, 2);
  Event_Wait(30);
  Event_ShowMessage(GARET, 0);
  Event_Wait(10);
  Actor_ShowEmote(ACTOR_PARTY_LEADER, 0x105, 60);
  Event_Wait(20);
  Actor_ShowEmote(GARET, 0x102, 60);
  Event_ShowMessage(GARET, 0);
  Event_Wait(10);
  Actor_SetAnimationAndWait(GARET, 4);
  Event_Wait(20);
  Event_ShowMessage(GARET, 0);
  Event_Wait(10);
  Actor_ShowEmote(ACTOR_PARTY_LEADER, 0x102, 80);
  Actor_ShowEmote(IVAN, 0x106, 60);
  Actor_FaceDirection(IVAN, FACING_NORTH, 0);
  Event_Wait(30);
  Actor_SetAnimationAndWait(IVAN, 4);
  Event_Wait(20);
  Event_ShowMessage(IVAN, 0);
  Event_Wait(10);
  Actor_FaceDirection(GARET, FACING_SOUTH, 0);
  Event_Wait(30);
  Actor_RunRepeatedMotion(GARET, 2);
  Event_Wait(45);
  Actor_FaceDirection(GARET, FACING_EAST, 0);
  Actor_FaceDirection(IVAN, FACING_NORTHEAST, 0);
  Event_Wait(30);
  Event_OpenMessage(GARET, 0);
  if (Event_ChooseYesNo(-1, 0) != 0) {
    Event_Wait(10);
    Actor_SetAnimationAndWait(ACTOR_PARTY_LEADER, 34);
    Event_Wait(20);
    Actor_SetAnimationAndWait(GARET, 3);
    Event_Wait(20);
    Event_ShowMessage(GARET, 0);
    Event_Wait(10);
    Actor_SetAnimationAndWait(ACTOR_PARTY_LEADER, 33);
    Event_Wait(30);
    Actor_SetAnimationAndWait(GARET, 3);
    Event_Wait(20);
    Event_ShowMessage(GARET, 0);
    gEventWork->message += 1;
  } else {
    Event_Wait(10);
    Actor_SetAnimationAndWait(ACTOR_PARTY_LEADER, 33);
    gEventWork->message += 2;
    Event_Wait(30);
    Actor_SetAnimationAndWait(GARET, 3);
    Event_Wait(20);
    Event_ShowMessage(GARET, 0);
  }
  Event_Wait(10);
  Actor_SetSpeed(GARET, 0x10000, 0x8000);
  Actor_WalkByAndWait(GARET, -16, 0);
  Actor_FaceDirection(GARET, FACING_EAST, 0);
  Event_Wait(35);
  Actor_Jump(ACTOR_PARTY_LEADER, 6, 0);
  Actor_SetSpeed(ACTOR_PARTY_LEADER, 0x1e666, 0xf333);
  Actor_WalkByAndWait(ACTOR_PARTY_LEADER, -32, 0);
  leader = Actor_Get(ACTOR_PARTY_LEADER);
  Actor_SetSpriteFlags(leader, 1);
  Actor_FaceDirection(MIA, FACING_SOUTH, 0);
  Actor_FaceDirection(IVAN, FACING_NORTH, 0);
  Event_Wait(40);
  Actor_SetAnimation(ACTOR_PARTY_LEADER, 3);
  Event_Wait(30);
  Actor_SetAnimation(IVAN, 3);
  Actor_SetAnimation(GARET, 3);
  Actor_SetAnimationAndWait(MIA, 3);
  Event_Wait(30);
  Actor_SetSpeed(GARET, 0x13333, 0x9999);
  Actor_SetSpeed(MIA, 0x13333, 0x9999);
  Actor_SetSpeed(IVAN, 0x13333, 0x9999);
  Actor_SetAnimation(GARET, 2);
  leader = Actor_Get(ACTOR_PARTY_LEADER);
  if (leader != 0) {
    Engine_ActorSetDestination(GARET, leader->x.part.pixel,
                               leader->z.part.pixel);
  }
  Actor_WaitForMove(GARET);
  Actor_SetPosition(GARET, 0, 0);
  Actor_SetAnimation(MIA, 2);
  leader = Actor_Get(ACTOR_PARTY_LEADER);
  if (leader != 0) {
    Engine_ActorSetDestination(MIA, leader->x.part.pixel, leader->z.part.pixel);
  }
  Actor_WaitForMove(MIA);
  Actor_SetPosition(MIA, 0, 0);
  Actor_SetAnimation(IVAN, 2);
  leader = Actor_Get(ACTOR_PARTY_LEADER);
  if (leader != 0) {
    Engine_ActorSetDestination(IVAN, leader->x.part.pixel,
                               leader->z.part.pixel);
  }
  Actor_WaitForMove(IVAN);
  Actor_SetPosition(IVAN, 0, 0);
  Event_Wait(10);
  Event_End();
}

// The bed search stages the party's rest and wake-up dialogue the first time.
static void BabiPalace_RestInPalaceBeds(void) {
  struct FieldActor *leader;
  s16 roundedFacing;
  u16 facing;

  leader = Actor_Get(ACTOR_PARTY_LEADER);
  roundedFacing = (leader->facing + 0x2000) & -0x4000;
  Event_Begin();
  Battle_ResetEffectCounter();
  Event_SetMessage(MsgTolbiLikeSleep);
  Event_OpenMessage(-1, 0);
  if (Event_ChooseYesNo(0, 0) != 0) {
  } else {
    if (GameFlag_IsSet(0x96a) != 0) {
      Event_Wait(20);
      Inn_PlaySleep(0);
      return;
    }
    Event_Wait(20);
    facing = roundedFacing;
    if (facing == 0) {
      Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 128, 120);
    }
    if (facing == 0x8000) {
      Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 240, 120);
    }
    Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 184, 120);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_EAST, 0);
    Event_Wait(10);
    FieldMotion_LaunchFromFocusedObject(GARET, 16, 0, 0x8000);
    Object_RefreshSelectorById(GARET);
    Event_Wait(10);
    Event_ShowMessage(GARET, 0);
    Event_Wait(10);
    Actor_SetAnimationAndWait(ACTOR_PARTY_LEADER, 3);
    Event_Wait(20);
    Actor_SetSpeed(GARET, 0x10000, 0x8000);
    Actor_WalkByAndWait(GARET, 40, 0);
    Actor_WalkByAndWait(GARET, 0, -32);
    Actor_FaceDirection(GARET, FACING_SOUTHWEST, 0);
    Event_Wait(20);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_WEST, 0);
    Event_Wait(20);
    FieldMotion_LaunchFromFocusedObject(IVAN, -16, 0, 0);
    Object_RefreshSelectorById(IVAN);
    Event_Wait(10);
    Event_ShowMessage(IVAN, 0);
    Event_Wait(10);
    Actor_SetAnimationAndWait(ACTOR_PARTY_LEADER, 3);
    Event_Wait(20);
    Actor_SetSpeed(IVAN, 0x10000, 0x8000);
    Actor_WalkByAndWait(IVAN, -40, 0);
    Actor_WalkByAndWait(IVAN, 0, 40);
    Actor_FaceDirection(IVAN, FACING_NORTHEAST, 0);
    Event_Wait(20);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_EAST, 0);
    Event_Wait(20);
    FieldMotion_LaunchFromFocusedObject(MIA, 16, 0, 0x8000);
    Object_RefreshSelectorById(MIA);
    Event_Wait(10);
    Event_ShowMessage(MIA, 0);
    Event_Wait(10);
    Actor_SetAnimationAndWait(ACTOR_PARTY_LEADER, 3);
    Event_Wait(20);
    Actor_SetSpeed(MIA, 0x10000, 0x8000);
    Actor_WalkByAndWait(MIA, 40, 0);
    Actor_WalkByAndWait(MIA, 0, 40);
    Actor_FaceDirection(MIA, FACING_NORTHWEST, 0);
    Event_Wait(20);
    Camera_MoveTo(-1, -1, -1, 0);
    Actor_WalkByAndWait(ACTOR_PARTY_LEADER, -56, 0);
    Actor_WalkByAndWait(ACTOR_PARTY_LEADER, 0, -32);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_SOUTHEAST, 0);
    Event_Wait(30);
    Actor_SetAnimationAndWait(MIA, 3);
    Event_Wait(10);
    Event_ShowMessage(MIA, 0);
    Actor_SetAnimationAndWait(GARET, 3);
    Event_Wait(10);
    if (GameFlag_IsSet(0x96a) == 0) {
      Event_ShowMessage(GARET, 0);
    } else {
      gEventWork->message++;
    }
    Actor_SetAnimationAndWait(IVAN, 3);
    Event_Wait(10);
    if (GameFlag_IsSet(0x96a) == 0) {
      Event_ShowMessage(IVAN, 0);
    } else {
      gEventWork->message++;
    }
    Event_Wait(20);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_EAST, 0);
    Actor_FaceDirection(GARET, FACING_WEST, 0);
    Actor_FaceDirection(MIA, FACING_WEST, 0);
    Actor_FaceDirection(IVAN, FACING_EAST, 0);
    Inn_PlaySleep(0);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_SOUTHEAST, 0);
    Actor_FaceDirection(GARET, FACING_SOUTHWEST, 0);
    Actor_FaceDirection(MIA, FACING_NORTHWEST, 0);
    Actor_FaceDirection(IVAN, FACING_NORTHEAST, 0);
    Event_Wait(20);
    Actor_SetAnimationAndWait(GARET, 3);
    Event_Wait(10);
    Event_ShowMessage(GARET, 0);
    Actor_SetAnimationAndWait(IVAN, 3);
    Event_Wait(10);
    if (GameFlag_IsSet(0x96a) == 0) {
      Event_ShowMessage(IVAN, 0);
    } else {
      gEventWork->message++;
    }
    Actor_SetAnimationAndWait(MIA, 3);
    Event_Wait(10);
    if (GameFlag_IsSet(0x96a) == 0) {
      Event_ShowMessage(MIA, 0);
    } else {
      gEventWork->message++;
    }
    Actor_WalkByAndWait(GARET, 0, 32);
    Actor_WalkBy(GARET, -112, 0);
    Actor_WalkByAndWait(MIA, 0, -40);
    Actor_WalkBy(MIA, -112, 0);
    Event_Wait(50);
    Actor_WalkByAndWait(IVAN, 0, -24);
    Actor_WaitForMove(GARET);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_SOUTH, 0);
    Actor_WalkByAndWait(GARET, 0, -16);
    Actor_WaitForMove(MIA);
    Actor_FaceDirection(MIA, FACING_NORTH, 0);
    Event_Wait(20);
    Event_ShowMessage(GARET, 0);
    Event_Wait(10);
    Actor_SetAnimationAndWait(ACTOR_PARTY_LEADER, 3);
    Event_Wait(20);
    Actor_SetSpeed(GARET, 0x13333, 0x9999);
    Actor_SetSpeed(IVAN, 0x13333, 0x9999);
    Actor_SetSpeed(MIA, 0x13333, 0x9999);
    Actor_SetAnimation(GARET, 2);
    leader = Actor_Get(ACTOR_PARTY_LEADER);
    if (leader != 0) {
      Engine_ActorSetDestination(GARET, leader->x.part.pixel,
                                 leader->z.part.pixel);
    }
    Actor_WaitForMove(GARET);
    Actor_SetPosition(GARET, 0, 0);
    Actor_SetAnimation(MIA, 2);
    leader = Actor_Get(ACTOR_PARTY_LEADER);
    if (leader != 0) {
      Engine_ActorSetDestination(MIA, leader->x.part.pixel,
                                 leader->z.part.pixel);
    }
    Actor_WaitForMove(MIA);
    Actor_SetPosition(MIA, 0, 0);
    Actor_SetAnimation(IVAN, 2);
    leader = Actor_Get(ACTOR_PARTY_LEADER);
    if (leader != 0) {
      Engine_ActorSetDestination(IVAN, leader->x.part.pixel,
                                 leader->z.part.pixel);
    }
    Actor_WaitForMove(IVAN);
    Actor_SetPosition(IVAN, 0, 0);
    GameFlag_Set(0x96a);
  }
  Event_End();
}

// Stages Iodem's conversation with actors 21 and 22, then walks them away.
static void BabiPalace_RunIodemDeparture(void) {
  GameFlag_Set(2411);
  Event_Begin();
  Battle_ResetEffectCounter();
  Event_SetMessage(MsgTolbiIodemIodem);
  Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 520, 424);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTHEAST, 0);
  Camera_MoveTo(PIXELS(560), -1, PIXELS(368), 1);
  Camera_WaitForMove();
  Actor_SetSpeed(20, 65536, 32768);
  Actor_WalkByAndWait(20, 40, 0);
  Actor_WalkToAndWait(20, 584, 360);
  Event_ShowMessage(21, 0);
  Event_Wait(10);
  Actor_ShowEmote(20, 257, 40);
  Actor_FaceDirection(20, 32768, 0);
  Actor_SetSpeed(21, 131072, 65536);
  Actor_SetSpeed(22, 131072, 65536);
  Actor_WalkTo(21, 528, 352);
  Actor_WalkToAndWait(22, 528, 368);
  Task_Wait(3);
  Actor_SetAnimation(21, 1);
  Event_Wait(30);
  Event_Wait(10);
  Actor_SetAnimationAndWait(22, 3);
  Event_Wait(30);
  Event_ShowMessage(22, 0);
  Event_Wait(10);
  Actor_RunRepeatedMotion(20, 2);
  Event_Wait(20);
  Actor_WalkByAndWait(20, -16, 0);
  Event_Wait(10);
  Event_ShowMessage(20, 0);
  Event_Wait(10);
  Actor_SetAnimation(21, 4);
  Actor_SetAnimationAndWait(22, 4);
  Event_Wait(20);
  Event_Wait(10);
  Actor_ShowEmote(20, 261, 40);
  Event_ShowMessage(20, 0);
  Event_Wait(10);
  Actor_ShowEmote(21, 258, 40);
  Actor_SetSpeed(21, 65536, 32768);
  Actor_WalkByAndWait(21, 8, 0);
  Event_Wait(20);
  Event_ShowMessage(21, 0);
  Event_Wait(10);
  Actor_ShowEmote(20, 257, 80);
  Event_Wait(10);
  Actor_ShowEmote(22, 258, 40);
  Actor_SetSpeed(22, 65536, 32768);
  Actor_WalkByAndWait(22, 8, 0);
  Event_Wait(20);
  Event_ShowMessage(22, 0);
  Event_Wait(10);
  Actor_SetAnimationAndWait(20, 4);
  Event_Wait(20);
  Event_ShowMessage(20, 0);
  Event_Wait(10);
  Actor_SetAnimation(21, 3);
  Actor_SetAnimationAndWait(22, 3);
  Event_Wait(30);
  Event_Wait(10);
  Actor_RunRepeatedMotion(21, 2);
  Event_Wait(20);
  Event_ShowMessage(21, 0);
  Event_Wait(10);
  Actor_SetAnimationAndWait(22, 4);
  Event_Wait(20);
  Event_ShowMessage(22, 0);
  Event_Wait(10);
  Actor_ShowEmote(20, 261, 40);
  Event_ShowMessage(20, 0);
  Event_Wait(10);
  Actor_ShowEmote(21, 257, 40);
  Event_ShowMessage(21, 0);
  Event_Wait(10);
  Actor_ShowEmote(20, 258, 40);
  Event_ShowMessage(20, 0);
  Event_Wait(10);
  Actor_RunRepeatedMotion(22, 2);
  Event_Wait(20);
  Event_ShowMessage(22, 0);
  Event_Wait(10);
  Actor_SetAnimationAndWait(20, 3);
  Event_Wait(30);
  Event_Wait(10);
  Actor_RunRepeatedMotion(21, 2);
  Event_Wait(20);
  Event_ShowMessage(21, 0);
  Event_Wait(10);
  Actor_ShowEmote(20, 258, 40);
  Event_ShowMessage(20, 0);
  Event_Wait(20);
  Actor_FaceEachOther(21, 22, 60);
  Actor_FaceActor(21, 20, 0);
  Actor_FaceActor(22, 20, 0);
  Event_Wait(20);
  Event_Wait(10);
  Actor_RunRepeatedMotion(20, 2);
  Event_Wait(20);
  Event_ShowMessage(20, 0);
  Event_Wait(10);
  Actor_ShowEmote(21, 256, 40);
  Event_ShowMessage(21, 0);
  Event_Wait(10);
  Actor_SetAnimationAndWait(22, 3);
  Event_Wait(30);
  Event_ShowMessage(22, 0);
  Event_Wait(10);
  Actor_SetAttachedEffect(20, 258);
  Actor_RunRepeatedMotion(20, 2);
  Event_Wait(30);
  Event_ShowMessage(20, 0);
  Event_Wait(10);
  Actor_ShowEmote(21, 257, 40);
  Event_ShowMessage(21, 0);
  Event_Wait(10);
  Actor_SetAnimationAndWait(20, 3);
  Event_Wait(30);
  Event_Wait(10);
  Actor_ShowEmote(22, 257, 40);
  Event_ShowMessage(22, 0);
  Event_Wait(10);
  Actor_SetAnimationAndWait(20, 4);
  Event_Wait(20);
  Event_ShowMessage(20, 0);
  Event_Wait(20);
  Actor_FaceEachOther(21, 22, 60);
  Actor_FaceActor(21, 20, 0);
  Actor_FaceActor(22, 20, 0);
  Event_Wait(20);
  Event_Wait(10);
  Actor_ShowEmote(20, 261, 40);
  Event_ShowMessage(20, 0);
  Event_Wait(10);
  Actor_RunRepeatedMotion(21, 2);
  Event_Wait(20);
  Event_ShowMessage(21, 0);
  Event_Wait(10);
  Actor_SetAnimationAndWait(22, 3);
  Event_Wait(30);
  Event_ShowMessage(22, 0);
  Event_Wait(10);
  Actor_SetAnimationAndWait(20, 3);
  Event_Wait(30);
  Event_Wait(10);
  Actor_RunRepeatedMotion(22, 2);
  Event_Wait(20);
  Event_ShowMessage(22, 0);
  Event_Wait(10);
  Actor_ShowEmote(20, 258, 40);
  Event_ShowMessage(20, 0);
  Event_Wait(10);
  Actor_RunRepeatedMotion(21, 2);
  Event_Wait(20);
  Event_ShowMessage(21, 0);
  Event_Wait(10);
  Actor_SetAnimationAndWait(20, 4);
  Event_Wait(20);
  Event_ShowMessage(20, 0);
  Event_Wait(10);
  Actor_ShowEmote(22, 256, 40);
  Event_ShowMessage(22, 0);
  Event_Wait(10);
  Actor_ShowEmote(20, 258, 40);
  Event_ShowMessage(20, 0);
  Event_Wait(10);
  Actor_ShowEmote(21, 257, 40);
  Event_ShowMessage(21, 0);
  Event_Wait(10);
  Actor_SetAnimationAndWait(20, 4);
  Event_Wait(20);
  Event_ShowMessage(20, 0);
  Event_Wait(20);
  Actor_FaceEachOther(21, 22, 60);
  Actor_FaceActor(21, 20, 0);
  Actor_FaceActor(22, 20, 0);
  Event_Wait(20);
  Event_Wait(10);
  Actor_RunRepeatedMotion(20, 2);
  Event_Wait(20);
  Event_ShowMessage(20, 0);
  Event_Wait(10);
  Actor_SetAttachedEffect(21, 258);
  Actor_SetAttachedEffect(22, 258);
  Actor_StartRepeatedMotion(21, 2);
  Actor_RunRepeatedMotion(22, 2);
  Event_Wait(30);
  Event_Wait(10);
  Actor_FaceDirection(20, FACING_EAST, 0);
  Event_Wait(20);
  Event_ShowMessage(20, 0);
  Event_Wait(10);
  Actor_SetAnimationAndWait(21, 4);
  Event_Wait(20);
  Event_ShowMessage(21, 0);
  Event_Wait(10);
  Actor_SetAnimationAndWait(22, 4);
  Event_Wait(20);
  Event_ShowMessage(22, 0);
  Event_Wait(10);
  Actor_RunRepeatedMotion(20, 2);
  Event_Wait(20);
  Actor_FaceDirection(20, 32768, 0);
  Event_Wait(20);
  Event_ShowMessage(20, 0);
  Event_Wait(10);
  Actor_SetAnimationAndWait(21, 3);
  Event_Wait(30);
  Event_ShowMessage(21, 0);
  Event_Wait(20);
  Actor_ShowEmote(20, 261, 40);
  Event_ShowMessage(20, 0);
  Event_Wait(10);
  Actor_SetAnimationAndWait(22, 3);
  Event_Wait(30);
  Event_ShowMessage(22, 0);
  Event_Wait(10);
  Actor_SetAnimationAndWait(20, 3);
  Event_Wait(40);
  Actor_FaceEachOther(21, 22, 60);
  Actor_SetAnimationAndWait(21, 3);
  Event_Wait(20);
  Actor_SetAnimationAndWait(22, 3);
  Event_Wait(20);
  Actor_FaceDirection(22, 16384, 0);
  Event_Wait(40);
  Actor_SetSpeed(21, 85196, 42598);
  Actor_SetSpeed(22, 85196, 42598);
  Actor_WalkBy(21, 0, 120);
  Actor_WalkByAndWait(22, 0, 120);
  Actor_SetPosition(21, 0, 0);
  Actor_SetPosition(22, 0, 0);
  Event_Wait(10);
  Actor_RunRepeatedMotion(20, 2);
  Event_Wait(40);
  Actor_SetSpeed(20, 52428, 26214);
  Actor_WalkByAndWait(20, -16, 0);
  Event_Wait(30);
  Event_ShowMessage(20, 0);
  Event_Wait(10);
  Actor_SetAnimationAndWait(20, 4);
  Event_Wait(30);
  Actor_FaceDirection(20, FACING_EAST, 0);
  Event_Wait(30);
  Actor_SetSpeed(20, 65536, 32768);
  Actor_WalkByAndWait(20, 120, 0);
  Actor_WalkByAndWait(20, 60, 0);
  Actor_SetPosition(20, 0, 0);
  Event_End();
}

// Wakes the party through a brightness transition and branching conversation.
static void BabiPalace_RunWakeUpConversation(void) {
  struct FieldActor *actor;
  s32 step;
  s32 pick;
  struct EventWork *event;
  struct DisplayTransitionState *work;

  Event_Begin();
  Event_SetMessage(MsgTolbiCallsName);
  Actor_SetAnimation(ACTOR_PARTY_LEADER, 31);
  actor = Actor_Get(ACTOR_PARTY_LEADER);
  Actor_SetSpriteFlags(actor, 0);
  Actor_SetPosition(GARET, 0x680000, 0x680000);
  Actor_SetPosition(MIA, 0x580000, 0x780000);
  Actor_SetPosition(IVAN, 0x780000, 0x780000);
  Actor_FaceDirection(GARET, FACING_SOUTH, 0);
  Actor_FaceDirection(MIA, FACING_EAST, 0);
  Actor_FaceDirection(IVAN, FACING_WEST, 0);
  event = (struct EventWork *)((u8 **)&gEventWork)[0];
  event->startTransition = SCENE_TRANSITION(TRANSITION_BACKDROP_FADE, 0);
  event->transitionFrames = 12;
  Task_Wait(1);
  DisplayTransition_InitializeBattleEffectState(9);
  // UB: slot 31 lies past scalar gEventWork. Its named load adds four
  // bytes in the native EN trial (0x82c to 0x830).
#ifdef UBFIX
  work = gDisplayTransition;
#else
  work = (struct DisplayTransitionState *)((u8 **)&gEventWork)[4];
#endif
  SetHalf(&work->value, 0);
  work->mask = 0x1f1f;
  work->active = 1;
  Engine_EventOpenScreen();
  Engine_EventWaitForScreen();
  for (step = 1; step <= 5; step++) {
    Task_Wait(3);
    work->value = step;
  }
  Event_Wait(40);
  Actor_RunRepeatedMotion(ACTOR_PARTY_LEADER, 2);
  Event_Wait(30);
  for (step = 5; step <= 31; step++) {
    Task_Wait(3);
    work->value = step;
  }
  work->active = 31;
  event = (struct EventWork *)((u8 **)&gEventWork)[0];
  event->startTransition = SCENE_TRANSITION(TRANSITION_WINDOW, 9);
  event->transitionFrames = 24;
  Event_Wait(20);
  Actor_ShowEmote(GARET, EMOTE_IN_FRONT, 50);
  Actor_FaceActor(GARET, ACTOR_PARTY_LEADER, 40);
  Actor_RunRepeatedMotion(GARET, 2);
  Event_Wait(30);
  Event_Wait(10);
  Actor_ShowEmote(IVAN, EMOTE_IN_FRONT | 1, 40);
  Actor_FaceDirection(IVAN, FACING_NORTHEAST, 0);
  Event_Wait(30);
  Event_ShowMessage(IVAN, 0);
  Event_Wait(10);
  Event_ShowMessage(MIA, 0);
  Actor_FaceDirection(MIA, FACING_NORTHEAST, 0);
  Event_Wait(30);
  Actor_RunRepeatedMotion(MIA, 2);
  Event_Wait(30);
  Actor_SetSpeed(MIA, 0x10000, 0x8000);
  Actor_WalkByAndWait(MIA, 0, -40);
  Actor_WalkByAndWait(MIA, 32, 0);
  Actor_FaceDirection(MIA, FACING_SOUTHEAST, 0);
  Event_Wait(10);
  Event_ShowMessage(MIA, 0);
  actor = Actor_Get(ACTOR_PARTY_LEADER);
  actor->z.fixed -= 0x30000;
  actor = Actor_Get(ACTOR_PARTY_LEADER);
  actor->targetZ -= 0x30000;
  Actor_SetAnimation(ACTOR_PARTY_LEADER, 32);
  Event_Wait(40);
  Actor_SetAnimationAndWait(ACTOR_PARTY_LEADER, 34);
  Event_Wait(30);
  Actor_SetAnimation(ACTOR_PARTY_LEADER, 33);
  Event_Wait(40);
  Actor_SetAnimationAndWait(GARET, ANIM_SHAKE_HEAD);
  Event_Wait(20);
  Event_ShowMessage(GARET, 0);
  Event_Wait(10);
  Actor_ShowEmote(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | 2, 80);
  Actor_ShowEmote(IVAN, EMOTE_IN_FRONT, 50);
  Event_OpenMessage(IVAN, 0);
  if (Event_ChooseYesNo(-1, 0) == 0) {
    Event_Wait(10);
    Actor_SetAnimationAndWait(ACTOR_PARTY_LEADER, 33);
    Event_Wait(20);
    Actor_ShowEmote(GARET, EMOTE_IN_FRONT | 3, 40);
    Actor_Jump(GARET, 4, 13);
    Actor_Jump(GARET, 4, 30);
    Event_ShowMessage(GARET, 0);
    ((struct EventWork *)((u8 **)&gEventWork)[0])->message++;
  } else {
    Event_Wait(10);
    Actor_SetAnimationAndWait(ACTOR_PARTY_LEADER, 34);
    Event_Wait(20);
    Actor_ShowEmote(GARET, EMOTE_IN_FRONT | 3, 40);
    Actor_Jump(GARET, 4, 13);
    Actor_Jump(GARET, 4, 30);
    ((struct EventWork *)((u8 **)&gEventWork)[0])->message++;
    Event_ShowMessage(GARET, 0);
  }
  Event_Wait(10);
  Actor_FaceDirection(IVAN, FACING_NORTHWEST, 0);
  Event_Wait(30);
  Actor_SetAnimationAndWait(IVAN, ANIM_SHAKE_HEAD);
  Event_Wait(20);
  Actor_FaceDirection(GARET, FACING_SOUTHEAST, 0);
  Event_Wait(30);
  Actor_RunRepeatedMotion(GARET, 2);
  Event_Wait(40);
  Actor_FaceDirection(GARET, FACING_NORTHEAST, 0);
  Actor_FaceDirection(IVAN, FACING_NORTHEAST, 0);
  Event_Wait(30);
  Actor_SetAnimationAndWait(GARET, ANIM_NOD);
  Event_Wait(30);
  Event_ShowMessage(GARET, 0);
  Event_Wait(10);
  Actor_FaceDirection(MIA, FACING_SOUTHWEST, 0);
  Event_Wait(50);
  Actor_FaceDirection(MIA, FACING_SOUTHEAST, 0);
  Event_Wait(35);
  Actor_ShowEmote(MIA, EMOTE_IN_FRONT | 8, 50);
  Event_Wait(10);
  Actor_SetAnimationAndWait(MIA, ANIM_NOD);
  Event_Wait(30);
  Event_ShowMessage(MIA, 0);
  Event_Wait(10);
  Actor_RunRepeatedMotion(IVAN, 2);
  Event_Wait(20);
  Event_OpenMessage(IVAN, 0);
  if (Event_ChooseYesNo(-1, 0) == 0) {
    Event_Wait(10);
    Actor_SetAnimationAndWait(ACTOR_PARTY_LEADER, 33);
    Event_Wait(20);
    Actor_ShowEmote(GARET, EMOTE_IN_FRONT | 7, 40);
    Event_ShowMessage(GARET, 0);
    pick = 0;
    ((struct EventWork *)((u8 **)&gEventWork)[0])->message++;
  } else {
    Event_Wait(10);
    Actor_SetAnimationAndWait(ACTOR_PARTY_LEADER, 34);
    Event_Wait(20);
    Actor_ShowEmote(GARET, EMOTE_IN_FRONT | 7, 40);
    ((struct EventWork *)((u8 **)&gEventWork)[0])->message++;
    pick = 1;
    Event_ShowMessage(GARET, 0);
  }
  Event_Wait(10);
  Actor_FaceDirection(IVAN, FACING_NORTHWEST, 0);
  Event_Wait(30);
  Actor_SetAnimationAndWait(IVAN, ANIM_SHAKE_HEAD);
  Event_Wait(20);
  Actor_FaceDirection(GARET, FACING_SOUTHEAST, 0);
  Event_Wait(30);
  Actor_RunRepeatedMotion(GARET, 2);
  Event_Wait(40);
  Actor_FaceDirection(GARET, FACING_NORTHEAST, 0);
  Actor_FaceDirection(IVAN, FACING_NORTHEAST, 0);
  Event_Wait(30);
  Actor_SetAnimationAndWait(GARET, ANIM_NOD);
  Event_Wait(30);
  Event_ShowMessage(GARET, 0);
  Event_Wait(10);
  Actor_FaceDirection(MIA, FACING_SOUTHWEST, 0);
  Event_Wait(50);
  Actor_FaceDirection(MIA, FACING_SOUTHEAST, 0);
  Event_Wait(35);
  Actor_ShowEmote(MIA, EMOTE_IN_FRONT | 8, 50);
  Event_Wait(10);
  Actor_SetAnimationAndWait(MIA, ANIM_NOD);
  Event_Wait(30);
  Event_ShowMessage(MIA, 0);
  if (pick == 0) {
    Event_Wait(10);
    Actor_SetAnimationAndWait(IVAN, ANIM_NOD);
    Event_Wait(30);
    Event_ShowMessage(IVAN, 0);
    Event_Wait(10);
    Actor_RunRepeatedMotion(GARET, 2);
    Event_Wait(30);
    Event_ShowMessage(GARET, 0);
    ((struct EventWork *)((u8 **)&gEventWork)[0])->message += 2;
  } else {
    ((struct EventWork *)((u8 **)&gEventWork)[0])->message += 2;
    Event_Wait(10);
    Actor_SetAnimationAndWait(IVAN, ANIM_SHAKE_HEAD);
    Event_Wait(20);
    Event_ShowMessage(IVAN, 0);
    Event_Wait(10);
    Actor_RunRepeatedMotion(GARET, 2);
    Event_Wait(20);
    Event_ShowMessage(GARET, 0);
  }
  Event_Wait(10);
  Actor_ShowEmote(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | 2, 60);
  Event_Wait(10);
  Actor_SetAnimationAndWait(GARET, ANIM_NOD);
  Event_Wait(30);
  Event_ShowMessage(GARET, 0);
  Event_Wait(20);
  Actor_FaceEachOther(IVAN, MIA, 40);
  Actor_FaceDirection(MIA, FACING_SOUTHEAST, 0);
  Actor_FaceDirection(IVAN, FACING_NORTHEAST, 0);
  Event_Wait(30);
  // Wait for the player to leave the bed before the companions rejoin.
  while ((gKeysHeld & KEYS_DPAD) == 0) {
    Task_Wait(1);
  }
  Actor_Jump(ACTOR_PARTY_LEADER, 6, 0);
  Actor_SetSpeed(ACTOR_PARTY_LEADER, 0x1e666, 0xf333);
  Actor_WalkByAndWait(ACTOR_PARTY_LEADER, -32, -8);
  actor = Actor_Get(ACTOR_PARTY_LEADER);
  Actor_SetSpriteFlags(actor, 1);
  Event_Wait(20);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 0);
  Actor_FaceDirection(MIA, FACING_SOUTH, 0);
  Actor_FaceDirection(IVAN, FACING_NORTH, 0);
  Event_Wait(20);
  Event_Wait(10);
  Actor_RunRepeatedMotion(MIA, 2);
  Event_Wait(20);
  Event_ShowMessage(MIA, 0);
  Event_Wait(10);
  Actor_RunRepeatedMotion(GARET, 2);
  Event_Wait(20);
  Event_ShowMessage(GARET, 0);
  Event_Wait(20);
  Actor_SetAnimationAndWait(IVAN, ANIM_NOD);
  Event_Wait(30);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_SOUTH, 0);
  Actor_FaceDirection(GARET, FACING_SOUTHEAST, 0);
  Event_Wait(30);
  Event_ShowMessage(IVAN, 0);
  Event_Wait(20);
  Actor_SetAnimation(ACTOR_PARTY_LEADER, ANIM_NOD);
  Actor_SetAnimation(GARET, ANIM_NOD);
  Actor_SetAnimationAndWait(MIA, ANIM_NOD);
  Event_Wait(30);
  Actor_SetSpeed(GARET, 0x13333, 0x9999);
  Actor_SetSpeed(MIA, 0x13333, 0x9999);
  Actor_SetSpeed(IVAN, 0x13333, 0x9999);
  Actor_SetAnimation(GARET, ANIM_WALK);
  actor = Actor_Get(ACTOR_PARTY_LEADER);
  if (actor != NULL) {
    Engine_ActorSetDestination(GARET, actor->x.part.pixel, actor->z.part.pixel);
  }
  Actor_WaitForMove(GARET);
  Actor_SetPosition(GARET, 0, 0);
  Actor_SetAnimation(MIA, ANIM_WALK);
  actor = Actor_Get(ACTOR_PARTY_LEADER);
  if (actor != NULL) {
    Engine_ActorSetDestination(MIA, actor->x.part.pixel, actor->z.part.pixel);
  }
  Actor_WaitForMove(MIA);
  Actor_SetPosition(MIA, 0, 0);
  Actor_SetAnimation(IVAN, ANIM_WALK);
  actor = Actor_Get(ACTOR_PARTY_LEADER);
  if (actor != NULL) {
    Engine_ActorSetDestination(IVAN, actor->x.part.pixel, actor->z.part.pixel);
  }
  Actor_WaitForMove(IVAN);
  Actor_SetPosition(IVAN, 0, 0);
  Event_Wait(10);
  Event_End();
}

// Gathers the waiting companions for the palace conversation and departure.
static void BabiPalace_RunWaitingCompanionsScene(void) {
  s16 *position;

  Event_PlayCue(30);
  Event_Begin();
  Event_SetMessage(MsgTolbiWaitingCompanions);
  Engine_EventOpenScreen();
  Engine_EventWaitForScreen();
  Camera_MoveTo(0xd80000, -1, 0x2e00000, 1);
  Camera_WaitForMove();
  Event_Wait(20);
  Event_Wait(10);
  Event_ShowMessage(20, 0);
  Event_Wait(10);
  Actor_SetSpeed(14, 0xcccc, 0x6666);
  Actor_WalkByAndWait(14, 0, 16);
  Event_Wait(10);
  Actor_SetAnimationAndWait(14, 3);
  Event_Wait(30);
  Event_ShowMessage(14, 0);
  Event_Wait(20);
  Event_ShowMessage(20, 0);
  Engine_CameraFollowActor(0, 1);
  Camera_WaitForMove();
  Event_Wait(40);
  Actor_SetSpeed(ACTOR_PARTY_LEADER, 0x10000, 0x8000);
  Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 208, 0x2f8);
  Event_Wait(10);
  Camera_MoveTo(0xd80000, -1, 0x2e00000, 1);
  FieldMotion_LaunchFromFocusedObject(GARET, -16, 16, 0xc000);
  FieldMotion_LaunchFromFocusedObject(MIA, 0, 24, 0xc000);
  FieldMotion_LaunchFromFocusedObject(IVAN, 16, 16, 0xc000);
  Actor_WaitForMove(GARET);
  Event_Wait(30);
  Event_Wait(10);
  Actor_SetAnimationAndWait(14, 3);
  Event_Wait(30);
  Event_ShowMessage(14, 0);

  // The reward branch selects a different starting point in the dialogue.
  if (GameFlag_IsSet(0x951) != 0) {
    Event_Wait(10);
    Actor_SetAnimationAndWait(20, 3);
    Event_Wait(20);
    Event_ShowMessage(20, 0);
    Event_Wait(10);
    Actor_FaceDirection(14, FACING_NORTHWEST, 0);
    Event_Wait(30);
    Actor_SetAnimationAndWait(14, 3);
    Event_Wait(20);
    Event_ShowMessage(14, 0);
    Event_Wait(10);
    Actor_SetAnimationAndWait(20, 3);
    Event_Wait(20);
    Event_ShowMessage(20, 0);
    Actor_FaceDirection(20, FACING_NORTH, 0);
    Event_Wait(20);
    Actor_SetSpeed(20, 0x10000, 0x8000);
    Actor_WalkByAndWait(20, 0, -16);
    Event_Wait(40);
    Actor_SetAnimationAndWait(20, 3);
    Event_Wait(40);
    Actor_FaceDirection(20, FACING_SOUTH, 0);
    Event_Wait(20);
    Actor_WalkByAndWait(20, 0, 32);
    Actor_FaceDirection(14, FACING_WEST, 0);
    Event_Wait(10);
    Actor_FaceDirection(20, FACING_EAST, 0);
    Event_Wait(30);
    Actor_SetAnimationAndWait(20, 3);
    Event_Wait(30);
    Actor_FaceDirection(20, FACING_SOUTH, 0);
    Event_Wait(20);
    Actor_SetSpeed(20, 0xcccc, 0x6666);
    Actor_Get(20)->steeringFlags &= ~ACTOR_FACES_MOTION;
    Actor_WalkByAndWait(20, 0, -16);
    Actor_Get(20)->steeringFlags |= ACTOR_FACES_MOTION;
    Actor_FaceDirection(14, FACING_SOUTH, 0);
    Event_Wait(40);
    Actor_SetSpeed(14, 0xcccc, 0x6666);
    Actor_WalkByAndWait(14, 0, 16);
    Event_Wait(40);
    Engine_UiTextDrawQuantity(164, 2);
    Event_ShowMessage(-1, 0);
    Item_ShowFound(164, 3);
    Party_GiveItem(164, 0);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 0);
    Event_Wait(30);
    Actor_WalkByAndWait(14, 0, -16);
    Actor_FaceDirection(14, FACING_SOUTH, 0);
    Event_Wait(30);
    Actor_RunRepeatedMotion(14, 2);
    Event_Wait(20);
    Event_OpenMessage(14, 0);
    gEventWork->message += 2;
  } else {
    gEventWork->message += 5;
    Event_Wait(10);
    Actor_SetAnimationAndWait(20, 3);
    Event_Wait(20);
    Event_ShowMessage(20, 0);
    Actor_FaceDirection(14, FACING_NORTHWEST, 0);
    Event_Wait(40);
    Actor_SetAnimationAndWait(14, 3);
    Event_Wait(20);
    Actor_SetAnimationAndWait(20, 3);
    Event_Wait(30);
    Actor_FaceDirection(14, FACING_SOUTH, 0);
    Event_Wait(30);
    Actor_RunRepeatedMotion(14, 2);
    Event_Wait(20);
    Event_OpenMessage(14, 0);
  }

  if (Event_ChooseYesNo(0, 0) == 0) {
    Event_Wait(20);
    Actor_SetAnimationAndWait(20, 3);
    Event_Wait(20);
    Event_ShowMessage(20, 0);
    gEventWork->message++;
  } else {
    Event_Wait(10);
    Actor_SetAnimationAndWait(20, 3);
    Event_Wait(20);
    gEventWork->message++;
    Event_ShowMessage(20, 0);
  }

  Event_Wait(10);
  Actor_FaceDirection(14, FACING_NORTHWEST, 0);
  Event_Wait(20);
  Actor_SetAnimationAndWait(14, 3);
  Event_Wait(30);
  Event_ShowMessage(14, 0);
  Event_Wait(10);
  Actor_SetAnimationAndWait(20, 3);
  Event_Wait(30);
  Event_Wait(10);
  Actor_Jump(GARET, 4, 13);
  Actor_Jump(GARET, 4, 30);
  Event_ShowMessage(GARET, 0);
  Event_Wait(10);
  Actor_FaceDirection(14, FACING_SOUTH, 0);
  Actor_RunRepeatedMotion(20, 2);
  Event_Wait(20);
  Actor_SetSpeed(14, 0x19999, 0xcccc);
  Actor_WalkByAndWait(20, 0, 16);
  Task_Wait(2);
  Actor_FaceDirection(20, FACING_SOUTHEAST, 0);
  Event_Wait(10);
  Event_ShowMessage(20, 0);
  Event_Wait(10);
  Actor_SetAnimationAndWait(14, 4);
  Event_Wait(30);
  Actor_FaceActor(14, 20, 30);
  Event_ShowMessage(14, 0);
  Event_Wait(10);
  Actor_RunRepeatedMotion(20, 2);
  Event_Wait(20);
  Actor_FaceEachOther(14, GARET, 0);
  Event_Wait(40);
  Event_ShowMessage(14, 0);
  Event_Wait(10);
  Actor_RunRepeatedMotion(GARET, 2);
  Event_Wait(30);
  Actor_FaceActor(GARET, IVAN, 30);
  Event_Wait(10);
  Actor_RunRepeatedMotion(IVAN, 2);
  Event_Wait(20);
  Actor_FaceActor(IVAN, GARET, 30);
  Event_ShowMessage(IVAN, 0);
  Event_Wait(10);
  Actor_ShowEmote(GARET, 0x106, 50);
  Event_Wait(10);
  Actor_SetAnimationAndWait(14, 3);
  Event_Wait(30);
  Event_ShowMessage(14, 0);
  Event_Wait(10);
  Actor_FaceDirection(GARET, FACING_NORTH, 0);
  Actor_FaceDirection(IVAN, FACING_NORTH, 0);
  Event_Wait(30);
  Event_Wait(10);
  Actor_RunRepeatedMotion(20, 2);
  Event_Wait(20);
  Event_ShowMessage(20, 0);
  Event_Wait(10);
  Actor_FaceEachOther(GARET, ACTOR_PARTY_LEADER, 0);
  Actor_FaceEachOther(MIA, IVAN, 50);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 0);
  Actor_FaceDirection(GARET, FACING_NORTH, 0);
  Actor_FaceDirection(MIA, FACING_NORTH, 0);
  Actor_FaceDirection(IVAN, FACING_NORTH, 0);
  Event_Wait(30);
  Event_Wait(10);
  Actor_SetAnimationAndWait(20, 4);
  Event_Wait(20);
  Event_ShowMessage(20, 0);
  Event_Wait(10);
  Actor_SetAnimationAndWait(14, 4);
  Event_Wait(20);
  Event_ShowMessage(14, 0);
  Event_Wait(10);
  Actor_ShowEmote(IVAN, 0x101, 40);
  Event_ShowMessage(IVAN, 0);
  Event_Wait(10);
  Actor_RunRepeatedMotion(14, 2);
  Event_Wait(20);
  Event_OpenMessage(14, 0);
  if (Event_ChooseYesNo(0, 0) == 0) {
    Event_Wait(20);
    Actor_SetAnimationAndWait(14, 3);
    Event_Wait(30);
    Event_ShowMessage(14, 0);
    gEventWork->message++;
  } else {
    Event_Wait(10);
    Actor_SetAnimationAndWait(14, 4);
    Event_Wait(20);
    gEventWork->message++;
    Event_ShowMessage(14, 0);
  }

  Event_Wait(10);
  Actor_ShowEmote(20, 0x102, 40);
  Event_ShowMessage(20, 0);
  Event_Wait(20);
  Actor_SetAnimation(ACTOR_PARTY_LEADER, 3);
  Actor_SetAnimation(GARET, 3);
  Actor_SetAnimation(MIA, 3);
  Actor_SetAnimationAndWait(IVAN, 3);
  Event_Wait(50);
  Actor_FaceActor(14, 20, 60);
  Actor_FaceDirection(14, FACING_SOUTH, 0);
  Event_Wait(20);
  Event_ShowMessage(14, 0);
  Event_Wait(10);
  Actor_ShowEmote(20, 0x102, 40);
  Event_ShowMessage(20, 0);
  Event_Wait(10);
  Actor_SetAnimationAndWait(20, 4);
  Event_Wait(20);
  Event_ShowMessage(20, 0);
  Event_Wait(10);
  Actor_ShowEmote(ACTOR_PARTY_LEADER, 0x102, 40);
  Event_Wait(10);
  Actor_SetAnimationAndWait(14, 4);
  Event_Wait(20);
  Event_ShowMessage(14, 0);
  Event_Wait(10);
  Actor_FaceEachOther(GARET, ACTOR_PARTY_LEADER, 50);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 0);
  Actor_FaceDirection(GARET, FACING_NORTH, 0);
  Event_Wait(30);
  Event_Wait(10);
  Actor_SetAnimation(ACTOR_PARTY_LEADER, 3);
  Actor_SetAnimationAndWait(GARET, 3);
  Event_Wait(30);
  Event_Wait(10);
  Actor_SetAnimationAndWait(14, 3);
  Event_Wait(30);
  Event_ShowMessage(14, 0);
  Event_Wait(10);
  Actor_ShowEmote(MIA, 0x100, 40);
  Event_ShowMessage(MIA, 0);
  Event_Wait(10);
  Actor_ShowEmote(14, 0x102, 50);
  Actor_FaceActor(20, 14, 50);
  Event_Wait(10);
  Actor_RunRepeatedMotion(14, 2);
  Event_Wait(20);
  Event_ShowMessage(14, 0);
  Event_Wait(20);
  Actor_FaceEachOther(GARET, ACTOR_PARTY_LEADER, 0);
  Actor_FaceEachOther(MIA, IVAN, 50);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 0);
  Actor_FaceDirection(GARET, FACING_NORTH, 0);
  Actor_FaceDirection(MIA, FACING_NORTH, 0);
  Actor_FaceDirection(IVAN, FACING_NORTH, 0);
  Event_Wait(50);
  Actor_FaceDirection(20, FACING_SOUTHEAST, 0);
  Event_Wait(20);
  Event_ShowMessage(20, 0);
  Event_Wait(10);
  Actor_RunRepeatedMotion(GARET, 2);
  Event_Wait(20);
  Event_ShowMessage(GARET, 0);
  Event_Wait(20);
  Actor_FaceDirection(14, FACING_WEST, 0);
  Event_Wait(40);
  Actor_ShowEmote(14, 0x105, 60);
  Event_ShowMessage(14, 0);
  Event_Wait(10);
  Actor_ShowEmote(IVAN, 0x100, 40);
  Event_ShowMessage(IVAN, 0);
  Event_Wait(10);
  Actor_FaceEachOther(14, IVAN, 40);
  Actor_SetAnimationAndWait(14, 3);
  Event_Wait(30);
  Event_ShowMessage(14, 0);
  Event_Wait(10);
  Actor_FaceEachOther(GARET, ACTOR_PARTY_LEADER, 0);
  Actor_FaceEachOther(MIA, IVAN, 60);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 0);
  Actor_FaceDirection(GARET, FACING_NORTH, 0);
  Actor_FaceDirection(MIA, FACING_NORTH, 0);
  Actor_FaceDirection(IVAN, FACING_NORTH, 0);
  Event_Wait(50);
  Actor_RunRepeatedMotion(20, 2);
  Event_Wait(20);
  Event_ShowMessage(20, 0);
  Event_Wait(10);
  Actor_FaceEachOther(GARET, ACTOR_PARTY_LEADER, 40);
  Actor_RunRepeatedMotion(GARET, 2);
  Event_Wait(20);
  Event_OpenMessage(GARET, 0);
  if (Event_ChooseYesNo(0, 0) == 0) {
    Event_Wait(20);
    Event_ShowMessage(GARET, 0);
    gEventWork->message += 3;
  } else {
    Event_Wait(10);
    gEventWork->message++;
    Event_OpenMessage(GARET, 0);
    if (Event_ChooseYesNo(0, 0) == 0) {
      Event_Wait(20);
      Event_ShowMessage(GARET, 0);
      gEventWork->message++;
    } else {
      gEventWork->message++;
      Event_ShowMessage(GARET, 0);
    }
  }

  Event_Wait(10);
  Actor_SetAnimationAndWait(14, 4);
  Event_Wait(20);
  Event_ShowMessage(14, 0);
  Event_Wait(10);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 0);
  Actor_FaceDirection(GARET, FACING_NORTH, 0);
  Event_Wait(40);
  Actor_RunRepeatedMotion(GARET, 2);
  Event_Wait(20);
  Event_Wait(10);
  Actor_SetAnimationAndWait(14, 3);
  Event_Wait(30);
  Event_ShowMessage(14, 0);
  Event_Wait(10);
  Actor_ShowEmote(MIA, 0x101, 40);
  Event_ShowMessage(MIA, 0);
  Event_Wait(10);
  Actor_RunRepeatedMotion(14, 2);
  Event_Wait(20);
  Event_ShowMessage(14, 0);
  Event_Wait(10);
  Actor_RunRepeatedMotion(IVAN, 2);
  Event_Wait(20);
  Event_ShowMessage(IVAN, 0);
  Event_Wait(10);
  Actor_FaceActor(20, 14, 40);
  Actor_RunRepeatedMotion(20, 2);
  Event_Wait(20);
  Event_ShowMessage(20, 0);
  Event_Wait(20);
  Actor_ShowEmote(14, 0x105, 70);
  Event_ShowMessage(14, 0);
  Event_Wait(10);
  Actor_ShowEmote(ACTOR_PARTY_LEADER, 0x101, 0);
  Actor_ShowEmote(GARET, 0x101, 0);
  Actor_ShowEmote(MIA, 0x101, 0);
  Actor_ShowEmote(IVAN, 0x101, 40);
  Event_Wait(10);
  Actor_SetAnimationAndWait(14, 4);
  Event_Wait(20);
  Event_ShowMessage(14, 0);
  Event_Wait(10);
  Actor_SetAnimationAndWait(14, 3);
  Event_Wait(30);
  Event_ShowMessage(14, 0);
  Event_Wait(20);
  Actor_SetAnimationAndWait(GARET, 3);
  Event_Wait(30);
  Event_ShowMessage(GARET, 0);
  Event_Wait(20);
  Actor_SetAnimationAndWait(14, 3);
  Event_Wait(30);
  Event_Wait(10);
  Actor_ShowEmote(IVAN, 0x101, 40);
  Event_ShowMessage(IVAN, 0);
  Event_Wait(10);
  Actor_SetAnimationAndWait(14, 3);
  Event_Wait(40);
  Actor_FaceActor(ACTOR_PARTY_LEADER, IVAN, 0);
  Actor_FaceActor(GARET, IVAN, 0);
  Actor_FaceActor(MIA, IVAN, 0);
  Actor_FaceActor(20, IVAN, 0);
  Event_Wait(50);
  Event_Wait(10);
  Actor_RunRepeatedMotion(IVAN, 2);
  Event_Wait(20);
  Event_ShowMessage(IVAN, 0);
  Event_Wait(10);
  Actor_ShowEmote(14, 0x101, 40);
  Event_ShowMessage(14, 0);
  Event_Wait(10);
  Actor_SetAnimation(ACTOR_PARTY_LEADER, 3);
  Actor_SetAnimation(GARET, 3);
  Actor_SetAnimationAndWait(MIA, 3);
  Event_Wait(30);
  Event_Wait(10);
  Actor_SetAnimationAndWait(IVAN, 4);
  Event_Wait(20);
  Event_ShowMessage(IVAN, 0);
  Event_Wait(20);
  Actor_SetAnimationAndWait(20, 3);
  Event_Wait(30);
  Actor_FaceActor(20, 14, 30);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 0);
  Actor_FaceDirection(GARET, FACING_NORTH, 0);
  Actor_FaceDirection(MIA, FACING_NORTH, 0);
  Event_Wait(20);
  Event_ShowMessage(20, 0);
  Event_Wait(10);
  Actor_SetAnimationAndWait(14, 3);
  Event_Wait(30);
  Event_ShowMessage(14, 0);
  Event_Wait(10);
  Actor_StartRepeatedMotion(ACTOR_PARTY_LEADER, 2);
  Actor_StartRepeatedMotion(GARET, 2);
  Actor_StartRepeatedMotion(MIA, 2);
  Actor_RunRepeatedMotion(IVAN, 2);
  Event_Wait(30);
  Event_Wait(10);
  Actor_RunRepeatedMotion(20, 2);
  Event_Wait(20);
  Event_ShowMessage(20, 0);
  Event_Wait(10);
  Actor_FaceDirection(14, FACING_WEST, 0);
  Event_Wait(20);
  Actor_SetAnimationAndWait(14, 4);
  Event_Wait(20);
  Event_ShowMessage(14, 0);
  Event_Wait(10);
  Actor_FaceEachOther(GARET, ACTOR_PARTY_LEADER, 0);
  Actor_FaceEachOther(MIA, IVAN, 50);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 0);
  Actor_FaceDirection(GARET, FACING_NORTH, 0);
  Actor_FaceDirection(MIA, FACING_NORTH, 0);
  Actor_FaceDirection(IVAN, FACING_NORTH, 0);
  Event_Wait(40);
  Actor_FaceDirection(14, FACING_SOUTH, 0);
  Event_Wait(30);
  Actor_ShowEmote(14, 0x102, 40);
  Event_ShowMessage(14, 0);
  Event_Wait(10);
  Actor_ShowEmote(20, 0x102, 40);
  Event_ShowMessage(20, 0);
  Event_Wait(20);
  Actor_FaceEachOther(14, 20, 40);
  Actor_RunRepeatedMotion(14, 2);
  Event_Wait(20);
  Event_ShowMessage(14, 0);
  Event_Wait(10);
  Actor_SetAttachedEffect(20, 0x102);
  Actor_RunRepeatedMotion(20, 2);
  Event_Wait(20);
  Event_ShowMessage(20, 0);
  Actor_SetSpeed(20, 0x19999, 0xcccc);
  Actor_WalkByAndWait(20, 0, 24);
  Actor_FaceActor(ACTOR_PARTY_LEADER, 20, 0);
  Actor_FaceActor(GARET, 20, 0);
  Actor_FaceActor(MIA, 20, 0);
  Actor_FaceActor(IVAN, 20, 0);
  Actor_FaceDirection(14, FACING_SOUTH, 0);
  Event_ShowMessage(14, 0);
  Event_Wait(10);
  Actor_ShowEmote(20, 0x100, 60);
  Actor_FaceActor(20, 14, 0);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 0);
  Actor_FaceDirection(GARET, FACING_NORTH, 0);
  Actor_FaceDirection(MIA, FACING_NORTH, 0);
  Actor_FaceDirection(IVAN, FACING_NORTH, 0);
  Event_Wait(10);
  Actor_RunRepeatedMotion(20, 2);
  Event_Wait(20);
  Event_ShowMessage(20, 0);
  Event_Wait(30);
  Actor_ShowEmote(14, 0x105, 40);
  Event_ShowMessage(14, 0);
  Event_Wait(10);
  Actor_ShowEmote(20, 0x101, 60);
  Actor_SetSpeed(20, 0x13333, 0x9999);
  Actor_WalkByAndWait(20, 0, -24);
  Actor_FaceDirection(20, FACING_EAST, 0);
  Event_Wait(20);
  Event_ShowMessage(20, 0);
  Event_Wait(10);
  Actor_FaceDirection(14, FACING_WEST, 0);
  Event_Wait(20);
  Actor_SetAnimationAndWait(14, 3);
  Event_Wait(30);
  Event_Wait(10);
  Actor_RunRepeatedMotion(20, 2);
  Event_Wait(20);
  Event_ShowMessage(20, 0);
  Event_Wait(10);
  Actor_SetAnimationAndWait(14, 4);
  Event_Wait(20);
  Event_ShowMessage(14, 0);
  Event_Wait(10);
  Actor_ShowEmote(20, 0x102, 40);
  Event_ShowMessage(20, 0);
  Event_Wait(10);
  Actor_SetAnimationAndWait(14, 3);
  Event_Wait(30);
  Event_ShowMessage(14, 0);
  Event_Wait(10);
  Actor_RunRepeatedMotion(GARET, 2);
  Event_Wait(20);
  Event_ShowMessage(GARET, 0);
  Event_Wait(10);
  Actor_FaceDirection(14, FACING_SOUTH, 0);
  Event_Wait(30);
  Actor_SetAnimationAndWait(14, 3);
  Event_Wait(30);
  Event_ShowMessage(14, 0);
  Event_Wait(10);
  Actor_ShowEmote(MIA, 0x102, 40);
  Event_ShowMessage(MIA, 0);
  Event_Wait(10);
  Actor_SetAnimationAndWait(14, 3);
  Event_Wait(30);
  Event_ShowMessage(14, 0);
  Event_Wait(10);
  Actor_SetAnimationAndWait(IVAN, 3);
  Event_Wait(30);
  Event_ShowMessage(IVAN, 0);
  Event_Wait(10);
  Actor_SetAnimationAndWait(14, 3);
  Event_Wait(30);
  Event_ShowMessage(14, 0);
  Event_Wait(10);
  Actor_FaceEachOther(GARET, ACTOR_PARTY_LEADER, 0);
  Actor_FaceEachOther(MIA, IVAN, 50);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 0);
  Actor_FaceDirection(GARET, FACING_NORTH, 0);
  Actor_FaceDirection(MIA, FACING_NORTH, 0);
  Actor_FaceDirection(IVAN, FACING_NORTH, 0);
  Event_Wait(30);
  Event_Wait(10);
  Actor_RunRepeatedMotion(14, 2);
  Event_Wait(20);
  Event_ShowMessage(14, 0);
  Event_Wait(20);
  Actor_ShowEmote(ACTOR_PARTY_LEADER, 0x101, 0);
  Actor_ShowEmote(GARET, 0x101, 0);
  Actor_ShowEmote(MIA, 0x101, 0);
  Actor_ShowEmote(IVAN, 0x101, 50);
  Event_Wait(10);
  Actor_SetAnimationAndWait(14, 3);
  Event_Wait(30);
  Event_ShowMessage(14, 0);
  Event_Wait(10);
  Actor_ShowEmote(20, 0x102, 40);
  Event_ShowMessage(20, 0);
  Event_Wait(10);
  Actor_FaceEachOther(14, 20, 30);
  Event_ShowMessage(14, 0);
  Event_Wait(10);
  Actor_SetAnimationAndWait(14, 4);
  Event_Wait(20);
  Event_ShowMessage(14, 0);
  Event_Wait(10);
  Actor_RunRepeatedMotion(GARET, 2);
  Event_Wait(20);
  Event_ShowMessage(GARET, 0);
  Event_Wait(10);
  Actor_RunRepeatedMotion(14, 2);
  Event_Wait(20);
  Actor_FaceDirection(14, FACING_SOUTH, 0);
  Event_Wait(20);
  Event_ShowMessage(14, 0);
  Event_Wait(10);
  Actor_ShowEmote(20, 0x101, 40);
  Event_ShowMessage(20, 0);
  Event_Wait(10);
  Actor_SetAnimationAndWait(14, 3);
  Event_Wait(30);
  Actor_FaceDirection(14, FACING_WEST, 0);
  Event_Wait(20);
  Event_ShowMessage(14, 0);
  Event_Wait(10);
  Actor_ShowEmote(IVAN, 0x101, 40);
  Event_ShowMessage(IVAN, 0);
  Event_Wait(10);
  Actor_FaceDirection(14, FACING_SOUTH, 0);
  Actor_FaceDirection(20, FACING_SOUTHEAST, 0);
  Event_Wait(30);
  Actor_SetAnimationAndWait(14, 3);
  Event_Wait(30);
  Event_Wait(10);
  Actor_RunRepeatedMotion(MIA, 2);
  Event_Wait(20);
  Event_ShowMessage(MIA, 0);
  Event_Wait(10);
  Actor_ShowEmote(14, 0x102, 40);
  Event_ShowMessage(14, 0);
  Event_Wait(20);
  Actor_SetAnimation(ACTOR_PARTY_LEADER, 3);
  Actor_SetAnimation(GARET, 3);
  Actor_SetAnimation(MIA, 3);
  Actor_SetAnimationAndWait(IVAN, 3);
  Event_Wait(30);
  Event_Wait(20);
  Actor_ShowEmote(14, 0x105, 60);
  Event_ShowMessage(14, 0);
  Event_Wait(10);
  Actor_ShowEmote(GARET, 0x101, 40);
  Event_ShowMessage(GARET, 0);
  Event_Wait(10);
  Actor_ShowEmote(14, 0x100, 40);
  Event_ShowMessage(14, 0);
  Event_Wait(10);
  Actor_FaceEachOther(GARET, ACTOR_PARTY_LEADER, 50);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 0);
  Actor_FaceDirection(GARET, FACING_NORTH, 0);
  Event_Wait(30);
  Event_Wait(10);
  Actor_SetAnimationAndWait(14, 3);
  Event_Wait(30);
  Event_ShowMessage(14, 0);
  Event_Wait(10);
  Actor_ShowEmote(MIA, 0x101, 40);
  Event_ShowMessage(MIA, 0);
  Event_Wait(10);
  Actor_SetAnimationAndWait(14, 4);
  Event_Wait(20);
  Event_ShowMessage(14, 0);
  Event_Wait(10);
  Actor_ShowEmote(ACTOR_PARTY_LEADER, 0x101, 0);
  Actor_ShowEmote(GARET, 0x101, 0);
  Actor_ShowEmote(IVAN, 0x101, 0);
  Actor_ShowEmote(MIA, 0x101, 40);
  Event_Wait(10);
  Actor_SetAnimationAndWait(14, 4);
  Event_Wait(20);
  Event_ShowMessage(14, 0);
  Event_Wait(10);
  Actor_ShowEmote(IVAN, 0x100, 40);
  Event_OpenMessage(IVAN, 0);
  if (Event_ChooseYesNo(14, 0) == 0) {
    Event_Wait(10);
    Event_ShowMessage(IVAN, 0);
    gEventWork->message++;
  } else {
    Event_Wait(10);
    gEventWork->message++;
    Event_ShowMessage(IVAN, 0);
  }

  Event_Wait(10);
  Actor_RunRepeatedMotion(14, 2);
  Event_Wait(20);
  Event_ShowMessage(14, 0);
  Event_Wait(10);
  Actor_SetAnimationAndWait(14, 4);
  Event_Wait(20);
  Event_ShowMessage(14, 0);
  Event_Wait(10);
  Actor_RunRepeatedMotion(20, 2);
  Event_Wait(20);
  Actor_FaceDirection(20, FACING_EAST, 0);
  Event_Wait(20);
  Event_ShowMessage(20, 0);
  Event_Wait(10);
  Actor_FaceActor(14, 20, 40);
  Actor_SetAnimationAndWait(14, 4);
  Event_Wait(30);
  Event_ShowMessage(14, 0);
  Event_Wait(10);
  Actor_FaceEachOther(GARET, ACTOR_PARTY_LEADER, 40);
  Event_ShowMessage(GARET, 0);
  Actor_FaceDirection(14, FACING_SOUTH, 0);
  Actor_FaceDirection(20, FACING_SOUTHEAST, 0);
  Event_Wait(30);
  Event_ShowMessage(14, 0);
  Event_Wait(10);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 0);
  Actor_FaceDirection(GARET, FACING_NORTH, 0);
  Event_Wait(30);
  Actor_FaceEachOther(MIA, IVAN, 60);
  Actor_FaceDirection(MIA, FACING_NORTH, 0);
  Actor_FaceDirection(IVAN, FACING_NORTH, 0);
  Event_Wait(30);
  Event_ShowMessage(MIA, 0);
  Event_Wait(10);
  Actor_SetAnimationAndWait(IVAN, 4);
  Event_Wait(20);
  Event_ShowMessage(IVAN, 0);
  Event_Wait(10);
  Actor_FaceEachOther(20, 14, 60);
  Actor_FaceDirection(20, FACING_SOUTHEAST, 0);
  Actor_FaceDirection(14, FACING_SOUTH, 0);
  Event_Wait(30);
  Actor_FaceDirection(14, FACING_SOUTH, 0);
  Event_Wait(10);
  Actor_SetAnimationAndWait(14, 3);
  Event_Wait(30);
  Event_ShowMessage(14, 0);
  Event_Wait(10);
  Actor_RunRepeatedMotion(20, 2);
  Event_Wait(20);
  Event_OpenMessage(20, 0);
  if (Event_ChooseYesNo(0, 0) == 0) {
    Event_Wait(20);
    Actor_ShowEmote(GARET, 0x102, 40);
    Event_ShowMessage(GARET, 0);
    gEventWork->message++;
  } else {
    Event_Wait(10);
    gEventWork->message++;
    Event_ShowMessage(GARET, 0);
  }

  Event_Wait(10);
  Actor_RunRepeatedMotion(20, 2);
  Event_Wait(20);
  Event_ShowMessage(20, 0);
  Event_Wait(10);
  Actor_ShowEmote(20, 0x100, 40);
  Event_ShowMessage(20, 0);
  Event_Wait(20);
  Actor_SetAnimation(ACTOR_PARTY_LEADER, 3);
  Actor_SetAnimation(GARET, 3);
  Actor_SetAnimation(MIA, 3);
  Actor_SetAnimationAndWait(IVAN, 3);
  Event_Wait(30);
  Event_Wait(10);
  Actor_ShowEmote(14, 0x100, 40);
  Event_ShowMessage(14, 0);
  Event_Wait(10);
  Actor_FaceActor(20, 14, 40);
  Event_Wait(10);
  Actor_ShowEmote(20, 0x102, 40);
  Event_ShowMessage(20, 0);
  Event_Wait(10);
  Actor_FaceActor(14, 20, 40);
  Actor_SetAnimationAndWait(14, 3);
  Event_Wait(30);
  Event_ShowMessage(14, 0);
  Event_Wait(10);
  Actor_RunRepeatedMotion(20, 2);
  Event_Wait(20);
  Event_ShowMessage(20, 0);
  Actor_FaceDirection(14, FACING_SOUTH, 0);
  Event_Wait(20);
  Actor_RunRepeatedMotion(14, 2);
  Event_Wait(20);
  Event_ShowMessage(14, 0);
  Event_Wait(10);
  Actor_ShowEmote(ACTOR_PARTY_LEADER, 0x101, 80);
  Actor_ShowEmote(20, 0x102, 70);
  Actor_SetAnimationAndWait(14, 4);
  Event_Wait(20);
  Event_ShowMessage(14, 0);
  Event_Wait(10);
  Actor_FaceEachOther(GARET, ACTOR_PARTY_LEADER, 0);
  Actor_FaceEachOther(MIA, IVAN, 50);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 0);
  Actor_FaceDirection(GARET, FACING_NORTH, 0);
  Actor_FaceDirection(MIA, FACING_NORTH, 0);
  Actor_FaceDirection(IVAN, FACING_NORTH, 0);
  Event_Wait(30);
  Event_Wait(10);
  Actor_ShowEmote(20, 0x106, 50);
  Event_ShowMessage(20, 0);
  Event_Wait(10);
  Actor_SetAnimationAndWait(14, 3);
  Event_Wait(30);
  Actor_FaceDirection(20, FACING_SOUTHEAST, 0);
  Event_ShowMessage(20, 0);
  Event_Wait(10);
  Actor_ShowEmote(14, 0x108, 40);
  Event_OpenMessage(14, 0);
  if (Event_ChooseYesNo(0, 0) == 0) {
    Event_Wait(20);
    Actor_ShowEmote(14, 0x100, 40);
    Event_ShowMessage(14, 0);
    gEventWork->message++;
  } else {
    Event_Wait(10);
    Actor_ShowEmote(14, 0x100, 40);
    gEventWork->message++;
    Event_ShowMessage(14, 0);
  }

  Event_Wait(10);
  Actor_RunRepeatedMotion(20, 2);
  Event_Wait(20);
  Event_ShowMessage(20, 0);
  Event_Wait(10);
  Actor_FaceActor(14, 20, 40);
  Actor_SetAnimationAndWait(14, 3);
  Event_Wait(30);
  Event_ShowMessage(20, 0);
  Event_Wait(10);
  Actor_FaceActor(20, 14, 40);
  Event_ShowMessage(20, 0);
  Event_Wait(10);
  Actor_ShowEmote(14, 0x100, 40);
  Actor_FaceDirection(14, FACING_WEST, 0);
  Event_Wait(20);
  Event_ShowMessage(14, 0);
  Event_Wait(10);
  Actor_ShowEmote(20, 0x100, 40);
  Event_ShowMessage(20, 0);
  Event_Wait(10);
  Actor_RunRepeatedMotion(14, 2);
  Event_Wait(20);
  Event_ShowMessage(14, 0);
  Event_Wait(10);
  Actor_RunRepeatedMotion(20, 2);
  Event_Wait(20);
  Event_ShowMessage(20, 0);
  Event_Wait(10);
  Actor_SetAnimationAndWait(20, 3);
  Event_Wait(30);
  Actor_SetAnimationAndWait(14, 3);
  Event_Wait(30);
  Actor_FaceDirection(20, FACING_SOUTHEAST, 0);
  Event_Wait(20);
  // The party tracks each departing speaker before gathering back together.
  Object_LinkObjectAndSetCallback(ACTOR_PARTY_LEADER, 20);
  Object_LinkObjectAndSetCallback(GARET, 20);
  Object_LinkObjectAndSetCallback(MIA, 20);
  Object_LinkObjectAndSetCallback(IVAN, 20);
  Actor_SetSpeed(20, 0x10000, 0x8000);
  Actor_WalkByAndWait(20, 0, 32);
  Actor_FaceDirection(20, FACING_EAST, 0);
  Actor_FaceDirection(14, FACING_SOUTH, 0);
  Event_Wait(20);
  Event_ShowMessage(20, 0);
  Event_Wait(10);
  Actor_SetAnimationAndWait(20, 3);
  Event_Wait(30);
  Actor_WalkByAndWait(GARET, 16, 0);
  Actor_WalkBy(20, 0, 80);
  Event_Wait(40);
  Actor_WalkByAndWait(GARET, -16, 0);
  Actor_FaceDirection(GARET, FACING_SOUTH, 0);
  Actor_FaceDirection(MIA, FACING_SOUTH, 0);
  Actor_WaitForMove(20);
  Event_Wait(80);
  Actor_SetPosition(20, 0, 0);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 0);
  Actor_FaceDirection(GARET, FACING_NORTH, 0);
  Actor_FaceDirection(MIA, FACING_NORTH, 0);
  Actor_FaceDirection(IVAN, FACING_NORTH, 0);
  Event_Wait(30);
  Object_LinkObjectAndSetCallback(ACTOR_PARTY_LEADER, 14);
  Object_LinkObjectAndSetCallback(GARET, 14);
  Object_LinkObjectAndSetCallback(MIA, 14);
  Object_LinkObjectAndSetCallback(IVAN, 14);
  Event_Wait(30);
  Actor_SetAnimationAndWait(14, 3);
  Event_Wait(30);
  Actor_SetSpeed(14, 0xcccc, 0x6666);
  Actor_WalkByAndWait(14, 0, 24);
  Actor_WalkByAndWait(14, -80, 0);
  Event_Wait(10);
  Actor_FaceDirection(14, FACING_EAST, 0);
  Event_Wait(20);
  Event_ShowMessage(14, 0);
  Event_Wait(20);
  Actor_FaceDirection(14, FACING_SOUTH, 0);
  Event_Wait(20);
  Actor_WalkByAndWait(14, 0, 48);
  Actor_WalkByAndWait(14, -64, 0);
  Actor_Stop(ACTOR_PARTY_LEADER);
  Actor_Stop(GARET);
  Actor_Stop(MIA);
  Actor_Stop(IVAN);
  Actor_SetPosition(14, 0, 0);
  Event_Wait(20);
  Actor_FaceEachOther(ACTOR_PARTY_LEADER, MIA, 0);
  Actor_FaceEachOther(GARET, IVAN, 0);
  Event_Wait(30);
  Actor_SetAnimation(ACTOR_PARTY_LEADER, 3);
  Actor_SetAnimation(GARET, 3);
  Actor_SetAnimation(MIA, 3);
  Actor_SetAnimationAndWait(IVAN, 3);
  Event_Wait(30);
  Event_PlayCue(17);
  Actor_SetSpeed(GARET, 0x13333, 0x9999);
  Actor_SetSpeed(IVAN, 0x13333, 0x9999);
  Actor_SetSpeed(MIA, 0x13333, 0x9999);
  Actor_SetAnimation(GARET, 2);
  position = Actor_Get(ACTOR_PARTY_LEADER);
  if (position != 0)
    Engine_ActorSetDestination(GARET, position[5], position[9]);
  Actor_WaitForMove(GARET);
  Actor_SetPosition(GARET, 0, 0);
  Actor_SetAnimation(IVAN, 2);
  position = Actor_Get(ACTOR_PARTY_LEADER);
  if (position != 0)
    Engine_ActorSetDestination(IVAN, position[5], position[9]);
  Actor_WaitForMove(IVAN);
  Actor_SetPosition(IVAN, 0, 0);
  Actor_SetAnimation(MIA, 2);
  position = Actor_Get(ACTOR_PARTY_LEADER);
  if (position != 0)
    Engine_ActorSetDestination(MIA, position[5], position[9]);
  Actor_WaitForMove(MIA);
  Actor_SetPosition(MIA, 0, 0);
  Event_Wait(10);
  Audio_PlayCueFromEventWork();
  Event_End();
}

// Opens the touched door's map layer, walks through and restores the layers.
static void BabiPalace_LeaveThroughDoor(void) {
  struct EventWork *work;

  work = (struct EventWork *)((u8 **)&gEventWork)[0];
  Event_Begin();
  Event_PlayCue(158);
  Actor_SetSpeed(ACTOR_PARTY_LEADER, 0x8000, 0x4000);
  Actor_SetAnimation(ACTOR_PARTY_LEADER, 2);
  if (work->touchedTrigger == 32) {
    Map_ClearLayerEntryFlag(1);
    Event_Wait(10);
    Actor_SetDestinationOffset(ACTOR_PARTY_LEADER, 0, -16);
  } else {
    if (work->touchedTrigger == 30) {
      Map_ClearLayerEntryFlag(4);
      Event_Wait(10);
      Actor_CenterAndWalk(ACTOR_PARTY_LEADER, 3, -16);
    } else {
      Map_ClearLayerEntryFlag(2);
      Event_Wait(10);
      Actor_CenterAndWalk(ACTOR_PARTY_LEADER, 3, -16);
    }
  }
  Event_Wait(16);
  Event_RequestExit(work->touchedTrigger);
  Map_SetLayerEntryFlag(1);
  Map_SetLayerEntryFlag(2);
  Map_SetLayerEntryFlag(4);
  Event_End();
}

// Animates the cell-step door and leaves through exit 2.
static void BabiPalace_LeaveThroughAnimatedDoor(void) {
  Event_Begin();
  Actor_SetSpeed(ACTOR_PARTY_LEADER, 32768, 16384);
  Event_PlayCue(158);
  Map_AnimateCells(gBabiPalace_CellSteps, 36, 10);
  Actor_CenterAndWalk(ACTOR_PARTY_LEADER, 2, -16);
  Event_Wait(16);
  Event_RequestExit(2);
  Event_End();
}

// Gives actor 8's yes/no exchange, with actor 9 reacting to the refusal.
static void BabiPalace_AskPlayerIdentity(void) {
  s32 question;
  GameFlag_Set(2412);
  Event_Begin();
  Battle_ResetEffectCounter();
  Actor_FaceDirection(8, 20480, 0);
  Actor_FaceDirection(9, 12288, 0);
  Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 200, 272);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, 49152, 0);
  Event_Wait(20);
  question = MsgTolbiArent;
  Event_SetMessage(question);
  Event_OpenMessage(8, 0);
  if (Event_ChooseYesNo(0, 0) == 0) {
    Event_Wait(20);
    Event_SetMessage(question + 1);
    Event_ShowMessage(8, 0);
  } else {
    Event_Wait(20);
    Event_SetMessage(question + 2);
    Event_ShowMessage(8, 0);
    Event_Wait(20);
    Actor_FaceEachOther(8, 9, 60);
    Actor_FaceDirection(9, 12288, 0);
    Event_Wait(40);
    Actor_RunRepeatedMotion(9, 2);
    Event_Wait(30);
    Actor_FaceEachOther(8, 9, 30);
    Actor_SetAnimationAndWait(9, 3);
    Event_Wait(30);
    Actor_ShowEmote(8, 258, 50);
    Actor_FaceDirection(8, 20480, 0);
    Actor_FaceDirection(9, 12288, 0);
    Event_Wait(20);
    Actor_SetAnimationAndWait(8, 4);
    Event_Wait(20);
    Event_ShowMessage(8, 0);
    Event_Wait(10);
    Actor_RunRepeatedMotion(8, 2);
    Event_Wait(20);
    Event_ShowMessage(8, 0);
  }
  Event_End();
}

// Thanks the party once, then directs them to Babi at the Colosseum.
static void BabiPalace_ThankForSavingBabi(void) {
  if (GameFlag_IsSet(0x96d) == 0) {
    GameFlag_Set(0x96d);
    Event_SetMessage(MsgTolbiIsaacIdReallyLikeThank);
    Event_ShowMessage(9, 0);
  } else {
    Event_SetMessage(MsgTolbiBabiWaitingForAtColosseum);
    Event_ShowMessage(9, 0);
  }
}

// Asks about the warriors who stayed, then shows the chosen answer.
static void BabiPalace_AskAboutRemainingWarriors(s32 actorId) {
  s32 question = MsgTolbiWarriorsWhoStayed;

  Engine_EventSetMessage(question);
  Engine_EventOpenMessage(actorId, 0);
  if (Engine_EventChooseYesNo(0, 0) == 0) {
    Engine_EventSetMessage(question + 1);
    Engine_EventShowMessage(actorId, 0);
  } else {
    Engine_EventSetMessage(question + 2);
    Engine_EventShowMessage(actorId, 0);
  }
}

// Shows the two lines describing the well's clean water.
static void BabiPalace_ExamineWell(void) {
  Engine_EventBegin();
  Engine_MessageShowCentered(MsgFieldPeeredWell, 1);
  Engine_MessageShowCentered(MsgTolbiItsFilledWithFreshClean, 1);
  Engine_EventEnd();
}

// Selects events by palace scene and the same Colosso state as the placements.
const struct SceneEvent *Scene_GetEvents(void) {
  if (gGameState.scene == (s32)&SceneId_BabiPalace2) {
    if (Engine_GameFlagIsSet(0x950) != 0) {
      return gBabiPalaceEvents2AfterColosso;
    }
    if (Engine_GameFlagIsSet(0x962) != 0) {
      return gBabiPalaceEvents2Colosso;
    }
    return gBabiPalaceEvents2;
  }
  if (Engine_GameFlagIsSet(0x950) != 0) {
    return gBabiPalaceEventsAfterColosso;
  }
  if (Engine_GameFlagIsSet(0x962) != 0) {
    return gBabiPalaceEventsColosso;
  }
  return gBabiPalaceEventsOther;
}

// Restores door layers, the Cloak Ball display and entrance-specific scenes.
s32 BabiPalace_ApplyEntryState(void) {
  struct FieldActor *actor;
  struct FieldSprite *sprite;
  u8 *buf;
  s32 set;

  Map_SetLayerEntryFlag(1);
  Map_SetLayerEntryFlag(2);
  Map_SetLayerEntryFlag(4);
  if (gGameState.entrance == 90) {
    GameFlag_Set(0x962);
  }
  if (gGameState.entrance == 91) {
    GameFlag_Set(0x962);
    GameFlag_Set(0x950);
  }
  if (gGameState.scene != (s32)&SceneId_BabiPalace2) {
    if (gGameState.entrance == 11) {
      GameFlag_Clear(0x12f);
    }
    if (GameFlag_IsSet(0x950)) {
      set = GameFlag_IsSet(0xf31);
      if (set) {
        Actor_SetPosition(16, 0, 0);
      } else {
        actor = Actor_Get(16);
        actor->unk5c = 1;
        actor->motionFlags = set;
        sprite = actor->sprite;
        actor->y.fixed = 0x40000;
        sprite->partCount = set;
        sprite->fullColor = 0;
        sprite->palette = 0;
        buf = Heap_Allocate(17, 0x608);
        Item_LoadIcon(205);
        Vram_Load(sprite->vramBlock, 128, buf + 0x400);
        Heap_Release(17);
      }
      if (gGameState.entrance == 33 && !GameFlag_IsSet(0x96f)) {
        GameFlag_Set(0x96f);
        Actor_SetPosition(14, 0xd00000, 0x2c00000);
        BabiPalace_RunWaitingCompanionsScene();
      }
      Actor_SetAnimation(14, 5);
      Actor_SetSpriteFlags(Actor_Get(14), 0);
    } else if (GameFlag_IsSet(0x962) && !GameFlag_IsSet(0x966)) {
      Actor_SetPosition(10, 0x780000, 0x480000);
    }
    gEventWork->startTransition = SCENE_TRANSITION(TRANSITION_WINDOW, 9);
    Actor_Get(9)->collisionFlags |= 4;
    if (gGameState.entrance == 99) {
      BabiPalace_RestorePartyAndInventory();
      BabiPalace_RunWakeUpConversation();
      gGameState.entrance = 8;
    }
    if (gGameState.entrance == 98) {
      BabiPalace_RestorePartyAndInventory();
      GameFlag_Set(0x966);
      GameFlag_Set(0x967);
      Actor_SetPosition(10, 0x380000, 0x780000);
      Actor_FaceDirection(10, 0xf000, 0);
      BabiPalace_RunPostRestWakeUp();
      gGameState.entrance = 8;
    }
  }
  return 0;
}

// clang-format off
u16 gBabiPalace_CellSteps[] = {
    MAP_CELL_STEP(25, 28, 1, 3, 5), MAP_CELL_STEP(26, 28, 1, 3, 5), MAP_CELL_STEP(27, 28, 1, 3, 5), MAP_CELL_STEPS_END,
};

s32 gBabiPalace_MiddleActionScript[] = {
    SC_SET(SW_SCALE_X, 0x10000),
    SC_SET(SW_SCALE_Y, 0x10000),
    SC_ADD(SW_SCALE_X, -0x4000),
    SC_ADD(SW_SCALE_Y, 0x2000),
    SC_WAIT(2),
    SC_ADD(SW_SCALE_X, 0x2000),
    SC_ADD(SW_SCALE_Y, -0x1000),
    SC_WAIT(2),
    SC_ADD(SW_SCALE_X, 0x1000),
    SC_ADD(SW_SCALE_Y, -2048),
    SC_WAIT(2),
    SC_ADD(SW_SCALE_X, 0x1000),
    SC_ADD(SW_SCALE_Y, -2048),
    SC_WAIT(4),
    SC_LOOP(SC_FOREVER, SC_TOP),
};

struct SceneEntrance gBabiPalaceEntrancesOther[] = {
    { 0, CONDITION_ALWAYS, 172, 0, 149, FACING_SOUTH, 0, -1, -1, -1, -1, 0 },
    { 1, CONDITION_ALWAYS, 768, 0, 200, FACING_NORTH, 0, 512, 0, 1024, 224, 0 },
    { 2, CONDITION_ALWAYS, 728, 0, 88, FACING_EAST, 0, 512, 0, 1024, 224, 0 },
    { 3, CONDITION_ALWAYS, 808, 0, 88, FACING_WEST, 0, 512, 0, 1024, 224, 0 },
    { 4, CONDITION_ALWAYS, 600, 0, 152, FACING_EAST, 0, 512, 0, 1024, 224, 0 },
    { 5, CONDITION_ALWAYS, 936, 0, 152, FACING_WEST, 0, 512, 0, 1024, 224, 0 },
    { 6, CONDITION_ALWAYS, 584, 0, 56, FACING_SOUTH, 0, 512, 0, 1024, 224, 0 },
    { 7, CONDITION_ALWAYS, 952, 0, 56, FACING_SOUTH, 0, 512, 0, 1024, 224, 0 },
    { 8, CONDITION_ALWAYS, 72, 0, 216, FACING_NORTH, 0, 0, 0, 512, 240, 0 },
    { 9, CONDITION_ALWAYS, 440, 0, 216, FACING_NORTH, 0, 0, 0, 512, 240, 0 },
    { 10, CONDITION_ALWAYS, 72, 0, 968, FACING_EAST, 0, 0, 864, 240, 1024, 0 },
    { 11, CONDITION_ALWAYS, 168, 0, 936, FACING_SOUTH, 0, 0, 864, 240, 1024, 0 },
    { 12, CONDITION_ALWAYS, 424, 0, 680, FACING_WEST, 0, 328, 600, 568, 760, 0 },
    { 13, CONDITION_ALWAYS, 472, 0, 696, FACING_SOUTH, 0, 328, 600, 568, 760, 0 },
    { 16, CONDITION_ALWAYS, 408, 0, 824, FACING_EAST, 0, 320, 728, 560, 888, 0 },
    { 17, CONDITION_ALWAYS, 456, 0, 808, FACING_SOUTH, 0, 320, 728, 560, 888, 0 },
    { 18, CONDITION_ALWAYS, 824, 0, 648, FACING_NORTH, 0, 768, 480, 1024, 672, 0 },
    { 20, CONDITION_ALWAYS, 872, 0, 936, FACING_WEST, 0, 784, 864, 1024, 1024, 0 },
    { 21, CONDITION_ALWAYS, 888, 0, 968, FACING_WEST, 0, 784, 864, 1024, 1024, 0 },
    { 22, CONDITION_ALWAYS, 760, 0, 936, FACING_EAST, 0, 608, 864, 848, 1024, 0 },
    { 23, CONDITION_ALWAYS, 744, 0, 968, FACING_EAST, 0, 608, 864, 848, 1024, 0 },
    { 24, CONDITION_ALWAYS, 440, 0, 584, FACING_WEST, 0, 272, 240, 784, 608, 0 },
    { 25, CONDITION_ALWAYS, 616, 0, 584, FACING_EAST, 0, 272, 240, 784, 608, 0 },
    { 26, CONDITION_ALWAYS, 328, 0, 312, FACING_WEST, 0, 272, 240, 784, 608, 0 },
    { 27, CONDITION_ALWAYS, 728, 0, 312, FACING_EAST, 0, 272, 240, 784, 608, 0 },
    { 28, CONDITION_ALWAYS, 312, 0, 296, FACING_WEST, 0, 272, 240, 784, 608, 0 },
    { 29, CONDITION_ALWAYS, 744, 0, 296, FACING_EAST, 0, 272, 240, 784, 608, 0 },
    { 30, CONDITION_ALWAYS, 472, 0, 504, FACING_SOUTH, 0, 272, 240, 784, 608, 0 },
    { 31, CONDITION_ALWAYS, 584, 0, 504, FACING_SOUTH, 0, 272, 240, 784, 608, 0 },
    { 32, CONDITION_ALWAYS, 528, 0, 296, FACING_SOUTH, 0, 272, 240, 784, 608, 0 },
    { 33, CONDITION_ALWAYS, 208, 0, 840, FACING_NORTH, 0, 0, 624, 336, 864, 0 },
    { 34, CONDITION_ALWAYS, 40, 0, 760, FACING_WEST, 0, 0, 624, 336, 864, 0 },
    { 35, CONDITION_ALWAYS, 200, 0, 360, FACING_NORTH, 0, 0, 240, 256, 480, 0 },
    { 36, CONDITION_ALWAYS, 824, 0, 360, FACING_NORTH, 0, 768, 240, 1024, 480, 0 },
    { 40, CONDITION_ALWAYS, 376, 0, 936, FACING_EAST, 0, 232, 864, 472, 1024, 0 },
    { 41, CONDITION_ALWAYS, 520, 0, 936, FACING_WEST, 0, 424, 864, 664, 1024, 0 },
    { 42, CONDITION_ALWAYS, 904, 0, 776, FACING_EAST, 0, 776, 704, 1016, 864, 0 },
    { 43, CONDITION_ALWAYS, 216, 0, 552, FACING_WEST, 0, 0, 472, 304, 632, 0 },
    { 44, CONDITION_ALWAYS, 88, 0, 552, FACING_SOUTH, 0, 0, 472, 304, 632, 0 },
    { 45, CONDITION_ALWAYS, 728, 0, 824, FACING_NORTH, 0, 552, 656, 792, 848, 0 },
    { 98, CONDITION_ALWAYS, 152, 0, 104, FACING_SOUTH, 0, 0, 0, 512, 240, 0 },
    { 99, CONDITION_ALWAYS, 152, 0, 104, FACING_SOUTH, 0, 0, 0, 512, 240, 0 },
    { SCENE_TABLE_END },
};

struct SceneEntrance gBabiPalaceEntrances2[] = {
    { 0, CONDITION_ALWAYS, 264, 0, 440, FACING_SOUTH, 0, -1, -1, -1, -1, 0 },
    { 1, CONDITION_ALWAYS, 264, 0, 424, FACING_NORTH, 0, -1, -1, -1, -1, 0 },
    { 2, CONDITION_ALWAYS, 200, 0, 248, FACING_SOUTH, 0, -1, -1, -1, -1, 0 },
    { SCENE_TABLE_END },
};

s16 gBabiPalace_EmptyTable[] = {
    -1, 0, 0, 0, 0, 0, 0, 0,
};

u32 gBabiPalace_MessageTable[] = {
    SCENE_EXITS(138),
    SCENE_EXIT(1, 139, 2),
    SCENE_EXIT(2, 138, 24),
    SCENE_EXIT(3, 138, 25),
    SCENE_EXIT(4, 138, 20),
    SCENE_EXIT(5, 138, 22),
    SCENE_EXIT(6, 138, 8),
    SCENE_EXIT(7, 138, 9),
    SCENE_EXIT(8, 138, 6),
    SCENE_EXIT(9, 138, 7),
    SCENE_EXIT(10, 138, 26),
    SCENE_EXIT(11, 8, 19),
    SCENE_EXIT(12, 138, 27),
    SCENE_EXIT(13, 141, 1),
    SCENE_EXIT(16, 138, 34),
    SCENE_EXIT(17, 138, 18),
    SCENE_EXIT(18, 138, 17),
    SCENE_EXIT(20, 138, 4),
    SCENE_EXIT(21, 138, 40),
    SCENE_EXIT(22, 138, 5),
    SCENE_EXIT(23, 138, 41),
    SCENE_EXIT(24, 138, 2),
    SCENE_EXIT(25, 138, 3),
    SCENE_EXIT(26, 138, 10),
    SCENE_EXIT(27, 138, 12),
    SCENE_EXIT(28, 138, 42),
    SCENE_EXIT(29, 138, 43),
    SCENE_EXIT(30, 138, 35),
    SCENE_EXIT(31, 138, 36),
    SCENE_EXIT(32, 138, 33),
    SCENE_EXIT(33, 138, 32),
    SCENE_EXIT(34, 138, 16),
    SCENE_EXIT(35, 138, 30),
    SCENE_EXIT(36, 138, 31),
    SCENE_EXIT(40, 138, 21),
    SCENE_EXIT(41, 138, 23),
    SCENE_EXIT(42, 138, 28),
    SCENE_EXIT(43, 138, 29),
    SCENE_EXIT(44, 138, 45),
    SCENE_EXIT(45, 138, 44),
    SCENE_EXIT(0, 139, 0),
    SCENE_EXIT(1, 135, 11),
    SCENE_EXIT(2, 138, 1),
    SCENE_EXITS_END,
};

struct ScenePlacement gBabiPalacePlacements2[] = {
    { 0x98, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(232), 0, PIXELS(248), FACING_SOUTH, TALK_FACE_PARTY_AND_BACK, 0 },
    { 0x98, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(168), 0, PIXELS(248), FACING_SOUTH, TALK_FACE_PARTY_AND_BACK, 0 },
    { SCENE_TABLE_END },
};

struct ScenePlacement gBabiPalacePlacementsOther[] = {
    { GARET, CONDITION_ALWAYS, ACTOR_STAND,
        0, 0, 0, FACING_EAST, TALK_FACE_PARTY, 0 },
    { IVAN, CONDITION_ALWAYS, ACTOR_STAND,
        0, 0, 0, FACING_EAST, TALK_FACE_PARTY, 0 },
    { MIA, CONDITION_ALWAYS, ACTOR_STAND,
        0, 0, 0, FACING_EAST, TALK_FACE_PARTY, 0 },
    { 0x67, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(752), 0, PIXELS(136), FACING_SOUTHEAST, TALK_FACE_PARTY, 0 },
    { 0x6a, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(56), 0, PIXELS(120), FACING_SOUTH, TALK_FACE_PARTY, 0 },
    { 0x3e, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(392), 0, PIXELS(96), FACING_EAST, TALK_FACE_PARTY, 0 },
    { 0x3e, CONDITION_ALWAYS, (s32)gBabiPalace_MiddleActionScript,
        PIXELS(472), 0, PIXELS(136), FACING_NORTHEAST, TALK_FACE_PARTY, 0 },
    { 0x6c, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(88), 0, PIXELS(296), FACING_SOUTHEAST, TALK_FACE_PARTY, 0 },
    { 0x98, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(888), 0, PIXELS(344), FACING_EAST, TALK_FACE_PARTY, 0 },
    { 0x98, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(984), 0, PIXELS(440), FACING_NORTHEAST, TALK_FACE_PARTY_AND_BACK, 0 },
    { 0x98, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(984), 0, PIXELS(312), FACING_SOUTH + FACING_STEP, TALK_FACE_PARTY, 0 },
    { 0x98, CONDITION_ALWAYS, ACTOR_WANDER,
        PIXELS(944), 0, PIXELS(384), FACING_SOUTH, TALK_FACE_PARTY, 0 },
    { 0x67, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(920), 0, PIXELS(560), FACING_WEST, TALK_FACE_PARTY_AND_BACK, 0 },
    { 0x67, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(112), 0, PIXELS(552), FACING_SOUTH, TALK_FACE_PARTY, 0 },
    { 0x6f, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(728), 0, PIXELS(728), FACING_WEST, TALK_FACE_PARTY_AND_BACK, 0 },
    { 0x6, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(600), 0, PIXELS(744), FACING_SOUTHWEST, TALK_FACE_PARTY_AND_BACK, 0 },
    { 0x39, 0x96b, ACTOR_STAND,
        PIXELS(392), 0, PIXELS(360), FACING_EAST, TALK_KEEP_FACING, 0 },
    { 0x98, 0x96b, ACTOR_STAND,
        PIXELS(392), 0, PIXELS(352), FACING_EAST, TALK_KEEP_FACING, 0 },
    { 0x98, 0x96b, ACTOR_STAND,
        PIXELS(392), 0, PIXELS(368), FACING_EAST, TALK_KEEP_FACING, 0 },
    { SCENE_TABLE_END },
};

struct ScenePlacement gBabiPalacePlacementsColosso[] = {
    { GARET, CONDITION_ALWAYS, ACTOR_STAND,
        0, 0, 0, FACING_EAST, TALK_FACE_PARTY, 0 },
    { IVAN, CONDITION_ALWAYS, ACTOR_STAND,
        0, 0, 0, FACING_EAST, TALK_FACE_PARTY, 0 },
    { MIA, CONDITION_ALWAYS, ACTOR_STAND,
        0, 0, 0, FACING_EAST, TALK_FACE_PARTY, 0 },
    { 0x67, CONDITION_ALWAYS, ACTOR_WANDER,
        PIXELS(712), 0, PIXELS(136), FACING_SOUTH, TALK_FACE_PARTY, 0 },
    { 0x6c, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(472), 0, PIXELS(696), FACING_SOUTH, TALK_FACE_PARTY, 0 },
    { 0x6a, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(56), 0, PIXELS(120), FACING_SOUTH, TALK_FACE_PARTY, 0 },
    { 0x67, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(920), 0, PIXELS(560), FACING_WEST, TALK_FACE_PARTY_AND_BACK, 0 },
    { 0x67, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(112), 0, PIXELS(552), FACING_SOUTH, TALK_FACE_PARTY, 0 },
    { 0x6f, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(728), 0, PIXELS(744), FACING_WEST, TALK_FACE_PARTY_AND_BACK, 0 },
    { 0x6, CONDITION_ALWAYS, ACTOR_WANDER,
        PIXELS(600), 0, PIXELS(744), FACING_SOUTHWEST, TALK_FACE_PARTY, 0 },
    { SCENE_TABLE_END },
};

struct ScenePlacement gBabiPalacePlacementsAfterColosso[] = {
    { GARET, CONDITION_ALWAYS, ACTOR_STAND,
        0, 0, 0, FACING_EAST, TALK_FACE_PARTY, 0 },
    { IVAN, CONDITION_ALWAYS, ACTOR_STAND,
        0, 0, 0, FACING_EAST, TALK_FACE_PARTY, 0 },
    { MIA, CONDITION_ALWAYS, ACTOR_STAND,
        0, 0, 0, FACING_EAST, TALK_FACE_PARTY, 0 },
    { 0x67, CONDITION_ALWAYS, ACTOR_WANDER,
        PIXELS(712), 0, PIXELS(136), FACING_SOUTH, TALK_FACE_PARTY, 0 },
    { 0x6a, CONDITION_ALWAYS, ACTOR_WANDER,
        PIXELS(392), 0, PIXELS(112), FACING_SOUTH, TALK_FACE_PARTY, 0 },
    { 0x98, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(472), 0, PIXELS(696), FACING_SOUTH, TALK_FACE_PARTY, 0 },
    { 0x6c, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(88), 0, PIXELS(296), FACING_SOUTH, TALK_FACE_PARTY, 0 },
    { 0x98, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(984), 0, PIXELS(440), FACING_NORTHEAST, TALK_FACE_PARTY, 0 },
    { 0x98, CONDITION_ALWAYS, ACTOR_WANDER,
        PIXELS(944), 0, PIXELS(384), FACING_SOUTH, TALK_FACE_PARTY, 0 },
    { 0x34, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(888), 0, PIXELS(553), FACING_SOUTH, TALK_FACE_PARTY_AND_BACK, 0 },
    { 0x67, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(920), 0, PIXELS(560), FACING_WEST, TALK_FACE_PARTY_AND_BACK, 0 },
    { 0x16, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(856), 0, PIXELS(532), FACING_SOUTH, TALK_FACE_PARTY_AND_BACK, 0 },
    { 0x67, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(112), 0, PIXELS(552), FACING_SOUTH, TALK_FACE_PARTY, 0 },
    { 0x6f, CONDITION_ALWAYS, ACTOR_WANDER,
        PIXELS(712), 0, PIXELS(744), FACING_SOUTH, TALK_FACE_PARTY, 0 },
    { 0x16, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(8), 0, PIXELS(8), FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { 0x39, 0x96f, ACTOR_STAND,
        PIXELS(184), 0, PIXELS(704), FACING_SOUTHEAST, TALK_KEEP_FACING, 0 },
    { SCENE_TABLE_END },
};

struct SceneEvent gBabiPalaceEventsOther[] = {
    { EVENT_EXIT, 1, CONDITION_ALWAYS, 1 },
    { EVENT_EXIT, 2, CONDITION_ALWAYS, 2 },
    { EVENT_EXIT, 3, CONDITION_ALWAYS, 3 },
    { EVENT_EXIT, 4, CONDITION_ALWAYS, 4 },
    { EVENT_EXIT, 5, CONDITION_ALWAYS, 5 },
    { TOUCH_WALKING_INTO(FACING_NORTH), 6, CONDITION_ALWAYS, EVENT_SCRIPT(BabiPalace_LeaveThroughDoor) },
    { TOUCH_WALKING_INTO(FACING_NORTH), 7, CONDITION_ALWAYS, EVENT_SCRIPT(BabiPalace_LeaveThroughDoor) },
    { EVENT_EXIT, 8, CONDITION_ALWAYS, 8 },
    { EVENT_EXIT, 9, CONDITION_ALWAYS, 9 },
    { EVENT_EXIT, 10, CONDITION_ALWAYS, 10 },
    { 0x31, 11, CONDITION_ALWAYS, 11 },
    { EVENT_EXIT, 12, CONDITION_ALWAYS, 12 },
    { 0x31, 13, CONDITION_ALWAYS, 13 },
    { EVENT_EXIT, 16, CONDITION_ALWAYS, 16 },
    { TOUCH_WALKING_INTO(FACING_NORTH), 17, CONDITION_ALWAYS, EVENT_SCRIPT(BabiPalace_LeaveThroughDoor) },
    { EVENT_EXIT, 18, CONDITION_ALWAYS, 18 },
    { EVENT_EXIT, 20, CONDITION_ALWAYS, 20 },
    { EVENT_EXIT, 21, CONDITION_ALWAYS, 21 },
    { EVENT_EXIT, 22, CONDITION_ALWAYS, 22 },
    { EVENT_EXIT, 23, CONDITION_ALWAYS, 23 },
    { EVENT_EXIT, 24, CONDITION_ALWAYS, 24 },
    { EVENT_EXIT, 25, CONDITION_ALWAYS, 25 },
    { EVENT_EXIT, 26, CONDITION_ALWAYS, 26 },
    { EVENT_EXIT, 27, CONDITION_ALWAYS, 27 },
    { EVENT_EXIT, 28, CONDITION_ALWAYS, 28 },
    { EVENT_EXIT, 29, CONDITION_ALWAYS, 29 },
    { TOUCH_WALKING_INTO(FACING_NORTH), 30, CONDITION_ALWAYS, EVENT_SCRIPT(BabiPalace_LeaveThroughDoor) },
    { TOUCH_WALKING_INTO(FACING_NORTH), 31, CONDITION_ALWAYS, EVENT_SCRIPT(BabiPalace_LeaveThroughDoor) },
    { TOUCH_WALKING_INTO(FACING_NORTH), 32, CONDITION_ALWAYS, EVENT_SCRIPT(BabiPalace_LeaveThroughDoor) },
    { EVENT_EXIT, 33, CONDITION_ALWAYS, 33 },
    { EVENT_EXIT, 34, CONDITION_ALWAYS, 34 },
    { EVENT_EXIT, 35, CONDITION_ALWAYS, 35 },
    { EVENT_EXIT, 36, CONDITION_ALWAYS, 36 },
    { EVENT_EXIT, 40, CONDITION_ALWAYS, 40 },
    { EVENT_EXIT, 41, CONDITION_ALWAYS, 41 },
    { EVENT_EXIT, 42, CONDITION_ALWAYS, 42 },
    { EVENT_EXIT, 43, CONDITION_ALWAYS, 43 },
    { TOUCH_WALKING_INTO(FACING_NORTH), 44, CONDITION_ALWAYS, EVENT_SCRIPT(BabiPalace_LeaveThroughDoor) },
    { EVENT_EXIT, 45, CONDITION_ALWAYS, 45 },
    { EVENT_TOUCH, 60, 0x96b, EVENT_SCRIPT(BabiPalace_RunIodemDeparture) },
    { EVENT_TALK, 8, CONDITION_ALWAYS, EVENT_SCRIPT(BabiPalace_AskAboutSoldiers) },
    { EVENT_TALK, 12, CONDITION_ALWAYS, MSGNO(MsgBabiPalaceIfYoureEnteringColossoHurryAlong) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 8, CONDITION_ALWAYS, MSGNO(MsgBabiPalaceIWonderHowDiligentlyBabisSoldiers) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 12, CONDITION_ALWAYS, MSGNO(MsgBabiPalaceWarriorsAreNotAllowedToStay) },
    { EVENT_TALK, 9, CONDITION_ALWAYS, EVENT_SCRIPT(BabiPalace_MoveBedAttendant) },
    { EVENT_TALK, 10, CONDITION_ALWAYS, EVENT_SCRIPT(BabiPalace_AskAboutColosso) },
    { EVENT_TALK, 11, CONDITION_ALWAYS, EVENT_SCRIPT(BabiPalace_ShowIndigestionDialogue) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 9, CONDITION_ALWAYS, MSGNO(MsgBabiPalaceIWantToWatchTheColosso) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 10, CONDITION_ALWAYS, MSGNO(MsgBabiPalaceIGetSoAngrySeeingAll) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 11, CONDITION_ALWAYS, MSGNO(MsgBabiPalaceMaybeIShouldntHaveEatenQuite) },
    { EVENT_TALK, 13, CONDITION_ALWAYS, MSGNO(MsgBabiPalaceOnlyWarriorsWhoFightWellIn) },
    { EVENT_TALK, 14, CONDITION_ALWAYS, MSGNO(MsgBabiPalaceLordBabiIsBuildingTheLighthouse) },
    { EVENT_TALK, 15, CONDITION_ALWAYS, MSGNO(MsgBabiPalaceBabisNameIsRidingOnThe) },
    { EVENT_TALK, 16, CONDITION_ALWAYS, MSGNO(MsgBabiPalaceEvenIfLordBabiDoesntReturn) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 13, CONDITION_ALWAYS, MSGNO(MsgBabiPalaceMyFirstColossoWasSoExciting) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 14, CONDITION_ALWAYS, MSGNO(MsgBabiPalaceOnceTheLighthouseIsCompleteBabis) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 15, CONDITION_ALWAYS, MSGNO(MsgBabiPalaceWedNeverGetNearTheLighthouse) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 16, CONDITION_ALWAYS, MSGNO(MsgBabiPalaceIHearThatGirlIsGoing) },
    { EVENT_TALK, 17, CONDITION_ALWAYS, MSGNO(MsgBabiPalaceItLooksLikeLordBabiDidnt) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 17, CONDITION_ALWAYS, MSGNO(MsgBabiPalaceIGuessLordBabiDidNot) },
    { EVENT_TALK, 18, CONDITION_ALWAYS, MSGNO(MsgBabiPalaceActorHasBeenStayingAsLord) },
    { EVENT_TALK, 19, CONDITION_ALWAYS, MSGNO(MsgBabiPalaceActorWasReveredAsAChild) },
    { EVENT_TALK, 6, CONDITION_ALWAYS, MSGNO(MsgBabiPalaceBabiIsHoldingMeHostageIn) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 18, CONDITION_ALWAYS, MSGNO(MsgBabiPalaceActorWasTakenHostageToForce) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 19, CONDITION_ALWAYS, MSGNO(MsgBabiPalaceActorIsNotPermittedToLeave) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 6, CONDITION_ALWAYS, EVENT_SCRIPT(BabiPalace_ReadEscapeThoughts) },
    { SEARCH(SEARCH_UNNAMED), 70, IF_FLAG_SET(0x969), EVENT_SCRIPT(BabiPalace_RestInPalaceBeds) },
    { SEARCH(SEARCH_SHELF), 200, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgBabiPalaceTheArmorLooksHeavyItLooks)) },
    { SEARCH(SEARCH_SHELF), 201, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgBabiPalaceTheresNothingInThereItsCompletely)) },
    { SEARCH(SEARCH_SHELF), 202, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgBabiPalaceTheresAHeadbandItSaysNever)) },
    { SEARCH(SEARCH_BOOKCASE), 203, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgBabiPalaceTheresABookEntitledTheHeart)) },
    { SEARCH(SEARCH_BOOKCASE), 204, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgBabiPalaceTheresANotebookThatShowsThe)) },
    { SEARCH(SEARCH_BOOKCASE), 205, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgBabiPalaceTheresABookEntitledBecomeA)) },
    { SEARCH(SEARCH_BOOKCASE), 206, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgBabiPalaceHeyItsBabisDiaryItReads)) },
    { SEARCH(SEARCH_BARREL), 100, 0xf9f, FIND_ITEM(ITEM_LUCKY_MEDAL) },
    { SCENE_EVENTS_END },
};

struct SceneEvent gBabiPalaceEventsColosso[] = {
    { EVENT_EXIT, 1, CONDITION_ALWAYS, 1 },
    { EVENT_EXIT, 2, CONDITION_ALWAYS, 2 },
    { EVENT_EXIT, 3, CONDITION_ALWAYS, 3 },
    { EVENT_EXIT, 4, CONDITION_ALWAYS, 4 },
    { EVENT_EXIT, 5, CONDITION_ALWAYS, 5 },
    { TOUCH_WALKING_INTO(FACING_NORTH), 6, CONDITION_ALWAYS, EVENT_SCRIPT(BabiPalace_LeaveThroughDoor) },
    { TOUCH_WALKING_INTO(FACING_NORTH), 7, CONDITION_ALWAYS, EVENT_SCRIPT(BabiPalace_LeaveThroughDoor) },
    { EVENT_EXIT, 8, CONDITION_ALWAYS, 8 },
    { EVENT_EXIT, 9, CONDITION_ALWAYS, 9 },
    { EVENT_EXIT, 10, CONDITION_ALWAYS, 10 },
    { 0x31, 11, CONDITION_ALWAYS, 11 },
    { EVENT_EXIT, 12, CONDITION_ALWAYS, 12 },
    { 0x31, 13, CONDITION_ALWAYS, 13 },
    { EVENT_EXIT, 16, CONDITION_ALWAYS, 16 },
    { TOUCH_WALKING_INTO(FACING_NORTH), 17, CONDITION_ALWAYS, EVENT_SCRIPT(BabiPalace_LeaveThroughDoor) },
    { EVENT_EXIT, 18, CONDITION_ALWAYS, 18 },
    { EVENT_EXIT, 20, CONDITION_ALWAYS, 20 },
    { EVENT_EXIT, 21, CONDITION_ALWAYS, 21 },
    { EVENT_EXIT, 22, CONDITION_ALWAYS, 22 },
    { EVENT_EXIT, 23, CONDITION_ALWAYS, 23 },
    { EVENT_EXIT, 24, CONDITION_ALWAYS, 24 },
    { EVENT_EXIT, 25, CONDITION_ALWAYS, 25 },
    { EVENT_EXIT, 26, CONDITION_ALWAYS, 26 },
    { EVENT_EXIT, 27, CONDITION_ALWAYS, 27 },
    { EVENT_EXIT, 28, CONDITION_ALWAYS, 28 },
    { EVENT_EXIT, 29, CONDITION_ALWAYS, 29 },
    { TOUCH_WALKING_INTO(FACING_NORTH), 30, CONDITION_ALWAYS, EVENT_SCRIPT(BabiPalace_LeaveThroughDoor) },
    { TOUCH_WALKING_INTO(FACING_NORTH), 31, CONDITION_ALWAYS, EVENT_SCRIPT(BabiPalace_LeaveThroughDoor) },
    { TOUCH_WALKING_INTO(FACING_NORTH), 32, CONDITION_ALWAYS, EVENT_SCRIPT(BabiPalace_LeaveThroughDoor) },
    { EVENT_EXIT, 33, CONDITION_ALWAYS, 33 },
    { EVENT_EXIT, 34, CONDITION_ALWAYS, 34 },
    { EVENT_EXIT, 35, CONDITION_ALWAYS, 35 },
    { EVENT_EXIT, 36, CONDITION_ALWAYS, 36 },
    { EVENT_EXIT, 40, CONDITION_ALWAYS, 40 },
    { EVENT_EXIT, 41, CONDITION_ALWAYS, 41 },
    { EVENT_EXIT, 42, CONDITION_ALWAYS, 42 },
    { EVENT_EXIT, 43, CONDITION_ALWAYS, 43 },
    { TOUCH_WALKING_INTO(FACING_NORTH), 44, CONDITION_ALWAYS, EVENT_SCRIPT(BabiPalace_LeaveThroughDoor) },
    { EVENT_EXIT, 45, CONDITION_ALWAYS, 45 },
    { EVENT_TALK, 8, CONDITION_ALWAYS, MSGNO(MsgBabiPalaceBabisSoldiersHaveGoneToSee) },
    { EVENT_TALK, 9, CONDITION_ALWAYS, MSGNO(MsgBabiPalaceWhatAreYouDoingHurryOver) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 8, CONDITION_ALWAYS, MSGNO(MsgBabiPalaceThoseGuysLookReallyConfidentAbout) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 9, CONDITION_ALWAYS, MSGNO(MsgBabiPalaceTheFinalsAreAllThatsLeft) },
    { EVENT_TALK, 10, CONDITION_ALWAYS, EVENT_SCRIPT(BabiPalace_OfferRest) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 10, CONDITION_ALWAYS, MSGNO(MsgBabiPalaceIHopeYoullAllDoWell) },
    { EVENT_TALK, 11, CONDITION_ALWAYS, MSGNO(MsgBabiPalaceBabiHasReturnedAndTheFinals) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 11, CONDITION_ALWAYS, MSGNO(MsgBabiPalaceWhenBabiReturnedHeHeadedOff) },
    { EVENT_TALK, 12, CONDITION_ALWAYS, MSGNO(MsgBabiPalaceWhatsGoingOnActorIsLooking) },
    { EVENT_TALK, 13, CONDITION_ALWAYS, MSGNO(MsgBabiPalaceTheSoldiersCameToSeeActor) },
    { EVENT_TALK, 6, CONDITION_ALWAYS, MSGNO(MsgBabiPalaceHmphIWishColossoWouldHurry) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 12, CONDITION_ALWAYS, MSGNO(MsgBabiPalaceASmileOnAPrettyGirl) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 13, CONDITION_ALWAYS, MSGNO(MsgBabiPalaceActorIsAGoodGirlBut) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 6, CONDITION_ALWAYS, MSGNO(MsgBabiPalaceIDontHaveTheLeastInterest) },
    { SEARCH(SEARCH_UNNAMED), 70, IF_FLAG_SET(0x967), EVENT_SCRIPT(BabiPalace_RestInPalaceBeds) },
    { SEARCH(SEARCH_SHELF), 200, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgBabiPalaceTheArmorLooksHeavyItLooks)) },
    { SEARCH(SEARCH_SHELF), 201, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgBabiPalaceTheresNothingInThereItsCompletely)) },
    { SEARCH(SEARCH_SHELF), 202, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgBabiPalaceTheresAHeadbandItSaysNever)) },
    { SEARCH(SEARCH_BOOKCASE), 203, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgBabiPalaceTheresABookEntitledTheHeart)) },
    { SEARCH(SEARCH_BOOKCASE), 204, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgBabiPalaceTheresANotebookThatShowsThe)) },
    { SEARCH(SEARCH_BOOKCASE), 205, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgBabiPalaceTheresABookEntitledBecomeA)) },
    { SEARCH(SEARCH_BOOKCASE), 206, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgBabiPalaceHeyItsBabisDiaryItReads)) },
    { SEARCH(SEARCH_BARREL), 100, 0xf9f, FIND_ITEM(ITEM_LUCKY_MEDAL) },
    { SCENE_EVENTS_END },
};

struct SceneEvent gBabiPalaceEventsAfterColosso[] = {
    { EVENT_EXIT, 1, CONDITION_ALWAYS, 1 },
    { EVENT_EXIT, 2, CONDITION_ALWAYS, 2 },
    { EVENT_EXIT, 3, CONDITION_ALWAYS, 3 },
    { EVENT_EXIT, 4, CONDITION_ALWAYS, 4 },
    { EVENT_EXIT, 5, CONDITION_ALWAYS, 5 },
    { TOUCH_WALKING_INTO(FACING_NORTH), 6, CONDITION_ALWAYS, EVENT_SCRIPT(BabiPalace_LeaveThroughDoor) },
    { TOUCH_WALKING_INTO(FACING_NORTH), 7, CONDITION_ALWAYS, EVENT_SCRIPT(BabiPalace_LeaveThroughDoor) },
    { EVENT_EXIT, 8, CONDITION_ALWAYS, 8 },
    { EVENT_EXIT, 9, CONDITION_ALWAYS, 9 },
    { EVENT_EXIT, 10, CONDITION_ALWAYS, 10 },
    { 0x31, 11, CONDITION_ALWAYS, 11 },
    { EVENT_EXIT, 12, CONDITION_ALWAYS, 12 },
    { 0x31, 13, CONDITION_ALWAYS, 13 },
    { EVENT_EXIT, 16, CONDITION_ALWAYS, 16 },
    { TOUCH_WALKING_INTO(FACING_NORTH), 17, CONDITION_ALWAYS, EVENT_SCRIPT(BabiPalace_LeaveThroughDoor) },
    { EVENT_EXIT, 18, CONDITION_ALWAYS, 18 },
    { EVENT_EXIT, 20, CONDITION_ALWAYS, 20 },
    { EVENT_EXIT, 21, CONDITION_ALWAYS, 21 },
    { EVENT_EXIT, 22, CONDITION_ALWAYS, 22 },
    { EVENT_EXIT, 23, CONDITION_ALWAYS, 23 },
    { EVENT_EXIT, 24, CONDITION_ALWAYS, 24 },
    { EVENT_EXIT, 25, CONDITION_ALWAYS, 25 },
    { EVENT_EXIT, 26, CONDITION_ALWAYS, 26 },
    { EVENT_EXIT, 27, CONDITION_ALWAYS, 27 },
    { EVENT_EXIT, 28, CONDITION_ALWAYS, 28 },
    { EVENT_EXIT, 29, CONDITION_ALWAYS, 29 },
    { TOUCH_WALKING_INTO(FACING_NORTH), 30, CONDITION_ALWAYS, EVENT_SCRIPT(BabiPalace_LeaveThroughDoor) },
    { TOUCH_WALKING_INTO(FACING_NORTH), 31, CONDITION_ALWAYS, EVENT_SCRIPT(BabiPalace_LeaveThroughDoor) },
    { TOUCH_WALKING_INTO(FACING_NORTH), 32, CONDITION_ALWAYS, EVENT_SCRIPT(BabiPalace_LeaveThroughDoor) },
    { EVENT_EXIT, 33, CONDITION_ALWAYS, 33 },
    { EVENT_EXIT, 34, CONDITION_ALWAYS, 34 },
    { EVENT_EXIT, 35, CONDITION_ALWAYS, 35 },
    { EVENT_EXIT, 36, CONDITION_ALWAYS, 36 },
    { EVENT_EXIT, 40, CONDITION_ALWAYS, 40 },
    { EVENT_EXIT, 41, CONDITION_ALWAYS, 41 },
    { EVENT_EXIT, 42, CONDITION_ALWAYS, 42 },
    { EVENT_EXIT, 43, CONDITION_ALWAYS, 43 },
    { TOUCH_WALKING_INTO(FACING_NORTH), 44, CONDITION_ALWAYS, EVENT_SCRIPT(BabiPalace_LeaveThroughDoor) },
    { EVENT_EXIT, 45, CONDITION_ALWAYS, 45 },
    { EVENT_TALK, 8, CONDITION_ALWAYS, EVENT_SCRIPT(BabiPalace_AskAboutMeetingBabi) },
    { EVENT_TALK, 11, CONDITION_ALWAYS, EVENT_SCRIPT(BabiPalace_AskAboutEasternShores) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 8, CONDITION_ALWAYS, MSGNO(MsgBabiPalaceIHaventSeenMuchOfBabi) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 11, CONDITION_ALWAYS, MSGNO(MsgBabiPalaceItWouldBeAWasteTo) },
    { EVENT_TALK, 9, CONDITION_ALWAYS, MSGNO(MsgBabiPalaceNoOnesGoingToSleepIn) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 9, CONDITION_ALWAYS, MSGNO(MsgBabiPalaceWouldThisRoomGetVeryDusty) },
    { EVENT_TALK, 10, CONDITION_ALWAYS, MSGNO(MsgBabiPalaceBabiWasFoundSafeAndSound) },
    { EVENT_TALK, 12, CONDITION_ALWAYS, MSGNO(MsgBabiPalaceNowThatColossoIsOverIm) },
    { EVENT_TALK, 13, CONDITION_ALWAYS, MSGNO(MsgBabiPalaceTheSouthernContinentIsADangerous) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 10, CONDITION_ALWAYS, MSGNO(MsgBabiPalaceIWasHopingIdBeThe) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 12, CONDITION_ALWAYS, MSGNO(MsgBabiPalaceColossoHasEndedSoWeHave) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 13, CONDITION_ALWAYS, MSGNO(MsgBabiPalaceIWantThatLighthouseFinishedQuickly) },
    { EVENT_TALK, 14, IF_FLAG_SET(0xf31), MSGNO(MsgBabiPalaceIfYouUseTheCloakBall) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 14, IF_FLAG_SET(0xf31), MSGNO(MsgBabiPalaceWhyWouldAnAdeptLikeYou) },
    { EVENT_TALK, 14, CONDITION_ALWAYS, MSGNO(MsgBabiPalaceSleepingALotHelpsToDelay) },
    { EVENT_TALK, 15, CONDITION_ALWAYS, MSGNO(MsgBabiPalaceBabisFaceIsVeryPaleLike) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 14, CONDITION_ALWAYS, MSGNO(MsgBabiPalaceIWishIHadMetYou) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 15, CONDITION_ALWAYS, MSGNO(MsgBabiPalaceIfSomethingHappensToBabiWhat) },
    { EVENT_TALK, 16, CONDITION_ALWAYS, EVENT_SCRIPT(BabiPalace_OfferCloakBall) },
    { EVENT_TALK, 17, CONDITION_ALWAYS, MSGNO(MsgBabiPalaceTheGirlFromLaliveroIsNo) },
    { EVENT_TALK, 18, CONDITION_ALWAYS, MSGNO(MsgBabiPalaceIAmSurprisedActorWasAllowed) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 17, CONDITION_ALWAYS, MSGNO(MsgBabiPalaceItSeemedOnlyNaturalThatActor) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 18, CONDITION_ALWAYS, MSGNO(MsgBabiPalaceIAmSoRelievedActorHas) },
    { SEARCH(SEARCH_UNNAMED), 70, CONDITION_ALWAYS, EVENT_SCRIPT(BabiPalace_RestInPalaceBeds) },
    { SEARCH(SEARCH_SHELF), 200, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgBabiPalaceTheArmorLooksHeavyItLooks)) },
    { SEARCH(SEARCH_SHELF), 201, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgBabiPalaceTheresNothingInThereItsCompletely)) },
    { SEARCH(SEARCH_SHELF), 202, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgBabiPalaceTheresAHeadbandItSaysNever)) },
    { SEARCH(SEARCH_BOOKCASE), 203, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgBabiPalaceTheresABookEntitledTheHeart)) },
    { SEARCH(SEARCH_BOOKCASE), 204, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgBabiPalaceTheresANotebookThatShowsThe)) },
    { SEARCH(SEARCH_BOOKCASE), 205, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgBabiPalaceTheresABookEntitledBecomeA)) },
    { SEARCH(SEARCH_BOOKCASE), 206, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgBabiPalaceHeyItsBabisDiaryItReads)) },
    { SEARCH(SEARCH_BARREL), 100, 0xf9f, FIND_ITEM(ITEM_LUCKY_MEDAL) },
    { SCENE_EVENTS_END },
};

struct SceneEvent gBabiPalaceEvents2[] = {
    { EVENT_EXIT, 1, CONDITION_ALWAYS, 1 },
    { TOUCH_WALKING_INTO(FACING_NORTH), 2, CONDITION_ALWAYS, EVENT_SCRIPT(BabiPalace_LeaveThroughAnimatedDoor) },
    { EVENT_TALK, 8, CONDITION_ALWAYS, MSGNO(MsgBabiPalaceNormallyNoOnesAllowedIntoBabis) },
    { EVENT_TALK, 9, CONDITION_ALWAYS, MSGNO(MsgBabiPalaceSomeOfTheRoomsInThe) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 8, CONDITION_ALWAYS, MSGNO(MsgBabiPalaceShouldThePalaceReallyBeOpen) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 9, CONDITION_ALWAYS, MSGNO(MsgBabiPalaceIWonderHowManyPeopleWill) },
    { SEARCH(SEARCH_UNNAMED), 90, CONDITION_ALWAYS, EVENT_SCRIPT(BabiPalace_ExamineWell) },
    { SCENE_EVENTS_END },
};

struct SceneEvent gBabiPalaceEvents2Colosso[] = {
    { EVENT_EXIT, 1, CONDITION_ALWAYS, 1 },
    { TOUCH_WALKING_INTO(FACING_NORTH), 2, CONDITION_ALWAYS, EVENT_SCRIPT(BabiPalace_LeaveThroughAnimatedDoor) },
    { EVENT_TALK, 8, 0x96c, EVENT_SCRIPT(BabiPalace_AskPlayerIdentity) },
    { EVENT_TALK, 9, 0x96c, EVENT_SCRIPT(BabiPalace_AskPlayerIdentity) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 0x408, 0x96c, EVENT_SCRIPT(BabiPalace_AskPlayerIdentity) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 0x409, 0x96c, EVENT_SCRIPT(BabiPalace_AskPlayerIdentity) },
    { EVENT_TALK, 8, CONDITION_ALWAYS, MSGNO(MsgBabiPalaceBabiWantsYouToGoStraight) },
    { EVENT_TALK, 9, CONDITION_ALWAYS, EVENT_SCRIPT(BabiPalace_ThankForSavingBabi) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 8, CONDITION_ALWAYS, MSGNO(MsgBabiPalaceSureTheySavedBabiButIm) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 9, CONDITION_ALWAYS, MSGNO(MsgBabiPalaceBabiIsADeliberateManHe) },
    { SEARCH(SEARCH_UNNAMED), 90, CONDITION_ALWAYS, EVENT_SCRIPT(BabiPalace_ExamineWell) },
    { SCENE_EVENTS_END },
};

struct SceneEvent gBabiPalaceEvents2AfterColosso[] = {
    { EVENT_EXIT, 1, CONDITION_ALWAYS, 1 },
    { TOUCH_WALKING_INTO(FACING_NORTH), 2, CONDITION_ALWAYS, EVENT_SCRIPT(BabiPalace_LeaveThroughAnimatedDoor) },
    { EVENT_TALK, 8, CONDITION_ALWAYS, MSGNO(MsgBabiPalaceIveLostAllMyEnthusiasmSince) },
    { EVENT_TALK, 9, CONDITION_ALWAYS, EVENT_SCRIPT(BabiPalace_AskAboutRemainingWarriors) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 8, CONDITION_ALWAYS, MSGNO(MsgBabiPalaceTheEndOfAFestivalIs) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 9, CONDITION_ALWAYS, MSGNO(MsgBabiPalaceAllTheHeroicWarriorsWhoTook) },
    { SEARCH(SEARCH_UNNAMED), 90, CONDITION_ALWAYS, EVENT_SCRIPT(BabiPalace_ExamineWell) },
    { SCENE_EVENTS_END },
};
// clang-format on
