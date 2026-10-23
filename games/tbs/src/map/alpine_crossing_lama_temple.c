/// @file
/// The Alpine Crossing (MapId_AlpineCrossing) and Lama Temple
/// (MapId_LamaTemple), two maps on Silk Road west of Xian that share this
/// overlay. The getters pick each map's tables by gGameState.mapId, both
/// maps share one warp table, and Map_Initialize sets up the map being
/// entered from the story flags of Hsu's rescue, which runs through both.
///
/// At the Alpine Crossing boulders block Silk Road. Once Feizhi has left Xian
/// (FLAG_FEIZHI_WENT_TO_HSU, set in fuchin_temple.c), stepping onto the road
/// beside them (TRIGGER_FEIZHI_ARRIVAL) plays AlpineCrossing_FeizhiArrives and
/// sets FLAG_FEIZHI_SAW_ROCKSLIDE; she then waits at the end of the path until
/// Hama's lesson. A signpost reads `North: Altin Mine West: Lama Temple`; the
/// Crossing opens onto the world map to the east, north and west, and by
/// ALPINE_CROSSING_TO_ALTIN_PEAK_SOUTH_ENTRANCE_WEST and
/// ALPINE_CROSSING_TO_ALTIN_PEAK_SOUTH_ENTRANCE_EAST into Altin Peak's
/// AltinPeakSouthEntrance, whose warp destinations from here the international
/// editions also call Alpine Crossing. Coming back from the mine
/// (ALPINE_CROSSING_FROM_ALTIN_PEAK_SOUTH_ENTRANCE_WEST and _EAST) the area
/// name is not shown again.
///
/// In Lama Temple the five monks have one set of lines and thoughts until Hsu
/// is freed (FLAG_HSU_RESCUED) and another after, and a traveling healer serves
/// across his counter; a defeat anywhere here returns the party to
/// LAMA_TEMPLE_AFTER_DEFEAT, in front of him. Walking north into the sliding
/// door (LamaTemple_EnterHamasRoom) leads into Hama's room, in
/// FuchinTempleInteriors (FUCHIN_TEMPLE_INTERIORS_FROM_LAMA_TEMPLE, in
/// fuchin_temple.c), whose lesson sets FLAG_XIAN_HEARD_HSU_TRAPPED; coming back
/// out (LAMA_TEMPLE_FROM_FUCHIN_TEMPLE_INTERIORS) the area name is not shown
/// again. Writing on a wall hints at Reveal, which shows a passage behind a
/// mossy rock (LamaTemple_RevealPassage); LamaTemple_ShowFalseWall puts the
/// rock back when Reveal wears off and on entry, the rock cell's own attributes
/// saved at (21, 73). The temple also holds a Psynergy Stone
/// (FLAG_LAMA_TEMPLE_PSYNERGY_STONE, a map-section temp flag) and a chest with
/// Water of Life.
///
/// Back at the Alpine Crossing after the lesson, until Hsu is freed, Hama and
/// Feizhi stand by him, pinned between two boulders. Arriving beside him from
/// AltinPeakSouthEntrance (ALPINE_CROSSING_FROM_ALTIN_PEAK_SOUTH_ENTRANCE_EAST)
/// the first time plays AlpineCrossing_HamaReachesHsu
/// (FLAG_HAMA_REACHED_HSU). Lift on the boulder (AlpineCrossing_LiftBoulder)
/// raises it while Hama and Feizhi pull him free; the scene sets
/// FLAG_HSU_RESCUED and leaves for Lama Temple (ALPINE_CROSSING_TO_LAMA_TEMPLE,
/// to LAMA_TEMPLE_FROM_ALPINE_CROSSING), where Map_Initialize plays Hama's
/// farewell (LamaTemple_RunHamasFarewell). Coming back instead by the world
/// map's north side (ALPINE_CROSSING_FROM_WORLD_MAP_NORTH), Hsu has been freed
/// without the party (FLAG_HSU_RESCUED_WITHOUT_PARTY, which lamakan_desert.c
/// can set too), and Hama and Feizhi are gone. Once he is free, two workers
/// from Xian stand by the road they have cleared, and Map_Initialize copies
/// the cleared cells over the rocks. The Crossing's other puzzles are kept
/// only within one visit (map temp flags): Reveal shows a hidden path, Frost
/// freezes a puddle into a pillar the party can climb, and Growth grows a
/// sprout into a vine; a chest holds Power Bread.
///
/// Open questions: none.

#include "action_script.h"
#include "game_state.h"
#include "unit/map/alpine_crossing_lama_temple.h"
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
#include "script_layer.h"
#include "map_tables.h"
#include "map_ids.h"
#include "call.h"
#include "text/msg_ids.h"
#include "ability_ids.h"
#include "flags.h"
#include "item_ids.h"
#include "objcmd.h"
#include "field_effect.h"
#include "songs.h"

/// The temple's Psynergy Stone, a map-section temp flag: it refills once the
/// party has been to another map section.
/// ★ Named from the game: "a Psynergy Stone" (MsgGotPsynergyStone)
#define FLAG_LAMA_TEMPLE_PSYNERGY_STONE FLAG_MAPSEC_TEMP_50

#define MAP_ALPINE_CROSSING ((s32)&MapId_AlpineCrossing)
#define MAP_LAMA_TEMPLE ((s32)&MapId_LamaTemple)

/// Lama Temple's object events. Garet, Ivan and Mia are placed in their own
/// party slots for Hama's farewell; the others count from LOCALID_FIRST_PLACED.
enum LamaTempleLocalId {
  /// Hama, back in her temple for her farewell.
  /// ★ Named from the game: "No, Master Hama"
  /// (MsgMountainAdeptsLetMeThankAgain)
  LOCALID_TEMPLE_HAMA = LOCALID_FIRST_PLACED,
  /// ★ Named from the game: "This is Lama Temple. Welcome, travelers."
  /// (MsgAlpineCrossingLamaTempleThisIsLamaTempleWelcomeTravelers)
  LOCALID_GREETING_MONK,
  /// ★ Named from the game: "Young warriors, do you come from the east?"
  /// (MsgMountainYoungWarriorsDoComeFrom)
  LOCALID_MONK_ASKING_ABOUT_THE_EAST,
  /// ★ Named from the game: "Do you know what meditation is?"
  /// (MsgMountainDoKnowMeditation)
  LOCALID_MEDITATING_MONK,
  /// ★ Named from the game: "I sense great insecurity and anxiety within you."
  /// (MsgAlpineCrossingLamaTempleYouWhoSpeakToMeI)
  LOCALID_MONK_SENSING_ANXIETY,
  /// ★ Named from the game: "Did you know that Master Hama is the greatest
  /// master of Chi?" (MsgMountainDidKnowMasterHamaGreatest)
  LOCALID_CHI_MONK,
  /// The rock Reveal takes away to show the passage (LamaTemple_RevealPassage).
  /// ★ Named from what the player sees: a round, moss-green rock on the grass
  /// east of the waterfall (its render)
  LOCALID_PASSAGE_ROCK,
  /// ★ Named from the game: "I am traveling around the world to spread good
  /// will." (MsgMountainAmTravelingAroundWorldSpread)
  LOCALID_TRAVELING_HEALER
};

/// Alpine Crossing's object events.
enum AlpineCrossingLocalId {
  /// ★ Named from the game: "Master Hama, I am okay." (MsgMountainHsuOkay)
  LOCALID_HAMA = LOCALID_FIRST_PLACED,
  /// ★ Named from the game: "Hsu, are you okay?" (MsgMountainHsuOkay)
  LOCALID_HSU,
  /// ★ Named from the game: "You must be glad, Feizhi." (MsgMountainHsuOkay)
  LOCALID_FEIZHI,
  /// ★ Named from the game: "did you lift the boulder?"
  /// (MsgMountainIsaacDidLiftBoulder)
  LOCALID_BOULDER,
  /// A young man in a white headband, one of the people of Xian who moved the
  /// rocks.
  /// ★ Named from the game: "We moved the rocks. Now people can pass."
  /// (MsgAlpineCrossingLamaTempleWeMovedTheRocksNowPeople)
  LOCALID_FIRST_WORKER,
  /// A young man with his hair tied back.
  /// ★ Named from the game: "Many people from Xian helped move the rocks."
  /// (MsgAlpineCrossingLamaTempleManyPeopleFromXianHelpedMove)
  LOCALID_SECOND_WORKER,
  /// The water Frost freezes into a pillar (AlpineCrossing_FreezePuddle).
  /// ★ Named from what the player sees: a small patch of blue water on the
  /// grass by the path (its render)
  LOCALID_WATER_FOR_FROST,
  /// The plant Growth grows into a vine (AlpineCrossing_GrowSprout).
  /// ★ Named from what the player sees: a small green sprout at the foot of
  /// the cliff (its render)
  LOCALID_PLANT
};

/// Alpine Crossing's Psynergy puzzles, kept while the party stays.
enum {
  /// Frost has frozen the puddle into an ice pillar.
  /// ★ Named from what the player sees: a small puddle of water on the path
  /// below the cliffs (its render)
  FLAG_PUDDLE_FROZEN = FLAG_MAP_TEMP_00,
  /// Growth has grown the sprout into a vine the party can climb.
  /// ★ Named from what the player sees: a small sprout at the foot of a cliff
  /// (its render)
  FLAG_SPROUT_GROWN = FLAG_MAP_TEMP_01
};

/// Animations of these maps' own sprites beyond enum ObjectEventAnimation,
/// named by when the scripts play them.
enum {
  ANIMATION_HAMA_PULL = 5,
  ANIMATION_FEIZHI_PULL = 6,
  /// Drops into a crouch, then holds a half crouch with her head bowed
  /// (spr_039.png's frames 33 and 36).
  ANIMATION_FEIZHI_CROUCH = 5,
  /// SPRITE_TURNING_ROCK's mossy rock (spr_293.png's frame 1); its first
  /// animations show frame 0, a purple glow.
  TURNING_ROCK_ANIMATION_MOSSY = 2,
  ANIMATION_HSU_STRUGGLE = 5,
  ANIMATION_MONK_MEDITATE = 5,
  PUDDLE_ANIMATION_FROZEN = 5,
  SPROUT_ANIMATION_GROWN = 4
};

/// Triggers that the map events below refer to.
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

/// The map object that draws the passage behind the mossy rock.
enum { MARKER_PASSAGE = 100 };

