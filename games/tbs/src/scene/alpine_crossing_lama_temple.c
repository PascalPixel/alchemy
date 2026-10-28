/// @file
/// The Alpine Crossing (SceneId_AlpineCrossing) and Lama Temple
/// (SceneId_LamaTemple), two scenes on Silk Road west of Xian that share this
/// overlay. The getters pick each scene's tables by gAdventure.sceneId,
/// both scenes share one exit table, and Scene_Initialize sets up the scene
/// being entered from the story flags of Hsu's rescue, which runs through both.
///
/// At the Alpine Crossing boulders block Silk Road. Once Feizhi has left Xian
/// (FLAG_FEIZHI_WENT_TO_HSU, set in fuchin_temple.c), stepping onto the road
/// beside them (CELL_BEHAVIOR_ALPINE_CROSSING_FEIZHI_ARRIVAL) plays
/// AlpineCrossing_FeizhiArrives and sets FLAG_FEIZHI_SAW_ROCKSLIDE; she then
/// waits at the end of the path until Hama's lesson. A signpost reads `North:
/// Altin Mine West: Lama Temple`; the Crossing opens onto the World Map to the
/// east, north and west, and by
/// ALPINE_CROSSING_TO_ALTIN_PEAK_SOUTH_ENTRANCE_WEST and
/// ALPINE_CROSSING_TO_ALTIN_PEAK_SOUTH_ENTRANCE_EAST into Altin Peak's
/// AltinPeakSouthEntrance, whose entrances from here the international
/// editions also call Alpine Crossing. Coming back from the mine
/// (ALPINE_CROSSING_FROM_ALTIN_PEAK_SOUTH_ENTRANCE_WEST and _EAST) the area
/// name is not shown again.
///
/// In Lama Temple the five monks have one set of lines and thoughts until Hsu
/// is freed (FLAG_HSU_RESCUED) and another after, and a traveling healer serves
/// across his counter; a defeat anywhere here returns the party to
/// LAMA_TEMPLE_AFTER_DEFEAT, in front of him. Walking north into the sliding
/// door (LamaTemple_EnterHamasRoom) leads into Hama's room, in
/// LacquerRoomsA (LACQUER_ROOMS_A_HAMAS_ROOM_FROM_LAMA_TEMPLE,
/// in fuchin_temple.c), whose lesson sets FLAG_XIAN_HEARD_HSU_TRAPPED; coming
/// back out (LAMA_TEMPLE_FROM_LACQUER_ROOMS_A_HAMAS_ROOM) the area name
/// is not shown again. Writing on a wall hints at Reveal, which shows a passage
/// behind a mossy rock (LamaTemple_RevealPassage); LamaTemple_ShowFalseWall
/// puts the rock back when Reveal wears off and on entry, the rock cell's own
/// attributes saved at (21, 73). The temple also holds a Psynergy Stone
/// (FLAG_LAMA_TEMPLE_PSYNERGY_STONE, an area temp flag) and a chest with
/// Water of Life.
///
/// Back at the Alpine Crossing after the lesson, until Hsu is freed, Hama and
/// Feizhi stand by him, pinned between two boulders. Arriving beside him from
/// AltinPeakSouthEntrance (ALPINE_CROSSING_FROM_ALTIN_PEAK_SOUTH_ENTRANCE_EAST)
/// the first time plays AlpineCrossing_HamaReachesHsu
/// (FLAG_HAMA_REACHED_HSU). Lift on the boulder (AlpineCrossing_LiftBoulder)
/// raises it while Hama and Feizhi pull him free; the event sets
/// FLAG_HSU_RESCUED and leaves for Lama Temple (ALPINE_CROSSING_TO_LAMA_TEMPLE,
/// to LAMA_TEMPLE_FROM_ALPINE_CROSSING), where Scene_Initialize plays Hama's
/// farewell (LamaTemple_RunHamasFarewell). Coming back instead by the World
/// Map's north side (ALPINE_CROSSING_FROM_WORLD_MAP_NORTH), Hsu has been freed
/// without the party (FLAG_HSU_RESCUED_WITHOUT_PARTY, which lamakan_desert.c
/// can set too), and Hama and Feizhi are gone. Once he is free, two workers
/// from Xian stand by the road they have cleared, and Scene_Initialize copies
/// the cleared cells over the rocks. The Crossing's other puzzles are kept
/// only within one visit (scene temp flags): Reveal shows a hidden path, Frost
/// freezes a puddle into a pillar the party can climb, and Growth grows a
/// sprout into a vine; a chest holds Power Bread.

#include "script_operands.h"
#include "adventure.h"
#include "module/scene/alpine_crossing_lama_temple.h"
#include "module/overworld/overworld.h"
#include "module/overworld/event_commands.h"
#include "module/runtime/runtime.h"
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
#include "event.h"
#include "scene_tables.h"
#include "scene_ids.h"
#include "call.h"
#include "text/msg_ids.h"
#include "ability_ids.h"
#include "flags.h"
#include "item_ids.h"
#include "actor_script.h"
#include "field_effect.h"
#include "sounds.h"

/// The temple's Psynergy Stone, an area temp flag: it refills once the
/// party has been to another area.
/// ★ Named from the game: "a Psynergy Stone" (MsgGotPsynergyStone)
#define FLAG_LAMA_TEMPLE_PSYNERGY_STONE FLAG_AREA_TEMP_50

#define MAP_ALPINE_CROSSING ((s32)&SceneId_AlpineCrossing)
#define MAP_LAMA_TEMPLE ((s32)&SceneId_LamaTemple)

/// Lama Temple's actors. Garet, Ivan and Mia are placed in their own
/// party slots for Hama's farewell; the others count from ACTOR_FIRST_PLACED.
enum LamaTempleActorId {
  /// Hama, back in her temple for her farewell.
  /// ★ Named from the game: "No, Master Hama"
  /// (MsgMountainAdeptsLetMeThankAgain)
  ACTOR_TEMPLE_HAMA = ACTOR_FIRST_PLACED,
  /// ★ Named from the game: "This is Lama Temple. Welcome, travelers."
  /// (MsgAlpineCrossingLamaTempleThisIsLamaTempleWelcomeTravelers)
  ACTOR_GREETING_MONK,
  /// ★ Named from the game: "Young warriors, do you come from the east?"
  /// (MsgMountainYoungWarriorsDoComeFrom)
  ACTOR_MONK_ASKING_ABOUT_THE_EAST,
  /// ★ Named from the game: "Do you know what meditation is?"
  /// (MsgMountainDoKnowMeditation)
  ACTOR_MEDITATING_MONK,
  /// ★ Named from the game: "I sense great insecurity and anxiety within you."
  /// (MsgAlpineCrossingLamaTempleYouWhoSpeakToMeI)
  ACTOR_MONK_SENSING_ANXIETY,
  /// ★ Named from the game: "Did you know that Master Hama is the greatest
  /// master of Chi?" (MsgMountainDidKnowMasterHamaGreatest)
  ACTOR_CHI_MONK,
  /// The rock Reveal takes away to show the passage (LamaTemple_RevealPassage).
  /// ★ Named from what the player sees: a round, moss-green rock on the grass
  /// east of the waterfall (its render)
  ACTOR_PASSAGE_ROCK,
  /// ★ Named from the game: "I am traveling around the world to spread good
  /// will." (MsgMountainAmTravelingAroundWorldSpread)
  ACTOR_TRAVELING_HEALER
};

/// Alpine Crossing's actors.
enum AlpineCrossingActorId {
  /// ★ Named from the game: "Master Hama, I am okay." (MsgMountainHsuOkay)
  ACTOR_HAMA = ACTOR_FIRST_PLACED,
  /// ★ Named from the game: "Hsu, are you okay?" (MsgMountainHsuOkay)
  ACTOR_HSU,
  /// ★ Named from the game: "You must be glad, Feizhi." (MsgMountainHsuOkay)
  ACTOR_FEIZHI,
  /// ★ Named from the game: "did you lift the boulder?"
  /// (MsgMountainIsaacDidLiftBoulder)
  ACTOR_BOULDER,
  /// A young man in a white headband, one of the people of Xian who moved the
  /// rocks.
  /// ★ Named from the game: "We moved the rocks. Now people can pass."
  /// (MsgAlpineCrossingLamaTempleWeMovedTheRocksNowPeople)
  ACTOR_FIRST_WORKER,
  /// A young man with his hair tied back.
  /// ★ Named from the game: "Many people from Xian helped move the rocks."
  /// (MsgAlpineCrossingLamaTempleManyPeopleFromXianHelpedMove)
  ACTOR_SECOND_WORKER,
  /// The water Frost freezes into a pillar (AlpineCrossing_FreezePuddle).
  /// ★ Named from what the player sees: a small patch of blue water on the
  /// grass by the path (its render)
  ACTOR_WATER_FOR_FROST,
  /// The plant Growth grows into a vine (AlpineCrossing_GrowSprout).
  /// ★ Named from what the player sees: a small green sprout at the foot of
  /// the cliff (its render)
  ACTOR_PLANT
};

/// Alpine Crossing's Psynergy puzzles, kept while the party stays.
enum {
  /// Frost has frozen the puddle into an ice pillar.
  /// ★ Named from what the player sees: a small puddle of water on the path
  /// below the cliffs (its render)
  FLAG_ALPINE_CROSSING_PUDDLE_FROZEN = FLAG_SCENE_TEMP_00,
  /// Growth has grown the sprout into a vine the party can climb.
  /// ★ Named from what the player sees: a small sprout at the foot of a cliff
  /// (its render)
  FLAG_ALPINE_CROSSING_SPROUT_GROWN = FLAG_SCENE_TEMP_01
};

