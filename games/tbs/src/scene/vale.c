/// @file
/// Vale (scene 4), Isaac's village by day, outside the houses: the door of
/// Isaac's house, the houses of Garet's family and of Jenna's grandparents,
/// the Sanctum and the paths out to Mt. Aleph and Kraden's cottage. The
/// placements and events come in three sets, chosen by the story flags: before
/// the party leaves Vale, after it has left (FLAG_PARTY_LEFT_VALE) and back
/// from Kalay (FLAG_REACHED_KALAY); entrance 16 has a set of its own. In story
/// order:
///
/// - The day the story begins, three years after the storm (vale_babi.c), which
///   took Jenna's family and Isaac's father, Kyle. The game opens at entrance
///   16 with Event_MendingTheRoof: under the caption "Three years later...",
///   Isaac is mending the roof of his house while Dora, his mother, calls up to
///   him; Garet, who will travel with him, trains his Psynergy nearby, and
///   Jenna comes by, and the two speak of that day before going to fetch Isaac
///   to go to Kraden's. Exit 12 brings the party back in at entrance 17, where
///   Event_RoofMended plays once (group flag 0x308): Dora praises the roof,
///   remembers Kyle, and sends the three off on their climb of Mt. Aleph with
///   Kraden, the scholar who teaches them Alchemy (FLAG_SCENE_TEMP_02); for the
///   rest of that visit Dora frets about the hole clumsy Garet left.
/// - Vale before the party leaves. Until Kraden has planned the visit to Sol
///   Sanctum (FLAG_SANCTUM_VISIT_PLANNED), Jenna stops Isaac on the path to Mt.
///   Aleph; a villager jumps out to warn him against sneaking up the sacred
///   peak (FLAG_VALE_WARNED_AGAINST_MT_ALEPH) and then keeps watching him,
///   asking again whenever he comes near (touch cells 51 to 53). Three
///   puppies (actors 15 to 17) play; walking up to them from either side
///   (touch cells 20 and 21) sets them barking, until the party has met Saturos
///   and Menardi, of the strangers staying at the inn
///   (FLAG_MET_SATUROS_AND_MENARDI); then the setup removes them. Villagers
///   talk of the mayor, Garet's grandfather, and of not going beyond Kraden's
///   cottage, one scolds Garet, and Dora says Isaac can fix the hole later.
/// - After the party has left Vale (FLAG_PARTY_LEFT_VALE): Dora will not open
///   the door of Isaac's house to him ("My Isaac would keep his promise... Go
///   away."), and the villagers speak of the tremors and of Kraden, who has
///   not come back.
/// - Back from Kalay (FLAG_REACHED_KALAY): Garet meets his sister (group flag
///   0x303) and asks one of his family to protect it while he is gone (0x304),
///   a villager asks whether Jenna is back (FLAG_VALE_ASKED_IF_JENNA_IS_BACK),
///   another feels a tremor, another asks about the lack of travellers since
///   the eruption (0x307). Isaac's house opens again: Dora lies ill inside
///   (vale_babi.c).
///
/// In every set: a Nut to catch with Catch (its flag 0xfd0; actor 26, or 20
/// back from Kalay), an object to push or Move, whose column the setup restores
/// from group flags 0x314 to 0x316, a passage the leader moves through on touch
/// cells 100 and 101 (FLAG_SCENE_TEMP_04 while inside), and cells of an upper
/// path that change while the leader is on it (FLAG_SCENE_TEMP_10); after a
/// battle (FLAG_KEEP_PARTY_POSITION) the setup restores both. Exits: 1 up Mt.
/// Aleph to Sol Entrance (SolEntrance, scene 19), 2 to ValeHome (scene
/// 5), 3 and 4 to ValeKraden (scene 6), the doors 5, 6, 8, 9 and 10 into the
/// houses (scene 7, vale_interiors.c, rooms 6, 5, 2, 1 and 2), 7 into Isaac's
/// house (ValeBabi, scene 8) and 11 into Vale Sanctum (Sanctum, scene 9). Two
/// scenes no table reaches remain: HouseScene_RunRepairMorning, a copy of the
/// roof's end, and FieldScene_RunCompanionActorSequence, which sets group flag
/// 0x305. FLAG_SCENE_TEMP_41 pauses the villager who watches Isaac while a menu
/// is open. The TBS_PATCH_4 editions also set Dora's sprite priority before
/// Kalay.

#include "game_state.h"
#include "vram_tab.h"
#include "unit/import.h"
#include "unit/scene/vale.h"
#include "unit/scene/common/object_orbit.h"
#include "unit/scene/common/effect_psynergy.h"
#include "unit/scene/common/world_map_linked_effects.h"
#include "unit/object/object.h"
#include "unit/object/psynergy_effects.h"
#include "unit/field/field.h"
#include "unit/window/window.h"
#include "unit/window/battle_command.h"
#include "unit/runtime/runtime.h"
#include "battle.h"
#include "debug.h"
#include "field.h"
#include "game.h"
#include "graphics.h"
#include "menu.h"
#include "minigame.h"
#include "sound.h"
#include "system.h"
#include "mem.h"
#include "global_cells.h"
#include "edition.h"
#include "vale.h"
#include "types.h"
#include "call.h"
#include "field_event.h"
#include "field_service.h"
#include "field_scene.h"
#include "iwram_call.h"
#include "text/msg_ids.h"
#include "ability_ids.h"
#include "objcmd.h"
#include "field_obj.h"
#include "field_effect.h"
#include "map_scroll.h"
#include "fixed_math.h"
#include "macros.h"
#include "flags.h"
#include "songs.h"

#if TBS_PATCH_2
#define VALE_GARET_REUNION_Z 829
#else
#define VALE_GARET_REUNION_Z 827
#endif

#if TBS_PATCH_4
#define VALE_SOUTH_VILLAGER_Z 887
#else
#define VALE_SOUTH_VILLAGER_Z 889
#endif

static void Vale_HaveJennaStopTheAlephDetour(void);
static void Vale_AskAboutSneakingToAleph(const s32 *leaderActions,
                                         const s32 *villagerActions);
static void Vale_MoveActorAlongLowerZPath(s32 actorId, s32 approachAnimationId,
                                          s32 departureAnimationId,
                                          s32 resumeMotion);
static void Vale_MoveActorAlongHigherZPath(s32 actorId, s32 approachAnimationId,
                                           s32 departureAnimationId,
                                           s32 resumeMotion);
static void SceneActor_ResetActorRun(s32 first, u32 count, s32 mode);
static void Event_MendingTheRoof(void);
static void Event_RoofMended(void);
static void Vale_UpdateGaretPaletteAndEffects(void);
static void Vale_UpdateLeaderPaletteAndEffects(void);
static void Vale_CycleVillagerNinePalette(void);
static void Vale_CycleFirstBoardPalette(void);
static void Vale_CycleSecondBoardPalette(void);
static void Vale_CycleActor25Palette(void);
#ifdef UBFIX
static void Vale_HaveVillagerWatchTheLeader(union FieldObject *fieldObject);
#else
static s32 Vale_HaveVillagerWatchTheLeader(union FieldObject *fieldObject);
#endif
static void Effect_ConfigureSpawnedParticle(union FieldObject *fieldObject);
static void Effect_SpawnRisingDustBurst(struct FieldActor *emitter);
static void Effect_UpdateParticlePosition(struct FieldActor *particle,
                                          s32 distance, s32 angle);
static void Event_SettleMovedObject(void);
static void Effect_PlayStepSound(void);
static void Vale_SynchronizeAlephVillagerPause(void);
static void Vale_AdjustVillagerPriorityForLeaderHeight(void);
static void Vale_FlashActorAndSpawnEffects(struct FieldActor *actor);
static void Vale_CycleActorPaletteAndSpawnEffects(struct FieldActor *actor);
static void Vale_CycleActorPalette(struct FieldActor *actor);

enum HouseActor { ACTOR_BOARD = 23, ACTOR_LAST_BOARD = 24 };

enum { ANIM_HAMMER = 11 };

#include "common/object_staged_actor.h"

const struct SceneEntrance *Vale_GetEntrances(void) { return gVale_Entrances; }

const struct SceneRegion *Vale_GetRegions(void) { return NULL; }

const u32 *Vale_GetExits(void) { return gVale_Exits; }

/// Places the villagers for the story so far, or for the roof repair (entrance
/// 16).
const struct ScenePlacement *Vale_GetPlacements(void) {
  u8 *scene_state = (u8 *)&gGameState;
  if (((struct GameState *)scene_state)->entrance == VALE_FROM_TITLE)
    return gVale_RepairPlacements;
  if (Engine_GameFlagIsSet(FLAG_REACHED_KALAY) != 0)
    return gVale_PlacementsAfterKalay;
  if (Engine_GameFlagIsSet(FLAG_PARTY_LEFT_VALE) != 0)
    return gVale_PlacementsAfterLeaving;
  return gVale_Placements;
}

/// Catch on the Nut in Vale: the party gets it.
static void Event_CatchNut(void) {
  Engine_EventBegin();
  Engine_ActorSetPosition(26, 0, 0);
  Engine_GameFlagSet(0xfd0);
  Engine_ItemShowFound(ITEM_NUT, 3);
  Engine_PartyGiveItem(ITEM_NUT, 0);
  Engine_EventEnd();
}

/// The same Nut after Kalay, where it is actor 20.
static void Event_CatchNutAfterKalay(void) {
  Engine_EventBegin();
  Engine_ActorSetPosition(20, 0, 0);
  Engine_GameFlagSet(0xfd0);
  Engine_ItemShowFound(ITEM_NUT, 3);
  Engine_PartyGiveItem(ITEM_NUT, 0);
  Engine_EventEnd();
}

/// Vale's events for the story so far: before leaving, after leaving, or back
/// from Kalay.
const struct SceneEvent *Vale_GetEvents(void) {
  if (Engine_GameFlagIsSet(FLAG_REACHED_KALAY) != 0)
    return gVale_EventsAfterKalay;
  if (Engine_GameFlagIsSet(FLAG_PARTY_LEFT_VALE) != 0)
    return gVale_EventsAfterLeaving;
  return gVale_Events;
}

/// A villager in Vale (actor 10) asks about the mayor before the party leaves,
/// and speaks of the tremors after.
static void Event_TalkAboutTheMayorAndTremors(void) {
  Engine_EventBegin();
  if (Engine_GameFlagIsSet(FLAG_PARTY_LEFT_VALE) != 0) {
    Engine_EventSetMessage(MsgValeTheGroundStillShakes);
    Engine_EventShowMessage(10, 0);
  } else {
    Engine_EventSetMessage(MsgValeYourGrandpaIsTheMayor);
    Engine_ActorFaceEachOther(10, ACTOR_PARTY_LEADER, 4);
    Engine_EventAskYesNo(10, 0);
  }
  Engine_EventEnd();
}

/// A villager in Vale (actor 14) asks whether Isaac is going to Kraden's
/// cottage and warns him not to go beyond it; after the party has left, says
/// Kraden has not come back.
static void Event_WarnAboutGoingBeyondKradensCottage(void) {
  s32 flag = FLAG_VALE_ASKED_IF_GOING_TO_KRADEN;
  Engine_EventBegin();
  if (Engine_GameFlagIsSet(FLAG_PARTY_LEFT_VALE) != 0) {
    Engine_EventSetMessage(MsgValeKradenHasntComeBack);
    Engine_EventShowMessage(14, 0);
  } else if (Engine_GameFlagIsSet(flag) == 0) {
    Engine_GameFlagSet(flag);
    Engine_EventSetMessage(MsgValeDoYouNeedToGo);
    Engine_ActorFaceEachOther(14, ACTOR_PARTY_LEADER, 4);
    Engine_EventAskYesNo(14, 0);
  } else {
    Engine_EventSetMessage(MsgValeDontGoBeyondKradensCottage);
    Engine_ActorFaceEachOther(14, ACTOR_PARTY_LEADER, 4);
    Engine_EventShowMessage(14, 0);
  }
  Engine_EventEnd();
}

/// A villager in Vale (actor 18) scolds Garet; afterwards the villager only
/// threatens him.
/// "Garet, you make me so mad sometimes!"
static void Event_ArgueWithTheAngryVillager(void) {
  Engine_EventBegin();
  if (GameFlag_IsSet(FLAG_VALE_ANGRY_VILLAGER_ARGUED) == 0) {
    GameFlag_Set(FLAG_VALE_ANGRY_VILLAGER_ARGUED);
    Engine_EventSetMessage(MsgValeYouMakeMeSoMad);
    Actor_ShowEmote(18, EMOTE_IN_FRONT | 3, 0);
    Engine_ActorFaceEachOther(ACTOR_PARTY_LEADER, 18, 20);
    Event_ShowMessageAndWait(18, 0, 6);
    Actor_FaceDirection(18, FACING_WEST, 30);
    Engine_ActorJump(18, 2, 20);
    Event_ShowMessageAndWait(18, 0, 6);
    Engine_ActorFaceEachOther(18, ACTOR_PARTY_LEADER, 10);
    Actor_ShowEmote(18, EMOTE_IN_FRONT | 3, 0);
    Event_ShowMessageAndWait(18, 0, 10);
    Actor_ShowEmote(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | 2, 60);
  } else {
    Actor_ShowEmote(18, EMOTE_IN_FRONT | 3, 0);
    Engine_EventSetMessage(MsgValeIllGetYouForMy);
    Event_ShowMessageAndWait(18, 0, 20);
  }
  Engine_EventEnd();
}

/// Talking to Dora in Vale (actor 21) before the party leaves: right after
/// the roof is mended (FLAG_SCENE_TEMP_02) she frets about the hole clumsy
/// Garet left; otherwise, since it will not rain for a while, Isaac can fix it
/// later.
static void Event_DoraFretsAboutTheRoof(void) {
  Engine_EventBegin();
  if (Engine_GameFlagIsSet(FLAG_SCENE_TEMP_02) != 0) {
    Engine_EventSetMessage(MsgValeIToldGaretItWas);
  } else {
    Engine_EventSetMessage(MsgValeItWontRainForSome);
  }
  Engine_EventShowMessage(21, 0);
  Engine_EventEnd();
}

/// Back from Kalay, a villager in Vale (actor 10) asks whether Jenna is back.
static void Event_AskWhetherJennaHasReturned(void) {
  Engine_EventBegin();
  Engine_ActorFaceEachOther(ACTOR_PARTY_LEADER, 10, 20);
  Engine_EventSetMessage(MsgValeIsJennaBackYet);
  Engine_EventShowMessage(10, 0);
  Engine_GameFlagSet(FLAG_VALE_ASKED_IF_JENNA_IS_BACK);
  Engine_EventEnd();
}

/// Back from Kalay, a villager in Vale (actor 17) feels another tremor, and
/// the view shakes.
static void Event_ReactToAnotherTremor(void) {
  Engine_EventBegin();
  Map_SetShake(PIXELS(3), PIXELS(3), Q16_ONE);
  Engine_TaskWait(10);
  Map_SetShake(-1, -1, 9 * Q16_ONE / 10);
  Engine_EventSetMessage(MsgValeIFeltAnotherOne);
  Event_ShowMessageAndWait(17, 0, 20);
  Actor_FaceActor(17, ACTOR_PARTY_LEADER, 20);
  Event_ShowMessage(17, 0);
  Engine_EventEnd();
}

/// Back from Kalay, a villager in Vale (actor 19) asks about the lack of
/// travellers since the eruption; sets group flag 0x307.
static void Event_AskAboutTheLackOfTravelers(void) {
  Engine_EventBegin();
  Engine_ActorRunRepeatedMotion(19, 2);
  Engine_EventWait(20);
  Engine_ActorFaceActor(19, ACTOR_PARTY_LEADER, 20);
  Engine_EventSetMessage(MsgValeNoTravelersSinceTheEruption);
  Engine_EventAskYesNo(19, 0);
  Engine_GameFlagSet(0x307);
  Engine_EventEnd();
}

/// Walking onto the upper path in Vale (touch cell 15): its collision cells
/// change, remembered in FLAG_SCENE_TEMP_10.
static void Event_SetPathCellOverride(void) {
  Engine_GameFlagSet(FLAG_SCENE_TEMP_10);
  Map_CopyCellAttributes(40, 84, 7, 4, 10, 84);
}

/// Walking off the upper path in Vale (touch cell 14) restores its collision
/// cells.
static void Event_ClearPathCellOverride(void) {
  Engine_GameFlagClear(FLAG_SCENE_TEMP_10);
  Map_CopyCellAttributes(40, 89, 7, 4, 10, 84);
}

/// The door of Vale Sanctum (touch cell 1): it opens, the leader walks in, and
/// exit 11 leads into the Sanctum.
static void Event_EnterSanctum(void) {
  Event_PlayCue(SE_188);
  Map_AnimateCells(gValeCellAnimC, 45, 11);
  Actor_WalkTo(ACTOR_PARTY_LEADER, 257, 420);
  Engine_EventRequestExit(VALE_TO_SANCTUM);
}

/// The path to Mt. Aleph in Vale (touch cell 2): Jenna stops Isaac until
/// Kraden has planned the visit (FLAG_SANCTUM_VISIT_PLANNED); then exit 1 leads
/// up to Sol Entrance. Jenna says "Kraden is waiting for us".
static void Event_ApproachMtAleph(void) {
  if (Engine_GameFlagIsSet(FLAG_SANCTUM_VISIT_PLANNED) == 0) {
    Vale_HaveJennaStopTheAlephDetour();
  } else {
    Engine_AudioPlayCue(SE_MAP_EXIT);
    Engine_EventRequestExit(VALE_TO_SOL_ENTRANCE);
  }
}

/// Touch cell 3 in Vale: exit 3, to ValeKraden's entrance 1.
static void Event_TakeExit3(void) {
  Engine_AudioPlayCue(SE_MAP_EXIT);
  Engine_EventRequestExit(3);
}

/// Touch cell 4 in Vale: exit 4, to ValeKraden's entrance 2.
static void Event_TakeExit4(void) {
  Engine_AudioPlayCue(SE_MAP_EXIT);
  Engine_EventRequestExit(4);
}

/// Touch cell 5 in Vale: exit 2, to ValeHome's entrance 8.
static void Event_TakeExit2(void) {
  Engine_AudioPlayCue(SE_MAP_EXIT);
  Engine_EventRequestExit(VALE_TO_VALE_HOME);
}

/// The door of the house of Garet's family in Vale (touch cell 6): it opens and
/// exit 5 leads in (room 6 of scene 7).
static void Event_EnterGaretsHouse(void) {
  Event_PlayCue(SE_DOOR_OPEN);
  Map_AnimateCells(gValeCellAnimA, 54, 32);
  Actor_WalkTo(ACTOR_PARTY_LEADER, 406, 727);
  Engine_EventRequestExit(VALE_TO_VALE_INTERIORS_EAST);
}

/// A house door in Vale (touch cell 7): it opens and exit 6 leads in
/// (room 5 of scene 7).
static void Event_EnterInteriorsRoom5(void) {
  Event_PlayCue(SE_DOOR_OPEN);
  Map_AnimateCells(gValeCellAnimB, 45, 39);
  Actor_WalkTo(ACTOR_PARTY_LEADER, 262, 805);
  Engine_EventRequestExit(6);
}

/// The door of Isaac's house in Vale (touch cell 8): after the party has left
/// and before Kalay, Dora will not open. "My Isaac would keep his promise...
/// Go away." Otherwise exit 7 leads in (ValeBabi's entrance 6).
static void Event_EnterIsaacsHouse(void) {
  if (Engine_GameFlagIsSet(FLAG_PARTY_LEFT_VALE) != 0 &&
      Engine_GameFlagIsSet(FLAG_REACHED_KALAY) == 0) {
    Engine_EventBegin();
    Engine_EventSetMessage(MsgValeYouCantBeIsaac);
    Engine_EventOpenMessage(21, 0);
    if (Engine_EventChooseYesNo(0, 0) == 0) {
      Engine_EventShowMessageAndWait(21, 0, 60);
      Engine_EventShowMessage(21, 0);
    } else {
      u16 *message = &gEventWork->message;

      *message = *message + 2;
      Engine_EventWait(40);
      Engine_EventShowMessage(21, 0);
    }
    Engine_EventEnd();
  } else {
    Engine_AudioPlayCue(SE_DOOR_OPEN);
    Engine_MapAnimateCells(gValeCellAnimA, 50, 44);
    Engine_ActorWalkTo(ACTOR_PARTY_LEADER, 340, 888);
    Engine_EventRequestExit(VALE_TO_VALE_BABI);
  }
}

/// The door of the house of Jenna's grandparents in Vale (touch cell 9): it
/// opens and exit 8 leads in (room 2 of scene 7).
static void Event_EnterJennasGrandparentsHouse(void) {
  Event_PlayCue(SE_DOOR_OPEN);
  Map_AnimateCells(gValeCellAnimB, 49, 69);
  Actor_WalkTo(ACTOR_PARTY_LEADER, 326, 1126);
  Engine_EventRequestExit(VALE_TO_VALE_INTERIORS_NORTH);
}

/// A house door in Vale (touch cell 10): it opens and exit 9 leads in
/// (room 1 of scene 7).
static void Event_EnterInteriorsRoom1(void) {
  Event_PlayCue(SE_DOOR_OPEN);
  Map_AnimateCells(gValeCellAnimD, 52, 76);
  Actor_WalkTo(ACTOR_PARTY_LEADER, 374, 1238);
  Engine_EventRequestExit(9);
}

/// A second door in Vale into the house of Jenna's grandparents (touch cell
/// 11): it opens and exit 10 leads in (room 2 of scene 7). XXX: likely a slip
/// in the exit table, because exit 10 sends this door, beside entrance 11,
/// into room 2, whose door returns the party at entrance 9 by the other door,
/// and no exit leads to entrance 11; entering the room by this door in a
/// play-through would settle it.
static void Event_EnterJennasGrandparentsHouseByExit10(void) {
  Event_PlayCue(SE_DOOR_OPEN);
  Map_AnimateCells(gValeCellAnimA, 35, 74);
  Actor_WalkTo(ACTOR_PARTY_LEADER, 102, 1206);
  Engine_EventRequestExit(10);
}

/// On the path to Mt. Aleph in Vale, before the visit is planned: Jenna comes
/// out to remind Isaac that Kraden is waiting, and he turns back.
static void Vale_HaveJennaStopTheAlephDetour(void) {
  struct FieldActor *leader;
  struct FieldActor *jenna;
  struct FieldActor *actor;

  leader = Object_GetById(ACTOR_PARTY_LEADER);
  jenna = Object_GetById(JENNA);
  Engine_EventBegin();
  jenna->x.fixed = leader->x.fixed;
  jenna->y.fixed = leader->y.fixed;
  jenna->z.fixed = leader->z.fixed;
  jenna->targetX = ACTOR_NO_TARGET;
  jenna->targetY = ACTOR_NO_TARGET;
  jenna->targetZ = ACTOR_NO_TARGET;
  SET_VELOCITY(jenna, 0, 0, 0);
  jenna->terrainHeight = leader->y.fixed;
  Engine_TaskWait(1);
  Actor_SetSpeed(JENNA, PIXELS(1), PIXELS(1) / 2);
  Actor_WalkToAndWait(JENNA, 110, 283);
  Engine_ActorFaceEachOther(ACTOR_PARTY_LEADER, JENNA, 2);
  Engine_EventSetMessage(MsgValeKradenIsWaitingForUs);
  if (leader->x.fixed < jenna->x.fixed) {
    Event_ShowMessageAndWait(0xa005, 0, 2);
  } else {
    Event_ShowMessageAndWait(SPEAKER_WINDOW_BELOW | 5, 0, 2);
  }
  Engine_ActorSetAnimationAndWait(ACTOR_PARTY_LEADER, ANIM_NOD);
  Engine_EventWait(2);
  Engine_ActorSetAnimation(JENNA, ANIM_WALK);
  actor = Object_GetById(ACTOR_PARTY_LEADER);
  if (actor != NULL) {
    Actor_SetDestination(JENNA, actor->x.part.pixel, actor->z.part.pixel);
  }
  Engine_ActorWaitForMove(JENNA);
  Actor_SetPosition(JENNA, 0, 0);
  Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 110, 303);
  Engine_EventEnd();
}