/// Draws the object event behind the background, on sprite priority 3 with
/// automatic priority off. The first worker from Xian at the Alpine Crossing
/// runs it from his object event template.
static s32
ObjectEvent_DrawBehindScenery(struct ScriptInterpreter *interpreter) {
  union FieldObject *fieldObject = (union FieldObject *)interpreter;
  fieldObject->objectEvent.priorityFlags &= ~OBJ_EVENT_PRIORITY_AUTOMATIC;
  fieldObject->objectEvent.sprite->image.priority = 3;
  fieldObject->objectEvent.sprite->shadow.priority = 3;
  return 0;
}

// Update callback that turns the party leader toward Feizhi while she runs
// past.
// UB: update hooks return void. Plain void removes the native zero result
// and changes its return sequence (three instructions in tbs-en).
#ifdef UBFIX
static void Leader_FaceFeizhi(union FieldObject *fieldObject) {
#else
static s32 Leader_FaceFeizhi(union FieldObject *fieldObject) {
#endif
  struct ObjectEvent *leader = (struct ObjectEvent *)fieldObject;
  struct ObjectEvent *feizhi;

  feizhi = ObjectEvent_GetById(LOCALID_FEIZHI);
  leader->facing = ArcTan2(feizhi->z.fixed - leader->z.fixed,
                           feizhi->x.fixed - leader->x.fixed);
#ifndef UBFIX
  return 0;
#endif
}

const struct MapWarpDest *Map_GetWarpDests(void) {
  if (gGameState.mapId == MAP_ALPINE_CROSSING) {
    return gAlpineCrossing_WarpDests;
  }
  return gLamaTemple_WarpDests;
}

const struct MapWarpRegion *Map_GetWarpRegions(void) { return 0; }

/// Both maps share one warp table.
const u32 *Map_GetWarps(void) { return gAlpineCrossingLamaTemple_Warps; }

const struct ObjectEventTemplate *Map_GetObjectEventTemplates(void) {
  if (gGameState.mapId == MAP_ALPINE_CROSSING) {
    return gAlpineCrossing_ObjectEventTemplates;
  }
  return gLamaTemple_ObjectEventTemplates;
}

/// A monk in Lama Temple, until Hsu is freed, asks whether the party comes
/// from the east. If not, it must have crossed Lamakan Desert; if so, he asks
/// whether it stopped in Xian, whose Master Feh is close to Master Hama.
static void LamaTemple_TalkToMonkAskingAboutTheEast(void) {

  Engine_ScriptStart();
  Engine_ScriptSetMessage(MsgMountainYoungWarriorsDoComeFrom);
  Engine_ScriptOpenMessage(LOCALID_MONK_ASKING_ABOUT_THE_EAST, 0);

  if (Engine_ScriptChooseYesNo(LOCALID_PARTY_LEADER, 0) == 1) {
    Engine_ScriptWait(20);
    Engine_ScriptShowMessage(LOCALID_MONK_ASKING_ABOUT_THE_EAST, 0);
  } else {
    Script_SkipMessages(1);
    Engine_ScriptAskYesNo(LOCALID_MONK_ASKING_ABOUT_THE_EAST, 0);
  }

  Engine_ScriptEnd();
}

/// The meditating monk explains meditation. He's in Lama Temple until Hsu is
/// freed, and first asks whether the party knows what meditation is.
static void LamaTemple_TalkToMeditatingMonk(void) {
  Engine_ScriptStart();
  Engine_ScriptSetMessage(MsgMountainDoKnowMeditation);
  Engine_ScriptAskYesNo(LOCALID_MEDITATING_MONK, 0);
  Engine_ScriptEnd();
}

/// The monk in Lama Temple who, until Hsu is freed, praises Master Hama as the
/// greatest master of Chi.
static void LamaTemple_TalkToChiMonk(void) {
  Engine_ScriptStart();
  Engine_ScriptSetMessage(MsgMountainDidKnowMasterHamaGreatest);
  Engine_ScriptAskYesNo(LOCALID_CHI_MONK, 0);
  Engine_ScriptEnd();
}

/// The door of Hama's room slides open for the party leader. Walking north into
/// it in Lama Temple slides it open and walks the leader through, into the room
/// (LAMA_TEMPLE_TO_FUCHIN_TEMPLE_INTERIORS).
static void LamaTemple_EnterHamasRoom(void) {
  Script_PlayCue(SE_188);
  MapGrid_AnimateCells(gLamaTemple_DoorOpenCells, DOOR_X, DOOR_Y);
  ObjectEvent_GetById(LOCALID_PARTY_LEADER)->motionFlags = 0;
  ObjectEvent_SetSpeed(LOCALID_PARTY_LEADER, 0xcccc, 0x6666);
  gEventWork->startTransition = MAP_TRANSITION(TRANSITION_BACKDROP_FADE, 0);
  Engine_ObjectEventSetAnimation(LOCALID_PARTY_LEADER, ANIMATION_WALK);
  ObjectEvent_SetDestinationOffset(LOCALID_PARTY_LEADER, 0, -16);
  Engine_ScriptWait(16);
  Engine_ScriptRequestWarp(LAMA_TEMPLE_TO_FUCHIN_TEMPLE_INTERIORS);
}

/// Hama's farewell, once the party has freed Hsu and been brought back to Lama
/// Temple: she thanks the Adepts, Garet and Mia squabble over whose idea
/// helping was, and Hama goes back to her room. Ivan wonders aloud about what
/// Hama told him, and Garet, Ivan and Mia rejoin the party. Garet's two
/// questions are yes or no choices.
static void LamaTemple_RunHamasFarewell(void) {
  Engine_ScriptStart();
  ObjectEvent_SetPosition(LOCALID_TEMPLE_HAMA, PIXELS(136), PIXELS(168));
  ObjectEvent_FaceDirection(LOCALID_TEMPLE_HAMA, FACING_SOUTH + FACING_STEP, 0);
  ObjectEvent_SetPosition(LOCALID_PARTY_LEADER, PIXELS(144), PIXELS(200));
  ObjectEvent_SetPosition(GARET, PIXELS(160), PIXELS(192));
  ObjectEvent_SetPosition(IVAN, PIXELS(128), PIXELS(200));
  ObjectEvent_SetPosition(MIA, PIXELS(112), PIXELS(192));
  ObjectEvent_FaceDirection(LOCALID_PARTY_LEADER, FACING_NORTH, 0);
  ObjectEvent_FaceDirection(GARET, FACING_NORTHWEST, 0);
  ObjectEvent_FaceDirection(IVAN, FACING_NORTH, 0);
  ObjectEvent_FaceDirection(MIA, FACING_NORTHEAST, 0);
  gEventWork->startTransition =
      MAP_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_SHAPE);
  Engine_ScriptOpenScreen();
  Engine_ScriptWaitForScreen();
  Engine_ScriptWait(60);

  // "Adepts, let me thank you again."
  Engine_ObjectEventSetAnimationAndWait(LOCALID_TEMPLE_HAMA, ANIMATION_NOD);
  Engine_ScriptWait(20);
  Engine_ScriptSetMessage(MsgMountainAdeptsLetMeThankAgain);
  Script_ShowMessageAndWait(LOCALID_TEMPLE_HAMA, 0, 20);
  Engine_ObjectEventSetAnimation(LOCALID_PARTY_LEADER, ANIMATION_NOD);
  Engine_ObjectEventSetAnimation(GARET, ANIMATION_NOD);
  Engine_ObjectEventSetAnimation(IVAN, ANIMATION_NOD);
  Engine_ObjectEventSetAnimationAndWait(MIA, ANIMATION_NOD);
  Engine_ScriptWait(20);
  Engine_ObjectEventSetAnimationAndWait(LOCALID_TEMPLE_HAMA,
                                        ANIMATION_SHAKE_HEAD);
  Engine_ScriptWait(20);
  Script_ShowMessageAndWait(LOCALID_TEMPLE_HAMA, 0, 20);
  Engine_ObjectEventSetAnimationAndWait(MIA, ANIMATION_SHAKE_HEAD);
  Engine_ScriptWait(20);
  Script_ShowMessageAndWait(MIA, 0, 20);
  Engine_ObjectEventRunRepeatedMotion(IVAN, 2);
  Engine_ScriptWait(20);
  Script_ShowMessageAndWait(IVAN, 0, 20);
  Engine_ObjectEventRunRepeatedMotion(LOCALID_TEMPLE_HAMA, 2);
  ObjectEvent_FaceDirection(LOCALID_TEMPLE_HAMA, FACING_SOUTH + FACING_STEP,
                            20);
  Script_ShowMessageAndWait(LOCALID_TEMPLE_HAMA, 0, 20);
  ObjectEvent_ShowEmote(IVAN, EMOTE_IN_FRONT | EMOTE_WINCE, 60);
  Engine_ScriptWait(120);

  // Garet claims that helping was his idea and asks the leader to agree.
  Engine_ObjectEventRunRepeatedMotion(GARET, 2);
  Engine_ScriptWait(20);
  ObjectEvent_FaceDirection(GARET, FACING_SOUTHWEST, 20);
  Script_OpenMessage(GARET, 0);
  if (Engine_ScriptChooseYesNo(LOCALID_PARTY_LEADER, 0) == 0) {
    Engine_ScriptWait(20);
    ObjectEvent_FaceDirection(MIA, FACING_EAST, 20);
    ObjectEvent_ShowEmote(MIA, EMOTE_IN_FRONT | EMOTE_QUESTION, 60);
    Script_ShowMessageAndWait(MIA, 0, 20);
    ObjectEvent_FaceDirection(LOCALID_PARTY_LEADER, FACING_NORTHEAST, 0);
    ObjectEvent_FaceDirection(IVAN, FACING_NORTHEAST, 0);
    Engine_ObjectEventStartRepeatedMotion(GARET, 2);
    ObjectEvent_SetAttachedEffect(GARET, ATTACHED_EFFECT_IN_FRONT |
                                             ATTACHED_EFFECT_ONCE);
    Engine_ScriptWait(60);
    ObjectEvent_FaceDirection(GARET, FACING_WEST, 20);
    Script_ShowMessageAndWait(GARET, 0, 20);
    Script_SkipMessages(2);
  } else {
    Script_SkipMessages(2);
    Engine_ScriptWait(20);
    ObjectEvent_FaceDirection(MIA, FACING_EAST, 20);
    Engine_ObjectEventSetAnimationAndWait(MIA, ANIMATION_NOD);
    Engine_ScriptWait(20);
    Script_ShowMessageAndWait(MIA, 0, 20);
    ObjectEvent_ShowEmote(GARET, EMOTE_IN_FRONT | EMOTE_WINCE, 60);
    ObjectEvent_FaceDirection(LOCALID_PARTY_LEADER, FACING_NORTHEAST, 0);
    ObjectEvent_FaceDirection(IVAN, FACING_NORTHEAST, 0);
    Script_ShowMessageAndWait(GARET, 0, 20);
  }

  // Hama wishes them well and goes back into her room.
  Engine_ObjectEventRunRepeatedMotion(LOCALID_TEMPLE_HAMA, 2);
  Engine_ScriptWait(20);
  ObjectEvent_FaceDirection(LOCALID_PARTY_LEADER, FACING_NORTH, 0);
  ObjectEvent_FaceDirection(GARET, FACING_NORTHWEST, 0);
  ObjectEvent_FaceDirection(IVAN, FACING_NORTH, 0);
  ObjectEvent_FaceDirection(MIA, FACING_NORTHEAST, 0);
  Engine_ScriptWait(20);
  ObjectEvent_FaceDirection(LOCALID_TEMPLE_HAMA, FACING_SOUTH - FACING_STEP,
                            20);
  Script_ShowMessageAndWait(LOCALID_TEMPLE_HAMA, 0, 20);
  Engine_ObjectEventRunRepeatedMotion(GARET, 2);
  Engine_ScriptWait(20);
  Engine_ObjectEventSetAnimationAndWait(GARET, ANIMATION_NOD);
  Engine_ScriptWait(30);
  Engine_ObjectEventSetAnimationAndWait(LOCALID_TEMPLE_HAMA, ANIMATION_NOD);
  Engine_ScriptWait(20);
  Script_ShowMessageAndWait(LOCALID_TEMPLE_HAMA, 0, 20);
  Engine_ObjectEventRunRepeatedMotion(LOCALID_TEMPLE_HAMA, 2);
  Engine_ScriptWait(20);
  Script_ShowMessageAndWait(LOCALID_TEMPLE_HAMA, 0, 20);
  Engine_ObjectEventSetAnimation(LOCALID_PARTY_LEADER, ANIMATION_NOD);
  Engine_ObjectEventSetAnimation(GARET, ANIMATION_NOD);
  Engine_ObjectEventSetAnimation(IVAN, ANIMATION_NOD);
  Engine_ObjectEventSetAnimationAndWait(MIA, ANIMATION_NOD);
  Engine_ScriptWait(20);
  Engine_ObjectEventSetAnimationAndWait(LOCALID_TEMPLE_HAMA, ANIMATION_NOD);
  Engine_ScriptWait(20);
  ObjectEvent_FaceDirection(LOCALID_TEMPLE_HAMA, FACING_NORTH, 30);
  // The door slides open with SE_188, Hama walks through, and it slides shut.
  Script_PlayCue(SE_188);
  MapGrid_AnimateCells(gLamaTemple_DoorOpenCells, DOOR_X, DOOR_Y);
  ObjectEvent_SetSpeed(LOCALID_TEMPLE_HAMA, 0xcccc, 0x6666);
  ObjectEvent_WalkToAndWait(LOCALID_TEMPLE_HAMA, 136, 136);
  ObjectEvent_SetPosition(LOCALID_TEMPLE_HAMA, 0, 0);
  Script_PlayCue(SE_188);
  MapGrid_AnimateCells(gLamaTemple_DoorCloseCells, DOOR_X, DOOR_Y);
  Engine_ScriptWait(60);
  FieldEffect_PlayQueuedSound();

  // Garet asks whether the leader means to leave Hama's words unasked.
  Engine_ObjectEventRunRepeatedMotion(GARET, 2);
  Engine_ScriptWait(20);
  ObjectEvent_FaceDirection(GARET, FACING_SOUTHWEST, 20);
  Script_ShowMessageAndWait(GARET, 0, 20);
  ObjectEvent_FaceDirection(LOCALID_PARTY_LEADER, FACING_WEST, 20);
  Engine_ObjectEventRunRepeatedMotion(IVAN, 2);
  Engine_ScriptWait(20);
  ObjectEvent_FaceDirection(MIA, FACING_SOUTHEAST, 20);
  Script_ShowMessageAndWait(MIA, 0, 20);
  Engine_ObjectEventRunRepeatedMotion(GARET, 1);
  ObjectEvent_FaceDirection(LOCALID_PARTY_LEADER, FACING_NORTHEAST, 20);
  Script_AskYesNo(GARET, 0);
  Engine_ScriptWait(20);

  // Ivan decides that Hama will tell him when their mission is done.
  Engine_ObjectEventRunRepeatedMotion(IVAN, 2);
  Engine_ScriptWait(20);
  ObjectEvent_FaceDirection(LOCALID_PARTY_LEADER, FACING_WEST, 20);
  Script_ShowMessageAndWait(IVAN, 0, 20);
  Engine_ObjectEventStartRepeatedMotion(LOCALID_PARTY_LEADER, 1);
  Engine_ObjectEventStartRepeatedMotion(GARET, 1);
  Engine_ObjectEventRunRepeatedMotion(MIA, 1);
  Engine_ScriptWait(20);
  Engine_ObjectEventSetAnimationAndWait(IVAN, ANIMATION_SHAKE_HEAD);
  Engine_ScriptWait(20);
  Script_ShowMessageAndWait(IVAN, 0, 20);
  Engine_ObjectEventRunRepeatedMotion(MIA, 2);
  Engine_ScriptWait(20);
  Script_ShowMessageAndWait(MIA, 0, 20);
  ObjectEvent_FaceDirection(IVAN, FACING_NORTHWEST + FACING_STEP, 20);
  Engine_ObjectEventSetAnimationAndWait(IVAN, ANIMATION_NOD);
  Engine_ScriptWait(20);
  Engine_ObjectEventSetAnimationAndWait(IVAN, ANIMATION_NOD);
  Engine_ScriptWait(20);
  Script_ShowMessageAndWait(IVAN, 0, 20);
  ObjectEvent_ShowEmote(GARET, EMOTE_IN_FRONT | EMOTE_WINCE, 60);
  Script_ShowMessageAndWait(GARET, 0, 20);
  ObjectEvent_FaceDirection(IVAN, FACING_NORTHEAST, 20);
  Engine_ObjectEventSetAnimationAndWait(IVAN, ANIMATION_SHAKE_HEAD);
  Engine_ScriptWait(20);
  Script_ShowMessageAndWait(IVAN, 0, 20);
  ObjectEvent_ShowEmote(LOCALID_PARTY_LEADER, EMOTE_IN_FRONT | EMOTE_WINCE, 0);
  ObjectEvent_ShowEmote(MIA, EMOTE_IN_FRONT | EMOTE_WINCE, 60);
  Script_ShowMessageAndWait(MIA, 0, 20);
  ObjectEvent_FaceDirection(IVAN, FACING_NORTHWEST, 20);
  Engine_ObjectEventSetAnimationAndWait(IVAN, ANIMATION_NOD);
  Engine_ScriptWait(20);
  Script_ShowMessageAndWait(IVAN, 0, 20);
  Engine_ObjectEventStartRepeatedMotion(LOCALID_PARTY_LEADER, 1);
  Engine_ObjectEventStartRepeatedMotion(GARET, 1);
  Engine_ObjectEventRunRepeatedMotion(MIA, 1);
  Engine_ScriptWait(20);
  ObjectEvent_SetSpeed(IVAN, 0x8000, 0x4000);
  ObjectEvent_WalkToAndWait(IVAN, 128, 184);
  ObjectEvent_FaceDirection(IVAN, FACING_SOUTH, 20);
  ObjectEvent_FaceDirection(LOCALID_PARTY_LEADER, FACING_NORTH, 0);
  ObjectEvent_FaceDirection(GARET, FACING_NORTHWEST, 0);
  ObjectEvent_FaceDirection(MIA, FACING_NORTHEAST, 20);
  Engine_ObjectEventSetAnimationAndWait(IVAN, ANIMATION_NOD);
  Engine_ScriptWait(20);
  Script_ShowMessageAndWait(IVAN, 0, 20);
  Engine_ObjectEventSetAnimation(LOCALID_PARTY_LEADER, ANIMATION_NOD);
  Engine_ObjectEventSetAnimation(GARET, ANIMATION_NOD);
  Engine_ObjectEventSetAnimationAndWait(MIA, ANIMATION_NOD);
  Engine_ScriptWait(20);

  // Garet, Ivan and Mia walk to the leader and rejoin the party.
  ObjectEvent_SetSpeed(GARET, 0x8000, 0x4000);
  ObjectEvent_SetSpeed(MIA, 0x8000, 0x4000);
  ObjectEvent_WalkTo(GARET, 144, 200);
  ObjectEvent_WalkTo(IVAN, 144, 200);
  ObjectEvent_WalkTo(MIA, 144, 200);
  Engine_ObjectEventWaitForMove(GARET);
  ObjectEvent_SetPosition(GARET, 0, 0);
  Engine_ObjectEventWaitForMove(IVAN);
  ObjectEvent_SetPosition(IVAN, 0, 0);
  Engine_ObjectEventWaitForMove(MIA);
  ObjectEvent_SetPosition(MIA, 0, 0);
  Engine_ScriptEnd();
}

