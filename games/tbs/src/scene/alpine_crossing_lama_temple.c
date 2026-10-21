/// @file
/// The Alpine Crossing and Lama Temple, two places on Silk Road west of Xian
/// that share this overlay: the Alpine Crossing is AlpineCrossing
/// (scene 74) and Lama Temple is LamaTemple (scene 88). The
/// getters pick each scene's tables by gGameState.scene, both scenes share one
/// exit table, and Scene_Initialize sets up the scene being entered from the
/// story flags of Hsu's rescue.
///
/// The party comes here from Xian, where Feizhi, the daughter of Master Feh,
/// Xian's kung fu master, has foreseen that Hsu, Feh's apprentice, is in
/// danger on his way home from training under Master Hama at Lama Temple, and
/// has gone off alone to find him (FLAG_FEIZHI_WENT_TO_HSU, set in
/// fuchin_temple.c). At the Alpine Crossing boulders block Silk Road. Stepping
/// onto the road beside them (TRIGGER_FEIZHI_ARRIVAL) plays
/// AlpineCrossing_FeizhiArrives: Feizhi runs up behind the party, finds that
/// her vision has come true, calls for Hsu and runs ahead down the path with
/// the leader following, and cannot hear him (FLAG_FEIZHI_SAW_ROCKSLIDE). She
/// stays at the end of that path, asking whether she should go back to Xian,
/// until Hama's lesson. A signpost reads
/// `North: Altin Mine West: Lama Temple`; the Crossing opens onto the world
/// map to the east, north and west, and by two ways
/// (ALPINE_CROSSING_TO_ALTIN_PEAK_SOUTH_ENTRANCE_WEST and
/// ALPINE_CROSSING_TO_ALTIN_PEAK_SOUTH_ENTRANCE_EAST) into Altin Peak's
/// AltinPeakSouthEntrance, whose entrances from here the international editions
/// also call Alpine Crossing. With the road closed, the villagers of Altin, to
/// the north, send the party through their mine, Altin Peak (altin.c,
/// altin_peak.c), and the mine brings it back out here from
/// AltinPeakSouthEntrance (ALPINE_CROSSING_FROM_ALTIN_PEAK_SOUTH_ENTRANCE_WEST
/// and ALPINE_CROSSING_FROM_ALTIN_PEAK_SOUTH_ENTRANCE_EAST); coming in that way
/// the area name is not shown again.
///
/// Lama Temple, west along the road, is Master Hama's temple, the last stop
/// before the Lamakan Desert. Its five monks talk of the desert ahead, of
/// Master Feh and of Hama, the greatest master of Chi: one set of lines and
/// thoughts until Hsu is freed (FLAG_HSU_RESCUED), and advice for the desert
/// after. A traveling healer offers the Sanctum's services across his counter.
/// Walking north into the sliding door (LamaTemple_EnterHamasRoom) leads into
/// Hama's room, in FuchinTempleInteriors
/// (FUCHIN_TEMPLE_INTERIORS_FROM_LAMA_TEMPLE, in fuchin_temple.c): there Hama
/// teaches Ivan Reveal for crossing the desert, and Feizhi, who came through
/// the mine, bursts in to say that Hsu is trapped by the boulders in the
/// mountains; Hama hurries to his aid, and the party may follow
/// (FLAG_XIAN_HEARD_HSU_TRAPPED, set as the lesson ends). Coming back out of
/// the room (LAMA_TEMPLE_FROM_FUCHIN_TEMPLE_INTERIORS) the area name is not
/// shown again. Writing on a wall hints at Reveal, which shows a passage behind
/// a false wall (LamaTemple_RevealPassage); LamaTemple_ShowFalseWall puts the
/// wall back when Reveal wears off and on entry, the wall cell's own attributes
/// saved at (21, 73). The temple also holds a Psynergy Stone (place flag 0x350)
/// and a chest with Water of Life.
///
/// Back at the Alpine Crossing after the lesson, until Hsu is freed, Hama and
/// Feizhi stand by him, pinned between two boulders. Arriving beside him from
/// AltinPeakSouthEntrance (ALPINE_CROSSING_FROM_ALTIN_PEAK_SOUTH_ENTRANCE_EAST)
/// the first time, AlpineCrossing_HamaReachesHsu plays: he is unhurt but cannot
/// move, and no one knows how to free him (FLAG_HAMA_REACHED_HSU). Lift on the
/// boulder (AlpineCrossing_LiftBoulder), the Psynergy of the Lifting Gem found
/// in Altin Peak, makes it float while Hama and Feizhi pull him free; they will
/// ask Master Feh to tend him and clear the road, and the scene sets
/// FLAG_HSU_RESCUED and leaves for Lama Temple (ALPINE_CROSSING_TO_LAMA_TEMPLE,
/// to LAMA_TEMPLE_FROM_ALPINE_CROSSING), where Scene_Initialize plays Hama's
/// farewell (LamaTemple_RunHamasFarewell). If the party instead comes back by
/// the world map's north side (ALPINE_CROSSING_FROM_WORLD_MAP_NORTH), Hsu has
/// been freed without it (FLAG_HSU_RESCUED_WITHOUT_PARTY, which
/// lamakan_desert.c can set too), and Hama and Feizhi are gone. Once he is
/// free, two workers from Xian stand by the road they have cleared, and
/// Scene_Initialize copies the cleared cells over the rocks. The Crossing's
/// other puzzles are kept only within one visit (scene flags): Reveal shows a
/// hidden path, Frost freezes a puddle into a pillar the party can climb, and
/// Growth grows a sprout into a vine; a chest holds Power Bread.

#include "script.h"
#include "game_state.h"
#include "unit/scene/alpine_crossing_lama_temple.h"
#include "unit/object/object.h"
#include "unit/object/psynergy_effects.h"
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
#include "types.h"
#include "field_event.h"
#include "field_scene.h"
#include "scene_ids.h"
#include "call.h"
#include "text/msg_ids.h"
#include "ability_ids.h"
#include "flags.h"
#include "item_ids.h"
#include "objcmd.h"
#include "field_effect.h"
#include "songs.h"

// Place flags: Scene_ResetFlagsOnEnter clears 0x300 to 0x3ff when the party
// enters another place group, and other places use the numbers for their own.
#define FLAG_LAMA_TEMPLE_PSYNERGY_STONE 0x350 ///< the temple's Psynergy Stone

#define SCENE_ALPINE_CROSSING ((s32)&SceneId_AlpineCrossing)
#define SCENE_LAMA_TEMPLE ((s32)&SceneId_LamaTemple)

/// Lama Temple's actors. Garet, Ivan and Mia are placed in their own party
/// slots for Hama's farewell; the others count from ACTOR_FIRST_PLACED.
enum LamaTempleActor {
  ACTOR_TEMPLE_HAMA = ACTOR_FIRST_PLACED,
  ACTOR_GREETING_MONK,
  ACTOR_EASTWARD_MONK,
  ACTOR_MEDITATING_MONK,
  ACTOR_ANXIOUS_MONK,
  ACTOR_CHI_MONK,
  ACTOR_FALSE_WALL,
  ACTOR_TRAVELING_HEALER
};

/// Alpine Crossing's actors.
enum AlpineCrossingActor {
  ACTOR_HAMA = ACTOR_FIRST_PLACED,
  ACTOR_HSU,
  ACTOR_FEIZHI,
  ACTOR_BOULDER,
  ACTOR_FIRST_WORKER,
  ACTOR_SECOND_WORKER,
  ACTOR_PUDDLE,
  ACTOR_SPROUT
};

/// Alpine Crossing's Psynergy puzzles, kept while the party stays.
enum {
  FLAG_PUDDLE_FROZEN = FLAG_SCENE_TEMP_00,
  FLAG_SPROUT_GROWN = FLAG_SCENE_TEMP_01
};

enum {
  ANIM_HAMA_PULL = 5,
  ANIM_FEIZHI_PULL = 6,
  ANIM_HSU_STRUGGLE = 5,
  ANIM_MONK_MEDITATE = 5,
  ANIM_FROZEN = 5,
  ANIM_GROWN = 4
};

/// Map triggers that the events below refer to.
enum {
  TRIGGER_TEMPLE_DOOR = 2,
  TRIGGER_TEMPLE_WALL_WRITING = 5,
  TRIGGER_SIGNPOST = 7,
  TRIGGER_CHEST = 100,
  TRIGGER_FEIZHI_ARRIVAL = 10,
  TRIGGER_STONE = 110
};

/// The door of Hama's room, three cells wide and two high: each step copies
/// the next door picture onto it for five frames.
enum { DOOR_X = 67, DOOR_Y = 6 };

/// A search made while facing the given way.
#define SEARCH_FACING(facing) (EVENT_SEARCH | EVENT_TOUCH_FACING | (facing))

/// The map object that draws the passage behind the false wall.
enum { MAP_OBJECT_PASSAGE = 100 };

/// Draws the actor behind the background, on sprite priority 3 with automatic
/// priority off. The first worker from Xian at the Alpine Crossing runs it
/// from his placement.
static s32 Actor_DrawBehindScenery(struct ScriptInterpreter *interpreter) {
  union FieldObject *object = (union FieldObject *)interpreter;
  object->actor.priorityFlags &= ~ACTOR_PRIORITY_AUTOMATIC;
  object->actor.sprite->image.priority = 3;
  object->actor.sprite->shadow.priority = 3;
  return 0;
}