/// The puppies in Vale (touch cell 20), until the party has met Saturos and
/// Menardi: they bark, the camera pans down to them and back, and the leader
/// turns away.
static void Event_WatchThePuppies(void) {
  struct MapScrollWork *map;
  s32 *savedOrigin;
  struct FieldActor *leader;
  s32 frame;
  s32 cameraPosition[3];

  if (Engine_GameFlagIsSet(FLAG_MET_SATUROS_AND_MENARDI) == 0) {
    map = gMapWork[0];
    Engine_EventBegin();
    Actor_SetSpeed(0, PIXELS(1), PIXELS(1) / 2);
    Engine_ActorSetAnimation(0, ANIM_STAND);
    Engine_EventWait(2);
    Engine_EventSetMessage(MsgValeRrruffRrrruff);
    Engine_EventShowMessageAndWait(15, 0, 2);
    Engine_EventShowMessageAndWait(16, 0, 2);
    leader = Object_GetById(ACTOR_PARTY_LEADER);
    SET_VECTOR(cameraPosition, leader->x.fixed, leader->y.fixed,
               leader->z.fixed);
    savedOrigin = map->origin;
    map->origin = cameraPosition;
    frame = 0;
    // Pan down to the puppies while the leader stays in place.
    do {
      cameraPosition[2] += PIXELS(2);
      Engine_EventWait(1);
      frame++;
      Engine_MapRedraw();
    } while (frame != 40);
    Engine_EventWait(60);
    Engine_MessageShowCentered(MsgValePuppiesPlayingOver, 1);
    frame = 0;
    Engine_EventWait(6);
    do {
      cameraPosition[2] -= PIXELS(2);
      Engine_EventWait(1);
      frame++;
      Engine_MapRedraw();
    } while (frame != 40);
    // Return camera ownership to the normal follow target.
    map->origin = savedOrigin;
    Engine_EventWait(60);
    Actor_WalkToAndWait(0, 70, 741);
    Engine_EventEnd();
  }
}

/// The puppies in Vale from the other side (touch cell 21): the same barking,
/// without the camera pan.
static void Event_TurnBackFromThePuppies(void) {
  s32 msg;

  if (GameFlag_IsSet(FLAG_MET_SATUROS_AND_MENARDI) == 0) {
    Engine_EventBegin();
    Actor_SetSpeed(ACTOR_PARTY_LEADER, PIXELS(1), PIXELS(1) / 2);
    msg = MsgValeRrruffRrrruff;
    Engine_EventSetMessage(msg);
    Event_ShowMessageAndWait(15, 0, 2);
    Event_ShowMessageAndWait(16, 0, 2);
    Engine_MessageShowCentered(msg + 2, 1);
    Engine_EventWait(6);
    Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 69, 870);
    Engine_EventEnd();
  }
}

/// Touch cell 100 in Vale: the passage's collision changes and the leader
/// moves through it from its northern end (FLAG_SCENE_TEMP_04 set).
static void Event_EnterNorthernPassage(void) {
  Engine_EventBegin();
  Map_CopyCellAttributes(49, 53, 8, 4, 20, 50);
  Vale_MoveActorAlongLowerZPath(0, 10, 11, 1);
  Engine_GameFlagSet(FLAG_SCENE_TEMP_04);
  Engine_EventEnd();
}

/// Touch cell 101 in Vale: the leader moves back through the passage, and its
/// collision is restored (FLAG_SCENE_TEMP_04 cleared).
static void Event_ReturnThroughSouthernPassage(void) {
  Engine_EventBegin();
  Vale_MoveActorAlongHigherZPath(0, 13, 10, 1);
  Engine_GameFlagClear(FLAG_SCENE_TEMP_04);
  Map_CopyCellAttributes(49, 46, 8, 4, 20, 50);
  Engine_EventEnd();
}

/// A villager in Vale (actor 22, touch cell 50) jumps out in front of Isaac,
/// then keeps watching him; sets FLAG_VALE_WARNED_AGAINST_MT_ALEPH.
/// "You're not planning on sneaking up to Mt. Aleph...?"
static void Event_WarnAgainstSneakingUpMtAleph(void) {
  struct FieldActor *villager;
  struct FieldActor *actor;

  villager = Object_GetById(22);
  Engine_EventBegin();
  Actor_SetSpeed(ACTOR_PARTY_LEADER, PIXELS(2), PIXELS(2));
  Engine_ActorJump(ACTOR_PARTY_LEADER, 5, 0);
  Actor_WalkTo(ACTOR_PARTY_LEADER, 215, 403);
  villager->steeringFlags |= ACTOR_FACES_MOTION;
  Actor_SetPosition(22, PIXELS(166), PIXELS(375));
  Actor_FaceDirection(22, FACING_SOUTHEAST, 20);
  villager->steeringFlags = (villager->steeringFlags ^ 1);
  Actor_SetSpeed(22, PIXELS(5) / 2, PIXELS(5) / 2);
  Engine_ActorJump(22, 4, 0);
  Actor_WalkToAndWait(22, 202, 395);
  Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_STAND);
  Engine_EventWait(10);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTHWEST + FACING_STEP, 0);
  Actor_FaceDirection(22, FACING_SOUTHEAST + FACING_STEP, 24);
  Actor_ShowEmote(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | 0, 0);
  Engine_ActorStartRepeatedMotion(ACTOR_PARTY_LEADER, 2);
  Engine_EventWait(20);
  Actor_SetSpeed(22, PIXELS(3) / 2, PIXELS(1));
  Actor_SetActionScript(ACTOR_PARTY_LEADER, gValeLeaderWalkActions);
  Engine_EventWait(10);
  Actor_ShowEmote(22, EMOTE_IN_FRONT | 3, 0);
  Actor_SetActionScript(22, gValeActor22WalkActions);
  Object_RefreshSelectorById(0);
  Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 256, 474);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 0);
  Object_RefreshSelectorById(22);
  Actor_WalkToAndWait(22, 256, 456);
  Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_STAND);
  Actor_FaceDirection(22, FACING_SOUTH, 20);
  Engine_ActorStartRepeatedMotion(22, 2);
  Engine_EventWait(20);
  Engine_EventSetMessage(MsgValeNotSneakingUpMtAleph);
  Event_AskYesNo(22, 0);
  actor = Actor_Get(22);
#ifdef UBFIX
  actor->update = Vale_HaveVillagerWatchTheLeader;
#else
  actor->update =
      (void (*)(union FieldObject *))Vale_HaveVillagerWatchTheLeader;
#endif
  Actor_SetActionScript(22, gValeActor22Actions);
  GameFlag_Set(FLAG_VALE_WARNED_AGAINST_MT_ALEPH);
  Engine_EventEnd();
}

/// Repeats the warning when the villager reaches the first pause.
static void Event_RepeatMtAlephWarningFromFirstApproach(void) {
  struct FieldActor *villager = Object_GetById(22);
  if (Engine_GameFlagIsSet(FLAG_VALE_WARNED_AGAINST_MT_ALEPH) != 0) {
    if (villager->timer == 1) {
      Vale_AskAboutSneakingToAleph(gValePairTableA, gValePairTableB);
    }
  }
}

/// Asks about the Aleph detour, then resumes each actor's response actions.
static void Vale_AskAboutSneakingToAleph(const s32 *leaderActions,
                                         const s32 *villagerActions) {
  const s32 *replyActions;
  struct FieldActor *villager;
  struct FieldActor *actor;

  replyActions = villagerActions;
  villager = Object_GetById(22);
  Engine_EventBegin();
  Engine_ActorStartRepeatedMotion(22, 2);
  Actor_ShowEmote(22, EMOTE_IN_FRONT | 0, 20);
  Engine_ActorStartRepeatedMotion(ACTOR_PARTY_LEADER, 2);
  Actor_ShowEmote(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | 2, 40);
  Actor_SetActionScript(ACTOR_PARTY_LEADER, leaderActions);
  Engine_EventWait(10);
  Actor_ShowEmote(22, EMOTE_IN_FRONT | 3, 0);
  Actor_RunActionScriptAndWait(22, replyActions);
  Object_RefreshSelectorById(0);
  Engine_EventWait(20);
  Engine_ActorRunRepeatedMotion(22, 2);
  villager->scaleX = Q16_ONE;
  villager->scaleY = Q16_ONE;
  actor = Object_GetById(ACTOR_PARTY_LEADER);
  actor->scaleX = Q16_ONE;
  actor->scaleY = Q16_ONE;
  Engine_EventSetMessage(MsgValeNotSneakingUpMtAleph);
  Event_AskYesNo(22, 0);
  actor = Actor_Get(22);
#ifdef UBFIX
  actor->update = Vale_HaveVillagerWatchTheLeader;
#else
  actor->update =
      (void (*)(union FieldObject *))Vale_HaveVillagerWatchTheLeader;
#endif
  Actor_SetActionScript(22, gValeActor22Actions);
  Engine_EventEnd();
}

/// Repeats the warning when the villager reaches the second pause.
static void Event_RepeatMtAlephWarningFromSecondApproach(void) {
  struct FieldActor *villager = Object_GetById(22);
  if (Engine_GameFlagIsSet(FLAG_VALE_WARNED_AGAINST_MT_ALEPH) != 0) {
    if (villager->timer == 2) {
      Vale_AskAboutSneakingToAleph(gValePairTableC, gValePairTableD);
    }
  }
}

/// Chooses the warning approach matching the current villager pause.
static void Event_RepeatMtAlephWarningAtEitherPause(void) {
  struct FieldActor *villager = Object_GetById(22);
  if (Engine_GameFlagIsSet(FLAG_VALE_WARNED_AGAINST_MT_ALEPH) != 0) {
    s32 pause;
    pause = villager->timer;
    if (pause == 1) {
      Vale_AskAboutSneakingToAleph(gValePairTableC, gValePairTableB);
    } else if (pause == 2) {
      Vale_AskAboutSneakingToAleph(gValePairTableC, gValePairTableD);
    }
  }
}

