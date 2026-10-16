#include "game_state.h"
#include "window.h"
#include "unit/scene/vault_revisit.h"
#include "unit/scene/common/vault_range.h"
#include "unit/scene/common/effect_spawn.h"
#include "unit/object/object.h"
#include "unit/object/psynergy_effects.h"
#include "unit/game/game.h"
#include "unit/runtime/runtime.h"
#include "field_obj.h"
#include "types.h"
#include "field_event.h"
#include "field_scene.h"
#include "battle.h"
#include "debug.h"
#include "field.h"
#include "game.h"
#include "graphics.h"
#include "menu.h"
#include "minigame.h"
#include "sound.h"
#include "system.h"
#include "iwram_call.h"
#include "tbs_edition.h"
#include "inventory.h"
#include "text/msg_ids.h"
#include "edition.h"
#include "call.h"

// Shared effect and proximity routines precede the scene's entry points.
#include "common/effect_create.h"
#include "common/effect_spawn.h"
#include "common/object_distance.h"
#include "common/vault_range.h"
#include "common/vault_nearby.h"

// Return the revisited town's entrance table through the existing overlay ABI.
const void *SceneData_GetScriptTable(void) { return gVaultRevisit_Scripts; }

// This overlay has no clipping regions.
int SceneData_ReturnZero(void) { return 0; }

// The legacy message-table entry actually returns packed scene exits.
const void *SceneData_GetMessageTable(void) { return gVaultRevisit_Messages; }

// Return the villagers, dog, and Djinni placements.
const void *SceneData_GetActorTable(void) { return gVaultRevisit_Actors; }

// Stand the actor still while showing the message already selected.
static void VaultRevisit_ShowStandingActorMessage(s32 actor) {
  Engine_EventBegin();
  Engine_ActorSetAnimation(actor, 1);
  Engine_EventShowMessage(actor, 0);
  Engine_EventEnd();
}

// Record the villager's recognition of the warriors who caught the thieves.
static void EV08_TALK(void) {
  Engine_EventBegin();
  Engine_ActorFaceActor(8, ACTOR_PARTY_LEADER, 2);
  Engine_GameFlagSet(0x305);
  Engine_EventSetMessage(MsgVaultWarriorsWhoCaptured);
  Engine_EventShowMessage(8, 0);
  Engine_EventEnd();
}

// Discuss the rumor about Lunpa's thieves while facing the leader.
static void EV11_TALK(void) {

  Engine_EventSetMessage(MsgVaultRumorsThievesLunpa);
  Engine_ActorFaceEachOther(11, ACTOR_PARTY_LEADER, 2);
  VaultRevisit_ShowStandingActorMessage(11);
}

// Record the conversation about the mayor and the escaped thieves.
static void EV12_TALK(void) {

  Engine_EventBegin();
  Engine_ActorFaceActor(12, ACTOR_PARTY_LEADER, 2);
  Engine_GameFlagSet(0x306);
  Engine_GameFlagSet(0x868);
  Engine_EventSetMessage(MsgVaultTalkingMayorStrong);
  Engine_EventShowMessage(12, 0);
  Engine_EventEnd();
}

// Face the leader while discussing the volcano's continued calm.
static void EV13_TALK(void) {

  Engine_EventSetMessage(MsgVaultVolcanoStaysCalm);
  Engine_ActorFaceEachOther(13, ACTOR_PARTY_LEADER, 2);
  VaultRevisit_ShowStandingActorMessage(13);
}

// Pause the proximity reaction, speak, then restore this actor's facing.
static void EV14_TALK(void) {
  struct FieldActor *actor = Actor_Get(14);
  u16 facing = actor->facing;
  s32 text;

  actor->timer |= 2;
  Engine_EventBegin();
  text = MsgVaultWarriorGuy;
  Engine_EventSetMessage(text);
  Engine_ActorSetAnimation(14, 0);
  Engine_ActorFaceEachOther(14, ACTOR_PARTY_LEADER, 2);
  if (GameFlag_IsSet(0x300) == 0) {
    Actor_ShowEmote(14, 256, 60);
    Event_ShowMessageAndWait(14, 0, 10);
    Event_ShowMessageAndWait(14, 0, 10);
    GameFlag_Set(0x300);
  }
  Engine_EventSetMessage(text + 2);
  Event_ShowMessageAndWait(14, 0, 10);
  actor->facing = facing;
  Engine_TaskWait(1);
  Engine_EventEnd();
  actor->timer = 1;
  GameFlag_Set(0x307);
}