/// Searching the writing on a wall in Lama Temple reads its hint toward Reveal.
static void LamaTemple_ReadWallWriting(void) {

  Engine_ScriptStart();
  Engine_ObjectEventSetAnimation(LOCALID_PARTY_LEADER, ANIMATION_STAND);
  Engine_MessageShowCentered(MsgMountainHeWhoHasPowerSee, 1);
  Engine_ScriptEnd();
}

/// Puts Lama Temple's mossy rock back when Reveal wears off (and on entry):
/// the wall's cell blocks the way again, the passage's map object is hidden and
/// the rock object event stands in front of it.
static void LamaTemple_ShowFalseWall(void) {

  MapGrid_CopyCellAttributes(85, 9, 1, 1, 21, 9);
  Engine_MarkerSetPosition(MARKER_PASSAGE, 0, 0);
  ObjectEvent_SetPosition(LOCALID_PASSAGE_ROCK, PIXELS(344), PIXELS(152));
}

/// Reveal in Lama Temple shows the passage behind the mossy rock: the cell's
/// original attributes (saved at (21, 73) on entry) come back, the passage's
/// map object returns to its place and the rock object event is put away.
static void LamaTemple_RevealPassage(void) {
  MapGrid_CopyCellAttributes(21, 73, 1, 1, 21, 9);
  Marker_SetPosition(MARKER_PASSAGE, -1, -1);
  ObjectEvent_SetPosition(LOCALID_PASSAGE_ROCK, 0, 0);
}