// Update callback that turns the party leader toward Feizhi while she runs
// past.
// UB: update hooks return void. Plain void removes the native zero result
// and changes its return sequence (three instructions in tbs-en).
#ifdef UBFIX
static void Leader_FaceFeizhi(union FieldObject *object) {
#else
static s32 Leader_FaceFeizhi(union FieldObject *object) {
#endif
  struct FieldActor *leader = (struct FieldActor *)object;
  struct FieldActor *feizhi;

  feizhi = Object_GetById(ACTOR_FEIZHI);
  leader->facing = ArcTan2(feizhi->z.fixed - leader->z.fixed,
                           feizhi->x.fixed - leader->x.fixed);
#ifndef UBFIX
  return 0;
#endif
}

const struct SceneEntrance *Scene_GetEntrances(void) {
  if (gGameState.scene == SCENE_ALPINE_CROSSING) {
    return gAlpineCrossing_Entrances;
  }
  return gLamaTemple_Entrances;
}

const struct SceneRegion *Scene_GetRegions(void) { return 0; }

/// Both scenes share one exit table.
const u32 *Scene_GetExits(void) { return gAlpineCrossingLamaTemple_Exits; }

const struct ScenePlacement *Scene_GetPlacements(void) {
  if (gGameState.scene == SCENE_ALPINE_CROSSING) {
    return gAlpineCrossing_Placements;
  }
  return gLamaTemple_Placements;
}

/// A monk in Lama Temple, until Hsu is freed, asks whether the party comes
/// from the east. If not, it must have crossed Lamakan Desert; if so, he asks
/// whether it stopped in Xian, whose Master Feh is close to Master Hama.
static void LamaTemple_TalkToEastwardMonk(void) {

  Engine_EventBegin();
  Engine_EventSetMessage(MsgMountainYoungWarriorsDoComeFrom);
  Engine_EventOpenMessage(ACTOR_EASTWARD_MONK, 0);

  if (Engine_EventChooseYesNo(ACTOR_PARTY_LEADER, 0) == 1) {
    Engine_EventWait(20);
    Engine_EventShowMessage(ACTOR_EASTWARD_MONK, 0);
  } else {
    Event_SkipMessages(1);
    Engine_EventAskYesNo(ACTOR_EASTWARD_MONK, 0);
  }

  Engine_EventEnd();
}

/// The meditating monk in Lama Temple, until Hsu is freed, asks whether the
/// party knows what meditation is, and explains it.
static void LamaTemple_TalkToMeditatingMonk(void) {
  Engine_EventBegin();
  Engine_EventSetMessage(MsgMountainDoKnowMeditation);
  Engine_EventAskYesNo(ACTOR_MEDITATING_MONK, 0);
  Engine_EventEnd();
}

/// The monk in Lama Temple who, until Hsu is freed, praises Master Hama as the
/// greatest master of Chi.
static void LamaTemple_TalkToChiMonk(void) {
  Engine_EventBegin();
  Engine_EventSetMessage(MsgMountainDidKnowMasterHamaGreatest);
  Engine_EventAskYesNo(ACTOR_CHI_MONK, 0);
  Engine_EventEnd();
}

/// In Lama Temple, walking north into the door of Hama's room slides it open
/// and walks the party leader through, into the room
/// (LAMA_TEMPLE_TO_FUCHIN_TEMPLE_INTERIORS).
static void LamaTemple_EnterHamasRoom(void) {
  Event_PlayCue(SE_188);
  Map_AnimateCells(gLamaTemple_DoorOpenCells, DOOR_X, DOOR_Y);
  Object_GetById(ACTOR_PARTY_LEADER)->motionFlags = 0;
  Actor_SetSpeed(ACTOR_PARTY_LEADER, 0xcccc, 0x6666);
  gEventWork->startTransition = SCENE_TRANSITION(TRANSITION_BACKDROP_FADE, 0);
  Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_WALK);
  Actor_SetDestinationOffset(ACTOR_PARTY_LEADER, 0, -16);
  Engine_EventWait(16);
  Engine_EventRequestExit(LAMA_TEMPLE_TO_FUCHIN_TEMPLE_INTERIORS);
}