/// Animations of these scenes' own sprites beyond enum ActorAnimation,
/// named by when the events play them.
enum {
  ANIM_HAMA_PULL = 5,
  ANIM_FEIZHI_PULL = 6,
  /// Drops into a crouch, then holds a half crouch with her head bowed
  /// (spr_039.png's frames 33 and 36).
  ANIM_FEIZHI_CROUCH = 5,
  /// SPRITE_TURNING_ROCK's mossy rock (spr_293.png's frame 1); its first
  /// animations show picture 0, a purple glow.
  TURNING_ROCK_ANIMATION_MOSSY = 2,
  ANIM_HSU_STRUGGLE = 5,
  ANIM_MONK_MEDITATE = 5,
  PUDDLE_ANIMATION_FROZEN = 5,
  SPROUT_ANIMATION_GROWN = 4
};

/// The cell behaviors Lama Temple's triggers answer.
enum LamaTempleCellBehavior {
  CELL_BEHAVIOR_LAMA_TEMPLE_DOOR = 2,
  CELL_BEHAVIOR_LAMA_TEMPLE_WALL_WRITING = 5,
  /// ★ Named from the game: "checked the chest" (MsgVaultIsaacCheckedChest)
  CELL_BEHAVIOR_LAMA_TEMPLE_CHEST = 100,
  /// ★ Named from the game: "a Psynergy Stone" (MsgGotPsynergyStone)
  CELL_BEHAVIOR_LAMA_TEMPLE_PSYNERGY_STONE = 110
};

/// The cell behaviors Alpine Crossing's triggers answer.
enum AlpineCrossingCellBehavior {
  CELL_BEHAVIOR_ALPINE_CROSSING_SIGNPOST = 7,
  CELL_BEHAVIOR_ALPINE_CROSSING_FEIZHI_ARRIVAL = 10,
  /// ★ Named from the game: "checked the chest" (MsgVaultIsaacCheckedChest)
  CELL_BEHAVIOR_ALPINE_CROSSING_CHEST = 100
};

/// The door of Hama's room, three cells wide and two high: each step copies
/// the next door picture onto it for five frames.
enum { DOOR_X = 67, DOOR_Y = 6 };

/// The map object that draws the passage behind the mossy rock.
enum { TREASURE_SPOT_PASSAGE = 100 };

/// Draws the actor behind the background, on sprite priority 3 with
/// automatic priority off. The first worker from Xian at the Alpine Crossing
/// runs it from his actor placement.
static s32 Actor_DrawBehindScenery(struct Actor *interpreter) {
  struct Actor *actor = (struct Actor *)interpreter;
  actor->priorityFlags &= ~ACTOR_PRIORITY_AUTOMATIC;
  actor->sprite->image.priority = 3;
  actor->sprite->shadow.priority = 3;
  return 0;
}

// Update routine that turns the party leader toward Feizhi while she runs
// past.
// UB: update routines return void. Plain void removes the native zero result
// and changes its return instructions (three in tbs-en).
#ifdef UBFIX
static void AlpineCrossing_LeaderFaceFeizhi(struct Actor *updated) {
#else
static s32 AlpineCrossing_LeaderFaceFeizhi(struct Actor *updated) {
#endif
  struct Actor *leader = (struct Actor *)updated;
  struct Actor *feizhi;

  feizhi = Actor_GetById(ACTOR_FEIZHI);
  leader->facing = ArcTan2(feizhi->z.fixed - leader->z.fixed,
                           feizhi->x.fixed - leader->x.fixed);
#ifndef UBFIX
  return 0;
#endif
}

const struct SceneEntrance *Scene_GetEntrances(void) {
  if (gAdventure.sceneId == MAP_ALPINE_CROSSING) {
    return gAlpineCrossing_Entrances;
  }
  return gLamaTemple_Entrances;
}

const struct SceneExitRegion *Scene_GetExitRegions(void) { return 0; }

/// Both scenes share one exit table.
const u32 *Scene_GetExits(void) { return gAlpineCrossingLamaTemple_Exits; }

const struct ActorPlacement *Scene_GetActorPlacements(void) {
  if (gAdventure.sceneId == MAP_ALPINE_CROSSING) {
    return gAlpineCrossing_ActorPlacements;
  }
  return gLamaTemple_ActorPlacements;
}

/// A monk in Lama Temple, until Hsu is freed, asks whether the party comes
/// from the east. If not, it must have crossed Lamakan Desert; if so, he asks
/// whether it stopped in Xian, whose Master Feh is close to Master Hama.
static void LamaTemple_TalkToMonkAskingAboutTheEast(void) {

  Event_Start();
  Event_SetMessageId(MsgMountainYoungWarriorsDoComeFrom);
  FieldMessage_ShowAtActor(ACTOR_MONK_ASKING_ABOUT_THE_EAST, 0);

  if (Event_AskYesNoWithActorAnimation(ACTOR_PARTY_LEADER, 0) == 1) {
    Event_WaitFrames(20);
    Event_ShowMessageAndWait(ACTOR_MONK_ASKING_ABOUT_THE_EAST, 0);
  } else {
    Event_SkipMessages(1);
    Event_AskYesNoAndShowResponse(ACTOR_MONK_ASKING_ABOUT_THE_EAST, 0);
  }

  Event_End();
}

/// The meditating monk explains meditation. He's in Lama Temple until Hsu is
/// freed, and first asks whether the party knows what meditation is.
static void LamaTemple_TalkToMeditatingMonk(void) {
  Event_Start();
  Event_SetMessageId(MsgMountainDoKnowMeditation);
  Event_AskYesNoAndShowResponse(ACTOR_MEDITATING_MONK, 0);
  Event_End();
}

/// The monk in Lama Temple who, until Hsu is freed, praises Master Hama as the
/// greatest master of Chi.
static void LamaTemple_TalkToChiMonk(void) {
  Event_Start();
  Event_SetMessageId(MsgMountainDidKnowMasterHamaGreatest);
  Event_AskYesNoAndShowResponse(ACTOR_CHI_MONK, 0);
  Event_End();
}

/// The door of Hama's room slides open for the party leader. Walking north into
/// it in Lama Temple slides it open and walks the leader through, into the room
/// (LAMA_TEMPLE_TO_LACQUER_ROOMS_A_HAMAS_ROOM).
static void LamaTemple_EnterHamasRoom(void) {
  Event_PlaySound(SE_THUMP);
  MapGrid_AnimateCells(gLamaTemple_DoorOpenCells, DOOR_X, DOOR_Y);
  Actor_GetById(ACTOR_PARTY_LEADER)->motionFlags = 0;
  Actor_SetSpeed(ACTOR_PARTY_LEADER, 0xcccc, 0x6666);
  gFieldWork->transition = SCENE_TRANSITION(TRANSITION_BACKDROP_FADE, 0);
  Import_ActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_WALK);
  Actor_SetDestinationOffset(ACTOR_PARTY_LEADER, 0, -16);
  Event_WaitFrames(16);
  Import_EventSetExitRequest(LAMA_TEMPLE_TO_LACQUER_ROOMS_A_HAMAS_ROOM);
}