/// The traveling healer in Lama Temple offers the Sanctum's services. He does
/// when spoken to across his counter, from the south; from any other side he
/// only greets the party.
static void LamaTemple_TalkToHealer(void) {

  u32 heading = ObjectEvent_GetById(LOCALID_PARTY_LEADER)->facing;

  Engine_ScriptStart();
  if (FACING_IS_NORTH(heading)) {
    Engine_SanctumOpen(LOCALID_TRAVELING_HEALER);
  } else {
    Engine_ScriptSetMessage(MsgMountainAmTravelingAroundWorldSpread);
    Engine_ScriptShowMessage(LOCALID_TRAVELING_HEALER, 0);
  }
  Engine_ScriptEnd();
}

/// Feizhi runs up behind the party and goes ahead to look for Hsu. It happens
/// at the Alpine Crossing when the party steps onto the road by the boulders
/// once Feizhi has set out after Hsu: she finds the boulders blocking Silk
/// Road, says her vision came true, cries out for Hsu and runs ahead along the
/// path to look for him, the party leader following part of the way.
static void AlpineCrossing_FeizhiArrives(void) {
  struct ObjectEvent *leader;

  if (GameFlag_IsSet(FLAG_FEIZHI_WENT_TO_HSU)) {
    Engine_ScriptStart();
    ObjectEvent_SetPosition(LOCALID_FEIZHI, PIXELS(536), PIXELS(216));
    Engine_ScriptSetMessage(MsgMountainYahhSilkRoadBouldersBlock);
    Script_ShowMessageAndWait(LOCALID_FEIZHI, 0, 20);
    Engine_ObjectEventRunRepeatedMotion(LOCALID_PARTY_LEADER, 2);
    Engine_ScriptWait(20);

    // The leader turns to follow Feizhi as she runs past; one standing in
    // her row steps aside first.
    leader = ObjectEvent_GetById(LOCALID_PARTY_LEADER);
#ifdef UBFIX
    leader->update = Leader_FaceFeizhi;
#else
    leader->update = (void (*)(union FieldObject *))Leader_FaceFeizhi;
#endif
    leader = ObjectEvent_GetById(LOCALID_PARTY_LEADER);
    if ((leader->z.fixed >> 20) == 13) {
      ObjectEvent_WalkToAndWait(LOCALID_PARTY_LEADER, 440, 200);
    }
    ObjectEvent_SetSpeed(LOCALID_FEIZHI, 0x20000, 0x10000);
    Engine_ObjectEventSetSpritePriority(LOCALID_FEIZHI, 2);
    ObjectEvent_WalkToAndWait(LOCALID_FEIZHI, 408, 216);
    ObjectEvent_GetById(LOCALID_FEIZHI)->priorityFlags |=
        OBJ_EVENT_PRIORITY_AUTOMATIC;
    Engine_ScriptWait(10);
    ObjectEvent_FaceDirection(LOCALID_FEIZHI, FACING_WEST, 20);
    Script_ShowMessageAndWait(LOCALID_FEIZHI, 0, 20);
    Engine_ObjectEventStartRepeatedMotion(LOCALID_FEIZHI, 2);
    ObjectEvent_SetAttachedEffect(LOCALID_FEIZHI, ATTACHED_EFFECT_IN_FRONT |
                                                      ATTACHED_EFFECT_ONCE);
    Engine_ScriptWait(60);
    Script_ShowMessageAndWait(LOCALID_FEIZHI, 0, 20);

    // Feizhi runs down the path with the camera; the leader follows her
    // until the camera arrives.
    ObjectEvent_SetActionScript(LOCALID_FEIZHI, gAlpineCrossing_FeizhiPath);
    Camera_MoveTo(PIXELS(296), -1, PIXELS(344), 1);
    GameFlag_Set(FLAG_FEIZHI_SAW_ROCKSLIDE);
    ObjectEvent_RefreshSelectorById(LOCALID_FEIZHI);
    Engine_CameraWaitForMove();
    ObjectEvent_SetSpeed(LOCALID_PARTY_LEADER, 0x10000, 0x8000);
    ObjectEvent_SetActionScript(LOCALID_PARTY_LEADER,
                                gAlpineCrossing_LeaderPath);
    ObjectEvent_RefreshSelectorById(LOCALID_PARTY_LEADER);
    Engine_ScriptWait(10);
    leader = ObjectEvent_GetById(LOCALID_PARTY_LEADER);
    leader->update = NULL;
    Engine_ScriptWait(30);

    // She cannot hear Hsu's voice.
    Engine_ObjectEventRunRepeatedMotion(LOCALID_FEIZHI, 2);
    Engine_ScriptWait(20);
    ObjectEvent_FaceDirection(LOCALID_FEIZHI, FACING_SOUTH + FACING_STEP, 120);
    ObjectEvent_ShowEmote(LOCALID_FEIZHI, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 60);
    ObjectEvent_ShowEmote(LOCALID_PARTY_LEADER, EMOTE_IN_FRONT | EMOTE_QUESTION,
                          60);
    Engine_ObjectEventSetAnimationAndWait(LOCALID_FEIZHI, ANIMATION_SHAKE_HEAD);
    Engine_ScriptWait(20);
    Script_ShowMessageAndWait(LOCALID_FEIZHI, 0, 20);
    Engine_ScriptEnd();
  }
}

/// Talking to Feizhi at the Alpine Crossing before Hama's Reveal lesson: she
/// asks whether she should return to Xian, and either way decides to keep
/// looking for Hsu.
static void AlpineCrossing_TalkToFeizhi(void) {
  Engine_ScriptStart();
  Engine_ScriptSetMessage(MsgMountainDoDoWarriorShouldReturn);
  ObjectEvent_ShowEmote(LOCALID_FEIZHI, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 60);
  Script_OpenMessage(LOCALID_FEIZHI, 0);
  if (Engine_ScriptChooseYesNo(LOCALID_PARTY_LEADER, 0) == 1) {
    Script_SkipMessages(1);
  }
  Engine_ScriptWait(20);
  Engine_ObjectEventSetAnimationAndWait(LOCALID_FEIZHI, ANIMATION_SHAKE_HEAD);
  Engine_ScriptWait(20);
  Script_ShowMessageAndWait(LOCALID_FEIZHI, 0, 20);
  Engine_ScriptEnd();
}

/// Arriving at the Alpine Crossing from AltinPeakSouthEntrance by
/// ALPINE_CROSSING_FROM_ALTIN_PEAK_SOUTH_ENTRANCE_EAST while Hsu is trapped,
/// until this has run once (FLAG_HAMA_REACHED_HSU): the camera finds Hsu, Hama
/// asks if he is okay, and he is unhurt but cannot move; none of them knows how
/// to free him.
static void AlpineCrossing_HamaReachesHsu(void) {
  Engine_ScriptStart();
  // Reveal Hsu's position before Hama and Feizhi discuss freeing him.
  Engine_ScriptOpenScreen();
  Engine_ScriptWaitForScreen();
  Engine_ScriptWait(30);
  ObjectEvent_FaceDirection(LOCALID_PARTY_LEADER, FACING_SOUTHWEST, 0);
  Engine_CameraMoveToObjectEvent(LOCALID_HSU, 1);
  Engine_CameraWaitForMove();
  Engine_ObjectEventRunRepeatedMotion(LOCALID_HAMA, 2);
  Engine_ScriptWait(20);
  Engine_ScriptSetMessage(MsgMountainHsuOkay);
  Script_ShowMessageAndWait(LOCALID_HAMA, 0, 20);
  Engine_ObjectEventRunRepeatedMotion(LOCALID_HSU, 1);
  Engine_ScriptWait(20);
  Script_ShowMessageAndWait(LOCALID_HSU, 0, 20);
  Engine_ObjectEventSetAnimationAndWait(LOCALID_HAMA, ANIMATION_NOD);
  Engine_ScriptWait(20);
  Script_ShowMessageAndWait(LOCALID_HAMA, 0, 20);
  ObjectEvent_FaceDirection(LOCALID_FEIZHI, FACING_NORTH + FACING_STEP, 20);
  Script_ShowMessageAndWait(LOCALID_FEIZHI, 0, 20);
  Engine_ObjectEventSetAnimationAndWait(LOCALID_HAMA, ANIMATION_NOD);
  Engine_ScriptWait(60);
  ObjectEvent_ShowEmote(LOCALID_HAMA, EMOTE_IN_FRONT | EMOTE_WINCE, 60);
  Engine_ObjectEventSetAnimationAndWait(LOCALID_HAMA, ANIMATION_SHAKE_HEAD);
  Engine_ScriptWait(20);
  Script_ShowMessageAndWait(LOCALID_HAMA, 0, 20);
  Engine_ObjectEventStartRepeatedMotion(LOCALID_FEIZHI, 2);
  ObjectEvent_SetAttachedEffect(LOCALID_FEIZHI, ATTACHED_EFFECT_IN_FRONT |
                                                    ATTACHED_EFFECT_ONCE);
  Engine_ScriptWait(60);
  ObjectEvent_FaceDirection(LOCALID_FEIZHI, FACING_NORTHWEST + FACING_STEP, 20);
  Engine_ObjectEventSetAnimation(LOCALID_HSU, ANIMATION_HSU_STRUGGLE);
  Engine_ScriptEnd();
  GameFlag_Set(FLAG_HAMA_REACHED_HSU);
}