/// Hama's farewell, once the party has freed Hsu and been brought back to Lama
/// Temple: she thanks the Adepts, Garet and Mia squabble over whose idea
/// helping was, and Hama goes back to her room. Ivan wonders aloud about what
/// Hama told him, and Garet, Ivan and Mia rejoin the party. Garet's two
/// questions are yes or no choices.
static void LamaTemple_RunHamasFarewell(void) {
  Engine_EventBegin();
  Actor_SetPosition(ACTOR_TEMPLE_HAMA, PIXELS(136), PIXELS(168));
  Actor_FaceDirection(ACTOR_TEMPLE_HAMA, FACING_SOUTH + FACING_STEP, 0);
  Actor_SetPosition(ACTOR_PARTY_LEADER, PIXELS(144), PIXELS(200));
  Actor_SetPosition(GARET, PIXELS(160), PIXELS(192));
  Actor_SetPosition(IVAN, PIXELS(128), PIXELS(200));
  Actor_SetPosition(MIA, PIXELS(112), PIXELS(192));
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 0);
  Actor_FaceDirection(GARET, FACING_NORTHWEST, 0);
  Actor_FaceDirection(IVAN, FACING_NORTH, 0);
  Actor_FaceDirection(MIA, FACING_NORTHEAST, 0);
  gEventWork->startTransition = SCENE_TRANSITION(TRANSITION_WINDOW, 1);
  Engine_EventOpenScreen();
  Engine_EventWaitForScreen();
  Engine_EventWait(60);

  // "Adepts, let me thank you again."
  Engine_ActorSetAnimationAndWait(ACTOR_TEMPLE_HAMA, ANIM_NOD);
  Engine_EventWait(20);
  Engine_EventSetMessage(MsgMountainAdeptsLetMeThankAgain);
  Event_ShowMessageAndWait(ACTOR_TEMPLE_HAMA, 0, 20);
  Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_NOD);
  Engine_ActorSetAnimation(GARET, ANIM_NOD);
  Engine_ActorSetAnimation(IVAN, ANIM_NOD);
  Engine_ActorSetAnimationAndWait(MIA, ANIM_NOD);
  Engine_EventWait(20);
  Engine_ActorSetAnimationAndWait(ACTOR_TEMPLE_HAMA, ANIM_SHAKE_HEAD);
  Engine_EventWait(20);
  Event_ShowMessageAndWait(ACTOR_TEMPLE_HAMA, 0, 20);
  Engine_ActorSetAnimationAndWait(MIA, ANIM_SHAKE_HEAD);
  Engine_EventWait(20);
  Event_ShowMessageAndWait(MIA, 0, 20);
  Engine_ActorRunRepeatedMotion(IVAN, 2);
  Engine_EventWait(20);
  Event_ShowMessageAndWait(IVAN, 0, 20);
  Engine_ActorRunRepeatedMotion(ACTOR_TEMPLE_HAMA, 2);
  Actor_FaceDirection(ACTOR_TEMPLE_HAMA, FACING_SOUTH + FACING_STEP, 20);
  Event_ShowMessageAndWait(ACTOR_TEMPLE_HAMA, 0, 20);
  Actor_ShowEmote(IVAN, EMOTE_IN_FRONT | 2, 60);
  Engine_EventWait(120);

  // Garet claims that helping was his idea and asks the leader to agree.
  Engine_ActorRunRepeatedMotion(GARET, 2);
  Engine_EventWait(20);
  Actor_FaceDirection(GARET, FACING_SOUTHWEST, 20);
  Event_OpenMessage(GARET, 0);
  if (Engine_EventChooseYesNo(ACTOR_PARTY_LEADER, 0) == 0) {
    Engine_EventWait(20);
    Actor_FaceDirection(MIA, FACING_EAST, 20);
    Actor_ShowEmote(MIA, EMOTE_IN_FRONT | 1, 60);
    Event_ShowMessageAndWait(MIA, 0, 20);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTHEAST, 0);
    Actor_FaceDirection(IVAN, FACING_NORTHEAST, 0);
    Engine_ActorStartRepeatedMotion(GARET, 2);
    Actor_SetAttachedEffect(GARET, EMOTE_IN_FRONT | 2);
    Engine_EventWait(60);
    Actor_FaceDirection(GARET, FACING_WEST, 20);
    Event_ShowMessageAndWait(GARET, 0, 20);
    Event_SkipMessages(2);
  } else {
    Event_SkipMessages(2);
    Engine_EventWait(20);
    Actor_FaceDirection(MIA, FACING_EAST, 20);
    Engine_ActorSetAnimationAndWait(MIA, ANIM_NOD);
    Engine_EventWait(20);
    Event_ShowMessageAndWait(MIA, 0, 20);
    Actor_ShowEmote(GARET, EMOTE_IN_FRONT | 2, 60);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTHEAST, 0);
    Actor_FaceDirection(IVAN, FACING_NORTHEAST, 0);
    Event_ShowMessageAndWait(GARET, 0, 20);
  }

  // Hama wishes them well and goes back into her room.
  Engine_ActorRunRepeatedMotion(ACTOR_TEMPLE_HAMA, 2);
  Engine_EventWait(20);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 0);
  Actor_FaceDirection(GARET, FACING_NORTHWEST, 0);
  Actor_FaceDirection(IVAN, FACING_NORTH, 0);
  Actor_FaceDirection(MIA, FACING_NORTHEAST, 0);
  Engine_EventWait(20);
  Actor_FaceDirection(ACTOR_TEMPLE_HAMA, FACING_SOUTH - FACING_STEP, 20);
  Event_ShowMessageAndWait(ACTOR_TEMPLE_HAMA, 0, 20);
  Engine_ActorRunRepeatedMotion(GARET, 2);
  Engine_EventWait(20);
  Engine_ActorSetAnimationAndWait(GARET, ANIM_NOD);
  Engine_EventWait(30);
  Engine_ActorSetAnimationAndWait(ACTOR_TEMPLE_HAMA, ANIM_NOD);
  Engine_EventWait(20);
  Event_ShowMessageAndWait(ACTOR_TEMPLE_HAMA, 0, 20);
  Engine_ActorRunRepeatedMotion(ACTOR_TEMPLE_HAMA, 2);
  Engine_EventWait(20);
  Event_ShowMessageAndWait(ACTOR_TEMPLE_HAMA, 0, 20);
  Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_NOD);
  Engine_ActorSetAnimation(GARET, ANIM_NOD);
  Engine_ActorSetAnimation(IVAN, ANIM_NOD);
  Engine_ActorSetAnimationAndWait(MIA, ANIM_NOD);
  Engine_EventWait(20);
  Engine_ActorSetAnimationAndWait(ACTOR_TEMPLE_HAMA, ANIM_NOD);
  Engine_EventWait(20);
  Actor_FaceDirection(ACTOR_TEMPLE_HAMA, FACING_NORTH, 30);
  // The door slides open with SE_188, Hama walks through, and it slides shut.
  Event_PlayCue(SE_188);
  Map_AnimateCells(gLamaTemple_DoorOpenCells, DOOR_X, DOOR_Y);
  Actor_SetSpeed(ACTOR_TEMPLE_HAMA, 0xcccc, 0x6666);
  Actor_WalkToAndWait(ACTOR_TEMPLE_HAMA, 136, 136);
  Actor_SetPosition(ACTOR_TEMPLE_HAMA, 0, 0);
  Event_PlayCue(SE_188);
  Map_AnimateCells(gLamaTemple_DoorCloseCells, DOOR_X, DOOR_Y);
  Engine_EventWait(60);
  BattleFx_PlayQueuedSound();

  // Garet asks whether the leader means to leave Hama's words unasked.
  Engine_ActorRunRepeatedMotion(GARET, 2);
  Engine_EventWait(20);
  Actor_FaceDirection(GARET, FACING_SOUTHWEST, 20);
  Event_ShowMessageAndWait(GARET, 0, 20);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_WEST, 20);
  Engine_ActorRunRepeatedMotion(IVAN, 2);
  Engine_EventWait(20);
  Actor_FaceDirection(MIA, FACING_SOUTHEAST, 20);
  Event_ShowMessageAndWait(MIA, 0, 20);
  Engine_ActorRunRepeatedMotion(GARET, 1);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTHEAST, 20);
  Event_AskYesNo(GARET, 0);
  Engine_EventWait(20);

  // Ivan decides that Hama will tell him when their mission is done.
  Engine_ActorRunRepeatedMotion(IVAN, 2);
  Engine_EventWait(20);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_WEST, 20);
  Event_ShowMessageAndWait(IVAN, 0, 20);
  Engine_ActorStartRepeatedMotion(ACTOR_PARTY_LEADER, 1);
  Engine_ActorStartRepeatedMotion(GARET, 1);
  Engine_ActorRunRepeatedMotion(MIA, 1);
  Engine_EventWait(20);
  Engine_ActorSetAnimationAndWait(IVAN, ANIM_SHAKE_HEAD);
  Engine_EventWait(20);
  Event_ShowMessageAndWait(IVAN, 0, 20);
  Engine_ActorRunRepeatedMotion(MIA, 2);
  Engine_EventWait(20);
  Event_ShowMessageAndWait(MIA, 0, 20);
  Actor_FaceDirection(IVAN, FACING_NORTHWEST + FACING_STEP, 20);
  Engine_ActorSetAnimationAndWait(IVAN, ANIM_NOD);
  Engine_EventWait(20);
  Engine_ActorSetAnimationAndWait(IVAN, ANIM_NOD);
  Engine_EventWait(20);
  Event_ShowMessageAndWait(IVAN, 0, 20);
  Actor_ShowEmote(GARET, EMOTE_IN_FRONT | 2, 60);
  Event_ShowMessageAndWait(GARET, 0, 20);
  Actor_FaceDirection(IVAN, FACING_NORTHEAST, 20);
  Engine_ActorSetAnimationAndWait(IVAN, ANIM_SHAKE_HEAD);
  Engine_EventWait(20);
  Event_ShowMessageAndWait(IVAN, 0, 20);
  Actor_ShowEmote(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | 2, 0);
  Actor_ShowEmote(MIA, EMOTE_IN_FRONT | 2, 60);
  Event_ShowMessageAndWait(MIA, 0, 20);
  Actor_FaceDirection(IVAN, FACING_NORTHWEST, 20);
  Engine_ActorSetAnimationAndWait(IVAN, ANIM_NOD);
  Engine_EventWait(20);
  Event_ShowMessageAndWait(IVAN, 0, 20);
  Engine_ActorStartRepeatedMotion(ACTOR_PARTY_LEADER, 1);
  Engine_ActorStartRepeatedMotion(GARET, 1);
  Engine_ActorRunRepeatedMotion(MIA, 1);
  Engine_EventWait(20);
  Actor_SetSpeed(IVAN, 0x8000, 0x4000);
  Actor_WalkToAndWait(IVAN, 128, 184);
  Actor_FaceDirection(IVAN, FACING_SOUTH, 20);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 0);
  Actor_FaceDirection(GARET, FACING_NORTHWEST, 0);
  Actor_FaceDirection(MIA, FACING_NORTHEAST, 20);
  Engine_ActorSetAnimationAndWait(IVAN, ANIM_NOD);
  Engine_EventWait(20);
  Event_ShowMessageAndWait(IVAN, 0, 20);
  Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_NOD);
  Engine_ActorSetAnimation(GARET, ANIM_NOD);
  Engine_ActorSetAnimationAndWait(MIA, ANIM_NOD);
  Engine_EventWait(20);

  // Garet, Ivan and Mia walk to the leader and rejoin the party.
  Actor_SetSpeed(GARET, 0x8000, 0x4000);
  Actor_SetSpeed(MIA, 0x8000, 0x4000);
  Actor_WalkTo(GARET, 144, 200);
  Actor_WalkTo(IVAN, 144, 200);
  Actor_WalkTo(MIA, 144, 200);
  Engine_ActorWaitForMove(GARET);
  Actor_SetPosition(GARET, 0, 0);
  Engine_ActorWaitForMove(IVAN);
  Actor_SetPosition(IVAN, 0, 0);
  Engine_ActorWaitForMove(MIA);
  Actor_SetPosition(MIA, 0, 0);
  Engine_EventEnd();
}

/// Searching the writing on a wall in Lama Temple reads its hint toward Reveal.
static void LamaTemple_ReadWallWriting(void) {

  Engine_EventBegin();
  Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_STAND);
  Engine_MessageShowCentered(MsgMountainHeWhoHasPowerSee, 1);
  Engine_EventEnd();
}

/// Puts Lama Temple's false wall back when Reveal wears off (and on entry):
/// the wall's cell blocks the way again, the passage's map object is hidden and
/// the wall actor stands in front of it.
static void LamaTemple_ShowFalseWall(void) {

  Map_CopyCellAttributes(85, 9, 1, 1, 21, 9);
  Engine_MapObjectSetPosition(MAP_OBJECT_PASSAGE, 0, 0);
  Actor_SetPosition(ACTOR_FALSE_WALL, PIXELS(344), PIXELS(152));
}

/// Reveal in Lama Temple shows the passage behind the false wall: the cell's
/// original attributes (saved at (21, 73) on entry) come back, the passage's
/// map object returns to its place and the wall actor is put away.
static void LamaTemple_RevealPassage(void) {
  Map_CopyCellAttributes(21, 73, 1, 1, 21, 9);
  MapObject_SetPosition(MAP_OBJECT_PASSAGE, -1, -1);
  Actor_SetPosition(ACTOR_FALSE_WALL, 0, 0);
}

/// The traveling healer in Lama Temple offers the Sanctum's services when
/// spoken to across his counter, from the south; from any other side he only
/// greets the party.
static void LamaTemple_TalkToHealer(void) {

  u32 heading = Object_GetById(ACTOR_PARTY_LEADER)->facing;

  Engine_EventBegin();
  if (FACING_IS_NORTH(heading)) {
    Engine_SanctumOpen(ACTOR_TRAVELING_HEALER);
  } else {
    Engine_EventSetMessage(MsgMountainAmTravelingAroundWorldSpread);
    Engine_EventShowMessage(ACTOR_TRAVELING_HEALER, 0);
  }
  Engine_EventEnd();
}