/// Hama's farewell, once the party has freed Hsu and been brought back to Lama
/// Temple: she thanks the Adepts, Garet and Mia squabble over whose idea
/// helping was, and Hama goes back to her room. Ivan wonders aloud about what
/// Hama told him, and Garet, Ivan and Mia rejoin the party. Garet's two
/// questions are yes or no choices.
static void LamaTemple_RunHamasFarewell(void) {
  Event_Start();
  Actor_SetPosition(ACTOR_TEMPLE_HAMA, PIXELS(136), PIXELS(168));
  Actor_FaceDirection(ACTOR_TEMPLE_HAMA, FACING_SOUTH + FACING_STEP, 0);
  Actor_SetPosition(ACTOR_PARTY_LEADER, PIXELS(144), PIXELS(200));
  Actor_SetPosition(ACTOR_GARET, PIXELS(160), PIXELS(192));
  Actor_SetPosition(ACTOR_IVAN, PIXELS(128), PIXELS(200));
  Actor_SetPosition(ACTOR_MIA, PIXELS(112), PIXELS(192));
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 0);
  Actor_FaceDirection(ACTOR_GARET, FACING_NORTHWEST, 0);
  Actor_FaceDirection(ACTOR_IVAN, FACING_NORTH, 0);
  Actor_FaceDirection(ACTOR_MIA, FACING_NORTHEAST, 0);
  gFieldWork->transition =
      SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_SHAPE);
  Event_StartScreenTransition();
  Event_WaitTransitionFrames();
  Event_WaitFrames(60);

  // "Adepts, let me thank you again."
  Actor_SetAnimationAndWait(ACTOR_TEMPLE_HAMA, ANIM_NOD);
  Event_WaitFrames(20);
  Event_SetMessageId(MsgMountainAdeptsLetMeThankAgain);
  FieldEvent_ShowMessageThenWait(ACTOR_TEMPLE_HAMA, 0, 20);
  Import_ActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_NOD);
  Import_ActorSetAnimation(ACTOR_GARET, ANIM_NOD);
  Import_ActorSetAnimation(ACTOR_IVAN, ANIM_NOD);
  Actor_SetAnimationAndWait(ACTOR_MIA, ANIM_NOD);
  Event_WaitFrames(20);
  Actor_SetAnimationAndWait(ACTOR_TEMPLE_HAMA, ANIM_SHAKE_HEAD);
  Event_WaitFrames(20);
  FieldEvent_ShowMessageThenWait(ACTOR_TEMPLE_HAMA, 0, 20);
  Actor_SetAnimationAndWait(ACTOR_MIA, ANIM_SHAKE_HEAD);
  Event_WaitFrames(20);
  FieldEvent_ShowMessageThenWait(ACTOR_MIA, 0, 20);
  Actor_RunRepeatedMotion(ACTOR_IVAN, 2);
  Event_WaitFrames(20);
  FieldEvent_ShowMessageThenWait(ACTOR_IVAN, 0, 20);
  Actor_RunRepeatedMotion(ACTOR_TEMPLE_HAMA, 2);
  Actor_FaceDirection(ACTOR_TEMPLE_HAMA, FACING_SOUTH + FACING_STEP, 20);
  FieldEvent_ShowMessageThenWait(ACTOR_TEMPLE_HAMA, 0, 20);
  Actor_ShowEmote(ACTOR_IVAN, EMOTE_IN_FRONT | EMOTE_WINCE, 60);
  Event_WaitFrames(120);

  // Garet claims that helping was his idea and asks the leader to agree.
  Actor_RunRepeatedMotion(ACTOR_GARET, 2);
  Event_WaitFrames(20);
  Actor_FaceDirection(ACTOR_GARET, FACING_SOUTHWEST, 20);
  Event_OpenMessage(ACTOR_GARET, 0);
  if (Event_AskYesNoWithActorAnimation(ACTOR_PARTY_LEADER, 0) == 0) {
    Event_WaitFrames(20);
    Actor_FaceDirection(ACTOR_MIA, FACING_EAST, 20);
    Actor_ShowEmote(ACTOR_MIA, EMOTE_IN_FRONT | EMOTE_QUESTION, 60);
    FieldEvent_ShowMessageThenWait(ACTOR_MIA, 0, 20);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTHEAST, 0);
    Actor_FaceDirection(ACTOR_IVAN, FACING_NORTHEAST, 0);
    Actor_StartRepeatedMotion(ACTOR_GARET, 2);
    Actor_SetAttachedEffect(ACTOR_GARET,
                            ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
    Event_WaitFrames(60);
    Actor_FaceDirection(ACTOR_GARET, FACING_WEST, 20);
    FieldEvent_ShowMessageThenWait(ACTOR_GARET, 0, 20);
    Event_SkipMessages(2);
  } else {
    Event_SkipMessages(2);
    Event_WaitFrames(20);
    Actor_FaceDirection(ACTOR_MIA, FACING_EAST, 20);
    Actor_SetAnimationAndWait(ACTOR_MIA, ANIM_NOD);
    Event_WaitFrames(20);
    FieldEvent_ShowMessageThenWait(ACTOR_MIA, 0, 20);
    Actor_ShowEmote(ACTOR_GARET, EMOTE_IN_FRONT | EMOTE_WINCE, 60);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTHEAST, 0);
    Actor_FaceDirection(ACTOR_IVAN, FACING_NORTHEAST, 0);
    FieldEvent_ShowMessageThenWait(ACTOR_GARET, 0, 20);
  }

  // Hama wishes them well and goes back into her room.
  Actor_RunRepeatedMotion(ACTOR_TEMPLE_HAMA, 2);
  Event_WaitFrames(20);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 0);
  Actor_FaceDirection(ACTOR_GARET, FACING_NORTHWEST, 0);
  Actor_FaceDirection(ACTOR_IVAN, FACING_NORTH, 0);
  Actor_FaceDirection(ACTOR_MIA, FACING_NORTHEAST, 0);
  Event_WaitFrames(20);
  Actor_FaceDirection(ACTOR_TEMPLE_HAMA, FACING_SOUTH - FACING_STEP, 20);
  FieldEvent_ShowMessageThenWait(ACTOR_TEMPLE_HAMA, 0, 20);
  Actor_RunRepeatedMotion(ACTOR_GARET, 2);
  Event_WaitFrames(20);
  Actor_SetAnimationAndWait(ACTOR_GARET, ANIM_NOD);
  Event_WaitFrames(30);
  Actor_SetAnimationAndWait(ACTOR_TEMPLE_HAMA, ANIM_NOD);
  Event_WaitFrames(20);
  FieldEvent_ShowMessageThenWait(ACTOR_TEMPLE_HAMA, 0, 20);
  Actor_RunRepeatedMotion(ACTOR_TEMPLE_HAMA, 2);
  Event_WaitFrames(20);
  FieldEvent_ShowMessageThenWait(ACTOR_TEMPLE_HAMA, 0, 20);
  Import_ActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_NOD);
  Import_ActorSetAnimation(ACTOR_GARET, ANIM_NOD);
  Import_ActorSetAnimation(ACTOR_IVAN, ANIM_NOD);
  Actor_SetAnimationAndWait(ACTOR_MIA, ANIM_NOD);
  Event_WaitFrames(20);
  Actor_SetAnimationAndWait(ACTOR_TEMPLE_HAMA, ANIM_NOD);
  Event_WaitFrames(20);
  Actor_FaceDirection(ACTOR_TEMPLE_HAMA, FACING_NORTH, 30);
  // The door slides open with SE_THUMP, Hama walks through, and it slides shut.
  Event_PlaySound(SE_THUMP);
  MapGrid_AnimateCells(gLamaTemple_DoorOpenCells, DOOR_X, DOOR_Y);
  Actor_SetSpeed(ACTOR_TEMPLE_HAMA, 0xcccc, 0x6666);
  Actor_WalkToAndWait(ACTOR_TEMPLE_HAMA, 136, 136);
  Actor_SetPosition(ACTOR_TEMPLE_HAMA, 0, 0);
  Event_PlaySound(SE_THUMP);
  MapGrid_AnimateCells(gLamaTemple_DoorCloseCells, DOOR_X, DOOR_Y);
  Event_WaitFrames(60);
  Scene_PlayQueuedMusic();

  // Garet asks whether the leader means to leave Hama's words unasked.
  Actor_RunRepeatedMotion(ACTOR_GARET, 2);
  Event_WaitFrames(20);
  Actor_FaceDirection(ACTOR_GARET, FACING_SOUTHWEST, 20);
  FieldEvent_ShowMessageThenWait(ACTOR_GARET, 0, 20);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_WEST, 20);
  Actor_RunRepeatedMotion(ACTOR_IVAN, 2);
  Event_WaitFrames(20);
  Actor_FaceDirection(ACTOR_MIA, FACING_SOUTHEAST, 20);
  FieldEvent_ShowMessageThenWait(ACTOR_MIA, 0, 20);
  Actor_RunRepeatedMotion(ACTOR_GARET, 1);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTHEAST, 20);
  Event_AskYesNo(ACTOR_GARET, 0);
  Event_WaitFrames(20);

  // Ivan decides that Hama will tell him when their mission is done.
  Actor_RunRepeatedMotion(ACTOR_IVAN, 2);
  Event_WaitFrames(20);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_WEST, 20);
  FieldEvent_ShowMessageThenWait(ACTOR_IVAN, 0, 20);
  Actor_StartRepeatedMotion(ACTOR_PARTY_LEADER, 1);
  Actor_StartRepeatedMotion(ACTOR_GARET, 1);
  Actor_RunRepeatedMotion(ACTOR_MIA, 1);
  Event_WaitFrames(20);
  Actor_SetAnimationAndWait(ACTOR_IVAN, ANIM_SHAKE_HEAD);
  Event_WaitFrames(20);
  FieldEvent_ShowMessageThenWait(ACTOR_IVAN, 0, 20);
  Actor_RunRepeatedMotion(ACTOR_MIA, 2);
  Event_WaitFrames(20);
  FieldEvent_ShowMessageThenWait(ACTOR_MIA, 0, 20);
  Actor_FaceDirection(ACTOR_IVAN, FACING_NORTHWEST + FACING_STEP, 20);
  Actor_SetAnimationAndWait(ACTOR_IVAN, ANIM_NOD);
  Event_WaitFrames(20);
  Actor_SetAnimationAndWait(ACTOR_IVAN, ANIM_NOD);
  Event_WaitFrames(20);
  FieldEvent_ShowMessageThenWait(ACTOR_IVAN, 0, 20);
  Actor_ShowEmote(ACTOR_GARET, EMOTE_IN_FRONT | EMOTE_WINCE, 60);
  FieldEvent_ShowMessageThenWait(ACTOR_GARET, 0, 20);
  Actor_FaceDirection(ACTOR_IVAN, FACING_NORTHEAST, 20);
  Actor_SetAnimationAndWait(ACTOR_IVAN, ANIM_SHAKE_HEAD);
  Event_WaitFrames(20);
  FieldEvent_ShowMessageThenWait(ACTOR_IVAN, 0, 20);
  Actor_ShowEmote(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | EMOTE_WINCE, 0);
  Actor_ShowEmote(ACTOR_MIA, EMOTE_IN_FRONT | EMOTE_WINCE, 60);
  FieldEvent_ShowMessageThenWait(ACTOR_MIA, 0, 20);
  Actor_FaceDirection(ACTOR_IVAN, FACING_NORTHWEST, 20);
  Actor_SetAnimationAndWait(ACTOR_IVAN, ANIM_NOD);
  Event_WaitFrames(20);
  FieldEvent_ShowMessageThenWait(ACTOR_IVAN, 0, 20);
  Actor_StartRepeatedMotion(ACTOR_PARTY_LEADER, 1);
  Actor_StartRepeatedMotion(ACTOR_GARET, 1);
  Actor_RunRepeatedMotion(ACTOR_MIA, 1);
  Event_WaitFrames(20);
  Actor_SetSpeed(ACTOR_IVAN, 0x8000, 0x4000);
  Actor_WalkToAndWait(ACTOR_IVAN, 128, 184);
  Actor_FaceDirection(ACTOR_IVAN, FACING_SOUTH, 20);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 0);
  Actor_FaceDirection(ACTOR_GARET, FACING_NORTHWEST, 0);
  Actor_FaceDirection(ACTOR_MIA, FACING_NORTHEAST, 20);
  Actor_SetAnimationAndWait(ACTOR_IVAN, ANIM_NOD);
  Event_WaitFrames(20);
  FieldEvent_ShowMessageThenWait(ACTOR_IVAN, 0, 20);
  Import_ActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_NOD);
  Import_ActorSetAnimation(ACTOR_GARET, ANIM_NOD);
  Actor_SetAnimationAndWait(ACTOR_MIA, ANIM_NOD);
  Event_WaitFrames(20);

  // Garet, Ivan and Mia walk to the leader and rejoin the party.
  Actor_SetSpeed(ACTOR_GARET, 0x8000, 0x4000);
  Actor_SetSpeed(ACTOR_MIA, 0x8000, 0x4000);
  Actor_WalkTo(ACTOR_GARET, 144, 200);
  Actor_WalkTo(ACTOR_IVAN, 144, 200);
  Actor_WalkTo(ACTOR_MIA, 144, 200);
  Actor_WaitForMoveById(ACTOR_GARET);
  Actor_SetPosition(ACTOR_GARET, 0, 0);
  Actor_WaitForMoveById(ACTOR_IVAN);
  Actor_SetPosition(ACTOR_IVAN, 0, 0);
  Actor_WaitForMoveById(ACTOR_MIA);
  Actor_SetPosition(ACTOR_MIA, 0, 0);
  Event_End();
}