UNUSED static void HouseScene_RunRepairMorning(void) {
  struct FieldActor *leader;
  struct FieldActor *actor;
  u8 leaderMotionFlags;
  u8 *leaderMotion;
  s32 i;

  Engine_EventBegin();
  Camera_MoveTo(-1, -1, -1, 0);
  actor = Engine_EventGetViewCenter();
  actor->motionFlags = 0;
  Camera_MoveTo(PIXELS(383), PIXELS(160), PIXELS(877), 0);
  Engine_EventWait(1);
  Engine_MapRedraw();
  Map_CopyCellAttributes(49, 41, 7, 3, 20, 50);
  Map_CopyCellsTo(2, 102, 84, 41, 2, 1);
  Map_CopyCellsTo(1, 102, 83, 41, 1, 1);
  Map_CopyCellsTo(0, 103, 82, 42, 1, 1);
  leader = Actor_Get(ACTOR_PARTY_LEADER);
  leaderMotion = &leader->motionFlags;
  leaderMotionFlags = *leaderMotion;
  *leaderMotion = 0;
  Actor_SetPosition(ACTOR_PARTY_LEADER, PIXELS(407), PIXELS(690));
  Actor_SetPosition(ACTOR_DORA, PIXELS(392), PIXELS(896));
  Actor_SetPosition(GARET, PIXELS(298), PIXELS(736));
  Actor_SetPosition(JENNA, PIXELS(298), PIXELS(760));
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 0);
  Actor_FaceDirection(ACTOR_DORA, FACING_NORTH, 0);
  Actor_FaceDirection(GARET, FACING_WEST, 0);
  Actor_FaceDirection(JENNA, FACING_WEST, 0);
  Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_HAMMER);
  Actor_SetActionScript(ACTOR_PARTY_LEADER, gLeaderHammerAction);
  SceneActor_ResetActorRun(ACTOR_BOARD, 2, 1);
  gEventWork->startTransition = 0;
  gEventWork->transitionFrames = 32;
  Engine_EventOpenScreen();
  Actor_SetSpeed(JENNA, PIXELS(1) / 2, PIXELS(1) / 4);
  Actor_SetSpeed(GARET, PIXELS(1) / 2, PIXELS(1) / 4);
  Actor_SetActionScript(JENNA, gJennaAction);
  Actor_SetActionScript(GARET, gGaretAction);

  Actor_SetBuiltinAction(ACTOR_PARTY_LEADER, ACTOR_ACTION_IDLE);
  leader->scaleX = Q16_ONE;
  leader->scaleY = Q16_ONE;
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTHWEST + FACING_STEP, 40);
  Engine_ActorSetAnimationAndWait(ACTOR_PARTY_LEADER, ANIM_NOD);
  Engine_EventWait(10);
  Actor_SetSpeed(ACTOR_PARTY_LEADER, PIXELS(3) / 10, PIXELS(3) / 20);
  Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 404, 843);
  Engine_EventWait(10);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 30);
  Engine_ActorRunRepeatedMotion(ACTOR_PARTY_LEADER, 1);
  Engine_EventWait(20);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_WEST, 40);
  Engine_ActorRunRepeatedMotion(ACTOR_PARTY_LEADER, 2);
  Engine_EventWait(20);
  Actor_SetChildValue(ACTOR_PARTY_LEADER, 2);
  Actor_SetChildValue(ACTOR_BOARD, 2);
  Actor_SetSpeed(ACTOR_BOARD, PIXELS(3) / 10, PIXELS(3) / 20);
  Actor_MoveToAndWait(ACTOR_BOARD, 390, 832);
  Engine_EventWait(80);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 0);
  Actor_MoveToAndWait(ACTOR_BOARD, 402, 828);
  Engine_EventWait(80);
  Actor_SetChildValue(ACTOR_PARTY_LEADER, 0);
  Actor_SetChildValue(ACTOR_BOARD, 0);
  Actor_SetPosition(ACTOR_BOARD, PIXELS(390), PIXELS(842));
  Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_HAMMER);
  Actor_SetActionScript(ACTOR_PARTY_LEADER, gLeaderHammerAction);
  Engine_EventWait(200);
  Map_CopyCellsTo(7, 102, 84, 41, 2, 1);

  Actor_SetBuiltinAction(ACTOR_PARTY_LEADER, ACTOR_ACTION_IDLE);
  leader->scaleX = Q16_ONE;
  leader->scaleY = Q16_ONE;
  Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_STAND);
  Engine_EventWait(30);
  Engine_ActorSetAnimationAndWait(ACTOR_PARTY_LEADER, ANIM_NOD);
  Engine_EventWait(20);
  Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 377, 843);
  Engine_EventWait(10);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 30);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_EAST, 20);
  Actor_SetChildValue(ACTOR_PARTY_LEADER, 2);
  Actor_SetChildValue(ACTOR_BOARD, 2);
  Actor_SetSpeed(ACTOR_BOARD, PIXELS(3) / 10, PIXELS(3) / 20);
  Actor_MoveToAndWait(ACTOR_BOARD, 390, 832);
  Engine_EventWait(80);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 0);
  Actor_MoveToAndWait(ACTOR_BOARD, 377, 828);
  Engine_EventWait(80);
  Actor_SetChildValue(ACTOR_PARTY_LEADER, 0);
  Actor_SetChildValue(ACTOR_BOARD, 0);
  Actor_SetPosition(ACTOR_BOARD, 0, 0);
  Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_HAMMER);
  Actor_SetActionScript(ACTOR_PARTY_LEADER, gLeaderHammerAction);
  Engine_EventWait(200);
  Map_CopyCellsTo(6, 102, 83, 41, 1, 1);

  Actor_SetBuiltinAction(ACTOR_PARTY_LEADER, ACTOR_ACTION_IDLE);
  leader->scaleX = Q16_ONE;
  leader->scaleY = Q16_ONE;
  Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_STAND);
  Engine_EventWait(30);
  Engine_ActorSetAnimationAndWait(ACTOR_PARTY_LEADER, ANIM_NOD);
  Engine_EventWait(20);
  Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 360, 855);
  Actor_FaceDirection(ACTOR_DORA, FACING_NORTHWEST + FACING_STEP, 10);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 30);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH + FACING_STEP, 20);
  Actor_SetChildValue(ACTOR_PARTY_LEADER, 2);
  Actor_SetChildValue(ACTOR_LAST_BOARD, 2);
  Actor_SetSpeed(ACTOR_LAST_BOARD, PIXELS(3) / 10, PIXELS(3) / 20);
  Actor_MoveToAndWait(ACTOR_LAST_BOARD, 390, 832);
  Engine_EventWait(80);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 0);
  Actor_MoveToAndWait(ACTOR_LAST_BOARD, 360, 842);
  Engine_EventWait(80);
  Actor_SetChildValue(ACTOR_PARTY_LEADER, 0);
  Actor_SetChildValue(ACTOR_LAST_BOARD, 0);
  Actor_SetPosition(ACTOR_LAST_BOARD, 0, 0);
  Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_HAMMER);
  Actor_SetActionScript(ACTOR_PARTY_LEADER, gLeaderHammerAction);
  Engine_EventWait(200);
  Map_CopyCellsTo(5, 103, 82, 42, 1, 1);
  Actor_SetBuiltinAction(ACTOR_PARTY_LEADER, ACTOR_ACTION_IDLE);
  leader->scaleX = Q16_ONE;
  leader->scaleY = Q16_ONE;

  Engine_EventSetMessage(MsgValeGoodJob);
  Engine_ActorJump(ACTOR_DORA, 2, 20);
  Event_ShowMessageAndWait(ACTOR_DORA, 0, 20);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_STEP, 20);
  Vale_MoveActorAlongLowerZPath(ACTOR_DORA, 5, 6, 0);
  Actor_SetSpeed(ACTOR_DORA, PIXELS(3) / 10, PIXELS(3) / 20);
  Actor_WalkToAndWait(ACTOR_DORA, 397, 832);
  Engine_EventWait(20);
  Actor_FaceDirection(ACTOR_DORA, FACING_SOUTH, 60);
  Actor_FaceDirection(ACTOR_DORA, FACING_NORTH, 60);
  Engine_ActorSetAnimationAndWait(ACTOR_DORA, ANIM_NOD);
  Engine_ActorSetAnimationAndWait(ACTOR_DORA, ANIM_NOD);
  Engine_EventWait(10);
  Event_ShowMessageAndWait(ACTOR_DORA, 0, 20);
  Actor_WalkToAndWait(ACTOR_DORA, 372, 832);
  Engine_EventWait(20);
  Actor_FaceDirection(ACTOR_DORA, FACING_SOUTH, 40);
  Actor_FaceDirection(ACTOR_DORA, FACING_WEST, 40);
  Engine_ActorSetAnimationAndWait(ACTOR_DORA, ANIM_NOD);
  Engine_ActorSetAnimationAndWait(ACTOR_DORA, ANIM_NOD);
  Engine_EventWait(10);
  Event_ShowMessageAndWait(ACTOR_DORA, 0, 20);
  Actor_FaceDirection(ACTOR_DORA, FACING_SOUTH + FACING_STEP, 30);
  Engine_ActorSetAnimationAndWait(ACTOR_DORA, ANIM_NOD);
  Engine_EventWait(10);
  Event_ShowMessageAndWait(ACTOR_DORA, 0, 20);
  Engine_ActorRunRepeatedMotion(ACTOR_PARTY_LEADER, 2);
  Engine_EventWait(40);
  Engine_ActorSetAnimationAndWait(ACTOR_DORA, ANIM_SHAKE_HEAD);
  Engine_EventWait(20);
  Event_OpenMessage(ACTOR_DORA, 0);
  if (Engine_EventChooseYesNo(ACTOR_PARTY_LEADER, 0) == 0) {
    gEventWork->message++;
  }
  Engine_ActorSetAnimationAndWait(ACTOR_DORA, ANIM_SHAKE_HEAD);
  Engine_EventWait(20);
  Event_ShowMessageAndWait(ACTOR_DORA, 0, 20);
  Engine_EventSetMessage(MsgValeWorkingYourselvesBone);
  Actor_WalkToAndWait(ACTOR_DORA, 386, 841);
  Engine_EventWait(10);
  Actor_FaceDirection(ACTOR_DORA, FACING_NORTH + FACING_STEP, 60);
  Engine_ActorRunRepeatedMotion(ACTOR_DORA, 2);
  Engine_EventWait(20);
  Event_ShowMessageAndWait(ACTOR_DORA, 0, 20);
  Actor_FaceDirection(ACTOR_DORA, FACING_SOUTH + FACING_STEP, 30);
  Event_OpenMessage(ACTOR_DORA, 0);
  if (Engine_EventChooseYesNo(ACTOR_PARTY_LEADER, 0) == 1) {
    gEventWork->message++;
  }
  Engine_EventWait(20);
  Event_ShowMessageAndWait(ACTOR_DORA, 0, 20);
  Actor_FaceDirection(ACTOR_DORA, FACING_NORTH + FACING_STEP, 60);
  Engine_ActorRunRepeatedMotion(ACTOR_DORA, 2);
  Engine_EventWait(20);
  Engine_EventSetMessage(MsgValeDevastatedWhenKyle);
  Event_ShowMessageAndWait(ACTOR_DORA, 0, 20);
  Actor_WalkToAndWait(ACTOR_DORA, 386, 825);
  Engine_EventWait(10);
  Engine_ActorSetAnimationAndWait(ACTOR_DORA, ANIM_SHAKE_HEAD);
  Engine_EventWait(60);
  Event_ShowMessageAndWait(ACTOR_DORA, 0, 60);
  Actor_FaceDirection(ACTOR_DORA, FACING_SOUTH + FACING_STEP, 10);
  Event_ShowMessageAndWait(ACTOR_DORA, 0, 20);
  Actor_WalkToAndWait(ACTOR_DORA, 372, 832);
  Actor_FaceDirection(ACTOR_DORA, FACING_SOUTH + FACING_STEP, 20);
  Engine_ActorRunRepeatedMotion(ACTOR_PARTY_LEADER, 2);
  Engine_EventWait(20);
  Engine_ActorSetAnimationAndWait(ACTOR_DORA, ANIM_NOD);
  Engine_EventWait(20);
  Event_ShowMessageAndWait(ACTOR_DORA, 0, 20);
  Actor_ShowEmote(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | 2, 60);
  Engine_ActorRunRepeatedMotion(ACTOR_DORA, 2);
  Engine_EventWait(20);
  Event_ShowMessageAndWait(ACTOR_DORA, 0, 20);
  Engine_ActorSetAnimationAndWait(ACTOR_PARTY_LEADER, ANIM_NOD);
  Engine_EventWait(30);
  Engine_ActorSetAnimationAndWait(ACTOR_DORA, ANIM_NOD);
  Engine_EventWait(20);
  Event_ShowMessageAndWait(ACTOR_DORA, 0, 10);

  Camera_SetSpeed(PIXELS(2) / 5, PIXELS(1) / 20);
  Camera_MoveTo(PIXELS(377), PIXELS(160), PIXELS(860), 1);
  Actor_SetSpeed(JENNA, PIXELS(1), PIXELS(1) / 2);
  Actor_SetSpeed(GARET, PIXELS(1), PIXELS(1) / 2);
  Actor_WalkTo(GARET, 369, 904);
  Actor_WalkToAndWait(JENNA, 392, 904);
  Engine_ActorSetAnimation(GARET, ANIM_STAND);
  Vale_MoveActorAlongLowerZPath(JENNA, 10, 11, 0);
  Actor_FaceDirection(JENNA, FACING_NORTHWEST, 0);
  Event_ShowMessageAndWait(JENNA, 0, 10);
  Engine_ActorRunRepeatedMotion(ACTOR_DORA, 2);
  Engine_EventWait(10);
  Actor_FaceDirection(ACTOR_DORA, FACING_SOUTHEAST + FACING_STEP, 0);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_STEP, 30);
  Engine_ActorJump(JENNA, 4, 0);
  Actor_WalkToAndWait(JENNA, 392, 843);
  Actor_FaceDirection(JENNA, FACING_WEST + FACING_STEP, 0);
  Actor_FaceDirection(ACTOR_DORA, FACING_SOUTHEAST + FACING_STEP, 0);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH + FACING_STEP, 40);
  Engine_ActorSetAnimationAndWait(ACTOR_DORA, ANIM_NOD);
  Engine_EventWait(20);
  Event_ShowMessageAndWait(ACTOR_DORA, 0, 20);
  Engine_ActorSetAnimationAndWait(JENNA, ANIM_NOD);
  Engine_EventWait(20);
  Event_ShowMessageAndWait(JENNA, 0, 20);
  Engine_ActorSetAnimation(ACTOR_DORA, ANIM_NOD);
  Engine_ActorSetAnimationAndWait(ACTOR_PARTY_LEADER, ANIM_NOD);
  Engine_EventWait(20);
  Event_ShowMessageAndWait(ACTOR_DORA, 0, 20);
  Vale_MoveActorAlongLowerZPath(GARET, 10, 11, 0);
  Actor_SetSpeed(JENNA, PIXELS(3) / 10, PIXELS(3) / 20);
  Actor_SetSpeed(GARET, PIXELS(3) / 10, PIXELS(3) / 20);
  Actor_WalkTo(GARET, 392, 843);
  Actor_Get(JENNA)->steeringFlags &= ~ACTOR_FACES_MOTION;
  Actor_WalkToAndWait(JENNA, 408, 843);
  Engine_EventWait(1);
  Actor_Get(JENNA)->steeringFlags |= ACTOR_FACES_MOTION;
  Actor_FaceDirection(JENNA, FACING_WEST, 0);
  Engine_ActorWaitForMove(GARET);
  Engine_ActorSetAnimation(GARET, ANIM_STAND);
  Actor_FaceDirection(GARET, FACING_WEST, 30);
  Engine_ActorJump(ACTOR_DORA, 4, 30);
  Event_ShowMessageAndWait(ACTOR_DORA, 0, 20);
  Engine_ActorSetAnimationAndWait(GARET, ANIM_NOD);
  Engine_EventWait(30);
  Actor_FaceDirection(ACTOR_DORA, FACING_SOUTH + FACING_STEP, 30);
  Event_ShowMessageAndWait(ACTOR_DORA, 0, 20);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH + FACING_STEP, 0);
  Engine_ActorJump(ACTOR_PARTY_LEADER, 2, 30);
  Actor_SetAttachedEffect(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | 2);
  Engine_EventWait(60);
  Actor_FaceDirection(ACTOR_DORA, FACING_SOUTHEAST + FACING_STEP, 40);
  Engine_ActorRunRepeatedMotion(GARET, 2);
  Event_ShowMessageAndWait(GARET, 0, 20);
  Actor_ShowEmote(ACTOR_DORA, EMOTE_IN_FRONT | 1, 80);
  Actor_FaceDirection(ACTOR_DORA, FACING_SOUTH + FACING_STEP, 30);
  Actor_ShowEmote(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | 2, 80);
  Actor_FaceDirection(ACTOR_DORA, FACING_SOUTHEAST + FACING_STEP, 0);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH + FACING_STEP, 40);
  Event_ShowMessageAndWait(ACTOR_DORA, 0, 20);
  Engine_ActorSetAnimationAndWait(GARET, ANIM_NOD);
  Engine_EventWait(100);
  Engine_ActorFaceEachOther(JENNA, GARET, 30);
  Engine_ActorStartRepeatedMotion(GARET, 2);
  Engine_ActorRunRepeatedMotion(JENNA, 2);
  Engine_EventWait(10);
  Actor_ShowEmote(ACTOR_DORA, EMOTE_IN_FRONT | 5, 60);
  Event_ShowMessageAndWait(ACTOR_DORA, 0, 20);
  Actor_FaceDirection(JENNA, FACING_WEST, 0);
  Actor_FaceDirection(GARET, FACING_WEST, 30);
  Engine_ActorRunRepeatedMotion(JENNA, 2);
  Engine_EventWait(30);
  Event_ShowMessageAndWait(JENNA, 0, 20);
  Engine_ActorStartRepeatedMotion(ACTOR_DORA, 2);
  Engine_EventWait(30);
  Event_ShowMessageAndWait(ACTOR_DORA, 0, 20);
  Engine_ActorSetAnimation(GARET, ANIM_NOD);
  Engine_ActorSetAnimationAndWait(JENNA, ANIM_NOD);
  Engine_EventWait(30);
  Engine_ActorSetAnimationAndWait(ACTOR_DORA, ANIM_SHAKE_HEAD);
  Engine_EventWait(20);
  Event_ShowMessageAndWait(ACTOR_DORA, 0, 20);
  Engine_ActorSetAnimation(GARET, ANIM_NOD);
  Engine_ActorSetAnimationAndWait(JENNA, ANIM_NOD);
  Engine_EventWait(10);
  Engine_ActorSetAnimationAndWait(ACTOR_DORA, ANIM_NOD);
  Engine_EventWait(20);
  Event_ShowMessageAndWait(ACTOR_DORA, 0, 20);
  Engine_ActorSetAnimation(GARET, ANIM_NOD);
  Engine_ActorSetAnimationAndWait(JENNA, ANIM_NOD);
  Engine_EventWait(10);
  Engine_ActorRunRepeatedMotion(ACTOR_DORA, 2);
  Engine_EventWait(30);
  Event_ShowMessageAndWait(ACTOR_DORA, 0, 20);
  Camera_SetSpeed(PIXELS(3) / 5, PIXELS(3) / 40);
  Camera_MoveTo(PIXELS(373), PIXELS(160), PIXELS(837), 1);
  Actor_WalkToAndWait(ACTOR_DORA, 364, 816);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH + FACING_STEP, 0);
  Actor_FaceDirection(ACTOR_DORA, FACING_SOUTHEAST + FACING_STEP, 30);
  Event_ShowMessageAndWait(ACTOR_DORA, 0, 40);
  Engine_ActorFaceEachOther(JENNA, GARET, 30);
  Engine_ActorStartRepeatedMotion(GARET, 2);
  Engine_ActorRunRepeatedMotion(JENNA, 2);
  Engine_EventWait(30);
  Actor_FaceDirection(GARET, FACING_WEST, 0);
  Actor_FaceDirection(JENNA, FACING_WEST, 30);
  Actor_FaceDirection(ACTOR_DORA, FACING_SOUTH + FACING_STEP, 30);
  Event_ShowMessageAndWait(ACTOR_DORA, 0, 30);
  Actor_ShowEmote(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | 5, 60);
  Engine_ActorSetAnimationAndWait(ACTOR_DORA, ANIM_SHAKE_HEAD);
  Engine_EventWait(10);
  Event_OpenMessage(ACTOR_DORA, 0);
  if (Engine_EventChooseYesNo(ACTOR_PARTY_LEADER, 0) == 1) {
    gEventWork->message++;
  }
  Engine_EventWait(40);
  Event_ShowMessageAndWait(ACTOR_DORA, 0, 20);

  Engine_EventSetMessage(MsgValeAsStubbornAsYourFather);
  Actor_ShowEmote(ACTOR_DORA, EMOTE_IN_FRONT | 3, 0);
  Engine_ActorRunRepeatedMotion(ACTOR_DORA, 3);
  Engine_EventWait(30);
  Event_ShowMessageAndWait(ACTOR_DORA, 0, 20);
  Engine_ActorJump(ACTOR_DORA, 4, 0);
  Engine_ActorRunRepeatedMotion(ACTOR_DORA, 3);
  Engine_ActorSetAnimation(ACTOR_DORA, 7);
  Engine_EventWait(5);
  Event_ShowTwoMessagesAndWait(ACTOR_DORA, 14, 2, 24, 2, GARET, 10, 14, 4, 14,
                               0);
  actor = Actor_Get(ACTOR_DORA);
  actor->sprite->flags = 0;
  actor->steeringFlags &= ~ACTOR_FACES_MOTION;
  Actor_SetSpeed(ACTOR_DORA, PIXELS(3), PIXELS(3) / 2);
  Actor_WalkToAndWait(ACTOR_DORA, 364, 815);
  Engine_EventWait(4);
  for (i = 0; i != 4; i++) {
    actor->z.fixed += PIXELS(3) / 2;
    actor->scaleY -= 0x1999;
    Engine_EventWait(1);
  }
  Actor_SetPosition(ACTOR_DORA, 0, 0);
  Actor_SetSpeed(GARET, PIXELS(3), PIXELS(3) / 2);
  Engine_ActorJump(GARET, 6, 0);
  Actor_WalkToAndWait(GARET, 374, 827);
  Event_ShowMessage(JENNA, 0);
  Actor_FaceDirection(GARET, FACING_NORTHWEST + FACING_STEP, 0);
  Actor_ShowEmote(JENNA, EMOTE_IN_FRONT | 0, 0);
  Engine_ActorStartRepeatedMotion(JENNA, 2);
  Actor_ShowEmote(GARET, EMOTE_IN_FRONT | 0, 10);
  Engine_ActorSetAnimation(GARET, 13);
  Engine_ActorJump(GARET, 2, 5);
  Map_SetShake(0, PIXELS(4), Q16_ONE);
  Map_CopyCellsTo(1, 102, 83, 41, 1, 1);
  Engine_ActorSetSpriteFlags(Actor_Get(GARET), 0);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH + FACING_STEP, 10);
  Engine_ActorStartRepeatedMotion(GARET, 3);
  Map_SetShake(-1, -1, 9 * Q16_ONE / 10);
  Engine_MapRenderWaitForValues();
  Actor_ShowEmote(GARET, EMOTE_IN_FRONT | 2, 80);
  Engine_ActorSetAnimation(ACTOR_DORA, 8);
  actor->scaleY = 0x8000;
  Actor_SetPosition(ACTOR_DORA, PIXELS(364), PIXELS(811));
  for (i = 0; i != 5; i++) {
    actor->scaleY += 0x1999;
    Engine_EventWait(1);
  }
  Engine_EventWait(60);
  Engine_ActorRunRepeatedMotion(GARET, 2);
  Actor_FaceDirection(GARET, FACING_SOUTH + FACING_STEP, 30);
  Engine_ActorStartRepeatedMotion(GARET, 2);
  Engine_ActorRunRepeatedMotion(JENNA, 2);
  Engine_EventWait(60);
  Engine_ActorRunRepeatedMotion(ACTOR_DORA, 2);
  Engine_EventWait(20);
  Event_ShowMessageAndWait(ACTOR_DORA, 0, 20);
  Camera_SetSpeed(PIXELS(3) / 10, PIXELS(3) / 80);
  Camera_MoveTo(PIXELS(372), PIXELS(160), PIXELS(859), 1);
  Actor_SetSpeed(ACTOR_DORA, PIXELS(3), PIXELS(3) / 2);
  Engine_ActorJump(ACTOR_DORA, 6, 0);
  Actor_WalkToAndWait(ACTOR_DORA, 359, 835);
  Engine_EventWait(30);
  Actor_FaceDirection(ACTOR_DORA, FACING_SOUTH, 30);
  Engine_ActorRunRepeatedMotion(ACTOR_DORA, 2);
  Engine_EventWait(30);
  actor = Actor_Get(ACTOR_DORA);
  actor->priorityFlags &= ~ACTOR_PRIORITY_AUTOMATIC;
  Event_ShowMessageAndWait(ACTOR_DORA, 0, 80);
  Actor_ShowEmote(ACTOR_DORA, EMOTE_IN_FRONT | 1, 80);
  Actor_FaceDirection(ACTOR_DORA, FACING_EAST, 60);
  Engine_ActorRunRepeatedMotion(ACTOR_DORA, 3);
  Event_ShowMessageAndWait(ACTOR_DORA, 0, 20);
  Actor_SetAttachedEffect(ACTOR_DORA, EMOTE_IN_FRONT | 2);
  Engine_EventWait(80);
  Actor_FaceDirection(GARET, FACING_SOUTH + FACING_STEP, 30);
  Actor_ShowEmote(GARET, EMOTE_IN_FRONT | 2, 80);
  Engine_ActorRunRepeatedMotion(GARET, 2);
  Engine_EventWait(20);
  Event_ShowMessageAndWait(GARET, 0, 20);
  Actor_FaceDirection(GARET, FACING_WEST, 30);
  Engine_ActorRunRepeatedMotion(GARET, 3);
  Engine_EventWait(10);
  Engine_ActorRunRepeatedMotion(GARET, 3);
  Engine_ActorSetSpriteFlags(Actor_Get(GARET), 1);
  Engine_ActorJump(GARET, 6, 0);
  Engine_ActorSetAnimation(GARET, ANIM_STAND);
  Actor_SetSpeed(GARET, PIXELS(4), PIXELS(2));
  actor = Actor_Get(GARET);
  actor->steeringFlags &= ~ACTOR_FACES_MOTION;
  Actor_SetDestination(GARET, 403, 827);
  Actor_SetAttachedEffect(JENNA, EMOTE_IN_FRONT | 2);
  Actor_FaceDirection(JENNA, FACING_NORTH, 20);
  Event_ShowMessageAndWait(JENNA, 0, 1);
  Engine_ActorWaitForMove(GARET);
  Actor_FaceDirection(GARET, FACING_SOUTH + FACING_STEP, 20);
  Event_ShowMessageAndWait(GARET, 0, 20);
  Actor_ShowEmote(GARET, EMOTE_IN_FRONT | 0, 0);
  Engine_ActorSetAnimation(GARET, 13);
  Engine_ActorJump(GARET, 2, 5);
  Engine_ActorSetSpriteFlags(Actor_Get(GARET), 0);
  Map_CopyCellsTo(2, 102, 84, 41, 2, 1);
  Map_SetShake(0, PIXELS(4), Q16_ONE);
  Engine_ActorRunRepeatedMotion(GARET, 3);
  Map_SetShake(-1, -1, 9 * Q16_ONE / 10);
  Engine_MapRenderWaitForValues();
  Actor_ShowEmote(GARET, EMOTE_IN_FRONT | 2, 30);
  Actor_SetSpeed(JENNA, PIXELS(3) / 10, PIXELS(3) / 20);
  Actor_WalkToAndWait(JENNA, 408, 855);
  Engine_EventWait(60);
  Engine_ActorRunRepeatedMotion(GARET, 2);
  Actor_ShowEmote(ACTOR_DORA, EMOTE_IN_FRONT | 5, 60);
  Engine_ActorStartRepeatedMotion(JENNA, 3);
  Engine_ActorRunRepeatedMotion(ACTOR_PARTY_LEADER, 3);
  Engine_EventWait(80);
  Actor_FaceDirection(GARET, FACING_SOUTH, 30);
  Engine_ActorRunRepeatedMotion(GARET, 3);
  Engine_EventWait(10);
  Engine_ActorSetAnimationAndWait(JENNA, ANIM_SHAKE_HEAD);
  Engine_EventWait(80);
  Engine_ActorSetAnimationAndWait(ACTOR_DORA, ANIM_NOD);
  Engine_EventWait(10);
  Event_ShowMessageAndWait(ACTOR_DORA, 0, 20);
  Actor_FaceDirection(JENNA, FACING_NORTHWEST + FACING_STEP, 0);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 0);
  Actor_FaceDirection(ACTOR_DORA, FACING_EAST, 60);
  Actor_FaceDirection(ACTOR_DORA, FACING_SOUTH, 60);
  Engine_ActorSetAnimationAndWait(ACTOR_DORA, ANIM_SHAKE_HEAD);
  Engine_EventWait(60);
  Actor_FaceDirection(ACTOR_DORA, FACING_EAST, 80);
  Actor_ShowEmote(ACTOR_DORA, EMOTE_IN_FRONT | 5, 80);
  Event_ShowMessageAndWait(ACTOR_DORA, 0, 60);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 0);
  Actor_ShowEmote(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | 1, 0);
  Actor_ShowEmote(JENNA, EMOTE_IN_FRONT | 1, 0);
  Actor_ShowEmote(GARET, EMOTE_IN_FRONT | 1, 60);
  Engine_ActorSetAnimationAndWait(ACTOR_DORA, ANIM_SHAKE_HEAD);
  Engine_EventWait(30);
  Event_ShowMessageAndWait(ACTOR_DORA, 0, 70);
  Engine_ActorStartRepeatedMotion(GARET, 2);
  Engine_ActorRunRepeatedMotion(JENNA, 2);
  Engine_EventWait(20);
  Actor_FaceDirection(JENNA, FACING_WEST, 60);
  Actor_FaceDirection(ACTOR_DORA, FACING_SOUTH, 30);
  Event_ShowMessageAndWait(ACTOR_DORA, 0, 30);
  Engine_ActorSetAnimationAndWait(JENNA, ANIM_NOD);
  Engine_EventWait(10);
  Event_ShowMessageAndWait(JENNA, 0, 20);
  Actor_FaceDirection(ACTOR_DORA, FACING_EAST, 30);
  Engine_ActorSetAnimationAndWait(ACTOR_DORA, ANIM_SHAKE_HEAD);
  Engine_EventWait(20);
  Event_ShowMessageAndWait(ACTOR_DORA, 0, 20);
  Engine_ActorSetAnimationAndWait(JENNA, ANIM_NOD);
  Engine_EventWait(20);
  Engine_ActorRunRepeatedMotion(GARET, 3);
  Engine_EventWait(10);
  Event_ShowMessageAndWait(GARET, 0, 20);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH + FACING_STEP, 0);
  Actor_ShowEmote(ACTOR_DORA, EMOTE_IN_FRONT | 0, 0);
  Engine_ActorRunRepeatedMotion(ACTOR_DORA, 3);
  Engine_EventWait(30);
  Event_ShowMessageAndWait(ACTOR_DORA, 0, 60);
  Engine_ActorRunRepeatedMotion(GARET, 3);
  Actor_SetSpeed(GARET, PIXELS(1), PIXELS(1) / 2);
  Engine_ActorSetSpriteFlags(Actor_Get(GARET), 0);
  Engine_ActorJump(GARET, 4, 0);
  Actor_WalkToAndWait(GARET, 398, 828);
  Engine_EventWait(60);
  Actor_FaceDirection(ACTOR_DORA, FACING_SOUTH, 0);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 60);
  Engine_ActorSetAnimationAndWait(ACTOR_PARTY_LEADER, ANIM_NOD);
  Engine_EventWait(60);
  Engine_ActorSetAnimationAndWait(ACTOR_DORA, ANIM_NOD);
  Engine_EventWait(60);

  actor = Actor_Get(GARET);
  actor->steeringFlags |= ACTOR_FACES_MOTION;
  actor = Actor_Get(JENNA);
  actor->steeringFlags |= ACTOR_FACES_MOTION;
  actor = Actor_Get(ACTOR_PARTY_LEADER);
  Actor_SetSpeed(GARET, PIXELS(1), PIXELS(1) / 2);
  Actor_SetSpeed(JENNA, PIXELS(1), PIXELS(1) / 2);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_EAST, 0);
  Actor_WalkTo(JENNA, actor->x.part.pixel + 16, actor->z.part.pixel);
  Actor_WalkToAndWait(GARET, actor->x.part.pixel + 16,
                      actor->z.part.pixel - 16);
  Engine_ActorWaitForMove(GARET);
  Actor_FaceDirection(GARET, FACING_SOUTH + FACING_STEP, 30);
  Engine_ActorSetAnimation(GARET, ANIM_NOD);
  Engine_ActorSetAnimation(JENNA, ANIM_NOD);
  Engine_ActorSetAnimationAndWait(ACTOR_PARTY_LEADER, ANIM_NOD);
  Engine_EventWait(40);
  Actor_WalkToAndWait(JENNA, actor->x.part.pixel, actor->z.part.pixel);
  Actor_SetPosition(JENNA, 0, 0);
  Actor_WalkToAndWait(GARET, actor->x.part.pixel, actor->z.part.pixel);
  Actor_SetPosition(GARET, 0, 0);
  Engine_PartyAddMembers(GARET, JENNA);
  Camera_MoveTo(PIXELS(377), PIXELS(160), PIXELS(887), 1);
  Vale_MoveActorAlongHigherZPath(ACTOR_PARTY_LEADER, 13, 10, 0);
  Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 376, 912);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 0);
  Actor_Get(ACTOR_DORA)->steeringFlags |= ACTOR_FACES_MOTION;
  Vale_MoveActorAlongHigherZPath(ACTOR_DORA, 6, 5, 0);
  Actor_WalkToAndWait(ACTOR_DORA, 373, 887);
  Actor_FaceDirection(ACTOR_DORA, FACING_SOUTH, 0);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 40);
  Engine_ActorSetAnimation(ACTOR_DORA, ANIM_NOD);
  Engine_ActorSetAnimationAndWait(ACTOR_PARTY_LEADER, ANIM_NOD);
  Engine_EventWait(20);
  Engine_CameraFollowActor(ACTOR_PARTY_LEADER, 1);
  Engine_CameraWaitForMove();
  Engine_EventWait(100);
  GameFlag_Set(FLAG_SCENE_TEMP_02);
  GameFlag_Clear(FLAG_SHOW_LOCATION_NAME);
  *leaderMotion = leaderMotionFlags;
  Engine_EventEnd();
}

/// Vale's setup: plays the roof mending (entrance 16) or its end (entrance
/// 17, once), floats the Nut, places the object by its column, sets the
/// watching villager going, removes the puppies once Saturos and Menardi have
/// been met, and after a battle restores the passage's and the upper path's
/// cells.
s32 Vale_Initialize(void) {
  struct FieldActor *actor;
  s32 set;

  if (gGameState.entrance == VALE_FROM_TITLE) {
    PaletteGlow_Update(((u8 *)&gGameState)[517], ((u8 *)&gGameState)[518]);
    Event_MendingTheRoof();
  } else {
    if (!Engine_GameFlagIsSet(0xfd0)) {
      if (!Engine_GameFlagIsSet(FLAG_REACHED_KALAY)) {
        Scene_FloatNut(26);
      } else {
        Scene_FloatNut(20);
      }
    }
    Map_CopyCellsTo(2, 102, 84, 41, 2, 1);
    Map_CopyCellsTo(1, 102, 83, 41, 1, 1);
    actor =
        Object_GetById((Engine_GameFlagIsSet(FLAG_REACHED_KALAY) != 0) + 20);
    Engine_ActorSetSpriteFlags(actor, 0);
    if (Engine_GameFlagIsSet(0x314)) {
      actor->x.fixed = 181 << 17;
    } else if (Engine_GameFlagIsSet(0x316)) {
      actor->x.fixed = 197 << 17;
    } else {
      actor->x.fixed = 189 << 17;
    }
    actor->z.fixed = PIXELS(584);
    actor->y.fixed = PIXELS(192);
    Event_SettleMovedObject();
    actor->terrainId = 3;
    actor->motionFlags = 0;
    FieldScheduler_AddCallback(Vale_AdjustVillagerPriorityForLeaderHeight,
                               0xc80);
#if TBS_PATCH_4
    if (Engine_GameFlagIsSet(FLAG_REACHED_KALAY)) {
      if (Engine_GameFlagIsSet(FLAG_KEEP_PARTY_POSITION)) {
        if (Engine_GameFlagIsSet(FLAG_SCENE_TEMP_04)) {
          Map_CopyCellAttributes(49, 53, 8, 4, 20, 50);
        }
        if (Engine_GameFlagIsSet(FLAG_SCENE_TEMP_10)) {
          Event_SetPathCellOverride();
        }
      }
    } else {
#else
    if (!Engine_GameFlagIsSet(FLAG_REACHED_KALAY)) {
#endif
      if (GameFlag_IsSet(FLAG_PARTY_LEFT_VALE)) {
        actor = Object_GetById(21);
        Engine_ActorSetSpriteFlags(Object_GetById(21), 0);
        actor->scaleX = 0x28f;
        actor->scaleY = 0x28f;
      }
      if (Engine_GameFlagIsSet(FLAG_MET_SATUROS_AND_MENARDI)) {
        Engine_ActorSetPosition(15, 0, 0);
        Engine_ActorSetPosition(16, 0, 0);
        Engine_ActorSetPosition(17, 0, 0);
      }
      set = Engine_GameFlagIsSet(FLAG_PARTY_LEFT_VALE);
      if (set == 0) {
        if (!Engine_GameFlagIsSet(FLAG_KEEP_PARTY_POSITION)) {
          if (Engine_GameFlagIsSet(FLAG_VALE_WARNED_AGAINST_MT_ALEPH)) {
            Actor_SetPosition(22, PIXELS(256), PIXELS(456));
#ifdef UBFIX
            Object_GetById(22)->update = Vale_HaveVillagerWatchTheLeader;
#else
            Object_GetById(22)->update =
                (void *)Vale_HaveVillagerWatchTheLeader;
#endif
            Actor_SetActionScript(22, gValeActor22Actions);
          }
        } else {
          Object_GetById(22)->paused = set;
          Engine_GameFlagClear(FLAG_SCENE_TEMP_41);
        }
        if (gGameState.entrance != VALE_FROM_TITLE &&
            !Engine_GameFlagIsSet(FLAG_REACHED_KALAY)) {
          FieldScheduler_AddCallback(Vale_SynchronizeAlephVillagerPause, 0xc80);
        }
      }
      if (!GameFlag_IsSet(0x308) && gGameState.entrance == 17) {
        Event_RoofMended();
        Engine_GameFlagSet(0x308);
      }
#if TBS_PATCH_4
      if (Engine_GameFlagIsSet(FLAG_KEEP_PARTY_POSITION)) {
        if (Engine_GameFlagIsSet(FLAG_SCENE_TEMP_04)) {
          Map_CopyCellAttributes(49, 53, 8, 4, 20, 50);
        }
        if (Engine_GameFlagIsSet(FLAG_SCENE_TEMP_10)) {
          Event_SetPathCellOverride();
        }
      }
      {
        s32 priority;
        s32 actor;

        priority = ((u32)(u8)set >> 8) + 2;
        actor = ACTOR_DORA;
        Engine_ActorSetSpritePriority(actor, priority);
      }
#endif
    }
#if !TBS_PATCH_4
    if (Engine_GameFlagIsSet(FLAG_KEEP_PARTY_POSITION)) {
      if (Engine_GameFlagIsSet(FLAG_SCENE_TEMP_04)) {
        Map_CopyCellAttributes(49, 53, 8, 4, 20, 50);
      }
      if (Engine_GameFlagIsSet(FLAG_SCENE_TEMP_10)) {
        Event_SetPathCellOverride();
      }
    }
#endif
    BattleFx_SetQueuedSoundAndPlay(SE_170);
    Engine_MapRedraw();
    Engine_TaskWait(1);
  }
  return 0;
}

/// Back from Kalay, Garet meets his sister in Vale (actor 8); sets group flag
/// 0x303. "Ah! It's been a while... You're back?" ("Sis...").
static void Event_GaretMeetsHisSister(void) {
  struct FieldActor *record;
  s32 msg;

  Engine_EventBegin();
  Actor_SetChildValue(ACTOR_PARTY_LEADER, 0);
  Actor_FaceActor(8, ACTOR_PARTY_LEADER, 20);
  msg = MsgValeAh;
  Engine_EventSetMessage(msg);
  Engine_ActorStartRepeatedMotion(8, 2);
  Event_ShowMessageAndWait(8, 0, 20);
  Camera_SetSpeed(PIXELS(1), PIXELS(1) / 8);
  Camera_MoveTo(PIXELS(398), -1, PIXELS(582), 1);
  Actor_SetSpeed(ACTOR_PARTY_LEADER, PIXELS(4) / 5, PIXELS(2) / 5);
  Actor_SetSpeed(GARET, PIXELS(4) / 5, PIXELS(2) / 5);
  Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 420, 608);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTHWEST, 0);
  Actor_FaceDirection(8, FACING_SOUTHEAST + FACING_STEP, 0);
  record = Object_GetById(0);
  if (record != 0) {
    Actor_SetPosition(GARET, record->x.fixed, record->z.fixed);
  }
  Actor_WalkToAndWait(GARET, 402, 608);
  Actor_FaceDirection(GARET, FACING_NORTH + FACING_STEP, 20);
  Event_ShowMessage(SPEAKER_PORTRAIT_LEFT | 1, 0);
  Actor_FaceDirection(8, FACING_SOUTH + FACING_STEP, 20);
  Engine_ActorSetAnimationAndWait(8, ANIM_NOD);
  Event_ShowMessage(SPEAKER_WINDOW_ABOVE | 8, 0);
  Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_NOD);
  Engine_ActorSetAnimationAndWait(GARET, ANIM_NOD);
  Engine_EventWait(20);
  Engine_ActorRunRepeatedMotion(8, 2);
  Event_OpenMessage(SPEAKER_WINDOW_ABOVE | 8, 0);
  if (Engine_EventChooseYesNo(0, 0) == 1) {
    gEventWork->message++;
    Engine_ActorStartRepeatedMotion(8, 1);
  }
  Event_ShowMessageAndWait(SPEAKER_WINDOW_ABOVE | 8, 0, 40);
  Actor_ShowEmote(8, EMOTE_IN_FRONT | 5, 60);
  Engine_EventSetMessage(msg + 6);
  Event_ShowMessageAndWait(SPEAKER_WINDOW_ABOVE | 8, 0, 20);
  Engine_ActorRunRepeatedMotion(GARET, 1);
  Engine_EventWait(40);
  Event_ShowMessageAndWait(SPEAKER_PORTRAIT_LEFT | 1, 0, 40);
  Engine_ActorRunRepeatedMotion(8, 1);
  Actor_FaceDirection(8, FACING_NORTH + FACING_STEP, 20);
  Event_ShowMessage(SPEAKER_WINDOW_ABOVE | 8, 0);
  Engine_ActorSetAnimationAndWait(GARET, ANIM_NOD);
  Event_ShowMessageAndWait(SPEAKER_PORTRAIT_LEFT | 1, 0, 120);
  Event_ShowMessageAndWait(SPEAKER_WINDOW_ABOVE | 8, 0, 20);
  Actor_ShowEmote(GARET, EMOTE_IN_FRONT | 5, 40);
  Event_ShowMessageAndWait(SPEAKER_PORTRAIT_LEFT | 1, 0, 40);
  Engine_ActorSetAnimationAndWait(8, ANIM_SHAKE_HEAD);
  Event_ShowMessageAndWait(SPEAKER_WINDOW_ABOVE | 8, 0, 20);
  Engine_ActorSetAnimationAndWait(GARET, ANIM_NOD);
  Engine_EventWait(40);
  Actor_FaceDirection(8, FACING_SOUTH + FACING_STEP, 20);
  Event_ShowMessageAndWait(SPEAKER_WINDOW_ABOVE | 8, 0, 10);
  Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_NOD);
  Engine_ActorSetAnimationAndWait(GARET, ANIM_NOD);
  Engine_EventWait(20);
  Engine_ActorSetAnimationAndWait(8, ANIM_NOD);
  Engine_ActorSetAnimation(GARET, ANIM_WALK);
  record = Object_GetById(0);
  if (record != 0) {
    Actor_SetDestination(GARET, record->x.part.pixel, record->z.part.pixel);
  }
  Engine_ActorWaitForMove(GARET);
  Actor_SetPosition(GARET, 0, 0);
  GameFlag_Set(0x303);
  Engine_EventEnd();
}