/// At the Alpine Crossing, when the party steps onto the road by the boulders
/// once Feizhi has set out after Hsu, she runs up behind it: she finds the
/// boulders blocking Silk Road, says her vision came true, cries out for Hsu
/// and runs ahead along the path to look for him, the party leader following
/// part of the way.
static void AlpineCrossing_FeizhiArrives(void) {
  struct FieldActor *leader;

  if (GameFlag_IsSet(FLAG_FEIZHI_WENT_TO_HSU)) {
    Engine_EventBegin();
    Actor_SetPosition(ACTOR_FEIZHI, PIXELS(536), PIXELS(216));
    Engine_EventSetMessage(MsgMountainYahhSilkRoadBouldersBlock);
    Event_ShowMessageAndWait(ACTOR_FEIZHI, 0, 20);
    Engine_ActorRunRepeatedMotion(ACTOR_PARTY_LEADER, 2);
    Engine_EventWait(20);

    // The leader turns to follow Feizhi as she runs past; one standing in
    // her row steps aside first.
    leader = Object_GetById(ACTOR_PARTY_LEADER);
#ifdef UBFIX
    leader->update = Leader_FaceFeizhi;
#else
    leader->update = (void (*)(union FieldObject *))Leader_FaceFeizhi;
#endif
    leader = Object_GetById(ACTOR_PARTY_LEADER);
    if ((leader->z.fixed >> 20) == 13) {
      Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 440, 200);
    }
    Actor_SetSpeed(ACTOR_FEIZHI, 0x20000, 0x10000);
    Engine_ActorSetSpritePriority(ACTOR_FEIZHI, 2);
    Actor_WalkToAndWait(ACTOR_FEIZHI, 408, 216);
    Object_GetById(ACTOR_FEIZHI)->priorityFlags |= ACTOR_PRIORITY_AUTOMATIC;
    Engine_EventWait(10);
    Actor_FaceDirection(ACTOR_FEIZHI, FACING_WEST, 20);
    Event_ShowMessageAndWait(ACTOR_FEIZHI, 0, 20);
    Engine_ActorStartRepeatedMotion(ACTOR_FEIZHI, 2);
    Actor_SetAttachedEffect(ACTOR_FEIZHI, EMOTE_IN_FRONT | 2);
    Engine_EventWait(60);
    Event_ShowMessageAndWait(ACTOR_FEIZHI, 0, 20);

    // Feizhi runs down the path with the camera; the leader follows her
    // until the camera arrives.
    Actor_SetActionScript(ACTOR_FEIZHI, gAlpineCrossing_FeizhiPath);
    Camera_MoveTo(PIXELS(296), -1, PIXELS(344), 1);
    GameFlag_Set(FLAG_FEIZHI_SAW_ROCKSLIDE);
    Object_RefreshSelectorById(ACTOR_FEIZHI);
    Engine_CameraWaitForMove();
    Actor_SetSpeed(ACTOR_PARTY_LEADER, 0x10000, 0x8000);
    Actor_SetActionScript(ACTOR_PARTY_LEADER, gAlpineCrossing_LeaderPath);
    Object_RefreshSelectorById(ACTOR_PARTY_LEADER);
    Engine_EventWait(10);
    leader = Object_GetById(ACTOR_PARTY_LEADER);
    leader->update = NULL;
    Engine_EventWait(30);

    // She cannot hear Hsu's voice.
    Engine_ActorRunRepeatedMotion(ACTOR_FEIZHI, 2);
    Engine_EventWait(20);
    Actor_FaceDirection(ACTOR_FEIZHI, FACING_SOUTH + FACING_STEP, 120);
    Actor_ShowEmote(ACTOR_FEIZHI, EMOTE_IN_FRONT | 5, 60);
    Actor_ShowEmote(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | 1, 60);
    Engine_ActorSetAnimationAndWait(ACTOR_FEIZHI, ANIM_SHAKE_HEAD);
    Engine_EventWait(20);
    Event_ShowMessageAndWait(ACTOR_FEIZHI, 0, 20);
    Engine_EventEnd();
  }
}

/// Talking to Feizhi at the Alpine Crossing before Hama's Reveal lesson: she
/// asks whether she should return to Xian, and either way decides to keep
/// looking for Hsu.
static void AlpineCrossing_TalkToFeizhi(void) {
  Engine_EventBegin();
  Engine_EventSetMessage(MsgMountainDoDoWarriorShouldReturn);
  Actor_ShowEmote(ACTOR_FEIZHI, EMOTE_IN_FRONT | 5, 60);
  Event_OpenMessage(ACTOR_FEIZHI, 0);
  if (Engine_EventChooseYesNo(ACTOR_PARTY_LEADER, 0) == 1) {
    Event_SkipMessages(1);
  }
  Engine_EventWait(20);
  Engine_ActorSetAnimationAndWait(ACTOR_FEIZHI, ANIM_SHAKE_HEAD);
  Engine_EventWait(20);
  Event_ShowMessageAndWait(ACTOR_FEIZHI, 0, 20);
  Engine_EventEnd();
}

/// Arriving at the Alpine Crossing from AltinPeakSouthEntrance by
/// ALPINE_CROSSING_FROM_ALTIN_PEAK_SOUTH_ENTRANCE_EAST while Hsu is trapped,
/// until this has run once (FLAG_HAMA_REACHED_HSU): the camera finds Hsu, Hama
/// asks if he is okay, and he is unhurt but cannot move; none of them knows how
/// to free him.
static void AlpineCrossing_HamaReachesHsu(void) {
  Engine_EventBegin();
  // Reveal Hsu's position before Hama and Feizhi discuss freeing him.
  Engine_EventOpenScreen();
  Engine_EventWaitForScreen();
  Engine_EventWait(30);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_SOUTHWEST, 0);
  Engine_CameraMoveToActor(ACTOR_HSU, 1);
  Engine_CameraWaitForMove();
  Engine_ActorRunRepeatedMotion(ACTOR_HAMA, 2);
  Engine_EventWait(20);
  Engine_EventSetMessage(MsgMountainHsuOkay);
  Event_ShowMessageAndWait(ACTOR_HAMA, 0, 20);
  Engine_ActorRunRepeatedMotion(ACTOR_HSU, 1);
  Engine_EventWait(20);
  Event_ShowMessageAndWait(ACTOR_HSU, 0, 20);
  Engine_ActorSetAnimationAndWait(ACTOR_HAMA, ANIM_NOD);
  Engine_EventWait(20);
  Event_ShowMessageAndWait(ACTOR_HAMA, 0, 20);
  Actor_FaceDirection(ACTOR_FEIZHI, FACING_NORTH + FACING_STEP, 20);
  Event_ShowMessageAndWait(ACTOR_FEIZHI, 0, 20);
  Engine_ActorSetAnimationAndWait(ACTOR_HAMA, ANIM_NOD);
  Engine_EventWait(60);
  Actor_ShowEmote(ACTOR_HAMA, EMOTE_IN_FRONT | 2, 60);
  Engine_ActorSetAnimationAndWait(ACTOR_HAMA, ANIM_SHAKE_HEAD);
  Engine_EventWait(20);
  Event_ShowMessageAndWait(ACTOR_HAMA, 0, 20);
  Engine_ActorStartRepeatedMotion(ACTOR_FEIZHI, 2);
  Actor_SetAttachedEffect(ACTOR_FEIZHI, EMOTE_IN_FRONT | 2);
  Engine_EventWait(60);
  Actor_FaceDirection(ACTOR_FEIZHI, FACING_NORTHWEST + FACING_STEP, 20);
  Engine_ActorSetAnimation(ACTOR_HSU, ANIM_HSU_STRUGGLE);
  Engine_EventEnd();
  GameFlag_Set(FLAG_HAMA_REACHED_HSU);
}