/// Searching the writing on a wall in Lama Temple reads its hint toward Reveal.
static void LamaTemple_ReadWallWriting(void) {

  Event_Start();
  Import_ActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_STAND);
  FieldMessage_Show(MsgMountainHeWhoHasPowerSee, 1);
  Event_End();
}

/// Puts Lama Temple's mossy rock back when Reveal wears off (and on entry):
/// the wall's cell blocks the way again, the passage's map object is hidden and
/// the rock actor stands in front of it.
static void LamaTemple_ShowFalseWall(void) {

  MapGrid_CopyCellAttributes(85, 9, 1, 1, 21, 9);
  TreasureSpot_SetActorPosition(TREASURE_SPOT_PASSAGE, 0, 0);
  Actor_SetPosition(ACTOR_PASSAGE_ROCK, PIXELS(344), PIXELS(152));
}

/// Reveal in Lama Temple shows the passage behind the mossy rock: the cell's
/// original attributes (saved at (21, 73) on entry) come back, the passage's
/// map object returns to its place and the rock actor is put away.
static void LamaTemple_RevealPassage(void) {
  MapGrid_CopyCellAttributes(21, 73, 1, 1, 21, 9);
  TreasureSpot_SetPosition(TREASURE_SPOT_PASSAGE, -1, -1);
  Actor_SetPosition(ACTOR_PASSAGE_ROCK, 0, 0);
}

/// The traveling healer in Lama Temple offers the Sanctum's services. He does
/// when spoken to across his counter, from the south; from any other side he
/// only greets the party.
static void LamaTemple_TalkToHealer(void) {

  u32 heading = Actor_GetById(ACTOR_PARTY_LEADER)->facing;

  Event_Start();
  if (FACING_IS_NORTH(heading)) {
    Sanctum_Run(ACTOR_TRAVELING_HEALER);
  } else {
    Event_SetMessageId(MsgMountainAmTravelingAroundWorldSpread);
    Event_ShowMessageAndWait(ACTOR_TRAVELING_HEALER, 0);
  }
  Event_End();
}

/// Feizhi runs up behind the party and goes ahead to look for Hsu. It happens
/// at the Alpine Crossing when the party steps onto the road by the boulders
/// once Feizhi has set out after Hsu: she finds the boulders blocking Silk
/// Road, says her vision came true, cries out for Hsu and runs ahead along the
/// path to look for him, the party leader following part of the way.
static void AlpineCrossing_FeizhiArrives(void) {
  struct Actor *leader;

  if (GameFlag_Get(FLAG_FEIZHI_WENT_TO_HSU)) {
    Event_Start();
    Actor_SetPosition(ACTOR_FEIZHI, PIXELS(536), PIXELS(216));
    Event_SetMessageId(MsgMountainYahhSilkRoadBouldersBlock);
    FieldEvent_ShowMessageThenWait(ACTOR_FEIZHI, 0, 20);
    Actor_RunRepeatedMotion(ACTOR_PARTY_LEADER, 2);
    Event_WaitFrames(20);

    // The leader turns to follow Feizhi as she runs past; one standing in
    // her row steps aside first.
    leader = Actor_GetById(ACTOR_PARTY_LEADER);
#ifdef UBFIX
    leader->update = AlpineCrossing_LeaderFaceFeizhi;
#else
    leader->update = (void (*)(struct Actor *))AlpineCrossing_LeaderFaceFeizhi;
#endif
    leader = Actor_GetById(ACTOR_PARTY_LEADER);
    if ((leader->z.fixed >> 20) == 13) {
      Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 440, 200);
    }
    Actor_SetSpeed(ACTOR_FEIZHI, 0x20000, 0x10000);
    Actor_SetSpritePriority(ACTOR_FEIZHI, 2);
    Actor_WalkToAndWait(ACTOR_FEIZHI, 408, 216);
    Actor_GetById(ACTOR_FEIZHI)->priorityFlags |= ACTOR_PRIORITY_AUTOMATIC;
    Event_WaitFrames(10);
    Actor_FaceDirection(ACTOR_FEIZHI, FACING_WEST, 20);
    FieldEvent_ShowMessageThenWait(ACTOR_FEIZHI, 0, 20);
    Actor_StartRepeatedMotion(ACTOR_FEIZHI, 2);
    Actor_SetAttachedEffect(ACTOR_FEIZHI,
                            ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
    Event_WaitFrames(60);
    FieldEvent_ShowMessageThenWait(ACTOR_FEIZHI, 0, 20);

    // Feizhi runs down the path with the camera; the leader follows her
    // until the camera arrives.
    Actor_SetScript(ACTOR_FEIZHI, gAlpineCrossing_FeizhiPath);
    FieldCamera_MoveTo(PIXELS(296), -1, PIXELS(344), 1);
    GameFlag_Set(FLAG_FEIZHI_SAW_ROCKSLIDE);
    Actor_RefreshSelectorById(ACTOR_FEIZHI);
    FieldCamera_WaitForArrival();
    Actor_SetSpeed(ACTOR_PARTY_LEADER, 0x10000, 0x8000);
    Actor_SetScript(ACTOR_PARTY_LEADER, gAlpineCrossing_LeaderPath);
    Actor_RefreshSelectorById(ACTOR_PARTY_LEADER);
    Event_WaitFrames(10);
    leader = Actor_GetById(ACTOR_PARTY_LEADER);
    leader->update = NULL;
    Event_WaitFrames(30);

    // She cannot hear Hsu's voice.
    Actor_RunRepeatedMotion(ACTOR_FEIZHI, 2);
    Event_WaitFrames(20);
    Actor_FaceDirection(ACTOR_FEIZHI, FACING_SOUTH + FACING_STEP, 120);
    Actor_ShowEmote(ACTOR_FEIZHI, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 60);
    Actor_ShowEmote(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | EMOTE_QUESTION, 60);
    Actor_SetAnimationAndWait(ACTOR_FEIZHI, ANIM_SHAKE_HEAD);
    Event_WaitFrames(20);
    FieldEvent_ShowMessageThenWait(ACTOR_FEIZHI, 0, 20);
    Event_End();
  }
}

/// Talking to Feizhi at the Alpine Crossing before Hama's Reveal lesson: she
/// asks whether she should return to Xian, and either way decides to keep
/// looking for Hsu.
static void AlpineCrossing_TalkToFeizhi(void) {
  Event_Start();
  Event_SetMessageId(MsgMountainDoDoWarriorShouldReturn);
  Actor_ShowEmote(ACTOR_FEIZHI, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 60);
  Event_OpenMessage(ACTOR_FEIZHI, 0);
  if (Event_AskYesNoWithActorAnimation(ACTOR_PARTY_LEADER, 0) == 1) {
    Event_SkipMessages(1);
  }
  Event_WaitFrames(20);
  Actor_SetAnimationAndWait(ACTOR_FEIZHI, ANIM_SHAKE_HEAD);
  Event_WaitFrames(20);
  FieldEvent_ShowMessageThenWait(ACTOR_FEIZHI, 0, 20);
  Event_End();
}

/// Arriving at the Alpine Crossing from AltinPeakSouthEntrance by
/// ALPINE_CROSSING_FROM_ALTIN_PEAK_SOUTH_ENTRANCE_EAST while Hsu is trapped,
/// until this has run once (FLAG_HAMA_REACHED_HSU): the camera finds Hsu, Hama
/// asks if he is okay, and he is unhurt but cannot move; none of them knows how
/// to free him.
static void AlpineCrossing_HamaReachesHsu(void) {
  Event_Start();
  // Reveal Hsu's position before Hama and Feizhi discuss freeing him.
  Event_StartScreenTransition();
  Event_WaitTransitionFrames();
  Event_WaitFrames(30);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_SOUTHWEST, 0);
  Actor_PlaceCurrentWithinCameraBounds(ACTOR_HSU, 1);
  FieldCamera_WaitForArrival();
  Actor_RunRepeatedMotion(ACTOR_HAMA, 2);
  Event_WaitFrames(20);
  Event_SetMessageId(MsgMountainHsuOkay);
  FieldEvent_ShowMessageThenWait(ACTOR_HAMA, 0, 20);
  Actor_RunRepeatedMotion(ACTOR_HSU, 1);
  Event_WaitFrames(20);
  FieldEvent_ShowMessageThenWait(ACTOR_HSU, 0, 20);
  Actor_SetAnimationAndWait(ACTOR_HAMA, ANIM_NOD);
  Event_WaitFrames(20);
  FieldEvent_ShowMessageThenWait(ACTOR_HAMA, 0, 20);
  Actor_FaceDirection(ACTOR_FEIZHI, FACING_NORTH + FACING_STEP, 20);
  FieldEvent_ShowMessageThenWait(ACTOR_FEIZHI, 0, 20);
  Actor_SetAnimationAndWait(ACTOR_HAMA, ANIM_NOD);
  Event_WaitFrames(60);
  Actor_ShowEmote(ACTOR_HAMA, EMOTE_IN_FRONT | EMOTE_WINCE, 60);
  Actor_SetAnimationAndWait(ACTOR_HAMA, ANIM_SHAKE_HEAD);
  Event_WaitFrames(20);
  FieldEvent_ShowMessageThenWait(ACTOR_HAMA, 0, 20);
  Actor_StartRepeatedMotion(ACTOR_FEIZHI, 2);
  Actor_SetAttachedEffect(ACTOR_FEIZHI,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  Event_WaitFrames(60);
  Actor_FaceDirection(ACTOR_FEIZHI, FACING_NORTHWEST + FACING_STEP, 20);
  Import_ActorSetAnimation(ACTOR_HSU, ANIM_HSU_STRUGGLE);
  Event_End();
  GameFlag_Set(FLAG_HAMA_REACHED_HSU);
}