/// Back from Kalay, Garet asks one of his family in Vale (actor 9) to protect
/// it while he is gone; sets group flag 0x304. "WHATEVER YOU DO, you must
/// protect the family while I'm gone."
static void Event_GaretAsksToProtectFamily(void) {
  struct FieldActor *record;

  Engine_EventBegin();
  Camera_MoveTo(PIXELS(357), -1, PIXELS(738), 1);
  Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 367, 745);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTHWEST, 0);
  record = Object_GetById(0);
  if (record != 0) {
    Actor_SetPosition(GARET, record->x.fixed, record->z.fixed);
  }
  Actor_WalkToAndWait(GARET, 346, 745);
  Actor_FaceDirection(GARET, FACING_NORTH + FACING_STEP, 20);
  Engine_EventSetMessage(MsgValeHowHaveYouBeen);
  Event_ShowMessage(GARET, 0);
  Engine_ActorRunRepeatedMotion(9, 2);
  Actor_ShowEmote(9, EMOTE_IN_FRONT | 0, 0);
  Actor_FaceDirection(9, FACING_SOUTHEAST + FACING_STEP, 10);
  Actor_FaceDirection(9, FACING_SOUTH + FACING_STEP, 10);
  Actor_FaceDirection(9, FACING_SOUTHEAST + FACING_STEP, 40);
  Event_ShowMessageAndWait(9, 0, 20);
  Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_NOD);
  Engine_ActorSetAnimationAndWait(GARET, ANIM_NOD);
  Engine_ActorRunRepeatedMotion(9, 1);
  Actor_FaceDirection(9, FACING_SOUTH + FACING_STEP, 10);
  Event_ShowMessageAndWait(9, 0, 20);
  Engine_ActorStartRepeatedMotion(GARET, 1);
  Actor_ShowEmote(GARET, EMOTE_IN_FRONT | 3, 40);
  Event_ShowMessageAndWait(GARET, 0, 10);
  Engine_ActorSetAnimationAndWait(9, ANIM_NOD);
  Event_ShowMessageAndWait(9, 0, 20);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_WEST, 0);
  Actor_FaceDirection(GARET, FACING_EAST + FACING_STEP, 40);
  Engine_ActorSetAnimationAndWait(9, ANIM_SHAKE_HEAD);
  Event_ShowMessage(9, 0);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTHWEST + FACING_STEP, 0);
  Actor_FaceDirection(GARET, FACING_NORTH + FACING_STEP, 10);
  Engine_ActorRunRepeatedMotion(GARET, 2);
  Engine_EventWait(20);
  Event_ShowMessageAndWait(GARET, 0, 10);
  Engine_ActorSetAnimationAndWait(9, ANIM_NOD);
  Event_ShowMessageAndWait(9, 0, 20);
  Actor_ShowEmote(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | 2, 0);
  Actor_ShowEmote(GARET, EMOTE_IN_FRONT | 2, 80);
  Event_ShowMessageAndWait(GARET, 0, 20);
  Engine_ActorRunRepeatedMotion(9, 2);
  Event_ShowMessageAndWait(9, 0, 20);
  Actor_FaceDirection(GARET, FACING_EAST + FACING_STEP, 20);
  Event_OpenMessage(GARET, 0);
  if (Engine_EventChooseYesNo(0, 0) == 0) {
    Actor_ShowEmote(GARET, EMOTE_IN_FRONT | 5, 60);
  } else {
    gEventWork->message++;
  }
  Event_ShowMessageAndWait(GARET, 0, 20);
  Actor_FaceDirection(GARET, FACING_NORTH + FACING_STEP, 10);
  Engine_EventSetMessage(MsgValeEverProtectFamily);
  Event_ShowMessage(GARET, 0);
  Engine_ActorSetAnimationAndWait(9, ANIM_NOD);
  Event_ShowMessageAndWait(9, 0, 20);
  Engine_ActorSetAnimationAndWait(GARET, ANIM_NOD);
  Engine_EventWait(20);
  Actor_FaceDirection(GARET, FACING_EAST + FACING_STEP, 20);
  Engine_ActorSetAnimationAndWait(GARET, ANIM_NOD);
  Engine_ActorSetAnimationAndWait(ACTOR_PARTY_LEADER, ANIM_NOD);
  Engine_ActorSetAnimation(GARET, ANIM_WALK);
  record = Object_GetById(0);
  if (record != 0) {
    Actor_SetDestination(GARET, record->x.part.pixel, record->z.part.pixel);
  }
  Engine_ActorWaitForMove(GARET);
  Actor_SetPosition(GARET, 0, 0);
  GameFlag_Set(0x304);
  Engine_EventEnd();
}

UNUSED static void FieldScene_RunCompanionActorSequence(void) {
  struct FieldActor *actor;
  const s32 *actions;

  Engine_EventBegin();
  actor = Actor_Get(ACTOR_A);
  Engine_ActorSetSpriteFlags(actor, 0);
  actor = Actor_Get(ACTOR_B);
  Engine_ActorSetSpriteFlags(actor, 0);
  actor = Actor_Get(ACTOR_C);
  Engine_ActorSetSpriteFlags(actor, 0);
  Engine_ActorSetAnimation(ACTOR_A, 0);
  Engine_ActorSetAnimation(ACTOR_B, 0);
  Engine_ActorSetAnimation(ACTOR_C, 0);
  Engine_TaskWait(20);
  Map_SetShake(PIXELS(2), PIXELS(2), Q16_ONE);
  actions = gVillagerAction;
  Actor_SetActionScript(ACTOR_A, actions);
  Engine_TaskWait(10);
  Actor_SetActionScript(ACTOR_B, actions);
  Map_SetShake(-1, -1, 9 * Q16_ONE / 10);
  Engine_TaskWait(20);
  Actor_RunActionScriptAndWait(ACTOR_C, actions);
  Actor_ShowEmote(ACTOR_D, EMOTE_IN_FRONT | 0, 40);
  Engine_ActorRunRepeatedMotion(ACTOR_D, 2);
  Actor_FaceDirection(ACTOR_D, FACING_NORTH + FACING_STEP, 10);
  Engine_EventSetMessage(MsgValeIUsedToPlayHere);
  Event_ShowMessageAndWait(ACTOR_D, 0, 40);
  Actor_FaceActor(ACTOR_D, ACTOR_PARTY_LEADER, 20);
  Event_ShowMessage(ACTOR_D, 0);
  Actor_FaceDirection(ACTOR_D, FACING_WEST, 10);
  GameFlag_Set(0x305);
  Engine_EventEnd();
}

/// Traverses the passage toward lower Z, changing animations at its lip.
static void Vale_MoveActorAlongLowerZPath(s32 actorId, s32 approachAnimationId,
                                          s32 departureAnimationId,
                                          s32 resumeMotion) {
  s32 departureAnimation;
  s32 approachAnimation;
  s32 restoreMotion;
  struct FieldActor *actor;
  struct AnimationObject *sprite;

  restoreMotion = resumeMotion;
  approachAnimation = approachAnimationId;
  departureAnimation = departureAnimationId;
  actor = Object_GetById(actorId);
  sprite = actor->sprite;
  Actor_SetSpeed(actorId, PIXELS(1), PIXELS(1) / 2);
  Actor_WalkToAndWait(actorId, 392, 886);
  Actor_FaceDirection(0, FACING_NORTH, 10);
  actor->motionFlags = 0;
  sprite->flags = 0;
  Engine_ActorSetAnimation(actorId, approachAnimation);
  Actor_SetSpeed(actorId, PIXELS(3) / 10, PIXELS(3) / 20);
  Actor_MoveToAndWait(actorId, 392, 875);
  Engine_EventWait(10);
  Engine_ActorSetAnimation(actorId, departureAnimation);
  Actor_SetSpeed(actorId, PIXELS(1), PIXELS(1) / 2);
  Actor_MoveToAndWait(actorId, 392, 859);
  sprite->flags = 1;
  if (restoreMotion != 0) {
    actor->motionFlags = 3;
  }
  Engine_EventWait(10);
  Engine_ActorSetAnimation(actorId, ANIM_STAND);
}

/// Traverses the passage toward higher Z and restores falling motion if
/// requested.
static void Vale_MoveActorAlongHigherZPath(s32 actorId, s32 approachAnimationId,
                                           s32 departureAnimationId,
                                           s32 resumeMotion) {
  s32 departureAnimation;
  s32 approachAnimation;
  s32 restoreMotion;
  struct FieldActor *actor;
  struct AnimationObject *sprite;

  restoreMotion = resumeMotion;
  approachAnimation = approachAnimationId;
  departureAnimation = departureAnimationId;
  actor = Object_GetById(actorId);
  sprite = actor->sprite;
  Actor_SetSpeed(actorId, PIXELS(1), PIXELS(1) / 2);
  Actor_WalkToAndWait(actorId, 392, 859);
  Actor_FaceDirection(actorId, FACING_NORTH, 10);
  actor->motionFlags = 0;
  sprite->flags = 0;
  Engine_ActorSetAnimation(actorId, approachAnimation);
  Actor_SetSpeed(actorId, PIXELS(1), PIXELS(1) / 2);
  Actor_MoveToAndWait(actorId, 392, 875);
  Engine_EventWait(10);
  Actor_SetSpeed(actorId, PIXELS(3) / 10, PIXELS(3) / 20);
  Engine_ActorSetAnimation(actorId, departureAnimation);
  Actor_MoveToAndWait(actorId, 392, 890);
  actor->velocityY = 0x20000;
  sprite->flags = 1;
  if (restoreMotion != 0) {
    actor->motionFlags = 3;
  }
  Engine_ActorSetAnimation(actorId, ANIM_STAND);
}

/// Resets consecutive repair actors to their staging position or hides them.
static void SceneActor_ResetActorRun(s32 first, u32 count, s32 mode) {
  s32 selector = first;
  u32 i;

  if (mode == 0) {
    for (i = 0; i < count; i++) {
      struct FieldActor *actor = Object_GetById(selector);

      actor->motionFlags = 0;
      Engine_ActorSetSpriteFlags(actor, 0);
      actor->x.fixed = PIXELS(390);
      actor->y.fixed = PIXELS(160);
      actor->z.fixed = PIXELS(842);
      selector++;
    }
    return;
  }

  for (i = 0; i < count; i++) {
    Engine_ActorSetPosition(selector, 0, 0);
    selector++;
  }
}

/// Arriving by entrance 16, the game's opening in Vale: Isaac is mending the
/// roof of his house, Garet trains his Psynergy, and Jenna comes by; the two
/// speak of the storm three years ago before going to fetch Isaac. "Three
/// years later... Isaac! Haven't you finished mending the roof yet?"
static void Event_MendingTheRoof(void) {
  u32 i;
  /* FAKEMATCH(dead): repairActor's zero initializer, never read, keeps the
   * scene's register allocation; without it, 59 of 1024 instructions differ
   * in tbs-en. */
  struct FieldActor *repairActor = NULL;
  struct FieldActor *leader;
  struct FieldActor *dustEmitter;
  struct FieldActor *actor;
  /* FAKEMATCH(temporary): one zero for the dust emitter's flags and update and
   * the message call; as literal zeros, 7 of 1024 instructions differ in
   * tbs-en. */
  s32 stoppedMotionFlags;
  s32 message;

  leader = Object_GetById(ACTOR_PARTY_LEADER);
  dustEmitter = Object_GetById(14);

  // Prepare the damaged house and start Isaac's hammering behind the caption.
  Engine_EventBegin();
  Camera_MoveTo(-1, -1, -1, 0);
  Engine_TaskWait(1);
  ((struct FieldActor *)Engine_EventGetViewCenter())->motionFlags = 0;
  Engine_TaskWait(1);
  Map_CopyCellAttributes(49, 53, 8, 4, 20, 50);
  Map_CopyCellsTo(2, 102, 84, 41, 2, 1);
  Map_CopyCellsTo(1, 102, 83, 41, 1, 1);
  Map_CopyCellsTo(0, 103, 82, 42, 1, 1);
  repairActor = Object_GetById(11);
  repairActor->motionFlags = 0;
  repairActor->x.fixed = PIXELS(388);
  repairActor->y.fixed = PIXELS(160);
  repairActor->z.fixed = PIXELS(840);
  Engine_ActorSetSpriteFlags(repairActor, 0);
  repairActor = Object_GetById(12);
  repairActor->motionFlags = 0;
  repairActor->y.fixed = PIXELS(160);
  repairActor->z.fixed = PIXELS(844);
  repairActor->x.fixed = PIXELS(388);
  Engine_ActorSetSpriteFlags(repairActor, 0);
  repairActor = Object_GetById(13);
  repairActor->motionFlags = 0;
  repairActor->z.fixed = PIXELS(848);
  repairActor->y.fixed = PIXELS(160);
  repairActor->x.fixed = PIXELS(388);
  Engine_ActorSetSpriteFlags(repairActor, 0);
  actor = Object_GetById(11);
  Engine_ActorSetSpriteFlags(actor, 0);
  actor = Object_GetById(12);
  Engine_ActorSetSpriteFlags(actor, 0);
  Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, 11);
  Actor_SetActionScript(ACTOR_PARTY_LEADER, gLeaderHammerAction);
  ObjectSystem_SuspendProjectionAndShowSprites();
  message = MsgValeRepairCaption;
  UiText_ShowCenteredMessage(message, 0, 0);
  ObjectSystem_ResumeProjectionAndHideLayers();

  // Follow Jenna through the village until she notices the repair work.
  Camera_MoveTo(PIXELS(339), PIXELS(160), PIXELS(1173), 0);
  Engine_MapRedraw();
  Engine_TaskWait(1);
  Camera_SetSpeed(PIXELS(33) / 100, PIXELS(33) / 800);
  Camera_MoveTo(PIXELS(296), PIXELS(160), PIXELS(921), 1);
  Actor_SetPosition(JENNA, PIXELS(409), PIXELS(1134));
  Engine_TaskWait(1);
  Actor_SetSpeed(JENNA, PIXELS(7) / 10, PIXELS(7) / 20);
  Actor_WalkTo(JENNA, 420, 1068);
  Audio_PlayCueFromEventWork();
  gEventWork->transitionFrames = 60;
  Engine_EventOpenScreen();
  Engine_ActorWaitForMove(JENNA);
  Actor_SetSpeed(JENNA, PIXELS(1), PIXELS(1) / 2);
  Actor_WalkToAndWait(JENNA, 341, 1064);
  Actor_SetSpeed(JENNA, PIXELS(4) / 5, PIXELS(2) / 5);
  Actor_WalkToAndWait(JENNA, 359, 1033);
  Actor_SetSpeed(8, PIXELS(1) / 2, PIXELS(1) / 4);
  Actor_WalkTo(8, 318, 947);
  Engine_ActorSetAnimation(8, ANIM_WALK);
  Actor_WalkToAndWait(JENNA, 412, 1033);
  Actor_WalkToAndWait(JENNA, 412, 1019);
  Actor_WalkToAndWait(JENNA, 374, 1008);
  Actor_WalkToAndWait(JENNA, 347, 955);
  Actor_WalkToAndWait(8, 318, 947);
  Engine_ActorFaceEachOther(JENNA, 8, 40);
  Engine_ActorRunRepeatedMotion(8, 2);
  Engine_ActorSetAnimationAndWait(JENNA, ANIM_NOD);
  Engine_EventWait(10);
  Actor_WalkTo(8, 379, 1017);
  Camera_SetSpeed(PIXELS(1) / 2, PIXELS(1) / 16);
  Actor_WalkToAndWait(JENNA, 333, 920);
  Actor_WalkToAndWait(JENNA, 299, 924);
  Engine_CameraWaitForMove();
  Engine_EventWait(10);
  Actor_FaceDirection(JENNA, FACING_NORTHEAST + FACING_STEP, 30);
  Engine_ActorRunRepeatedMotion(JENNA, 2);
  Engine_EventWait(20);
  Camera_SetSpeed(PIXELS(2), PIXELS(1) / 4);
  Camera_MoveTo(PIXELS(387), PIXELS(160), PIXELS(866), 1);
  Engine_CameraWaitForMove();
  Engine_ActorJump(10, 2, 20);
  Engine_EventSetMessage(message + 1);
  Event_ShowMessageAndWait(SPEAKER_PORTRAIT_LEFT | 10, 0, 10);
  leader->scaleX = Q16_ONE;
  leader->scaleY = Q16_ONE;
  Actor_SetBuiltinAction(ACTOR_PARTY_LEADER, ACTOR_ACTION_IDLE);
  Engine_ActorFaceEachOther(10, ACTOR_PARTY_LEADER, 40);
  Actor_SetAttachedEffect(ACTOR_PARTY_LEADER, 0x102);
  Engine_ActorRunRepeatedMotion(ACTOR_PARTY_LEADER, 2);
  Engine_EventWait(40);
  Engine_ActorStartRepeatedMotion(10, 2);
  Engine_EventWait(40);
  Event_ShowMessageAndWait(SPEAKER_PORTRAIT_LEFT | 10, 0, 40);
  Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, 11);
  Actor_SetActionScript(ACTOR_PARTY_LEADER, gLeaderHammerAction);
  Engine_CameraFollowActor(JENNA, 1);
  Engine_CameraWaitForMove();
  Engine_EventWait(40);
  Engine_ActorSetAnimationAndWait(JENNA, ANIM_NOD);
  Actor_FaceDirection(JENNA, FACING_NORTH + FACING_STEP, 10);
  Actor_WalkToAndWait(JENNA, 312, 759);
  Actor_WalkToAndWait(JENNA, 361, 760);
  Engine_EventWait(20);
  Actor_FaceDirection(JENNA, FACING_WEST, 40);
  Actor_FaceDirection(JENNA, FACING_EAST, 40);
  Event_ShowMessageAndWait(0x6001, 0, 10);
  Actor_ShowEmote(JENNA, EMOTE_IN_FRONT | 0, 0);
  Engine_ActorJump(JENNA, 4, 40);
  Actor_FaceDirection(JENNA, FACING_NORTH, 30);
  Engine_ActorRunRepeatedMotion(JENNA, 2);
  Engine_EventWait(40);
  Event_ShowMessageAndWait(JENNA, 0, 30);
  Engine_ActorSetAnimationAndWait(JENNA, ANIM_NOD);
  Engine_EventWait(10);
  Camera_SetSpeed(PIXELS(4), PIXELS(1) / 2);
  Camera_MoveTo(PIXELS(396), -1, PIXELS(588), 1);
  Actor_WalkToAndWait(JENNA, 456, 739);
  Engine_CameraWaitForMove();
  Engine_EventWait(10);
  Actor_ShowEmote(GARET, EMOTE_IN_FRONT | 5, 40);
  Engine_ActorRunRepeatedMotion(GARET, 2);
  Engine_EventWait(20);

  // Garet's repair effect trails dust across the house.
  Scene_BeginCutscenePsynergy();
  Engine_ActorSetAnimation(GARET, 17);
  Event_ShowMessageAndWait(SPEAKER_PORTRAIT_RIGHT | 1, 0, 20);
  Event_PlayCue(SE_131);
  for (i = 0; i < 60; i++) {
    Vale_FlashActorAndSpawnEffects(Object_GetById(GARET));
    Engine_TaskWait(1);
  }
  Engine_ActorSetSpritePriority(GARET, 1);
  Task_AddCallback(Vale_UpdateGaretPaletteAndEffects, 0xc80);
  Task_AddCallback(Vale_CycleVillagerNinePalette, 0xc80);
  actor = Object_GetById(14);
  Engine_ActorSetSpriteFlags(actor, 0);
  stoppedMotionFlags = 0;
  dustEmitter->motionFlags = stoppedMotionFlags;
  dustEmitter->x.fixed = PIXELS(428);
  dustEmitter->y.fixed = PIXELS(208);
  dustEmitter->z.fixed = PIXELS(584);
  dustEmitter->facing = 0x8000;
  dustEmitter->update = Effect_ConfigureSpawnedParticle;
  Engine_EventWait(4);
  Actor_SetSpeed(14, PIXELS(2), PIXELS(2));
  FieldObject_SetPosition(dustEmitter, PIXELS(408), PIXELS(208), PIXELS(584));
  Engine_EventWait(40);
  Actor_SetSpeed(9, PIXELS(3) / 20, PIXELS(3) / 40);
  Actor_SetSpeed(14, PIXELS(3) / 20, PIXELS(3) / 40);
  Object_GetById(9);
  Engine_ObjectSetPosition(dustEmitter, PIXELS(392), PIXELS(208), PIXELS(584));
  Actor_MoveToAndWait(9, 378, 584);
  Engine_EventWait(20);
  Event_ShowMessageAndWait(SPEAKER_PORTRAIT_RIGHT | 5, 0, 10);
  dustEmitter->update = (void (*)(union FieldObject *))stoppedMotionFlags;
  Engine_ActorSetSpritePriority(GARET, 2);
  Actor_Get(GARET)->priorityFlags |= ACTOR_PRIORITY_AUTOMATIC;
  Scheduler_RemoveCallback(Vale_UpdateGaretPaletteAndEffects);
  Scheduler_RemoveCallback(Vale_CycleVillagerNinePalette);
  Engine_TaskWait(1);
  Actor_SetChildValue(GARET, 0);
  Actor_SetChildValue(9, 0);
  Actor_FaceDirection(GARET, FACING_SOUTHEAST + FACING_STEP, 0);
  Engine_ActorSetAnimation(GARET, ANIM_STAND);
  Effect_SpawnRisingDustBurst(dustEmitter);
  Scene_EndCutscenePsynergy();

  // Finish the reunion conversation after the repair effects settle.
  Engine_EventWait(10);
  Actor_WalkToAndWait(JENNA, 424, 624);
  Engine_ActorFaceEachOther(GARET, JENNA, 60);
  Engine_ActorRunRepeatedMotion(GARET, 2);
  Engine_EventWait(10);
  Event_ShowMessageAndWait(0x6001, 0, 20);
  Engine_ActorSetAnimationAndWait(JENNA, ANIM_NOD);
  Engine_EventWait(10);
  Event_ShowMessageAndWait(JENNA, 0, 20);
  Actor_ShowEmote(GARET, EMOTE_IN_FRONT | 2, 40);
  Engine_ActorSetAnimationAndWait(JENNA, ANIM_SHAKE_HEAD);
  Engine_EventWait(10);
  Event_ShowMessageAndWait(JENNA, 0, 20);
  Actor_ShowEmote(GARET, EMOTE_IN_FRONT | 1, 80);
  Engine_ActorRunRepeatedMotion(JENNA, 1);
  Engine_EventWait(10);
  Event_ShowMessageAndWait(JENNA, 0, 20);
  Actor_ShowEmote(GARET, EMOTE_IN_FRONT | 2, 80);
  Engine_ActorSetAnimationAndWait(JENNA, ANIM_SHAKE_HEAD);
  Engine_EventWait(10);
  Event_ShowMessageAndWait(JENNA, 0, 20);
  Engine_ActorSetAnimationAndWait(GARET, ANIM_NOD);
  Engine_EventWait(10);
  Actor_WalkToAndWait(JENNA, 388, 604);
  Actor_FaceDirection(JENNA, FACING_NORTHWEST, 20);
  Engine_ActorRunRepeatedMotion(JENNA, 2);
  Engine_EventWait(10);
  Event_ShowMessageAndWait(SPEAKER_PORTRAIT_LEFT | 5, 0, 20);
  Engine_ActorSetAnimationAndWait(JENNA, ANIM_SHAKE_HEAD);
  Engine_EventWait(10);
  Actor_ShowEmote(GARET, EMOTE_IN_FRONT | 5, 40);
  Engine_ActorRunRepeatedMotion(GARET, 2);
  Engine_EventWait(10);
  Actor_FaceDirection(JENNA, FACING_EAST, 0);
  Actor_FaceDirection(GARET, FACING_WEST, 40);
  Event_ShowTwoMessagesAndWait(1, 1, 2, 25, 2, 5, 10, 14, 4, 14,
                               stoppedMotionFlags);
  Engine_EventWait(40);
  Actor_SetAttachedEffect(JENNA, 0x102);
  Actor_SetAttachedEffect(GARET, 0x102);
  Engine_EventWait(80);
  Actor_ShowEmote(JENNA, EMOTE_IN_FRONT | 1, 40);
  Event_ShowMessageAndWait(SPEAKER_PORTRAIT_LEFT | 5, 0, 20);
  Actor_SetAttachedEffect(GARET, 0x102);
  Engine_EventWait(80);
  Engine_ActorSetAnimationAndWait(GARET, ANIM_SHAKE_HEAD);
  Engine_EventWait(10);
  Actor_ShowEmote(JENNA, EMOTE_IN_FRONT | 1, 40);
  Event_ShowMessageAndWait(SPEAKER_PORTRAIT_LEFT | 5, 0, 20);
  Engine_ActorRunRepeatedMotion(GARET, 2);
  Engine_EventWait(40);
  Engine_ActorSetAnimationAndWait(GARET, ANIM_SHAKE_HEAD);
  Engine_EventWait(20);
  Actor_ShowEmote(JENNA, EMOTE_IN_FRONT | 1, 80);
  Engine_ActorSetAnimationAndWait(JENNA, ANIM_SHAKE_HEAD);
  Engine_EventWait(10);
  Event_ShowMessageAndWait(SPEAKER_PORTRAIT_LEFT | 5, 0, 10);
  Actor_FaceDirection(JENNA, FACING_EAST + FACING_STEP, 40);
  Actor_FaceActor(JENNA, GARET, 40);
  Engine_ActorRunRepeatedMotion(GARET, 2);
  Engine_EventWait(10);
  Actor_WalkToAndWait(JENNA, 380, 620);
  Engine_EventWait(10);
  Event_ShowMessageAndWait(SPEAKER_PORTRAIT_LEFT | 5, 0, 10);
  Actor_FaceDirection(GARET, FACING_SOUTH + FACING_STEP, 30);
  Actor_SetAttachedEffect(GARET, 0x102);
  Engine_EventWait(80);
  Engine_ActorRunRepeatedMotion(GARET, 2);
  Engine_EventWait(10);
  Event_ShowMessageAndWait(0x6001, 0, 20);
  Engine_ActorSetAnimationAndWait(JENNA, ANIM_SHAKE_HEAD);
  Engine_EventWait(10);
  Actor_ShowEmote(GARET, EMOTE_IN_FRONT | 1, 80);
  Engine_ActorSetAnimationAndWait(JENNA, ANIM_SHAKE_HEAD);
  Engine_EventWait(10);
  Actor_SetSpeed(GARET, PIXELS(4) / 5, PIXELS(2) / 5);
  Actor_WalkToAndWait(GARET, 412, 604);
  Actor_FaceDirection(GARET, FACING_SOUTH + FACING_STEP, 20);
  Engine_ActorRunRepeatedMotion(JENNA, 1);
  Engine_EventWait(10);
  Event_ShowMessageAndWait(SPEAKER_PORTRAIT_LEFT | 5, 0, 20);
  Actor_ShowEmote(GARET, EMOTE_IN_FRONT | 1, 80);
  Engine_ActorJump(JENNA, 4, 30);
  Event_ShowMessageAndWait(SPEAKER_PORTRAIT_LEFT | 5, 0, 20);
  Engine_ActorRunRepeatedMotion(GARET, 3);
  Engine_EventWait(10);
  Event_ShowMessageAndWait(0x6001, 0, 20);
  Engine_EventWait(30);
  Actor_FaceDirection(JENNA, FACING_NORTHEAST, 40);
  Engine_ActorSetAnimationAndWait(JENNA, ANIM_NOD);
  Event_ShowMessageAndWait(SPEAKER_PORTRAIT_LEFT | 5, 0, 20);
  Actor_ShowEmote(GARET, EMOTE_IN_FRONT | 1, 80);
  Actor_ShowEmote(JENNA, EMOTE_IN_FRONT | 3, 40);
  Actor_SetSpeed(JENNA, PIXELS(4) / 5, PIXELS(2) / 5);
  Actor_WalkTo(JENNA, 428, 628);
  Engine_EventWait(20);
  Actor_FaceDirection(GARET, FACING_SOUTHEAST + FACING_STEP, 0);
  Engine_ActorWaitForMove(JENNA);
  Event_ShowMessage(0x5001, 0);
  Engine_ActorSetAnimation(JENNA, ANIM_STAND);
  Engine_EventWait(60);
  Actor_FaceDirection(JENNA, FACING_NORTHWEST + FACING_STEP, 30);
  Engine_ActorRunRepeatedMotion(GARET, 2);
  Actor_WalkToAndWait(JENNA, 428, 628);
  Actor_FaceDirection(JENNA, FACING_NORTHWEST + FACING_STEP, 20);
  Engine_ActorRunRepeatedMotion(GARET, 3);
  Engine_EventWait(10);
  Engine_ActorSetAnimationAndWait(JENNA, ANIM_SHAKE_HEAD);
  Engine_EventWait(10);
  Event_ShowMessageAndWait(SPEAKER_PORTRAIT_RIGHT | 5, 0, 20);
  Actor_SetAttachedEffect(GARET, 0x102);
  Engine_EventWait(40);
  Event_ShowMessageAndWait(0x5001, 0, 20);
  Engine_ActorSetAnimation(GARET, ANIM_NOD);
  Engine_ActorSetAnimationAndWait(JENNA, ANIM_NOD);
  Engine_EventWait(10);
  Actor_SetSpeed(JENNA, PIXELS(1) / 2, PIXELS(1) / 4);
  Actor_SetSpeed(GARET, PIXELS(1) / 2, PIXELS(1) / 4);
  Actor_WalkTo(JENNA, 450, 750);
  Actor_WalkTo(GARET, 450, 750);
  Engine_EventWait(60);
  gEventWork->transitionFrames = 60;
  Engine_EventCloseScreen();
  Engine_EventWaitForScreen();
  Engine_EventRequestExit(12);
  Engine_EventEnd();
}