// Preserve the actor's facing around its conversation about the elders.
static void EV15_TALK(void) {
  struct FieldActor *actor;
  s16 originalFacing;

  actor = Object_GetById(15);
  originalFacing = actor->facing;
  actor->timer |= 2;
  Engine_EventBegin();
  Engine_EventSetMessage(MsgVaultWishVaultsElders);
  Engine_ActorSetAnimation(15, 0);
  Engine_ActorFaceEachOther(15, ACTOR_PARTY_LEADER, 2);
  Engine_EventShowMessageAndWait(15, 0, 10);
  actor->facing = originalFacing;
  Engine_TaskWait(1);
  Engine_EventEnd();
  actor->timer = 0;
}

// Ask the leader to check the jail and record that the request was heard.
static void EV16_TALK(void) {
  Engine_EventBegin();
  Engine_EventSetMessage(MsgVaultGuysCheckJail);
  Engine_ActorFaceEachOther(16, ACTOR_PARTY_LEADER, 2);
  Engine_EventOpenMessage(16, 0);
  if (Engine_EventChooseYesNo(0, 0) != 0) {
    ((struct BranchCounterWork *)gEventWork)->branchCounter += 1;
  }
  Engine_EventShowMessage(16, 0);
  Engine_GameFlagSet(0x308);
  Engine_EventEnd();
}

// Mind Read on the unacknowledged villager first triggers recognition.
static void EV1032_PSYNERGY(void) {
  Engine_EventBegin();
  Engine_ActorRunRepeatedMotion(8, 1);
  Engine_EventWait(20);
  Engine_ActorFaceActor(8, ACTOR_PARTY_LEADER, 20);
  Engine_GameFlagSet(0x305);
  Engine_EventSetMessage(MsgVaultWarriorsWhoCaptured);
  Engine_EventShowMessageAndWait(8, 0, 20);
  Engine_EventEnd();
}

// Temporarily mark actor 11 while showing its thoughts about Dodonpa.
static void EV11_PSYNERGY(void) {
  Engine_EventSetMessage(MsgVaultOnceDodonpaTook);
  ((struct AcceptActor11 *)Object_GetById(11))->accepted = 1;
  VaultRevisit_ShowStandingActorMessage(11);
  ((struct AcceptActor11 *)Object_GetById(11))->accepted = 0;
}

// Mind Read first acknowledges the conversation about the mayor.
static void EV1036_PSYNERGY(void) {
  Engine_EventBegin();
  Engine_ActorRunRepeatedMotion(12, 1);
  Engine_EventWait(20);
  Engine_ActorFaceActor(12, ACTOR_PARTY_LEADER, 20);
  Engine_GameFlagSet(0x306);
  Engine_GameFlagSet(0x868);
  Engine_EventSetMessage(MsgVaultTalkingMayorStrong);
  Engine_EventShowMessageAndWait(12, 0, 20);
  Engine_EventEnd();
}

// Temporarily mark actor 13 while showing its thoughts about the resolved
// crisis.
static void EV13_PSYNERGY(void) {
  Engine_EventSetMessage(MsgVaultGreatEverythingSolved);
  ((struct AcceptActor13 *)Object_GetById(13))->accepted = 1;
  VaultRevisit_ShowStandingActorMessage(13);
  ((struct AcceptActor13 *)Object_GetById(13))->accepted = 0;
}

// Mind Read reuses the first conversation until actor 14 has spoken once.
static void VaultRevisit_RunActor14Talk(void) {
  Actor_Get(14)->timer |= 2;
  Engine_EventBegin();
  if (GameFlag_IsSet(0x307) != 0) {
    Engine_EventSetMessage(MsgVaultWatchingGuysMakes);
    VaultRevisit_ShowStandingActorMessage(14);
  } else {
    EV14_TALK();
    GameFlag_Set(0x307);
  }
  Engine_EventEnd();
  Actor_Get(14)->timer = 1;
}

// Pause the proximity reaction while reading actor 15's thoughts.
static void EV15_PSYNERGY(void) {
  {
    struct FieldActor *actor = Object_GetById(15);
    actor->timer |= 2;
  }
  Engine_EventBegin();
  Engine_EventSetMessage(MsgVaultGrownUpsAlways);
  VaultRevisit_ShowStandingActorMessage(15);
  Engine_EventEnd();
  {
    s32 clear = 0;
    struct FieldActor *actor = Object_GetById(15);
    actor->timer = clear;
  }
}