/// Lift on the boulder at the Alpine Crossing, until Hsu is freed: it floats,
/// and Hama and Feizhi pull him free in two heaves of two tugs each, asking the
/// leader in between to hold on. Hsu is free; they will ask Master Feh to tend
/// him and clear the road, and the scene fades out to Hama's farewell at Lama
/// Temple.
static void AlpineCrossing_LiftBoulder(void) {

  Engine_ScriptStart();
  ObjectEvent_SetAttachedEffect(LOCALID_HAMA, ATTACHED_EFFECT_IN_FRONT |
                                                  ATTACHED_EFFECT_ONCE);
  Engine_ObjectEventStartRepeatedMotion(LOCALID_HAMA, 2);
  Engine_ScriptWait(60);
  Engine_ScriptSetMessage(MsgMountainIsaacDidLiftBoulder);
  Script_ShowMessageAndWait(LOCALID_HAMA, 0, 20);
  ObjectEvent_SetAttachedEffect(LOCALID_FEIZHI, ATTACHED_EFFECT_IN_FRONT |
                                                    ATTACHED_EFFECT_ONCE);
  Engine_ObjectEventJump(LOCALID_FEIZHI, 4, 0);
  Engine_ScriptWait(60);
  Script_ShowMessageAndWait(LOCALID_FEIZHI, 0, 20);
  Engine_ObjectEventRunRepeatedMotion(LOCALID_HAMA, 1);
  Engine_ScriptWait(20);
  Script_ShowMessageAndWait(LOCALID_HAMA, 0, 20);
  ObjectEvent_FaceDirection(LOCALID_FEIZHI, FACING_NORTH + FACING_STEP, 20);
  Engine_ObjectEventSetAnimationAndWait(LOCALID_FEIZHI, ANIMATION_NOD);
  Engine_ScriptWait(20);

  // Hama and Feizhi take hold of Hsu.
  ObjectEvent_WalkTo(LOCALID_HAMA, 178, 276);
  ObjectEvent_WalkToAndWait(LOCALID_FEIZHI, 172, 284);
  Engine_ObjectEventWaitForMove(LOCALID_HAMA);
  ObjectEvent_FaceDirection(LOCALID_HAMA, FACING_SOUTH + FACING_STEP, 0);
  ObjectEvent_FaceDirection(LOCALID_FEIZHI, FACING_NORTHWEST + FACING_STEP, 0);
  Engine_ScriptWait(20);
  Engine_ObjectEventRunRepeatedMotion(LOCALID_HAMA, 2);
  Engine_ScriptWait(20);
  Script_ShowMessageAndWait(LOCALID_HAMA, 0, 20);
  ObjectEvent_GetById(LOCALID_HAMA)->steeringFlags &=
      (u8)~OBJ_EVENT_FACES_MOTION;
  ObjectEvent_GetById(LOCALID_FEIZHI)->steeringFlags &=
      (u8)~OBJ_EVENT_FACES_MOTION;
  ObjectEvent_SetSpeed(LOCALID_HAMA, 0x3333, 0x1999);
  ObjectEvent_SetSpeed(LOCALID_FEIZHI, 0x3333, 0x1999);

  // The first heave: two tugs draw Hsu halfway out.
  Engine_ObjectEventSetAnimation(LOCALID_HAMA, ANIMATION_HAMA_PULL);
  Engine_ObjectEventSetAnimation(LOCALID_FEIZHI, ANIMATION_FEIZHI_PULL);
  Engine_ScriptWait(20);
  Script_PlayCue(SE_125);
  ObjectEvent_SetDestinationOffset(LOCALID_HAMA, 2, 0);
  ObjectEvent_SetDestinationOffset(LOCALID_HSU, 2, 0);
  ObjectEvent_SetDestinationOffset(LOCALID_FEIZHI, 2, 0);
  Engine_ObjectEventWaitForMove(LOCALID_FEIZHI);
  Engine_ScriptWait(30);
  Engine_ObjectEventSetAnimation(LOCALID_HAMA, ANIMATION_HAMA_PULL);
  Engine_ObjectEventSetAnimation(LOCALID_FEIZHI, ANIMATION_FEIZHI_PULL);
  Engine_ScriptWait(20);
  Script_PlayCue(SE_125);
  ObjectEvent_SetDestinationOffset(LOCALID_HAMA, 4, 0);
  ObjectEvent_SetDestinationOffset(LOCALID_HSU, 4, 0);
  ObjectEvent_SetDestinationOffset(LOCALID_FEIZHI, 4, 0);
  Engine_ObjectEventWaitForMove(LOCALID_FEIZHI);
  Engine_ObjectEventStop(LOCALID_HSU);
  Engine_ObjectEventSetAnimation(LOCALID_HAMA, ANIMATION_STAND);
  Engine_ObjectEventSetAnimation(LOCALID_FEIZHI, ANIMATION_STAND);
  Engine_ScriptWait(50);
  Engine_ObjectEventJump(LOCALID_FEIZHI, 2, 0);
  Engine_ScriptWait(20);
  Script_ShowMessageAndWait(LOCALID_FEIZHI, 0, 20);

  // Hama asks the leader to hold the boulder a moment longer.
  Engine_ObjectEventSetAnimationAndWait(LOCALID_HAMA, ANIMATION_NOD);
  Engine_ScriptWait(20);
  Script_AskYesNo(LOCALID_HAMA, 0);
  Script_ShowMessageAndWait(LOCALID_HAMA, 0, 20);

  // The second heave pulls Hsu free.
  Engine_ObjectEventSetAnimation(LOCALID_HAMA, ANIMATION_HAMA_PULL);
  Engine_ObjectEventSetAnimation(LOCALID_FEIZHI, ANIMATION_FEIZHI_PULL);
  Engine_ScriptWait(20);
  Script_PlayCue(SE_125);
  ObjectEvent_SetDestinationOffset(LOCALID_HAMA, 2, 0);
  ObjectEvent_SetDestinationOffset(LOCALID_HSU, 2, 0);
  ObjectEvent_SetDestinationOffset(LOCALID_FEIZHI, 2, 0);
  Engine_ObjectEventWaitForMove(LOCALID_FEIZHI);
  Engine_ScriptWait(30);
  Engine_ObjectEventSetAnimation(LOCALID_HAMA, ANIMATION_HAMA_PULL);
  Engine_ObjectEventSetAnimation(LOCALID_FEIZHI, ANIMATION_FEIZHI_PULL);
  Engine_ScriptWait(20);
  Script_PlayCue(SE_125);
  ObjectEvent_SetDestinationOffset(LOCALID_HAMA, 4, 0);
  ObjectEvent_SetDestinationOffset(LOCALID_HSU, 4, 0);
  ObjectEvent_SetDestinationOffset(LOCALID_FEIZHI, 4, 0);
  Engine_ObjectEventWaitForMove(LOCALID_FEIZHI);
  Engine_ScriptWait(40);
  Engine_ObjectEventSetAnimation(LOCALID_HAMA, ANIMATION_STAND);
  Engine_ObjectEventSetAnimation(LOCALID_FEIZHI, ANIMATION_STAND);
  Engine_ObjectEventJump(LOCALID_FEIZHI, 2, 0);
  Engine_ScriptWait(20);
  ObjectEvent_FaceDirection(LOCALID_FEIZHI, FACING_NORTH + FACING_STEP, 20);
  Script_ShowMessageAndWait(LOCALID_FEIZHI, 0, 20);
  Engine_ObjectEventSetAnimationAndWait(LOCALID_HAMA, ANIMATION_NOD);
  Engine_ScriptWait(20);
  Script_ShowMessageAndWait(LOCALID_HAMA, 0, 30);
  Script_ShowMessageAndWait(LOCALID_HAMA, 0, 20);

  // Feizhi steps back; they agree to send for Master Feh.
  ObjectEvent_GetById(LOCALID_FEIZHI)->steeringFlags |= OBJ_EVENT_FACES_MOTION;
  ObjectEvent_SetSpeed(LOCALID_FEIZHI, 0xcccc, 0x6666);
  ObjectEvent_WalkToAndWait(LOCALID_FEIZHI, 168, 296);
  ObjectEvent_FaceDirection(LOCALID_FEIZHI, FACING_NORTH + FACING_STEP, 20);
  Engine_ObjectEventSetAnimation(LOCALID_FEIZHI, ANIMATION_FEIZHI_CROUCH);
  Engine_ObjectEventSetAnimationAndWait(LOCALID_HAMA, ANIMATION_NOD);
  Engine_ScriptWait(20);
  Script_ShowMessageAndWait(LOCALID_HAMA, 0, 20);
  Engine_ObjectEventSetAnimationAndWait(LOCALID_FEIZHI, ANIMATION_NOD);
  Engine_ScriptWait(20);
  Script_ShowMessageAndWait(LOCALID_FEIZHI, 0, 20);
  Engine_ObjectEventSetAnimationAndWait(LOCALID_HAMA, ANIMATION_NOD);
  Engine_ScriptWait(20);
  Script_ShowMessageAndWait(LOCALID_HAMA, 0, 20);
  Engine_ObjectEventSetAnimationAndWait(LOCALID_PARTY_LEADER, ANIMATION_NOD);
  Engine_ScriptWait(20);
  Engine_ScriptEnd();
  GameFlag_Set(FLAG_HSU_RESCUED);
  gEventWork->startTransition =
      MAP_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_SHAPE);
  Engine_ScriptRequestWarp(ALPINE_CROSSING_TO_LAMA_TEMPLE);
}

/// Searching the signpost at the Alpine Crossing.
/// "North: Altin Mine. West: Lama Temple."
static void AlpineCrossing_ReadSignpost(void) {
  Engine_ScriptStart();
  Engine_ObjectEventSetAnimation(LOCALID_PARTY_LEADER, ANIMATION_STAND);
  Engine_MessageShowCentered(MsgMountainNorthAltinMineWestLama, 1);
  Engine_ScriptEnd();
}

/// Reveal at the Alpine Crossing shows the path hidden at (5, 13): the revealed
/// cells, kept at (5, 28), are copied there with their attributes.
static void AlpineCrossing_RevealPath(void) {
  MapGrid_CopyCellsTo(5, 28, 5, 13, 1, 2);
  MapGrid_CopyCellAttributes(5, 28, 1, 2, 5, 13);
  Engine_ScriptWait(1);
}

/// When Reveal wears off at the Alpine Crossing, the plain cells kept at
/// (6, 28) cover the path again.
static void AlpineCrossing_HidePath(void) {
  MapGrid_CopyCellsTo(6, 28, 5, 13, 1, 2);
  MapGrid_CopyCellAttributes(6, 28, 1, 2, 5, 13);
  Engine_ScriptWait(1);
}