/// Lift on the boulder at the Alpine Crossing, until Hsu is freed: it floats,
/// and Hama and Feizhi pull him free in two heaves of two tugs each, asking the
/// leader in between to hold on. Hsu is free; they will ask Master Feh to tend
/// him and clear the road, and the scene fades out to Hama's farewell at Lama
/// Temple.
static void AlpineCrossing_LiftBoulder(void) {

  Engine_EventBegin();
  Actor_SetAttachedEffect(ACTOR_HAMA, EMOTE_IN_FRONT | 2);
  Engine_ActorStartRepeatedMotion(ACTOR_HAMA, 2);
  Engine_EventWait(60);
  Engine_EventSetMessage(MsgMountainIsaacDidLiftBoulder);
  Event_ShowMessageAndWait(ACTOR_HAMA, 0, 20);
  Actor_SetAttachedEffect(ACTOR_FEIZHI, EMOTE_IN_FRONT | 2);
  Engine_ActorJump(ACTOR_FEIZHI, 4, 0);
  Engine_EventWait(60);
  Event_ShowMessageAndWait(ACTOR_FEIZHI, 0, 20);
  Engine_ActorRunRepeatedMotion(ACTOR_HAMA, 1);
  Engine_EventWait(20);
  Event_ShowMessageAndWait(ACTOR_HAMA, 0, 20);
  Actor_FaceDirection(ACTOR_FEIZHI, FACING_NORTH + FACING_STEP, 20);
  Engine_ActorSetAnimationAndWait(ACTOR_FEIZHI, ANIM_NOD);
  Engine_EventWait(20);

  // Hama and Feizhi take hold of Hsu.
  Actor_WalkTo(ACTOR_HAMA, 178, 276);
  Actor_WalkToAndWait(ACTOR_FEIZHI, 172, 284);
  Engine_ActorWaitForMove(ACTOR_HAMA);
  Actor_FaceDirection(ACTOR_HAMA, FACING_SOUTH + FACING_STEP, 0);
  Actor_FaceDirection(ACTOR_FEIZHI, FACING_NORTHWEST + FACING_STEP, 0);
  Engine_EventWait(20);
  Engine_ActorRunRepeatedMotion(ACTOR_HAMA, 2);
  Engine_EventWait(20);
  Event_ShowMessageAndWait(ACTOR_HAMA, 0, 20);
  Object_GetById(ACTOR_HAMA)->steeringFlags &= (u8)~ACTOR_FACES_MOTION;
  Object_GetById(ACTOR_FEIZHI)->steeringFlags &= (u8)~ACTOR_FACES_MOTION;
  Actor_SetSpeed(ACTOR_HAMA, 0x3333, 0x1999);
  Actor_SetSpeed(ACTOR_FEIZHI, 0x3333, 0x1999);

  // The first heave: two tugs draw Hsu halfway out.
  Engine_ActorSetAnimation(ACTOR_HAMA, ANIM_HAMA_PULL);
  Engine_ActorSetAnimation(ACTOR_FEIZHI, ANIM_FEIZHI_PULL);
  Engine_EventWait(20);
  Event_PlayCue(SE_125);
  Actor_SetDestinationOffset(ACTOR_HAMA, 2, 0);
  Actor_SetDestinationOffset(ACTOR_HSU, 2, 0);
  Actor_SetDestinationOffset(ACTOR_FEIZHI, 2, 0);
  Engine_ActorWaitForMove(ACTOR_FEIZHI);
  Engine_EventWait(30);
  Engine_ActorSetAnimation(ACTOR_HAMA, ANIM_HAMA_PULL);
  Engine_ActorSetAnimation(ACTOR_FEIZHI, ANIM_FEIZHI_PULL);
  Engine_EventWait(20);
  Event_PlayCue(SE_125);
  Actor_SetDestinationOffset(ACTOR_HAMA, 4, 0);
  Actor_SetDestinationOffset(ACTOR_HSU, 4, 0);
  Actor_SetDestinationOffset(ACTOR_FEIZHI, 4, 0);
  Engine_ActorWaitForMove(ACTOR_FEIZHI);
  Engine_ActorStop(ACTOR_HSU);
  Engine_ActorSetAnimation(ACTOR_HAMA, ANIM_STAND);
  Engine_ActorSetAnimation(ACTOR_FEIZHI, ANIM_STAND);
  Engine_EventWait(50);
  Engine_ActorJump(ACTOR_FEIZHI, 2, 0);
  Engine_EventWait(20);
  Event_ShowMessageAndWait(ACTOR_FEIZHI, 0, 20);

  // Hama asks the leader to hold the boulder a moment longer.
  Engine_ActorSetAnimationAndWait(ACTOR_HAMA, ANIM_NOD);
  Engine_EventWait(20);
  Event_AskYesNo(ACTOR_HAMA, 0);
  Event_ShowMessageAndWait(ACTOR_HAMA, 0, 20);

  // The second heave pulls Hsu free.
  Engine_ActorSetAnimation(ACTOR_HAMA, ANIM_HAMA_PULL);
  Engine_ActorSetAnimation(ACTOR_FEIZHI, ANIM_FEIZHI_PULL);
  Engine_EventWait(20);
  Event_PlayCue(SE_125);
  Actor_SetDestinationOffset(ACTOR_HAMA, 2, 0);
  Actor_SetDestinationOffset(ACTOR_HSU, 2, 0);
  Actor_SetDestinationOffset(ACTOR_FEIZHI, 2, 0);
  Engine_ActorWaitForMove(ACTOR_FEIZHI);
  Engine_EventWait(30);
  Engine_ActorSetAnimation(ACTOR_HAMA, ANIM_HAMA_PULL);
  Engine_ActorSetAnimation(ACTOR_FEIZHI, ANIM_FEIZHI_PULL);
  Engine_EventWait(20);
  Event_PlayCue(SE_125);
  Actor_SetDestinationOffset(ACTOR_HAMA, 4, 0);
  Actor_SetDestinationOffset(ACTOR_HSU, 4, 0);
  Actor_SetDestinationOffset(ACTOR_FEIZHI, 4, 0);
  Engine_ActorWaitForMove(ACTOR_FEIZHI);
  Engine_EventWait(40);
  Engine_ActorSetAnimation(ACTOR_HAMA, ANIM_STAND);
  Engine_ActorSetAnimation(ACTOR_FEIZHI, ANIM_STAND);
  Engine_ActorJump(ACTOR_FEIZHI, 2, 0);
  Engine_EventWait(20);
  Actor_FaceDirection(ACTOR_FEIZHI, FACING_NORTH + FACING_STEP, 20);
  Event_ShowMessageAndWait(ACTOR_FEIZHI, 0, 20);
  Engine_ActorSetAnimationAndWait(ACTOR_HAMA, ANIM_NOD);
  Engine_EventWait(20);
  Event_ShowMessageAndWait(ACTOR_HAMA, 0, 30);
  Event_ShowMessageAndWait(ACTOR_HAMA, 0, 20);

  // Feizhi steps back; they agree to send for Master Feh.
  Object_GetById(ACTOR_FEIZHI)->steeringFlags |= ACTOR_FACES_MOTION;
  Actor_SetSpeed(ACTOR_FEIZHI, 0xcccc, 0x6666);
  Actor_WalkToAndWait(ACTOR_FEIZHI, 168, 296);
  Actor_FaceDirection(ACTOR_FEIZHI, FACING_NORTH + FACING_STEP, 20);
  Engine_ActorSetAnimation(ACTOR_FEIZHI, 5);
  Engine_ActorSetAnimationAndWait(ACTOR_HAMA, ANIM_NOD);
  Engine_EventWait(20);
  Event_ShowMessageAndWait(ACTOR_HAMA, 0, 20);
  Engine_ActorSetAnimationAndWait(ACTOR_FEIZHI, ANIM_NOD);
  Engine_EventWait(20);
  Event_ShowMessageAndWait(ACTOR_FEIZHI, 0, 20);
  Engine_ActorSetAnimationAndWait(ACTOR_HAMA, ANIM_NOD);
  Engine_EventWait(20);
  Event_ShowMessageAndWait(ACTOR_HAMA, 0, 20);
  Engine_ActorSetAnimationAndWait(ACTOR_PARTY_LEADER, ANIM_NOD);
  Engine_EventWait(20);
  Engine_EventEnd();
  GameFlag_Set(FLAG_HSU_RESCUED);
  gEventWork->startTransition = SCENE_TRANSITION(TRANSITION_WINDOW, 1);
  Engine_EventRequestExit(ALPINE_CROSSING_TO_LAMA_TEMPLE);
}

/// Searching the signpost at the Alpine Crossing.
/// "North: Altin Mine. West: Lama Temple."
static void AlpineCrossing_ReadSignpost(void) {
  Engine_EventBegin();
  Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_STAND);
  Engine_MessageShowCentered(MsgMountainNorthAltinMineWestLama, 1);
  Engine_EventEnd();
}

/// Reveal at the Alpine Crossing shows the path hidden at (5, 13): the revealed
/// cells, kept at (5, 28), are copied there with their attributes.
static void AlpineCrossing_RevealPath(void) {
  Map_CopyCellsTo(5, 28, 5, 13, 1, 2);
  Map_CopyCellAttributes(5, 28, 1, 2, 5, 13);
  Engine_EventWait(1);
}

/// When Reveal wears off at the Alpine Crossing, the plain cells kept at
/// (6, 28) cover the path again.
static void AlpineCrossing_HidePath(void) {
  Map_CopyCellsTo(6, 28, 5, 13, 1, 2);
  Map_CopyCellAttributes(6, 28, 1, 2, 5, 13);
  Engine_EventWait(1);
}