// Mind Read introduces the jail request once, then shows its later thought.
static void VaultRevisit_ReadJailRumor(void) {
  s32 requestHeard;
  s32 v5;

  requestHeard = GameFlag_IsSet(0x308);
  if (requestHeard == 0) {
    Engine_EventBegin();
    Actor_Get(16)->paused = 1;
    Engine_ActorSetAnimation(16, 1);
    Engine_ActorRunRepeatedMotion(16, 1);
    Engine_EventWait(20);
    Engine_EventSetMessage(MsgVaultGuysCheckJail);
    Engine_ActorFaceEachOther(16, 0, 2);
    Event_OpenMessage(16, 0);
    if (Engine_EventChooseYesNo(0, 0) != 0) {
      gEventWork->message++;
    }
    Event_ShowMessage(16, 0);
    Actor_Get(16)->paused = requestHeard;
    Actor_SetBuiltinAction(16, 2);
    Engine_EventEnd();
    GameFlag_Set(0x308);
  } else {
    Engine_EventSetMessage(MsgVaultNotLikeEasy);
    Actor_Get(16)->paused = 1;
    VaultRevisit_ShowStandingActorMessage(16);
    v5 = 0;
    Actor_Get(16)->paused = v5;
  }
}

// Give the actor vertical velocity, move it, and wait briefly for it to settle.
static void ActorPresentation_MoveActorToPositionAndWait(int actor, int x,
                                                         int z, int velocityY) {
  struct FieldActor *jumpingActor = Object_GetById(actor);
  int frames;
  Engine_ActorSetSpeed(actor, 0x30000, 0x18000);
  jumpingActor->gravity = 0x8000;
  jumpingActor->bounce = 0;
  jumpingActor->velocityY = velocityY;
  Engine_ActorSetSpriteFlags(jumpingActor, 0);
  Engine_ActorMoveToAndWait(actor, x, z);
  Engine_ActorSetPosition(actor, x << 16, z << 16);
  for (frames = 60; frames != 0; --frames) {
    Engine_TaskWait(1);
    if ((s16)((u32)jumpingActor->velocityY >> 16) == 0)
      break;
  }
  Engine_ActorSetSpriteFlags(jumpingActor, 1);
  jumpingActor->gravity = 0x10000;
}

// Ringing the bell makes the Djinni jump out and move to its new position once.
static void EV30_SEARCH(void) {
  Engine_EventBegin();
  Event_PlayCue(100);
  Engine_EventWait(40);
  if (GameFlag_IsSet(0x867) == 0) {
    Actor_SetAttachedEffect(21, 0x102);
    Engine_ActorJump(21, 4, 0);
    Engine_EventWait(12);
    Engine_ActorJump(21, 4, 0);
    Engine_EventWait(20);
    ActorPresentation_MoveActorToPositionAndWait(21, 0x188, 104, 0x70000);
    Engine_EventWait(20);
    Actor_WalkToAndWait(21, 0x198, 104);
    Actor_WalkToAndWait(21, 0x198, 120);
    GameFlag_Set(0x867);
  }
  Engine_EventEnd();
}

// Walk the leader into a doorway during the sixteen-frame exit delay.
static void Vault_WalkThroughExit(s32 x, s32 z, s32 exitId) {
  s32 zero = 0;

  Actor_SetSpeed(zero, 0x8000, 0x4000);
  Actor_WalkTo(zero, x, z);
  gEventWork->transitionFrames = 16;
  Engine_EventRequestExit(exitId);
}

// Open the doorway at trigger 5 and walk through it.
static void EV05_TOUCH(void) {
  Engine_AudioPlayCue(158);
  Engine_MapAnimateCells(gVaultRevisit_Scene5Cells, 56, 19);
  Vault_WalkThroughExit(408, 320, 5);
}

// Open the doorway at trigger 6 and walk through it.
static void EV06_TOUCH(void) {
  Engine_AudioPlayCue(158);
  Engine_MapAnimateCells(gVaultRevisit_Scene6Cells, 50, 18);
  Vault_WalkThroughExit(312, 304, 6);
}

// Open the doorway at trigger 7 and walk through it.
static void EV07_TOUCH(void) {
  Engine_AudioPlayCue(158);
  Engine_MapAnimateCells(gVaultRevisit_Scene7Cells, 44, 17);
  Vault_WalkThroughExit(216, 288, 7);
}

// Open trigger 8's doorway and update the leader's drawing flags for entry.
static void EV08_TOUCH(void) {
  struct PresentedActor *actor = Object_GetById(ACTOR_PARTY_LEADER);
  struct Presentation *presentation = actor->presentation;
  u8 flags;

  Engine_AudioPlayCue(158);
  Engine_MapAnimateCells(gVaultRevisit_Scene8Cells, 54, 13);
  {

    Map_CopyCellAttributes(33, 20, 1, 3, 23, 12);
  }
  actor->state23 &= ~1;
  flags = presentation->flags;
  flags |= 12;
  presentation->flags = flags;
  Vault_WalkThroughExit(376, 224, 8);
}