/// Lift on the boulder at the Alpine Crossing, until Hsu is freed: it floats,
/// and Hama and Feizhi pull him free in two heaves of two tugs each, asking the
/// leader in between to hold on. Hsu is free; they will ask Master Feh to tend
/// him and clear the road, and the event fades out to Hama's farewell at Lama
/// Temple.
static void AlpineCrossing_LiftBoulder(void) {

  Event_Start();
  Actor_SetAttachedEffect(ACTOR_HAMA,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  Actor_StartRepeatedMotion(ACTOR_HAMA, 2);
  Event_WaitFrames(60);
  Event_SetMessageId(MsgMountainIsaacDidLiftBoulder);
  FieldEvent_ShowMessageThenWait(ACTOR_HAMA, 0, 20);
  Actor_SetAttachedEffect(ACTOR_FEIZHI,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  Actor_Jump(ACTOR_FEIZHI, 4, 0);
  Event_WaitFrames(60);
  FieldEvent_ShowMessageThenWait(ACTOR_FEIZHI, 0, 20);
  Actor_RunRepeatedMotion(ACTOR_HAMA, 1);
  Event_WaitFrames(20);
  FieldEvent_ShowMessageThenWait(ACTOR_HAMA, 0, 20);
  Actor_FaceDirection(ACTOR_FEIZHI, FACING_NORTH + FACING_STEP, 20);
  Actor_SetAnimationAndWait(ACTOR_FEIZHI, ANIM_NOD);
  Event_WaitFrames(20);

  // Hama and Feizhi take hold of Hsu.
  Actor_WalkTo(ACTOR_HAMA, 178, 276);
  Actor_WalkToAndWait(ACTOR_FEIZHI, 172, 284);
  Actor_WaitForMoveById(ACTOR_HAMA);
  Actor_FaceDirection(ACTOR_HAMA, FACING_SOUTH + FACING_STEP, 0);
  Actor_FaceDirection(ACTOR_FEIZHI, FACING_NORTHWEST + FACING_STEP, 0);
  Event_WaitFrames(20);
  Actor_RunRepeatedMotion(ACTOR_HAMA, 2);
  Event_WaitFrames(20);
  FieldEvent_ShowMessageThenWait(ACTOR_HAMA, 0, 20);
  Actor_GetById(ACTOR_HAMA)->steeringFlags &= (u8)~ACTOR_FACES_MOTION;
  Actor_GetById(ACTOR_FEIZHI)->steeringFlags &= (u8)~ACTOR_FACES_MOTION;
  Actor_SetSpeed(ACTOR_HAMA, 0x3333, 0x1999);
  Actor_SetSpeed(ACTOR_FEIZHI, 0x3333, 0x1999);

  // The first heave: two tugs draw Hsu halfway out.
  Import_ActorSetAnimation(ACTOR_HAMA, ANIM_HAMA_PULL);
  Import_ActorSetAnimation(ACTOR_FEIZHI, ANIM_FEIZHI_PULL);
  Event_WaitFrames(20);
  Event_PlaySound(SE_CHEST_CLOSE);
  Actor_SetDestinationOffset(ACTOR_HAMA, 2, 0);
  Actor_SetDestinationOffset(ACTOR_HSU, 2, 0);
  Actor_SetDestinationOffset(ACTOR_FEIZHI, 2, 0);
  Actor_WaitForMoveById(ACTOR_FEIZHI);
  Event_WaitFrames(30);
  Import_ActorSetAnimation(ACTOR_HAMA, ANIM_HAMA_PULL);
  Import_ActorSetAnimation(ACTOR_FEIZHI, ANIM_FEIZHI_PULL);
  Event_WaitFrames(20);
  Event_PlaySound(SE_CHEST_CLOSE);
  Actor_SetDestinationOffset(ACTOR_HAMA, 4, 0);
  Actor_SetDestinationOffset(ACTOR_HSU, 4, 0);
  Actor_SetDestinationOffset(ACTOR_FEIZHI, 4, 0);
  Actor_WaitForMoveById(ACTOR_FEIZHI);
  Actor_Stop(ACTOR_HSU);
  Import_ActorSetAnimation(ACTOR_HAMA, ANIM_STAND);
  Import_ActorSetAnimation(ACTOR_FEIZHI, ANIM_STAND);
  Event_WaitFrames(50);
  Actor_Jump(ACTOR_FEIZHI, 2, 0);
  Event_WaitFrames(20);
  FieldEvent_ShowMessageThenWait(ACTOR_FEIZHI, 0, 20);

  // Hama asks the leader to hold the boulder a moment longer.
  Actor_SetAnimationAndWait(ACTOR_HAMA, ANIM_NOD);
  Event_WaitFrames(20);
  Event_AskYesNo(ACTOR_HAMA, 0);
  FieldEvent_ShowMessageThenWait(ACTOR_HAMA, 0, 20);

  // The second heave pulls Hsu free.
  Import_ActorSetAnimation(ACTOR_HAMA, ANIM_HAMA_PULL);
  Import_ActorSetAnimation(ACTOR_FEIZHI, ANIM_FEIZHI_PULL);
  Event_WaitFrames(20);
  Event_PlaySound(SE_CHEST_CLOSE);
  Actor_SetDestinationOffset(ACTOR_HAMA, 2, 0);
  Actor_SetDestinationOffset(ACTOR_HSU, 2, 0);
  Actor_SetDestinationOffset(ACTOR_FEIZHI, 2, 0);
  Actor_WaitForMoveById(ACTOR_FEIZHI);
  Event_WaitFrames(30);
  Import_ActorSetAnimation(ACTOR_HAMA, ANIM_HAMA_PULL);
  Import_ActorSetAnimation(ACTOR_FEIZHI, ANIM_FEIZHI_PULL);
  Event_WaitFrames(20);
  Event_PlaySound(SE_CHEST_CLOSE);
  Actor_SetDestinationOffset(ACTOR_HAMA, 4, 0);
  Actor_SetDestinationOffset(ACTOR_HSU, 4, 0);
  Actor_SetDestinationOffset(ACTOR_FEIZHI, 4, 0);
  Actor_WaitForMoveById(ACTOR_FEIZHI);
  Event_WaitFrames(40);
  Import_ActorSetAnimation(ACTOR_HAMA, ANIM_STAND);
  Import_ActorSetAnimation(ACTOR_FEIZHI, ANIM_STAND);
  Actor_Jump(ACTOR_FEIZHI, 2, 0);
  Event_WaitFrames(20);
  Actor_FaceDirection(ACTOR_FEIZHI, FACING_NORTH + FACING_STEP, 20);
  FieldEvent_ShowMessageThenWait(ACTOR_FEIZHI, 0, 20);
  Actor_SetAnimationAndWait(ACTOR_HAMA, ANIM_NOD);
  Event_WaitFrames(20);
  FieldEvent_ShowMessageThenWait(ACTOR_HAMA, 0, 30);
  FieldEvent_ShowMessageThenWait(ACTOR_HAMA, 0, 20);

  // Feizhi steps back; they agree to send for Master Feh.
  Actor_GetById(ACTOR_FEIZHI)->steeringFlags |= ACTOR_FACES_MOTION;
  Actor_SetSpeed(ACTOR_FEIZHI, 0xcccc, 0x6666);
  Actor_WalkToAndWait(ACTOR_FEIZHI, 168, 296);
  Actor_FaceDirection(ACTOR_FEIZHI, FACING_NORTH + FACING_STEP, 20);
  Import_ActorSetAnimation(ACTOR_FEIZHI, ANIM_FEIZHI_CROUCH);
  Actor_SetAnimationAndWait(ACTOR_HAMA, ANIM_NOD);
  Event_WaitFrames(20);
  FieldEvent_ShowMessageThenWait(ACTOR_HAMA, 0, 20);
  Actor_SetAnimationAndWait(ACTOR_FEIZHI, ANIM_NOD);
  Event_WaitFrames(20);
  FieldEvent_ShowMessageThenWait(ACTOR_FEIZHI, 0, 20);
  Actor_SetAnimationAndWait(ACTOR_HAMA, ANIM_NOD);
  Event_WaitFrames(20);
  FieldEvent_ShowMessageThenWait(ACTOR_HAMA, 0, 20);
  Actor_SetAnimationAndWait(ACTOR_PARTY_LEADER, ANIM_NOD);
  Event_WaitFrames(20);
  Event_End();
  GameFlag_Set(FLAG_HSU_RESCUED);
  gFieldWork->transition =
      SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_SHAPE);
  Import_EventSetExitRequest(ALPINE_CROSSING_TO_LAMA_TEMPLE);
}

/// Searching the signpost at the Alpine Crossing.
/// "North: Altin Mine. West: Lama Temple."
static void AlpineCrossing_ReadSignpost(void) {
  Event_Start();
  Import_ActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_STAND);
  FieldMessage_Show(MsgMountainNorthAltinMineWestLama, 1);
  Event_End();
}

/// Reveal at the Alpine Crossing shows the path hidden at (5, 13): the revealed
/// cells, kept at (5, 28), are copied there with their attributes.
static void AlpineCrossing_RevealPath(void) {
  MapGrid_CopyCellsTo(5, 28, 5, 13, 1, 2);
  MapGrid_CopyCellAttributes(5, 28, 1, 2, 5, 13);
  Event_WaitFrames(1);
}