// Update callback of the frozen pillar: it draws under the party leader
// while the leader stands higher than its top, so the leader can climb onto
// it, and over the leader otherwise.
// UB: this update hook has no result. Plain void changes the native return
// sequence from pop {r1}; bx r1 to pop {r0}; bx r0 (tbs-en).
#ifdef UBFIX
static void Pillar_SortAgainstLeader(union FieldObject *fieldObject) {
#else
static s32 Pillar_SortAgainstLeader(union FieldObject *fieldObject) {
#endif
  struct ObjectEvent *pillar = (struct ObjectEvent *)fieldObject;
  if (ObjectEvent_GetById(LOCALID_PARTY_LEADER)->y.fixed > pillar->y.fixed) {
    pillar->priorityFlags |= OBJ_EVENT_PRIORITY_UNDERFOOT;
  } else {
    pillar->priorityFlags &= (u8)~OBJ_EVENT_PRIORITY_UNDERFOOT;
  }
}

/// Puts the rising pillar in front when the leader stands north of the puddle.
/// As Frost reaches the puddle at the Alpine Crossing, a leader standing north
/// of it sees the pillar rise in front of the background.
static void AlpineCrossing_PrepareFrost(void) {
  struct ObjectEvent *leader = ObjectEvent_GetById(LOCALID_PARTY_LEADER);
  struct ObjectEvent *puddle = ObjectEvent_GetById(LOCALID_WATER_FOR_FROST);

  if (leader->z.fixed <= puddle->z.fixed) {
    Engine_ObjectEventSetSpritePriority(LOCALID_WATER_FOR_FROST, 1);
  }
}

/// Frost at the Alpine Crossing freezes the puddle into a pillar the party can
/// climb: it can no longer be pushed or move, sorts itself against the
/// leader each frame, and the cells beside it (and their copy at (20, 18))
/// become walkable. Also run on entry while the pillar is already frozen.
static void AlpineCrossing_FreezePuddle(void) {
  Engine_ScriptStart();

  // Stop the pillar and install its height-dependent drawing priority.
  ObjectEvent_GetById(LOCALID_WATER_FOR_FROST)->priorityFlags &=
      (u8)~OBJ_EVENT_PRIORITY_UNDERFOOT;
  ObjectEvent_GetById(LOCALID_WATER_FOR_FROST)->collisionFlags &=
      (u8)~OBJ_EVENT_PUSHABLE;
  ObjectEvent_GetById(LOCALID_WATER_FOR_FROST)->motionFlags = 0;
#ifdef UBFIX
  ObjectEvent_GetById(LOCALID_WATER_FOR_FROST)->update =
      Pillar_SortAgainstLeader;
#else
  ObjectEvent_GetById(LOCALID_WATER_FOR_FROST)->update =
      (void (*)(union FieldObject *))Pillar_SortAgainstLeader;
#endif

  MapGrid_CopyCellAttributes(55, 16, 1, 1, 56, 18);
  MapGrid_CopyCellAttributes(55, 16, 1, 1, 20, 18);

  Engine_TaskWait(1);
  Engine_GameFlagSet(FLAG_PUDDLE_FROZEN);
  Engine_ObjectEventSetSpritePriority(LOCALID_WATER_FOR_FROST, 2);
  Engine_ScriptEnd();
}

/// Growth at the Alpine Crossing grows the sprout into a vine the party can
/// climb: the two cells above it take the climbable attributes kept at
/// (14, 6). Also run on entry while the vine is already grown.
static void AlpineCrossing_GrowSprout(void) {
  Engine_ScriptStart();
  MapGrid_CopyCellAttributes(14, 6, 1, 2, 21, 11);
  ObjectEvent_GetById(LOCALID_PLANT)->collisionFlags = (u8)~OBJ_EVENT_SOLID;
  Engine_GameFlagSet(FLAG_SPROUT_GROWN);
  Engine_ScriptEnd();
}

const struct MapEvent *Map_GetEvents(void) {
  if (gGameState.mapId == MAP_ALPINE_CROSSING) {
    return gAlpineCrossing_Events;
  }
  return gLamaTemple_Events;
}

/// Sets up whichever map is being entered and runs its arrival scripts.
s32 Map_Initialize(void) {
  s32 warpDest;

  gEventWork->startTransition = MAP_TRANSITION(TRANSITION_BACKDROP_FADE, 0);
  if (gGameState.mapId == MAP_LAMA_TEMPLE) {
    // Lama Temple: the temple's queued sound (SE_169) plays, two monks
    // meditate and the mossy rock stands; the rock's own cell attributes are
    // saved at (21, 73) for Reveal to restore.
    FieldEffect_SetQueuedSoundAndPlay(SE_169);
    Engine_ObjectEventSetAnimation(LOCALID_MEDITATING_MONK,
                                   ANIMATION_MONK_MEDITATE);
    Engine_ObjectEventSetAnimation(LOCALID_MONK_SENSING_ANXIETY,
                                   ANIMATION_MONK_MEDITATE);
    Engine_ObjectEventSetAnimation(LOCALID_PASSAGE_ROCK,
                                   TURNING_ROCK_ANIMATION_MOSSY);
    MapGrid_CopyCellAttributes(21, 9, 1, 1, 21, 73);
    LamaTemple_ShowFalseWall();
    if (Engine_GameFlagIsSet(FLAG_HSU_RESCUED)) {
      ObjectEvent_SetPosition(LOCALID_CHI_MONK, PIXELS(136), PIXELS(256));
      Engine_ObjectEventFaceDirection(LOCALID_CHI_MONK, FACING_EAST, 0);
    }
    warpDest = gGameState.warpDest;
    if (warpDest == LAMA_TEMPLE_FROM_FUCHIN_TEMPLE_INTERIORS) {
      Engine_GameFlagClear(FLAG_SHOW_LOCATION_NAME);
    } else if (warpDest == LAMA_TEMPLE_FROM_ALPINE_CROSSING &&
               !Engine_GameFlagIsSet(FLAG_KEEP_PARTY_POSITION)) {
      LamaTemple_RunHamasFarewell();
    }
  } else if (gGameState.mapId == MAP_ALPINE_CROSSING) {
    // Alpine Crossing: restore the Psynergy puzzles already solved.
    Engine_ObjectEventSetSpriteFlags(
        ObjectEvent_GetById(LOCALID_WATER_FOR_FROST), 0);
    ObjectEvent_GetById(LOCALID_WATER_FOR_FROST)->priorityFlags |=
        OBJ_EVENT_PRIORITY_UNDERFOOT;
    if (Engine_GameFlagIsSet(FLAG_PUDDLE_FROZEN)) {
      Engine_ObjectEventSetAnimation(LOCALID_WATER_FOR_FROST,
                                     PUDDLE_ANIMATION_FROZEN);
      AlpineCrossing_FreezePuddle();
    }
    if (Engine_GameFlagIsSet(FLAG_SPROUT_GROWN)) {
      Engine_ObjectEventSetAnimation(LOCALID_PLANT, SPROUT_ANIMATION_GROWN);
      AlpineCrossing_GrowSprout();
    }
    if (gGameState.warpDest ==
            ALPINE_CROSSING_FROM_ALTIN_PEAK_SOUTH_ENTRANCE_WEST ||
        gGameState.warpDest ==
            ALPINE_CROSSING_FROM_ALTIN_PEAK_SOUTH_ENTRANCE_EAST) {
      Engine_GameFlagClear(FLAG_SHOW_LOCATION_NAME);
    }

    // Feizhi is here only once she has set out after Hsu.
    if (!Engine_GameFlagIsSet(FLAG_FEIZHI_WENT_TO_HSU) &&
        !Engine_GameFlagIsSet(FLAG_XIAN_HEARD_HSU_TRAPPED) &&
        !Engine_GameFlagIsSet(FLAG_HSU_RESCUED)) {
      Engine_ObjectEventSetPosition(LOCALID_FEIZHI, 0, 0);
    }

    // Arriving from the north after Hama's lesson, before the party has freed
    // Hsu: he has been freed without it and the boulder is gone.
    if (!GameFlag_IsSet(FLAG_HSU_RESCUED) &&
        Engine_GameFlagIsSet(FLAG_XIAN_HEARD_HSU_TRAPPED) &&
        gGameState.warpDest == ALPINE_CROSSING_FROM_WORLD_MAP_NORTH) {
      Engine_ObjectEventSetPosition(LOCALID_BOULDER, 0, 0);
      Engine_GameFlagSet(FLAG_HSU_RESCUED);
      Engine_GameFlagSet(FLAG_HSU_RESCUED_WITHOUT_PARTY);
      Engine_ObjectEventSetPosition(LOCALID_FEIZHI, 0, 0);
    }

    // Once Hsu is free, the cleared rocks open the road.
    if (GameFlag_IsSet(FLAG_HSU_RESCUED)) {
      MapGrid_CopyCellsTo(54, 21, 53, 21, 1, 2);
      MapGrid_CopyCellAttributes(18, 20, 1, 3, 17, 21);
      MapGrid_CopyCellsTo(44, 18, 43, 17, 1, 1);
      MapGrid_CopyCellAttributes(8, 17, 1, 1, 7, 17);
    }

    // After Hama's lesson, while Hsu is still trapped, Hama and Feizhi stand
    // by him, and the workers from Xian have not come yet.
    if (Engine_GameFlagIsSet(FLAG_XIAN_HEARD_HSU_TRAPPED) &&
        !Engine_GameFlagIsSet(FLAG_HSU_RESCUED)) {
      Engine_ObjectEventSetPosition(LOCALID_FIRST_WORKER, 0, 0);
      Engine_ObjectEventSetPosition(LOCALID_SECOND_WORKER, 0, 0);
      ObjectEvent_SetPosition(LOCALID_HAMA, PIXELS(192), PIXELS(264));
      ObjectEvent_SetPosition(LOCALID_HSU, PIXELS(164), PIXELS(280));
      ObjectEvent_SetPosition(LOCALID_FEIZHI, PIXELS(184), PIXELS(304));
      ObjectEvent_FaceDirection(LOCALID_HAMA, FACING_SOUTH + FACING_STEP, 0);
      ObjectEvent_FaceDirection(LOCALID_FEIZHI, FACING_NORTHWEST + FACING_STEP,
                                0);
      ObjectEvent_SetActionScript(LOCALID_HSU, gAlpineCrossing_HsuStruggle);
      ObjectEvent_GetById(LOCALID_HSU)->scaleX = -0x10000;
    }
    if (!GameFlag_IsSet(FLAG_HSU_RESCUED)) {
      ObjectEvent_SetPosition(LOCALID_HSU, PIXELS(164), PIXELS(280));
      ObjectEvent_SetActionScript(LOCALID_HSU, gAlpineCrossing_HsuStruggle);
      ObjectEvent_GetById(LOCALID_HSU)->scaleX = -0x10000;
    }
    if (gGameState.warpDest ==
            ALPINE_CROSSING_FROM_ALTIN_PEAK_SOUTH_ENTRANCE_EAST &&
        !Engine_GameFlagIsSet(FLAG_HAMA_REACHED_HSU) &&
        !Engine_GameFlagIsSet(FLAG_KEEP_PARTY_POSITION) &&
        !Engine_GameFlagIsSet(FLAG_HSU_RESCUED)) {
      AlpineCrossing_HamaReachesHsu();
    }
  }
  return 0;
}

/// The first worker's object event template script: draw him behind the
/// background.
s32 gAlpineCrossing_WorkerObjectEventTemplateScript[] = {
    SC_CALL(ObjectEvent_DrawBehindScenery),
    SC_END,
};

/// Feizhi's run down the path toward Hsu, walking, then standing.
s32 gAlpineCrossing_FeizhiPath[] = {
    SC_ANIMATION(ANIMATION_WALK),
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
    SC_ANIMATION(ANIMATION_STAND),
    SC_END,
};

/// The party leader follows Feizhi most of the way.
s32 gAlpineCrossing_LeaderPath[] = {
    SC_ANIMATION(ANIMATION_WALK),
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
    SC_ANIMATION(ANIMATION_STAND),
    SC_END,
};

/// Hsu, pinned under the boulder, struggles now and then.
s32 gAlpineCrossing_HsuStruggle[] = {
    SC_ANIMATION(ANIMATION_STAND),
    SC_ANIMATION(ANIMATION_HSU_STRUGGLE),
    SC_WAIT(240),
    SC_ANIMATION(ANIMATION_STAND),
    SC_ANIMATION(ANIMATION_HSU_STRUGGLE),
    SC_WAIT(60),
    SC_ANIMATION(ANIMATION_STAND),
    SC_ANIMATION(ANIMATION_HSU_STRUGGLE),
    SC_WAIT(300),
    SC_LOOP(SC_FOREVER, SC_TOP),
    SC_END,
};

/// The door of Hama's room sliding open, and shut again.
u16 gLamaTemple_DoorOpenCells[] = {
    MAPGRID_ANIMATION_STEP(66, 46, 3, 2, 5),
    MAPGRID_ANIMATION_STEP(63, 46, 3, 2, 5),
    MAPGRID_ANIMATION_STEPS_END,
};

u16 gLamaTemple_DoorCloseCells[] = {
    MAPGRID_ANIMATION_STEP(66, 46, 3, 2, 5),
    MAPGRID_ANIMATION_STEP(69, 46, 3, 2, 5),
    MAPGRID_ANIMATION_STEPS_END,
};

struct MapWarpDest gLamaTemple_WarpDests[] = {
    {WARP_DEST_DEFAULT, CONDITION_ALWAYS, 496, 0, 216, FACING_WEST, 0, -1, -1,
     -1, -1, 0},
    {LAMA_TEMPLE_FROM_WORLD_MAP, CONDITION_ALWAYS, 216, 0, 520, FACING_NORTH, 0,
     -1, -1, -1, -1, 0},
    {LAMA_TEMPLE_FROM_FUCHIN_TEMPLE_INTERIORS, CONDITION_ALWAYS, 136, 0, 168,
     FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {LAMA_TEMPLE_FROM_ALPINE_CROSSING, CONDITION_ALWAYS, 144, 0, 200,
     FACING_NORTH, 0, -1, -1, -1, -1, 0},
    {LAMA_TEMPLE_AFTER_DEFEAT, CONDITION_ALWAYS, 232, 0, 352, FACING_SOUTH, 0,
     -1, -1, -1, -1, 0},
    {MAP_TABLE_END},
};

struct MapWarpDest gAlpineCrossing_WarpDests[] = {
    {WARP_DEST_DEFAULT, CONDITION_ALWAYS, 496, 0, 216, FACING_WEST, 0, -1, -1,
     -1, -1, 0},
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
    {MAP_TABLE_END},
};

u32 gAlpineCrossingLamaTemple_Warps[] = {
    MAP_WARPS(LamaTemple),
    MAP_WARP(LAMA_TEMPLE_TO_WORLD_MAP, WorldMap, WORLD_MAP_FROM_LAMA_TEMPLE),
    MAP_WARP(LAMA_TEMPLE_TO_FUCHIN_TEMPLE_INTERIORS, FuchinTempleInteriors,
             FUCHIN_TEMPLE_INTERIORS_FROM_LAMA_TEMPLE),
    MAP_WARPS(AlpineCrossing),
    MAP_WARP(ALPINE_CROSSING_TO_WORLD_MAP_EAST, WorldMap,
             WORLD_MAP_FROM_ALPINE_CROSSING_EAST),
    MAP_WARP(ALPINE_CROSSING_TO_WORLD_MAP_NORTH, WorldMap,
             WORLD_MAP_FROM_ALPINE_CROSSING_NORTH),
    MAP_WARP(ALPINE_CROSSING_TO_WORLD_MAP_WEST, WorldMap,
             WORLD_MAP_FROM_ALPINE_CROSSING_WEST),
    MAP_WARP(ALPINE_CROSSING_TO_ALTIN_PEAK_SOUTH_ENTRANCE_WEST,
             AltinPeakSouthEntrance,
             ALTIN_PEAK_SOUTH_ENTRANCE_FROM_ALPINE_CROSSING_WEST),
    MAP_WARP(ALPINE_CROSSING_TO_ALTIN_PEAK_SOUTH_ENTRANCE_EAST,
             AltinPeakSouthEntrance,
             ALTIN_PEAK_SOUTH_ENTRANCE_FROM_ALPINE_CROSSING_EAST),
    MAP_WARP(ALPINE_CROSSING_TO_LAMA_TEMPLE, LamaTemple,
             LAMA_TEMPLE_FROM_ALPINE_CROSSING),
    MAP_WARPS_END,
};

// clang-format off
struct ObjectEventTemplate gLamaTemple_ObjectEventTemplates[] = {
    { GARET, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        0, 0, 0, FACING_NORTH, TALK_KEEP_FACING, 0 },
    { IVAN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        0, 0, 0, FACING_NORTH, TALK_KEEP_FACING, 0 },
    { MIA, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        0, 0, 0, FACING_NORTH, TALK_KEEP_FACING, 0 },
    { SPRITE_HAMA, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        0, 0, 0, FACING_NORTH, TALK_FACE_PARTY_AND_BACK, 0 },
    { SPRITE_MONK_IN_ORANGE_ROBE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(168), 0, PIXELS(336), FACING_EAST, TALK_FACE_PARTY, 0 },
    { SPRITE_MONK_IN_ORANGE_ROBE, CONDITION_ALWAYS, MOVEMENT_TYPE_WANDER,
        PIXELS(216), 0, PIXELS(240), FACING_SOUTH, TALK_FACE_PARTY, 0 },
    { SPRITE_MONK_IN_ORANGE_ROBE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(312), 0, PIXELS(320), FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { SPRITE_MONK_IN_ORANGE_ROBE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(344), 0, PIXELS(320), FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { SPRITE_MONK_IN_ORANGE_ROBE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(120), 0, PIXELS(176), FACING_SOUTH, TALK_FACE_PARTY_AND_BACK, 0 },
    { SPRITE_TURNING_ROCK, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(344), 0, PIXELS(152), FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { SPRITE_MAN_IN_TALL_HAT, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(232), 0, PIXELS(328), FACING_SOUTH, TALK_FACE_PARTY_AND_BACK, 0 },
    { MAP_TABLE_END },
};

struct ObjectEventTemplate gAlpineCrossing_ObjectEventTemplates[] = {
    { SPRITE_HAMA, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        0, 0, 0, FACING_NORTH, TALK_FACE_PARTY_AND_BACK, 0 },
    { SPRITE_HSU, CONDITION_ALWAYS, (s32)gAlpineCrossing_HsuStruggle,
        0, 0, 0, FACING_NORTH, TALK_KEEP_FACING, 0 },
    { SPRITE_FEIZHI, FLAG_HSU_RESCUED, MOVEMENT_TYPE_STAND,
        PIXELS(296), 0, PIXELS(344), FACING_WEST, TALK_FACE_PARTY_AND_BACK, 0 },
    { SPRITE_ROUND_BOULDER, FLAG_HSU_RESCUED, MOVEMENT_TYPE_STAND,
        PIXELS(152), 0, PIXELS(276), FACING_WEST, TALK_FACE_PARTY, 0 },
    { SPRITE_YOUTH_WITH_HEADBAND, IF_FLAG_SET(FLAG_XIAN_HEARD_HSU_TRAPPED), (s32)gAlpineCrossing_WorkerObjectEventTemplateScript,
        PIXELS(456), 0, PIXELS(200), FACING_SOUTH, TALK_FACE_PARTY, 0 },
    { SPRITE_YOUTH_WITH_TIED_HAIR, IF_FLAG_SET(FLAG_XIAN_HEARD_HSU_TRAPPED), MOVEMENT_TYPE_STAND,
        PIXELS(184), 0, PIXELS(264), FACING_SOUTH, TALK_FACE_PARTY, 0 },
    { SPRITE_PUDDLE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(328), 0, PIXELS(296), FACING_SOUTH, TALK_FACE_PARTY, 0 },
    { SPRITE_SPROUT, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(344), 0, PIXELS(194), FACING_SOUTH, TALK_FACE_PARTY, 0 },
    { MAP_TABLE_END },
};

struct MapEvent gLamaTemple_Events[] = {
    { MAP_EVENT_WARP, 1, CONDITION_ALWAYS, LAMA_TEMPLE_TO_WORLD_MAP },
    { COORD_EVENT_WALKING_INTO(FACING_NORTH), TRIGGER_TEMPLE_DOOR, CONDITION_ALWAYS, EVENT_SCRIPT(LamaTemple_EnterHamasRoom) },
    { MAP_EVENT_TALK, LOCALID_GREETING_MONK, FLAG_HSU_RESCUED, MSGNO(MsgAlpineCrossingLamaTempleThisIsLamaTempleWelcomeTravelers) },
    { MAP_EVENT_TALK, LOCALID_MONK_ASKING_ABOUT_THE_EAST, FLAG_HSU_RESCUED, EVENT_SCRIPT(LamaTemple_TalkToMonkAskingAboutTheEast) },
    { MAP_EVENT_TALK, LOCALID_MEDITATING_MONK, FLAG_HSU_RESCUED, EVENT_SCRIPT(LamaTemple_TalkToMeditatingMonk) },
    { MAP_EVENT_TALK, LOCALID_MONK_SENSING_ANXIETY, FLAG_HSU_RESCUED, MSGNO(MsgAlpineCrossingLamaTempleYouWhoSpeakToMeI) },
    { MAP_EVENT_TALK, LOCALID_CHI_MONK, FLAG_HSU_RESCUED, EVENT_SCRIPT(LamaTemple_TalkToChiMonk) },
    { MAP_EVENT_TALK, LOCALID_GREETING_MONK, CONDITION_ALWAYS, MSGNO(MsgAlpineCrossingLamaTempleItIsVeryDangerousToCross) },
    { MAP_EVENT_TALK, LOCALID_MONK_ASKING_ABOUT_THE_EAST, CONDITION_ALWAYS, MSGNO(MsgAlpineCrossingLamaTempleTheEvilDesertIsHotterThan) },
    { MAP_EVENT_TALK, LOCALID_MEDITATING_MONK, CONDITION_ALWAYS, MSGNO(MsgAlpineCrossingLamaTempleIBelieveItIsPossibleTo) },
    { MAP_EVENT_TALK, LOCALID_MONK_SENSING_ANXIETY, CONDITION_ALWAYS, MSGNO(MsgAlpineCrossingLamaTempleIWillBeMostThankfulOnce) },
    { MAP_EVENT_TALK, LOCALID_CHI_MONK, CONDITION_ALWAYS, MSGNO(MsgAlpineCrossingLamaTempleWhenYourTemperatureReachesItsMaximum) },
    { MAP_EVENT_TALK, LOCALID_TRAVELING_HEALER, CONDITION_ALWAYS, EVENT_SCRIPT(LamaTemple_TalkToHealer) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_GREETING_MONK, FLAG_HSU_RESCUED, MSGNO(MsgAlpineCrossingLamaTempleTheySeemToBeAnxiousThey) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_MONK_ASKING_ABOUT_THE_EAST, FLAG_HSU_RESCUED, MSGNO(MsgAlpineCrossingLamaTempleTheySayAPersonCanOvercome) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_MEDITATING_MONK, FLAG_HSU_RESCUED, MSGNO(MsgAlpineCrossingLamaTempleIAmHungry) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_MONK_SENSING_ANXIETY, FLAG_HSU_RESCUED, MSGNO(MsgAlpineCrossingLamaTempleISenseImpureThoughtsFromThe) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_CHI_MONK, FLAG_HSU_RESCUED, MSGNO(MsgAlpineCrossingLamaTempleChiIsButAFragmentOf) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_GREETING_MONK, CONDITION_ALWAYS, MSGNO(MsgAlpineCrossingLamaTempleICannotCrossTheDesertIt) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_MONK_ASKING_ABOUT_THE_EAST, CONDITION_ALWAYS, MSGNO(MsgAlpineCrossingLamaTempleTheEvilInTheDesertDistorts) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_MEDITATING_MONK, CONDITION_ALWAYS, MSGNO(MsgAlpineCrossingLamaTempleHowNiceItWouldBeIf) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_MONK_SENSING_ANXIETY, CONDITION_ALWAYS, MSGNO(MsgAlpineCrossingLamaTempleMasterHamaWillBeVerySad) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_CHI_MONK, CONDITION_ALWAYS, MSGNO(MsgAlpineCrossingLamaTempleWithRevealYouWillFindOases) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_TRAVELING_HEALER, CONDITION_ALWAYS, MSGNO(MsgAlpineCrossingLamaTempleForSomeReasonThesePeopleGive) },
    { PSYNERGY_IN_AREA(ABILITY_REVEAL, PSYNERGY_PHASE_EFFECT_ACTS), 0, CONDITION_ALWAYS, EVENT_SCRIPT(LamaTemple_RevealPassage) },
    { PSYNERGY_IN_AREA(ABILITY_REVEAL, PSYNERGY_PHASE_EFFECT_ENDS), 0, CONDITION_ALWAYS, EVENT_SCRIPT(LamaTemple_ShowFalseWall) },
    { BG_EVENT(BG_EVENT_UNNAMED), TRIGGER_TEMPLE_WALL_WRITING, CONDITION_ALWAYS, EVENT_SCRIPT(LamaTemple_ReadWallWriting) },
    { BG_EVENT_FACING(FACING_NORTH), TRIGGER_STONE, FLAG_LAMA_TEMPLE_PSYNERGY_STONE, FIND_PSYNERGY_STONE },
    { BG_EVENT_FACING(FACING_NORTHEAST), TRIGGER_STONE, FLAG_LAMA_TEMPLE_PSYNERGY_STONE, FIND_PSYNERGY_STONE },
    { BG_EVENT_FACING(FACING_NORTHWEST), TRIGGER_STONE, FLAG_LAMA_TEMPLE_PSYNERGY_STONE, FIND_PSYNERGY_STONE },
    { BG_EVENT_FACING(FACING_WEST), TRIGGER_STONE, FLAG_LAMA_TEMPLE_PSYNERGY_STONE, FIND_PSYNERGY_STONE },
    { BG_EVENT(BG_EVENT_CHEST), TRIGGER_CHEST, FLAG_LAMA_TEMPLE_WATER_OF_LIFE, FIND_ITEM(ITEM_WATER_OF_LIFE) },
    { MAP_EVENTS_END },
};

struct MapEvent gAlpineCrossing_Events[] = {
    { MAP_EVENT_WARP, 1, CONDITION_ALWAYS, ALPINE_CROSSING_TO_WORLD_MAP_EAST },
    { MAP_EVENT_WARP, 2, CONDITION_ALWAYS, ALPINE_CROSSING_TO_WORLD_MAP_NORTH },
    { MAP_EVENT_WARP, 3, CONDITION_ALWAYS, ALPINE_CROSSING_TO_WORLD_MAP_WEST },
    { MAP_EVENT_WARP, 5, CONDITION_ALWAYS, ALPINE_CROSSING_TO_ALTIN_PEAK_SOUTH_ENTRANCE_WEST },
    { MAP_EVENT_WARP, 4, CONDITION_ALWAYS, ALPINE_CROSSING_TO_ALTIN_PEAK_SOUTH_ENTRANCE_EAST },
    { MAP_EVENT_COORD, TRIGGER_FEIZHI_ARRIVAL, FLAG_FEIZHI_SAW_ROCKSLIDE, EVENT_SCRIPT(AlpineCrossing_FeizhiArrives) },
    { MAP_EVENT_TALK, LOCALID_FEIZHI, FLAG_XIAN_HEARD_HSU_TRAPPED, EVENT_SCRIPT(AlpineCrossing_TalkToFeizhi) },
    { MAP_EVENT_TALK, LOCALID_HAMA, CONDITION_ALWAYS, MSGNO(MsgAlpineCrossingLamaTempleWellIsaacAdeptsYouCameTo) },
    { MAP_EVENT_TALK, LOCALID_HSU, CONDITION_ALWAYS, MSGNO(MsgAlpineCrossingLamaTempleIAmTrappedBetweenTheseTwo) },
    { MAP_EVENT_TALK, LOCALID_FEIZHI, CONDITION_ALWAYS, MSGNO(MsgAlpineCrossingLamaTempleBeStrongHsuWeWillHelp) },
    { MAP_EVENT_TALK, LOCALID_FIRST_WORKER, CONDITION_ALWAYS, MSGNO(MsgAlpineCrossingLamaTempleWeMovedTheRocksNowPeople) },
    { MAP_EVENT_TALK, LOCALID_SECOND_WORKER, CONDITION_ALWAYS, MSGNO(MsgAlpineCrossingLamaTempleManyPeopleFromXianHelpedMove) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_LIFT), LOCALID_BOULDER, FLAG_HSU_RESCUED, EVENT_SCRIPT(AlpineCrossing_LiftBoulder) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_FEIZHI, FLAG_XIAN_HEARD_HSU_TRAPPED, MSGNO(MsgAlpineCrossingLamaTempleHsuIsInDangerICan) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_HAMA, CONDITION_ALWAYS, MSGNO(MsgAlpineCrossingLamaTempleHowShouldWeMoveTheBoulders) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_HSU, CONDITION_ALWAYS, MSGNO(MsgAlpineCrossingLamaTempleIWantToScreamButFeizhi) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_FEIZHI, CONDITION_ALWAYS, MSGNO(MsgAlpineCrossingLamaTempleHsuTiresHeLooksExhaustedI) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_FIRST_WORKER, CONDITION_ALWAYS, MSGNO(MsgAlpineCrossingLamaTempleWeClearedTheWayButWhat) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_SECOND_WORKER, CONDITION_ALWAYS, MSGNO(MsgAlpineCrossingLamaTempleIAlmostHurtMyBackIt) },
    { PSYNERGY_IN_AREA(ABILITY_REVEAL, PSYNERGY_PHASE_EFFECT_ACTS), 0, CONDITION_ALWAYS, EVENT_SCRIPT(AlpineCrossing_RevealPath) },
    { PSYNERGY_IN_AREA(ABILITY_REVEAL, PSYNERGY_PHASE_EFFECT_ENDS), 0, CONDITION_ALWAYS, EVENT_SCRIPT(AlpineCrossing_HidePath) },
    // Frost's first phase, as it reaches the puddle
    { PSYNERGY_PHASE_ON_OBJ_EVENT(ABILITY_FROST, PSYNERGY_PHASE_BEFORE_CAST), LOCALID_WATER_FOR_FROST, FLAG_PUDDLE_FROZEN, EVENT_SCRIPT(AlpineCrossing_PrepareFrost) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_FROST), LOCALID_WATER_FOR_FROST, FLAG_PUDDLE_FROZEN, EVENT_SCRIPT(AlpineCrossing_FreezePuddle) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_GROWTH), LOCALID_PLANT, FLAG_SPROUT_GROWN, EVENT_SCRIPT(AlpineCrossing_GrowSprout) },
    { BG_EVENT(BG_EVENT_UNNAMED), TRIGGER_SIGNPOST, CONDITION_ALWAYS, EVENT_SCRIPT(AlpineCrossing_ReadSignpost) },
    { BG_EVENT(BG_EVENT_CHEST), TRIGGER_CHEST, FLAG_ALPINE_CROSSING_POWER_BREAD, FIND_ITEM(ITEM_POWER_BREAD) },
    { MAP_EVENTS_END },
};
// clang-format on