// Open trigger 9's doorway and update the leader's drawing flags for entry.
static void EV09_TOUCH(void) {
  struct PresentedActor *actor = Object_GetById(ACTOR_PARTY_LEADER);
  struct Presentation *presentation = actor->presentation;
  u8 flags;

  Engine_AudioPlayCue(158);
  Engine_MapAnimateCells(gVaultRevisit_Scene9Cells, 49, 10);
  {

    Map_CopyCellAttributes(33, 20, 1, 3, 18, 10);
  }
  actor->state23 &= ~1;
  flags = presentation->flags;
  flags |= 12;
  presentation->flags = flags;
  Vault_WalkThroughExit(296, 176, 9);
}

// Open the doorway at trigger 10 and walk through it.
static void EV10_TOUCH(void) {
  Engine_AudioPlayCue(158);
  Engine_MapAnimateCells(gVaultRevisit_Scene10Cells, 38, 6);
  Vault_WalkThroughExit(120, 144, 10);
}

// The remaining doorway helper retains its position in the overlay.
#include "common/vault_scene_cells.h"
#include "ability_ids.h"
#include "item_ids.h"
#include "objcmd.h"

// Open trigger 12's doorway and update the leader's drawing flags for entry.
static void EV12_TOUCH(void) {
  struct PresentedActorB *actor = Object_GetById(ACTOR_PARTY_LEADER);
  struct PresentationB *presentation = actor->presentation;
  u8 flags;

  Engine_AudioPlayCue(158);
  Engine_MapAnimateCells(gVaultRevisit_Scene12Cells, 35, 9);
  {

    Map_CopyCellAttributes(33, 20, 1, 3, 4, 10);
  }
  actor->state23 &= ~1;
  flags = presentation->flags;
  flags |= 12;
  presentation->flags = flags;
  Vault_WalkThroughExit(72, 160, 12);
}

// Shared entrance toggles precede the Djinni and dog events.
#include "common/vault_toggle.h"

// Join the Venus Djinni in actor slot 21 to the party.
static void EV21_TALK(void) { Djinn_JoinParty(21, 0, 4); }

// Consume the offered bone and make the dog indicate the hidden passage.
static void EV18_SCRIPT(void) {
  PartyInventory_Discard(ITEM_BONE);
  Engine_EventBegin();
  Engine_EventWait(10);
  Engine_ActorRunRepeatedMotion(18, 2);
  Actor_SetSpeed(18, 0xcccc, 0x6666);
  Actor_WalkToAndWait(18, 216, 0x198);
  Engine_EventWait(10);
  Actor_FaceDirection(18, 0x4000, 20);
  Engine_ActorJump(18, 6, 0);
  Engine_EventWait(30);
  Engine_ActorJump(18, 6, 0);
  Engine_EventWait(30);
  Engine_ActorJump(18, 6, 0);
  Engine_EventWait(30);
  Actor_WalkToAndWait(18, 216, 0x188);
  Engine_EventWait(10);
  Actor_FaceDirection(18, 0x4000, 20);
  GameFlag_Set(0x858);
  Engine_EventEnd();
}

// Cover the hidden passage when Reveal ends.
static void ActorPresentation_SetPairedSceneCells(void) {

  Map_CopyCellAttributes(41, 43, 1, 1, 13, 25);
  Map_CopyCellsTo(40, 42, 12, 22, 3, 3);
}

// Restore the passage's visible tiles and collision cell during Reveal.
static void ActorPresentation_SetAlternatePairedSceneCells(void) {

  Map_CopyCellAttributes(37, 43, 1, 1, 13, 25);
  Map_CopyCellsTo(36, 42, 12, 22, 3, 3);
}

// Bark at the leader and offer the bone interaction if the party has one.
static void EV18_TALK(void) {
  Engine_EventBegin();
  Engine_EventSetMessage(MsgVaultRuffRrruff2);
  Engine_ActorSetAnimation(18, 0);
  Engine_ActorFaceEachOther(18, ACTOR_PARTY_LEADER, 0);
  Engine_EventWait(2);
  Engine_EventShowMessage(18, 0);
  Engine_ActorSetAnimation(18, 1);

  if (PartyInventory_FindOwner(ITEM_BONE) != -1 &&
      Engine_GameFlagIsSet(0x858) == 0) {
    ((struct Actor18MarkerWork *)gEventWork)->actor18Marker = 1;
  }

  Engine_EventEnd();
}