/// Arriving by entrance 17 after the opening, in Vale: Dora praises the mended
/// roof, remembers Kyle, and sends Isaac, Garet and Jenna off to Mt. Aleph
/// with Kraden. "Good job, Isaac. You patched it nicely."
static void Event_RoofMended(void) {
  struct FieldActor *leader;
  struct FieldActor *actor;
  u32 i;
  s32 turnBack;
  s32 turnSide;
  struct FieldActor *turned;
  s32 none;
  s32 flag;
  SchedulerCallback callbackA;
  SchedulerCallback callbackB;
  SchedulerCallback callbackC;
  SchedulerCallback callbackD;
  SchedulerCallback callbackE;
  SchedulerCallback callbackF;

  leader = Actor_Get(ACTOR_PARTY_LEADER);
  Engine_EventBegin();
  Camera_MoveTo(-1, -1, -1, 0);
  Engine_TaskWait(1);
  Map_CopyCellAttributes(49, 53, 8, 4, 20, 50);
  Map_CopyCellsTo(2, 102, 84, 41, 2, 1);
  Map_CopyCellsTo(1, 102, 83, 41, 1, 1);
  Map_CopyCellsTo(0, 103, 82, 42, 1, 1);
  Actor_SetPosition(ACTOR_DORA, PIXELS(392), PIXELS(896));
  turned = Actor_Get(ACTOR_DORA);

  turned->facing = 0;
  turnBack = 0xc000;
  turned->facing = turnBack;
  Actor_SetPosition(GARET, PIXELS(298), PIXELS(736));
  turned = Actor_Get(GARET);
  turnSide = 0x4000;
  turned->facing = turnSide;
  Actor_SetPosition(JENNA, PIXELS(298), PIXELS(760));
  turned = Actor_Get(JENNA);
  turned->facing = turnSide;
  Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, 11);
  Actor_SetActionScript(0, gLeaderHammerAction);
  actor = Actor_Get(23);
  actor->motionFlags = 0;
  actor->x.fixed = PIXELS(388);
  actor->y.fixed = PIXELS(160);
  actor->z.fixed = PIXELS(840);
  Engine_ActorSetSpriteFlags(actor, 0);
  actor = Actor_Get(24);
  actor->motionFlags = 0;
  actor->x.fixed = PIXELS(388);
  actor->y.fixed = PIXELS(160);
  actor->z.fixed = PIXELS(844);
  Engine_ActorSetSpriteFlags(actor, 0);
  actor = Actor_Get(25);
  actor->motionFlags = 0;
  actor->x.fixed = PIXELS(388);
  actor->y.fixed = PIXELS(160);
  actor->z.fixed = PIXELS(848);
  Engine_ActorSetSpriteFlags(actor, 0);
  ((u8 *)Engine_EventGetViewCenter())[85] = 0;
  Engine_TaskWait(1);
  Camera_MoveTo(PIXELS(383), PIXELS(160), PIXELS(877), 0);
  Engine_MapRedraw();
  Engine_TaskWait(1);
  gEventWork->transitionFrames = 32;
  Engine_EventOpenScreen();
  Actor_SetSpeed(JENNA, PIXELS(1) / 2, turnSide);
  Actor_SetSpeed(GARET, PIXELS(1) / 2, turnSide);
  Actor_SetActionScript(5, gJennaAction);
  Actor_SetActionScript(1, gGaretAction);
  Engine_EventWait(40);
  Actor_SetBuiltinAction(0, ACTOR_ACTION_IDLE);
  leader->scaleX = Q16_ONE;
  leader->scaleY = Q16_ONE;
  Actor_FaceDirection(0, FACING_NORTHWEST + FACING_STEP, 40);
  Engine_ActorSetAnimationAndWait(ACTOR_PARTY_LEADER, ANIM_NOD);
  Engine_EventWait(10);
  Actor_SetSpeed(ACTOR_PARTY_LEADER, PIXELS(3) / 10, PIXELS(3) / 20);
  Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 400, 840);
  Engine_EventWait(10);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, turnBack, 30);
  Engine_ActorRunRepeatedMotion(ACTOR_PARTY_LEADER, 1);
  Engine_EventWait(20);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_WEST, 40);
  Scene_BeginCutscenePsynergy();
  Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, 17);
  FieldScheduler_AddCallback(Effect_PlayStepSound, 3200);
  for (i = 0; i < 40; i++) {
    Vale_FlashActorAndSpawnEffects(leader);
    Engine_TaskWait(1);
  }
  Engine_ActorSetSpritePriority(ACTOR_PARTY_LEADER, 1);

  callbackA = Vale_UpdateLeaderPaletteAndEffects;
  Task_AddCallback(callbackA, 3200);
  callbackB = Vale_CycleFirstBoardPalette;
  Task_AddCallback(callbackB, 3200);
  Actor_SetSpeed(23, PIXELS(1) / 5, PIXELS(1) / 10);
  Actor_MoveToAndWait(23, 390, 832);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 0);
  Actor_MoveToAndWait(23, 400, 826);
  Engine_EventWait(20);
  {
    struct FieldActor *record = Actor_Get(ACTOR_PARTY_LEADER);

    record->priorityFlags = ACTOR_PRIORITY_AUTOMATIC | record->priorityFlags;
  }
  Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_STAND);
  Scheduler_RemoveCallback(Effect_PlayStepSound);
  Scheduler_RemoveCallback(callbackA);
  Scheduler_RemoveCallback(callbackB);
  Engine_TaskWait(1);
  Actor_SetChildValue(ACTOR_PARTY_LEADER, 0);
  Actor_SetChildValue(23, 0);
  Actor_SetPosition(23, 0, 0);
  Engine_EventWait(20);
  Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, 11);
  Actor_SetActionScript(0, gLeaderHammerAction);
  Engine_EventWait(120);
  Map_CopyCellsTo(7, 102, 84, 41, 2, 1);
  Actor_SetBuiltinAction(0, ACTOR_ACTION_IDLE);
  leader->scaleX = Q16_ONE;
  leader->scaleY = Q16_ONE;
  Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_STAND);
  Engine_EventWait(40);
  Engine_ActorSetAnimationAndWait(ACTOR_PARTY_LEADER, ANIM_NOD);
  Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 377, 843);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 40);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_EAST, 20);
  Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, 17);
  Task_AddCallback(Effect_PlayStepSound, 3200);
  for (i = 0; i < 40; i++) {
    Vale_FlashActorAndSpawnEffects(leader);
    Engine_TaskWait(1);
  }
  Engine_ActorSetSpritePriority(ACTOR_PARTY_LEADER, 1);
  callbackC = Vale_UpdateLeaderPaletteAndEffects;
  Task_AddCallback(callbackC, 3200);
  callbackD = Vale_CycleSecondBoardPalette;
  Task_AddCallback(callbackD, 3200);
  Actor_SetSpeed(24, PIXELS(1) / 5, PIXELS(1) / 10);
  Actor_MoveToAndWait(24, 390, 832);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 0);
  Actor_MoveToAndWait(24, 377, 828);
  Engine_EventWait(20);
  {
    struct FieldActor *record = Actor_Get(ACTOR_PARTY_LEADER);

    record->priorityFlags = ACTOR_PRIORITY_AUTOMATIC | record->priorityFlags;
  }
  Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_STAND);
  Scheduler_RemoveCallback(Effect_PlayStepSound);
  Scheduler_RemoveCallback(callbackC);
  Scheduler_RemoveCallback(callbackD);
  Engine_TaskWait(1);
  Actor_SetChildValue(ACTOR_PARTY_LEADER, 0);
  Actor_SetChildValue(24, 0);
  Actor_SetPosition(24, 0, 0);
  Engine_EventWait(20);
  Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, 11);
  Actor_SetActionScript(0, gLeaderHammerAction);
  Engine_EventWait(120);
  Map_CopyCellsTo(6, 102, 83, 41, 1, 1);
  Actor_SetBuiltinAction(0, ACTOR_ACTION_IDLE);
  leader->scaleX = Q16_ONE;
  leader->scaleY = Q16_ONE;
  Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_STAND);
  Engine_EventWait(40);
  Engine_ActorSetAnimationAndWait(ACTOR_PARTY_LEADER, ANIM_NOD);
  Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 360, 855);
  Actor_FaceDirection(ACTOR_DORA, FACING_NORTHWEST + FACING_STEP, 10);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 30);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH + FACING_STEP, 20);
  Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, 17);
  Task_AddCallback(Effect_PlayStepSound, 3200);
  for (i = 0; i < 40; i++) {
    Vale_FlashActorAndSpawnEffects(leader);
    Engine_TaskWait(1);
  }
  Engine_ActorSetSpritePriority(ACTOR_PARTY_LEADER, 1);
  callbackE = Vale_UpdateLeaderPaletteAndEffects;
  Task_AddCallback(callbackE, 3200);
  callbackF = Vale_CycleActor25Palette;
  FieldScheduler_AddCallback(callbackF, 3200);
  Actor_SetSpeed(25, PIXELS(1) / 5, PIXELS(1) / 10);
  Actor_MoveToAndWait(25, 390, 832);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 0);
  Actor_MoveToAndWait(25, 360, 837);
  Engine_EventWait(20);
  {
    struct FieldActor *record = Actor_Get(ACTOR_PARTY_LEADER);

    record->priorityFlags = ACTOR_PRIORITY_AUTOMATIC | record->priorityFlags;
  }
  Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_STAND);
  Scheduler_RemoveCallback(Effect_PlayStepSound);
  Scheduler_RemoveCallback(callbackE);
  Scheduler_RemoveCallback(callbackF);
  Engine_TaskWait(1);
  Actor_SetChildValue(ACTOR_PARTY_LEADER, 0);
  Actor_SetChildValue(25, 0);
  Actor_SetPosition(25, 0, 0);
  Engine_EventWait(20);
  Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, 11);
  Actor_SetActionScript(0, gLeaderHammerAction);
  Engine_EventWait(120);
  Scene_EndCutscenePsynergy();
  Map_CopyCellsTo(5, 103, 82, 42, 1, 1);
  Actor_SetBuiltinAction(0, ACTOR_ACTION_IDLE);
  leader->scaleX = Q16_ONE;
  leader->scaleY = Q16_ONE;
  Engine_ActorJump(ACTOR_DORA, 2, 20);
  Engine_EventSetMessage(MsgValeGoodJob);
  Event_ShowMessageAndWait(ACTOR_DORA, 0, 10);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_EAST + FACING_STEP, 10);
  Vale_MoveActorAlongLowerZPath(21, 5, 6, 0);
  Actor_SetSpeed(ACTOR_DORA, PIXELS(3) / 10, PIXELS(3) / 20);
  Actor_WalkToAndWait(ACTOR_DORA, 397, 832);
  Engine_EventWait(20);
  Actor_FaceDirection(ACTOR_DORA, FACING_SOUTH, 60);
  Actor_FaceDirection(ACTOR_DORA, FACING_NORTH, 60);
  Engine_ActorSetAnimationAndWait(ACTOR_DORA, ANIM_NOD);
  Engine_ActorSetAnimationAndWait(ACTOR_DORA, ANIM_NOD);
  Event_ShowMessageAndWait(ACTOR_DORA, 0, 10);
  Actor_WalkToAndWait(ACTOR_DORA, 372, 832);
  Engine_EventWait(20);
  Actor_FaceDirection(ACTOR_DORA, FACING_SOUTH, 40);
  Actor_FaceDirection(ACTOR_DORA, FACING_WEST, 40);
  Engine_ActorSetAnimationAndWait(ACTOR_DORA, ANIM_NOD);
  Engine_ActorSetAnimationAndWait(ACTOR_DORA, ANIM_NOD);
  Event_ShowMessageAndWait(ACTOR_DORA, 0, 20);
  Actor_FaceDirection(ACTOR_DORA, FACING_SOUTH + FACING_STEP, 20);
  Engine_ActorSetAnimationAndWait(ACTOR_DORA, ANIM_NOD);
  Event_ShowMessageAndWait(ACTOR_DORA, 0, 10);
  Engine_ActorRunRepeatedMotion(ACTOR_PARTY_LEADER, 2);
  Engine_EventWait(20);
  Engine_ActorSetAnimationAndWait(ACTOR_DORA, ANIM_SHAKE_HEAD);
  Event_OpenMessage(ACTOR_DORA, 0);
  if (Engine_EventChooseYesNo(0, 0) == 0) {
    Engine_ActorSetAnimationAndWait(ACTOR_DORA, ANIM_NOD);
    gEventWork->message++;
  } else {
    Engine_ActorSetAnimationAndWait(ACTOR_DORA, ANIM_SHAKE_HEAD);
  }
  Event_ShowMessageAndWait(ACTOR_DORA, 0, 20);
  Engine_EventSetMessage(MsgValeWorkingYourselvesBone);
  Actor_WalkToAndWait(ACTOR_DORA, 386, 841);
  Engine_EventWait(10);
  Actor_FaceDirection(ACTOR_DORA, FACING_NORTH + FACING_STEP, 60);
  Engine_ActorRunRepeatedMotion(ACTOR_DORA, 2);
  Event_ShowMessageAndWait(ACTOR_DORA, 0, 20);
  Actor_FaceDirection(ACTOR_DORA, FACING_SOUTH + FACING_STEP, 30);
  Event_OpenMessage(ACTOR_DORA, 0);
  if (Engine_EventChooseYesNo(0, 0) == 1) {
    gEventWork->message++;
  }
  Event_ShowMessageAndWait(ACTOR_DORA, 0, 20);
  Actor_FaceDirection(ACTOR_DORA, FACING_NORTH + FACING_STEP, 60);
  Engine_ActorRunRepeatedMotion(ACTOR_DORA, 2);
  Engine_EventSetMessage(MsgValeDevastatedWhenKyle);
  Event_ShowMessageAndWait(ACTOR_DORA, 0, 20);
  Actor_WalkToAndWait(ACTOR_DORA, 386, 825);
  Engine_EventWait(10);
  Engine_ActorSetAnimationAndWait(ACTOR_DORA, ANIM_SHAKE_HEAD);
  Engine_EventWait(60);
  Event_ShowMessageAndWait(ACTOR_DORA, 0, 60);
  Actor_FaceDirection(ACTOR_DORA, FACING_SOUTH + FACING_STEP, 10);
  Event_ShowMessageAndWait(ACTOR_DORA, 0, 10);
  Actor_WalkToAndWait(ACTOR_DORA, 372, 832);
  Actor_FaceDirection(ACTOR_DORA, FACING_SOUTH + FACING_STEP, 10);
  Engine_ActorRunRepeatedMotion(ACTOR_PARTY_LEADER, 2);
  Engine_EventWait(20);
  Engine_ActorSetAnimationAndWait(ACTOR_DORA, ANIM_NOD);
  Engine_EventWait(20);
  Event_ShowMessageAndWait(ACTOR_DORA, 0, 20);
  Actor_ShowEmote(ACTOR_PARTY_LEADER, 258, 60);
  Engine_ActorRunRepeatedMotion(ACTOR_DORA, 2);
  Event_ShowMessageAndWait(ACTOR_DORA, 0, 20);
  Engine_ActorSetAnimationAndWait(ACTOR_PARTY_LEADER, ANIM_NOD);
  Engine_ActorSetAnimationAndWait(ACTOR_DORA, ANIM_NOD);
  Event_ShowMessageAndWait(ACTOR_DORA, 0, 10);
  Camera_SetSpeed(PIXELS(2) / 5, PIXELS(1) / 20);
  Camera_MoveTo(PIXELS(377), PIXELS(160), PIXELS(860), 1);
  Actor_SetSpeed(JENNA, PIXELS(1), PIXELS(1) / 2);
  Actor_SetSpeed(GARET, PIXELS(1), PIXELS(1) / 2);
  Actor_WalkTo(GARET, 369, 904);
  Actor_WalkToAndWait(JENNA, 392, 904);
  Engine_ActorSetAnimation(GARET, ANIM_STAND);
  Vale_MoveActorAlongLowerZPath(5, 10, 11, 0);
  Actor_FaceDirection(JENNA, FACING_NORTHWEST, 0);
  Event_ShowMessageAndWait(JENNA, 0, 10);
  Engine_ActorRunRepeatedMotion(ACTOR_DORA, 2);
  Engine_EventWait(10);
  Actor_FaceDirection(ACTOR_DORA, FACING_SOUTHEAST + FACING_STEP, 0);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_EAST + FACING_STEP, 20);
  Engine_ActorJump(JENNA, 4, 0);
  Actor_WalkToAndWait(JENNA, 392, 843);
  Actor_FaceDirection(JENNA, FACING_WEST + FACING_STEP, 0);
  Actor_FaceDirection(ACTOR_DORA, FACING_SOUTHEAST + FACING_STEP, 0);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH + FACING_STEP, 20);
  Engine_ActorSetAnimationAndWait(21, ANIM_NOD);
  Event_ShowMessageAndWait(ACTOR_DORA, 0, 10);
  Engine_ActorSetAnimationAndWait(JENNA, ANIM_NOD);
  Event_ShowMessageAndWait(JENNA, 0, 10);
  Engine_ActorSetAnimation(ACTOR_DORA, ANIM_NOD);
  Engine_ActorSetAnimationAndWait(ACTOR_PARTY_LEADER, ANIM_NOD);
  Event_ShowMessageAndWait(ACTOR_DORA, 0, 20);
  Vale_MoveActorAlongLowerZPath(1, 10, 11, 0);
  Actor_SetSpeed(JENNA, PIXELS(3) / 10, PIXELS(3) / 20);
  Actor_SetSpeed(GARET, PIXELS(3) / 10, PIXELS(3) / 20);
  Actor_WalkTo(GARET, 392, 843);
  Object_GetById(5)->steeringFlags &= ~ACTOR_FACES_MOTION;
  Actor_WalkToAndWait(JENNA, 408, 843);
  Engine_EventWait(1);
  {
    struct FieldActor *record = Actor_Get(JENNA);

    record->steeringFlags = ACTOR_FACES_MOTION | record->steeringFlags;
  }
  Actor_FaceDirection(JENNA, FACING_WEST, 0);
  Engine_ActorWaitForMove(GARET);
  Engine_ActorSetAnimation(GARET, ANIM_STAND);
  Actor_FaceDirection(GARET, FACING_WEST, 30);
  Engine_ActorJump(ACTOR_DORA, 4, 30);
  Event_ShowMessageAndWait(ACTOR_DORA, 0, 20);
  Engine_ActorSetAnimationAndWait(GARET, ANIM_NOD);
  Engine_EventWait(20);
  Actor_FaceDirection(ACTOR_DORA, FACING_SOUTH + FACING_STEP, 20);
  Event_ShowMessageAndWait(ACTOR_DORA, 0, 10);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH + FACING_STEP, 0);
  Engine_ActorJump(ACTOR_PARTY_LEADER, 2, 30);
  Actor_SetAttachedEffect(ACTOR_PARTY_LEADER, 258);
  Engine_EventWait(60);
  Actor_FaceDirection(ACTOR_DORA, FACING_SOUTHEAST + FACING_STEP, 40);
  Engine_ActorRunRepeatedMotion(GARET, 2);
  Event_ShowMessageAndWait(GARET, 0, 20);
  Actor_ShowEmote(ACTOR_DORA, 257, 80);
  Actor_FaceDirection(ACTOR_DORA, FACING_SOUTH + FACING_STEP, 30);
  Actor_ShowEmote(ACTOR_PARTY_LEADER, 258, 60);
  Actor_FaceDirection(ACTOR_DORA, FACING_SOUTHEAST + FACING_STEP, 0);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH + FACING_STEP, 20);
  Event_ShowMessageAndWait(ACTOR_DORA, 0, 10);
  Engine_ActorSetAnimationAndWait(GARET, ANIM_NOD);
  Engine_EventWait(80);
  Engine_ActorFaceEachOther(JENNA, GARET, 30);
  Engine_ActorStartRepeatedMotion(GARET, 2);
  Engine_ActorRunRepeatedMotion(JENNA, 2);
  Engine_EventWait(10);
  Actor_ShowEmote(ACTOR_DORA, 261, 60);
  Event_ShowMessageAndWait(ACTOR_DORA, 0, 10);
  Actor_FaceDirection(JENNA, FACING_WEST, 0);
  Actor_FaceDirection(GARET, FACING_WEST, 10);
  Engine_ActorRunRepeatedMotion(JENNA, 2);
  Engine_EventWait(20);
  Event_ShowMessageAndWait(JENNA, 0, 10);
  Engine_ActorStartRepeatedMotion(ACTOR_DORA, 2);
  Engine_EventWait(40);
  Event_ShowMessageAndWait(ACTOR_DORA, 0, 10);
  Engine_ActorSetAnimation(GARET, ANIM_NOD);
  Engine_ActorSetAnimationAndWait(JENNA, ANIM_NOD);
  Engine_EventWait(20);
  Engine_ActorSetAnimationAndWait(ACTOR_DORA, ANIM_SHAKE_HEAD);
  Event_ShowMessageAndWait(ACTOR_DORA, 0, 10);
  Engine_ActorSetAnimation(GARET, ANIM_NOD);
  Engine_ActorSetAnimationAndWait(JENNA, ANIM_NOD);
  Engine_ActorSetAnimationAndWait(ACTOR_DORA, ANIM_NOD);
  Event_ShowMessageAndWait(ACTOR_DORA, 0, 10);
  Engine_ActorSetAnimation(GARET, ANIM_NOD);
  Engine_ActorSetAnimationAndWait(JENNA, ANIM_NOD);
  Engine_EventWait(20);
  Engine_ActorRunRepeatedMotion(ACTOR_DORA, 2);
  Event_ShowMessageAndWait(ACTOR_DORA, 0, 20);
  Camera_SetSpeed(PIXELS(3) / 5, PIXELS(3) / 40);
  Camera_MoveTo(PIXELS(373), PIXELS(160), PIXELS(837), 1);
  Actor_WalkToAndWait(ACTOR_DORA, 364, 816);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH + FACING_STEP, 0);
  Actor_FaceDirection(ACTOR_DORA, FACING_SOUTHEAST + FACING_STEP, 10);
  Event_ShowMessageAndWait(ACTOR_DORA, 0, 40);
  Engine_ActorFaceEachOther(JENNA, GARET, 30);
  Engine_ActorStartRepeatedMotion(GARET, 2);
  Engine_ActorRunRepeatedMotion(JENNA, 2);
  Engine_EventWait(20);
  Actor_FaceDirection(GARET, FACING_WEST, 0);
  Actor_FaceDirection(5, FACING_WEST, 20);
  Actor_FaceDirection(ACTOR_DORA, FACING_SOUTH + FACING_STEP, 20);
  Event_ShowMessageAndWait(ACTOR_DORA, 0, 10);
  Actor_ShowEmote(ACTOR_PARTY_LEADER, 261, 60);
  Engine_ActorSetAnimationAndWait(ACTOR_DORA, ANIM_SHAKE_HEAD);
  Event_OpenMessage(ACTOR_DORA, 0);
  none = 0;

  if (Engine_EventChooseYesNo(0, 0) == 1) {
    gEventWork->message++;
  }
  Event_ShowMessageAndWait(ACTOR_DORA, 0, 20);
  Engine_EventSetMessage(MsgValeAsStubbornAsYourFather);
  Actor_ShowEmote(ACTOR_DORA, 259, 0);
  Engine_ActorRunRepeatedMotion(ACTOR_DORA, 3);
  Event_ShowMessageAndWait(ACTOR_DORA, 0, 10);
  Engine_ActorJump(ACTOR_DORA, 4, 0);
  Engine_ActorRunRepeatedMotion(ACTOR_DORA, 3);
  Engine_ActorSetAnimation(ACTOR_DORA, 7);
  Engine_EventWait(5);
  Event_ShowTwoMessagesAndWait(21, 14, 2, 24, 2, 1, 10, 14, 4, 14, none);
  Event_PlayCue(SE_161);
  actor = Actor_Get(ACTOR_DORA);
  actor->sprite->flags = none;
  actor->steeringFlags &= 0xfe;
  Actor_SetSpeed(ACTOR_DORA, PIXELS(3), PIXELS(3) / 2);
  Actor_WalkToAndWait(ACTOR_DORA, 364, 815);
  Engine_EventWait(4);
  for (i = 0; i != 4; i++) {
    actor->z.fixed += PIXELS(3) / 2;
    actor->scaleY += -0x1999;
    Engine_EventWait(1);
  }
  Actor_SetPosition(ACTOR_DORA, 0, 0);
  Actor_SetSpeed(GARET, PIXELS(3), PIXELS(3) / 2);
  Engine_ActorJump(GARET, 6, 0);
  Actor_WalkToAndWait(GARET, 374, VALE_GARET_REUNION_Z);
