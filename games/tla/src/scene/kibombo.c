/// @file
/// Kibombo, the warriors' village below the Gabomba Statue, in the six
/// rooms of this overlay: the village, its shops, inn and Sanctum
/// (Kibombo_OpenWeaponShop, Kibombo_OpenInn), and the grounds before the
/// statue. On the sacred night Akafubu offers the Black Orb to the Great
/// Gabomba (Kibombo_CeremonyBegins to Kibombo_CeremonyEnds), where Piers, come
/// to take his orb back, joins the party (Kibombo_PiersThanksParty); the way to
/// the statue past the dead end is a vent Scoop uncovers
/// (Kibombo_DeadEndByTheStatue). The villagers' talk changes once Akafubu is
/// the witch doctor (Kibombo_TalkPathToPeace).

#include "flags.h"
#include "item_ids.h"
#include "types.h"
struct Actor;
#include "kibombo.h"
#include "event_work.h"
#include "gamepak/far_call/system.h"
#include "module/field/event_commands.h"
#include "module/scene/kibombo.h"
#include "graphics/resource.h"

s32 Func_02000038(struct Actor *object) {
  ((struct FieldSprite *)object->sprite)->rotation += 0x600;
  return 0;
}

s32 Func_02000048(struct Actor *object) {
  object->bounce = object->x;
  object->gravity = object->y;
  return 0;
}

extern s16 gData_0200635c[];

struct KibomboWork90 {
  u8 unk00[0x18];
  struct Actor *leader;
};

s32 Func_02000054(struct Actor *object) {
  if (gFrameCount & 1) {
    s32 dx;
    s32 dy;

    dx = -0x10000;
    dy = dx;
    dx += ((Random16() * 3) >> 16) << 16;
    dy += ((Random16() * 3) >> 16) << 16;

    Actor_SetPositionAndResetMotion(object, object->bounce + dx,
                                    object->gravity + dy, object->z);
  }
  return 1;
}

void Func_020000a0(struct Actor *object) {
  if (gFrameCount & 2)
    Actor_SetSpriteColor(object, 10);
  else
    Actor_SetSpriteColor(object, 0);
}

s32 Func_020000c4(struct Actor *object) {
  if (object->riseCounter != 0) {
    object->riseCounter--;
  } else {
    object->riseCounter = ((Random16() * 10) >> 16) + 10;
    object->facing = Random16();
  }
  return 1;
}

#include "battle/battle.h"
#include "field/field.h"
#include "game/game.h"
#include "graphics/graphics.h"
#include "lib.h"
#include "menu/menu.h"
#include "sound/sound.h"
#include "system/system.h"
#include "types.h"
#include "scene_tables.h"
#include "scene_ids.h"

const struct SceneEntrance *Scene_GetEntrances(void) { return gSceneEntrances; }

u8 *Scene_GetRegions(void) {
  s16 scene = gMainRoutineWork.scene;

  if (scene == (s32)&SceneId_Kibombo3)
    return gData_0200635c;
  return 0;
}

const u32 *Scene_GetExits(void) { return gSceneExits; }

extern struct ActorPlacement gKibombo_Room1Placements[];
extern struct ActorPlacement gKibombo_Room4Placements[];
extern struct ActorPlacement gKibombo_Room2Placements[];
extern struct ActorPlacement gKibombo_Room5Placements[];
extern struct ActorPlacement gKibombo_Room3Placements[];
extern struct ActorPlacement gKibombo_Placements[];
extern struct Trigger gKibombo_Room1Triggers[];
extern struct Trigger gKibombo_Room4Triggers[];
extern struct Trigger gKibombo_Room2Triggers[];
extern struct Trigger gKibombo_Room5Triggers[];
extern struct Trigger gKibombo_Room3Triggers[];
extern struct Trigger gKibombo_Triggers[];

const struct ActorPlacement *Scene_GetPlacements(void) {
  s16 scene = gMainRoutineWork.scene;

  if (scene == (s32)&SceneId_Kibombo1)
    return gKibombo_Room1Placements;
  if (scene == (s32)&SceneId_Kibombo4)
    return gKibombo_Room4Placements;
  if (scene == (s32)&SceneId_Kibombo2)
    return gKibombo_Room2Placements;
  if (scene == (s32)&SceneId_Kibombo5)
    return gKibombo_Room5Placements;
  if (scene == (s32)&SceneId_Kibombo3) {
    if (GameFlag_Get(0x8ee))
      gKibombo_Room3Placements[0].talkFacing = 0;
    return gKibombo_Room3Placements;
  }
  return gKibombo_Placements;
}

const struct Trigger *Scene_GetEvents(void) {
  s16 scene = gMainRoutineWork.scene;

  if (scene == (s32)&SceneId_Kibombo1)
    return gKibombo_Room1Triggers;
  if (scene == (s32)&SceneId_Kibombo4)
    return gKibombo_Room4Triggers;
  if (scene == (s32)&SceneId_Kibombo2)
    return gKibombo_Room2Triggers;
  if (scene == (s32)&SceneId_Kibombo5)
    return gKibombo_Room5Triggers;
  if (scene == (s32)&SceneId_Kibombo3)
    return gKibombo_Room3Triggers;
  return gKibombo_Triggers;
}

void Kibombo_AskAboutFewerWarriors(void) {
  Event_Start();
  Event_ResolvePendingActions(0);
  Event_SetMessageId((s32)MsgItLooksLikeThere);
  Event_AskYesNoAndShowResponse(15, 0);
  Event_End();
}

void Kibombo_AskAboutRevenge(void) {
  Event_Start();
  Event_ResolvePendingActions(0);
  Event_SetMessageId((s32)MsgYouDidnT);
  Event_AskYesNoAndShowResponse(23, 0);
  Event_End();
}

void Kibombo_TalkPathToPeace(void) {
  Event_Start();
  Event_ResolvePendingActions(0);
  Event_SetMessageId((s32)MsgNowThatAkafubu);
  Event_ShowMessageAndWait(24, 0);
  Actor_FaceActorById(24, gMainRoutineWork.currentOwner, 0);
  Event_Wait(20);
  Event_ShowMessageAndWait(24, 0);
  Kibombo_ObjTurn(24, FACING_SOUTHEAST + FACING_STEP);
  Event_End();
}

void Kibombo_AskGoingWest(void) {
  Event_Start();
  Event_ResolvePendingActions(0);
  Event_SetMessageId((s32)MsgAreYouGoing);
  Event_AskYesNoAndShowResponse(12, 0);
  Event_End();
}

#include "shared/leader_is_facing_north.h"

void Kibombo_OpenWeaponShop(void) {
  if (Leader_IsFacingNorth()) {
    Shop_Run(17, 20);
  } else {
    Event_Start();
    Event_ResolvePendingActions(0);
    Event_SetMessageId((s32)MsgTheWeaponsI);
    Event_ShowMessageAndWait(20, 0);
    Event_End();
  }
}

void Kibombo_OpenArmorShop(void) {
  if (Leader_IsFacingNorth()) {
    Shop_Run(18, 21);
  } else {
    Event_Start();
    Event_ResolvePendingActions(0);
    Event_SetMessageId((s32)MsgTheOnlyDefenses);
    Event_ShowMessageAndWait(21, 0);
    Event_End();
  }
}

void Kibombo_OpenItemShop(void) {
  if (Leader_IsFacingNorth()) {
    Shop_Run(19, 22);
  } else {
    Event_Start();
    Event_ResolvePendingActions(0);
    Event_SetMessageId((s32)MsgIMSorryBut);
    Event_ShowMessageAndWait(22, 0);
    Event_End();
  }
}

void Kibombo_OpenInn(void) {
  if (Leader_IsFacingNorth()) {
    Inn_CheckIn(6, 23);
  } else {
    Event_Start();
    Event_ResolvePendingActions(0);
    Event_SetMessageId((s32)MsgDonTWorryAbout);
    Event_ShowMessageAndWait(23, 0);
    Event_End();
  }
}

void Kibombo_OpenSanctum(void) {
  if (Leader_IsFacingNorth()) {
    Sanctum_Run(25);
  } else {
    Event_Start();
    Event_ResolvePendingActions(0);
    Event_SetMessageId((s32)MsgKibomboSPeace);
    Event_ShowMessageAndWait(25, 0);
    Event_End();
  }
}

void Kibombo_TalkForeignCeremony(void) {
  if (Kibombo_FlagCheck(0x9df)) {
    Event_Start();
    Event_ResolvePendingActions(0);
    Actor_FaceActorById(36, gMainRoutineWork.currentOwner, 0);
    Event_Wait(10);
    Sanctum_Run(36);
    Kibombo_ObjArm(36, FACING_NORTHWEST + FACING_STEP, 0);
    Event_End();
  } else {
    Event_Start();
    Event_ResolvePendingActions(0);
    Event_SetMessageId((s32)MsgOhOhMy);
    Event_ShowMessageThenWait(36, 0, 40);
    Event_ShowMessageAndWait(36, 0);
    Actor_FaceActorById(36, gMainRoutineWork.currentOwner, 0);
    Event_Wait(10);
    Sanctum_Run(36);
    Kibombo_ObjArm(36, FACING_NORTHWEST + FACING_STEP, 0);
    GameFlag_Set(0x9df);
    Event_End();
  }
}

void Kibombo_TalkAchingBack(void) {
  Event_Start();
  Event_ResolvePendingActions(0);
  Event_SetMessageId((s32)MsgOhMyBack);
  Event_ShowMessageAndWait(16, 0);
  Event_End();
}

void Kibombo_ReadMindNeverAgain(void) {
  Event_Start();
  Event_ResolvePendingActions(0);
  Event_SetMessageId((s32)MsgISwearThat);
  Event_ShowMessageAndWait(16, 0);
  Event_End();
}

void Kibombo_SetActorsHidden(s32 show) {
  s16 scene = gMainRoutineWork.scene;

  if (scene == (s32)&SceneId_Kibombo2 || scene == (s32)&SceneId_Kibombo5)
    Actors_SetHidden(show, 15, 2);
  else if (scene == (s32)&SceneId_Kibombo3)
    Actors_SetHidden(show, 26, 2);
}

void Kibombo_HideActors(void) {
  Kibombo_SetActorsHidden(1);
  GameFlag_Set(FLAG_SCENE_TEMP_00);
}

void Kibombo_ShowActors(void) {
  Kibombo_SetActorsHidden(0);
  GameFlag_Clear(FLAG_SCENE_TEMP_00);
}

void Actors_SetHidden(s32 show, s32 first, u32 count) {
  u32 i;

  for (i = 0; i < count; i++) {
    struct Actor *actor = Actor_GetById(first + i);

    if (show)
      actor->priorityFlags &= ~0x10;
    else
      actor->priorityFlags |= 0x10;
  }
}

void Func_020005d0(void) { FlagBlock_ApplyFlags(gData_02006324); }

void Kibombo_UncoverVent(void) {
  MapGrid_CopyCellPictureIndicesRect(34, 33, 23, 18, 1, 1);
  Kibombo_BlendStart(34, 33, 1, 1, 23, 18);
  GameFlag_Set(0x86a);
}

void Kibombo_ScoopVent(s32 mode) {
  if (mode == 1)
    Kibombo_UncoverVent();
}

void Func_02000624(void) {
  MapGrid_CopyCellPictureIndicesRect(36, 34, 16, 7, 1, 1);
  Kibombo_BlendStart(36, 34, 1, 1, 16, 7);
  GameFlag_Set(FLAG_SCENE_TEMP_04);
}

void Func_0200065c(s32 mode) {
  if (mode == 1)
    Func_02000624();
}

void Func_02000668(void) {
  MapGrid_CopyCellPictureIndicesRect(38, 34, 16, 11, 1, 1);
  Kibombo_BlendStart(38, 34, 1, 1, 16, 11);
  GameFlag_Set(FLAG_SCENE_TEMP_05);
}

void Func_020006a0(s32 mode) {
  if (mode == 1)
    Func_02000668();
}

void Func_020006ac(void) {
  MapGrid_CopyCellPictureIndicesRect(36, 36, 14, 9, 1, 1);
  Kibombo_BlendStart(36, 36, 1, 1, 14, 9);
  GameFlag_Set(FLAG_SCENE_TEMP_06);
}

void Func_020006e4(s32 mode) {
  if (mode == 1)
    Func_020006ac();
}

void Func_020006f0(void) {
  MapGrid_CopyCellPictureIndicesRect(38, 36, 18, 9, 1, 1);
  Kibombo_BlendStart(38, 36, 1, 1, 18, 9);
  GameFlag_Set(FLAG_SCENE_TEMP_07);
}

void Func_02000728(s32 mode) {
  if (mode == 1)
    Func_020006f0();
}

void Func_02000734(void) {
  struct Actor *object = Actor_GetById(11);

  if (object->x >> 20 == 6 && object->z >> 20 == 18) {
    Event_Start();
    Event_ResolvePendingActions(0);
    Actor_GetById(11)->motionFlags = 3;
    object->motionFlags = 3;
    do
      wait(1);
    while (object->velocityY != 0);
    Sound_Play(SE_THUMP);
    wait(10);
    object->motionFlags = 0;
    Actor_GetById(11)->motionFlags = 0;
    Kibombo_BlendStart(5, 18, 1, 1, 6, 18);
    GameFlag_Set(0x9ea);
    if (!GameFlag_Get(0x8f4))
      Kibombo_PiersJoinsParty();
    Event_End();
  }
}

void Kibombo_MeetPiers(void) {
  Event_Start();
  Event_ResolvePendingActions(0);
  Kibombo_CamMove(-1, -1, -1, 0);
  FieldCamera_GetActor()->motionFlags = 0;
  Camera_SetSpeedParameters(0x9999, 0x1333);
  FieldCamera_MoveWithinMapBounds(PIXELS(88), -1, PIXELS(392), 1);
  Kibombo_ObjSpeed(gMainRoutineWork.currentOwner, 0xcccc, 0x6666);
  Kibombo_ObjWalk(gMainRoutineWork.currentOwner, 72, 392);
  Kibombo_ObjWalk(gMainRoutineWork.currentOwner, 88, 392);
  Actor_ArmCallback(7, FACING_SOUTHWEST, 10);
  Event_SetMessageId((s32)MsgWhWhoAre);
  Event_ShowMessageAndWait(7, 0);
  Actor_MoveToObject(26, gMainRoutineWork.currentOwner);
  Actor_MoveToObject(5, gMainRoutineWork.currentOwner);
  Actor_MoveToObject(6, gMainRoutineWork.currentOwner);
  wait(1);
  Actor_SetSpritePriority(26, 2);
  Actor_SetSpritePriority(5, 2);
  Actor_SetSpritePriority(6, 2);
  Kibombo_ObjSpeed(26, 0xcccc, 0x6666);
  Kibombo_ObjSpeed(5, 0xcccc, 0x6666);
  Kibombo_ObjSpeed(6, 0xcccc, 0x6666);
  Actor_SetMovementTypeById(26, gData_02005d18);
  Actor_SetMovementTypeById(5, gData_02005d5c);
  Actor_SetMovementTypeAndWaitById(6, gData_02005da0);
  Event_ShowMessageAndWait(7, 0);
  if (GameFlag_Get(0x911))
    Kibombo_PiersHasForgottenParty();
  else
    Kibombo_PiersTakesPartyForEnemies();
  Func_02000d74();
  Kibombo_ObjSpeed(26, 0x19999, 0xcccc);
  Kibombo_ObjSpeed(5, 0x19999, 0xcccc);
  Kibombo_ObjSpeed(6, 0x19999, 0xcccc);
  Actor_SetMovementTypeById(5, gData_02005e54);
  Actor_SetMovementTypeById(6, gData_02005e54);
  Actor_SetMovementTypeAndWaitById(26, gData_02005e54);
  GameFlag_Set(0x8f3);
  Event_End();
}