// Describe the water heard at the bottom of the well.
static void EV90_SEARCH(void) {
  Engine_EventBegin();
  Engine_MessageShowCentered(MsgFieldPeeredWell, 1);
  Engine_MessageShowCentered(MsgVaultCanHearWaterRumblingDown, 1);
  Engine_EventEnd();
}

// Return the town's talk, search, Psynergy, and doorway events.
const u8 *SceneData_GetExtraTable(void) {
  return (const u8 *)gVaultRevisit_Extras;
}

// Restore dog and Djinni progress, proximity reactions, and entrance flags.
s32 SceneSetup_InitializeActorsAndFlags(void) {

  struct FieldActor *actor;
  s32 mode;

  if (Engine_GameFlagIsSet(0x200))
    Map_CopyCellAttributes(55, 26, 4, 2, 23, 26);
  OverlayObject_CreateConfiguredObject(0x800000, 0, 0x1a40000, 223);
  Map_CopyCellsTo(45, 41, 8, 45, 3, 3);
  Engine_TaskWait(1);
  actor = Object_GetById(14);
#ifdef UBFIX
  actor->update = SceneActor_UpdateProximityToLeader;
#else
  actor->update =
      (void (*)(union FieldObject *))SceneActor_UpdateProximityToLeader;
#endif
  {
    struct FieldActor *actor = Object_GetById(14);
    s32 mode = 1;
    actor->timer = mode;
  }
  mode = 0;
  actor = Object_GetById(15);
#ifdef UBFIX
  actor->update = SceneActor_UpdateProximityToLeader;
#else
  actor->update =
      (void (*)(union FieldObject *))SceneActor_UpdateProximityToLeader;
#endif
  Object_GetById(15)->timer = mode;
  if (Engine_GameFlagIsSet(0x858))
    Actor_SetPosition(18, 0xd80000, 0x1880000);
  if (gGameState.entrance <= 2 && !Engine_GameFlagIsSet(52)
#if TBS_PATCH_1
      && !Engine_GameFlagIsSet(0x109)
#endif
  )
    Engine_GameFlagClear(0x867);
  if (Engine_GameFlagIsSet(0x867) && !Engine_GameFlagIsSet(52))
    Actor_SetPosition(21, 0x1980000, 0x780000);
  if (gGameState.entrance == 11)
    Engine_GameFlagClear(FLAG_ARRIVAL_EVENT_PENDING);
  if (gGameState.entrance == 13)
    Engine_GameFlagClear(0x120);
  return 0;
}

s32 gEffectScript0[] = {
    SC_WAIT(1),      SC_LOOP(22, SC_TOP), SC_WAIT(1),
    SC_POS(0, 0, 0), SC_WAIT(1),          SC_FREE,
};

s32 gEffectScript1[] = {
    SC_WAIT(1),      SC_LOOP(44, SC_TOP), SC_WAIT(1),
    SC_POS(0, 0, 0), SC_WAIT(1),          SC_FREE,
};

s32 gEffectScript2[] = {
    SC_WAIT(1),      SC_LOOP(126, SC_TOP), SC_WAIT(1),
    SC_POS(0, 0, 0), SC_WAIT(1),           SC_FREE,
};

const s32 *gEffectScripts[] = {
    gEffectScript0,
    gEffectScript1,
    gEffectScript2,
};

s32 gVaultRevisit_Actor0[] = {
    SC_ARG(2),
    SC_SET(SW_SPEED, 0xcccc),
    SC_SET(SW_ACCEL, 0x3333),
    SC_LABEL(1),
    SC_MOVE(PIXELS(136), 0, PIXELS(376)),
    SC_SYNC,
    SC_MOVE(PIXELS(120), 0, PIXELS(376)),
    SC_SYNC,
    SC_MOVE(PIXELS(104), 0, PIXELS(392)),
    SC_SYNC,
    SC_MOVE(PIXELS(104), 0, PIXELS(408)),
    SC_SYNC,
    SC_MOVE(PIXELS(120), 0, PIXELS(424)),
    SC_SYNC,
    SC_MOVE(PIXELS(136), 0, PIXELS(424)),
    SC_SYNC,
    SC_MOVE(PIXELS(152), 0, PIXELS(408)),
    SC_SYNC,
    SC_MOVE(PIXELS(152), 0, PIXELS(392)),
    SC_SYNC,
    SC_LOOP(SC_FOREVER, SC_TO(1)),
    SC_END,
};