#if TBS_PATCH_2
  Actor_SetPosition(GARET, PIXELS(374), PIXELS(VALE_GARET_REUNION_Z));
#endif
  Event_ShowMessage(JENNA, 0);
  Actor_FaceDirection(GARET, FACING_NORTHWEST + FACING_STEP, 0);
  Actor_ShowEmote(JENNA, 256, 0);
  Engine_ActorStartRepeatedMotion(JENNA, 2);
  Actor_ShowEmote(GARET, 256, 10);
  Engine_ActorSetAnimation(GARET, 13);
  Engine_ActorJump(GARET, 2, 5);
  Event_PlayCue(SE_143);
  Map_SetShake(0, PIXELS(4), Q16_ONE);
  Map_CopyCellsTo(1, 102, 83, 41, 1, 1);
  Engine_ActorSetSpriteFlags(Actor_Get(GARET), 0);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH + FACING_STEP, 10);
  Engine_ActorStartRepeatedMotion(GARET, 3);
  Map_SetShake(-1, -1, 9 * Q16_ONE / 10);
  Engine_MapRenderWaitForValues();
  Actor_ShowEmote(GARET, 258, 80);
  Engine_ActorSetAnimation(ACTOR_DORA, 8);
  actor->scaleY = 0x8000;
  Actor_SetPosition(ACTOR_DORA, PIXELS(364), PIXELS(811));
  for (i = 0; i != 5; i++) {
    actor->scaleY += 0x1999;
    Engine_EventWait(1);
  }
  Engine_EventWait(60);
  Engine_ActorRunRepeatedMotion(GARET, 2);
  Actor_FaceDirection(GARET, FACING_SOUTH + FACING_STEP, 30);
  Engine_ActorStartRepeatedMotion(GARET, 2);
  Engine_ActorRunRepeatedMotion(JENNA, 2);
  Engine_EventWait(60);
  Engine_ActorRunRepeatedMotion(ACTOR_DORA, 2);
  Engine_EventWait(20);
  Event_ShowMessageAndWait(ACTOR_DORA, 0, 20);
  Camera_SetSpeed(PIXELS(3) / 10, PIXELS(3) / 80);
  Camera_MoveTo(PIXELS(372), PIXELS(160), PIXELS(859), 1);
  Actor_SetSpeed(ACTOR_DORA, PIXELS(3), PIXELS(3) / 2);
  Engine_ActorJump(ACTOR_DORA, 6, 0);
  Actor_WalkToAndWait(ACTOR_DORA, 359, 835);
  Engine_EventWait(20);
  Actor_FaceDirection(ACTOR_DORA, FACING_SOUTH, 20);
  Engine_ActorRunRepeatedMotion(ACTOR_DORA, 2);
  actor->priorityFlags &= 0xfe;
  Event_ShowMessageAndWait(ACTOR_DORA, 0, 80);
  Actor_ShowEmote(ACTOR_DORA, 257, 80);
  Actor_FaceDirection(ACTOR_DORA, FACING_EAST, 60);
  Engine_ActorRunRepeatedMotion(ACTOR_DORA, 3);
  Event_ShowMessageAndWait(ACTOR_DORA, 0, 10);
  Actor_SetAttachedEffect(ACTOR_DORA, 258);
  Engine_EventWait(80);
  Actor_FaceDirection(GARET, FACING_SOUTH + FACING_STEP, 40);
  Actor_ShowEmote(GARET, 258, 80);
  Engine_ActorRunRepeatedMotion(GARET, 2);
  Event_ShowMessageAndWait(GARET, 0, 20);
  Actor_FaceDirection(GARET, FACING_WEST, 20);
  Engine_ActorRunRepeatedMotion(GARET, 3);
  Engine_EventWait(10);
  Engine_ActorRunRepeatedMotion(GARET, 3);
  Engine_ActorSetSpriteFlags(Actor_Get(GARET), 1);
  Engine_ActorJump(GARET, 6, 0);
  Engine_ActorSetAnimation(GARET, ANIM_STAND);
  Actor_SetSpeed(GARET, PIXELS(4), PIXELS(2));
  actor = Actor_Get(GARET);
  actor->steeringFlags &= ~ACTOR_FACES_MOTION;
  Actor_SetDestination(GARET, 403, VALE_GARET_REUNION_Z);
  Actor_SetAttachedEffect(JENNA, 258);
  Actor_FaceDirection(JENNA, FACING_NORTH, 20);
  Event_ShowMessageAndWait(JENNA, 0, 1);
  Engine_ActorWaitForMove(GARET);
  Actor_FaceDirection(GARET, FACING_SOUTH + FACING_STEP, 20);
  Event_ShowMessageAndWait(GARET, 0, 20);
  Actor_ShowEmote(GARET, 256, 0);
  Engine_ActorSetAnimation(GARET, 13);
  Engine_ActorJump(GARET, 2, 5);
  Engine_ActorSetSpriteFlags(Actor_Get(GARET), 0);
  Map_CopyCellsTo(2, 102, 84, 41, 2, 1);
  Event_PlayCue(SE_143);
  Map_SetShake(0, PIXELS(4), Q16_ONE);
  Engine_ActorRunRepeatedMotion(GARET, 3);
  Map_SetShake(-1, -1, 9 * Q16_ONE / 10);
  Engine_MapRenderWaitForValues();
  Actor_ShowEmote(GARET, 258, 30);
  Actor_SetSpeed(JENNA, PIXELS(3) / 10, PIXELS(3) / 20);
  Actor_WalkToAndWait(JENNA, 408, 855);
  Engine_EventWait(60);
  Engine_ActorRunRepeatedMotion(GARET, 2);
  Actor_ShowEmote(ACTOR_DORA, 261, 60);
  Engine_ActorStartRepeatedMotion(JENNA, 3);
  Engine_ActorRunRepeatedMotion(ACTOR_PARTY_LEADER, 3);
  Engine_EventWait(80);
  Actor_FaceDirection(GARET, FACING_SOUTH, 30);
  Engine_ActorRunRepeatedMotion(GARET, 3);
  Engine_EventWait(10);
  Engine_ActorSetAnimationAndWait(JENNA, ANIM_SHAKE_HEAD);
  Engine_EventWait(80);
  Engine_ActorSetAnimationAndWait(ACTOR_DORA, ANIM_NOD);
  Engine_EventWait(10);
  Event_ShowMessageAndWait(ACTOR_DORA, 0, 10);
  Actor_FaceDirection(JENNA, FACING_NORTHWEST + FACING_STEP, 0);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 0);
  Actor_FaceDirection(ACTOR_DORA, FACING_EAST, 60);
  Actor_FaceDirection(ACTOR_DORA, FACING_SOUTH, 60);
  Engine_ActorSetAnimationAndWait(ACTOR_DORA, ANIM_SHAKE_HEAD);
  Engine_EventWait(60);
  Actor_FaceDirection(ACTOR_DORA, FACING_EAST, 80);
  Actor_ShowEmote(ACTOR_DORA, 261, 80);
  Event_ShowMessageAndWait(ACTOR_DORA, 0, 60);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 0);
  Actor_ShowEmote(ACTOR_PARTY_LEADER, 257, 0);
  Actor_ShowEmote(JENNA, 257, 0);
  Actor_ShowEmote(GARET, 257, 60);
  Engine_ActorSetAnimationAndWait(ACTOR_DORA, ANIM_SHAKE_HEAD);
  Engine_EventWait(30);
  Event_ShowMessageAndWait(ACTOR_DORA, 0, 60);
  Engine_ActorStartRepeatedMotion(GARET, 2);
  Engine_ActorRunRepeatedMotion(JENNA, 2);
  Engine_EventWait(20);
  Actor_FaceDirection(JENNA, FACING_WEST, 60);
  Actor_FaceDirection(ACTOR_DORA, FACING_SOUTH, 30);
  Event_ShowMessageAndWait(ACTOR_DORA, 0, 30);
  Engine_ActorSetAnimationAndWait(JENNA, ANIM_NOD);
  Engine_EventWait(10);
  Event_ShowMessageAndWait(JENNA, 0, 20);
  Actor_FaceDirection(ACTOR_DORA, FACING_EAST, 30);
  Engine_ActorSetAnimationAndWait(ACTOR_DORA, ANIM_SHAKE_HEAD);
  Engine_EventWait(20);
  Event_ShowMessageAndWait(ACTOR_DORA, 0, 20);
  Engine_ActorSetAnimationAndWait(JENNA, ANIM_NOD);
  Engine_EventWait(20);
  Engine_ActorRunRepeatedMotion(GARET, 3);
  Engine_EventWait(10);
  Event_ShowMessageAndWait(GARET, 0, 20);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH + FACING_STEP, 0);
  Actor_ShowEmote(ACTOR_DORA, 256, 0);
  Engine_ActorRunRepeatedMotion(ACTOR_DORA, 3);
  Engine_EventWait(30);
  Event_ShowMessageAndWait(ACTOR_DORA, 0, 60);
  Engine_ActorRunRepeatedMotion(GARET, 3);
  Actor_SetSpeed(GARET, PIXELS(1), PIXELS(1) / 2);
  Engine_ActorSetSpriteFlags(Actor_Get(GARET), 0);
  Engine_ActorJump(GARET, 4, 0);
  Engine_ActorWalkToAndWait(1, 398, 828);
  Engine_EventWait(60);
  Actor_FaceDirection(ACTOR_DORA, FACING_SOUTH, 0);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 60);
  Engine_ActorSetAnimationAndWait(ACTOR_PARTY_LEADER, ANIM_NOD);
  Engine_EventWait(60);
  Engine_ActorSetAnimationAndWait(ACTOR_DORA, ANIM_NOD);
  Engine_EventWait(60);
  actor = Actor_Get(GARET);
  flag = 1;
  actor->steeringFlags |= flag;
  actor = Actor_Get(JENNA);
  actor->steeringFlags |= flag;
  actor = Actor_Get(ACTOR_PARTY_LEADER);
  Actor_SetSpeed(GARET, PIXELS(1), PIXELS(1) / 2);
  Actor_SetSpeed(JENNA, PIXELS(1), PIXELS(1) / 2);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_EAST, 0);
  Actor_WalkTo(JENNA, actor->x.part.pixel + 16, actor->z.part.pixel);
  Actor_WalkToAndWait(GARET, actor->x.part.pixel + 16,
                      actor->z.part.pixel - 16);
  Engine_ActorWaitForMove(GARET);
  Actor_FaceDirection(GARET, FACING_SOUTH + FACING_STEP, 30);
  Engine_ActorSetAnimation(GARET, ANIM_NOD);
  Engine_ActorSetAnimation(JENNA, ANIM_NOD);
  Engine_ActorSetAnimationAndWait(ACTOR_PARTY_LEADER, ANIM_NOD);
  Engine_EventWait(40);
  Actor_WalkToAndWait(JENNA, actor->x.part.pixel, actor->z.part.pixel);
  Actor_SetPosition(JENNA, 0, 0);
  Actor_WalkToAndWait(GARET, actor->x.part.pixel, actor->z.part.pixel);
  Actor_SetPosition(GARET, 0, 0);
  Engine_PartyAddMembers(1, 5);
  Camera_MoveTo(PIXELS(377), PIXELS(160), PIXELS(887), 1);
  Vale_MoveActorAlongHigherZPath(0, 13, 10, 0);
  Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 376, 912);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 0);
  {
    struct FieldActor *record = Actor_Get(ACTOR_DORA);

    record->steeringFlags = flag | record->steeringFlags;
  }
  Vale_MoveActorAlongHigherZPath(21, 6, 5, 0);
  Actor_WalkToAndWait(ACTOR_DORA, 373, 887);
  Actor_FaceDirection(ACTOR_DORA, FACING_SOUTH, 0);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 40);
  Engine_ActorSetAnimation(ACTOR_DORA, ANIM_NOD);
  Engine_ActorSetAnimationAndWait(ACTOR_PARTY_LEADER, ANIM_NOD);
  Engine_EventWait(20);
  Engine_CameraFollowActor(ACTOR_PARTY_LEADER, 1);
  Engine_CameraWaitForMove();
  Engine_EventWait(100);
  Map_CopyCellAttributes(49, 46, 8, 4, 20, 50);
  GameFlag_Set(FLAG_SCENE_TEMP_02);
  GameFlag_Clear(FLAG_SHOW_LOCATION_NAME);
#if TBS_PATCH_1
  leader->motionFlags = 3;
  leader->y.fixed = PIXELS(160);
  leader->targetY = 0x80000000;
  leader->velocityY = 0;
#else
  Object_GetById(ACTOR_PARTY_LEADER)->motionFlags = 3;
#endif
  Engine_EventEnd();
}

#include "common/effect_psynergy.h"

/// Cycles palettes on Garet and periodically spawns attached effects.
static void Vale_UpdateGaretPaletteAndEffects(void) {
  Vale_CycleActorPaletteAndSpawnEffects(Object_GetById(GARET));
}

/// Cycles the leader palette and periodically spawns attached effects.
static void Vale_UpdateLeaderPaletteAndEffects(void) {
  Vale_CycleActorPaletteAndSpawnEffects(Object_GetById(ACTOR_PARTY_LEADER));
}

/// Cycles actor 9 palette during the repair scene.
static void Vale_CycleVillagerNinePalette(void) {
  Vale_CycleActorPalette(Object_GetById(9));
}

/// Cycles the first board palette while its repair effect runs.
static void Vale_CycleFirstBoardPalette(void) {
  Vale_CycleActorPalette(Object_GetById(0x17));
}

/// Cycles the second board palette while its repair effect runs.
static void Vale_CycleSecondBoardPalette(void) {
  Vale_CycleActorPalette(Object_GetById(0x18));
}

/// Cycles actor 25 palette during the repair scene.
static void Vale_CycleActor25Palette(void) {
  Vale_CycleActorPalette(Object_GetById(0x19));
}

static s32 Runtime_ComputeFixedPointDistance(s32 *first_position,
                                             s32 *second_position) {
  s32 delta_x = (*first_position++ - *second_position++) >> 16;
  s32 delta_y = (*first_position++ - *second_position++) >> 16;
  s32 delta_z = (*first_position - *second_position) >> 16;
  s32 delta_x_squared = delta_x * delta_x;
  s32 delta_y_squared = delta_y * delta_y;
  s32 delta_z_squared = delta_z * delta_z;

  return Iwram_Sqrt(delta_x_squared + delta_y_squared + delta_z_squared);
}

static s32 Vale_TestFacing(struct FieldActor *obj, struct FieldActor *target,
                           s32 range, s32 force) {
  s32 result;
  u32 angle;
  u32 left;
  u32 right;
  u32 dir;

  result = 0;
  if (obj->paused == 1) {
    if (obj->riseCounter == 0) {
      Object_SetMode(obj, 1);
      return 1;
    }
  }
  if (Runtime_ComputeFixedPointDistance(&target->x.fixed, &obj->x.fixed) <
          range ||
      force != 0) {
    angle = (u16)ArcTan2(target->z.fixed - obj->z.fixed,
                         target->x.fixed - obj->x.fixed);
    left = (angle - 0x1000) & 0xf000;
    right = (angle + 0x1000) & 0xf000;
    angle &= 0xf000;
    dir = obj->facing & 0xf000;
    if (angle == dir || right == dir || left == dir || force != 0) {
      obj->paused = 1;
      Object_SetMode(obj, 1);
      result = 1;
      obj->riseCounter = result;
    } else {
      obj->paused = 0;
      Object_SetMode(obj, 2);
      obj->riseCounter = 0;
    }
  } else {
    obj->paused = 0;
    Object_SetMode(obj, 2);
    obj->riseCounter = 0;
  }
  return result;
}