// Update callback of the frozen pillar: it draws under the party leader
// while the leader stands higher than its top, so the leader can climb onto
// it, and over the leader otherwise.
// UB: this update hook has no result. Plain void changes the native return
// sequence from pop {r1}; bx r1 to pop {r0}; bx r0 (tbs-en).
#ifdef UBFIX
static void Pillar_SortAgainstLeader(union FieldObject *object) {
#else
static s32 Pillar_SortAgainstLeader(union FieldObject *object) {
#endif
  struct FieldActor *pillar = (struct FieldActor *)object;
  if (Object_GetById(ACTOR_PARTY_LEADER)->y.fixed > pillar->y.fixed) {
    pillar->priorityFlags |= ACTOR_PRIORITY_UNDERFOOT;
  } else {
    pillar->priorityFlags &= (u8)~ACTOR_PRIORITY_UNDERFOOT;
  }
}

/// As Frost reaches the puddle at the Alpine Crossing, a leader standing north
/// of it sees the pillar rise in front of the background.
static void AlpineCrossing_PrepareFrost(void) {
  struct FieldActor *leader = Object_GetById(ACTOR_PARTY_LEADER);
  struct FieldActor *puddle = Object_GetById(ACTOR_PUDDLE);

  if (leader->z.fixed <= puddle->z.fixed) {
    Engine_ActorSetSpritePriority(ACTOR_PUDDLE, 1);
  }
}

/// Frost at the Alpine Crossing freezes the puddle into a pillar the party can
/// climb: it stops blocking movement and moving, sorts itself against the
/// leader each frame, and the cells beside it (and their copy at (20, 18))
/// become walkable. Also run on entry while the pillar is already frozen.
static void AlpineCrossing_FreezePuddle(void) {
  Engine_EventBegin();

  // Stop the pillar and install its height-dependent drawing priority.
  Object_GetById(ACTOR_PUDDLE)->priorityFlags &= (u8)~ACTOR_PRIORITY_UNDERFOOT;
  Object_GetById(ACTOR_PUDDLE)->collisionFlags &= 0xfd;
  Object_GetById(ACTOR_PUDDLE)->motionFlags = 0;
#ifdef UBFIX
  Object_GetById(ACTOR_PUDDLE)->update = Pillar_SortAgainstLeader;
#else
  Object_GetById(ACTOR_PUDDLE)->update =
      (void (*)(union FieldObject *))Pillar_SortAgainstLeader;
#endif

  Map_CopyCellAttributes(55, 16, 1, 1, 56, 18);
  Map_CopyCellAttributes(55, 16, 1, 1, 20, 18);

  Engine_TaskWait(1);
  Engine_GameFlagSet(FLAG_PUDDLE_FROZEN);
  Engine_ActorSetSpritePriority(ACTOR_PUDDLE, 2);
  Engine_EventEnd();
}

/// Growth at the Alpine Crossing grows the sprout into a vine the party can
/// climb: the two cells above it take the climbable attributes kept at
/// (14, 6). Also run on entry while the vine is already grown.
static void AlpineCrossing_GrowSprout(void) {
  Engine_EventBegin();
  Map_CopyCellAttributes(14, 6, 1, 2, 21, 11);
  Object_GetById(ACTOR_SPROUT)->collisionFlags = 254;
  Engine_GameFlagSet(FLAG_SPROUT_GROWN);
  Engine_EventEnd();
}

const struct SceneEvent *Scene_GetEvents(void) {
  if (gGameState.scene == SCENE_ALPINE_CROSSING) {
    return gAlpineCrossing_Events;
  }
  return gLamaTemple_Events;
}

/// Sets up whichever scene is being entered and runs its arrival events.
s32 Scene_Initialize(void) {
  s32 entrance;

  gEventWork->startTransition = SCENE_TRANSITION(TRANSITION_BACKDROP_FADE, 0);
  if (gGameState.scene == SCENE_LAMA_TEMPLE) {
    // Lama Temple: the temple's queued sound (SE_169) plays, two monks
    // meditate and the false wall stands; the wall's own cell attributes are
    // saved at (21, 73) for Reveal to restore.
    BattleFx_SetQueuedSoundAndPlay(SE_169);
    Engine_ActorSetAnimation(ACTOR_MEDITATING_MONK, ANIM_MONK_MEDITATE);
    Engine_ActorSetAnimation(ACTOR_ANXIOUS_MONK, ANIM_MONK_MEDITATE);
    Engine_ActorSetAnimation(ACTOR_FALSE_WALL, 2);
    Map_CopyCellAttributes(21, 9, 1, 1, 21, 73);
    LamaTemple_ShowFalseWall();
    if (Engine_GameFlagIsSet(FLAG_HSU_RESCUED)) {
      Actor_SetPosition(ACTOR_CHI_MONK, PIXELS(136), PIXELS(256));
      Engine_ActorFaceDirection(ACTOR_CHI_MONK, FACING_EAST, 0);
    }
    entrance = gGameState.entrance;
    if (entrance == LAMA_TEMPLE_FROM_FUCHIN_TEMPLE_INTERIORS) {
      Engine_GameFlagClear(FLAG_SHOW_LOCATION_NAME);
    } else if (entrance == LAMA_TEMPLE_FROM_ALPINE_CROSSING &&
               !Engine_GameFlagIsSet(FLAG_KEEP_PARTY_POSITION)) {
      LamaTemple_RunHamasFarewell();
    }
  } else if (gGameState.scene == SCENE_ALPINE_CROSSING) {
    // Alpine Crossing: restore the Psynergy puzzles already solved.
    Engine_ActorSetSpriteFlags(Object_GetById(ACTOR_PUDDLE), 0);
    Object_GetById(ACTOR_PUDDLE)->priorityFlags |= ACTOR_PRIORITY_UNDERFOOT;
    if (Engine_GameFlagIsSet(FLAG_PUDDLE_FROZEN)) {
      Engine_ActorSetAnimation(ACTOR_PUDDLE, ANIM_FROZEN);
      AlpineCrossing_FreezePuddle();
    }
    if (Engine_GameFlagIsSet(FLAG_SPROUT_GROWN)) {
      Engine_ActorSetAnimation(ACTOR_SPROUT, ANIM_GROWN);
      AlpineCrossing_GrowSprout();
    }
    if (gGameState.entrance ==
            ALPINE_CROSSING_FROM_ALTIN_PEAK_SOUTH_ENTRANCE_WEST ||
        gGameState.entrance ==
            ALPINE_CROSSING_FROM_ALTIN_PEAK_SOUTH_ENTRANCE_EAST) {
      Engine_GameFlagClear(FLAG_SHOW_LOCATION_NAME);
    }

    // Feizhi is here only once she has set out after Hsu.
    if (!Engine_GameFlagIsSet(FLAG_FEIZHI_WENT_TO_HSU) &&
        !Engine_GameFlagIsSet(FLAG_XIAN_HEARD_HSU_TRAPPED) &&
        !Engine_GameFlagIsSet(FLAG_HSU_RESCUED)) {
      Engine_ActorSetPosition(ACTOR_FEIZHI, 0, 0);
    }

    // Arriving from the north after Hama's lesson, before the party has freed
    // Hsu: he has been freed without it and the boulder is gone.
    if (!GameFlag_IsSet(FLAG_HSU_RESCUED) &&
        Engine_GameFlagIsSet(FLAG_XIAN_HEARD_HSU_TRAPPED) &&
        gGameState.entrance == ALPINE_CROSSING_FROM_WORLD_MAP_NORTH) {
      Engine_ActorSetPosition(ACTOR_BOULDER, 0, 0);
      Engine_GameFlagSet(FLAG_HSU_RESCUED);
      Engine_GameFlagSet(FLAG_HSU_RESCUED_WITHOUT_PARTY);
      Engine_ActorSetPosition(ACTOR_FEIZHI, 0, 0);
    }

    // Once Hsu is free, the cleared rocks open the road.
    if (GameFlag_IsSet(FLAG_HSU_RESCUED)) {
      Map_CopyCellsTo(54, 21, 53, 21, 1, 2);
      Map_CopyCellAttributes(18, 20, 1, 3, 17, 21);
      Map_CopyCellsTo(44, 18, 43, 17, 1, 1);
      Map_CopyCellAttributes(8, 17, 1, 1, 7, 17);
    }

    // After Hama's lesson, while Hsu is still trapped, Hama and Feizhi stand
    // by him, and the workers from Xian have not come yet.
    if (Engine_GameFlagIsSet(FLAG_XIAN_HEARD_HSU_TRAPPED) &&
        !Engine_GameFlagIsSet(FLAG_HSU_RESCUED)) {
      Engine_ActorSetPosition(ACTOR_FIRST_WORKER, 0, 0);
      Engine_ActorSetPosition(ACTOR_SECOND_WORKER, 0, 0);
      Actor_SetPosition(ACTOR_HAMA, PIXELS(192), PIXELS(264));
      Actor_SetPosition(ACTOR_HSU, PIXELS(164), PIXELS(280));
      Actor_SetPosition(ACTOR_FEIZHI, PIXELS(184), PIXELS(304));
      Actor_FaceDirection(ACTOR_HAMA, FACING_SOUTH + FACING_STEP, 0);
      Actor_FaceDirection(ACTOR_FEIZHI, FACING_NORTHWEST + FACING_STEP, 0);
      Actor_SetActionScript(ACTOR_HSU, gAlpineCrossing_HsuStruggle);
      Object_GetById(ACTOR_HSU)->scaleX = -0x10000;
    }
    if (!GameFlag_IsSet(FLAG_HSU_RESCUED)) {
      Actor_SetPosition(ACTOR_HSU, PIXELS(164), PIXELS(280));
      Actor_SetActionScript(ACTOR_HSU, gAlpineCrossing_HsuStruggle);
      Object_GetById(ACTOR_HSU)->scaleX = -0x10000;
    }
    if (gGameState.entrance ==
            ALPINE_CROSSING_FROM_ALTIN_PEAK_SOUTH_ENTRANCE_EAST &&
        !Engine_GameFlagIsSet(FLAG_HAMA_REACHED_HSU) &&
        !Engine_GameFlagIsSet(FLAG_KEEP_PARTY_POSITION) &&
        !Engine_GameFlagIsSet(FLAG_HSU_RESCUED)) {
      AlpineCrossing_HamaReachesHsu();
    }
  }
  return 0;
}

/// The first worker's placement script: draw him behind the background.
s32 gAlpineCrossing_WorkerPlacementScript[] = {
    SC_CALL(Actor_DrawBehindScenery),
    SC_END,
};

/// Feizhi's run down the path toward Hsu, walking, then standing.
s32 gAlpineCrossing_FeizhiPath[] = {
    SC_ARG(ANIM_WALK),
    SC_MOVE(PIXELS(392), 0, PIXELS(232)),
    SC_SYNC,
    SC_MOVE(PIXELS(376), 0, PIXELS(232)),
    SC_SYNC,
    SC_MOVE(PIXELS(344), 0, PIXELS(264)),
    SC_SYNC,
    SC_MOVE(PIXELS(344), 0, PIXELS(312)),
    SC_SYNC,
    SC_MOVE(PIXELS(312), 0, PIXELS(344)),
    SC_SYNC,
    SC_MOVE(PIXELS(296), 0, PIXELS(344)),
    SC_SYNC,
    SC_ARG(ANIM_STAND),
    SC_END,
};

/// The party leader follows Feizhi most of the way.
s32 gAlpineCrossing_LeaderPath[] = {
    SC_ARG(ANIM_WALK),
    SC_MOVE(PIXELS(392), 0, PIXELS(232)),
    SC_SYNC,
    SC_MOVE(PIXELS(376), 0, PIXELS(232)),
    SC_SYNC,
    SC_MOVE(PIXELS(344), 0, PIXELS(264)),
    SC_SYNC,
    SC_MOVE(PIXELS(344), 0, PIXELS(312)),
    SC_SYNC,
    SC_MOVE(PIXELS(328), 0, PIXELS(328)),
    SC_SYNC,
    SC_ARG(ANIM_STAND),
    SC_END,
};

/// Hsu, pinned under the boulder, struggles now and then.
s32 gAlpineCrossing_HsuStruggle[] = {
    SC_ARG(ANIM_STAND),
    SC_ARG(ANIM_HSU_STRUGGLE),
    SC_WAIT(240),
    SC_ARG(ANIM_STAND),
    SC_ARG(ANIM_HSU_STRUGGLE),
    SC_WAIT(60),
    SC_ARG(ANIM_STAND),
    SC_ARG(ANIM_HSU_STRUGGLE),
    SC_WAIT(300),
    SC_LOOP(SC_FOREVER, SC_TOP),
    SC_END,
};

/// The door of Hama's room sliding open, and shut again.
u16 gLamaTemple_DoorOpenCells[] = {
    MAP_CELL_STEP(66, 46, 3, 2, 5),
    MAP_CELL_STEP(63, 46, 3, 2, 5),
    MAP_CELL_STEPS_END,
};

u16 gLamaTemple_DoorCloseCells[] = {
    MAP_CELL_STEP(66, 46, 3, 2, 5),
    MAP_CELL_STEP(69, 46, 3, 2, 5),
    MAP_CELL_STEPS_END,
};

struct SceneEntrance gLamaTemple_Entrances[] = {
    {0, CONDITION_ALWAYS, 496, 0, 216, FACING_WEST, 0, -1, -1, -1, -1, 0},
    {LAMA_TEMPLE_FROM_WORLD_MAP, CONDITION_ALWAYS, 216, 0, 520, FACING_NORTH, 0,
     -1, -1, -1, -1, 0},
    {LAMA_TEMPLE_FROM_FUCHIN_TEMPLE_INTERIORS, CONDITION_ALWAYS, 136, 0, 168,
     FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {LAMA_TEMPLE_FROM_ALPINE_CROSSING, CONDITION_ALWAYS, 144, 0, 200,
     FACING_NORTH, 0, -1, -1, -1, -1, 0},
    {5, CONDITION_ALWAYS, 232, 0, 352, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {SCENE_TABLE_END},
};

struct SceneEntrance gAlpineCrossing_Entrances[] = {
    {0, CONDITION_ALWAYS, 496, 0, 216, FACING_WEST, 0, -1, -1, -1, -1, 0},
    {ALPINE_CROSSING_FROM_WORLD_MAP_EAST, CONDITION_ALWAYS, 496, 0, 216,
     FACING_WEST, 0, 16, 32, 520, 416, 0},
    {ALPINE_CROSSING_FROM_WORLD_MAP_NORTH, CONDITION_ALWAYS, 384, 0, 72,
     FACING_SOUTH, 0, 16, 32, 520, 416, 0},
    {ALPINE_CROSSING_FROM_WORLD_MAP_WEST, CONDITION_ALWAYS, 40, 0, 280,
     FACING_EAST, 0, 16, 32, 520, 416, 0},
    {ALPINE_CROSSING_FROM_ALTIN_PEAK_SOUTH_ENTRANCE_WEST, CONDITION_ALWAYS, 88,
     0, 248, FACING_SOUTH, 0, 16, 32, 520, 416, 0},
    {ALPINE_CROSSING_FROM_ALTIN_PEAK_SOUTH_ENTRANCE_EAST, CONDITION_ALWAYS, 216,
     0, 248, FACING_SOUTH, 0, 16, 32, 520, 416, 0},
    {SCENE_TABLE_END},
};

u32 gAlpineCrossingLamaTemple_Exits[] = {
    SCENE_EXITS(LamaTemple),
    SCENE_EXIT(LAMA_TEMPLE_TO_WORLD_MAP, WorldMap, WORLD_MAP_FROM_LAMA_TEMPLE),
    SCENE_EXIT(LAMA_TEMPLE_TO_FUCHIN_TEMPLE_INTERIORS, FuchinTempleInteriors,
               FUCHIN_TEMPLE_INTERIORS_FROM_LAMA_TEMPLE),
    SCENE_EXITS(AlpineCrossing),
    SCENE_EXIT(ALPINE_CROSSING_TO_WORLD_MAP_EAST, WorldMap,
               WORLD_MAP_FROM_ALPINE_CROSSING_EAST),
    SCENE_EXIT(ALPINE_CROSSING_TO_WORLD_MAP_NORTH, WorldMap,
               WORLD_MAP_FROM_ALPINE_CROSSING_NORTH),
    SCENE_EXIT(ALPINE_CROSSING_TO_WORLD_MAP_WEST, WorldMap,
               WORLD_MAP_FROM_ALPINE_CROSSING_WEST),
    SCENE_EXIT(ALPINE_CROSSING_TO_ALTIN_PEAK_SOUTH_ENTRANCE_WEST,
               AltinPeakSouthEntrance,
               ALTIN_PEAK_SOUTH_ENTRANCE_FROM_ALPINE_CROSSING_WEST),
    SCENE_EXIT(ALPINE_CROSSING_TO_ALTIN_PEAK_SOUTH_ENTRANCE_EAST,
               AltinPeakSouthEntrance,
               ALTIN_PEAK_SOUTH_ENTRANCE_FROM_ALPINE_CROSSING_EAST),
    SCENE_EXIT(ALPINE_CROSSING_TO_LAMA_TEMPLE, LamaTemple,
               LAMA_TEMPLE_FROM_ALPINE_CROSSING),
    SCENE_EXITS_END,
};

// clang-format off
struct ScenePlacement gLamaTemple_Placements[] = {
    { GARET, CONDITION_ALWAYS, ACTOR_STAND,
        0, 0, 0, FACING_NORTH, TALK_KEEP_FACING, 0 },
    { IVAN, CONDITION_ALWAYS, ACTOR_STAND,
        0, 0, 0, FACING_NORTH, TALK_KEEP_FACING, 0 },
    { MIA, CONDITION_ALWAYS, ACTOR_STAND,
        0, 0, 0, FACING_NORTH, TALK_KEEP_FACING, 0 },
    { 0x3d, CONDITION_ALWAYS, ACTOR_STAND,
        0, 0, 0, FACING_NORTH, TALK_FACE_PARTY_AND_BACK, 0 },
    { 0x9e, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(168), 0, PIXELS(336), FACING_EAST, TALK_FACE_PARTY, 0 },
    { 0x9e, CONDITION_ALWAYS, ACTOR_WANDER,
        PIXELS(216), 0, PIXELS(240), FACING_SOUTH, TALK_FACE_PARTY, 0 },
    { 0x9e, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(312), 0, PIXELS(320), FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { 0x9e, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(344), 0, PIXELS(320), FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { 0x9e, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(120), 0, PIXELS(176), FACING_SOUTH, TALK_FACE_PARTY_AND_BACK, 0 },
    { 0x125, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(344), 0, PIXELS(152), FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { 0x79, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(232), 0, PIXELS(328), FACING_SOUTH, TALK_FACE_PARTY_AND_BACK, 0 },
    { SCENE_TABLE_END },
};

struct ScenePlacement gAlpineCrossing_Placements[] = {
    { 0x3d, CONDITION_ALWAYS, ACTOR_STAND,
        0, 0, 0, FACING_NORTH, TALK_FACE_PARTY_AND_BACK, 0 },
    { 0x29, CONDITION_ALWAYS, (s32)gAlpineCrossing_HsuStruggle,
        0, 0, 0, FACING_NORTH, TALK_KEEP_FACING, 0 },
    { 0x27, FLAG_HSU_RESCUED, ACTOR_STAND,
        PIXELS(296), 0, PIXELS(344), FACING_WEST, TALK_FACE_PARTY_AND_BACK, 0 },
    { 0x127, FLAG_HSU_RESCUED, ACTOR_STAND,
        PIXELS(152), 0, PIXELS(276), FACING_WEST, TALK_FACE_PARTY, 0 },
    { 0x82, IF_FLAG_SET(FLAG_XIAN_HEARD_HSU_TRAPPED), (s32)gAlpineCrossing_WorkerPlacementScript,
        PIXELS(456), 0, PIXELS(200), FACING_SOUTH, TALK_FACE_PARTY, 0 },
    { 0x83, IF_FLAG_SET(FLAG_XIAN_HEARD_HSU_TRAPPED), ACTOR_STAND,
        PIXELS(184), 0, PIXELS(264), FACING_SOUTH, TALK_FACE_PARTY, 0 },
    { 0xe3, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(328), 0, PIXELS(296), FACING_SOUTH, TALK_FACE_PARTY, 0 },
    { 0xee, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(344), 0, PIXELS(194), FACING_SOUTH, TALK_FACE_PARTY, 0 },
    { SCENE_TABLE_END },
};

struct SceneEvent gLamaTemple_Events[] = {
    { EVENT_EXIT, 1, CONDITION_ALWAYS, LAMA_TEMPLE_TO_WORLD_MAP },
    { TOUCH_WALKING_INTO(FACING_NORTH), TRIGGER_TEMPLE_DOOR, CONDITION_ALWAYS, EVENT_SCRIPT(LamaTemple_EnterHamasRoom) },
    { EVENT_TALK, ACTOR_GREETING_MONK, FLAG_HSU_RESCUED, MSGNO(MsgAlpineCrossingLamaTempleThisIsLamaTempleWelcomeTravelers) },
    { EVENT_TALK, ACTOR_EASTWARD_MONK, FLAG_HSU_RESCUED, EVENT_SCRIPT(LamaTemple_TalkToEastwardMonk) },
    { EVENT_TALK, ACTOR_MEDITATING_MONK, FLAG_HSU_RESCUED, EVENT_SCRIPT(LamaTemple_TalkToMeditatingMonk) },
    { EVENT_TALK, ACTOR_ANXIOUS_MONK, FLAG_HSU_RESCUED, MSGNO(MsgAlpineCrossingLamaTempleYouWhoSpeakToMeI) },
    { EVENT_TALK, ACTOR_CHI_MONK, FLAG_HSU_RESCUED, EVENT_SCRIPT(LamaTemple_TalkToChiMonk) },
    { EVENT_TALK, ACTOR_GREETING_MONK, CONDITION_ALWAYS, MSGNO(MsgAlpineCrossingLamaTempleItIsVeryDangerousToCross) },
    { EVENT_TALK, ACTOR_EASTWARD_MONK, CONDITION_ALWAYS, MSGNO(MsgAlpineCrossingLamaTempleTheEvilDesertIsHotterThan) },
    { EVENT_TALK, ACTOR_MEDITATING_MONK, CONDITION_ALWAYS, MSGNO(MsgAlpineCrossingLamaTempleIBelieveItIsPossibleTo) },
    { EVENT_TALK, ACTOR_ANXIOUS_MONK, CONDITION_ALWAYS, MSGNO(MsgAlpineCrossingLamaTempleIWillBeMostThankfulOnce) },
    { EVENT_TALK, ACTOR_CHI_MONK, CONDITION_ALWAYS, MSGNO(MsgAlpineCrossingLamaTempleWhenYourTemperatureReachesItsMaximum) },
    { EVENT_TALK, ACTOR_TRAVELING_HEALER, CONDITION_ALWAYS, EVENT_SCRIPT(LamaTemple_TalkToHealer) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_GREETING_MONK, FLAG_HSU_RESCUED, MSGNO(MsgAlpineCrossingLamaTempleTheySeemToBeAnxiousThey) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_EASTWARD_MONK, FLAG_HSU_RESCUED, MSGNO(MsgAlpineCrossingLamaTempleTheySayAPersonCanOvercome) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_MEDITATING_MONK, FLAG_HSU_RESCUED, MSGNO(MsgAlpineCrossingLamaTempleIAmHungry) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_ANXIOUS_MONK, FLAG_HSU_RESCUED, MSGNO(MsgAlpineCrossingLamaTempleISenseImpureThoughtsFromThe) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_CHI_MONK, FLAG_HSU_RESCUED, MSGNO(MsgAlpineCrossingLamaTempleChiIsButAFragmentOf) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_GREETING_MONK, CONDITION_ALWAYS, MSGNO(MsgAlpineCrossingLamaTempleICannotCrossTheDesertIt) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_EASTWARD_MONK, CONDITION_ALWAYS, MSGNO(MsgAlpineCrossingLamaTempleTheEvilInTheDesertDistorts) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_MEDITATING_MONK, CONDITION_ALWAYS, MSGNO(MsgAlpineCrossingLamaTempleHowNiceItWouldBeIf) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_ANXIOUS_MONK, CONDITION_ALWAYS, MSGNO(MsgAlpineCrossingLamaTempleMasterHamaWillBeVerySad) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_CHI_MONK, CONDITION_ALWAYS, MSGNO(MsgAlpineCrossingLamaTempleWithRevealYouWillFindOases) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_TRAVELING_HEALER, CONDITION_ALWAYS, MSGNO(MsgAlpineCrossingLamaTempleForSomeReasonThesePeopleGive) },
    { PSYNERGY_IN_AREA(ABILITY_REVEAL, 5), 0, CONDITION_ALWAYS, EVENT_SCRIPT(LamaTemple_RevealPassage) },
    { PSYNERGY_IN_AREA(ABILITY_REVEAL, 4), 0, CONDITION_ALWAYS, EVENT_SCRIPT(LamaTemple_ShowFalseWall) },
    { SEARCH(SEARCH_UNNAMED), TRIGGER_TEMPLE_WALL_WRITING, CONDITION_ALWAYS, EVENT_SCRIPT(LamaTemple_ReadWallWriting) },
    { SEARCH_FACING(FACING_NORTH), TRIGGER_STONE, FLAG_LAMA_TEMPLE_PSYNERGY_STONE, FIND_PSYNERGY_STONE },
    { SEARCH_FACING(FACING_NORTHEAST), TRIGGER_STONE, FLAG_LAMA_TEMPLE_PSYNERGY_STONE, FIND_PSYNERGY_STONE },
    { SEARCH_FACING(FACING_NORTHWEST), TRIGGER_STONE, FLAG_LAMA_TEMPLE_PSYNERGY_STONE, FIND_PSYNERGY_STONE },
    { SEARCH_FACING(FACING_WEST), TRIGGER_STONE, FLAG_LAMA_TEMPLE_PSYNERGY_STONE, FIND_PSYNERGY_STONE },
    { SEARCH(SEARCH_CHEST), TRIGGER_CHEST, FLAG_LAMA_TEMPLE_WATER_OF_LIFE, FIND_ITEM(ITEM_WATER_OF_LIFE) },
    { SCENE_EVENTS_END },
};

struct SceneEvent gAlpineCrossing_Events[] = {
    { EVENT_EXIT, 1, CONDITION_ALWAYS, ALPINE_CROSSING_TO_WORLD_MAP_EAST },
    { EVENT_EXIT, 2, CONDITION_ALWAYS, ALPINE_CROSSING_TO_WORLD_MAP_NORTH },
    { EVENT_EXIT, 3, CONDITION_ALWAYS, ALPINE_CROSSING_TO_WORLD_MAP_WEST },
    { EVENT_EXIT, 5, CONDITION_ALWAYS, ALPINE_CROSSING_TO_ALTIN_PEAK_SOUTH_ENTRANCE_WEST },
    { EVENT_EXIT, 4, CONDITION_ALWAYS, ALPINE_CROSSING_TO_ALTIN_PEAK_SOUTH_ENTRANCE_EAST },
    { EVENT_TOUCH, TRIGGER_FEIZHI_ARRIVAL, FLAG_FEIZHI_SAW_ROCKSLIDE, EVENT_SCRIPT(AlpineCrossing_FeizhiArrives) },
    { EVENT_TALK, ACTOR_FEIZHI, FLAG_XIAN_HEARD_HSU_TRAPPED, EVENT_SCRIPT(AlpineCrossing_TalkToFeizhi) },
    { EVENT_TALK, ACTOR_HAMA, CONDITION_ALWAYS, MSGNO(MsgAlpineCrossingLamaTempleWellIsaacAdeptsYouCameTo) },
    { EVENT_TALK, ACTOR_HSU, CONDITION_ALWAYS, MSGNO(MsgAlpineCrossingLamaTempleIAmTrappedBetweenTheseTwo) },
    { EVENT_TALK, ACTOR_FEIZHI, CONDITION_ALWAYS, MSGNO(MsgAlpineCrossingLamaTempleBeStrongHsuWeWillHelp) },
    { EVENT_TALK, ACTOR_FIRST_WORKER, CONDITION_ALWAYS, MSGNO(MsgAlpineCrossingLamaTempleWeMovedTheRocksNowPeople) },
    { EVENT_TALK, ACTOR_SECOND_WORKER, CONDITION_ALWAYS, MSGNO(MsgAlpineCrossingLamaTempleManyPeopleFromXianHelpedMove) },
    { PSYNERGY_ON_ACTOR(ABILITY_LIFT), ACTOR_BOULDER, FLAG_HSU_RESCUED, EVENT_SCRIPT(AlpineCrossing_LiftBoulder) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_FEIZHI, FLAG_XIAN_HEARD_HSU_TRAPPED, MSGNO(MsgAlpineCrossingLamaTempleHsuIsInDangerICan) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_HAMA, CONDITION_ALWAYS, MSGNO(MsgAlpineCrossingLamaTempleHowShouldWeMoveTheBoulders) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_HSU, CONDITION_ALWAYS, MSGNO(MsgAlpineCrossingLamaTempleIWantToScreamButFeizhi) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_FEIZHI, CONDITION_ALWAYS, MSGNO(MsgAlpineCrossingLamaTempleHsuTiresHeLooksExhaustedI) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_FIRST_WORKER, CONDITION_ALWAYS, MSGNO(MsgAlpineCrossingLamaTempleWeClearedTheWayButWhat) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_SECOND_WORKER, CONDITION_ALWAYS, MSGNO(MsgAlpineCrossingLamaTempleIAlmostHurtMyBackIt) },
    { PSYNERGY_IN_AREA(ABILITY_REVEAL, 5), 0, CONDITION_ALWAYS, EVENT_SCRIPT(AlpineCrossing_RevealPath) },
    { PSYNERGY_IN_AREA(ABILITY_REVEAL, 4), 0, CONDITION_ALWAYS, EVENT_SCRIPT(AlpineCrossing_HidePath) },
    // Frost's first phase, as it reaches the puddle
    { PSYNERGY_PHASE_ON_ACTOR(ABILITY_FROST, 1), ACTOR_PUDDLE, FLAG_PUDDLE_FROZEN, EVENT_SCRIPT(AlpineCrossing_PrepareFrost) },
    { PSYNERGY_ON_ACTOR(ABILITY_FROST), ACTOR_PUDDLE, FLAG_PUDDLE_FROZEN, EVENT_SCRIPT(AlpineCrossing_FreezePuddle) },
    { PSYNERGY_ON_ACTOR(ABILITY_GROWTH), ACTOR_SPROUT, FLAG_SPROUT_GROWN, EVENT_SCRIPT(AlpineCrossing_GrowSprout) },
    { SEARCH(SEARCH_UNNAMED), TRIGGER_SIGNPOST, CONDITION_ALWAYS, EVENT_SCRIPT(AlpineCrossing_ReadSignpost) },
    { SEARCH(SEARCH_CHEST), TRIGGER_CHEST, FLAG_ALPINE_CROSSING_POWER_BREAD, FIND_ITEM(ITEM_POWER_BREAD) },
    { SCENE_EVENTS_END },
};
// clang-format on