s32 gVaultRevisit_Actor1[] = {
    SC_ARG(2),
    SC_SET(SW_SPEED, 0xcccc),
    SC_SET(SW_ACCEL, 0x3333),
    SC_LABEL(1),
    SC_MOVE(PIXELS(120), 0, PIXELS(424)),
    SC_SYNC,
    SC_MOVE(PIXELS(136), 0, PIXELS(424)),
    SC_SYNC,
    SC_MOVE(PIXELS(152), 0, PIXELS(408)),
    SC_SYNC,
    SC_MOVE(PIXELS(152), 0, PIXELS(392)),
    SC_SYNC,
    SC_MOVE(PIXELS(136), 0, PIXELS(376)),
    SC_SYNC,
    SC_MOVE(PIXELS(120), 0, PIXELS(376)),
    SC_SYNC,
    SC_MOVE(PIXELS(104), 0, PIXELS(392)),
    SC_SYNC,
    SC_MOVE(PIXELS(104), 0, PIXELS(408)),
    SC_SYNC,
    SC_LOOP(SC_FOREVER, SC_TO(1)),
    SC_END,
};

struct SceneEntrance gVaultRevisit_Scripts[] = {
    {0, CONDITION_ALWAYS, 172, 0, 149, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {1, CONDITION_ALWAYS, 400, 0, 472, FACING_NORTH, 0, -1, -1, -1, -1, 0},
    {2, CONDITION_ALWAYS, 224, 0, 138, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {5, CONDITION_ALWAYS, 408, 0, 344, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {6, CONDITION_ALWAYS, 312, 0, 328, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {7, CONDITION_ALWAYS, 216, 0, 312, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {8, CONDITION_ALWAYS, 376, 0, 248, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {9, CONDITION_ALWAYS, 296, 0, 200, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {10, CONDITION_ALWAYS, 120, 0, 168, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {11, CONDITION_ALWAYS, 64, 0, 294, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {12, CONDITION_ALWAYS, 70, 0, 183, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {13, CONDITION_ALWAYS, 217, 0, 409, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {14, CONDITION_ALWAYS, 420, 0, 136, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {SCENE_TABLE_END},
};

u32 gVaultRevisit_Messages[] = {
    SCENE_EXITS(23),       SCENE_EXIT(1, 2, 3),    SCENE_EXIT(2, 2, 44),
    SCENE_EXIT(5, 24, 5),  SCENE_EXIT(6, 24, 6),   SCENE_EXIT(7, 24, 7),
    SCENE_EXIT(8, 24, 8),  SCENE_EXIT(9, 24, 9),   SCENE_EXIT(10, 24, 10),
    SCENE_EXIT(11, 9, 11), SCENE_EXIT(12, 22, 10), SCENE_EXIT(13, 96, 10),
    SCENE_EXIT(14, 98, 1), SCENE_EXITS_END,
};

// clang-format off
struct ScenePlacement gVaultRevisit_Actors[] = {
    { 0x6c, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(381), 0, PIXELS(342), FACING_SOUTHEAST + FACING_STEP, TALK_FACE_PARTY, 0 },
    { 0x6a, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(280), 0, PIXELS(396), FACING_SOUTH + FACING_STEP, TALK_FACE_PARTY_AND_BACK, 0 },
    { 0x65, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(264), 0, PIXELS(150), FACING_NORTHWEST + FACING_STEP, TALK_FACE_PARTY_AND_BACK, 0 },
    { 0x73, CONDITION_ALWAYS, ACTOR_WANDER,
        PIXELS(244), 0, PIXELS(204), FACING_SOUTH + FACING_STEP, TALK_FACE_PARTY, 0 },
    { 0x67, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(124), 0, PIXELS(196), FACING_SOUTHEAST, TALK_FACE_PARTY, 0 },
    { 0x6f, CONDITION_ALWAYS, ACTOR_WANDER,
        PIXELS(244), 0, PIXELS(300), FACING_NORTHWEST + FACING_STEP, TALK_FACE_PARTY, 0 },
    { 0x6b, CONDITION_ALWAYS, (s32)gVaultRevisit_Actor0,
        PIXELS(132), 0, PIXELS(380), FACING_WEST, TALK_KEEP_FACING, 0 },
    { 0x66, CONDITION_ALWAYS, (s32)gVaultRevisit_Actor1,
        PIXELS(132), 0, PIXELS(420), FACING_EAST, TALK_KEEP_FACING, 0 },
    { 0x74, CONDITION_ALWAYS, ACTOR_WANDER,
        PIXELS(332), 0, PIXELS(412), FACING_NORTHWEST + FACING_STEP, TALK_FACE_PARTY, 0 },
    { 0x7a, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(92), 0, PIXELS(296), FACING_SOUTHEAST + FACING_STEP, TALK_FACE_PARTY, 0 },
    { 0x68, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(216), 0, PIXELS(440), FACING_SOUTH + FACING_STEP, TALK_FACE_PARTY, 0 },
    { 0x6c, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(152), 0, PIXELS(296), FACING_SOUTHEAST + FACING_STEP, TALK_FACE_PARTY, 0 },
    { 0xf8, CONDITION_ALWAYS, 7,
        0, 0, 0, FACING_NORTH, TALK_FACE_PARTY, 0 },
    { 0x5a, 0x34, ACTOR_STAND,
        PIXELS(344), 0, PIXELS(96), FACING_SOUTH, TALK_FACE_PARTY, 0 },
    { SCENE_TABLE_END },
};

u16 gVaultRevisit_Scene5Cells[] = {
    MAP_CELL_STEP(35, 52, 2, 2, 4), MAP_CELL_STEP(33, 52, 2, 2, 4), MAP_CELL_STEPS_END,
};

u16 gVaultRevisit_Scene6Cells[] = {
    MAP_CELL_STEP(35, 50, 2, 2, 4), MAP_CELL_STEP(33, 50, 2, 2, 4), MAP_CELL_STEPS_END,
};

u16 gVaultRevisit_Scene7Cells[] = {
    MAP_CELL_STEP(35, 48, 2, 2, 4), MAP_CELL_STEP(33, 48, 2, 2, 4), MAP_CELL_STEPS_END,
};

u16 gVaultRevisit_Scene8Cells[] = {
    MAP_CELL_STEP(42, 48, 2, 2, 4), MAP_CELL_STEP(40, 48, 2, 2, 4), MAP_CELL_STEPS_END,
};

u16 gVaultRevisit_Scene9Cells[] = {
    MAP_CELL_STEP(42, 46, 2, 2, 4), MAP_CELL_STEP(40, 46, 2, 2, 4), MAP_CELL_STEPS_END,
};

u16 gVaultRevisit_Scene10Cells[] = {
    MAP_CELL_STEP(35, 46, 2, 2, 4), MAP_CELL_STEP(33, 46, 2, 2, 4), MAP_CELL_STEPS_END,
};

u16 gVaultRevisit_Scene12Cells[] = {
    MAP_CELL_STEP(35, 33, 2, 2, 4), MAP_CELL_STEP(33, 33, 2, 2, 4), MAP_CELL_STEPS_END,
};

struct SceneEvent gVaultRevisit_Extras[] = {
    { EVENT_EXIT, 20, CONDITION_ALWAYS, 1 },
    { EVENT_EXIT, 21, CONDITION_ALWAYS, 2 },
    { EVENT_EXIT, 40, CONDITION_ALWAYS, 13 },
    { EVENT_EXIT, 41, CONDITION_ALWAYS, 14 },
    { EVENT_TOUCH, 1, CONDITION_ALWAYS, EVENT_SCRIPT(Vault_EnableAlternatePassageCells) },
    { EVENT_TOUCH, 2, CONDITION_ALWAYS, EVENT_SCRIPT(Vault_RestorePassageCells) },
    { TOUCH_WALKING_INTO(FACING_NORTH), 5, CONDITION_ALWAYS, EVENT_SCRIPT(EV05_TOUCH) },
    { TOUCH_WALKING_INTO(FACING_NORTH), 6, CONDITION_ALWAYS, EVENT_SCRIPT(EV06_TOUCH) },
    { TOUCH_WALKING_INTO(FACING_NORTH), 7, CONDITION_ALWAYS, EVENT_SCRIPT(EV07_TOUCH) },
    { TOUCH_WALKING_INTO(FACING_NORTH), 8, CONDITION_ALWAYS, EVENT_SCRIPT(EV08_TOUCH) },
    { TOUCH_WALKING_INTO(FACING_NORTH), 9, CONDITION_ALWAYS, EVENT_SCRIPT(EV09_TOUCH) },
    { TOUCH_WALKING_INTO(FACING_NORTH), 10, CONDITION_ALWAYS, EVENT_SCRIPT(EV10_TOUCH) },
    { TOUCH_WALKING_INTO(FACING_NORTH), 11, CONDITION_ALWAYS, EVENT_SCRIPT(Vault_OpenDoorwayAndEnter) },
    { TOUCH_WALKING_INTO(FACING_NORTH), 12, CONDITION_ALWAYS, EVENT_SCRIPT(EV12_TOUCH) },
    { EVENT_TALK, 8, CONDITION_ALWAYS, EVENT_SCRIPT(EV08_TALK) },
    { EVENT_TALK, 9, CONDITION_ALWAYS, MSGNO(MsgVaultYouveProbablyAlreadyHeardButThe) },
    { EVENT_TALK, 10, CONDITION_ALWAYS, MSGNO(MsgVaultIWonderWhatHappenedToHammet) },
    { EVENT_TALK, 11, CONDITION_ALWAYS, EVENT_SCRIPT(EV11_TALK) },
    { EVENT_TALK, 12, CONDITION_ALWAYS, EVENT_SCRIPT(EV12_TALK) },
    { EVENT_TALK, 13, CONDITION_ALWAYS, EVENT_SCRIPT(EV13_TALK) },
    { EVENT_TALK, 14, CONDITION_ALWAYS, EVENT_SCRIPT(EV14_TALK) },
    { EVENT_TALK, 15, CONDITION_ALWAYS, EVENT_SCRIPT(EV15_TALK) },
    { EVENT_TALK, 16, CONDITION_ALWAYS, EVENT_SCRIPT(EV16_TALK) },
    { EVENT_TALK, 17, CONDITION_ALWAYS, MSGNO(MsgVaultTheThievesEscapedButIThink) },
    { EVENT_TALK, 18, CONDITION_ALWAYS, EVENT_SCRIPT(EV18_TALK) },
    { EVENT_TALK, 19, CONDITION_ALWAYS, MSGNO(MsgVaultThereYouSeeTheRoofRepairs) },
    { EVENT_TALK, 21, CONDITION_ALWAYS, EVENT_SCRIPT(EV21_TALK) },
    { SEARCH(SEARCH_UNNAMED), 30, CONDITION_ALWAYS, EVENT_SCRIPT(EV30_SEARCH) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 0x408, 0x305, EVENT_SCRIPT(EV1032_PSYNERGY) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 8, CONDITION_ALWAYS, MSGNO(MsgVaultICantBelieveTheyreHereWhat) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 9, CONDITION_ALWAYS, MSGNO(MsgVaultWhenThoseGuysFromKalayCame) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 10, CONDITION_ALWAYS, MSGNO(MsgVaultYeahSomeWarriorsFromKalayCame) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 11, CONDITION_ALWAYS, EVENT_SCRIPT(EV11_PSYNERGY) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 0x40c, 0x306, EVENT_SCRIPT(EV1036_PSYNERGY) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 12, CONDITION_ALWAYS, MSGNO(MsgVaultThoseStupidThievesEscapedAndWe) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 13, CONDITION_ALWAYS, EVENT_SCRIPT(EV13_PSYNERGY) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 0x40e, 0x307, EVENT_SCRIPT(VaultRevisit_RunActor14Talk) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 14, CONDITION_ALWAYS, EVENT_SCRIPT(VaultRevisit_RunActor14Talk) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 15, CONDITION_ALWAYS, EVENT_SCRIPT(EV15_PSYNERGY) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 0x410, 0x308, EVENT_SCRIPT(VaultRevisit_ReadJailRumor) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 16, CONDITION_ALWAYS, EVENT_SCRIPT(VaultRevisit_ReadJailRumor) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 17, CONDITION_ALWAYS, MSGNO(MsgVaultIKeepWonderingWasOurStatue) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 18, IF_FLAG_SET(0x858), MSGNO(MsgVaultCarefulSearchWillRevealPassage) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 18, CONDITION_ALWAYS, MSGNO(MsgVaultIfYouBringMeSomeBones) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 19, CONDITION_ALWAYS, MSGNO(MsgVaultThatsTheLastOfTheRoof) },
    { SEARCH(SEARCH_UNNAMED), 90, CONDITION_ALWAYS, EVENT_SCRIPT(EV90_SEARCH) },
    { SEARCH(SEARCH_WOODEN_BOX), 101, 0xf4a, FIND_ITEM(ITEM_NUT) },
    { 0xe714, 18, 0x858, EVENT_SCRIPT(EV18_SCRIPT) },
    { PSYNERGY_IN_AREA(ABILITY_REVEAL, 5), 0, CONDITION_ALWAYS, EVENT_SCRIPT(ActorPresentation_SetAlternatePairedSceneCells) },
    { PSYNERGY_IN_AREA(ABILITY_REVEAL, 4), 0, CONDITION_ALWAYS, EVENT_SCRIPT(ActorPresentation_SetPairedSceneCells) },
    { SCENE_EVENTS_END },
};
// clang-format on