void Kibombo_PiersHasForgottenParty(void) {
  Actor_ArmCallback(gMainRoutineWork.currentOwner, FACING_WEST, 20);
  Event_SetMessageId((s32)MsgItLooksLikeHas);
  FieldMessage_ShowAtActor(6, 0);
  if (Event_AskYesNoWithActorAnimation(gMainRoutineWork.currentOwner, 0) == 0) {
    Kibombo_ObjArm(5, FACING_SOUTH, 10);
    Actor_SetModeById(5, 3);
    Event_ShowMessageAndWait(5, 0);
    ((struct KibomboEventWork *)Ram_HeapSlots->eventWork)->field1c4++;
  } else {
    Kibombo_ObjArm(5, FACING_SOUTH, 10);
    Actor_SetModeById(5, 4);
    ((struct KibomboEventWork *)Ram_HeapSlots->eventWork)->field1c4++;
    Event_ShowMessageAndWait(5, 0);
  }
  Actor_ShowEmoteById(7, EMOTE_IN_FRONT | EMOTE_QUESTION, 20);
  Event_ShowMessageAndWait(7, 0);
  Actor_ArmCallback(gMainRoutineWork.currentOwner, FACING_SOUTHEAST, 0);
  Kibombo_ObjArm(26, FACING_NORTHWEST + FACING_STEP, 0);
  Kibombo_ObjArm(5, FACING_SOUTHWEST, 0);
  Kibombo_ObjArm(6, FACING_NORTHEAST, 40);
  Kibombo_ObjArm(gMainRoutineWork.currentOwner, FACING_NORTHEAST, 0);
  Kibombo_ObjArm(26, FACING_NORTH + FACING_STEP, 0);
  Actor_ArmCallback(5, FACING_EAST, 0);
  Kibombo_ObjArm(6, FACING_NORTHEAST, 20);
  Event_ShowMessageAndWait(7, 0);
  Actor_ShowEmoteById(26, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
  Event_ShowMessageAndWait(26, 0);
  Event_ShowMessageAndWait(7, 0);
  Event_ShowMessageAndWait(6, 0);
  Kibombo_ObjArm(5, FACING_SOUTH, 40);
  Actor_ArmCallback(5, FACING_EAST, 0);
  Event_ShowMessageAndWait(5, 0);
  Event_ShowMessageAndWait(7, 0);
  Kibombo_ObjEmote(6, 259, 0);
  Actor_Jump(6, 2, 10);
  Actor_Jump(6, 4, 10);
  Event_ShowMessageAndWait(6, 0);
  Kibombo_ObjEmote(5, 259, 10);
  Event_ShowMessageAndWait(5, 0);
  Actor_ShowEmoteById(7, EMOTE_IN_FRONT | EMOTE_ANGER_VEIN, 10);
  Event_ShowMessageAndWait(7, 0);
}

void Kibombo_PiersTakesPartyForEnemies(void) {
  Event_SetMessageId((s32)MsgHeSeemsTo);
  Event_ShowMessageAndWait(6, 0);
  Kibombo_ObjArm(5, FACING_SOUTH, 10);
  Actor_SetModeById(5, 4);
  Event_ShowMessageAndWait(5, 0);
  Actor_ShowEmoteById(7, EMOTE_IN_FRONT | EMOTE_QUESTION, 20);
  Event_ShowMessageAndWait(7, 0);
  Actor_ArmCallback(gMainRoutineWork.currentOwner, FACING_SOUTHEAST, 0);
  Kibombo_ObjArm(26, FACING_NORTHWEST + FACING_STEP, 0);
  Kibombo_ObjArm(5, FACING_SOUTHWEST, 0);
  Kibombo_ObjArm(6, FACING_NORTHEAST, 40);
  Kibombo_ObjArm(gMainRoutineWork.currentOwner, FACING_NORTHEAST, 0);
  Kibombo_ObjArm(26, FACING_NORTH + FACING_STEP, 0);
  Actor_ArmCallback(5, FACING_EAST, 0);
  Kibombo_ObjArm(6, FACING_NORTHEAST, 0);
  FieldEffect_SetAttached(7, ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  Event_Wait(20);
  Event_ShowMessageAndWait(7, 0);
  Event_ShowMessageThenWait(26, 0, 20);
  Event_ShowMessageAndWait(7, 0);
  Actor_ShowEmoteById(6, EMOTE_IN_FRONT | EMOTE_SWEAT_DROP, 0);
  Event_ShowMessageAndWait(6, 0);
  Kibombo_ObjArm(5, FACING_SOUTHWEST, 40);
  Actor_ArmCallback(5, FACING_EAST, 0);
  Event_ShowMessageAndWait(5, 0);
  Event_ShowMessageAndWait(7, 0);
  Kibombo_ObjEmote(6, 259, 0);
  Actor_Jump(6, 2, 10);
  Actor_Jump(6, 4, 10);
  Event_ShowMessageAndWait(6, 0);
  Kibombo_ObjEmote(5, 259, 10);
  Event_ShowMessageAndWait(5, 0);
  Actor_ShowEmoteById(7, EMOTE_IN_FRONT | EMOTE_ANGER_VEIN, 10);
  Event_ShowMessageAndWait(7, 0);
}

void Kibombo_PiersThanksParty(void) {
  struct Actor *leader;

  Event_Start();
  Event_ResolvePendingActions(0);
  Actor_ArmCallback(gMainRoutineWork.currentOwner, FACING_EAST, 0);
  Camera_SetSpeedParameters(0x26666, 0x4ccc);
  FieldCamera_MoveWithinMapBounds(PIXELS(280), -1, PIXELS(88), 1);
  FieldCamera_WaitForArrival();
  Event_Wait(20);
  Event_SetMessageId((s32)MsgYouVeCome);
  Event_ShowMessageAndWait(SPEAKER_WINDOW_BELOW | 7, 0);
  Kibombo_ObjTurn(7, FACING_WEST);
  Motion_SetModeAndWaitAnimation(gMainRoutineWork.currentOwner, 3);
  Motion_SetModeAndWaitAnimation(7, 3);
  Kibombo_ObjSpeed(7, 0x19999, 0xcccc);
  Actor_SetModeById(7, 2);
  leader = Actor_GetById(gMainRoutineWork.currentOwner);
  if (leader != 0)
    Actor_SetDestinationById(7, *(s16 *)((u8 *)leader + 10),
                             *(s16 *)((u8 *)leader + 18));
  Actor_WaitForMoveById(7);
  Actor_SetHorizontalPositionWithTerrain(7, 0, 0);
  GameFlag_Clear(0x92f);
  GameFlag_Set(0x92e);
  Event_PrepareObjectAndApplyValue(7, 0);
  Event_End();
}

void Func_02005c7c(void);
void Func_02002eb0(void);
void Func_020032b0(void);
void Func_020032c8(void);
void Func_02003610(void);

void Func_02000d74(void) {
  Func_020032c8();
  Sound_Play(MUS_000);
  Event_ShowMessageAndWait(17, 0);
  Actor_ArmCallback(7, FACING_EAST, 0);
  Actor_ArmCallback(gMainRoutineWork.currentOwner, FACING_EAST, 0);
  Actor_ArmCallback(26, FACING_EAST, 0);
  Actor_ArmCallback(5, FACING_EAST, 0);
  Actor_ArmCallback(6, FACING_EAST, 10);
  Camera_SetSpeedParameters(0x4cccc, 0x9999);
  Kibombo_CamMove(PIXELS(264), -1, PIXELS(388), 1);
  FieldCamera_WaitForArrival();
  Event_Wait(40);
  Kibombo_ObjArm(17, FACING_SOUTHEAST + FACING_STEP, 20);
  Motion_SetModeAndWaitAnimation(17, 3);
  Event_ShowMessageAndWait(SPEAKER_WINDOW_ABOVE | 17, 0);
  Sound_Play(MUS_026);
  Actor_SetSpriteFlags(Actor_GetById(27), 0);
  Actor_GetById(27)->motionFlags = 0;
  Actor_SetPositionAndResetMotion(Actor_GetById(27), PIXELS(265), PIXELS(24),
                                  PIXELS(344));
  Camera_SetSpeedParameters(0x6666, 0xccc);
  FieldCamera_MoveWithinMapBounds(PIXELS(264), -1, PIXELS(340), 1);
  Kibombo_ObjSpeed(17, 0x3333, 0x1999);
  Kibombo_ObjWalk(17, 264, 374);
  Kibombo_TaskAdd(Func_02002eb0, 0x480);
  Event_Wait(20);
  Task_Remove(Func_02002ccc);
  Func_020032e0();
  KIBOMBO_TASK_ADD(Func_02002ccc, 0x480);
  Event_ShowMessageAndWait(7, 0);
  Event_ShowMessageAndWait(17, 0);
  Sound_Play(MUS_011);
  Func_02003610();
  Event_Wait(160);
  Event_ShowMessageThenWait(17, 0, 80);
  Sound_Play(SE_576);
  Func_02003640(1, 1);
  Event_Wait(20);
  Event_ShowMessageThenWait(17, 0, 80);
  Sound_Play(SE_RATTLE);
  Func_02003640(0, 1);
  Event_Wait(20);
  FieldEffect_SetAttached(17, ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  Event_Wait(20);
  Event_ShowMessageThenWait(17, 0, 20);
  Task_Remove(Func_02002eb0);
  wait(1);
  Actor_SetHorizontalPositionWithTerrain(27, 0, 0);
  Kibombo_ObjArm(17, FACING_NORTH + FACING_STEP, 0);
  if (gData_02007950 != 0) {
    if (gData_02007950->y > 0x240000) {
    wait:
      Kibombo_ObjArm(17, FACING_NORTH + FACING_STEP, 0);
      gData_02007950->y += 0xffffc000;
      wait(1);
      if (gData_02007950->y > 0x240000)
        goto wait;
    }
  }
  Kibombo_ObjTurn(17, FACING_SOUTHEAST + FACING_STEP);
  Event_ShowMessageAndWait(17, 0);
  Camera_SetSpeedParameters(0x4cccc, 0x9999);
  FieldCamera_MoveWithinMapBounds(PIXELS(264), PIXELS(16), PIXELS(420), 1);
  Kibombo_ObjSpeed(17, 0x19999, 0xcccc);
  Actor_WalkToAndWaitById(17, 264, 392);
  Event_Wait(20);
  Event_ShowMessageAndWait(SPEAKER_WINDOW_BELOW | 17, 0);
  Sound_Play(SE_BOSS_ROAR);
  Sound_Play(MUS_011);
  Func_020032b0();
  Event_Wait(80);
  FieldCamera_MoveWithinMapBounds(PIXELS(88), PIXELS(32), PIXELS(392), 1);
  FieldCamera_WaitForArrival();
  Event_Wait(40);
  Event_ShowMessageAndWait(7, 0);
  Kibombo_ObjEmoteWait(7, 258);
  Event_ShowMessageAndWait(7, 0);
  Motion_SetModeAndWaitAnimation(7, 4);
  Event_ShowMessageAndWait(7, 0);
  Actor_SetSpritePriority(7, 2);
  Kibombo_ObjSpeed(7, 0x19999, 0xcccc);
  Kibombo_ObjWalk(7, 152, 386);
  Kibombo_ObjArm(7, FACING_NORTHEAST, 40);
  Kibombo_ObjArm(7, FACING_SOUTHEAST, 20);
  Kibombo_ObjArm(7, FACING_NORTHEAST, 20);
  Kibombo_ObjArm(7, FACING_SOUTHEAST, 40);
  Event_ShowMessageAndWait(7, 0);
  Kibombo_ObjWalk(7, 124, 356);
  Kibombo_ObjArm(7, FACING_NORTHEAST, 20);
  Kibombo_ObjArm(7, FACING_NORTHWEST, 20);
  Kibombo_ObjArm(7, FACING_NORTHEAST, 20);
  Event_ShowMessageAndWait(SPEAKER_PORTRAIT_RIGHT | 7, 0);
  Kibombo_ObjArm(5, FACING_SOUTHEAST, 0);
  Event_ShowMessageAndWait(5, 0);
  Actor_ArmCallback(gMainRoutineWork.currentOwner, FACING_NORTHEAST, 40);
  Kibombo_ObjSpeed(26, 0xcccc, 0x6666);
  Actor_WalkToById(26, 104, 368);
  Event_Wait(20);
  Camera_SetSpeedParameters(0xcccc, 0x1999);
  FieldCamera_MoveWithinMapBounds(PIXELS(111), -1, PIXELS(356), 1);
  Actor_WaitForMoveById(26);
  Actor_SetModeById(26, 1);
  Kibombo_ObjArm(26, FACING_NORTH + FACING_STEP, 40);
  Actor_ShowEmoteById(26, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 20);
  Event_ShowMessageAndWait(26, 0);
  Kibombo_ObjArm(gMainRoutineWork.currentOwner, FACING_NORTH, 0);
  Kibombo_ObjArm(5, FACING_NORTH, 0);
  Kibombo_ObjArm(6, FACING_NORTH, 0);
  Kibombo_ObjArm(26, FACING_SOUTH + FACING_STEP, 20);
  Kibombo_MessageOpen(0x601a, 0);
  if (Event_AskYesNoWithActorAnimation(gMainRoutineWork.currentOwner, 0) == 0) {
    Event_ShowMessageAndWait(SPEAKER_PORTRAIT_RIGHT | SPEAKER_WINDOW_ABOVE | 26,
                             0);
    ((struct KibomboEventWork *)Ram_HeapSlots->eventWork)->field1c4++;
  } else {
    ((struct KibomboEventWork *)Ram_HeapSlots->eventWork)->field1c4++;
    Event_ShowMessageAndWait(SPEAKER_PORTRAIT_RIGHT | SPEAKER_WINDOW_ABOVE | 26,
                             0);
    Func_02005c7c();
  }
}

void Kibombo_PiersRefusesToLeave(void) {
  if (!Kibombo_FlagCheck(0x92e)) {
    Import_EventSetExitRequest(5);
    return;
  }
  Event_Start();
  Event_ResolvePendingActions(0);
  Actor_MoveToObject(7, gMainRoutineWork.currentOwner);
  Actor_SetSpritePriority(
      7, ((struct FieldSprite *)Actor_GetById(gMainRoutineWork.currentOwner)
              ->sprite)
             ->priority);
  wait(1);
  Actor_ArmCallback(gMainRoutineWork.currentOwner, FACING_NORTHWEST, 0);
  Kibombo_ObjSpeed(7, 0x19999, 0xcccc);
  Actor_WalkToAndWaitById(7, 62, 444);
  Actor_FaceDirectionById(7, FACING_SOUTHEAST);
  Event_SetMessageId((s32)MsgNowThatIKnow);
  Event_ShowMessageAndWait(7, 0);
  Actor_GetById(7)->priorityFlags |= 1;
  Kibombo_ObjWalk(7, 56, 420);
  Kibombo_ObjWalk(7, 40, 344);
  Actor_SetHorizontalPositionWithTerrain(7, 0, 0);
  Kibombo_ObjPlace(gMainRoutineWork.currentOwner, 80, 510);
  Event_Wait(10);
  Import_EventSetExitRequest(5);
  GameFlag_Clear(0x92e);
  GameFlag_Set(0x92f);
  Party_RemoveMemberRestored(7);
  Event_End();
}

#include "system/ram_buffer.h"
#include "actor.h"
#include "system/main_routine_work.h"
#include "field/event_inline.h"

#define EVENT_WORK ((u8 *)Ram_HeapSlots->eventWork)

void Party_GivePiersDjinn(void);
extern u32 gData_02005e88[];
extern u32 gData_02005eec[];
extern u32 gData_02005f14[];
extern u32 gData_02005f58[];
extern u32 gData_02005f9c[];
extern u32 gData_02005fe0[];

/// A scene in Kibombo: actor 7 speaks, the camera pans, actors 26, 5, 6 and 7
/// move, turn and speak around the controlled character, two yes-or-no
/// questions count up the event work's halfword 0x1c4, and the scene ends with
/// Party_GivePiersDjinn. Each call goes through the call layer (call.h).
void Kibombo_PiersJoinsParty(void) {
  Actor_ArmCallback(7, FACING_NORTH, 0);
  Event_SetMessageId(MsgBlank7);
  Event_ShowMessage(SPEAKER_PORTRAIT_RIGHT | 7, 0);
  FieldCamera_MoveTo(CAMERA_KEEP, CAMERA_KEEP, CAMERA_KEEP, 0);
  ((struct Actor *)((s32)FieldCamera_GetActor()))->motionFlags = 0;
  FieldCamera_SetSpeed(3.2);
  FieldCamera_MoveTo(96, CAMERA_KEEP, 340, 1);
  FieldCamera_WaitForArrival();
  Actor_FaceDirection(7, FACING_NORTHWEST, 0);
  Actor_FaceDirection(gMainRoutineWork.currentOwner, FACING_SOUTHEAST, 0);
  FieldEffect_SetAttached(7, ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  Event_Wait(20);
  Event_AskYesNo(SPEAKER_PORTRAIT_RIGHT | 7, 0);
  FieldCamera_SetSpeed(0.4);
  FieldCamera_MoveTo(96, CAMERA_KEEP, 372, 1);
  Actor_SetSpeed(gMainRoutineWork.currentOwner, 1.2);
  Actor_SetSpeed(7, 1.2);
  Actor_SetMovementTypeById(7, gData_02005eec);
  Actor_SetMovementTypeAndWaitById(gMainRoutineWork.currentOwner,
                                   gData_02005e88);
  Actor_ArmCallback(gMainRoutineWork.currentOwner, FACING_EAST, 0);
  Actor_SetSpeed(26, 0.8);
  Actor_SetSpeed(5, 0.8);
  Actor_SetSpeed(6, 0.8);
  Actor_SetPosition(26, 56, 372);
  Actor_SetPosition(5, 56, 372);
  Actor_SetHorizontalPositionWithTerrain(6, PIXELS(56), PIXELS(372));
  wait(1);
  Actor_SetMovementTypeById(26, gData_02005f14);
  Actor_SetMovementTypeById(5, gData_02005f58);
  Actor_SetMovementTypeAndWaitById(6, gData_02005f9c);
  Event_Wait(20);
  Event_ShowMessage(SPEAKER_PORTRAIT_RIGHT | 7, 0);
  Actor_ShowEmoteById(5, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 20);
  Event_ShowMessage(SPEAKER_PORTRAIT_LEFT | 5, 0);
  Motion_SetModeAndWaitAnimation(7, 3);
  Event_ShowMessage(SPEAKER_PORTRAIT_RIGHT | 7, 0);
  Actor_SetModeById(gMainRoutineWork.currentOwner, 3);
  Actor_SetModeById(26, 3);
  Actor_SetModeById(5, 3);
  Motion_SetModeAndWaitAnimation(6, 3);
  Actor_SetModeById(7, 3);
  Event_ShowMessage(SPEAKER_PORTRAIT_RIGHT | 7, 0);
  Actor_ShowEmote(6, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
  Event_ShowMessage(SPEAKER_PORTRAIT_LEFT | 6, 0);
  Actor_FaceDirection(7, FACING_SOUTHWEST, 0);
  Event_ShowMessage(SPEAKER_PORTRAIT_RIGHT | 7, 0);
  Actor_ShowEmote(26, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
  Event_ShowMessage(SPEAKER_PORTRAIT_LEFT | 26, 0);
  Actor_FaceDirection(26, FACING_SOUTHEAST + FACING_STEP, 20);
  Actor_FaceDirection(gMainRoutineWork.currentOwner, FACING_SOUTH, 0);
  Actor_FaceDirection(5, FACING_SOUTH, 0);
  Actor_FaceDirection(6, FACING_SOUTH, 40);
  Event_ShowMessage(SPEAKER_PORTRAIT_LEFT | 26, 0);
  Actor_ShowEmote(gMainRoutineWork.currentOwner,
                  EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
  Actor_ShowEmote(5, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
  Actor_ShowEmote(6, EMOTE_IN_FRONT | EMOTE_QUESTION, 40);
  Actor_FaceDirection(26, FACING_NORTH + FACING_STEP, 0);
  Event_ShowMessage(SPEAKER_PORTRAIT_LEFT | 26, 0);
  Actor_SetModeById(7, 3);
  Event_ShowMessage(SPEAKER_PORTRAIT_RIGHT | 7, 0);
  Actor_ShowEmote(5, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
  Actor_ArmCallback(5, FACING_EAST, 20);
  Event_ShowMessage(SPEAKER_PORTRAIT_LEFT | 5, 0);
  Actor_ArmCallback(gMainRoutineWork.currentOwner, FACING_EAST, 0);
  Actor_ArmCallback(6, FACING_EAST, 0);
  Actor_ArmCallback(26, FACING_EAST, 0);
  Actor_FaceDirectionAndWait(7, FACING_WEST);
  Actor_SetModeById(7, 4);
  Event_ShowMessage(SPEAKER_PORTRAIT_RIGHT | 7, 0);
  Actor_FaceDirection(26, FACING_SOUTHEAST + FACING_STEP, 0);
  Actor_ShowEmote(gMainRoutineWork.currentOwner,
                  EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
  Actor_ShowEmote(5, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
  Actor_ShowEmote(6, EMOTE_IN_FRONT | EMOTE_QUESTION, 40);
  Actor_ShowEmoteById(7, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 20);
  Event_ShowMessage(SPEAKER_PORTRAIT_RIGHT | 7, 0);
  Actor_SetModeById(6, 3);
  Event_ShowMessageAndWait(SPEAKER_PORTRAIT_LEFT | 6, 0);
  Actor_FaceDirectionAndWait(7, FACING_SOUTHEAST);
  Actor_SetModeById(7, 4);
  Event_ShowMessage(SPEAKER_PORTRAIT_RIGHT | 7, 0);
  Event_ShowMessage(SPEAKER_PORTRAIT_LEFT | 5, 0);
  Actor_FaceDirection(7, FACING_WEST, 0);
  Event_ShowMessage(SPEAKER_PORTRAIT_RIGHT | 7, 0);
  Actor_SetModeById(7, 4);
  Event_ShowMessage(SPEAKER_PORTRAIT_RIGHT | 7, 0);
  Actor_FaceDirectionAndWait(26, FACING_NORTHWEST + FACING_STEP);
  Event_ShowMessage(SPEAKER_PORTRAIT_LEFT | 26, 0);
  Actor_FaceDirectionAndWait(7, FACING_SOUTHWEST);
  Event_ShowMessageAndWait(SPEAKER_PORTRAIT_RIGHT | 7, 0);
  Actor_FaceDirectionById(26, FACING_EAST);
  Actor_SetModeById(26, 3);
  Event_ShowMessage(SPEAKER_PORTRAIT_LEFT | 26, 0);
  Motion_SetModeAndWaitAnimation(7, 3);
  Actor_ShowEmoteById(26, EMOTE_IN_FRONT | EMOTE_SMILE, 20);
  Event_ShowMessage(SPEAKER_PORTRAIT_LEFT | 26, 0);
  Actor_FaceDirection(gMainRoutineWork.currentOwner, FACING_SOUTH, 0);
  Actor_FaceDirection(5, FACING_SOUTH, 0);
  Actor_FaceDirection(6, FACING_SOUTH, 0);
  Actor_ShowEmote(gMainRoutineWork.currentOwner,
                  EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
  Actor_ShowEmote(5, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
  Actor_ShowEmoteById(6, EMOTE_IN_FRONT | EMOTE_QUESTION, 40);
  Actor_FaceDirection(26, FACING_NORTH + FACING_STEP, 0);
  Event_OpenMessage(SPEAKER_PORTRAIT_LEFT | 26, 0);
  if (Event_AskYesNoWithActorAnimation(gMainRoutineWork.currentOwner, 0) == 1) {
    Event_ShowMessageAndWait(SPEAKER_PORTRAIT_LEFT | 26, 0);
    Motion_SetModeAndWaitAnimation(5, 4);
    Event_ShowMessageAndWait(SPEAKER_PORTRAIT_LEFT | 5, 0);
    *(u16 *)(EVENT_WORK + 0x1c4) += 1;
  } else {
    *(u16 *)(EVENT_WORK + 0x1c4) += 2;
    Motion_SetModeAndWaitAnimation(26, 3);
    Event_ShowMessageAndWait(SPEAKER_PORTRAIT_LEFT | 26, 0);
  }
  Actor_FaceDirection(26, FACING_NORTHWEST + FACING_STEP, 20);
  Actor_ArmCallback(26, FACING_EAST, 20);
  Motion_SetModeAndWaitAnimation(26, 3);
  Event_ShowMessage(SPEAKER_PORTRAIT_LEFT | 26, 0);
  Event_ShowMessage(SPEAKER_PORTRAIT_RIGHT | 7, 0);
  Actor_FaceDirection(26, FACING_NORTHWEST + FACING_STEP, 0);
  Event_ShowMessage(SPEAKER_PORTRAIT_LEFT | 26, 0);
  Actor_ShowEmote(6, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 20);
  Event_ShowMessage(SPEAKER_PORTRAIT_LEFT | 6, 0);
  Motion_SetModeAndWaitAnimation(26, 3);
  Event_ShowMessage(SPEAKER_PORTRAIT_LEFT | 26, 0);
  Actor_FaceDirection(7, FACING_WEST, 0);
  Actor_ArmCallback(gMainRoutineWork.currentOwner, FACING_EAST, 0);
  Actor_ArmCallback(5, FACING_EAST, 0);
  Actor_ArmCallback(6, FACING_EAST, 0);
  Actor_ArmCallback(26, FACING_EAST, 20);
  Motion_SetModeAndWaitAnimation(7, 3);
  Actor_SetModeById(26, 4);
  Event_ShowMessage(SPEAKER_PORTRAIT_LEFT | 26, 0);
  Actor_FaceDirection(gMainRoutineWork.currentOwner, FACING_SOUTH, 0);
  Actor_FaceDirection(5, FACING_SOUTH, 0);
  Actor_FaceDirection(6, FACING_SOUTH, 0);
  Actor_FaceDirection(7, FACING_SOUTHWEST, 0);
  Actor_FaceDirection(26, FACING_NORTHWEST + FACING_STEP, 0);
  Actor_ShowEmoteById(26, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 20);
  Event_ShowMessage(SPEAKER_PORTRAIT_LEFT | 26, 0);
  Actor_ShowEmoteById(7, EMOTE_IN_FRONT | EMOTE_LIGHT_BULB, 40);
  Event_ShowMessage(SPEAKER_PORTRAIT_RIGHT | 7, 0);
  Actor_ArmCallback(gMainRoutineWork.currentOwner, FACING_EAST, 0);
  Actor_ArmCallback(5, FACING_EAST, 0);
  Actor_ArmCallback(6, FACING_EAST, 0);
  Actor_ArmCallback(26, FACING_EAST, 0);
  Actor_ShowEmote(gMainRoutineWork.currentOwner,
                  EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
  Actor_ShowEmote(5, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
  Actor_ShowEmote(6, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 40);
  Actor_FaceDirection(7, FACING_WEST, 20);
  Actor_SetModeById(7, 4);
  Event_ShowMessage(SPEAKER_PORTRAIT_RIGHT | 7, 0);
  Motion_SetModeAndWaitAnimation(26, 3);
  Event_ShowMessage(SPEAKER_PORTRAIT_LEFT | 26, 0);
  Actor_FaceDirection(gMainRoutineWork.currentOwner, FACING_SOUTH, 0);
  Actor_FaceDirection(5, FACING_SOUTH, 0);
  Actor_FaceDirection(6, FACING_SOUTH, 0);
  Actor_FaceDirection(7, FACING_SOUTH + FACING_STEP, 20);
  Motion_SetModeAndWaitAnimation(5, 4);
  Event_ShowMessage(SPEAKER_PORTRAIT_LEFT | 5, 0);
  Actor_FaceDirection(7, FACING_WEST, 0);
  Actor_ShowEmote(7, EMOTE_IN_FRONT | EMOTE_QUESTION, 40);
  Actor_ArmCallback(5, FACING_EAST, 0);
  Event_ShowMessage(SPEAKER_PORTRAIT_LEFT | 5, 0);
  Actor_FaceDirectionAndWait(26, FACING_NORTHWEST + FACING_STEP);
  Actor_SetModeById(26, 4);
  Event_ShowMessage(SPEAKER_PORTRAIT_LEFT | 26, 0);
  Actor_FaceDirection(5, FACING_SOUTH, 0);
  Actor_ShowEmote(6, EMOTE_IN_FRONT | EMOTE_QUESTION, 20);
  Event_ShowMessage(SPEAKER_PORTRAIT_LEFT | 6, 0);
  Actor_FaceDirection(7, FACING_SOUTHWEST, 0);
  Actor_SetModeById(7, 4);
  Event_ShowMessage(SPEAKER_PORTRAIT_RIGHT | 7, 0);
  Actor_ArmCallback(5, FACING_EAST, 0);
  Actor_ArmCallback(gMainRoutineWork.currentOwner, FACING_EAST, 0);
  Actor_ArmCallback(6, FACING_EAST, 0);
  Actor_ArmCallback(26, FACING_EAST, 0);
  Actor_ShowEmote(5, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 20);
  Event_ShowMessage(SPEAKER_PORTRAIT_LEFT | 5, 0);
  Actor_FaceDirection(7, FACING_WEST, 0);
  Event_ShowMessage(SPEAKER_PORTRAIT_RIGHT | 7, 0);
  Actor_SetModeById(26, 3);
  Event_ShowMessage(SPEAKER_PORTRAIT_LEFT | 26, 0);
  Actor_FaceDirection(5, FACING_SOUTH, 0);
  Actor_FaceDirection(gMainRoutineWork.currentOwner, FACING_SOUTH, 0);
  Actor_FaceDirection(6, FACING_SOUTH, 0);
  Actor_FaceDirectionAndWait(7, FACING_SOUTHWEST);
  Motion_SetModeAndWaitAnimation(7, 3);
  Event_ShowMessage(SPEAKER_PORTRAIT_RIGHT | 7, 0);
  Actor_FaceDirection(gMainRoutineWork.currentOwner, FACING_WEST, 0);
  Actor_ArmCallback(6, FACING_EAST, 40);
  Actor_ArmCallback(gMainRoutineWork.currentOwner, FACING_EAST, 0);
  Actor_ArmCallback(6, FACING_EAST, 20);
  Event_ShowMessage(SPEAKER_PORTRAIT_LEFT | 6, 0);
  Actor_ArmCallback(5, FACING_EAST, 0);
  Actor_FaceDirection(7, FACING_WEST, 0);
  Actor_FaceDirection(7, FACING_WEST, 0);
  Actor_SetAttachedEffect(7, ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  Event_Wait(20);
  Event_ShowMessage(SPEAKER_PORTRAIT_RIGHT | 7, 0);
  Actor_ShowEmote(6, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
  Event_ShowMessage(SPEAKER_PORTRAIT_LEFT | 6, 0);
  Actor_FaceDirection(26, FACING_NORTHWEST + FACING_STEP, 10);
  Actor_SetModeById(26, 4);
  Event_ShowMessage(SPEAKER_PORTRAIT_LEFT | 26, 0);
  Motion_SetModeAndWaitAnimation(7, 3);
  Actor_ArmCallback(26, FACING_EAST, 0);
  Event_ShowMessage(SPEAKER_PORTRAIT_RIGHT | 7, 0);
  Actor_ShowEmote(7, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
  Event_ShowMessage(SPEAKER_PORTRAIT_RIGHT | 7, 0);
  Actor_FaceDirection(7, FACING_NORTH, 20);
  Event_ShowMessage(SPEAKER_PORTRAIT_RIGHT | 7, 0);
  Motion_SetModeAndWaitAnimation(7, 3);
  Event_ShowMessageAndWait(SPEAKER_PORTRAIT_RIGHT | 7, 0);
  Actor_WatchAndSetScript(gMainRoutineWork.currentOwner, 7);
  Actor_WatchAndSetScript(5, 7);
  Actor_WatchAndSetScript(6, 7);
  Actor_WatchAndSetScript(26, 7);
  Actor_SetSpeed(7, 1.6);
  Actor_WalkToAndWaitById(7, 72, 368);
  Actor_SetAttachedEffect(26, ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  Actor_WalkToAndWait(7, 40, 348);
  Actor_WalkToAndWait(7, 40, 332);
  Actor_FaceDirection(7, FACING_NORTHEAST, 0);
  Actor_Stop(gMainRoutineWork.currentOwner);
  Actor_Stop(5);
  Actor_Stop(6);
  Actor_Stop(26);
  Actor_FaceDirection(26, FACING_NORTHWEST + FACING_STEP, 0);
  Event_ShowMessage(SPEAKER_PORTRAIT_LEFT | 26, 0);
  Actor_ShowEmoteById(7, EMOTE_IN_FRONT | EMOTE_QUESTION, 20);
  Actor_FaceDirection(7, FACING_SOUTH, 40);
  Actor_FaceDirectionAndWait(26, FACING_NORTH + FACING_STEP);
  Event_ShowMessage(SPEAKER_PORTRAIT_LEFT | 26, 0);
  Actor_ShowEmoteById(7, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 20);
  Event_ShowMessage(SPEAKER_PORTRAIT_LEFT | 7, 0);
  Actor_FaceDirection(26, FACING_NORTHWEST + FACING_STEP, 0);
  Motion_SetModeAndWaitAnimation(26, 4);
  Event_ShowMessage(SPEAKER_PORTRAIT_LEFT | 26, 0);
  Event_ShowMessage(SPEAKER_PORTRAIT_LEFT | 26, 0);
  Actor_ShowEmoteById(7, EMOTE_IN_FRONT | EMOTE_SWEAT_DROP, 40);
  Event_ShowMessage(SPEAKER_PORTRAIT_LEFT | 7, 0);
  Actor_SetAttachedEffect(26, ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  Event_Wait(20);
  Event_ShowMessage(SPEAKER_PORTRAIT_LEFT | 26, 0);
  Motion_SetModeAndWaitAnimation(7, 4);
  Event_ShowMessageAndWait(SPEAKER_PORTRAIT_LEFT | 7, 0);
  Actor_SetModeById(26, 4);
  Event_ShowMessage(SPEAKER_PORTRAIT_LEFT | 26, 0);
  Actor_FaceDirection(gMainRoutineWork.currentOwner, FACING_SOUTH, 0);
  Actor_FaceDirection(5, FACING_SOUTH, 0);
  Actor_FaceDirection(6, FACING_SOUTH, 40);
  Event_ShowMessage(SPEAKER_PORTRAIT_LEFT | 26, 0);
  Actor_FaceDirection(26, FACING_NORTH + FACING_STEP, 0);
  Event_OpenMessage(SPEAKER_PORTRAIT_LEFT | 26, 0);
  if (Event_AskYesNoWithActorAnimation(gMainRoutineWork.currentOwner, 0) == 0) {
    Actor_ShowEmoteById(26, EMOTE_IN_FRONT | EMOTE_SMILE, 20);
    Event_ShowMessageAndWait(SPEAKER_PORTRAIT_LEFT | 26, 0);
    Actor_FaceDirection(5, FACING_SOUTHWEST, 0);
    Actor_FaceDirection(6, FACING_NORTHEAST, 20);
    Actor_SetModeById(5, 3);
    Motion_SetModeAndWaitAnimation(6, 3);
    *(u16 *)(EVENT_WORK + 0x1c4) += 3;
  } else {
    *(u16 *)(EVENT_WORK + 0x1c4) += 1;
    Actor_ShowEmoteById(26, EMOTE_IN_FRONT | EMOTE_SCOWL, 20);
    Event_ShowMessageAndWait(SPEAKER_PORTRAIT_LEFT | 26, 0);
    Actor_FaceDirectionAndWait(5, FACING_SOUTHEAST);
    Event_ShowMessageAndWait(SPEAKER_PORTRAIT_LEFT | 5, 0);
    Actor_FaceDirectionById(6, FACING_EAST);
    Event_ShowMessageAndWait(SPEAKER_PORTRAIT_LEFT | 6, 0);
    Actor_ArmCallback(gMainRoutineWork.currentOwner, FACING_WEST, 0);
    FieldEffect_SetAttached(gMainRoutineWork.currentOwner,
                            ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
    Event_Wait(40);
    Func_02005c7c();
  }
  Actor_ShowEmoteById(7, EMOTE_IN_FRONT | EMOTE_SWEAT_DROP, 40);
  Event_ShowMessageAndWait(SPEAKER_PORTRAIT_LEFT | 7, 0);
  Actor_FaceDirection(gMainRoutineWork.currentOwner, FACING_NORTH, 0);
  Actor_FaceDirection(6, FACING_NORTH, 0);
  Actor_FaceDirection(26, FACING_NORTHWEST + FACING_STEP, 0);
  Actor_FaceDirectionAndWait(5, FACING_NORTH);
  Actor_SetModeById(5, 3);
  Event_ShowMessageAndWait(SPEAKER_PORTRAIT_LEFT | SPEAKER_WINDOW_BELOW | 5, 0);
  Motion_SetModeAndWaitAnimation(7, 3);
  Actor_SetModeById(6, 3);
  Event_ShowMessageAndWait(SPEAKER_PORTRAIT_LEFT | 6, 0);
  Actor_SetModeById(26, 3);
  Event_ShowMessageAndWait(SPEAKER_PORTRAIT_LEFT | 26, 0);
  Motion_SetModeAndWaitAnimation(7, 3);
  Actor_SetSpeed(7, 0.4);
  Actor_WalkToAndWaitById(7, 40, 350);
  Actor_SetModeById(gMainRoutineWork.currentOwner, 3);
  Actor_SetModeById(5, 3);
  Actor_SetModeById(6, 3);
  Motion_SetModeAndWaitAnimation(26, 3);
  Motion_SetModeAndWaitAnimation(7, 3);
  Actor_SetSpeed(7, 0.8);
  Actor_SetMovementTypeById(5, gData_02005fe0 + 0);
  Actor_SetMovementTypeById(6, gData_02005fe0 + 0);
  Actor_SetMovementTypeById(26, gData_02005fe0 + 0);
  Actor_SetMovementTypeAndWaitById(7, gData_02005fe0 + 0);
  GameFlag_Set(0x8f4);
  GameFlag_Set(0x92e);
  Event_PrepareObjectAndApplyValue(7, 1);
  Party_GivePiersDjinn();
}
#undef EVENT_WORK

#define EVENT_WORK ((u8 *)Ram_HeapSlots->eventWork)

extern u32 gData_02006014[];
extern u32 gData_02006058[];
extern u32 gData_0200609c[];
extern u32 gData_020060e0[];
extern u32 gData_02006108[];
void Func_0200384c(void);

void Kibombo_DeadEndByTheStatue(void) {
  s32 done = Event_GetFlag(0x86a);

  if (done != 0)
    return;
  Event_Start();
  Event_ResolvePendingActions(0);
  Actor_SetSpeed(gMainRoutineWork.currentOwner, 0.8);
  Actor_WalkToAndWaitById(gMainRoutineWork.currentOwner, 344, 208);
  Actor_WalkToAndWaitById(gMainRoutineWork.currentOwner, 308, 200);
  Actor_WalkToAndWait(gMainRoutineWork.currentOwner, 290, 188);
  Actor_ArmCallback(gMainRoutineWork.currentOwner, FACING_NORTHWEST, 0);
  Actor_SetSpeed(5, 0.8);
  Actor_SetSpeed(6, 0.8);
  Actor_SetSpeed(26, 0.8);
  Actor_SetSpeed(7, 0.8);
  Actor_MoveToObject(5, gMainRoutineWork.currentOwner);
  Actor_MoveToObject(6, gMainRoutineWork.currentOwner);
  Actor_MoveToObject(26, gMainRoutineWork.currentOwner);
  Actor_MoveToObject(7, gMainRoutineWork.currentOwner);
  wait(1);
  Actor_SetMovementTypeById(5, gData_02006014);
  Actor_SetMovementTypeById(6, gData_02006058);
  Actor_SetMovementTypeById(26, gData_0200609c);
  Actor_SetMovementTypeAndWaitById(7, gData_020060e0);
  Event_Wait(20);
  Actor_FaceDirection(7, FACING_WEST, 0);
  Event_SetMessageId(MsgItSADead);
  Event_ShowMessageAndWait(SPEAKER_WINDOW_BELOW | 7, 0);
  Camera_SetSpeedParameters(0x26666, 0x4ccc);
  ((struct Actor *)((s32)FieldCamera_GetActor()))->motionFlags = done;
  FieldCamera_MoveTo(200, CAMERA_KEEP, 180, 1);
  FieldCamera_WaitForArrival();
  Event_Wait(10);
  FieldCamera_MoveTo(174, CAMERA_KEEP, 224, 1);
  FieldCamera_WaitForArrival();
  Event_Wait(10);
  FieldCamera_MoveTo(268, CAMERA_KEEP, 120, 1);
  FieldCamera_WaitForArrival();
  Event_Wait(20);
  FieldCamera_MoveTo(272, CAMERA_KEEP, 180, 1);
  FieldCamera_WaitForArrival();
  Actor_SetModeById(6, 4);
  Event_ShowMessageAndWait(6, 0);
  Actor_ShowEmote(5, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
  Event_ShowMessageAndWait(5, 0);
  Actor_ShowEmoteById(26, EMOTE_IN_FRONT | EMOTE_LIGHT_BULB, 20);
  Actor_WatchAndSetScript(gMainRoutineWork.currentOwner, 26);
  Actor_WatchAndSetScript(5, 26);
  Actor_WatchAndSetScript(6, 26);
  Actor_WatchAndSetScript(7, 26);
  FieldCamera_SetSpeed(1.2);
  FieldCamera_MoveTo(268, CAMERA_KEEP, 146, 1);
  Actor_SetSpeed(26, 1.2);
  Actor_WalkToAndWait(26, 290, 116);
  Actor_WalkToAndWait(26, 268, 109);
  Actor_FaceDirection(26, FACING_SOUTH + FACING_STEP, 20);
  Actor_FaceDirection(26, FACING_SOUTHEAST + FACING_STEP, 20);
  Actor_FaceDirection(26, FACING_SOUTH + FACING_STEP, 40);
  Event_ShowMessageAndWait(SPEAKER_WINDOW_BELOW | 26, 0);
  FieldCamera_MoveWithinMapBounds(PIXELS(268), -1, PIXELS(188), 1);
  Actor_WalkToAndWaitById(26, 218, 156);
  Actor_ArmCallback(26, FACING_EAST, 40);
  Actor_Stop(gMainRoutineWork.currentOwner);
  Actor_Stop(5);
  Actor_Stop(6);
  Actor_Stop(7);
  Event_ShowMessageAndWait(26, 0);
  Actor_FaceDirection(gMainRoutineWork.currentOwner, FACING_WEST, 0);
  Actor_ArmCallback(7, FACING_EAST, 0);
  Actor_FaceDirection(5, FACING_NORTHEAST, 0);
  Actor_FaceDirection(6, FACING_SOUTHWEST, 20);
  Actor_SetSpeed(26, 0.4);
  Actor_WalkToAndWaitById(26, 248, 156);
  Event_Wait(40);
  Motion_SetModeAndWaitAnimation(26, 3);
  Event_ShowMessageAndWait(26, 0);
  Actor_WatchAndSetScript(gMainRoutineWork.currentOwner, 26);
  Actor_WatchAndSetScript(5, 26);
  Actor_WatchAndSetScript(6, 26);
  Actor_WatchAndSetScript(7, 26);
  Event_Wait(20);
  Actor_GetById(26)->actionFlags &= ~1;
  Actor_WalkToAndWaitById(26, 248, 188);
  Event_Wait(1);
  Actor_GetById(26)->actionFlags |= 1;
  Event_Wait(20);
  Actor_SetModeById(26, 7);
  Event_Wait(20);
  Actor_ShowEmote(gMainRoutineWork.currentOwner,
                  EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
  Actor_ShowEmote(5, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
  Actor_ShowEmote(6, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
  Actor_ShowEmote(7, EMOTE_IN_FRONT | EMOTE_QUESTION, 40);
  Actor_Stop(gMainRoutineWork.currentOwner);
  Actor_Stop(5);
  Actor_Stop(6);
  Actor_Stop(7);
  Event_ShowMessageAndWait(26, 0);
  Actor_FaceDirection(5, FACING_NORTHEAST, 0);
  Actor_FaceDirection(6, FACING_SOUTHWEST, 0);
  Actor_ShowEmote(5, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
  Actor_ShowEmote(6, EMOTE_IN_FRONT | EMOTE_QUESTION, 20);
  Motion_SetModeAndWaitAnimation(gMainRoutineWork.currentOwner, 3);
  Actor_SetSpeedParameters(gMainRoutineWork.currentOwner, 0x6666, 0x3333);
  Actor_WalkToAndWaitById(gMainRoutineWork.currentOwner, 280, 188);
  Actor_SetModeById(26, 1);
  Event_Wait(10);
  Actor_GetById(26)->actionFlags &= ~1;
  Actor_WalkToAndWaitById(26, 238, 188);
  Event_Wait(1);
  Actor_GetById(26)->actionFlags |= 1;
  FieldMessage_ShowAtActor(26, 0);
  Actor_ShowEmoteById(gMainRoutineWork.currentOwner,
                      EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
  Actor_FaceDirection(5, FACING_WEST, 0);
  Actor_FaceDirection(6, FACING_WEST, 0);
ask:
  if (Event_AskYesNoWithActorAnimation(gMainRoutineWork.currentOwner, 0) == 0) {
    *(u16 *)(EVENT_WORK + 0x1c4) += 1;
  } else {
    Event_ShowMessageAndWait(26, 0);
    *(s16 *)(EVENT_WORK + 0x1c4) -= 2;
    FieldMessage_ShowAtActor(26, 0);
    goto ask;
  }
  Event_SetFlag(FLAG_SCENE_TEMP_21);
  Func_0200384c();
  GameFlag_Clear(FLAG_SCENE_TEMP_21);
  Actor_ShowEmote(gMainRoutineWork.currentOwner,
                  EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
  Actor_ShowEmote(5, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
  Actor_ShowEmote(6, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
  Actor_ShowEmote(7, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 40);
  Actor_SetModeById(26, 3);
  Event_ShowMessageAndWait(26, 0);
  Actor_SetAttachedEffect(5, ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  Event_ShowMessageAndWait(5, 0);
  Actor_SetModeById(26, 3);
  Event_ShowMessageAndWait(26, 0);
  Actor_FaceDirection(6, FACING_SOUTH, 0);
  Event_ShowMessageAndWait(SPEAKER_WINDOW_ABOVE | 6, 0);
  Actor_FaceDirection(26, FACING_SOUTHEAST + FACING_STEP, 20);
  Motion_SetModeAndWaitAnimation(26, 3);
  Actor_FaceDirection(6, FACING_WEST, 0);
  Actor_FaceDirectionById(26, FACING_EAST);
  Event_ShowMessageAndWait(26, 0);
  Event_ShowMessageAndWait(26, 0);
  Motion_SetModeAndWaitAnimation(gMainRoutineWork.currentOwner, 3);
  Actor_WatchAndSetScript(5, gMainRoutineWork.currentOwner);
  Actor_WatchAndSetScript(6, gMainRoutineWork.currentOwner);
  Actor_WatchAndSetScript(26, gMainRoutineWork.currentOwner);
  Actor_WatchAndSetScript(7, gMainRoutineWork.currentOwner);
  FieldCamera_MoveTo(268, CAMERA_KEEP, 164, 1);
  Actor_SetSpeedParameters(gMainRoutineWork.currentOwner, 0xcccc, 0x6666);
  Actor_WalkToAndWait(gMainRoutineWork.currentOwner, 280, 120);
  Actor_FaceDirectionAndWait(gMainRoutineWork.currentOwner, FACING_WEST);
  Event_SetFlag(FLAG_SCENE_TEMP_20);
  Func_0200384c();
  GameFlag_Clear(FLAG_SCENE_TEMP_20);
  Actor_WalkToAndWaitById(gMainRoutineWork.currentOwner, 232, 142);
  Actor_FaceDirectionAndWait(gMainRoutineWork.currentOwner, FACING_SOUTH);
  Event_SetFlag(FLAG_SCENE_TEMP_22);
  Func_0200384c();
  GameFlag_Clear(FLAG_SCENE_TEMP_22);
  Actor_WalkToAndWait(gMainRoutineWork.currentOwner, 296, 142);
  Actor_FaceDirectionAndWait(gMainRoutineWork.currentOwner, FACING_SOUTH);
  Event_SetFlag(FLAG_SCENE_TEMP_23);
  Func_0200384c();
  GameFlag_Clear(FLAG_SCENE_TEMP_23);
  FieldCamera_MoveTo(268, CAMERA_KEEP, 188, 1);
  Actor_WalkToAndWaitById(gMainRoutineWork.currentOwner, 284, 188);
  Actor_FaceDirection(gMainRoutineWork.currentOwner, FACING_WEST, 10);
  Actor_Stop(5);
  Actor_Stop(6);
  Actor_Stop(26);
  Actor_Stop(7);
  Motion_SetModeAndWaitAnimation(26, 3);
  Event_ShowMessageAndWait(26, 0);
  Actor_ShowEmote(5, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
  Event_ShowMessageAndWait(5, 0);
  Actor_FaceDirection(26, FACING_NORTH + FACING_STEP, 0);
  Event_ShowMessageAndWait(26, 0);
  Actor_FaceDirection(gMainRoutineWork.currentOwner, FACING_NORTHWEST, 0);
  Actor_FaceDirection(5, FACING_NORTHWEST, 0);
  Actor_FaceDirection(6, FACING_NORTHWEST, 0);
  Actor_FaceDirection(7, FACING_NORTHEAST, 0);
  Actor_SetAttachedEffect(7, ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  Event_ShowMessageAndWait(7, 0);
  Actor_ShowEmoteById(26, EMOTE_IN_FRONT | EMOTE_SWEAT_DROP, 0);
  Actor_FaceDirectionAndWait(26, FACING_WEST);
  Event_ShowMessage(SPEAKER_PORTRAIT_RIGHT | 26, 0);
  Actor_ShowEmote(7, EMOTE_IN_FRONT | EMOTE_QUESTION, 20);
  Actor_FaceDirectionAndWait(26, FACING_NORTH + FACING_STEP);
  Event_ShowMessageThenWait(26, 0, 20);
  Actor_FaceDirection(5, FACING_NORTHEAST, 0);
  Actor_FaceDirection(6, FACING_SOUTHWEST, 20);
  Event_ShowMessageAndWait(6, 0);
  Actor_ArmCallback(26, FACING_EAST, 10);
  Actor_SetModeById(26, 4);
  Event_ShowMessageAndWait(26, 0);
  Actor_FaceDirection(6, FACING_NORTHWEST, 0);
  Actor_FaceDirection(26, FACING_NORTH + FACING_STEP, 0);
  FieldCamera_MoveTo(268, CAMERA_KEEP, 164, 1);
  Actor_WalkToAndWait(5, 296, 168);
  Actor_WalkToAndWait(5, 280, 154);
  Actor_FaceDirection(5, FACING_WEST, 40);
  Event_ShowMessageAndWait(5, 0);
  Actor_ShowEmote(26, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
  Event_ShowMessageAndWait(26, 0);
  Actor_FaceDirection(5, FACING_SOUTHWEST, 0);
  Event_ShowMessageAndWait(5, 0);
  Actor_ShowEmote(26, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 20);
  Event_ShowMessageAndWait(26, 0);
  FieldCamera_MoveWithinMapBounds(PIXELS(268), -1, PIXELS(188), 1);
  Actor_FaceDirection(gMainRoutineWork.currentOwner, FACING_WEST, 0);
  Actor_FaceDirection(6, FACING_WEST, 0);
  Actor_ArmCallback(7, FACING_EAST, 0);
  Actor_FaceDirection(26, FACING_SOUTHEAST + FACING_STEP, 40);
  Actor_FaceDirection(26, FACING_SOUTH + FACING_STEP, 20);
  Actor_FaceDirection(26, FACING_NORTH + FACING_STEP, 40);
  Actor_FaceDirectionById(26, FACING_EAST);
  Event_ShowMessageAndWait(26, 0);
  Actor_SetAttachedEffect(26, ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  Event_ShowMessageAndWait(26, 0);
  Actor_ArmCallback(gMainRoutineWork.currentOwner, FACING_NORTH, 0);
  Actor_FaceDirection(5, FACING_SOUTH, 20);
  Actor_SetModeById(26, 4);
  Event_ShowMessageAndWait(26, 0);
  Actor_FaceDirection(5, FACING_SOUTHWEST, 0);
  Actor_FaceDirection(gMainRoutineWork.currentOwner, FACING_WEST, 0);
  Actor_FaceDirection(26, FACING_SOUTHEAST + FACING_STEP, 20);
  Event_ShowMessageAndWait(26, 0);
  Actor_ArmCallback(26, FACING_EAST, 10);
  Event_ShowMessageAndWait(26, 0);
  Actor_ShowEmoteById(7, EMOTE_IN_FRONT | EMOTE_SCOWL, 0);
  Event_ShowMessageAndWait(7, 0);
  Actor_FaceDirectionAndWait(26, FACING_WEST);
  Actor_SetModeById(26, 4);
  Event_ShowMessageAndWait(SPEAKER_PORTRAIT_RIGHT | 26, 0);
  Actor_ShowEmoteById(7, EMOTE_IN_FRONT | EMOTE_QUESTION, 40);
  Actor_FaceDirectionById(26, FACING_EAST);
  Event_ShowMessageAndWait(26, 0);
  Actor_ShowEmote(7, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
  Actor_ShowEmote(gMainRoutineWork.currentOwner,
                  EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
  Actor_ShowEmote(5, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
  Actor_ShowEmoteById(6, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 40);
  Actor_ShowEmoteById(26, EMOTE_IN_FRONT | EMOTE_SMILE, 20);
  Event_ShowMessageAndWait(26, 0);
  Actor_SetModeById(26, 3);
  Event_ShowMessageAndWait(26, 0);
  Actor_SetModeById(gMainRoutineWork.currentOwner, 3);
  Actor_SetModeById(5, 3);
  Actor_SetModeById(6, 3);
  Motion_SetModeAndWaitAnimation(7, 3);
  Actor_SetSpeed(5, 1.2);
  Actor_SetSpeed(6, 1.2);
  Actor_SetSpeed(26, 1.2);
  Actor_SetSpeed(7, 1.2);
  Actor_SetMovementTypeById(5, gData_02006108);
  Actor_SetMovementTypeById(6, gData_02006108);
  Actor_SetMovementTypeById(26, gData_02006108);
  Actor_SetMovementTypeAndWaitById(7, gData_02006108);
  GameFlag_Set(0x8f5);
  Event_End();
}

#undef EVENT_WORK

void Func_02002ad8(void) {
  struct KibomboWork90 *work = Ram_HeapSlots->unk84[3];

  work->leader = Actor_GetById(gMainRoutineWork.currentOwner);
}

void Func_02002af8(void) {
  struct KibomboWork90 *work = Ram_HeapSlots->unk84[3];

  work->leader = 0;
}

s32 Func_02002b08(s32 value) {
  if (value > 31)
    value = 31;
  if (value < 0)
    value = 0;
  return value;
}

#include "system/io_reg.h"

void Func_02002b18(s32 index, s32 entry, s32 delta) {
  u16 color = gData_02007948[entry];
  s16 r = color & 0x1f;
  s16 g = (color >> 5) & 0x1f;
  s16 b = (color >> 10) & 0x1f;

  r += delta;
  g += delta >> 1;
  b += delta >> 2;

  r = Func_02002b08(r);
  g = Func_02002b08(g);
  b = Func_02002b08(b);
  {
    u16 *dst = &((u16 *)BG_PLTT)[index];

    r |= (b << 10) | (g << 5);
    *dst = r;
  }
}

void Func_02002ba4(s32 index, s32 entry, s32 delta) {
  u16 color = gData_02007948[entry];
  s16 r = color & 0x1f;
  s16 g = (color >> 5) & 0x1f;
  s16 b = (color >> 10) & 0x1f;

  r += delta + 4;
  delta -= 4;
  g += delta >> 1;
  b += delta >> 2;

  r = Func_02002b08(r);
  g = Func_02002b08(g);
  b = Func_02002b08(b);
  {
    u16 *dst = &((u16 *)BG_PLTT)[index];

    r |= (b << 10) | (g << 5);
    *dst = r;
  }
}

#define DEFINE_KIBOMBO_WOBBLE(name, n1, n2, n3, n4, call1, n5, n6, n7)         \
  void name(void) {                                                            \
    struct KibomboEventWork *work = Ram_HeapSlots->eventWork;                  \
                                                                               \
    if (work->field166 == 0 && work->field15a == 0 && work->field15e == 0 &&   \
        (gFrameCount & 7) == 0) {                                              \
      Func_02002b18(9, 0, ((Random16() * 10) >> 16) - 5);                      \
      Func_02002b18(n1, 1, ((Random16() * n2) >> 16) - n3);                    \
      Func_02002b18(n4, 2, ((Random16() * 4) >> 16) - 2);                      \
      call1(n5, 3, ((Random16() * n6) >> 16) - n7);                            \
    }                                                                          \
  }

DEFINE_KIBOMBO_WOBBLE(Func_02002c34, 87, 8, 4, 103, Func_02002b18, 119, 4, 2)

DEFINE_KIBOMBO_WOBBLE(Func_02002ccc, 7, 6, 3, 6, Func_02002ba4, 76, 8, 4)

void Func_02002d64(void) {
  struct KibomboEventWork *work = Ram_HeapSlots->eventWork;

  if (work->field166 == 0 && work->field15a == 0 && work->field15e == 0 &&
      (gFrameCount & 7) == 0) {
    Func_02002b18(169, 0, ((Random16() * 4) >> 16) - 2);
    Func_02002b18(167, 1, ((Random16() * 2) >> 16) - 1);
    Func_02002b18(166, 2, Random16() >> 16);
  }
}

void Func_02002de0(void) {
  struct Actor **slot = &gData_02007950;
  s32 zero = 0;
  struct Actor *object =
      Import_ActorCreate(0x1e9, 0x1090000, 0x240000, 0x1580000);

  *slot = object;
  if (object != 0) {
    struct FieldSprite *sprite = object->sprite;
    u8 *buffer;

    sprite->fullColor = 0;
    sprite->palette = 0;
    sprite->priority = 1;
    object->motionFlags = zero;
    object->unk5c[0] = 1;
    sprite->nextFrame = zero;
    sprite->unk1b[0] = zero;
    buffer = HeapSlot_Alloc(68, 0x608);
    Icon_DrawItem(ITEM_BLACK_CRYSTAL);
    VramCache_Load(sprite->vramBlock, 128, buffer + 0x400);
    HeapSlot_Release(68);
  }
}

void Func_02002e60(void) {
  struct Actor *leader = Actor_GetById(gMainRoutineWork.currentOwner);

  if (leader->y > 0x100000) {
    Actor_SetHorizontalPositionWithTerrain(29, 0, 0);
    Actor_SetSpritePriority(30, 2);
  } else {
    Kibombo_ObjSetPos(Actor_GetById(29), PIXELS(120), 0, PIXELS(168));
    Actor_SetSpritePriority(30, 3);
  }
}

void Func_02002eb0(void) {
  if (gData_02007950 != 0) {
    struct Actor *object = Actor_GetById(27);

    Actor_SetPositionAndResetMotion(gData_02007950, object->x,
                                    object->y + PIXELS(12), object->z);
  }
}

#include "../field/field_effect/advance_motion.c"

#undef DEFINE_KIBOMBO_WOBBLE
#undef Actor_SetSpriteFlags
#undef Actor_SetBlendMode
#undef Actor_WaitForMove
#undef FieldCamera_GetActor
#undef EVENT_WORK
#undef Actor_SetPositionAndResetMotion
#undef Actor_SetSpritePriority
#undef Actor_SetMoveTarget
#undef Import_ActorCreate
#undef Actor_SetSpritePriority
#undef Actor_SetPositionAndResetMotion
// @file
#include "flags.h"
#include "shared/tables.h"
#include "kibombo.h"
#include "event_work.h"
#include "module/field/event_commands.h"
#include "module/scene/kibombo.h"
#include "module/scene/scene.h"
#include "module/field/field_effect.h"

void Actor_StartScript(struct Actor *object, const void *script);
extern u32 gData_02005ca4[];

void Func_02002f34(void) {
  s32 phase = gFrameCount & 3;

  if (phase == 0) {
    struct Actor *object =
        Import_ActorCreate(0x16e, 0x1080000, 0x200000, 0x13c0000);
    s32 value;

    if (object != 0) {
      object->bounce = Trig_Cos(((Random16() << 13) >> 16) + 0x3000);
      object->gravity = 0x10000;
      object->unk4c = 0x40000 - (u16)Random16();
      value = (u16)Random16() + 0xffff8000;
      object->timer = value;
      object->motionFlags = 0;
      Actor_SetSpriteFlags(object, 0);
      Actor_SelectAnimation(object, 2);
      Actor_StartScript(object, gData_02005ca4);
      ((struct FieldSprite *)object->sprite)->priority = 1;
      object->timer = 0;
      object->speedLimit = 0;
      object->acceleration = 0;
      object->update = (void (*)(void))FieldEffect_AdvanceMotion;
    }
  }
}

void Func_02002fec(struct Actor *object) {
  object->x += object->timer << 8;

  object->y += 0x8000;

  object->scaleX += 0x51e;
  object->scaleY += 0x51e;
  object->timer += 2;
  if (--*(s32 *)&object->watchedObject == 0)
    Actor_Release(object);
}

void Func_02003030(void) {
  if ((gFrameCount & 63) == 0) {
    struct Actor *object =
        Import_ActorCreate(0x11d, 0x580000, 0x100000, 0x13a0000);
    u8 *part;
    s32 mask;

    if (object != 0) {
      object->unk5c[0] = 2;
      object->motionFlags = 0;
      object->timer = 0;
      object->priorityFlags &= ~1;
      mask = 13;
      part = (u8 *)object->sprite;
      part[9] = (part[9] & -mask) | 8;
      Actor_SetSpriteFlags(object, 0);
      object->scaleX = 0x8000;
      object->scaleY = 0x8000;
      *(s32 *)&object->watchedObject = 60;
      object->update = (void (*)(void))Func_02002fec;
      Actor_SelectAnimation(object, 5);
      Actor_SetBlendMode(object, 1);
    }
  }
}

void Func_020030b8(struct Actor *object) {
  if (object->timer != 0) {
    object->timer--;
  } else {
    object->timer = ((Random16() * 10) >> 16) + 10;
    if (object->unk66 != 0) {
      object->scaleX = 0x18000;
      object->unk66 = 0;
    } else {
      object->scaleX = 0xfffe8000;
      object->unk66 = 1;
    }
  }
}

void Func_02003104(s32 x, s32 y, s32 z) {
  struct Actor *object = Import_ActorCreate(0x106, x, y, z);
  struct FieldSprite *sprite;

  if (object != 0) {
    object->motionFlags = 0;
    object->priorityFlags &= ~1;
    object->scaleY = 0x20000;
    object->timer = ((Random16() * 10) >> 16) + 10;
    object->unk66 = 0;
    object->update = (void (*)(void))Func_020030b8;
    sprite = object->sprite;
    sprite->priority = 1;
    Actor_SetSpriteFlags(object, 0);
    Actor_SelectAnimation(object, 5);
    Actor_SetBlendMode(object, 1);
  }
}

#include "system/io_reg.h"

struct KibomboEffectWork {
  u8 unk000[0x7c4];
  s16 field7c4;
};

static inline void Kibombo_Spawn(s32 x, s32 y, s32 z) {
  Func_02003104(x, y, z);
}

void Func_02003184(void) {
  s32 x = 0x1b80000;

  REG_BLDALPHA = BLEND_ALPHA(8, 12);
  REG_BLDCNT = BLDCNT_TGT1_OBJ | BLDCNT_TGT2_ALL;
  Kibombo_Spawn(x, 0x280000, 0x1c20000);
  Kibombo_Spawn(x, 0x2e0000, 0x1b60000);
  Kibombo_Spawn(x, 0x340000, 0x1aa0000);
  Kibombo_Spawn(x, 0x3a0000, 0x19e0000);
  /* FAKEMATCH(dead): an empty once-only block before the last spawn. */
  do {
  } while (0);
  Func_02003104(x, 0x400000, 0x1920000);
}

#include "scene_tables.h"

#include "shared/actor_hide.h"

void Scene_RunUnlessFlag(void) {
  if (GameFlag_Get(FLAG_KEEP_PARTY_POSITION)) {
    if (GameFlag_Get(0x200) == 0)
      Kibombo_ShowActors();
  } else {
    GameFlag_Set(0x200);
  }
}

void Func_0200324c(void) {
  u32 i;
  s32 zero;

  for (i = 0; i <= 1; i++) {
    s32 id = i + 15;
    struct Actor *object = Actor_GetById(id);

    Actor_Hide(id);
    zero = 0;
    object->motionFlags = zero;
  }
  Actor_GetById(15)->y = PIXELS(32);
  Actor_GetById(16)->y = zero;
  Scene_RunUnlessFlag();
}

void Func_02003288(void) {
  u32 i;

  for (i = 0; i <= 1; i++) {
    s32 id = i + 26;
    struct Actor *object = Actor_GetById(id);

    Actor_Hide(id);
    object->motionFlags = 0;
  }
  Scene_RunUnlessFlag();
}

#define DEFINE_KIBOMBO_SET_CROWD_MODE(name, n1)                                \
  void name(void) {                                                            \
    u32 i;                                                                     \
                                                                               \
    for (i = 0; i <= 7; i++)                                                   \
      Actor_SetModeById(i + 18, n1);                                           \
  }

DEFINE_KIBOMBO_SET_CROWD_MODE(Func_020032b0, 5)

DEFINE_KIBOMBO_SET_CROWD_MODE(Func_020032c8, 1)

void Func_020032e0(void) {
  BattleEffect_LoadActionEffectResources(143, 1);
  FieldPsynergy_SetupActorPair(17, 27);
  EventObject_Initialize();
  FieldPsynergy_RunHandler(1);
  BattleEffect_StopMainActor();
  BattleEffect_CleanupSceneObjects();
}

#include "types.h"
#include "actor.h"
#include "field_sprite.h"
#include "system/vector3.h"
#include "field/event_inline.h"

s32 VramCache_FreeEntry(u32 slot);

union Whirl {
  struct Actor actor;
  struct {
    u8 unused[0x62];
    u8 stage;
    u8 timer;
  } whirl;
};

void Func_02003308(struct Actor *actor) {
  s32 wobble = 0;
  union Whirl *object;
  u8 *timer = &((union Whirl *)actor)->whirl.timer;
  s16 *action;
  u8 *stage;
  struct Vector3 centre;
  s32 radius;
  s32 spin;
  s32 turn;

  if (*timer != 0)
    wobble = Trig_Sin((actor->timer + (s16)actor->unk66) * 9 << 9) * 6;
  object = (union Whirl *)actor;
  action = &object->actor.timer;
  centre.x = object->actor.targetX;
  centre.y = object->actor.targetY;
  centre.z = object->actor.targetZ;
  radius = object->actor.unk4c;
  spin = (s16)object->actor.unk66;
  turn = *action;
  Vector_AddPolarOffset((radius << 16) + wobble, turn * 3 * 256 + spin,
                        &centre);
  object->actor.x = centre.x;
  object->actor.y = centre.y;
  object->actor.z = centre.z;
  stage = &object->whirl.stage;
  switch (*stage) {
  case 0:
    *action += 4;
    object->actor.unk4c = 1;
    ((struct FieldSprite *)object->actor.sprite)->rotation += 0x1000;
    object->actor.scaleX += 0x666;
    object->actor.scaleY += 0x666;
    if (object->actor.scaleX > 0x18000 &&
        ((struct FieldSprite *)object->actor.sprite)->rotation == 0) {
      *stage = 1;
      *timer = 1;
    }
    break;
  case 1:
    *action += 4;
    ((struct FieldSprite *)object->actor.sprite)->rotation += 0x1000;
    if (((struct FieldSprite *)object->actor.sprite)->rotation == 0 &&
        --*timer == 0) {
      Sound_Play(SE_SHIMMER);
      *stage = 2;
    }
    break;
  case 2:
    *action += 1;
    object->actor.targetY += 0x40000;
    if (object->actor.targetY > 0x640000)
      *stage = 3;
    break;
  case 3:
    *action += 1;
    object->actor.unk4c += 2;
    *timer = 10;
    if (object->actor.unk4c > 40)
      *stage = 4;
    break;
  case 4:
    *action += 1;
    if (--*timer == 0)
      *stage = 5;
    break;
  case 5:
    *action += 1;
    object->actor.unk4c -= 2;
    if (object->actor.targetY < 0x880000)
      object->actor.targetY += 0x20000;
    if (object->actor.unk4c <= 1) {
      object->actor.unk4c = 0;
      *stage = 6;
    }
    break;
  case 6:
    *action += 4;
    ((struct FieldSprite *)object->actor.sprite)->rotation += 0x1000;
    object->actor.scaleX += -0x1000;
    object->actor.scaleY += -0x1000;
    if (object->actor.scaleX < 0x1200) {
      object->actor.targetX = 100;
      object->actor.targetZ = 100;
      *stage = 7;
    }
    break;
  case 7:
    VramCache_FreeEntry(
        ((struct FieldSprite *)object->actor.sprite)->vramBlock);
    Actor_Release(&object->actor);
    break;
  }
}

void Func_02003504(void) {
  struct Actor *center = Actor_GetById(17);
  struct Actor *spark;
  s32 previous = 0;
  u32 i;
  u32 angle;
  s32 x;
  s32 y;
  s32 z;

  Sound_Play(SE_CAST);
  for (i = 0; i <= 7; i++) {
    Actor_SetSpriteColor(Actor_GetById(17), 7);
    wait(2);
    Actor_SetSpriteColor(Actor_GetById(17), 0);
    wait(2);
  }
  Sound_Play(SE_MERCURY_LIGHTHOUSE_DOOR_OPEN);
  i = 0;
  angle = 0;
  do {
    x = center->x;
    y = center->y + 0x180000;
    z = center->z;
    spark = Import_ActorCreate(673, x, y, z);
    if (spark != 0) {
      previous = Sprite_ShareVramCacheEntry(spark->sprite, previous);
      spark->motionFlags = 0;
      ((struct FieldSprite *)spark->sprite)->priority = 1;
      Actor_SetSpriteFlags(spark, 0);
      Actor_SelectAnimation(spark, 1);
      spark->timer = 0;
      spark->unk4c = 0;
      spark->unk66 = angle / 360;
      spark->riseCounter = 0;
      spark->targetX = center->x;
      spark->targetY = center->y + 0x180000;
      spark->targetZ = center->z;
      spark->update = (void (*)(void))Func_02003308;
    }
    angle += 0x160000;
    i++;
  } while (i <= 15);
}

void Func_02003610(void) {
  Task_Remove(Func_02002ccc);
  wait(1);
  FieldCamera_WaitForArrival();
  Event_Wait(40);
  if (gData_02007950 != 0)
    Func_02003504();
}

void Func_02003640(s32 open, s32 hide) {
  if (open != 0) {
    Kibombo_MapCopy(49, 37, 79, 15, 3, 2);
    Kibombo_ObjSetPos(Actor_GetById(12), PIXELS(253), PIXELS(32), PIXELS(289));
    Actor_SetPositionAndResetMotion(Actor_GetById(13), PIXELS(275), PIXELS(32),
                                    PIXELS(289));
    if (hide != 0) {
      Actor_SetModeById(12, 0);
      Actor_SetModeById(13, 0);
    } else {
      Actor_SetModeById(12, 1);
      Actor_SetModeById(13, 1);
    }
  } else {
    Kibombo_MapCopy(45, 37, 79, 15, 3, 2);
    Actor_SetHorizontalPositionWithTerrain(12, 0, 0);
    Actor_SetHorizontalPositionWithTerrain(13, 0, 0);
  }
}

void Func_020036e0(s32 open, s32 mode) {
  if (open != 0) {
    MapGrid_CopyCellPictureIndicesRect(41, 38, 79, 16, 3, 3);
    Kibombo_BlendStart(12, 21, 1, 1, 16, 22);
    if (mode != 0)
      Actor_SetPositionAndResetMotion(Actor_GetById(14), PIXELS(264),
                                      PIXELS(32), PIXELS(308));
    switch (mode) {
    case 0:
      Actor_SetHorizontalPositionWithTerrain(14, 0, 0);
      break;
    case 1:
      Actor_SetModeById(14, 0);
      break;
    case 2:
      Actor_SetModeById(14, 1);
      break;
    case 3:
      Actor_SetModeById(14, 2);
      break;
    case 4:
      Actor_SetModeById(14, 3);
      break;
    case 5:
      Actor_SetModeById(14, 6);
      Actor_GetById(14)->y -= PIXELS(34);
      break;
    case 6:
      Actor_SetModeById(14, 7);
      Actor_GetById(14)->y -= PIXELS(34);
      break;
    }
  } else {
    MapGrid_CopyCellPictureIndicesRect(45, 38, 79, 16, 3, 3);
    Kibombo_BlendStart(17, 22, 1, 1, 16, 22);
    Actor_SetHorizontalPositionWithTerrain(14, 0, 0);
  }
}

void Func_020037e4(void) {
  struct KibomboEffectWork *work = Ram_HeapSlots->effectWork;

  if (work->field7c4 != 0) {
    if (GameFlag_Get(FLAG_SCENE_TEMP_20))
      Func_02000624();
    else if (GameFlag_Get(FLAG_SCENE_TEMP_21))
      Func_02000668();
    else if (GameFlag_Get(FLAG_SCENE_TEMP_22))
      Func_020006ac();
    else if (GameFlag_Get(FLAG_SCENE_TEMP_23))
      Func_020006f0();
  }
}

/* VIEW(kibombo): routine Task_Add as void Task_Add(void (*func)(void), s32
 * order), as this routine calls it; task.h has s32 Task_Add(TaskFunc func, s32
 * order); called through the definition's s32, 2 of 31 instructions move in
 * every edition; not tried: Task_Add's bound declaration was not found. */
extern void Task_AddVoid__kibombo(void (*func)(void),
                                  s32 order) __asm__("Task_Add");

void Func_0200384c(void) {
  BattleEffect_LoadActionEffectResources(136, 1);
  Task_AddVoid__kibombo(Func_020037e4, 0x480);
  FieldPsynergy_SetupActorPair(gMainRoutineWork.currentOwner, -1);
  EventObject_Initialize();
  FieldPsynergy_RunHandler(1);
  BattleEffect_StopMainActor();
  Task_Remove(Func_020037e4);
  BattleEffect_CleanupSceneObjects();
}

void Func_02003898(void) {
  s16 scene = gMainRoutineWork.scene;

  if (scene == (s32)&SceneId_Kibombo4)
    Task_Remove(Func_02002c34);
  else if (scene == (s32)&SceneId_Kibombo5)
    Task_Remove(Func_02002ccc);
  else if (scene == (s32)&SceneId_Kibombo6)
    Task_Remove(Func_02002d64);
}

void Kibombo_LeaveRoom(void) {
  struct KibomboEventWork *work = Ram_HeapSlots->eventWork;

  Event_Start();
  Event_ResolvePendingActions(0);
  if (GameFlag_Get(FLAG_SCENE_TEMP_02))
    Func_02003898();
  Sound_Play(SE_MAP_EXIT);
  Event_FinishScreenTransition();
  Event_WaitTransitionFrames();
  Import_EventSetExitRequest(work->exit);
}

void Kibombo_LeaveRoomFading(void) {
  struct KibomboEventWork *work = Ram_HeapSlots->eventWork;

  work->screenEffect = 0x209;
  Kibombo_LeaveRoom();
}

void Func_02003948(void) {
  struct KibomboEventWork *work = Ram_HeapSlots->eventWork;
  struct Actor *leader;
  s32 x;
  s32 px;

  leader = Actor_GetById(gMainRoutineWork.currentOwner);
  Event_Start();
  Event_ResolvePendingActions(0);
  Kibombo_CamMove(-1, -1, -1, 0);
  Actor_SetModeById(gMainRoutineWork.currentOwner, 2);
  Actor_SetSpeedParameters(gMainRoutineWork.currentOwner, 0x8000, 0x4000);
  x = leader->x;
  px = x >> 16;
  Actor_SetMoveTarget(leader, ((8 - px % 16) << 16) + x, leader->y, leader->z);
  Actor_WaitForMove(leader);
  leader->facing = 0xc000;
  if (GameFlag_Get(FLAG_SCENE_TEMP_02))
    Func_02003898();
  Actor_SetMovementTypeById(gMainRoutineWork.currentOwner, gData_02005cd0);
  wait(12);
  Sound_Play(SE_MAP_EXIT);
  Event_FinishScreenTransition();
  Event_WaitTransitionFrames();
  Import_EventSetExitRequest(work->exit);
}

void Func_02003a08(s32 x, s32 z) {
  struct MainRoutineWork *party = &gMainRoutineWork;
  struct KibomboEventWork *work = Ram_HeapSlots->eventWork;
  struct Actor *leader = Actor_GetById(party->currentOwner);

  Event_Start();
  Event_ResolvePendingActions(0);
  leader->motionFlags = 0;
  Actor_SetSpeedParameters(gMainRoutineWork.currentOwner, 0x10000, 0x8000);
  Actor_WalkToAndWaitById(gMainRoutineWork.currentOwner, x, z);
  Actor_FaceDirectionById(gMainRoutineWork.currentOwner, FACING_NORTH);
  Actor_SetSpriteFlags(Actor_GetById(gMainRoutineWork.currentOwner), 0);
  Actor_SetModeById(gMainRoutineWork.currentOwner, 13);
  Actor_SetMoveTarget(leader, x << 16, PIXELS(-12), z << 16);
  Actor_WaitForMove(leader);
  Actor_SetModeById(gMainRoutineWork.currentOwner, 10);
  Sound_Play(SE_MAP_EXIT);
  Event_FinishScreenTransition();
  Event_WaitTransitionFrames();
  Import_EventSetExitRequest(work->exit);
}

#include "system/input.h"

void Func_02003ac8(void) {
  struct Actor *leader = Actor_GetById(gMainRoutineWork.currentOwner);
  s32 x = leader->x >> 20;
  s32 z = leader->z >> 20;

  if (x == 42 && z == 23 && (gInput.held & KEY_UP) == 0)
    return;
  if (x == 43 && z == 22 && (gInput.held & KEY_LEFT) == 0)
    return;
  Func_02003a08(680, 352);
}

#define DEFINE_KIBOMBO_STEP_WHEN_HELD(name, n1, n2, n3, n4, n5, n6, n7)        \
  void name(void) {                                                            \
    struct Actor *leader = Actor_GetById(gMainRoutineWork.currentOwner);       \
    s32 x = leader->x >> 20;                                                   \
    s32 z = leader->z >> 20;                                                   \
                                                                               \
    if (z == n1) {                                                             \
      if (x == n2 && (gInput.held & KEY_RIGHT) == 0)                           \
        return;                                                                \
      if (x == n3 && (gInput.held & KEY_LEFT) == 0)                            \
        return;                                                                \
    } else if (x == n4 && z == n5 && (gInput.held & KEY_UP) == 0) {            \
      return;                                                                  \
    }                                                                          \
    Func_02003a08(n6, n7);                                                     \
  }

DEFINE_KIBOMBO_STEP_WHEN_HELD(Func_02003b20, 38, 23, 25, 24, 39, 392, 608)

DEFINE_KIBOMBO_STEP_WHEN_HELD(Func_02003b88, 54, 42, 44, 43, 55, 696, 864)

void Kibombo_EnterVent(void) {
  if (GameFlag_Get(FLAG_SCENE_TEMP_02))
    Func_02003898();
  Func_02003a08(376, 288);
}

// Fades the screen in and runs Func_02002d64 each frame for up to 160 frames,
// or until A, B or Start is held, then fades out.
void Func_02003c14(void) {
  u32 i;

  Event_Start();
  Event_ResolvePendingActions(0);
  Event_StartScreenTransition();
  Event_WaitTransitionFrames();
  KIBOMBO_TASK_ADD(Func_02002d64, 0x480);
  Event_Wait(20);
  i = 0;
  goto test;
next:
  i++;
test:
  if (i > 159)
    goto done;
  Event_Wait(1);
  if ((gInput.held & (KEY_A | KEY_B | KEY_START)) == 0)
    goto next;
done:
  Task_Remove(Func_02002d64);
  ((struct KibomboEventWork *)Ram_HeapSlots->eventWork)->field1b4 = 32;
  Event_FinishScreenTransition();
  Event_WaitTransitionFrames();
  Import_EventSetExitRequest(11);
}

void Func_02003c84(void) {
  Sound_Play(SE_POWER_UP);
  Kibombo_FadeTo(0x7fff, 0);
  PaletteFade_StartInterpolation(1);
  wait(1);
  Kibombo_FadeTo(0x10000, 0);
  ((struct KibomboEventWork *)Ram_HeapSlots->eventWork)->field1b4 = 1;
  Event_StartScreenTransition();
  Event_WaitTransitionFrames();
  Kibombo_TaskAdd(Func_02002d64, 0x480);
  wait(60);
  Task_Remove(Func_02002d64);
  wait(1);

  Sound_Play(SE_POWER_UP);
  Kibombo_FadeTo(0x7fff, 0);
  PaletteFade_StartInterpolation(1);
  wait(1);
  Kibombo_CamMove(146 << 18, -1, PIXELS(248), 0);
  Map_CenterOnFollowedPoint();
  wait(1);
  Kibombo_FadeTo(0x10000, 0);
  PaletteFade_StartInterpolation(1);
  wait(1);
  Kibombo_TaskAdd(Func_02002d64, 0x480);
  wait(60);
  Task_Remove(Func_02002d64);
  wait(1);

  Sound_Play(SE_POWER_UP);
  BattleEffect_ApplyColorToTargetBuffer(0x7fff, 0);
  PaletteFade_StartInterpolation(1);
  wait(1);
  FieldCamera_MoveWithinMapBounds(PIXELS(248), -1, 134 << 18, 0);
  Map_CenterOnFollowedPoint();
  wait(1);
  BattleEffect_ApplyColorToTargetBuffer(0x10000, 0);
  PaletteFade_StartInterpolation(1);
  wait(1);
  Kibombo_TaskAdd(Func_02002d64, 0x480);
  wait(180);
  Task_Remove(Func_02002d64);
  wait(1);

  ((struct KibomboEventWork *)Ram_HeapSlots->eventWork)->field1b4 = 1;
  Event_FinishScreenTransition();
  Event_WaitTransitionFrames();
  Event_Wait(10);
  Import_EventSetExitRequest(10);
}

void Kibombo_CeremonyBegins(void) {
  struct Actor *leader;

  Kibombo_CamMove(-1, -1, -1, 0);
  FieldCamera_GetActor()->motionFlags = 0;
  Event_SetMessageId((s32)MsgAtLastThe);
  Event_ShowMessageAndWait(17, 0);
  Sound_Play(SOUND_FADE_OUT_BGM);
  Kibombo_ObjSpeed(gMainRoutineWork.currentOwner, 0xcccc, 0x6666);
  Kibombo_ObjWalk(gMainRoutineWork.currentOwner, 86, 455);
  Actor_SetHorizontalPositionWithTerrain(26, PIXELS(86), PIXELS(455));
  wait(1);
  Kibombo_ObjSpeed(26, 0xcccc, 0x6666);
  Kibombo_ObjWalk(26, 72, 455);
  Actor_ArmCallback(26, FACING_EAST, 0);
  Kibombo_ObjArm(gMainRoutineWork.currentOwner, FACING_WEST, 40);
  Kibombo_ObjArm(26, FACING_NORTH + FACING_STEP, 0);
  Actor_ArmCallback(gMainRoutineWork.currentOwner, FACING_NORTHEAST, 0);
  Camera_SetSpeedParameters(0x26666, 0x4ccc);
  Kibombo_CamMove(PIXELS(264), PIXELS(16), PIXELS(420), 1);
  FieldCamera_WaitForArrival();
  Event_Wait(40);
  Camera_SetSpeedParameters(0x6666, 0xccc);
  Kibombo_CamMove(PIXELS(264), PIXELS(32), PIXELS(320), 1);
  FieldCamera_WaitForArrival();
  Event_Wait(80);
  Actor_Jump(17, 4, 40);
  Event_ShowMessageAndWait(17, 0);
  Kibombo_CamSpeed(0x4cccc, 0x9999);
  Kibombo_CamMove(PIXELS(264), PIXELS(32), PIXELS(400), 1);
  FieldCamera_WaitForArrival();
  Event_Wait(20);
  Kibombo_ObjArm(17, FACING_SOUTHEAST + FACING_STEP, 20);
  Event_ShowMessageAndWait(0x4000 + 17, 0);
  Sound_Play(MUS_011);
  GameFlag_Set(0x8f2);
  Sound_Play(SE_BOSS_ROAR);
  Actor_GetById(18)->velocityY = PIXELS(4);
  wait(5);
  Actor_GetById(19)->velocityY = PIXELS(4);
  wait(5);
  Actor_GetById(20)->velocityY = PIXELS(4);
  wait(5);
  Actor_GetById(21)->velocityY = PIXELS(4);
  wait(5);
  Actor_GetById(22)->velocityY = PIXELS(4);
  wait(5);
  Actor_GetById(23)->velocityY = PIXELS(4);
  wait(5);
  Actor_GetById(24)->velocityY = PIXELS(4);
  wait(5);
  Actor_GetById(25)->velocityY = PIXELS(4);
  wait(20);
  Actor_ArmCallback(17, FACING_NORTH + FACING_STEP, 20);
  Func_020032b0();
  Event_Wait(40);
  Camera_SetSpeedParameters(0x3333, 0x666);
  FieldCamera_MoveWithinMapBounds(PIXELS(264), 0, PIXELS(400), 1);
  FieldCamera_WaitForArrival();
  Event_Wait(200);
  Camera_SetSpeedParameters(0x4cccc, 0x9999);
  FieldCamera_MoveWithinMapBounds(PIXELS(111), PIXELS(32), PIXELS(456), 1);
  FieldCamera_WaitForArrival();
  Actor_ArmCallback(26, FACING_EAST, 0);
  Event_ShowMessageAndWait(26, 0);
  Actor_ArmCallback(gMainRoutineWork.currentOwner, FACING_WEST, 20);
  Motion_SetModeAndWaitAnimation(gMainRoutineWork.currentOwner, 3);
  Actor_SetModeById(26, 4);
  Event_ShowMessageAndWait(26, 0);
  Motion_SetModeAndWaitAnimation(gMainRoutineWork.currentOwner, 3);
  Motion_SetModeAndWaitAnimation(26, 3);
  Actor_SetModeById(26, 2);
  leader = Actor_GetById(gMainRoutineWork.currentOwner);
  if (leader != 0)
    Actor_SetDestinationById(26, ((s16 *)&leader->x)[1],
                             ((s16 *)&leader->z)[1]);
  Actor_WaitForMoveById(26);
  Actor_SetHorizontalPositionWithTerrain(26, 0, 0);
  GameFlag_Set(0x8f1);
}

void Func_02002eb0(void);

void Kibombo_PresentOrbToGabomba(void) {
  Actor_SetSpriteFlags(Actor_GetById(gMainRoutineWork.currentOwner), 0);
  Actor_SetSpriteColor(Actor_GetById(gMainRoutineWork.currentOwner), 15);
  Kibombo_CamMove(-1, -1, -1, 0);
  FieldCamera_GetActor()->motionFlags = 0;
  Event_StartScreenTransition();
  Event_WaitTransitionFrames();
  Kibombo_TaskAdd(Func_02002ccc, 0x480);
  Event_Wait(40);
  Sound_Play(SOUND_FADE_OUT_BGM);
  Kibombo_CamSpeed(0xcccc, 0x1999);
  Kibombo_CamMove(PIXELS(264), -1, PIXELS(388), 1);
  FieldCamera_WaitForArrival();
  Kibombo_ObjTurn(17, FACING_SOUTHEAST + FACING_STEP);
  Motion_SetModeAndWaitAnimation(17, 3);
  Event_SetMessageId((s32)MsgLetUsNow);
  Event_ShowMessageAndWait(0x4000 + 17, 0);
  Sound_Play(MUS_026);
  Actor_SetSpriteFlags(Actor_GetById(27), 0);
  Actor_GetById(27)->motionFlags = 0;
  Actor_SetPositionAndResetMotion(Actor_GetById(27), PIXELS(265), PIXELS(24),
                                  PIXELS(344));
  Camera_SetSpeedParameters(0x6666, 0xccc);
  FieldCamera_MoveWithinMapBounds(PIXELS(264), -1, PIXELS(340), 1);
  Kibombo_ObjSpeed(17, 0x3333, 0x1999);
  Actor_WalkToAndWaitById(17, 264, 374);
  wait(1);
  Kibombo_TaskAdd(Func_02002eb0, 0x480);
  Event_Wait(20);
  Task_Remove(Func_02002ccc);
  Func_020032e0();
  Kibombo_TaskAdd(Func_02002ccc, 0x480);
  Event_ShowMessageAndWait(17, 0);
  Sound_Play(MUS_011);
  Func_02003610();
  Event_Wait(160);
  Event_ShowMessageThenWait(17, 0, 80);
  Sound_Play(SE_576);
  Func_02003640(1, 1);
  Event_Wait(20);
  Event_ShowMessageThenWait(17, 0, 40);
  Task_Remove(Func_02002ccc);
  wait(1);
  ((struct KibomboEventWork *)Ram_HeapSlots->eventWork)->screenEffect = 0x202;
  Event_FinishScreenTransition();
  Event_WaitTransitionFrames();
  Import_EventSetExitRequest(10);
}

void Kibombo_GabombaNotPleased(void) {
  struct Actor **slot;
  struct Actor *object;
  s32 y;

  Actor_SetSpriteFlags(Actor_GetById(gMainRoutineWork.currentOwner), 0);
  Actor_SetSpriteColor(Actor_GetById(gMainRoutineWork.currentOwner), 15);
  Kibombo_CamMove(-1, -1, -1, 0);
  FieldCamera_GetActor()->motionFlags = 0;
  Actor_SetSpriteFlags(Actor_GetById(27), 0);
  Actor_GetById(27)->motionFlags = 0;
  Actor_SetPositionAndResetMotion(Actor_GetById(27), PIXELS(265), PIXELS(24),
                                  PIXELS(344));
  Func_02003640(1, 1);
  wait(1);
  if (gData_02007950 != 0) {
    gData_02007950->y += 0x200000;
    wait(1);
    gData_02007950->motionFlags = 4;
  }
  Event_StartScreenTransition();
  Event_WaitTransitionFrames();
  KIBOMBO_TASK_ADD(Func_02002ccc, 0x480);
  Event_Wait(40);
  Sound_Play(SE_RATTLE);
  Func_02003640(0, 1);
  Event_Wait(20);
  FieldEffect_SetAttached(17, ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  Event_Wait(20);
  Event_SetMessageId((s32)MsgYouAreStill);
  Event_ShowMessageAndWait(17, 0);
  slot = &gData_02007950;
  object = *slot;
  if (object != 0) {
    // Lowers the object 0x4000 a frame down to y 0x240000; the loop is
    // entered at its test, which reuses the pointer read for the test above.
    object->motionFlags = 0;
    goto test;
    do {
      Kibombo_ObjArm(17, FACING_NORTH + FACING_STEP, 0);
      slot = &gData_02007950;
      (*slot)->y += 0xffffc000;
      wait(1);
      object = *slot;
    test:
      y = object->y;
      /* FAKEMATCH(barrier): an empty asm on the height. */
      __asm__("" : "+l"(y));
    } while (y > 0x240000);
  }
  Motion_SetModeAndWaitAnimation(17, 4);
  Event_ShowMessageAndWait(17, 0);
  Kibombo_ObjTurn(17, FACING_SOUTHEAST + FACING_STEP);
  Event_ShowMessageAndWait(0x4000 + 17, 0);
  GameFlag_Clear(0x8f7);
  Sound_Play(MUS_011);
  Event_Wait(80);
  ((struct KibomboEventWork *)Ram_HeapSlots->eventWork)->screenEffect = 0x201;
  Event_FinishScreenTransition();
  Event_WaitTransitionFrames();
  Import_EventSetExitRequest(11);
}

void Kibombo_WitchDoctorFeelsPower(void) {
  struct KibomboEventWork *work;
  s32 effect;

  Event_Start();
  Event_ResolvePendingActions(0);
  Actor_SetSpriteColor(Actor_GetById(gMainRoutineWork.currentOwner), 15);
  Actor_SetSpriteFlags(Actor_GetById(gMainRoutineWork.currentOwner), 0);
  Func_020032b0();
  KIBOMBO_TASK_ADD(Func_02002ccc, 1152);
  work = Ram_HeapSlots->eventWork;
  work->screenEffect = effect = 513;
  Event_StartScreenTransition();
  Event_WaitTransitionFrames();
  Event_Wait(40);
  Event_SetMessageId((s32)MsgOhTheMagic);
  Event_ShowMessageAndWait(17, 0);
  Kibombo_ObjTurn(17, FACING_SOUTHEAST + FACING_STEP);
  Kibombo_MessagePut(0x4011, 0);
  Kibombo_ObjTurn(17, FACING_SOUTH + FACING_STEP);
  Event_ShowMessageAndWait(SPEAKER_WINDOW_ABOVE | 17, 0);
  Task_Remove(Func_02002ccc);
  Kibombo_ObjTurn(17, FACING_NORTH + FACING_STEP);
  BattleEffect_ApplyColorToSourceBuffer(0x10000, 0);
  BattleEffect_ApplyColorToTargetBuffer(0, 0);
  ((struct KibomboEventWork *)Ram_HeapSlots->eventWork)->screenEffect = effect;
  Event_FinishScreenTransition();
  Event_WaitTransitionFrames();
  GameFlag_Set(0x8fc);
  GameFlag_Set(FLAG_KEEP_PARTY_POSITION);
  Import_EventSetExitRequest(12);
}

void Func_02004524(void) {
  struct KibomboEventWork *work;

  Event_Start();
  Event_ResolvePendingActions(0);
  Kibombo_CamMove(-1, -1, -1, 0);
  FieldCamera_GetActor()->motionFlags = 0;
  Actor_SetSpriteColor(Actor_GetById(gMainRoutineWork.currentOwner), 15);
  Actor_SetSpriteFlags(Actor_GetById(gMainRoutineWork.currentOwner), 0);
  KIBOMBO_TASK_ADD(Func_02002ccc, 0x480);
  Func_02003640(1, 1);
  if (gData_02007950 != 0) {
    gData_02007950->y += 0x200000;
    wait(1);
    gData_02007950->motionFlags = 4;
  }
  wait(1);
  work = Ram_HeapSlots->eventWork;
  work->screenEffect = 0x201;
  Event_StartScreenTransition();
  Event_WaitTransitionFrames();
  Event_Wait(40);
}

extern s32 gData_020061a8[];

void Func_020045d0(void) {
  Camera_SetSpeedParameters(0x4cccc, 0x9999);
  Kibombo_CamMove(132 << 17, 0, 200 << 17, 1);
  FieldCamera_WaitForArrival();
  Event_Wait(10);
  Actor_Jump(18, 6, 0);
  Actor_Jump(19, 6, 0);
  Actor_Jump(20, 6, 0);
  Actor_Jump(21, 6, 0);
  Actor_Jump(22, 6, 0);
  Actor_Jump(23, 6, 0);
  Actor_Jump(24, 6, 0);
  Actor_Jump(25, 6, 40);
  Kibombo_ObjTurn(20, FACING_SOUTH + FACING_STEP);
  Actor_Jump(20, 2, 0);
  Event_ShowMessageAndWait(0x4000 + 20, 0);
  Kibombo_ObjTurn(23, FACING_SOUTHEAST + FACING_STEP);
  Actor_Jump(23, 2, 0);
  Event_ShowMessageAndWait(0x4000 + 23, 0);
  Actor_SetMovementTypeById(18, gData_020061a8);
  Actor_SetMovementTypeById(19, gData_020061a8);
  Actor_SetMovementTypeById(20, gData_020061a8);
  Actor_SetMovementTypeById(21, gData_020061a8);
  Actor_SetMovementTypeById(22, gData_020061a8);
  Actor_SetMovementTypeById(23, gData_020061a8);
  Actor_SetMovementTypeById(24, gData_020061a8);
  Actor_SetMovementTypeById(25, gData_020061a8);
  Event_Wait(40);
  Kibombo_ObjTurn(17, FACING_SOUTHEAST + FACING_STEP);
  Actor_Jump(17, 2, 10);
  Actor_Jump(17, 4, 10);
  Event_ShowMessageAndWait(17, 0);
  Actor_Stop(18);
  Actor_Stop(19);
  Actor_Stop(20);
  Actor_Stop(21);
  Actor_Stop(22);
  Actor_Stop(23);
  Actor_Stop(24);
  Actor_Stop(25);
  Event_Wait(40);
  Kibombo_ObjArm(18, FACING_NORTH + FACING_STEP, 0);
  Kibombo_ObjArm(19, FACING_NORTH + FACING_STEP, 0);
  Kibombo_ObjArm(20, FACING_NORTH + FACING_STEP, 0);
  Kibombo_ObjArm(21, FACING_NORTH + FACING_STEP, 0);
  Kibombo_ObjArm(22, FACING_NORTHWEST + FACING_STEP, 0);
  Kibombo_ObjArm(23, FACING_NORTHWEST + FACING_STEP, 0);
  Kibombo_ObjArm(24, FACING_NORTHWEST + FACING_STEP, 0);
  Kibombo_ObjArm(25, FACING_NORTHWEST + FACING_STEP, 0);
  Actor_ShowEmoteById(17, EMOTE_IN_FRONT | EMOTE_SCOWL, 0);
  Kibombo_MessagePut(0x8000 + 17, 0);
  Kibombo_MessagePut(0x8000 + 17, 0);
  Motion_SetModeAndWaitAnimation(17, 3);
  Kibombo_MessagePut(0x8000 + 17, 0);
  Actor_Jump(17, 4, 10);
  Event_ShowMessageAndWait(0x8000 + 17, 0);
}

void Kibombo_GabombaStillNotPleased(void) {
  struct Actor **slot;
  struct Actor *object;

  Func_02004524();
  Sound_Play(SE_RATTLE);
  Func_02003640(0, 1);
  Event_Wait(20);
  Event_SetMessageId((s32)MsgYouAreStillNot);
  Event_ShowMessageAndWait(17, 0);
  object = gData_02007950;
  if (object != 0) {
    // Lowers the object 0x4000 a frame down to y 0x240000.
    object->motionFlags = 0;
    if (object->y > 144 << 14) {
      do {
        Actor_ArmCallback(17, FACING_NORTH + FACING_STEP, 0);
        slot = &gData_02007950;
        object = *slot;
        object->y += 0xffffc000;
        wait(1);
      } while ((*slot)->y > 0x240000);
    }
  }
  Motion_SetModeAndWaitAnimation(17, 4);
  Event_ShowMessageAndWait(17, 0);
  Kibombo_ObjTurn(17, FACING_SOUTHEAST + FACING_STEP);
  Event_ShowMessageAndWait(SPEAKER_WINDOW_ABOVE | 17, 0);
  GameFlag_Set(0x8fc);
  GameFlag_Set(FLAG_KEEP_PARTY_POSITION);
  Import_EventSetExitRequest(12);
}

void Kibombo_GabombaSeesOrb(void) {
  Func_02004524();
  Sound_Play(SE_576);
  Actor_SetSpriteColor(Actor_GetById(12), 7);
  Actor_SetSpriteColor(Actor_GetById(13), 7);
  wait(8);
  Actor_SetSpriteColor(Actor_GetById(12), 0);
  Actor_SetSpriteColor(Actor_GetById(13), 0);
  wait(8);
  Actor_ShowEmoteById(17, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 20);
  Event_SetMessageId((s32)MsgLookLookThe);
  Event_ShowMessageAndWait(17, 0);
  Sound_Play(SE_577);
  Actor_SetMovementTypeById(12, gData_0200614c);
  Actor_SetMovementTypeAndWaitById(13, gData_0200614c);
  Actor_SetMovementTypeById(12, gData_0200613c);
  Actor_SetMovementTypeById(13, gData_0200613c);
  Actor_SetSpriteColor(Actor_GetById(12), 12);
  Actor_SetSpriteColor(Actor_GetById(13), 12);
  Event_Wait(120);
  Func_020045d0();
  GameFlag_Set(0x8fc);
  GameFlag_Set(FLAG_KEEP_PARTY_POSITION);
  Import_EventSetExitRequest(12);
}

void Kibombo_GabombaResponds(void) {
  Func_02004524();
  Kibombo_SePlay(0x240);
  Actor_SetSpriteColor(Actor_GetById(12), 7);
  Actor_SetSpriteColor(Actor_GetById(13), 7);
  wait(8);
  Actor_GetById(12)->update = (void (*)(void))Func_020000a0;
  Actor_GetById(13)->update = (void (*)(void))Func_020000a0;
  Actor_ShowEmoteById(17, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 20);
  Event_SetMessageId((s32)MsgLookTheGreat);
  Event_ShowMessageThenWait(17, 0, 20);
  Sound_Play(SE_576);
  Func_020036e0(1, 0);
  Event_Wait(20);
  Sound_Play(SE_162);
  Task_Add(Func_02002f34, 0x480);
  Event_Wait(120);
  Func_020045d0();
  GameFlag_Set(0x8fc);
  GameFlag_Set(FLAG_KEEP_PARTY_POSITION);
  Import_EventSetExitRequest(12);
}

void Kibombo_GabombaRespondsToGift(void) {
  struct Actor *object;
  u32 i;

  object = Actor_GetById(14);
  Func_02004524();
  Actor_SetSpritePriority(17, 1);
  Kibombo_SePlay(0x240);
  Actor_SetSpriteColor(Actor_GetById(12), 7);
  Actor_SetSpriteColor(Actor_GetById(13), 7);
  wait(8);
  Actor_SetSpriteColor(Actor_GetById(12), 0);
  Actor_SetSpriteColor(Actor_GetById(13), 0);
  wait(8);
  Actor_ShowEmoteById(17, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 20);
  Event_SetMessageId((s32)MsgLookTheGreatGabomba);
  Event_ShowMessageThenWait(17, 0, 20);
  Kibombo_SePlay(0x240);
  Func_020036e0(1, 0);
  Event_Wait(20);
  Sound_Play(SE_CART_ROLL);
  object->scaleY = 0x1999;
  Actor_SetPositionAndResetMotion(Actor_GetById(14), 0x1080000, 0x180000,
                                  0x1340000);
  wait(1);
  Actor_SetModeById(14, 3);
  for (i = 0; i < 64; i++) {
    object->scaleY += 0x400;
    object->y += 0x2000;
    wait(1);
  }
  Sound_Play(SE_298);
  if (gData_02007950 != 0) {
    struct Actor *gift = gData_02007950;

    gift->motionFlags = 0;
    if (gift->y > 0x240000) {
    lower:
      gData_02007950->y -= 0x8000;
      wait(1);
      if (gData_02007950->y > 0x240000)
        goto lower;
    }
  }
  Sound_Play(SE_CRUNCH);
  if (gData_02007950 != 0) {
    gData_02007950->motionFlags = 0;
    i = 0;
  rise:
    gData_02007950->y += 0x60000;
    object->scaleY -= 0x2000;
    object->y -= 0x8000;
    wait(1);
    i++;
    if (i < 4)
      goto rise;
    Actor_SetPositionAndResetMotion(gData_02007950, 0, 0, 0);
    for (i = 0; i < 4; i++) {
      object->scaleY -= 0x2000;
      object->y -= 0x8000;
      wait(1);
    }
  }
  Actor_SetHorizontalPositionWithTerrain(14, 0, 0);
  Func_020036e0(0, 0);
  Func_02003640(1, 1);
  Event_Wait(10);
  Func_02003640(0, 1);
  Event_Wait(40);
  Kibombo_SePlay(0x240);
  Func_02003640(1, 1);
  Func_020036e0(1, 0);
  Sound_Play(SE_WATER);
  Actor_SetPositionAndResetMotion(Actor_GetById(14), 0x1080000, 0x180000,
                                  0x1340000);
  wait(1);
  for (i = 0; i < 8; i++) {
    object->scaleY += 0x2000;
    object->y += 0x8000;
    wait(1);
  }
  FieldEffect_SetAttached(17, ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  Event_Wait(40);
  Kibombo_ObjTurn(17, 0x3000);
  Kibombo_MessagePut(0x4011, 0);
  Kibombo_ObjEmote(17, 264, 20);
  Kibombo_MessagePut(0x4011, 0);
  Motion_SetModeAndWaitAnimation(17, 3);
  Kibombo_MessagePut(0x4011, 0);
  Kibombo_ObjSpeed(17, 0xcccc, 0x6666);
  Kibombo_ObjWalk(17, 264, 320);
  Actor_SetHorizontalPositionWithTerrain(17, 0, 0);
  ((struct KibomboEventWork *)Ram_HeapSlots->eventWork)->screenEffect = 0x203;
  Event_FinishScreenTransition();
  Event_WaitTransitionFrames();
  GameFlag_Set(0x8fb);
  Import_EventSetExitRequest(13);
}

void Kibombo_CeremonyEnds(void) {
  struct KibomboEventWork *work;
  struct Actor *leader;

  Event_Start();
  Event_ResolvePendingActions(0);
  Kibombo_CamMove(-1, -1, -1, 0);
  FieldCamera_MoveWithinMapBounds(PIXELS(384), -1, PIXELS(601), 0);
  wait(1);
  Actor_SetHorizontalPositionAndAngleWithTerrain(
      gMainRoutineWork.currentOwner, PIXELS(370), PIXELS(636), FACING_NORTH);
  Map_CenterOnFollowedPoint();
  wait(1);
  ((struct KibomboEventWork *)Ram_HeapSlots->eventWork)->screenEffect = 0x100;
  BattleEffect_ApplyColorToSourceBuffer(0, 0);
  BattleEffect_ApplyColorToTargetBuffer(0x10002, 0);
  ((struct KibomboEventWork *)Ram_HeapSlots->eventWork)->field1b4 = 60;
  Event_StartScreenTransition();
  Event_WaitTransitionFrames();
  Event_Wait(40);
  Kibombo_ObjArm(10, FACING_NORTHWEST + FACING_STEP, 20);
  Kibombo_ObjArm(10, FACING_NORTH + FACING_STEP, 20);
  Motion_SetModeAndWaitAnimation(10, 3);
  Actor_SetModeById(9, 3);
  Motion_SetModeAndWaitAnimation(8, 3);
  Kibombo_ObjArm(10, FACING_NORTHWEST + FACING_STEP, 20);
  Kibombo_ObjArm(10, FACING_NORTH + FACING_STEP, 20);
  Actor_GetById(10)->velocityY = PIXELS(3);
  Event_Wait(40);
  Actor_SetModeById(9, 3);
  Motion_SetModeAndWaitAnimation(8, 3);
  BattleEffect_ApplyColorToTargetBuffer(0x10000, 0);
  PaletteFade_StartInterpolation(40);
  wait(60);
  Motion_SetModeAndWaitAnimation(10, 3);
  Event_SetMessageId((s32)MsgRightWellI);
  Event_ShowMessageAndWait(10, 0);
  Motion_SetModeAndWaitAnimation(9, 3);
  Event_ShowMessageAndWait(9, 0);
  Actor_ArmCallback(10, FACING_EAST, 0);
  Kibombo_ObjArm(gMainRoutineWork.currentOwner, FACING_WEST, 40);
  Kibombo_ObjArm(10, FACING_NORTH + FACING_STEP, 0);
  Actor_ArmCallback(gMainRoutineWork.currentOwner, FACING_NORTH, 10);
  Actor_SetModeById(10, 3);
  Motion_SetModeAndWaitAnimation(gMainRoutineWork.currentOwner, 3);
  Actor_Jump(8, 4, 10);
  Kibombo_MessagePut(0x2008, 0);
  Actor_FaceDirectionById(9, FACING_EAST);
  Actor_ShowEmoteById(8, EMOTE_IN_FRONT | EMOTE_ANGER_VEIN, 20);
  Kibombo_MessagePut(0x2008, 0);
  Actor_ShowEmoteById(9, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 20);
  Event_ShowMessageAndWait(9, 0);
  Kibombo_ObjTurn(8, FACING_WEST);
  Actor_SetModeById(8, 4);
  Kibombo_MessagePut(0x2008, 0);
  Kibombo_ObjTurn(8, FACING_SOUTH + FACING_STEP);
  Kibombo_MessagePut(0x2008, 0);
  Actor_SetModeById(8, 4);
  Event_ShowMessageAndWait(SPEAKER_PORTRAIT_RIGHT | 8, 0);
  Kibombo_ObjArm(9, FACING_NORTH + FACING_STEP, 0);
  Kibombo_ObjSpeed(8, 0x19999, 0xcccc);
  Kibombo_ObjWalk(8, 364, 576);
  Kibombo_ObjArm(8, FACING_NORTHWEST + FACING_STEP, 0);
  Actor_ShowEmoteById(9, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 80);
  Event_ShowMessageAndWait(9, 0);
  Kibombo_ObjTurn(9, FACING_SOUTHEAST + FACING_STEP);
  Event_ShowMessageAndWait(9, 0);
  Actor_SetModeById(9, 4);
  Event_ShowMessageAndWait(9, 0);
  Event_ShowMessageAndWait(9, 0);
  Motion_SetModeAndWaitAnimation(10, 3);
  Event_ShowMessageAndWait(10, 0);
  Actor_FaceDirectionById(10, FACING_EAST);
  Event_ShowMessageAndWait(10, 0);
  Actor_FaceDirectionById(gMainRoutineWork.currentOwner, FACING_WEST);
  Actor_SetModeById(10, 3);
  Event_ShowMessageAndWait(10, 0);
  Motion_SetModeAndWaitAnimation(gMainRoutineWork.currentOwner, 3);
  Kibombo_ObjSpeed(10, 0xcccc, 0x6666);
  Actor_SetModeById(10, 2);
  leader = Actor_GetById(gMainRoutineWork.currentOwner);
  if (leader != 0)
    Actor_SetDestinationById(10, ((s16 *)&leader->x)[1],
                             ((s16 *)&leader->z)[1]);
  Actor_WaitForMoveById(10);
  Actor_SetHorizontalPositionWithTerrain(10, 0, 0);
  work = Ram_HeapSlots->eventWork;
  work->field1b4 = 16;
  work->screenEffect = 0x209;
  GameFlag_Set(0x8ef);
  Event_End();
}

s32 Scene_Initialize(void) {
  s16 scene = gMainRoutineWork.scene;

  if (scene == (s32)&SceneId_Kibombo1)
    Kibombo_InitializeRoom1();
  else if (scene == (s32)&SceneId_Kibombo4)
    Kibombo_InitializeRoom4();
  else if (scene == (s32)&SceneId_Kibombo2)
    Kibombo_InitializeRoom2();
  else if (scene == (s32)&SceneId_Kibombo5)
    Kibombo_InitializeRoom5();
  else if (scene == (s32)&SceneId_Kibombo6)
    Kibombo_InitializeRoom6();
  else if (scene == (s32)&SceneId_Kibombo3)
    Kibombo_InitializeRoom3();
  return 0;
}

s32 Scene_PrepareMap(void) { return 0; }

void Func_020050ac(void) {
  Actor_SetSpriteFlags(Actor_GetById(9), 0);
  Actor_SetSpriteFlags(Actor_GetById(10), 0);
  FieldEffect_StartBlankOverlay(0);
  FlagBlock_PlaceAll(gData_02006324);
  Actor_SetSpritePriority(8, 3);
  Rope_Initialize();
  Rope_Place(0, 0x9e9, 12, 13);
  Actor_GetById(22)->collisionFlags |= 0x80;
  Actor_GetById(23)->collisionFlags |= 0x80;
}

void Kibombo_InitializeRoom1(void) {
  Actor_SetHorizontalPositionWithTerrain(11, PIXELS(88), PIXELS(314));
  Actor_SetSpriteFlags(Actor_GetById(11), 0);
  Actor_GetById(11)->scaleY = 0x8000;
  Func_020050ac();
  GameFlag_Clear(0x86b);
  Actor_DisableById(25);
  Actor_SetModeById(25, 2);
  KIBOMBO_TASK_ADD(Func_02003030, 0x480)
  Func_02003184();
}

// Kibombo's setup.
#include "system/main_routine_work.h"
#include "field/field.h"

void *Unit_Get(s32 owner);

static __inline__ s32 Gauge(s32 value, s32 max) {
  s32 gauge = (value << 14) / max;

  s32 result;

  if (gauge > 0x4000)
    result = 0x4000;
  else {
    result = 0;
    if (gauge >= 0)
      result = gauge;
  }
  return result;
}

// Unless flag 265 is set, places actor 11 and, arriving by entrance 19 with
// unit 7 in the party (flag 7), restores unit 7's HP and PP, sets their
// gauges, clears two of its bytes, sets flag 0x92f, clears 0x92e and removes
// it from the party; then saves four palette colours, frees actors 30 to 35,
// sets flags 0x202 and 0x209, places actor 29, places actors 21 and 26 to 28
// when flag 0x8fd is set, sets flag 0x86b and fades in with Func_02002c34
// scheduled.
void Kibombo_InitializeRoom4(void) {
  struct Actor *actor = Actor_GetById(11);
  u8 *unit;
  s32 done;

  Event_Start();
  Event_ResolvePendingActions(0);
  done = GameFlag_Get(FLAG_KEEP_PARTY_POSITION);
  if (!done) {
    actor->motionFlags = done;
    actor->y = PIXELS(42);
    actor->z += PIXELS(-42);
    Actor_SetSpriteFlags(Actor_GetById(11), 0);
    Actor_SetSpritePriority(11, 1);
    if (gMainRoutineWork.entrance == 19 && GameFlag_Get(7)) {
      unit = Unit_Get(7);
      *(u16 *)(unit + 0x38) = *(u16 *)(unit + 0x34);
      *(u16 *)(unit + 0x3a) = *(u16 *)(unit + 0x36);
      /* FAKEMATCH(dead): both copies stored before the gauge's argument is
       * extended. */
      do {
      } while (0);
      *(u16 *)(unit + 0x14) =
          Gauge(*(s16 *)(unit + 0x38), *(s16 *)(unit + 0x34));
      if (*(u16 *)(unit + 0x14) == 0 && *(s16 *)(unit + 0x38) != 0) {
        s32 one = 1;

        *(s16 *)(unit + 0x14) = one;
      }
      *(u16 *)(unit + 0x16) =
          Gauge(*(s16 *)(unit + 0x3a), *(s16 *)(unit + 0x36));
      if (*(u16 *)(unit + 0x16) == 0 && *(s16 *)(unit + 0x3a) != 0) {
        s32 one = 1;

        *(s16 *)(unit + 0x16) = one;
      }
      unit[0x131] = 0;
      unit[0x140] = 0;
      GameFlag_Set(0x92f);
      GameFlag_Clear(0x92e);
      Party_RemoveMemberRestored(7);
    }
  }
  GameFlag_Set(FLAG_SCENE_TEMP_02);
  gData_02007948[0] = *(u16 *)&BG_PLTT_COLOR(0, 9);
  gData_02007948[1] = *(u16 *)&BG_PLTT_COLOR(5, 7);
  gData_02007948[2] = *(u16 *)&BG_PLTT_COLOR(6, 7);
  gData_02007948[3] = *(u16 *)&BG_PLTT_COLOR(7, 7);
  Func_020050ac();
  Actor_SetSpriteFlags(Actor_GetById(30), 0);
  Actor_SetSpriteFlags(Actor_GetById(31), 0);
  Actor_SetSpriteFlags(Actor_GetById(32), 0);
  Actor_SetSpriteFlags(Actor_GetById(33), 0);
  Actor_SetSpriteFlags(Actor_GetById(34), 0);
  Actor_SetSpriteFlags(Actor_GetById(35), 0);
  GameFlag_Set(FLAG_SCENE_TEMP_09);
  actor = Actor_GetById(29);
  actor->motionFlags = 0;
  actor->y = 0;
  Actor_Hide(29);
  Actor_SetSpritePriority(30, 3);
  if (GameFlag_Get(0x8fd)) {
    Actor_SetHorizontalPositionWithTerrain(26, 0, 0);
    Actor_SetHorizontalPositionWithTerrain(27, 0, 0);
    Actor_SetHorizontalPositionWithTerrain(28, 0, 0);
    Actor_SetPosition(21, 302, 172);
  }
  GameFlag_Set(0x86b);
  Event_StartScreenTransition();
  Event_WaitTransitionFrames();
  {
    s32 priority = 0x480;

    Task_Add(Func_02002c34, priority);
  }
  Event_End();
}

void Func_02002de0(void);

void Func_0200538c(void) {
  Actor_SetSpriteFlags(Actor_GetById(9), 0);
  Actor_SetSpriteFlags(Actor_GetById(10), 0);
  if (!GameFlag_Get(0x8fb))
    Func_02002de0();
  Func_0200324c();
  if (!GameFlag_Get(0x869))
    GameFlag_Set(FLAG_SCENE_TEMP_01);
  FlagBlock_PlaceAll(gData_02006324);
  Actor_SetSpritePriority(8, 2);
  Actor_SetSpriteFlags(Actor_GetById(11), 0);
  Actor_GetById(11)->motionFlags = 0;
  if (GameFlag_Get(0x9ea)) {
    Kibombo_ObjSetPos(Actor_GetById(11), 208 << 15, 0, 148 << 17);
    Kibombo_BlendStart(5, 18, 1, 1, 6, 18);
    wait(10);
  }
  Actor_GetById(gMainRoutineWork.currentOwner)->priorityFlags |= 0x20;
  if (GameFlag_Get(0x86a))
    Kibombo_UncoverVent();
  if (GameFlag_Get(FLAG_SCENE_TEMP_04))
    Func_02000624();
  if (GameFlag_Get(FLAG_SCENE_TEMP_05))
    Func_02000668();
  if (GameFlag_Get(FLAG_SCENE_TEMP_06))
    Func_020006ac();
  if (GameFlag_Get(FLAG_SCENE_TEMP_07))
    Func_020006f0();
  Actor_DisableById(12);
  Actor_DisableById(13);
  Actor_DisableById(14);
  Actor_SetSpritePriority(12, 1);
  Actor_SetSpritePriority(13, 1);
  Actor_SetSpritePriority(14, 1);
}

void Kibombo_InitializeRoom2(void) {
  s16 entrance;

  Func_0200538c();
  if (GameFlag_Get(0x8fb)) {
    Func_02003640(1, 0);
    Func_020036e0(1, 5);
  }
  wait(1);
  entrance = gMainRoutineWork.entrance;
  if (entrance == 99) {
    Func_02003640(0, 0);
    Func_020036e0(0, 0);
    Actor_SetPositionAndResetMotion(gData_02007950, 0, 0, 0);
  } else if (entrance == 98) {
    Func_02003640(1, 0);
    Func_020036e0(1, 0);
    Actor_SetPositionAndResetMotion(gData_02007950, 0, 0, 0);
  } else if (entrance == 97) {
    Func_02003640(1, 0);
    Func_020036e0(1, 2);
    Actor_SetPositionAndResetMotion(gData_02007950, 0, 0, 0);
  } else if (entrance == 96) {
    Func_02003640(1, 1);
    Func_020036e0(1, 4);
    Actor_SetPositionAndResetMotion(gData_02007950, 0, 0, 0);
    Actor_SetModeById(14, 4);
  }
}

void Kibombo_InitializeRoom5(void) {
  u32 i;

  Event_Start();
  Event_ResolvePendingActions(0);
  GameFlag_Set(0x869);
  GameFlag_Set(FLAG_SCENE_TEMP_02);
  gData_02007948[0] = BG_PLTT[9];
  gData_02007948[1] = BG_PLTT[7];
  gData_02007948[2] = BG_PLTT[6];
  gData_02007948[3] = BG_PLTT[76];
  Func_0200538c();
  if (GameFlag_Get(0x8fb)) {
    Func_02003640(1, 1);
    Func_020036e0(1, 4);
  }
  wait(1);
  switch (gMainRoutineWork.entrance) {
  case 10:
    Kibombo_PresentOrbToGabomba();
    return;
  case 11:
    Kibombo_GabombaNotPleased();
    return;
  case 12:
    Kibombo_WitchDoctorFeelsPower();
    return;
  case 13:
    Kibombo_GabombaStillNotPleased();
    return;
  case 14:
    Kibombo_GabombaSeesOrb();
    return;
  case 15:
    Kibombo_GabombaResponds();
    return;
  case 16:
    Kibombo_GabombaRespondsToGift();
    return;
  }
  Actor_SetSpritePriority(17, 1);
  Actor_SetSpritePriority(7, 2);
  Actor_SetSpritePriority(26, 2);
  Actor_SetSpritePriority(5, 2);
  Actor_SetSpritePriority(6, 2);
  if (GameFlag_Get(0x92e))
    Actor_SetHorizontalPositionWithTerrain(7, 0, 0);
  else if (GameFlag_Get(0x92f)) {
    s32 angle = 0x2000;

    Kibombo_ObjPlaceFacing(7, 152 << 17, 176 << 15, angle);
  }
  if (GameFlag_Get(0x8fd)) {
    Func_020036e0(1, 6);
    Actor_SetHorizontalPositionWithTerrain(17, 0, 0);
  }
  Event_StartScreenTransition();
  Event_WaitTransitionFrames();
  Task_Add(Func_02002ccc, 0x480);
  for (i = 0; i <= 7; i++) {
    u8 *field = &Actor_GetById(i + 18)->unk5c[0];
    s32 value = 2;

    *field = value;
  }
  if (GameFlag_Get(0x8f1)) {
    if (!GameFlag_Get(0x8fd))
      Func_020032b0();
  } else if (gMainRoutineWork.entrance == 5) {
    Kibombo_CeremonyBegins();
  }
  Event_End();
  if (gMainRoutineWork.entrance == 99) {
    Func_02003640(0, 0);
    Func_020036e0(0, 0);
    Actor_SetPositionAndResetMotion(gData_02007950, 0, 0, 0);
  } else if (gMainRoutineWork.entrance == 98) {
    Func_02003640(1, 1);
    Func_020036e0(1, 0);
    Actor_SetPositionAndResetMotion(gData_02007950, 0, 0, 0);
  } else if (gMainRoutineWork.entrance == 97) {
    Func_02003640(1, 1);
    Func_020036e0(1, 4);
    Actor_SetPositionAndResetMotion(gData_02007950, 0, 0, 0);
  } else if (gMainRoutineWork.entrance == 96) {
    Func_02003640(1, 1);
    Func_020036e0(1, 4);
    Actor_SetPositionAndResetMotion(gData_02007950, 0, 0, 0);
    Actor_SetModeById(14, 5);
  }
}

#define PALETTE_WORD(index) (*(u16 *)(0x05000000 + (index) * 2))

void Kibombo_InitializeRoom6(void) {
  Event_Start();
  Event_ResolvePendingActions(0);
  GameFlag_Set(FLAG_SCENE_TEMP_02);
  gData_02007948[0] = PALETTE_WORD(169);
  gData_02007948[1] = PALETTE_WORD(167);
  gData_02007948[2] = PALETTE_WORD(166);
  if (gMainRoutineWork.entrance == 10) {
    Actor_SetSpriteFlags(Actor_GetById(gMainRoutineWork.currentOwner), 0);
    Actor_SetSpriteColor(Actor_GetById(gMainRoutineWork.currentOwner), 15);
    Func_02003c84();
  } else if (gMainRoutineWork.entrance == 11) {
    Actor_SetSpriteFlags(Actor_GetById(gMainRoutineWork.currentOwner), 0);
    Actor_SetSpriteColor(Actor_GetById(gMainRoutineWork.currentOwner), 15);
    Func_02003c14();
  } else {
    Event_StartScreenTransition();
    Event_WaitTransitionFrames();
    KIBOMBO_TASK_ADD(Func_02002d64, 1152);
    Event_End();
  }
}

#include "version.h"
#include "fixed_math.h"

void Actor_FallOverById(s32 actor, s32 angle);

void Kibombo_InitializeRoom3(void) {
  struct Actor *object;

  ((struct KibomboEventWork *)Ram_HeapSlots->eventWork)->screenEffect = 0x209;
  Func_02003288();
  if (gMainRoutineWork.entrance == 9) {
    if (GameFlag_Get(0x8ef)) {
      Actor_SetHorizontalPositionWithTerrain(10, 0, 0);
      if (GameFlag_Get(0x8ee)) {
        s32 angle = FACING_NORTHWEST + FACING_STEP;

        Kibombo_ObjPlaceFacing(8, PIXELS(434), PIXELS(634), angle);
      } else {
        s32 angle = FACING_NORTHWEST + FACING_STEP;

        Kibombo_ObjPlaceFacing(8, PIXELS(364), PIXELS(576), angle);
      }
    } else {
      Kibombo_CeremonyEnds();
    }
  }
#if TLA_PATCH_1
  if (GameFlag_Get(FLAG_KEEP_PARTY_POSITION)) {
    object = Actor_GetById(16);
    object->x -= PIXELS(14);
    object->z -= PIXELS(8);
  }
#endif
  Actor_FallOverById(16, 0);
  object = Actor_GetById(16);
  ((struct FieldSprite *)object->sprite)->rotation = 0;
  object->scaleX = Q16_ONE;
  object->scaleY = Q16_ONE;
  object->x += PIXELS(14);
  object->z += PIXELS(8);
  Actor_SetModeById(16, 6);
  if (gMainRoutineWork.entrance == 14)
    GameFlag_Set(0x8ee);
}

void Func_02005994(void) { Field_TryJumpForward(); }

#include "text/msg_ids.h"
#include "objcmd.h"

void Kibombo_AskAboutFewerWarriors(void);
void Kibombo_AskAboutRevenge(void);
void Kibombo_TalkPathToPeace(void);
void Kibombo_AskGoingWest(void);
void Kibombo_OpenWeaponShop(void);
void Kibombo_OpenArmorShop(void);
void Kibombo_OpenItemShop(void);
void Kibombo_OpenInn(void);
void Kibombo_OpenSanctum(void);
void Kibombo_TalkForeignCeremony(void);
void Kibombo_TalkAchingBack(void);
void Kibombo_ReadMindNeverAgain(void);
void Func_020005d0(void);
void Func_02000734(void);
void Kibombo_MeetPiers(void);
void Kibombo_PiersThanksParty(void);
void Kibombo_PiersRefusesToLeave(void);
void Kibombo_DeadEndByTheStatue(void);
void Func_02002ad8(void);
void Func_02002af8(void);

// clang-format off
u32 gData_02005ca4[] = {
    0, 28, 22, 38, 0, 0,
    1, 3, 0, 1, 38,
};

s32 gData_02005cd0[] = {                                                               
      SC_SET(SW_MOTION, 0),                                                    
      SC_ADD(SW_Z, -0x8000),                                                   
      SC_ADD(SW_SCALE_X, -1024),                                               
      SC_ADD(SW_SCALE_Y, -1024),                                               
      SC_WAIT(1),                                                              
      SC_LOOP(24, SC_TOP),                                                     
      SC_END,                                                                  
  };

u32 gData_02005d18[] = {                                                               
      SC_ARG(2),   SC_MOVE(PIXELS(98), 0, PIXELS(408)),                         
      SC_SYNC,     SC_ARG(1),                                                  
      SC_WAIT(1), SC_SET(SW_FACING, FACING_NORTH + FACING_STEP),              
      SC_WAIT(1), SC_END,                                                     
  };

u32 gData_02005d5c[] = {                                                               
      SC_ARG(2),   SC_MOVE(PIXELS(80), 0, PIXELS(376)), SC_SYNC,     SC_ARG(1), 
      SC_WAIT(1), SC_SET(SW_FACING, FACING_EAST),     SC_WAIT(1), SC_END,    
  };

s32 gData_02005da0[] = {
    SC_ARG(2),
    SC_MOVE(PIXELS(68), 0, PIXELS(392)),
    SC_SYNC,
    SC_ARG(1),
    SC_WAIT(1),
    SC_SET(SW_FACING, FACING_NORTHEAST),
    SC_WAIT(1),
    SC_END,
    SC_ADD(SW_SCALE_X, 0x800),
    SC_ADD(SW_SCALE_Y, 0x800),
    SC_CALL(Func_02000038),
    SC_WAIT(1),
    SC_LOOP(10, SC_TOP),
    SC_LABEL(1),
    SC_ADD(SW_SCALE_X, -2048),
    SC_ADD(SW_SCALE_Y, -2048),
    SC_CALL(Func_02000038),
    SC_WAIT(1),
    SC_LOOP(24, SC_TO(1)),
    SC_FREE,
};

u32 gData_02005e54[] = {
    39, 2, 4, 0x580000, 0, 0x1880000,
    1, 39, 1, 3, 0, 1,
    17,
};

u32 gData_02005e88[] = {
    SC_ARG(2),
    SC_MOVE(PIXELS(72), 0, PIXELS(296)),
    SC_SYNC,
    SC_MOVE(PIXELS(40), 0, PIXELS(340)),
    SC_SYNC,
    SC_MOVE(PIXELS(40), 0, PIXELS(356)),
    SC_SYNC,
    SC_MOVE(PIXELS(56), 0, PIXELS(372)),
    SC_SYNC,
    SC_ARG(1),
    SC_END,
};

u32 gData_02005eec[] =                                                                 
      {                                                                        
          SC_ARG(2), SC_MOVE(PIXELS(88), 0, PIXELS(372)), SC_SYNC, SC_ARG(1),   
          SC_END,                                                              
  };

u32 gData_02005f14[] = {                                                               
      SC_ARG(2),   SC_MOVE(PIXELS(48), 0, PIXELS(384)), SC_SYNC,     SC_ARG(1), 
      SC_WAIT(1), SC_SET(SW_FACING, FACING_EAST),     SC_WAIT(1), SC_END,    
  };

u32 gData_02005f58[] = {                                                               
      SC_ARG(2),   SC_MOVE(PIXELS(48), 0, PIXELS(360)), SC_SYNC,     SC_ARG(1), 
      SC_WAIT(1), SC_SET(SW_FACING, FACING_EAST),     SC_WAIT(1), SC_END,    
  };

u32 gData_02005f9c[] = {                                                               
      SC_ARG(2),   SC_MOVE(PIXELS(36), 0, PIXELS(372)), SC_SYNC,     SC_ARG(1), 
      SC_WAIT(1), SC_SET(SW_FACING, FACING_EAST),     SC_WAIT(1), SC_END,    
  };

u32 gData_02005fe0[] = {
    39, 2, 4, 0x380000, 0, 0x1740000,
    1, 39, 1, 3, 0, 1,
    17,
};

u32 gData_02006014[] =                                                                 
      {                                                                        
          SC_ARG(2),   SC_MOVE(PIXELS(298), 0, PIXELS(200)),                     
          SC_SYNC,     SC_ARG(1),                                              
          SC_WAIT(1), SC_SET(SW_FACING, FACING_NORTHWEST),                    
          SC_WAIT(1), SC_END,                                                 
  };

u32 gData_02006058[] =                                                                 
      {                                                                        
          SC_ARG(2),   SC_MOVE(PIXELS(308), 0, PIXELS(188)),                     
          SC_SYNC,     SC_ARG(1),                                              
          SC_WAIT(1), SC_SET(SW_FACING, FACING_NORTHWEST),                    
          SC_WAIT(1), SC_END,                                                 
  };

u32 gData_0200609c[] =                                                                 
      {                                                                        
          SC_ARG(2),   SC_MOVE(PIXELS(298), 0, PIXELS(176)),                     
          SC_SYNC,     SC_ARG(1),                                              
          SC_WAIT(1), SC_SET(SW_FACING, FACING_NORTHWEST),                    
          SC_WAIT(1), SC_END,                                                 
  };

u32 gData_020060e0[] =                                                                 
      {                                                                        
          SC_ARG(2), SC_MOVE(PIXELS(222), 0, PIXELS(184)), SC_SYNC, SC_ARG(1),   
          SC_END,                                                              
  };

u32 gData_02006108[] = {
    39, 2, 4, 0x11c0000, 0, 0xbc0000,
    1, 39, 1, 3, 0, 1,
    17,
};

s32 gData_0200613c[] = {
    46,
    (s32)Func_02000048,
    46,
    (s32)Func_02000054,
};

s32 gData_0200614c[] = {
    SC_SET(SW_SCALE_X, 0x18000),
    SC_SET(SW_SCALE_Y, 0x18000),
    SC_ADD(SW_Y, PIXELS(-2)),
    SC_WAIT(20),
    SC_SET(SW_SCALE_X, 0x10000),
    SC_SET(SW_SCALE_Y, 0x10000),
    SC_ADD(SW_Y, PIXELS(2)),
    SC_WAIT(1),
    SC_END,
};

s32 gData_020061a8[] = {
    22,
    39,
    0,
    46,
    (s32)Func_020000c4,
    22,
    15,
    0x6666,
    22,
    16,
    0x6666,
    (s32)0x80010000,
    39,
    2,
    4,
    0xe60000,
    0,
    0x1e60000,
    1,
    39,
    1,
    0,
    20,
    22,
    3,
    0x5000,
    0,
    40,
    39,
    2,
    4,
    0x1000000,
    0,
    0x1e60000,
    1,
    39,
    1,
    22,
    3,
    0x3000,
    0,
    20,
    22,
    3,
    0x5000,
    0,
    40,
    13,
    0xffff,
    0,
    22,
    15,
    0x6666,
    22,
    16,
    0x6666,
    (s32)0x80010000,
    39,
    2,
    4,
    0x12a0000,
    0,
    0x1e60000,
    1,
    39,
    1,
    0,
    20,
    22,
    3,
    0x3000,
    0,
    40,
    39,
    2,
    4,
    0x1100000,
    0,
    0x1e60000,
    1,
    39,
    1,
    22,
    3,
    0x5000,
    0,
    20,
    22,
    3,
    0x3000,
    0,
    40,
    13,
    0xffff,
    0,
};

s16 gData_02006324[] = {
    8, 513, -1, 0,
};

struct SceneEntrance gSceneEntrances[] = {
    {0, CONDITION_ALWAYS, 172, 0, 149, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {SCENE_TABLE_END},
};

s16 gData_0200635c[] = {
    400, 40, 368, 416, 56, 384, -1, 14,
    112, 40, 624, 128, 56, 640, -1, 15,
    384, 40, 880, 400, 56, 896, -1, 16,
    -1, 0, 0, 0, 0, 0, 0, 0,
};

u32 gSceneExits[] = {
    SCENE_EXITS(Kibombo1),
    SCENE_EXIT(1, WorldMap, 27),
    SCENE_EXIT(2, Kibombo2, 1),
    SCENE_EXIT(3, Kibombo3, 1),
    SCENE_EXIT(4, Kibombo3, 6),
    SCENE_EXIT(5, Kibombo3, 5),
    SCENE_EXIT(6, Kibombo3, 4),
    SCENE_EXIT(7, Kibombo3, 8),
    SCENE_EXIT(8, Kibombo3, 10),
    SCENE_EXIT(9, Kibombo2, 2),
    SCENE_EXIT(10, Kibombo2, 5),
    SCENE_EXITS(Kibombo4),
    SCENE_EXIT(1, WorldMap, 27),
    SCENE_EXIT(2, Kibombo5, 1),
    SCENE_EXIT(3, Kibombo3, 1),
    SCENE_EXIT(4, Kibombo3, 6),
    SCENE_EXIT(5, Kibombo3, 5),
    SCENE_EXIT(6, Kibombo3, 4),
    SCENE_EXIT(7, Kibombo3, 8),
    SCENE_EXIT(8, Kibombo3, 10),
    SCENE_EXIT(9, Kibombo5, 2),
    SCENE_EXIT(10, Kibombo5, 5),
    SCENE_EXITS(Kibombo2),
    SCENE_EXIT(1, Kibombo1, 2),
    SCENE_EXIT(2, Kibombo1, 9),
    SCENE_EXIT(3, GabombaStatue7, 1),
    SCENE_EXIT(4, GabombaStatueChamber1, 3),
    SCENE_EXIT(5, Kibombo1, 10),
    SCENE_EXITS(Kibombo5),
    SCENE_EXIT(1, Kibombo4, 2),
    SCENE_EXIT(2, Kibombo4, 9),
    SCENE_EXIT(3, GabombaStatue7, 1),
    SCENE_EXIT(4, GabombaStatueChamber1, 3),
    SCENE_EXIT(5, Kibombo4, 10),
    SCENE_EXIT(10, GabombaStatue6, 2),
    SCENE_EXIT(11, GabombaStatue6, 3),
    SCENE_EXIT(12, GabombaStatue6, 1),
    SCENE_EXIT(13, GabombaStatueChamber1, 5),
    SCENE_EXITS(Kibombo3),
    SCENE_EXIT_UNLESS(1, Kibombo1, 3, 0x86b),
    SCENE_EXIT_UNLESS(1, Kibombo4, 3, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(2, Kibombo3, 3, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(3, Kibombo3, 2, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(4, Kibombo1, 6, 0x86b),
    SCENE_EXIT_UNLESS(4, Kibombo4, 6, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(5, Kibombo1, 5, 0x86b),
    SCENE_EXIT_UNLESS(5, Kibombo4, 5, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(6, Kibombo1, 4, 0x86b),
    SCENE_EXIT_UNLESS(6, Kibombo4, 4, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(7, Kibombo3, 15, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(8, Kibombo1, 7, 0x86b),
    SCENE_EXIT_UNLESS(8, Kibombo4, 7, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(9, Kibombo3, 14, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(10, Kibombo1, 8, 0x86b),
    SCENE_EXIT_UNLESS(10, Kibombo4, 8, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(11, Kibombo3, 12, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(12, Kibombo3, 11, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(13, Kibombo3, 16, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(14, Kibombo3, 9, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(15, Kibombo3, 7, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(16, Kibombo3, 13, EXIT_FLAG_NONE),
    SCENE_EXITS(Kibombo6),
    SCENE_EXIT(10, Kibombo5, 16),
    SCENE_EXIT(11, Kibombo5, 10),
    SCENE_EXITS_END,
};

struct ActorPlacement gKibombo_Placements[] = {
    {SCENE_TABLE_END},
};

struct ActorPlacement gKibombo_Room1Placements[] = {
    ACTOR_PLACEMENT(290, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 120, 0, 72, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(387, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 328, 0, 200, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(387, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 392, 0, 200, FACING_SOUTH, TALK_FACING_KEEP, 1),
    ACTOR_PLACEMENT(365, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 264, 0, 232, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(407, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 104, 0, 488, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(407, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 56, 0, 488, FACING_SOUTH, TALK_FACING_KEEP, 1),
    ACTOR_PLACEMENT(115, CONDITION_ALWAYS, MOVEMENT_TYPE_WANDER_NEAR, 300, 0, 228, FACING_WEST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(116, CONDITION_ALWAYS, MOVEMENT_TYPE_WANDER_NEAR, 248, 0, 288, FACING_NORTH + FACING_STEP, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(114, CONDITION_ALWAYS, MOVEMENT_TYPE_WANDER_NEAR, 208, 0, 176, FACING_EAST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(128, CONDITION_ALWAYS, MOVEMENT_TYPE_WANDER_NEAR, 288, 0, 364, FACING_WEST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(139, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 432, 0, 274, FACING_NORTHWEST + FACING_STEP, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(140, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 58, 0, 298, FACING_SOUTHEAST + FACING_STEP, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(146, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 102, 0, 328, FACING_NORTHWEST + FACING_STEP, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(147, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 88, 0, 336, FACING_NORTH + FACING_STEP, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(83, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 246, 0, 488, FACING_SOUTHEAST + FACING_STEP, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(83, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 282, 0, 488, FACING_SOUTH + FACING_STEP, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(144, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 430, 0, 442, FACING_SOUTHEAST + FACING_STEP, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(368, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 440, 0, 460, FACING_SOUTHEAST + FACING_STEP, TALK_FACING_KEEP, 0),
    {SCENE_TABLE_END},
};

struct ActorPlacement gKibombo_Room4Placements[] = {
    ACTOR_PLACEMENT(290, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 120, 0, 72, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(386, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 328, 0, 200, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(386, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 392, 0, 200, FACING_SOUTH, TALK_FACING_KEEP, 1),
    ACTOR_PLACEMENT(365, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 264, 0, 232, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(407, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 104, 0, 488, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(407, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 56, 0, 488, FACING_SOUTH, TALK_FACING_KEEP, 1),
    ACTOR_PLACEMENT(144, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 264, 0, 104, FACING_NORTH + FACING_STEP, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(141, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 296, 0, 114, FACING_NORTH + FACING_STEP, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(139, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 208, 0, 152, FACING_NORTH + FACING_STEP, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(145, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 292, 0, 128, FACING_NORTHWEST + FACING_STEP, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(145, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 230, 0, 112, FACING_NORTH + FACING_STEP, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(140, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 304, 0, 220, FACING_NORTHWEST + FACING_STEP, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(146, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 264, 0, 278, FACING_NORTH + FACING_STEP, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(147, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 320, 0, 158, FACING_NORTHWEST + FACING_STEP, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(145, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 234, 0, 132, FACING_NORTH + FACING_STEP, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(139, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 326, 0, 88, FACING_NORTH + FACING_STEP, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(140, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 72, 0, 308, FACING_NORTH + FACING_STEP, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(141, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 278, 0, 146, FACING_NORTHWEST + FACING_STEP, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(143, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 264, 0, 138, FACING_NORTH + FACING_STEP, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(144, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 196, 0, 142, FACING_NORTH + FACING_STEP, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(144, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 234, 0, 224, FACING_NORTH + FACING_STEP, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(337, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 120, 0, 168, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(386, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 120, 0, 181, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(386, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 360, 0, 194, FACING_SOUTH, TALK_FACING_KEEP, 1),
    ACTOR_PLACEMENT(386, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 440, 0, 242, FACING_SOUTH, TALK_FACING_KEEP, 1),
    ACTOR_PLACEMENT(386, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 104, 0, 274, FACING_SOUTH, TALK_FACING_KEEP, 1),
    ACTOR_PLACEMENT(386, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 184, 0, 338, FACING_SOUTH, TALK_FACING_KEEP, 1),
    ACTOR_PLACEMENT(386, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 328, 0, 370, FACING_SOUTH, TALK_FACING_KEEP, 1),
    ACTOR_PLACEMENT(201, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 412, 0, 60, FACING_NORTHWEST + FACING_STEP, TALK_FACING_KEEP, 0),
    {SCENE_TABLE_END},
};

struct ActorPlacement gKibombo_Room2Placements[] = {
    ACTOR_PLACEMENT(290, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 120, 0, 504, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(386, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 232, 0, 392, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(386, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 296, 0, 392, FACING_SOUTH, TALK_FACING_KEEP, 1),
    ACTOR_PLACEMENT(272, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 120, 0, 296, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(395, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(395, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 1),
    ACTOR_PLACEMENT(396, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(337, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 72, 0, 424, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(337, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 328, 0, 184, FACING_SOUTH, TALK_FACING_KEEP, 1),
    {SCENE_TABLE_END},
};

struct ActorPlacement gKibombo_Room5Placements[] = {
    ACTOR_PLACEMENT(290, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 120, 0, 504, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(386, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 232, 0, 392, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(386, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 296, 0, 392, FACING_SOUTH, TALK_FACING_KEEP, 1),
    ACTOR_PLACEMENT(272, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 120, 0, 296, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(395, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(395, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 1),
    ACTOR_PLACEMENT(396, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(337, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 72, 0, 424, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(337, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 328, 0, 184, FACING_SOUTH, TALK_FACING_KEEP, 1),
    ACTOR_PLACEMENT(26, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 264, 0, 392, FACING_NORTH + FACING_STEP, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(83, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 218, 0, 420, FACING_NORTH + FACING_STEP, TALK_FACING_PARTY_THEN_WATCH, 0),
    ACTOR_PLACEMENT(83, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 232, 0, 432, FACING_NORTH + FACING_STEP, TALK_FACING_PARTY_THEN_WATCH, 0),
    ACTOR_PLACEMENT(83, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 248, 0, 438, FACING_NORTH + FACING_STEP, TALK_FACING_PARTY_THEN_WATCH, 0),
    ACTOR_PLACEMENT(83, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 264, 0, 440, FACING_NORTH + FACING_STEP, TALK_FACING_PARTY_THEN_WATCH, 0),
    ACTOR_PLACEMENT(83, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 280, 0, 438, FACING_NORTHWEST + FACING_STEP, TALK_FACING_PARTY_THEN_WATCH, 0),
    ACTOR_PLACEMENT(83, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 296, 0, 432, FACING_NORTHWEST + FACING_STEP, TALK_FACING_PARTY_THEN_WATCH, 0),
    ACTOR_PLACEMENT(83, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 310, 0, 422, FACING_NORTHWEST + FACING_STEP, TALK_FACING_PARTY_THEN_WATCH, 0),
    ACTOR_PLACEMENT(83, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 312, 0, 406, FACING_NORTHWEST + FACING_STEP, TALK_FACING_PARTY_THEN_WATCH, 0),
    ACTOR_PLACEMENT(56, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(7, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 130, 0, 364, FACING_SOUTHEAST, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(5, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(6, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(489, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 0),
    {SCENE_TABLE_END},
};

struct ActorPlacement gKibombo_Room3Placements[] = {
    ACTOR_PLACEMENT(27, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 366, 0, 586, FACING_SOUTH + FACING_STEP, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(28, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 352, 0, 586, FACING_SOUTHEAST + FACING_STEP, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(56, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 352, 0, 634, FACING_NORTH + FACING_STEP, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(115, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 136, 0, 108, FACING_SOUTH + FACING_STEP, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(145, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 124, 0, 120, FACING_NORTH + FACING_STEP, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(136, CONDITION_ALWAYS, MOVEMENT_TYPE_WANDER_NEAR, 100, 0, 132, FACING_NORTH + FACING_STEP, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(114, CONDITION_ALWAYS, MOVEMENT_TYPE_WANDER_NEAR, 710, 0, 134, FACING_EAST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(128, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 632, 0, 108, FACING_SOUTHEAST + FACING_STEP, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(112, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 432, 0, 120, FACING_SOUTHEAST + FACING_STEP, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(113, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 448, 0, 108, FACING_SOUTH + FACING_STEP, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(83, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 106, 0, 360, FACING_WEST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(83, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 88, 0, 352, FACING_EAST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(108, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 92, 0, 600, FACING_SOUTHEAST + FACING_STEP, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(109, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 72, 0, 600, FACING_SOUTHEAST + FACING_STEP, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(110, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 648, 0, 348, FACING_SOUTHEAST + FACING_STEP, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(134, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 84, 0, 858, FACING_SOUTHEAST + FACING_STEP, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(135, CONDITION_ALWAYS, MOVEMENT_TYPE_WANDER_NEAR, 148, 0, 878, FACING_SOUTH + FACING_STEP, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(201, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 374, 0, 344, FACING_SOUTHEAST + FACING_STEP, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(337, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 152, 0, 824, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(337, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 168, 0, 824, FACING_SOUTH, TALK_FACING_KEEP, 1),
    ACTOR_PLACEMENT(489, CONDITION_ALWAYS, 7, 448, 0, 120, FACING_SOUTH, TALK_FACING_KEEP, 0),
    {SCENE_TABLE_END},
};
// clang-format on

struct Trigger gKibombo_Triggers[] = {
    {TRIGGER_TABLE_END},
};

struct Trigger gKibombo_Room1Triggers[] = {
    {TRIGGER_EXIT, 1, CONDITION_ALWAYS, 1},
    {TRIGGER_EXIT, 2, CONDITION_ALWAYS, 2},
    {STEP_TRIGGER_WALKING_INTO(FACING_NORTH), 3, CONDITION_ALWAYS,
     EVENT(Func_02003948)},
    {STEP_TRIGGER_WALKING_INTO(FACING_NORTH), 4, CONDITION_ALWAYS,
     EVENT(Func_02003948)},
    {STEP_TRIGGER_WALKING_INTO(FACING_NORTH), 5, CONDITION_ALWAYS,
     EVENT(Func_02003948)},
    {STEP_TRIGGER_WALKING_INTO(FACING_NORTH), 6, CONDITION_ALWAYS,
     EVENT(Func_02003948)},
    {STEP_TRIGGER_WALKING_INTO(FACING_NORTH), 7, CONDITION_ALWAYS,
     EVENT(Func_02003948)},
    {STEP_TRIGGER_WALKING_INTO(FACING_NORTH), 8, CONDITION_ALWAYS,
     EVENT(Func_02003948)},
    {TRIGGER_EXIT, 9, CONDITION_ALWAYS, 9},
    {TRIGGER_EXIT, 10, CONDITION_ALWAYS, 10},
    {TRIGGER_STEP, 16, CONDITION_ALWAYS, EVENT(Func_02002ad8)},
    {TRIGGER_STEP, 15, CONDITION_ALWAYS, EVENT(Func_02002af8)},
    {PSYNERGY_ON_ACTOR(ABILITY_FROST), 8, 0x201, EVENT(Func_020005d0)},
    {PSYNERGY_ON_ACTOR(ABILITY_LASH), 12, 0x9e9, 0},
    {STEP_TRIGGER_WALKING_INTO(FACING_NORTH), 20, CONDITION_ALWAYS,
     EVENT(Func_02005994)},
    {STEP_TRIGGER_WALKING_INTO(FACING_WEST), 21, CONDITION_ALWAYS,
     EVENT(Func_02005994)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_BLAZE, PSYNERGY_PHASE_BEFORE_CAST), 11,
     CONDITION_ALWAYS, 0},
    {TRIGGER_TALK, 14, CONDITION_ALWAYS, MSGNO(MsgAkafubuHasFinally)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 14, CONDITION_ALWAYS,
     MSGNO(MsgEveryoneWasTerribly)},
    {TRIGGER_TALK, 15, CONDITION_ALWAYS, EVENT(Kibombo_AskAboutFewerWarriors)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 15, CONDITION_ALWAYS,
     MSGNO(MsgCoveringOurWarriors)},
    {TRIGGER_TALK, 16, CONDITION_ALWAYS, MSGNO(MsgTheGreatGabomba)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 16, CONDITION_ALWAYS,
     MSGNO(MsgIWantTo)},
    {TRIGGER_TALK, 17, CONDITION_ALWAYS, MSGNO(MsgNowThatThe)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 17, CONDITION_ALWAYS,
     MSGNO(MsgNowThatTheWitch)},
    {TRIGGER_TALK, 18, CONDITION_ALWAYS, MSGNO(MsgICouldnT)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 18, CONDITION_ALWAYS,
     MSGNO(MsgBecauseKibomboLies)},
    {TRIGGER_TALK, 19, CONDITION_ALWAYS, MSGNO(MsgOhISmell)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 19, CONDITION_ALWAYS,
     MSGNO(MsgYouDonTThink)},
    {TRIGGER_TALK, 20, CONDITION_ALWAYS, MSGNO(MsgGoodOlMeat)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 20, CONDITION_ALWAYS,
     MSGNO(MsgGulpTheSmell)},
    {TRIGGER_TALK, 21, CONDITION_ALWAYS, MSGNO(MsgWeGetTo)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 21, CONDITION_ALWAYS,
     MSGNO(MsgLatelyTheMonsters)},
    {TRIGGER_TALK, 22, CONDITION_ALWAYS, EVENT(Kibombo_AskAboutRevenge)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 22, CONDITION_ALWAYS,
     MSGNO(MsgNoOneInKibombo)},
    {TRIGGER_TALK, 23, CONDITION_ALWAYS, MSGNO(MsgTheOtherWarriors)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 23, CONDITION_ALWAYS,
     MSGNO(MsgWithAllOur)},
    {TRIGGER_TALK, 24, CONDITION_ALWAYS, EVENT(Kibombo_TalkPathToPeace)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 24, CONDITION_ALWAYS,
     MSGNO(MsgIWonderIfI)},
    {TRIGGER_TABLE_END},
};

struct Trigger gKibombo_Room4Triggers[] = {
    {TRIGGER_STEP, 1, CONDITION_ALWAYS, EVENT(Kibombo_LeaveRoom)},
    {TRIGGER_STEP, 2, CONDITION_ALWAYS, EVENT(Kibombo_LeaveRoom)},
    {STEP_TRIGGER_WALKING_INTO(FACING_NORTH), 3, 0x209, EVENT(Func_02003948)},
    {STEP_TRIGGER_WALKING_INTO(FACING_NORTH), 4, 0x209, EVENT(Func_02003948)},
    {STEP_TRIGGER_WALKING_INTO(FACING_NORTH), 5, 0x209, EVENT(Func_02003948)},
    {STEP_TRIGGER_WALKING_INTO(FACING_NORTH), 6, 0x209, EVENT(Func_02003948)},
    {STEP_TRIGGER_WALKING_INTO(FACING_NORTH), 7, 0x209, EVENT(Func_02003948)},
    {STEP_TRIGGER_WALKING_INTO(FACING_NORTH), 8, 0x209, EVENT(Func_02003948)},
    {TRIGGER_STEP, 9, CONDITION_ALWAYS, EVENT(Kibombo_LeaveRoom)},
    {TRIGGER_STEP, 10, CONDITION_ALWAYS, EVENT(Kibombo_LeaveRoom)},
    {TRIGGER_STEP, 16, CONDITION_ALWAYS, EVENT(Func_02002ad8)},
    {TRIGGER_STEP, 15, CONDITION_ALWAYS, EVENT(Func_02002af8)},
    {PSYNERGY_ON_ACTOR(ABILITY_FROST), 8, 0x201, EVENT(Func_020005d0)},
    {PSYNERGY_ON_ACTOR(ABILITY_LASH), 12, 0x9e9, 0},
    {STEP_TRIGGER_WALKING_INTO(FACING_NORTH), 20, CONDITION_ALWAYS,
     EVENT(Func_02005994)},
    {TRIGGER_TALK, 20, 0x8fd, MSGNO(MsgAkafubuSCeremony)},
    {TRIGGER_TALK, 20, CONDITION_ALWAYS, MSGNO(MsgTheyToldMe)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 20, 0x8fd, MSGNO(MsgItSStrange)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 20, CONDITION_ALWAYS,
     MSGNO(MsgAsLongAsThis)},
    {TRIGGER_TALK, 14, 0x8fd, MSGNO(MsgDuringTheLast)},
    {TRIGGER_TALK, 14, CONDITION_ALWAYS, MSGNO(MsgAkafubuHasBeen)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 14, 0x8fd,
     MSGNO(MsgTheGreatGabombaIs)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 14, CONDITION_ALWAYS,
     MSGNO(MsgWhatSTaking)},
    {TRIGGER_TALK, 27, CONDITION_ALWAYS, MSGNO(MsgIMAfraid)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 27, CONDITION_ALWAYS,
     MSGNO(MsgWhenHeFails)},
    {TRIGGER_TALK, 21, 0x8fd, MSGNO(MsgIHopeThe)},
    {TRIGGER_TALK, 21, CONDITION_ALWAYS, MSGNO(MsgWhewThankGoodness)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 21, 0x8fd, MSGNO(MsgIfTheGreat)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 21, CONDITION_ALWAYS,
     MSGNO(MsgAmIGoing)},
    {TRIGGER_TALK, 17, 0x8fd, MSGNO(MsgIsMakingThe)},
    {TRIGGER_TALK, 17, CONDITION_ALWAYS, MSGNO(MsgOnceTheCeremony)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 17, 0x8fd, MSGNO(MsgHeSFailed)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 17, CONDITION_ALWAYS,
     MSGNO(MsgKibomboWasSo)},
    {TRIGGER_TALK, 22, 0x8fd, MSGNO(MsgWhenTheGabomba)},
    {TRIGGER_TALK, 22, CONDITION_ALWAYS, MSGNO(MsgLastTimeAkafubu)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 22, 0x8fd, MSGNO(MsgIHopeAkafubu)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 22, CONDITION_ALWAYS,
     MSGNO(MsgAkafubuSAlways)},
    {TRIGGER_TALK, 28, CONDITION_ALWAYS, MSGNO(MsgAkafubuWasA)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 28, CONDITION_ALWAYS,
     MSGNO(MsgAkafubuHasNot)},
    {TRIGGER_TALK, 15, 0x8fd, MSGNO(MsgAkafubuLooksReally)},
    {TRIGGER_TALK, 15, CONDITION_ALWAYS, MSGNO(MsgIThinkAkafubu)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 15, 0x8fd, MSGNO(MsgHeTravelsTo)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 15, CONDITION_ALWAYS,
     MSGNO(MsgAkafubuIsObsessed)},
    {TRIGGER_TALK, 25, 0x8fd, MSGNO(MsgIfTheGreatGabomba)},
    {TRIGGER_TALK, 25, CONDITION_ALWAYS, MSGNO(MsgIBelieveIn)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 25, 0x8fd,
     MSGNO(MsgIfTheGreatGabombaActually)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 25, CONDITION_ALWAYS,
     MSGNO(MsgAkafubuAndOeia)},
    {TRIGGER_TALK, 18, 0x8fd, MSGNO(MsgAkafubuHasTried)},
    {TRIGGER_TALK, 18, CONDITION_ALWAYS, MSGNO(MsgWhatDonT)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 18, 0x8fd,
     MSGNO(MsgAsAChildAkafubu)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 18, CONDITION_ALWAYS,
     MSGNO(MsgIMSoFar)},
    {TRIGGER_TALK, 26, CONDITION_ALWAYS, MSGNO(MsgHeyNoPushing)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 26, CONDITION_ALWAYS,
     MSGNO(MsgThereAreSo)},
    {TRIGGER_TALK, 16, 0x8fd, MSGNO(MsgAkafubuIsAttempting)},
    {TRIGGER_TALK, 16, CONDITION_ALWAYS, MSGNO(MsgTonightSeemsTo)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 16, 0x8fd, MSGNO(MsgOeiaSSudden)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 16, CONDITION_ALWAYS,
     MSGNO(MsgWillTheGreat)},
    {TRIGGER_TALK, 23, 0x8fd, MSGNO(MsgHeWhoIs)},
    {TRIGGER_TALK, 23, CONDITION_ALWAYS, MSGNO(MsgOeiaSDeath)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 23, 0x8fd, MSGNO(MsgIfOnlyOeia)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 23, CONDITION_ALWAYS,
     MSGNO(MsgISuspectAkafubu)},
    {TRIGGER_TALK, 19, 0x8fd, MSGNO(MsgAkafubuBlamesHis)},
    {TRIGGER_TALK, 19, CONDITION_ALWAYS, MSGNO(MsgTheySayThe)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 19, 0x8fd,
     MSGNO(MsgTheyAttackedSome)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 19, CONDITION_ALWAYS,
     MSGNO(MsgTheyReSaying)},
    {TRIGGER_TALK, 24, 0x8fd, MSGNO(MsgIfTheGreatGabombaS)},
    {TRIGGER_TALK, 24, CONDITION_ALWAYS, MSGNO(MsgAkafubuSeesThe)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 24, 0x8fd, MSGNO(MsgIFeelSo)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 24, CONDITION_ALWAYS,
     MSGNO(MsgIReallyWonder)},
    {TRIGGER_TALK, 36, CONDITION_ALWAYS, EVENT(Kibombo_TalkForeignCeremony)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 36, CONDITION_ALWAYS,
     MSGNO(MsgIWonderIfA)},
    {TRIGGER_TABLE_END},
};

struct Trigger gKibombo_Room2Triggers[] = {
    {TRIGGER_STEP, 1, CONDITION_ALWAYS, EVENT(Kibombo_LeaveRoom)},
    {TRIGGER_STEP, 2, CONDITION_ALWAYS, EVENT(Kibombo_LeaveRoom)},
    {STEP_TRIGGER_WALKING_INTO(FACING_SOUTH), 3, IF_FLAG_SET(0x86a),
     EVENT(Kibombo_EnterVent)},
    {TRIGGER_STEP, 4, CONDITION_ALWAYS, EVENT(Kibombo_LeaveRoomFading)},
    {TRIGGER_STEP, 5, CONDITION_ALWAYS, EVENT(Kibombo_LeaveRoom)},
    {PSYNERGY_ON_ACTOR(ABILITY_FROST), 8, 0x201, EVENT(Func_020005d0)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), 11, 0x9ea, EVENT(Func_02000734)},
    {TRIGGER_PUSHED, 0, 0x9ea, EVENT(Func_02000734)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_SCOOP, PSYNERGY_PHASE_ACTS), 10, 0x86a,
     EVENT(Kibombo_ScoopVent)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_SCOOP, PSYNERGY_PHASE_ACTS), 11, 0x204,
     EVENT(Func_0200065c)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_SCOOP, PSYNERGY_PHASE_ACTS), 12, 0x205,
     EVENT(Func_020006a0)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_SCOOP, PSYNERGY_PHASE_ACTS), 13, 0x206,
     EVENT(Func_020006e4)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_SCOOP, PSYNERGY_PHASE_ACTS), 14, 0x207,
     EVENT(Func_02000728)},
    {TRIGGER_TABLE_END},
};

struct Trigger gKibombo_Room5Triggers[] = {
    {TRIGGER_STEP, 1, CONDITION_ALWAYS, EVENT(Kibombo_LeaveRoom)},
    {TRIGGER_STEP, 2, CONDITION_ALWAYS, EVENT(Kibombo_LeaveRoom)},
    {STEP_TRIGGER_WALKING_INTO(FACING_SOUTH), 3, IF_FLAG_SET(0x86a),
     EVENT(Kibombo_EnterVent)},
    {TRIGGER_STEP, 4, CONDITION_ALWAYS, EVENT(Kibombo_LeaveRoomFading)},
    {TRIGGER_STEP, 5, 0x8f3, EVENT(Kibombo_LeaveRoom)},
    {TRIGGER_STEP, 5, 0x92f, EVENT(Kibombo_PiersRefusesToLeave)},
    {TRIGGER_STEP, 5, CONDITION_ALWAYS, EVENT(Kibombo_LeaveRoom)},
    {PSYNERGY_ON_ACTOR(ABILITY_FROST), 8, 0x201, EVENT(Func_020005d0)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), 11, 0x9ea, EVENT(Func_02000734)},
    {TRIGGER_PUSHED, 0, 0x9ea, EVENT(Func_02000734)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_SCOOP, PSYNERGY_PHASE_ACTS), 10, 0x86a,
     EVENT(Kibombo_ScoopVent)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_SCOOP, PSYNERGY_PHASE_ACTS), 11, 0x204,
     EVENT(Func_0200065c)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_SCOOP, PSYNERGY_PHASE_ACTS), 12, 0x205,
     EVENT(Func_020006a0)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_SCOOP, PSYNERGY_PHASE_ACTS), 13, 0x206,
     EVENT(Func_020006e4)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_SCOOP, PSYNERGY_PHASE_ACTS), 14, 0x207,
     EVENT(Func_02000728)},
    {TRIGGER_STEP, 20, 0x8f3, EVENT(Kibombo_MeetPiers)},
    {TRIGGER_STEP, 22, 0x92e, EVENT(Kibombo_PiersThanksParty)},
    {TRIGGER_TALK, 7, CONDITION_ALWAYS, MSGNO(MsgTheOrbIs)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 7, CONDITION_ALWAYS,
     MSGNO(MsgIAmCertain)},
    {TRIGGER_STEP, 21, 0x8f5, EVENT(Kibombo_DeadEndByTheStatue)},
    {TRIGGER_TALK, 18, CONDITION_ALWAYS, MSGNO(MsgAhhSomethingIs)},
    {TRIGGER_TALK, 19, CONDITION_ALWAYS, MSGNO(MsgYouLookHuman)},
    {TRIGGER_TALK, 20, CONDITION_ALWAYS, MSGNO(MsgAkafubuPossessesThe)},
    {TRIGGER_TALK, 21, CONDITION_ALWAYS, MSGNO(MsgWeMustStand)},
    {TRIGGER_TALK, 22, CONDITION_ALWAYS, MSGNO(MsgMenOrGods)},
    {TRIGGER_TALK, 23, CONDITION_ALWAYS, MSGNO(MsgItSeemsThese)},
    {TRIGGER_TALK, 24, CONDITION_ALWAYS, MSGNO(MsgThisIsnT)},
    {TRIGGER_TALK, 25, CONDITION_ALWAYS, MSGNO(MsgItIsProper)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 18, CONDITION_ALWAYS,
     MSGNO(MsgTheyLookLike)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 19, CONDITION_ALWAYS,
     MSGNO(MsgIfNoPerson)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 20, CONDITION_ALWAYS,
     MSGNO(MsgISawAkafubu)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 21, CONDITION_ALWAYS,
     MSGNO(MsgIHopeAkafubuCompletes)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 22, CONDITION_ALWAYS,
     MSGNO(MsgWhatIfThe)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 23, CONDITION_ALWAYS,
     MSGNO(MsgLookAtThese)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 24, CONDITION_ALWAYS,
     MSGNO(MsgSureThisIs)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 25, CONDITION_ALWAYS,
     MSGNO(MsgKoffKoffAll)},
    {TRIGGER_TABLE_END},
};

struct Trigger gKibombo_Room3Triggers[] = {
    {TRIGGER_EXIT, 1, CONDITION_ALWAYS, 1},
    {TRIGGER_EXIT, 2, CONDITION_ALWAYS, 2},
    {TRIGGER_EXIT, 3, CONDITION_ALWAYS, 3},
    {TRIGGER_EXIT, 4, CONDITION_ALWAYS, 4},
    {TRIGGER_EXIT, 5, CONDITION_ALWAYS, 5},
    {TRIGGER_EXIT, 6, CONDITION_ALWAYS, 6},
    {STEP_TRIGGER_WALKING, 7, CONDITION_ALWAYS, EVENT(Func_02003ac8)},
    {TRIGGER_EXIT, 8, CONDITION_ALWAYS, 8},
    {STEP_TRIGGER_WALKING, 9, CONDITION_ALWAYS, EVENT(Func_02003b20)},
    {TRIGGER_EXIT, 10, CONDITION_ALWAYS, 10},
    {TRIGGER_EXIT, 11, CONDITION_ALWAYS, 11},
    {TRIGGER_EXIT, 12, CONDITION_ALWAYS, 12},
    {STEP_TRIGGER_WALKING, 13, CONDITION_ALWAYS, EVENT(Func_02003b88)},
    {TRIGGER_TALK, 8, 0x8ee, MSGNO(MsgIMGoing)},
    {TRIGGER_TALK, 8, CONDITION_ALWAYS, MSGNO(MsgIJustCan)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 8, CONDITION_ALWAYS,
     MSGNO(MsgDangWhyDid)},
    {TRIGGER_TALK, 9, 0x8ee, MSGNO(MsgIThinkThat)},
    {TRIGGER_TALK, 9, CONDITION_ALWAYS, MSGNO(MsgMySonHasBeen)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 9, CONDITION_ALWAYS,
     MSGNO(MsgAfterHeBecame)},
    {TRIGGER_TALK, 11, CONDITION_ALWAYS, MSGNO(MsgIHaveHeard)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 11, CONDITION_ALWAYS,
     MSGNO(MsgIKnowI)},
    {TRIGGER_TALK, 12, CONDITION_ALWAYS, EVENT(Kibombo_AskGoingWest)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 12, CONDITION_ALWAYS,
     MSGNO(MsgCrossingTheRiver)},
    {TRIGGER_TALK, 13, CONDITION_ALWAYS, MSGNO(MsgYouCanSee)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 13, CONDITION_ALWAYS,
     MSGNO(MsgIfIM)},
    {TRIGGER_TALK, 14, CONDITION_ALWAYS, MSGNO(MsgIfYouAreLooking)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 14, CONDITION_ALWAYS,
     MSGNO(MsgSureOurLegends)},
    {TRIGGER_TALK, 15, CONDITION_ALWAYS, MSGNO(MsgEachYearWhen)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 15, CONDITION_ALWAYS,
     MSGNO(MsgIWonderWhatOeia)},
    {TRIGGER_TALK, 16, CONDITION_ALWAYS, EVENT(Kibombo_TalkAchingBack)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 16, CONDITION_ALWAYS,
     EVENT(Kibombo_ReadMindNeverAgain)},
    {TRIGGER_TALK, 28, CONDITION_ALWAYS, EVENT(Kibombo_TalkAchingBack)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 28, CONDITION_ALWAYS,
     EVENT(Kibombo_ReadMindNeverAgain)},
    {TRIGGER_TALK, 17, CONDITION_ALWAYS, MSGNO(MsgAsSoonAs)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 17, CONDITION_ALWAYS,
     MSGNO(MsgAkafubuGetsSo)},
    {TRIGGER_TALK, 18, CONDITION_ALWAYS, MSGNO(MsgManyInGondowan)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 18, CONDITION_ALWAYS,
     MSGNO(MsgItWonT)},
    {TRIGGER_TALK, 19, CONDITION_ALWAYS, MSGNO(MsgWeThinkIt)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 19, CONDITION_ALWAYS,
     MSGNO(MsgIAmAMighty)},
    {TRIGGER_TALK, 20, CONDITION_ALWAYS, EVENT(Kibombo_OpenWeaponShop)},
    {TRIGGER_EXAMINE, 20, CONDITION_ALWAYS, EVENT(Kibombo_OpenWeaponShop)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 20, CONDITION_ALWAYS,
     MSGNO(MsgOurWarriorsAppear)},
    {TRIGGER_TALK, 21, CONDITION_ALWAYS, EVENT(Kibombo_OpenArmorShop)},
    {TRIGGER_EXAMINE, 21, CONDITION_ALWAYS, EVENT(Kibombo_OpenArmorShop)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 21, CONDITION_ALWAYS,
     MSGNO(MsgBecauseOurPaint)},
    {TRIGGER_TALK, 22, CONDITION_ALWAYS, EVENT(Kibombo_OpenItemShop)},
    {TRIGGER_EXAMINE, 22, CONDITION_ALWAYS, EVENT(Kibombo_OpenItemShop)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 22, CONDITION_ALWAYS,
     MSGNO(MsgIfIHad)},
    {TRIGGER_TALK, 23, CONDITION_ALWAYS, EVENT(Kibombo_OpenInn)},
    {TRIGGER_EXAMINE, 23, CONDITION_ALWAYS, EVENT(Kibombo_OpenInn)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 23, CONDITION_ALWAYS,
     MSGNO(MsgThatBedMakes)},
    {TRIGGER_TALK, 24, CONDITION_ALWAYS, MSGNO(MsgICanTRemember)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 24, CONDITION_ALWAYS,
     MSGNO(MsgILoveKibombo)},
    {TRIGGER_TALK, 25, CONDITION_ALWAYS, EVENT(Kibombo_OpenSanctum)},
    {TRIGGER_EXAMINE, 24, CONDITION_ALWAYS, EVENT(Kibombo_OpenSanctum)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 25, CONDITION_ALWAYS,
     MSGNO(MsgICanTBelieve)},
    {TRIGGER_TABLE_END},
};
#undef PALETTE_WORD
#undef DEFINE_KIBOMBO_SET_CROWD_MODE
#undef Actor_WaitForMove
#undef FieldCamera_GetActor
#undef DEFINE_KIBOMBO_STEP_WHEN_HELD
#undef Actor_SetSpriteFlags
#undef Actor_SetBlendMode
#undef Actor_SetPositionAndResetMotion
#undef Actor_SetSpritePriority
#undef Actor_SetMoveTarget
#undef Import_ActorCreate
#undef Actor_SetSpritePriority
#undef Actor_SetPositionAndResetMotion