// UB: calling this s32 function through a void update hook is incompatible.
// Plain void changes 48 to 44 bytes in tbs-en.
#ifdef UBFIX
static void Vale_HaveVillagerWatchTheLeader(union FieldObject *fieldObject) {
#else
static s32 Vale_HaveVillagerWatchTheLeader(union FieldObject *fieldObject) {
#endif
  struct FieldActor *villager = &fieldObject->actor;
  struct FieldActor *leader = Object_GetById(ACTOR_PARTY_LEADER);
  if (villager->targetX == ACTOR_NO_TARGET &&
      villager->targetZ == ACTOR_NO_TARGET)
#ifdef UBFIX
    return;
#else
    return 0;
#endif
  Vale_TestFacing(villager, leader, 18, 0);
#ifdef UBFIX
  return;
#else
  return 0;
#endif
}

/// Emits one dust particle below the moving source actor.
static void Effect_ConfigureSpawnedParticle(union FieldObject *fieldObject) {
  struct FieldActor *source = &fieldObject->actor;
  s32 spawnPosition[3];
  s32 randomDistance;
  struct FieldActor *particle;
  SET_VECTOR(spawnPosition, source->x.fixed,
             source->y.fixed - (Engine_RandomNext() << 4) - PIXELS(8),
             source->z.fixed);
  randomDistance = Engine_RandomNext();
  Vector_AddPolarOffset(((randomDistance << 1) + randomDistance) << 4,
                        Engine_RandomNext(), spawnPosition);
  particle = Engine_ObjectCreate(0x11d, spawnPosition[0], spawnPosition[1],
                                 spawnPosition[2]);
  if (particle != 0) {
    particle->motionFlags = 2;
    particle->gravity = 0x1999;
    particle->wait = 12;
    Engine_ActorSetSpriteFlags(particle, 0);
    Object_SetMode(particle, 0);
    Engine_ObjectSetScript(particle, gDustBurstScript);
    {
      struct AnimationObject *record = particle->sprite;
      s32 priorityFlags = ~12;
      priorityFlags &= ((u8 *)&record->image)[9];
      priorityFlags |= 4;
      ((u8 *)&record->image)[9] = priorityFlags;
    }
  }
  Engine_AudioPlayCue(SE_ITEM_BREAK);
}

/// Shrinks and raises the emitter, scatters dust, then clears its motion
/// targets.
static void Effect_SpawnRisingDustBurst(struct FieldActor *emitter) {
  s32 frameCountdown;

  Engine_AudioPlayCue(SE_CAST);

  for (frameCountdown = 30; frameCountdown >= 0; frameCountdown--) {
    emitter->y.fixed += PIXELS(1);
    emitter->facing = (u16)(emitter->facing + 0x2000);
    emitter->scaleX += -2048;
    emitter->scaleY += -2048;
    Engine_TaskWait(1);
  }

  for (frameCountdown = 7; frameCountdown >= 0; frameCountdown--) {
    struct FieldActor *particle = Engine_ObjectCreate(
        0x11d, emitter->x.fixed, emitter->y.fixed, emitter->z.fixed);

    if (particle != 0) {
      s32 verticalSpeed;

      Engine_ActorSetSpriteFlags(particle, 0);
      Engine_ObjectSetScript(particle, (const void *)&gDustBurstScript[1]);

      verticalSpeed = Engine_RandomNext() + PIXELS(1);
      particle->acceleration = Q16_ONE;
      particle->speed = verticalSpeed;
      particle->motionFlags = 2;
      particle->gravity = 0x0a3d;

      particle->velocityY = Engine_RandomNext() - Engine_RandomNext();

      Effect_UpdateParticlePosition(
          particle, ((Engine_RandomNext() * 3) << 3) + PIXELS(8),
          Engine_RandomNext());
    }
  }

  Engine_AudioPlayCue(SE_131);

  emitter->x.fixed = 0;
  emitter->y.fixed = 0;
  emitter->z.fixed = 0;
  emitter->targetX = ACTOR_NO_TARGET;
  emitter->targetY = ACTOR_NO_TARGET;
  emitter->targetZ = ACTOR_NO_TARGET;
  SET_VELOCITY(emitter, 0, 0, 0);
}

/// Moves a particle outward in the horizontal plane without changing its
/// height.
static void Effect_UpdateParticlePosition(struct FieldActor *particle,
                                          s32 distance, s32 angle) {
  s32 position[3];
  if (particle != 0) {
    SET_VECTOR(position, particle->x.fixed, particle->y.fixed,
               particle->z.fixed);
    Vector_AddPolarOffset(distance, angle, position);
    Engine_ObjectSetPosition(particle, position[0], position[1], position[2]);
  }
}

/// Pushing the object in Vale (actor 20; 21 after Kalay; touch cells 30 and
/// 31) moves it a cell.
static void Event_PushObject(void) {
#if !TBS_PATCH_1
  // Japanese edition only: look up a villager but do not use the return value.
  if (Engine_GameFlagIsSet(FLAG_REACHED_KALAY))
    Object_GetById(ACTOR_DORA);
  else
    Object_GetById(20);
#endif
  Map_CopyCellAttributes(17, 0, 3, 1, 22, 36);
  StagedActor_AdvancePair();
  Event_SettleMovedObject();
}

/// After Move or a push, record the object's column in group flags 0x314 to
/// 0x316 and block the cell it now stands on.
static void Event_SettleMovedObject(void) {
  struct FieldActor *actor;
  s32 column;

  Map_CopyCellAttributes(17, 0, 3, 1, 22, 36);
  if (Engine_GameFlagIsSet(FLAG_REACHED_KALAY))
    actor = Object_GetById(21);
  else
    actor = Object_GetById(20);
  if (actor == NULL)
    return;
  GameFlag_Clear(0x314);
  GameFlag_Clear(0x315);
  GameFlag_Clear(0x316);
  column = actor->x.fixed >> 20;
  if (column == 22) {
    Map_CopyCellAttributes(17, 1, 1, 1, column, 36);
    Engine_GameFlagSet(0x314);
  } else if (column == 23) {
    Map_CopyCellAttributes(17, 1, 1, 1, column, 36);
    Engine_GameFlagSet(0x315);
  } else {
    Map_CopyCellAttributes(17, 1, 1, 1, 24, 36);
    Engine_GameFlagSet(0x316);
  }
}

static void Effect_PlayStepSound(void) {
  if ((*(u32 *)&gFrameCount & 15) == 0)
    Engine_AudioPlayCue(SE_131);
}

/// Searching a locked door in Vale (search code 24).
static void Event_SearchTheLockedDoor(void) {
  Engine_EventBegin();
  Engine_MessageShowCentered(MsgValeDoorWontOpen, 1);
  Engine_EventEnd();
}

/// Pauses the Aleph villager while the corresponding event flag is set.
static void Vale_SynchronizeAlephVillagerPause(void) {
  s32 pauseFlag;

  if (GameFlag_IsSet(FLAG_SCENE_TEMP_41) != 0) {
    pauseFlag = GameFlag_IsSet(FLAG_MENU_OPEN);
    if (pauseFlag == 0) {
      Object_GetById(22)->paused = pauseFlag;
      GameFlag_Clear(FLAG_SCENE_TEMP_41);
    }
  } else {
    if (GameFlag_IsSet(FLAG_MENU_OPEN) != 0) {
      Object_GetById(22)->paused = 1;
      GameFlag_Set(FLAG_SCENE_TEMP_41);
    }
  }
}

/// Keeps raised villager priority consistent with the leader height.
static void Vale_AdjustVillagerPriorityForLeaderHeight(void) {
  struct FieldActor *leader = Object_GetById(ACTOR_PARTY_LEADER);
  struct FieldActor *villager;
  if (Engine_GameFlagIsSet(FLAG_REACHED_KALAY) != 0)
    villager = Object_GetById(21);
  else
    villager = Object_GetById(20);
  if (villager != 0) {
    if (leader->y.fixed > PIXELS(200))
      villager->priorityFlags = 3;
    else
      villager->priorityFlags = 1;
  }
}

#include "common/object_orbit.h"

static void Vale_FlashActorAndSpawnEffects(struct FieldActor *actor) {

  if ((gFrameCount & 2) != 0)
    Engine_ObjectSetPartPalettes(actor, 7);
  else
    Engine_ObjectSetPartPalettes(actor, 0);
  if ((gFrameCount & 0xf) == 0)
    WorldMap_CreateLinkedEffects(actor);
}

static void Vale_CycleActorPaletteAndSpawnEffects(struct FieldActor *actor) {
  if ((gFrameCount & 1) != 0)
    Engine_ObjectSetPartPalettes(actor, (gFrameCount >> 1) % 6);
  if ((gFrameCount & 0xf) == 0)
    WorldMap_CreateLinkedEffects(actor);
}

static void Vale_CycleActorPalette(struct FieldActor *actor) {
  if ((gFrameCount & 1) != 0)
    Engine_ObjectSetPartPalettes(actor, (gFrameCount >> 1) % 6);
}

#include "common/effect_vertical.h"
#include "common/world_map_linked_effects.h"

/// Pixel step by direction: X in the upper halfword, Z in the lower.
#define DIRECTION_STEP(dx, dz) (((dx) << 16) | ((dz) & 0xffff))

s32 gStagedActor_DirectionSteps[] = {
    DIRECTION_STEP(16, 0),  DIRECTION_STEP(16, 0),  DIRECTION_STEP(16, 0),
    DIRECTION_STEP(0, 16),  DIRECTION_STEP(0, 16),  DIRECTION_STEP(0, 16),
    DIRECTION_STEP(0, 16),  DIRECTION_STEP(-16, 0), DIRECTION_STEP(-16, 0),
    DIRECTION_STEP(-16, 0), DIRECTION_STEP(-16, 0), DIRECTION_STEP(0, -16),
    DIRECTION_STEP(0, -16), DIRECTION_STEP(0, -16), DIRECTION_STEP(0, -16),
    DIRECTION_STEP(16, 0),
};

// clang-format off
u32 gStagedActor_FootprintKinds[] = {
    207, 205, 228, 229, 298, 297,
};

s32 gStagedActor_FootprintBounds[] = {
    -32, -8, 32, 8,
    -8, -32, 8, 32,
    -32, -16, 32, 0,
    -8, -32, 8, 32,
    -32, -8, 32, 8,
    -8, -32, 8, 32,
};

s32 gValeActor22Actions[] = {
    SC_SET(SW_SPEED, 0x8000),
    SC_SET(SW_ACCEL, 0x4000),
    SC_SET(0x23, 2),
    SC_LABEL(1),
    SC_ARG(2),
    SC_MOVE(PIXELS(296), 0, PIXELS(456)),
    SC_SYNC,
    SC_SET(0x23, 3),
    SC_MOVE(PIXELS(296), 0, PIXELS(420)),
    SC_SYNC,
    SC_ARG(1),
    SC_WAIT(10),
    SC_SET(SW_FACING, FACING_NORTHWEST),
    SC_WAIT(5),
    SC_SET(SW_FACING, FACING_WEST),
    SC_WAIT(10),
    SC_SET(SW_FACING, FACING_SOUTHWEST),
    SC_WAIT(5),
    SC_SET(SW_FACING, FACING_SOUTH),
    SC_WAIT(5),
    SC_SET(SW_FACING, FACING_SOUTHEAST),
    SC_WAIT(5),
    SC_SET(SW_FACING, FACING_EAST),
    SC_WAIT(10),
    SC_ARG(2),
    SC_MOVE(PIXELS(296), 0, PIXELS(456)),
    SC_SYNC,
    SC_SET(0x23, 2),
    SC_MOVE(PIXELS(256), 0, PIXELS(456)),
    SC_SYNC,
    SC_ARG(1),
    SC_WAIT(15),
    SC_SET(SW_FACING, FACING_SOUTHWEST),
    SC_WAIT(5),
    SC_SET(SW_FACING, FACING_SOUTH),
    SC_WAIT(60),
    SC_ARG(2),
    SC_MOVE(PIXELS(212), 0, PIXELS(456)),
    SC_SYNC,
    SC_SET(0x23, 1),
    SC_MOVE(PIXELS(212), 0, PIXELS(388)),
    SC_SYNC,
    SC_ARG(1),
    SC_WAIT(20),
    SC_SET(SW_FACING, FACING_NORTHWEST),
    SC_WAIT(5),
    SC_SET(SW_FACING, FACING_WEST),
    SC_WAIT(40),
    SC_ARG(2),
    SC_MOVE(PIXELS(212), 0, PIXELS(429)),
    SC_SYNC,
    SC_ARG(1),
    SC_WAIT(10),
    SC_SET(SW_FACING, FACING_SOUTHWEST),
    SC_WAIT(5),
    SC_SET(SW_FACING, FACING_WEST),
    SC_WAIT(30),
    SC_SET(SW_FACING, FACING_NORTHWEST),
    SC_WAIT(5),
    SC_SET(SW_FACING, FACING_NORTH),
    SC_WAIT(20),
    SC_SET(SW_FACING, FACING_NORTHWEST),
    SC_WAIT(5),
    SC_SET(SW_FACING, FACING_WEST),
    SC_WAIT(5),
    SC_ARG(2),
    SC_MOVE(PIXELS(212), 0, PIXELS(456)),
    SC_SYNC,
    SC_SET(0x23, 2),
    SC_MOVE(PIXELS(256), 0, PIXELS(456)),
    SC_SYNC,
    SC_ARG(1),
    SC_WAIT(10),
    SC_SET(SW_FACING, FACING_SOUTHEAST),
    SC_WAIT(5),
    SC_SET(SW_FACING, FACING_SOUTH),
    SC_WAIT(40),
    SC_SET(SW_FACING, FACING_SOUTHWEST),
    SC_WAIT(5),
    SC_SET(SW_FACING, FACING_WEST),
    SC_WAIT(20),
    SC_SET(SW_FACING, FACING_SOUTHWEST),
    SC_WAIT(5),
    SC_SET(SW_FACING, FACING_SOUTH),
    SC_WAIT(5),
    SC_LOOP(SC_FOREVER, SC_TO(1)),
};

s32 gLeaderHammerAction[] = {
    SC_LABEL(1),
    SC_ADD(SW_SCALE_Y, 1966),
    SC_WAIT(12),
    SC_SET(SW_SCALE_Y, 0x10000),
    SC_WAIT(12),
    SC_LOOP(SC_FOREVER, SC_TOP),
    SC_END,
};

s32 gGaretAction[] = {
    SC_ARG(2),
    SC_MOVE(PIXELS(300), 0, PIXELS(880)),
    SC_SYNC,
    SC_MOVE(PIXELS(300), 0, PIXELS(904)),
    SC_SYNC,
    SC_ARG(1),
    SC_SET(SW_FACING, 0xd000),
    SC_END,
};

s32 gJennaAction[] = {
    SC_ARG(2),
    SC_MOVE(PIXELS(300), 0, PIXELS(904)),
    SC_SYNC,
    SC_MOVE(PIXELS(321), 0, PIXELS(904)),
    SC_SYNC,
    SC_ARG(1),
    SC_SET(SW_FACING, 0xd000),
    SC_END,
};

s32 gVillagerAction[] = {
    SC_ARG(1),
    SC_POS(PIXELS(266), 0, PIXELS(1230)),
    SC_SET(SW_SCALE_X, 0x1999),
    SC_SET(SW_SCALE_Y, 0x1999),
    SC_WAIT(1),
    SC_LABEL(1),
    SC_ADD(SW_SCALE_X, 3932),
    SC_ADD(SW_SCALE_Y, 0x147a),
    SC_WAIT(1),
    SC_LOOP(24, SC_TO(1)),
    SC_POS(0, 0, 0),
    SC_WAIT(1),
    SC_END,
};

s32 gDustBurstScript[] = {
    SC_FREE,
    SC_SYNC,
    SC_SET(SW_SPEED, 0x1000),
    SC_ARG(0),
    SC_WAIT(10),
    SC_FREE,
};

struct SceneEntrance gVale_Entrances[] = {
    { 0, CONDITION_ALWAYS, 167, 0, 1281, FACING_SOUTH, 0, -1, -1, -1, -1, 0 },
    { 1, CONDITION_ALWAYS, 256, 0, 440, FACING_SOUTH, 0, -1, -1, -1, -1, 0 },
    { 2, CONDITION_ALWAYS, 113, 0, 303, FACING_SOUTH, 0, -1, -1, -1, -1, 0 },
    { 3, CONDITION_ALWAYS, 38, 0, 636, FACING_EAST, 0, -1, -1, -1, -1, 0 },
    { 4, CONDITION_ALWAYS, 29, 0, 792, FACING_EAST, 0, -1, -1, -1, -1, 0 },
    { 5, CONDITION_ALWAYS, 458, 0, 1393, FACING_WEST, 0, -1, -1, -1, -1, 0 },
    { 6, CONDITION_ALWAYS, 406, 0, 743, FACING_SOUTH, 0, -1, -1, -1, -1, 0 },
    { 7, CONDITION_ALWAYS, 262, 0, 821, FACING_SOUTH, 0, -1, -1, -1, -1, 0 },
    { 8, CONDITION_ALWAYS, 340, 0, 904, FACING_SOUTH, 0, -1, -1, -1, -1, 0 },
    { 9, CONDITION_ALWAYS, 326, 0, 1142, FACING_SOUTH, 0, -1, -1, -1, -1, 0 },
    { 10, CONDITION_ALWAYS, 374, 0, 1254, FACING_SOUTH, 0, -1, -1, -1, -1, 0 },
    { 11, CONDITION_ALWAYS, 102, 0, 1222, FACING_SOUTH, 0, -1, -1, -1, -1, 0 },
    { 16, CONDITION_ALWAYS, 400, 0, 852, FACING_NORTH, 0, -1, -1, -1, -1, 0 },
    { 17, CONDITION_ALWAYS, 400, 0, 852, FACING_NORTH, 0, -1, -1, -1, -1, 0 },
    { SCENE_TABLE_END },
};

u32 gVale_Exits[] = {
    SCENE_EXITS(Vale),
    SCENE_EXIT(VALE_TO_SOL_ENTRANCE, SolEntrance, SOL_ENTRANCE_FROM_VALE),
    SCENE_EXIT(VALE_TO_VALE_HOME, ValeHome, VALE_HOME_FROM_VALE),
    SCENE_EXIT(3, ValeKraden, VALE_KRADEN_FROM_VALE_NORTH),
    SCENE_EXIT(4, ValeKraden, VALE_KRADEN_FROM_VALE_SOUTH),
    SCENE_EXIT(VALE_TO_VALE_INTERIORS_EAST, ValeInteriors, VALE_INTERIORS_FROM_VALE_EAST),
    SCENE_EXIT(6, ValeInteriors, VALE_INTERIORS_FROM_VALE_SOUTH),
    SCENE_EXIT(VALE_TO_VALE_BABI, ValeBabi, VALE_BABI_FROM_VALE),
    SCENE_EXIT(VALE_TO_VALE_INTERIORS_NORTH, ValeInteriors, VALE_INTERIORS_FROM_VALE_NORTH),
    SCENE_EXIT(9, ValeInteriors, VALE_INTERIORS_FROM_VALE_WEST),
    SCENE_EXIT(10, ValeInteriors, VALE_INTERIORS_FROM_VALE_NORTH),
    SCENE_EXIT(VALE_TO_SANCTUM, Sanctum, SANCTUM_FROM_VALE),
    SCENE_EXIT(12, Vale, 17),
    SCENE_EXITS_END,
};

struct ScenePlacement gVale_Placements[] = {
    { GARET, CONDITION_ALWAYS, ACTOR_STAND,
        0, 0, 0, FACING_NORTH, TALK_FACE_PARTY, 0 },
    { JENNA, CONDITION_ALWAYS, ACTOR_STAND,
        0, 0, 0, FACING_NORTH, TALK_FACE_PARTY, 0 },
    { 0x6f, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(132), 0, PIXELS(908), FACING_NORTH, TALK_FACE_PARTY_AND_BACK, 0 },
    { 0x73, CONDITION_ALWAYS, 3,
        PIXELS(92), 0, PIXELS(508), FACING_SOUTH, TALK_FACE_PARTY, 0 },
    { 0x66, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(252), 0, PIXELS(620), FACING_WEST, TALK_FACE_PARTY, 0 },
    { 0x6c, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(164), 0, PIXELS(1236), FACING_NORTH, TALK_FACE_PARTY_AND_BACK, 0 },
    { 0x74, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(268), 0, PIXELS(1276), FACING_WEST, TALK_FACE_PARTY_AND_BACK, 0 },
    { 0x65, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(276), 0, PIXELS(1044), FACING_NORTH, TALK_FACE_PARTY, 0 },
    { 0x6a, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(19), 0, PIXELS(614), FACING_EAST, TALK_FACE_PARTY, 0 },
    { 0x68, CONDITION_ALWAYS, 4,
        PIXELS(50), 0, PIXELS(795), FACING_WEST, TALK_FACE_PARTY, 0 },
    { 0x68, CONDITION_ALWAYS, 4,
        PIXELS(65), 0, PIXELS(800), FACING_WEST, TALK_FACE_PARTY, 0 },
    { 0x68, CONDITION_ALWAYS, 4,
        PIXELS(57), 0, PIXELS(805), FACING_WEST, TALK_FACE_PARTY, 0 },
    { 0x2a, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(412), 0, PIXELS(586), FACING_WEST, TALK_FACE_PARTY_AND_BACK, 0 },
    { 0x25, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(398), 0, PIXELS(616), FACING_NORTH, TALK_FACE_PARTY_AND_BACK, 0 },
    { 0xd3, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(378), 0, PIXELS(584), FACING_EAST, TALK_KEEP_FACING, 0 },
    { 0x1f, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(373), 0, PIXELS(VALE_SOUTH_VILLAGER_Z), FACING_SOUTH, TALK_FACE_PARTY, 0 },
    { 0x7a, CONDITION_ALWAYS, ACTOR_STAND,
        0, 0, 0, FACING_EAST, TALK_FACE_PARTY, 0 },
    { 0xce, CONDITION_ALWAYS, 7,
        0, 0, 0, FACING_EAST, TALK_FACE_PARTY, 0 },
    { 0xce, CONDITION_ALWAYS, 7,
        0, 0, 0, FACING_EAST, TALK_FACE_PARTY, 0 },
    { 0xce, CONDITION_ALWAYS, 7,
        0, 0, 0, FACING_EAST, TALK_FACE_PARTY, 0 },
    { 0x16, 0xfd0, 7,
        PIXELS(152), 0, PIXELS(1016), FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { SCENE_TABLE_END },
};

struct ScenePlacement gVale_PlacementsAfterLeaving[] = {
    { JENNA, CONDITION_ALWAYS, ACTOR_STAND,
        0, 0, 0, FACING_NORTH, TALK_FACE_PARTY, 0 },
    { GARET, CONDITION_ALWAYS, ACTOR_STAND,
        0, 0, 0, FACING_NORTH, TALK_FACE_PARTY, 0 },
    { 0x6f, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(132), 0, PIXELS(908), FACING_NORTH, TALK_FACE_PARTY_AND_BACK, 0 },
    { 0x73, CONDITION_ALWAYS, 3,
        PIXELS(92), 0, PIXELS(508), FACING_SOUTH, TALK_FACE_PARTY, 0 },
    { 0x66, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(252), 0, PIXELS(620), FACING_WEST, TALK_FACE_PARTY, 0 },
    { 0x6c, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(164), 0, PIXELS(1236), FACING_NORTH, TALK_FACE_PARTY_AND_BACK, 0 },
    { 0x74, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(268), 0, PIXELS(1276), FACING_WEST, TALK_FACE_PARTY_AND_BACK, 0 },
    { 0x65, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(276), 0, PIXELS(1044), FACING_NORTH, TALK_FACE_PARTY, 0 },
    { 0x6a, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(19), 0, PIXELS(614), FACING_EAST, TALK_FACE_PARTY, 0 },
    { 0x68, CONDITION_ALWAYS, 4,
        PIXELS(50), 0, PIXELS(795), FACING_WEST, TALK_FACE_PARTY, 0 },
    { 0x68, CONDITION_ALWAYS, 4,
        PIXELS(65), 0, PIXELS(800), FACING_WEST, TALK_FACE_PARTY, 0 },
    { 0x68, CONDITION_ALWAYS, 4,
        PIXELS(57), 0, PIXELS(805), FACING_WEST, TALK_FACE_PARTY, 0 },
    { 0x2a, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(412), 0, PIXELS(586), FACING_WEST, 3, 0 },
    { 0x25, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(398), 0, PIXELS(616), FACING_NORTH, 3, 0 },
    { 0xd3, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(378), 0, PIXELS(584), FACING_EAST, TALK_KEEP_FACING, 0 },
    { 0x1f, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(335), 0, PIXELS(869), FACING_SOUTH, TALK_FACE_PARTY, 0 },
    { 0x7a, CONDITION_ALWAYS, ACTOR_STAND,
        0, 0, 0, FACING_EAST, TALK_FACE_PARTY, 0 },
    { 0xce, CONDITION_ALWAYS, 7,
        0, 0, 0, FACING_EAST, TALK_FACE_PARTY, 0 },
    { 0xce, CONDITION_ALWAYS, 7,
        0, 0, 0, FACING_EAST, TALK_FACE_PARTY, 0 },
    { 0xce, CONDITION_ALWAYS, 7,
        0, 0, 0, FACING_EAST, TALK_FACE_PARTY, 0 },
    { 0x16, 0xfd0, 7,
        PIXELS(152), 0, PIXELS(1016), FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { SCENE_TABLE_END },
};

struct ScenePlacement gVale_PlacementsAfterKalay[] = {
    { GARET, CONDITION_ALWAYS, ACTOR_STAND,
        0, 0, 0, FACING_NORTH, TALK_FACE_PARTY, 0 },
    { 0x2a, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(412), 0, PIXELS(586), FACING_NORTH + FACING_STEP, 3, 0 },
    { 0x25, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(356), 0, PIXELS(727), FACING_NORTHWEST + FACING_STEP, TALK_FACE_PARTY, 0 },
    { 0x6c, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(164), 0, PIXELS(1236), FACING_NORTH, TALK_FACE_PARTY_AND_BACK, 0 },
    { 0x74, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(268), 0, PIXELS(1276), FACING_WEST, TALK_KEEP_FACING, 0 },
    { 0xd6, CONDITION_ALWAYS, ACTOR_STAND,
        0, 0, 0, FACING_SOUTH, TALK_FACE_PARTY, 0 },
    { 0xd6, CONDITION_ALWAYS, ACTOR_STAND,
        0, 0, 0, FACING_SOUTH, TALK_FACE_PARTY, 0 },
    { 0xd6, CONDITION_ALWAYS, ACTOR_STAND,
        0, 0, 0, FACING_SOUTH, TALK_FACE_PARTY, 0 },
    { 0x6a, CONDITION_ALWAYS, ACTOR_STAND,
        0, 0, 0, FACING_EAST, TALK_FACE_PARTY, 0 },
    { 0x6f, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(132), 0, PIXELS(908), FACING_NORTH, TALK_FACE_PARTY_AND_BACK, 0 },
    { 0x73, CONDITION_ALWAYS, 3,
        PIXELS(92), 0, PIXELS(508), FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { 0x66, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(252), 0, PIXELS(620), FACING_WEST, TALK_FACE_PARTY_AND_BACK, 0 },
    { 0x65, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(276), 0, PIXELS(1044), FACING_NORTH, TALK_KEEP_FACING, 0 },
    { 0x16, 0xfd0, ACTOR_STAND,
        PIXELS(152), 0, PIXELS(1016), FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { 0xd3, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(378), 0, PIXELS(584), FACING_EAST, TALK_KEEP_FACING, 0 },
    { SCENE_TABLE_END },
};

struct ScenePlacement gVale_RepairPlacements[] = {
    { JENNA, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(409), 0, PIXELS(1134), FACING_NORTH, TALK_FACE_PARTY, 0 },
    { GARET, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(428), 0, PIXELS(588), FACING_WEST, TALK_FACE_PARTY, 0 },
    { 0x73, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(204), 0, PIXELS(924), FACING_EAST, TALK_FACE_PARTY, 0 },
    { 0xd3, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(394), 0, PIXELS(584), FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { 0x1f, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(373), 0, PIXELS(VALE_SOUTH_VILLAGER_Z), FACING_NORTH, TALK_FACE_PARTY, 0 },
    { 0xce, CONDITION_ALWAYS, ACTOR_STAND,
        0, 0, 0, FACING_SOUTH, TALK_FACE_PARTY, 0 },
    { 0xce, CONDITION_ALWAYS, ACTOR_STAND,
        0, 0, 0, FACING_SOUTH, TALK_FACE_PARTY, 0 },
    { 0xce, CONDITION_ALWAYS, ACTOR_STAND,
        0, 0, 0, FACING_SOUTH, TALK_FACE_PARTY, 0 },
    { 0xd7, CONDITION_ALWAYS, ACTOR_STAND,
        0, 0, 0, FACING_SOUTH, TALK_FACE_PARTY, 0 },
    { SCENE_TABLE_END },
};

struct SceneEvent gVale_Events[] = {
    { EVENT_TALK, 8, CONDITION_ALWAYS, MSGNO(MsgValeMtAlephLooksBeautifulTodayIts) },
    { EVENT_TALK, 9, CONDITION_ALWAYS, MSGNO(MsgValeValeStaysPristineBecauseOfMt) },
    { EVENT_TALK, 10, CONDITION_ALWAYS, EVENT_SCRIPT(Event_TalkAboutTheMayorAndTremors) },
    { EVENT_TALK, 11, CONDITION_ALWAYS, MSGNO(MsgValeThisPlaceLooksJustLikeIt) },
    { EVENT_TALK, 12, CONDITION_ALWAYS, MSGNO(MsgValeIsaacWatchOutDontGetToo) },
    { EVENT_TALK, 13, CONDITION_ALWAYS, MSGNO(MsgValeValeMustRemainHiddenPsynergysSecrets) },
    { EVENT_TALK, 14, CONDITION_ALWAYS, EVENT_SCRIPT(Event_WarnAboutGoingBeyondKradensCottage) },
    { EVENT_TALK, 18, CONDITION_ALWAYS, EVENT_SCRIPT(Event_ArgueWithTheAngryVillager) },
    { EVENT_TALK, 19, CONDITION_ALWAYS, MSGNO(MsgValeYoureInTroubleNowGaretYour) },
    { EVENT_TALK, 21, CONDITION_ALWAYS, EVENT_SCRIPT(Event_DoraFretsAboutTheRoof) },
    { EVENT_TALK, 22, CONDITION_ALWAYS, MSGNO(MsgValeMtAlephIsASacredPeak) },
    { TOUCH_WALKING_INTO(FACING_NORTH), 1, CONDITION_ALWAYS, EVENT_SCRIPT(Event_EnterSanctum) },
    { EVENT_TOUCH, 2, CONDITION_ALWAYS, EVENT_SCRIPT(Event_ApproachMtAleph) },
    { EVENT_TOUCH, 3, CONDITION_ALWAYS, EVENT_SCRIPT(Event_TakeExit3) },
    { EVENT_TOUCH, 4, CONDITION_ALWAYS, EVENT_SCRIPT(Event_TakeExit4) },
    { EVENT_TOUCH, 5, CONDITION_ALWAYS, EVENT_SCRIPT(Event_TakeExit2) },
    { TOUCH_WALKING_INTO(FACING_NORTH), 6, CONDITION_ALWAYS, EVENT_SCRIPT(Event_EnterGaretsHouse) },
    { TOUCH_WALKING_INTO(FACING_NORTH), 7, CONDITION_ALWAYS, EVENT_SCRIPT(Event_EnterInteriorsRoom5) },
    { TOUCH_WALKING_INTO(FACING_NORTH), 8, CONDITION_ALWAYS, EVENT_SCRIPT(Event_EnterIsaacsHouse) },
    { TOUCH_WALKING_INTO(FACING_NORTH), 9, CONDITION_ALWAYS, EVENT_SCRIPT(Event_EnterJennasGrandparentsHouse) },
    { TOUCH_WALKING_INTO(FACING_NORTH), 10, CONDITION_ALWAYS, EVENT_SCRIPT(Event_EnterInteriorsRoom1) },
    { TOUCH_WALKING_INTO(FACING_NORTH), 11, CONDITION_ALWAYS, EVENT_SCRIPT(Event_EnterJennasGrandparentsHouseByExit10) },
    { EVENT_TOUCH, 15, CONDITION_ALWAYS, EVENT_SCRIPT(Event_SetPathCellOverride) },
    { EVENT_TOUCH, 14, CONDITION_ALWAYS, EVENT_SCRIPT(Event_ClearPathCellOverride) },
    { EVENT_TOUCH, 20, CONDITION_ALWAYS, EVENT_SCRIPT(Event_WatchThePuppies) },
    { EVENT_TOUCH, 21, CONDITION_ALWAYS, EVENT_SCRIPT(Event_TurnBackFromThePuppies) },
    { EVENT_TOUCH, 50, FLAG_VALE_WARNED_AGAINST_MT_ALEPH, EVENT_SCRIPT(Event_WarnAgainstSneakingUpMtAleph) },
    { EVENT_TOUCH, 51, CONDITION_ALWAYS, EVENT_SCRIPT(Event_RepeatMtAlephWarningFromFirstApproach) },
    { EVENT_TOUCH, 52, CONDITION_ALWAYS, EVENT_SCRIPT(Event_RepeatMtAlephWarningFromSecondApproach) },
    { EVENT_TOUCH, 53, CONDITION_ALWAYS, EVENT_SCRIPT(Event_RepeatMtAlephWarningAtEitherPause) },
    { TOUCH_WALKING_INTO(FACING_NORTH), 100, CONDITION_ALWAYS, EVENT_SCRIPT(Event_EnterNorthernPassage) },
    { TOUCH_WALKING_INTO(FACING_SOUTH), 101, CONDITION_ALWAYS, EVENT_SCRIPT(Event_ReturnThroughSouthernPassage) },
    { PSYNERGY_ON_ACTOR(ABILITY_CATCH), 26, 0xfd0, EVENT_SCRIPT(Event_CatchNut) },
    { PSYNERGY_ON_ACTOR(ABILITY_MOVE), 20, CONDITION_ALWAYS, EVENT_SCRIPT(Event_SettleMovedObject) },
    { TOUCH_WALKING_INTO(FACING_EAST), 30, CONDITION_ALWAYS, EVENT_SCRIPT(Event_PushObject) },
    { TOUCH_WALKING_INTO(FACING_WEST), 31, CONDITION_ALWAYS, EVENT_SCRIPT(Event_PushObject) },
    { SEARCH(SEARCH_UNNAMED), 24, CONDITION_ALWAYS, EVENT_SCRIPT(Event_SearchTheLockedDoor) },
    { SCENE_EVENTS_END },
};

struct SceneEvent gVale_EventsAfterLeaving[] = {
    { EVENT_TALK, 8, CONDITION_ALWAYS, MSGNO(MsgValeValeIsSavedThanksToThe) },
    { EVENT_TALK, 9, CONDITION_ALWAYS, MSGNO(MsgValeEverSinceTheTremorsStartedMt) },
    { EVENT_TALK, 10, CONDITION_ALWAYS, EVENT_SCRIPT(Event_TalkAboutTheMayorAndTremors) },
    { EVENT_TALK, 11, CONDITION_ALWAYS, MSGNO(MsgValeSighNoLongerWillISee) },
    { EVENT_TALK, 12, CONDITION_ALWAYS, MSGNO(MsgValeIDontBlameFelixIfOnly) },
    { EVENT_TALK, 13, CONDITION_ALWAYS, MSGNO(MsgValeTheMayorWasTooKindTo) },
    { EVENT_TALK, 14, CONDITION_ALWAYS, EVENT_SCRIPT(Event_WarnAboutGoingBeyondKradensCottage) },
    { EVENT_TALK, 18, CONDITION_ALWAYS, MSGNO(MsgValeYouGuysAreSoLuckyGaret) },
    { EVENT_TALK, 19, CONDITION_ALWAYS, MSGNO(MsgValePleaseTakeCareOfMyBrother) },
    { EVENT_TALK, 22, CONDITION_ALWAYS, MSGNO(MsgValeItIsVeryDangerousToGo) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 22, CONDITION_ALWAYS, MSGNO(MsgValeIAmAllAloneInThis) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 18, CONDITION_ALWAYS, MSGNO(MsgValeGeeIWishICouldSee) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 19, CONDITION_ALWAYS, MSGNO(MsgValeMyBrothersStrengthIsHisBest) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 11, CONDITION_ALWAYS, MSGNO(MsgValeImGladThatFelixIsAlive) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 12, CONDITION_ALWAYS, MSGNO(MsgValeFelixWasAGoodPersonI) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 14, CONDITION_ALWAYS, MSGNO(MsgValePoorKradenWasTakenToSome) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 8, CONDITION_ALWAYS, MSGNO(MsgValeValeWasSavedBecausePeopleLike) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 9, CONDITION_ALWAYS, MSGNO(MsgValeICantBelieveAnyoneWouldClimb) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 10, CONDITION_ALWAYS, MSGNO(MsgValeIJustDontFeelSafeKnowing) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 13, CONDITION_ALWAYS, MSGNO(MsgValeThePeopleOfValeWillNever) },
    { TOUCH_WALKING_INTO(FACING_NORTH), 1, CONDITION_ALWAYS, EVENT_SCRIPT(Event_EnterSanctum) },
    { EVENT_TOUCH, 2, CONDITION_ALWAYS, EVENT_SCRIPT(Event_ApproachMtAleph) },
    { EVENT_TOUCH, 3, CONDITION_ALWAYS, EVENT_SCRIPT(Event_TakeExit3) },
    { EVENT_TOUCH, 4, CONDITION_ALWAYS, EVENT_SCRIPT(Event_TakeExit4) },
    { EVENT_TOUCH, 5, CONDITION_ALWAYS, EVENT_SCRIPT(Event_TakeExit2) },
    { TOUCH_WALKING_INTO(FACING_NORTH), 6, CONDITION_ALWAYS, EVENT_SCRIPT(Event_EnterGaretsHouse) },
    { TOUCH_WALKING_INTO(FACING_NORTH), 7, CONDITION_ALWAYS, EVENT_SCRIPT(Event_EnterInteriorsRoom5) },
    { TOUCH_WALKING_INTO(FACING_NORTH), 8, CONDITION_ALWAYS, EVENT_SCRIPT(Event_EnterIsaacsHouse) },
    { TOUCH_WALKING_INTO(FACING_NORTH), 9, CONDITION_ALWAYS, EVENT_SCRIPT(Event_EnterJennasGrandparentsHouse) },
    { TOUCH_WALKING_INTO(FACING_NORTH), 10, CONDITION_ALWAYS, EVENT_SCRIPT(Event_EnterInteriorsRoom1) },
    { TOUCH_WALKING_INTO(FACING_NORTH), 11, CONDITION_ALWAYS, EVENT_SCRIPT(Event_EnterJennasGrandparentsHouseByExit10) },
    { EVENT_TOUCH, 15, CONDITION_ALWAYS, EVENT_SCRIPT(Event_SetPathCellOverride) },
    { EVENT_TOUCH, 14, CONDITION_ALWAYS, EVENT_SCRIPT(Event_ClearPathCellOverride) },
    { EVENT_TOUCH, 20, CONDITION_ALWAYS, EVENT_SCRIPT(Event_WatchThePuppies) },
    { EVENT_TOUCH, 21, CONDITION_ALWAYS, EVENT_SCRIPT(Event_TurnBackFromThePuppies) },
    { EVENT_TOUCH, 50, FLAG_VALE_WARNED_AGAINST_MT_ALEPH, EVENT_SCRIPT(Event_WarnAgainstSneakingUpMtAleph) },
    { EVENT_TOUCH, 51, CONDITION_ALWAYS, EVENT_SCRIPT(Event_RepeatMtAlephWarningFromFirstApproach) },
    { EVENT_TOUCH, 52, CONDITION_ALWAYS, EVENT_SCRIPT(Event_RepeatMtAlephWarningFromSecondApproach) },
    { EVENT_TOUCH, 53, CONDITION_ALWAYS, EVENT_SCRIPT(Event_RepeatMtAlephWarningAtEitherPause) },
    { TOUCH_WALKING_INTO(FACING_NORTH), 100, CONDITION_ALWAYS, EVENT_SCRIPT(Event_EnterNorthernPassage) },
    { TOUCH_WALKING_INTO(FACING_SOUTH), 101, CONDITION_ALWAYS, EVENT_SCRIPT(Event_ReturnThroughSouthernPassage) },
    { PSYNERGY_ON_ACTOR(ABILITY_CATCH), 26, 0xfd0, EVENT_SCRIPT(Event_CatchNut) },
    { PSYNERGY_ON_ACTOR(ABILITY_MOVE), 20, CONDITION_ALWAYS, EVENT_SCRIPT(Event_SettleMovedObject) },
    { TOUCH_WALKING_INTO(FACING_EAST), 30, CONDITION_ALWAYS, EVENT_SCRIPT(Event_PushObject) },
    { TOUCH_WALKING_INTO(FACING_WEST), 31, CONDITION_ALWAYS, EVENT_SCRIPT(Event_PushObject) },
    { SEARCH(SEARCH_UNNAMED), 24, CONDITION_ALWAYS, EVENT_SCRIPT(Event_SearchTheLockedDoor) },
    { SCENE_EVENTS_END },
};

struct SceneEvent gVale_EventsAfterKalay[] = {
    { EVENT_TALK, 8, 0x303, EVENT_SCRIPT(Event_GaretMeetsHisSister) },
    { EVENT_TALK, 8, CONDITION_ALWAYS, MSGNO(MsgValeICanTellJustByLooking) },
    { EVENT_TALK, 9, 0x304, EVENT_SCRIPT(Event_GaretAsksToProtectFamily) },
    { EVENT_TALK, 9, CONDITION_ALWAYS, MSGNO(MsgValeIWillDoMyBestTo) },
    { EVENT_TALK, 10, FLAG_VALE_ASKED_IF_JENNA_IS_BACK, EVENT_SCRIPT(Event_AskWhetherJennaHasReturned) },
    { EVENT_TALK, 10, CONDITION_ALWAYS, MSGNO(MsgValePromiseMeIsaacPromiseMeYoull) },
    { EVENT_TALK, 11, CONDITION_ALWAYS, MSGNO(MsgValeIUsedToPlayHere) },
    { EVENT_TALK, 16, CONDITION_ALWAYS, MSGNO(MsgValeMtAlephsEruptionWasSoPowerful) },
    { EVENT_TALK, 17, CONDITION_ALWAYS, EVENT_SCRIPT(Event_ReactToAnotherTremor) },
    { EVENT_TALK, 18, CONDITION_ALWAYS, MSGNO(MsgValeTheySayTheDangerIsGone) },
    { EVENT_TALK, 19, CONDITION_ALWAYS, EVENT_SCRIPT(Event_AskAboutTheLackOfTravelers) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 0x408, 0x303, EVENT_SCRIPT(Event_GaretMeetsHisSister) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 8, CONDITION_ALWAYS, MSGNO(MsgValeYouGuysAreWorkingSoHard) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 0x409, 0x304, EVENT_SCRIPT(Event_GaretAsksToProtectFamily) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 9, CONDITION_ALWAYS, MSGNO(MsgValeICouldGoWithYouIf) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 0x40a, FLAG_VALE_ASKED_IF_JENNA_IS_BACK, EVENT_SCRIPT(Event_AskWhetherJennaHasReturned) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 10, CONDITION_ALWAYS, MSGNO(MsgValeYouDidntBringJennaWithYou) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 11, CONDITION_ALWAYS, MSGNO(MsgValeFelixMustBeProtectingJennaIm) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 16, CONDITION_ALWAYS, MSGNO(MsgValeTheEarthquakesArentCausedByEruptions) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 17, CONDITION_ALWAYS, MSGNO(MsgValeAhhICantStandTheseEarthquakes) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 18, CONDITION_ALWAYS, MSGNO(MsgValeTheSmokeFromMtAlephIsnt) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 0x413, 0x307, EVENT_SCRIPT(Event_AskAboutTheLackOfTravelers) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 19, CONDITION_ALWAYS, MSGNO(MsgValeHavingNoTravelersIsNoFun) },
    { TOUCH_WALKING_INTO(FACING_NORTH), 1, CONDITION_ALWAYS, EVENT_SCRIPT(Event_EnterSanctum) },
    { EVENT_TOUCH, 2, CONDITION_ALWAYS, EVENT_SCRIPT(Event_ApproachMtAleph) },
    { EVENT_TOUCH, 3, CONDITION_ALWAYS, EVENT_SCRIPT(Event_TakeExit3) },
    { EVENT_TOUCH, 4, CONDITION_ALWAYS, EVENT_SCRIPT(Event_TakeExit4) },
    { EVENT_TOUCH, 5, CONDITION_ALWAYS, EVENT_SCRIPT(Event_TakeExit2) },
    { TOUCH_WALKING_INTO(FACING_NORTH), 6, CONDITION_ALWAYS, EVENT_SCRIPT(Event_EnterGaretsHouse) },
    { TOUCH_WALKING_INTO(FACING_NORTH), 7, CONDITION_ALWAYS, EVENT_SCRIPT(Event_EnterInteriorsRoom5) },
    { TOUCH_WALKING_INTO(FACING_NORTH), 8, CONDITION_ALWAYS, EVENT_SCRIPT(Event_EnterIsaacsHouse) },
    { TOUCH_WALKING_INTO(FACING_NORTH), 9, CONDITION_ALWAYS, EVENT_SCRIPT(Event_EnterJennasGrandparentsHouse) },
    { TOUCH_WALKING_INTO(FACING_NORTH), 10, CONDITION_ALWAYS, EVENT_SCRIPT(Event_EnterInteriorsRoom1) },
    { TOUCH_WALKING_INTO(FACING_NORTH), 11, CONDITION_ALWAYS, EVENT_SCRIPT(Event_EnterJennasGrandparentsHouseByExit10) },
    { EVENT_TOUCH, 15, CONDITION_ALWAYS, EVENT_SCRIPT(Event_SetPathCellOverride) },
    { EVENT_TOUCH, 14, CONDITION_ALWAYS, EVENT_SCRIPT(Event_ClearPathCellOverride) },
    { TOUCH_WALKING_INTO(FACING_NORTH), 100, CONDITION_ALWAYS, EVENT_SCRIPT(Event_EnterNorthernPassage) },
    { TOUCH_WALKING_INTO(FACING_SOUTH), 101, CONDITION_ALWAYS, EVENT_SCRIPT(Event_ReturnThroughSouthernPassage) },
    { PSYNERGY_ON_ACTOR(ABILITY_CATCH), 20, 0xfd0, EVENT_SCRIPT(Event_CatchNutAfterKalay) },
    { PSYNERGY_ON_ACTOR(ABILITY_MOVE), 21, CONDITION_ALWAYS, EVENT_SCRIPT(Event_SettleMovedObject) },
    { TOUCH_WALKING_INTO(FACING_EAST), 30, CONDITION_ALWAYS, EVENT_SCRIPT(Event_PushObject) },
    { TOUCH_WALKING_INTO(FACING_WEST), 31, CONDITION_ALWAYS, EVENT_SCRIPT(Event_PushObject) },
    { SEARCH(SEARCH_UNNAMED), 24, CONDITION_ALWAYS, EVENT_SCRIPT(Event_SearchTheLockedDoor) },
    { SCENE_EVENTS_END },
};
// clang-format on

u16 gValeCellAnimC[] = {
    MAP_CELL_STEP(0, 98, 2, 2, 2),
    MAP_CELL_STEP(2, 98, 2, 2, 2),
    MAP_CELL_STEPS_END,
};

u16 gValeCellAnimA[] = {
    MAP_CELL_STEP(0, 96, 2, 2, 2),
    MAP_CELL_STEP(2, 96, 2, 2, 2),
    MAP_CELL_STEPS_END,
};

u16 gValeCellAnimB[] = {
    MAP_CELL_STEP(4, 98, 2, 2, 2),
    MAP_CELL_STEP(4, 98, 2, 2, 2),
    MAP_CELL_STEPS_END,
};

u16 gValeCellAnimD[] = {
    MAP_CELL_STEP(4, 96, 2, 2, 2),
    MAP_CELL_STEP(4, 96, 2, 2, 2),
    MAP_CELL_STEPS_END,
};

s32 gValeLeaderWalkActions[] = {
    SC_ARG(5), SC_MOVE(PIXELS(199), 0, PIXELS(446)),
    SC_SYNC,   SC_MOVE(PIXELS(215), 0, PIXELS(472)),
    SC_SYNC,   SC_MOVE(PIXELS(239), 0, PIXELS(461)),
    SC_SYNC,   SC_ARG(1),
    SC_END,
};

s32 gValeActor22WalkActions[] = {
    SC_ARG(2), SC_MOVE(PIXELS(199), 0, PIXELS(446)),
    SC_SYNC,   SC_MOVE(PIXELS(215), 0, PIXELS(472)),
    SC_SYNC,   SC_MOVE(PIXELS(239), 0, PIXELS(461)),
    SC_SYNC,   SC_ARG(1),
    SC_END,
};

s32 gValePairTableA[] = {
    SC_SET(SW_SPEED, 0x20000),
    SC_SET(SW_ACCEL, 0x20000),
    SC_ARG(5),
    SC_MOVE(PIXELS(224), 0, PIXELS(458)),
    SC_SYNC,
    SC_MOVE(PIXELS(256), 0, PIXELS(458)),
    SC_SYNC,
    SC_MOVE(PIXELS(256), 0, PIXELS(474)),
    SC_SYNC,
    SC_ARG(1),
    SC_WAIT(5),
    SC_SET(SW_FACING, FACING_NORTHWEST),
    SC_WAIT(5),
    SC_SET(SW_FACING, FACING_NORTH),
    SC_END,
};

s32 gValePairTableB[] = {
    SC_SET(SW_SPEED, 0x18000),
    SC_SET(SW_ACCEL, 0x10000),
    SC_ARG(2),
    SC_MOVE(PIXELS(216), 0, PIXELS(458)),
    SC_SYNC,
    SC_MOVE(PIXELS(256), 0, PIXELS(456)),
    SC_SYNC,
    SC_ARG(1),
    SC_WAIT(5),
    SC_SET(SW_FACING, FACING_SOUTHEAST),
    SC_WAIT(5),
    SC_SET(SW_FACING, FACING_SOUTH),
    SC_END,
};

s32 gValePairTableC[] = {
    SC_SET(SW_SPEED, 0x20000),
    SC_SET(SW_ACCEL, 0x20000),
    SC_ARG(5),
    SC_MOVE(PIXELS(256), 0, PIXELS(458)),
    SC_SYNC,
    SC_MOVE(PIXELS(256), 0, PIXELS(474)),
    SC_SYNC,
    SC_ARG(1),
    SC_WAIT(5),
    SC_SET(SW_FACING, FACING_NORTHWEST),
    SC_WAIT(5),
    SC_SET(SW_FACING, FACING_NORTH),
    SC_END,
};

s32 gValePairTableD[] = {
    SC_SET(SW_SPEED, 0x18000),
    SC_SET(SW_ACCEL, 0x10000),
    SC_ARG(2),
    SC_MOVE(PIXELS(256), 0, PIXELS(456)),
    SC_SYNC,
    SC_ARG(1),
    SC_WAIT(5),
    SC_SET(SW_FACING, FACING_SOUTHEAST),
    SC_WAIT(5),
    SC_SET(SW_FACING, FACING_SOUTH),
    SC_END,
};