/// When Reveal wears off at the Alpine Crossing, the plain cells kept at
/// (6, 28) cover the path again.
static void AlpineCrossing_HidePath(void) {
  MapGrid_CopyCellsTo(6, 28, 5, 13, 1, 2);
  MapGrid_CopyCellAttributes(6, 28, 1, 2, 5, 13);
  Event_WaitFrames(1);
}

// Update routine of the frozen pillar: it draws under the party leader
// while the leader stands higher than its top, so the leader can climb onto
// it, and over the leader otherwise.
// UB: this update routine has no result. Plain void changes the native return
// from pop {r1}; bx r1 to pop {r0}; bx r0 (tbs-en).
#ifdef UBFIX
static void Pillar_SortAgainstLeader(struct Actor *updated) {
#else
static s32 Pillar_SortAgainstLeader(struct Actor *updated) {
#endif
  struct Actor *pillar = (struct Actor *)updated;
  if (Actor_GetById(ACTOR_PARTY_LEADER)->y.fixed > pillar->y.fixed) {
    pillar->priorityFlags |= ACTOR_PRIORITY_UNDERFOOT;
  } else {
    pillar->priorityFlags &= (u8)~ACTOR_PRIORITY_UNDERFOOT;
  }
}

/// Puts the rising pillar in front when the leader stands north of the puddle.
/// As Frost reaches the puddle at the Alpine Crossing, a leader standing north
/// of it sees the pillar rise in front of the background.
static void AlpineCrossing_PrepareFrost(void) {
  struct Actor *leader = Actor_GetById(ACTOR_PARTY_LEADER);
  struct Actor *puddle = Actor_GetById(ACTOR_WATER_FOR_FROST);

  if (leader->z.fixed <= puddle->z.fixed) {
    Actor_SetSpritePriority(ACTOR_WATER_FOR_FROST, 1);
  }
}

/// Frost at the Alpine Crossing freezes the puddle into a pillar the party can
/// climb: it can no longer be pushed or move, sorts itself against the
/// leader each frame, and the cells beside it (and their copy at (20, 18))
/// become walkable. Also run on entry while the pillar is already frozen.
static void AlpineCrossing_FreezePuddle(void) {
  Event_Start();

  // Stop the pillar and install its height-dependent drawing priority.
  Actor_GetById(ACTOR_WATER_FOR_FROST)->priorityFlags &=
      (u8)~ACTOR_PRIORITY_UNDERFOOT;
  Actor_GetById(ACTOR_WATER_FOR_FROST)->collisionFlags &= (u8)~ACTOR_PUSHABLE;
  Actor_GetById(ACTOR_WATER_FOR_FROST)->motionFlags = 0;
#ifdef UBFIX
  Actor_GetById(ACTOR_WATER_FOR_FROST)->update = Pillar_SortAgainstLeader;
#else
  Actor_GetById(ACTOR_WATER_FOR_FROST)->update =
      (void (*)(struct Actor *))Pillar_SortAgainstLeader;
#endif

  MapGrid_CopyCellAttributes(55, 16, 1, 1, 56, 18);
  MapGrid_CopyCellAttributes(55, 16, 1, 1, 20, 18);

  WaitFrames(1);
  GameFlag_Set(FLAG_ALPINE_CROSSING_PUDDLE_FROZEN);
  Actor_SetSpritePriority(ACTOR_WATER_FOR_FROST, 2);
  Event_End();
}

/// Growth at the Alpine Crossing grows the sprout into a vine the party can
/// climb: the two cells above it take the climbable attributes kept at
/// (14, 6). Also run on entry while the vine is already grown.
static void AlpineCrossing_GrowSprout(void) {
  Event_Start();
  MapGrid_CopyCellAttributes(14, 6, 1, 2, 21, 11);
  Actor_GetById(ACTOR_PLANT)->collisionFlags = (u8)~ACTOR_SOLID;
  GameFlag_Set(FLAG_ALPINE_CROSSING_SPROUT_GROWN);
  Event_End();
}

const struct Trigger *Scene_GetTriggers(void) {
  if (gAdventure.sceneId == MAP_ALPINE_CROSSING) {
    return gAlpineCrossing_Triggers;
  }
  return gLamaTemple_Triggers;
}

/// Sets up whichever scene is being entered and runs its arrival events.
s32 Scene_Initialize(void) {
  s32 entrance;

  gFieldWork->transition = SCENE_TRANSITION(TRANSITION_BACKDROP_FADE, 0);
  if (gAdventure.sceneId == MAP_LAMA_TEMPLE) {
    // Lama Temple: the temple's queued sound (SE_AMBIENT_WIND) plays, two monks
    // meditate and the mossy rock stands; the rock's own cell attributes are
    // saved at (21, 73) for Reveal to restore.
    Scene_QueueMusic(SE_AMBIENT_WIND);
    Import_ActorSetAnimation(ACTOR_MEDITATING_MONK, ANIM_MONK_MEDITATE);
    Import_ActorSetAnimation(ACTOR_MONK_SENSING_ANXIETY, ANIM_MONK_MEDITATE);
    Import_ActorSetAnimation(ACTOR_PASSAGE_ROCK, TURNING_ROCK_ANIMATION_MOSSY);
    MapGrid_CopyCellAttributes(21, 9, 1, 1, 21, 73);
    LamaTemple_ShowFalseWall();
    if (GameFlag_Get(FLAG_HSU_RESCUED)) {
      Actor_SetPosition(ACTOR_CHI_MONK, PIXELS(136), PIXELS(256));
      Actor_ArmCallback(ACTOR_CHI_MONK, FACING_EAST, 0);
    }
    entrance = gAdventure.entrance;
    if (entrance == LAMA_TEMPLE_FROM_LACQUER_ROOMS_A_HAMAS_ROOM) {
      GameFlag_Clear(FLAG_SHOW_AREA_NAME);
    } else if (entrance == LAMA_TEMPLE_FROM_ALPINE_CROSSING &&
               !GameFlag_Get(FLAG_KEEP_PARTY_POSITION)) {
      LamaTemple_RunHamasFarewell();
    }
  } else if (gAdventure.sceneId == MAP_ALPINE_CROSSING) {
    // Alpine Crossing: restore the Psynergy puzzles already solved.
    Actor_SetSpriteFlags(Actor_GetById(ACTOR_WATER_FOR_FROST), 0);
    Actor_GetById(ACTOR_WATER_FOR_FROST)->priorityFlags |=
        ACTOR_PRIORITY_UNDERFOOT;
    if (GameFlag_Get(FLAG_ALPINE_CROSSING_PUDDLE_FROZEN)) {
      Import_ActorSetAnimation(ACTOR_WATER_FOR_FROST, PUDDLE_ANIMATION_FROZEN);
      AlpineCrossing_FreezePuddle();
    }
    if (GameFlag_Get(FLAG_ALPINE_CROSSING_SPROUT_GROWN)) {
      Import_ActorSetAnimation(ACTOR_PLANT, SPROUT_ANIMATION_GROWN);
      AlpineCrossing_GrowSprout();
    }
    if (gAdventure.entrance ==
            ALPINE_CROSSING_FROM_ALTIN_PEAK_SOUTH_ENTRANCE_WEST ||
        gAdventure.entrance ==
            ALPINE_CROSSING_FROM_ALTIN_PEAK_SOUTH_ENTRANCE_EAST) {
      GameFlag_Clear(FLAG_SHOW_AREA_NAME);
    }

    // Feizhi is here only once she has set out after Hsu.
    if (!GameFlag_Get(FLAG_FEIZHI_WENT_TO_HSU) &&
        !GameFlag_Get(FLAG_XIAN_HEARD_HSU_TRAPPED) &&
        !GameFlag_Get(FLAG_HSU_RESCUED)) {
      Import_ActorSetPosition(ACTOR_FEIZHI, 0, 0);
    }

    // Arriving from the north after Hama's lesson, before the party has freed
    // Hsu: he has been freed without it and the boulder is gone.
    if (!Event_GetFlag(FLAG_HSU_RESCUED) &&
        GameFlag_Get(FLAG_XIAN_HEARD_HSU_TRAPPED) &&
        gAdventure.entrance == ALPINE_CROSSING_FROM_WORLD_MAP_NORTH) {
      Import_ActorSetPosition(ACTOR_BOULDER, 0, 0);
      GameFlag_Set(FLAG_HSU_RESCUED);
      GameFlag_Set(FLAG_HSU_RESCUED_WITHOUT_PARTY);
      Import_ActorSetPosition(ACTOR_FEIZHI, 0, 0);
    }

    // Once Hsu is free, the cleared rocks open the road.
    if (Event_GetFlag(FLAG_HSU_RESCUED)) {
      MapGrid_CopyCellsTo(54, 21, 53, 21, 1, 2);
      MapGrid_CopyCellAttributes(18, 20, 1, 3, 17, 21);
      MapGrid_CopyCellsTo(44, 18, 43, 17, 1, 1);
      MapGrid_CopyCellAttributes(8, 17, 1, 1, 7, 17);
    }

    // After Hama's lesson, while Hsu is still trapped, Hama and Feizhi stand
    // by him, and the workers from Xian have not come yet.
    if (GameFlag_Get(FLAG_XIAN_HEARD_HSU_TRAPPED) &&
        !GameFlag_Get(FLAG_HSU_RESCUED)) {
      Import_ActorSetPosition(ACTOR_FIRST_WORKER, 0, 0);
      Import_ActorSetPosition(ACTOR_SECOND_WORKER, 0, 0);
      Actor_SetPosition(ACTOR_HAMA, PIXELS(192), PIXELS(264));
      Actor_SetPosition(ACTOR_HSU, PIXELS(164), PIXELS(280));
      Actor_SetPosition(ACTOR_FEIZHI, PIXELS(184), PIXELS(304));
      Actor_FaceDirection(ACTOR_HAMA, FACING_SOUTH + FACING_STEP, 0);
      Actor_FaceDirection(ACTOR_FEIZHI, FACING_NORTHWEST + FACING_STEP, 0);
      Actor_SetScript(ACTOR_HSU, gAlpineCrossing_HsuStruggle);
      Actor_GetById(ACTOR_HSU)->scaleX = -0x10000;
    }
    if (!Event_GetFlag(FLAG_HSU_RESCUED)) {
      Actor_SetPosition(ACTOR_HSU, PIXELS(164), PIXELS(280));
      Actor_SetScript(ACTOR_HSU, gAlpineCrossing_HsuStruggle);
      Actor_GetById(ACTOR_HSU)->scaleX = -0x10000;
    }
    if (gAdventure.entrance ==
            ALPINE_CROSSING_FROM_ALTIN_PEAK_SOUTH_ENTRANCE_EAST &&
        !GameFlag_Get(FLAG_HAMA_REACHED_HSU) &&
        !GameFlag_Get(FLAG_KEEP_PARTY_POSITION) &&
        !GameFlag_Get(FLAG_HSU_RESCUED)) {
      AlpineCrossing_HamaReachesHsu();
    }
  }
  return 0;
}

/// The first worker's actor placement script: draw him behind the
/// background.
s32 gAlpineCrossing_WorkerActorPlacementScript[] = {
    SC_CALL(Actor_DrawBehindScenery),
    SC_END,
};

/// Feizhi's run down the path toward Hsu, walking, then standing.
s32 gAlpineCrossing_FeizhiPath[] = {
    SC_ANIMATION(ANIM_WALK),
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
    SC_ANIMATION(ANIM_STAND),
    SC_END,
};

/// The party leader follows Feizhi most of the way.
s32 gAlpineCrossing_LeaderPath[] = {
    SC_ANIMATION(ANIM_WALK),
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
    SC_ANIMATION(ANIM_STAND),
    SC_END,
};

/// Hsu, pinned under the boulder, struggles now and then.
s32 gAlpineCrossing_HsuStruggle[] = {
    SC_ANIMATION(ANIM_STAND),
    SC_ANIMATION(ANIM_HSU_STRUGGLE),
    SC_WAIT(240),
    SC_ANIMATION(ANIM_STAND),
    SC_ANIMATION(ANIM_HSU_STRUGGLE),
    SC_WAIT(60),
    SC_ANIMATION(ANIM_STAND),
    SC_ANIMATION(ANIM_HSU_STRUGGLE),
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

struct SceneEntrance gLamaTemple_Entrances[] = {
    {ENTRANCE_DEFAULT, CONDITION_ALWAYS, 496, 0, 216, FACING_WEST, 0, -1, -1,
     -1, -1, 0},
    {LAMA_TEMPLE_FROM_WORLD_MAP, CONDITION_ALWAYS, 216, 0, 520, FACING_NORTH, 0,
     -1, -1, -1, -1, 0},
    {LAMA_TEMPLE_FROM_LACQUER_ROOMS_A_HAMAS_ROOM, CONDITION_ALWAYS, 136, 0, 168,
     FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {LAMA_TEMPLE_FROM_ALPINE_CROSSING, CONDITION_ALWAYS, 144, 0, 200,
     FACING_NORTH, 0, -1, -1, -1, -1, 0},
    {LAMA_TEMPLE_AFTER_DEFEAT, CONDITION_ALWAYS, 232, 0, 352, FACING_SOUTH, 0,
     -1, -1, -1, -1, 0},
    {SCENE_TABLE_END},
};

struct SceneEntrance gAlpineCrossing_Entrances[] = {
    {ENTRANCE_DEFAULT, CONDITION_ALWAYS, 496, 0, 216, FACING_WEST, 0, -1, -1,
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
    {SCENE_TABLE_END},
};

u32 gAlpineCrossingLamaTemple_Exits[] = {
    SCENE_EXITS(LamaTemple),
    SCENE_EXIT(LAMA_TEMPLE_TO_WORLD_MAP, WorldMap, WORLD_MAP_FROM_LAMA_TEMPLE),
    SCENE_EXIT(LAMA_TEMPLE_TO_LACQUER_ROOMS_A_HAMAS_ROOM, LacquerRoomsA,
               LACQUER_ROOMS_A_HAMAS_ROOM_FROM_LAMA_TEMPLE),
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
struct ActorPlacement gLamaTemple_ActorPlacements[] = {
    { ACTOR_GARET, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        0, 0, 0, FACING_NORTH, TALK_FACING_KEEP, 0 },
    { ACTOR_IVAN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        0, 0, 0, FACING_NORTH, TALK_FACING_KEEP, 0 },
    { ACTOR_MIA, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        0, 0, 0, FACING_NORTH, TALK_FACING_KEEP, 0 },
    { SPRITE_HAMA, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        0, 0, 0, FACING_NORTH, TALK_FACING_PARTY_AND_BACK, 0 },
    { SPRITE_MONK_IN_ORANGE_ROBE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(168), 0, PIXELS(336), FACING_EAST, TALK_FACING_PARTY, 0 },
    { SPRITE_MONK_IN_ORANGE_ROBE, CONDITION_ALWAYS, MOVEMENT_TYPE_WANDER,
        PIXELS(216), 0, PIXELS(240), FACING_SOUTH, TALK_FACING_PARTY, 0 },
    { SPRITE_MONK_IN_ORANGE_ROBE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(312), 0, PIXELS(320), FACING_SOUTH, TALK_FACING_KEEP, 0 },
    { SPRITE_MONK_IN_ORANGE_ROBE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(344), 0, PIXELS(320), FACING_SOUTH, TALK_FACING_KEEP, 0 },
    { SPRITE_MONK_IN_ORANGE_ROBE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(120), 0, PIXELS(176), FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, 0 },
    { SPRITE_TURNING_ROCK, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(344), 0, PIXELS(152), FACING_SOUTH, TALK_FACING_KEEP, 0 },
    { SPRITE_MAN_IN_TALL_HAT, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(232), 0, PIXELS(328), FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, 0 },
    { SCENE_TABLE_END },
};

struct ActorPlacement gAlpineCrossing_ActorPlacements[] = {
    { SPRITE_HAMA, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        0, 0, 0, FACING_NORTH, TALK_FACING_PARTY_AND_BACK, 0 },
    { SPRITE_HSU, CONDITION_ALWAYS, (s32)gAlpineCrossing_HsuStruggle,
        0, 0, 0, FACING_NORTH, TALK_FACING_KEEP, 0 },
    { SPRITE_FEIZHI, FLAG_HSU_RESCUED, MOVEMENT_TYPE_STAND,
        PIXELS(296), 0, PIXELS(344), FACING_WEST, TALK_FACING_PARTY_AND_BACK, 0 },
    { SPRITE_ROUND_BOULDER, FLAG_HSU_RESCUED, MOVEMENT_TYPE_STAND,
        PIXELS(152), 0, PIXELS(276), FACING_WEST, TALK_FACING_PARTY, 0 },
    { SPRITE_YOUTH_WITH_HEADBAND, IF_FLAG_SET(FLAG_XIAN_HEARD_HSU_TRAPPED), (s32)gAlpineCrossing_WorkerActorPlacementScript,
        PIXELS(456), 0, PIXELS(200), FACING_SOUTH, TALK_FACING_PARTY, 0 },
    { SPRITE_YOUTH_WITH_TIED_HAIR, IF_FLAG_SET(FLAG_XIAN_HEARD_HSU_TRAPPED), MOVEMENT_TYPE_STAND,
        PIXELS(184), 0, PIXELS(264), FACING_SOUTH, TALK_FACING_PARTY, 0 },
    { SPRITE_PUDDLE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(328), 0, PIXELS(296), FACING_SOUTH, TALK_FACING_PARTY, 0 },
    { SPRITE_SPROUT, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(344), 0, PIXELS(194), FACING_SOUTH, TALK_FACING_PARTY, 0 },
    { SCENE_TABLE_END },
};

struct Trigger gLamaTemple_Triggers[] = {
    { TRIGGER_EXIT, 1, CONDITION_ALWAYS, LAMA_TEMPLE_TO_WORLD_MAP },
    { STEP_TRIGGER_WALKING_INTO(FACING_NORTH), CELL_BEHAVIOR_LAMA_TEMPLE_DOOR, CONDITION_ALWAYS, EVENT(LamaTemple_EnterHamasRoom) },
    { TRIGGER_TALK, ACTOR_GREETING_MONK, FLAG_HSU_RESCUED, MSGNO(MsgAlpineCrossingLamaTempleThisIsLamaTempleWelcomeTravelers) },
    { TRIGGER_TALK, ACTOR_MONK_ASKING_ABOUT_THE_EAST, FLAG_HSU_RESCUED, EVENT(LamaTemple_TalkToMonkAskingAboutTheEast) },
    { TRIGGER_TALK, ACTOR_MEDITATING_MONK, FLAG_HSU_RESCUED, EVENT(LamaTemple_TalkToMeditatingMonk) },
    { TRIGGER_TALK, ACTOR_MONK_SENSING_ANXIETY, FLAG_HSU_RESCUED, MSGNO(MsgAlpineCrossingLamaTempleYouWhoSpeakToMeI) },
    { TRIGGER_TALK, ACTOR_CHI_MONK, FLAG_HSU_RESCUED, EVENT(LamaTemple_TalkToChiMonk) },
    { TRIGGER_TALK, ACTOR_GREETING_MONK, CONDITION_ALWAYS, MSGNO(MsgAlpineCrossingLamaTempleItIsVeryDangerousToCross) },
    { TRIGGER_TALK, ACTOR_MONK_ASKING_ABOUT_THE_EAST, CONDITION_ALWAYS, MSGNO(MsgAlpineCrossingLamaTempleTheEvilDesertIsHotterThan) },
    { TRIGGER_TALK, ACTOR_MEDITATING_MONK, CONDITION_ALWAYS, MSGNO(MsgAlpineCrossingLamaTempleIBelieveItIsPossibleTo) },
    { TRIGGER_TALK, ACTOR_MONK_SENSING_ANXIETY, CONDITION_ALWAYS, MSGNO(MsgAlpineCrossingLamaTempleIWillBeMostThankfulOnce) },
    { TRIGGER_TALK, ACTOR_CHI_MONK, CONDITION_ALWAYS, MSGNO(MsgAlpineCrossingLamaTempleWhenYourTemperatureReachesItsMaximum) },
    { TRIGGER_TALK, ACTOR_TRAVELING_HEALER, CONDITION_ALWAYS, EVENT(LamaTemple_TalkToHealer) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_GREETING_MONK, FLAG_HSU_RESCUED, MSGNO(MsgAlpineCrossingLamaTempleTheySeemToBeAnxiousThey) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_MONK_ASKING_ABOUT_THE_EAST, FLAG_HSU_RESCUED, MSGNO(MsgAlpineCrossingLamaTempleTheySayAPersonCanOvercome) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_MEDITATING_MONK, FLAG_HSU_RESCUED, MSGNO(MsgAlpineCrossingLamaTempleIAmHungry) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_MONK_SENSING_ANXIETY, FLAG_HSU_RESCUED, MSGNO(MsgAlpineCrossingLamaTempleISenseImpureThoughtsFromThe) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_CHI_MONK, FLAG_HSU_RESCUED, MSGNO(MsgAlpineCrossingLamaTempleChiIsButAFragmentOf) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_GREETING_MONK, CONDITION_ALWAYS, MSGNO(MsgAlpineCrossingLamaTempleICannotCrossTheDesertIt) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_MONK_ASKING_ABOUT_THE_EAST, CONDITION_ALWAYS, MSGNO(MsgAlpineCrossingLamaTempleTheEvilInTheDesertDistorts) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_MEDITATING_MONK, CONDITION_ALWAYS, MSGNO(MsgAlpineCrossingLamaTempleHowNiceItWouldBeIf) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_MONK_SENSING_ANXIETY, CONDITION_ALWAYS, MSGNO(MsgAlpineCrossingLamaTempleMasterHamaWillBeVerySad) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_CHI_MONK, CONDITION_ALWAYS, MSGNO(MsgAlpineCrossingLamaTempleWithRevealYouWillFindOases) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_TRAVELING_HEALER, CONDITION_ALWAYS, MSGNO(MsgAlpineCrossingLamaTempleForSomeReasonThesePeopleGive) },
    { PSYNERGY_ANYWHERE(ABILITY_REVEAL, PSYNERGY_PHASE_ACTS), 0, CONDITION_ALWAYS, EVENT(LamaTemple_RevealPassage) },
    { PSYNERGY_ANYWHERE(ABILITY_REVEAL, PSYNERGY_PHASE_FADES), 0, CONDITION_ALWAYS, EVENT(LamaTemple_ShowFalseWall) },
    { EXAMINE_TRIGGER(EXAMINE_SILENT), CELL_BEHAVIOR_LAMA_TEMPLE_WALL_WRITING, CONDITION_ALWAYS, EVENT(LamaTemple_ReadWallWriting) },
    { EXAMINE_TRIGGER_FACING(FACING_NORTH), CELL_BEHAVIOR_LAMA_TEMPLE_PSYNERGY_STONE, FLAG_LAMA_TEMPLE_PSYNERGY_STONE, FIND_PSYNERGY_STONE },
    { EXAMINE_TRIGGER_FACING(FACING_NORTHEAST), CELL_BEHAVIOR_LAMA_TEMPLE_PSYNERGY_STONE, FLAG_LAMA_TEMPLE_PSYNERGY_STONE, FIND_PSYNERGY_STONE },
    { EXAMINE_TRIGGER_FACING(FACING_NORTHWEST), CELL_BEHAVIOR_LAMA_TEMPLE_PSYNERGY_STONE, FLAG_LAMA_TEMPLE_PSYNERGY_STONE, FIND_PSYNERGY_STONE },
    { EXAMINE_TRIGGER_FACING(FACING_WEST), CELL_BEHAVIOR_LAMA_TEMPLE_PSYNERGY_STONE, FLAG_LAMA_TEMPLE_PSYNERGY_STONE, FIND_PSYNERGY_STONE },
    { EXAMINE_TRIGGER(EXAMINE_CHEST), CELL_BEHAVIOR_LAMA_TEMPLE_CHEST, FLAG_LAMA_TEMPLE_WATER_OF_LIFE, FIND_ITEM(ITEM_WATER_OF_LIFE) },
    { TRIGGERS_END },
};

struct Trigger gAlpineCrossing_Triggers[] = {
    { TRIGGER_EXIT, 1, CONDITION_ALWAYS, ALPINE_CROSSING_TO_WORLD_MAP_EAST },
    { TRIGGER_EXIT, 2, CONDITION_ALWAYS, ALPINE_CROSSING_TO_WORLD_MAP_NORTH },
    { TRIGGER_EXIT, 3, CONDITION_ALWAYS, ALPINE_CROSSING_TO_WORLD_MAP_WEST },
    { TRIGGER_EXIT, 5, CONDITION_ALWAYS, ALPINE_CROSSING_TO_ALTIN_PEAK_SOUTH_ENTRANCE_WEST },
    { TRIGGER_EXIT, 4, CONDITION_ALWAYS, ALPINE_CROSSING_TO_ALTIN_PEAK_SOUTH_ENTRANCE_EAST },
    { TRIGGER_STEP, CELL_BEHAVIOR_ALPINE_CROSSING_FEIZHI_ARRIVAL, FLAG_FEIZHI_SAW_ROCKSLIDE, EVENT(AlpineCrossing_FeizhiArrives) },
    { TRIGGER_TALK, ACTOR_FEIZHI, FLAG_XIAN_HEARD_HSU_TRAPPED, EVENT(AlpineCrossing_TalkToFeizhi) },
    { TRIGGER_TALK, ACTOR_HAMA, CONDITION_ALWAYS, MSGNO(MsgAlpineCrossingLamaTempleWellIsaacAdeptsYouCameTo) },
    { TRIGGER_TALK, ACTOR_HSU, CONDITION_ALWAYS, MSGNO(MsgAlpineCrossingLamaTempleIAmTrappedBetweenTheseTwo) },
    { TRIGGER_TALK, ACTOR_FEIZHI, CONDITION_ALWAYS, MSGNO(MsgAlpineCrossingLamaTempleBeStrongHsuWeWillHelp) },
    { TRIGGER_TALK, ACTOR_FIRST_WORKER, CONDITION_ALWAYS, MSGNO(MsgAlpineCrossingLamaTempleWeMovedTheRocksNowPeople) },
    { TRIGGER_TALK, ACTOR_SECOND_WORKER, CONDITION_ALWAYS, MSGNO(MsgAlpineCrossingLamaTempleManyPeopleFromXianHelpedMove) },
    { PSYNERGY_ON_ACTOR(ABILITY_LIFT), ACTOR_BOULDER, FLAG_HSU_RESCUED, EVENT(AlpineCrossing_LiftBoulder) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_FEIZHI, FLAG_XIAN_HEARD_HSU_TRAPPED, MSGNO(MsgAlpineCrossingLamaTempleHsuIsInDangerICan) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_HAMA, CONDITION_ALWAYS, MSGNO(MsgAlpineCrossingLamaTempleHowShouldWeMoveTheBoulders) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_HSU, CONDITION_ALWAYS, MSGNO(MsgAlpineCrossingLamaTempleIWantToScreamButFeizhi) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_FEIZHI, CONDITION_ALWAYS, MSGNO(MsgAlpineCrossingLamaTempleHsuTiresHeLooksExhaustedI) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_FIRST_WORKER, CONDITION_ALWAYS, MSGNO(MsgAlpineCrossingLamaTempleWeClearedTheWayButWhat) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_SECOND_WORKER, CONDITION_ALWAYS, MSGNO(MsgAlpineCrossingLamaTempleIAlmostHurtMyBackIt) },
    { PSYNERGY_ANYWHERE(ABILITY_REVEAL, PSYNERGY_PHASE_ACTS), 0, CONDITION_ALWAYS, EVENT(AlpineCrossing_RevealPath) },
    { PSYNERGY_ANYWHERE(ABILITY_REVEAL, PSYNERGY_PHASE_FADES), 0, CONDITION_ALWAYS, EVENT(AlpineCrossing_HidePath) },
    // Frost's first phase, as it reaches the puddle
    { PSYNERGY_PHASE_ON_ACTOR(ABILITY_FROST, PSYNERGY_PHASE_BEFORE_CAST), ACTOR_WATER_FOR_FROST, FLAG_ALPINE_CROSSING_PUDDLE_FROZEN, EVENT(AlpineCrossing_PrepareFrost) },
    { PSYNERGY_ON_ACTOR(ABILITY_FROST), ACTOR_WATER_FOR_FROST, FLAG_ALPINE_CROSSING_PUDDLE_FROZEN, EVENT(AlpineCrossing_FreezePuddle) },
    { PSYNERGY_ON_ACTOR(ABILITY_GROWTH), ACTOR_PLANT, FLAG_ALPINE_CROSSING_SPROUT_GROWN, EVENT(AlpineCrossing_GrowSprout) },
    { EXAMINE_TRIGGER(EXAMINE_SILENT), CELL_BEHAVIOR_ALPINE_CROSSING_SIGNPOST, CONDITION_ALWAYS, EVENT(AlpineCrossing_ReadSignpost) },
    { EXAMINE_TRIGGER(EXAMINE_CHEST), CELL_BEHAVIOR_ALPINE_CROSSING_CHEST, FLAG_ALPINE_CROSSING_POWER_BREAD, FIND_ITEM(ITEM_POWER_BREAD) },
    { TRIGGERS_END },
};
// clang-format on
