/// @file
/// The Alpine Crossing (SceneId_AlpineCrossing) and Lama Temple
/// (SceneId_LamaTemple), two places on Silk Road west of Xian that share this
/// overlay. The getters pick each scene's tables by gMainRoutineWork.next, and
/// the two share one exit table.
///
/// The Alpine Crossing is a valley of cliffs and ledges between Xian and Altin.
/// Silk Road comes in from the World Map on the east, a road climbs north to
/// Altin, and the way west to Lama Temple is buried under a rockslide (the
/// rocks at cells (17, 21) to (17, 23)). A signpost beside the road west of
/// the east entrance reads `North: Altin Mine West: Lama Temple`.
///
/// If the party has seen Feizhi leave Xian (FLAG_FEIZHI_WENT_TO_HSU, set in
/// fuchin_temple.c), she runs in behind them a few steps inside the east
/// entrance (AlpineCrossing_FeizhiArrives), cries out at the rockslide and
/// runs down past the puddle to it, where she waits until Hama's lesson. On
/// this first visit the party can freeze the puddle at the foot of the gully
/// into an ice pillar with Frost, grow the sprout below the signpost into a
/// vine with Growth, and climb round to the chest of Power Bread in the
/// south-east corner. These two puzzles are kept only while the party stays
/// (scene temp flags). Then the road north leads on to Altin.
///
/// Lama Temple lies west, at the head of a wooded valley: a path from the World
/// Map on the south crosses a bridge, where a traveling healer stands, and
/// climbs to the temple building with five monks about it. They have one set
/// of lines and thoughts until Hsu is freed (FLAG_HSU_RESCUED) and another
/// after. A defeat anywhere here brings the party back in front of the healer
/// (LAMA_TEMPLE_AFTER_DEFEAT).
///
/// The temple's sliding door (LamaTemple_EnterHamasRoom) leads into Hama's
/// room, in LacquerRoomsA (LACQUER_ROOMS_A_HAMAS_ROOM_FROM_LAMA_TEMPLE, in
/// fuchin_temple.c), where she teaches Ivan Reveal and Feizhi brings word that
/// Hsu is trapped (FLAG_XIAN_HEARD_HSU_TRAPPED). Coming back out, the area name
/// is not shown again. East of the waterfall, writing on the cliff hints at
/// Reveal, and Reveal takes away the mossy rock beneath it to show a chest of
/// Water of Life (LamaTemple_RevealChest). The temple grounds also hold a
/// Psynergy Stone.
///
/// Back at the Alpine Crossing after the lesson, Hama and Feizhi stand by Hsu,
/// who is pinned under a boulder west of the rockslide. Reveal on the cliff
/// west of him shows the mouth of a small cave (AlpineCrossing_RevealPath). It
/// is part of Altin Peak's AltinPeakSouthEntrance, whose other mouth opens
/// just north-east of Hsu. The international editions call that cave
/// Alpine Crossing too, so coming back out of it the area name is not shown
/// again. The first time the party comes out beside Hsu, the camera finds him
/// and Hama asks if he is hurt (AlpineCrossing_HamaReachesHsu). Lift on the
/// boulder (AlpineCrossing_LiftBoulder) raises it while Hama and Feizhi pull
/// him free, and the party is taken back to Lama Temple, where Hama says
/// farewell outside her door (LamaTemple_RunHamasFarewell).
///
/// If the party comes back by the road from the north instead
/// (ALPINE_CROSSING_FROM_WORLD_MAP_NORTH), Hsu has been freed without them
/// (FLAG_HSU_RESCUED_WITHOUT_PARTY, which lamakan_desert.c can set too), and
/// Hama and Feizhi are gone. Either way, once he is free the rockslide has been
/// cleared and two workers from Xian stand by the open road.

#include "ability_ids.h"
#include "flags.h"
#include "item_ids.h"
#include "types.h"
#include "battle/battle.h"
#include "debug/debug.h"
#include "field/event.h"
#include "field/event_commands.h"
#include "field/field.h"
#include "field/field_effect.h"
#include "field/scene_tables.h"
#include "field/script_operands.h"
#include "game/game.h"
#include "graphics/graphics.h"
#include "menu/menu.h"
#include "sound/sound.h"
#include "system/main_routine_work.h"
#include "system/system.h"
#include "text/msg_ids.h"

extern s32 gAlpineCrossing_FeizhiPath[];
extern s32 gAlpineCrossing_LeaderPath[];
extern s32 gAlpineCrossing_HsuStruggle[];
extern u16 gLamaTemple_DoorOpenCells[];
extern u16 gLamaTemple_DoorCloseCells[];
extern struct SceneEntrance gLamaTemple_Entrances[];
extern struct SceneEntrance gAlpineCrossing_Entrances[];
extern u32 gAlpineCrossingLamaTemple_Exits[];
extern struct ActorPlacement gLamaTemple_ActorPlacements[];
extern struct ActorPlacement gAlpineCrossing_ActorPlacements[];
extern struct Trigger gLamaTemple_Triggers[];
extern struct Trigger gAlpineCrossing_Triggers[];

/// The temple's Psynergy Stone. It is an area temp flag, so it refills once the
/// party has been to another area.
#define FLAG_LAMA_TEMPLE_PSYNERGY_STONE FLAG_AREA_TEMP_50

/// The overlay's two scenes.
#define MAP_ALPINE_CROSSING (SCENE_ALPINE_CROSSING)
#define MAP_LAMA_TEMPLE (SCENE_LAMA_TEMPLE)

/// Lama Temple's actors. Garet, Ivan and Mia are placed in their own party
/// slots for Hama's farewell. The others count from ACTOR_FIRST_PLACED.
enum LamaTempleActorId {
  /// Hama, back in her temple for her farewell.
  ACTOR_TEMPLE_HAMA = ACTOR_FIRST_PLACED,
  ACTOR_GREETING_MONK,
  ACTOR_MONK_ASKING_ABOUT_THE_EAST,
  ACTOR_MEDITATING_MONK,
  ACTOR_MONK_SENSING_ANXIETY,
  ACTOR_CHI_MONK,
  /// The mossy rock east of the waterfall that Reveal takes away to show the
  /// chest under it (LamaTemple_RevealChest).
  ACTOR_MOSSY_ROCK,
  ACTOR_TRAVELING_HEALER
};

/// Alpine Crossing's actors.
enum AlpineCrossingActorId {
  ACTOR_HAMA = ACTOR_FIRST_PLACED,
  ACTOR_HSU,
  ACTOR_FEIZHI,
  ACTOR_BOULDER,
  /// A young man in a white headband, one of the people of Xian who moved the
  /// rocks.
  ACTOR_FIRST_WORKER,
  /// A young man with his hair tied back.
  ACTOR_SECOND_WORKER,
  /// The water Frost freezes into a pillar (AlpineCrossing_FreezePuddle).
  ACTOR_WATER_FOR_FROST,
  /// The plant Growth grows into a vine (AlpineCrossing_GrowSprout).
  ACTOR_PLANT
};

/// Alpine Crossing's Psynergy puzzles, kept while the party stays.
enum {
  /// Frost has frozen the puddle into an ice pillar.
  FLAG_ALPINE_CROSSING_PUDDLE_FROZEN = FLAG_SCENE_TEMP_00,
  /// Growth has grown the sprout into a vine the party can climb.
  FLAG_ALPINE_CROSSING_SPROUT_GROWN = FLAG_SCENE_TEMP_01
};

/// Animations of these scenes' own sprites beyond enum ActorAnimation,
/// named by when the events play them.
enum {
  ANIM_HAMA_PULL = 5,
  ANIM_FEIZHI_PULL = 6,
  /// Drops into a crouch, then holds a half crouch with her head bowed
  /// (feizhi.png's frames 33 and 36).
  ANIM_FEIZHI_CROUCH = 5,
  /// SPRITE_TURNING_ROCK's mossy rock (turning_rock.png's frame 1). Its first
  /// animations show picture 0, a purple glow.
  ANIM_TURNING_ROCK_MOSSY = 2,
  ANIM_HSU_STRUGGLE = 5,
  ANIM_MONK_MEDITATE = 5
};

/// The cell behaviors Lama Temple's triggers answer.
enum LamaTempleCellBehavior {
  /// The path out to the World Map at the south edge.
  CELL_BEHAVIOR_LAMA_TEMPLE_SOUTH_ROAD = 1,
  CELL_BEHAVIOR_LAMA_TEMPLE_DOOR = 2,
  CELL_BEHAVIOR_LAMA_TEMPLE_WALL_WRITING = 5,
  CELL_BEHAVIOR_LAMA_TEMPLE_CHEST = 100,
  CELL_BEHAVIOR_LAMA_TEMPLE_PSYNERGY_STONE = 110
};

/// The cell behaviors Alpine Crossing's triggers answer.
enum AlpineCrossingCellBehavior {
  /// Silk Road at the east edge, back toward Xian.
  CELL_BEHAVIOR_ALPINE_CROSSING_EAST_ROAD = 1,
  /// The road at the top of the valley, toward Altin.
  CELL_BEHAVIOR_ALPINE_CROSSING_NORTH_ROAD = 2,
  /// The road at the west edge, toward Lama Temple.
  CELL_BEHAVIOR_ALPINE_CROSSING_WEST_ROAD = 3,
  /// The cave mouth north-east of Hsu.
  CELL_BEHAVIOR_ALPINE_CROSSING_EAST_CAVE_MOUTH = 4,
  /// The cave mouth Reveal shows in the cliff west of Hsu
  /// (AlpineCrossing_RevealPath).
  CELL_BEHAVIOR_ALPINE_CROSSING_WEST_CAVE_MOUTH = 5,
  CELL_BEHAVIOR_ALPINE_CROSSING_SIGNPOST = 7,
  CELL_BEHAVIOR_ALPINE_CROSSING_FEIZHI_ARRIVAL = 10,
  CELL_BEHAVIOR_ALPINE_CROSSING_CHEST = 100
};

/// The door of Hama's room, three cells wide and two high. Each step copies the
/// next door picture onto it for five frames.
enum { DOOR_X = 67, DOOR_Y = 6 };

/// The chest of Water of Life under the mossy rock: its treasure spot is the
/// chest cell's own.
enum { TREASURE_SPOT_WATER_OF_LIFE = CELL_BEHAVIOR_LAMA_TEMPLE_CHEST };

/// Draws the actor behind the background, on sprite priority 3 with automatic
/// priority off.
static s32 Actor_DrawBehindScenery(struct Actor *actor) {
  actor->priorityFlags &= ~ACTOR_PRIORITY_AUTOMATIC;
  actor->sprite->image.priority = 3;
  actor->sprite->shadow.priority = 3;
  return 0;
}

/// Update routine that turns the party leader toward Feizhi while she runs
/// past.
static UNREAD_RESULT AlpineCrossing_LeaderFaceFeizhi(struct Actor *leader) {
  struct Actor *feizhi;

  feizhi = Actor_Get(ACTOR_FEIZHI);
  leader->facing = Trig_Atan(feizhi->z.fixed - leader->z.fixed,
                             feizhi->x.fixed - leader->x.fixed);
#ifndef UBFIX
  return 0;
#endif
}

const struct SceneEntrance *Scene_GetEntrances(void) {
  if (gMainRoutineWork.next == MAP_ALPINE_CROSSING) {
    return gAlpineCrossing_Entrances;
  }
  return gLamaTemple_Entrances;
}

const struct SceneExitRegion *Scene_GetExitRegions(void) { return 0; }

/// Both scenes share one exit table.
const u32 *Scene_GetExits(void) { return gAlpineCrossingLamaTemple_Exits; }

const struct ActorPlacement *Scene_GetActorPlacements(void) {
  if (gMainRoutineWork.next == MAP_ALPINE_CROSSING) {
    return gAlpineCrossing_ActorPlacements;
  }
  return gLamaTemple_ActorPlacements;
}

/// A monk in Lama Temple, until Hsu is freed, asks whether the party comes from
/// the east.
static void LamaTemple_TalkToMonkAskingAboutTheEast(void) {
  Event_Start();
  Event_SetMessageId(MsgMountainYoungWarriorsDoComeFrom);
  // "Young warriors, do you come from the east?"
  Event_OpenMessage(ACTOR_MONK_ASKING_ABOUT_THE_EAST, 0);

  if (Event_AskYesNoWithActorAnimation(ACTOR_PARTY_LEADER, 0) == 1) {
    Event_Wait(20);
    // "Then you must have come across Lamakan Desert..."
    Event_ShowMessage(ACTOR_MONK_ASKING_ABOUT_THE_EAST, 0);
  } else {
    Event_SkipMessages(1);
    // "Did you stop in Xian along the way?"
    // Yes: "Master Feh, in Xian, is very close to our Master Hama...."
    // No: "Then it is good that you have come here. Feh's kung fu is..."
    Event_AskYesNo(ACTOR_MONK_ASKING_ABOUT_THE_EAST, 0);
  }

  Event_End();
}

/// The meditating monk explains meditation. He's in Lama Temple until Hsu is
/// freed, and first asks whether the party knows what meditation is.
static void LamaTemple_TalkToMeditatingMonk(void) {
  Event_Start();
  Event_SetMessageId(MsgMountainDoKnowMeditation);
  // "Do you know what meditation is?"
  // Yes: "It allows you to find peace by closing your eyes... and..."
  // No: "It is training in which you clear your mind of all..."
  Event_AskYesNo(ACTOR_MEDITATING_MONK, 0);
  Event_End();
}

/// A monk in Lama Temple praises Master Hama as the greatest master of Chi,
/// until Hsu is freed.
static void LamaTemple_TalkToChiMonk(void) {
  Event_Start();
  Event_SetMessageId(MsgMountainDidKnowMasterHamaGreatest);
  // "Did you know that Master Hama is the greatest master of Chi?"
  // Yes: "You do? Then you must have already met Master Feh in Xian,..."
  // No: "Then today, you shall see that Master Hama is truly the..."
  Event_AskYesNo(ACTOR_CHI_MONK, 0);
  Event_End();
}

/// Walking north into the temple's sliding door (cell (8, 10)) slides it open
/// with a thump, and Isaac steps through into Hama's room.
static void LamaTemple_EnterHamasRoom(void) {
  Sound_Play(SE_THUMP);
  MapGrid_AnimateCells(gLamaTemple_DoorOpenCells, DOOR_X, DOOR_Y);
  Actor_Get(ACTOR_PARTY_LEADER)->motionFlags = 0;
  Actor_SetSpeed(ACTOR_PARTY_LEADER, 0.8);
  gFieldWork->transition = SCENE_TRANSITION(TRANSITION_BACKDROP_FADE, 0);
  Actor_SetAnimation(ACTOR_PARTY_LEADER, ANIM_WALK);
  Actor_SetDestinationOffset(ACTOR_PARTY_LEADER, 0, -16);
  Event_Wait(16);
  Import_EventSetExitRequest(LAMA_TEMPLE_TO_LACQUER_ROOMS_A_HAMAS_ROOM);
}

/// Hama thanks the party in front of her temple door once they have freed Hsu,
/// Mia teases Garet over whose idea it was to help, and Hama goes back inside;
/// Ivan then decides not to ask her what she would not say to him. The party is
/// brought here straight from the rescue at the Alpine Crossing.
static void LamaTemple_RunHamasFarewell(void) {
  Event_Start();
  Actor_SetPosition(ACTOR_TEMPLE_HAMA, 136, 168);
  Actor_FaceDirection(ACTOR_TEMPLE_HAMA, FACING_SOUTH_SOUTHWEST, 0);
  Actor_SetPosition(ACTOR_PARTY_LEADER, 144, 200);
  Actor_SetPosition(ACTOR_GARET, 160, 192);
  Actor_SetPosition(ACTOR_IVAN, 128, 200);
  Actor_SetPosition(ACTOR_MIA, 112, 192);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 0);
  Actor_FaceDirection(ACTOR_GARET, FACING_NORTHWEST, 0);
  Actor_FaceDirection(ACTOR_IVAN, FACING_NORTH, 0);
  Actor_FaceDirection(ACTOR_MIA, FACING_NORTHEAST, 0);
  gFieldWork->transition =
      SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_SHAPE);
  Event_StartScreenTransition();
  Event_WaitTransitionFrames();
  Event_Wait(60);

  // "Adepts, let me thank you again."
  Actor_SetAnimationAndWait(ACTOR_TEMPLE_HAMA, ANIM_NOD);
  Event_Wait(20);
  Event_SetMessageId(MsgMountainAdeptsLetMeThankAgain);
  // "Adepts, let me thank you again. I am truly grateful."
  Event_ShowMessageThenWait(ACTOR_TEMPLE_HAMA, 0, 20);
  Actor_SetAnimation(ACTOR_PARTY_LEADER, ANIM_NOD);
  Actor_SetAnimation(ACTOR_GARET, ANIM_NOD);
  Actor_SetAnimation(ACTOR_IVAN, ANIM_NOD);
  Actor_SetAnimationAndWait(ACTOR_MIA, ANIM_NOD);
  Event_Wait(20);
  Actor_SetAnimationAndWait(ACTOR_TEMPLE_HAMA, ANIM_SHAKE_HEAD);
  Event_Wait(20);
  // "I did not expect you to come and aid us."
  Event_ShowMessageThenWait(ACTOR_TEMPLE_HAMA, 0, 20);
  Actor_SetAnimationAndWait(ACTOR_MIA, ANIM_SHAKE_HEAD);
  Event_Wait(20);
  // "No, Master Hama... We are the ones who should be grateful."
  Event_ShowMessageThenWait(ACTOR_MIA, 0, 20);
  Actor_RunRepeatedMotion(ACTOR_IVAN, 2);
  Event_Wait(20);
  // "Thank you for passing the power of Reveal on to me... But..."
  Event_ShowMessageThenWait(ACTOR_IVAN, 0, 20);
  Actor_RunRepeatedMotion(ACTOR_TEMPLE_HAMA, 2);
  Actor_FaceDirection(ACTOR_TEMPLE_HAMA, FACING_SOUTH_SOUTHWEST, 20);
  // "What is it, Ivan?"
  Event_ShowMessageThenWait(ACTOR_TEMPLE_HAMA, 0, 20);
  Actor_ShowEmote(ACTOR_IVAN, EMOTE_IN_FRONT | EMOTE_WINCE, 60);
  Event_Wait(120);

  // Garet claims that helping was his idea and asks the leader to agree.
  Actor_RunRepeatedMotion(ACTOR_GARET, 2);
  Event_Wait(20);
  Actor_FaceDirection(ACTOR_GARET, FACING_SOUTHWEST, 20);
  // "Well, really, I was the one who suggested we help you, right?"
  Event_OpenMessage(ACTOR_GARET, 0);
  if (Event_AskYesNoWithActorAnimation(ACTOR_PARTY_LEADER, 0) == 0) {
    Event_Wait(20);
    Actor_FaceDirection(ACTOR_MIA, FACING_EAST, 20);
    Actor_ShowEmote(ACTOR_MIA, EMOTE_IN_FRONT | EMOTE_QUESTION, 60);
    // "What? Didn't you want us to hurry on to the desert, Garet?"
    Event_ShowMessageThenWait(ACTOR_MIA, 0, 20);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTHEAST, 0);
    Actor_FaceDirection(ACTOR_IVAN, FACING_NORTHEAST, 0);
    Actor_StartRepeatedMotion(ACTOR_GARET, 2);
    Actor_SetAttachedEffect(ACTOR_GARET,
                            ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
    Event_Wait(60);
    Actor_FaceDirection(ACTOR_GARET, FACING_WEST, 20);
    // "Did I, Mia? Did I really? I think you must be mistaken...."
    Event_ShowMessageThenWait(ACTOR_GARET, 0, 20);
    Event_SkipMessages(2);
  } else {
    Event_SkipMessages(2);
    Event_Wait(20);
    Actor_FaceDirection(ACTOR_MIA, FACING_EAST, 20);
    Actor_SetAnimationAndWait(ACTOR_MIA, ANIM_NOD);
    Event_Wait(20);
    // "No, Garet... You wanted us to get moving!"
    Event_ShowMessageThenWait(ACTOR_MIA, 0, 20);
    Actor_ShowEmote(ACTOR_GARET, EMOTE_IN_FRONT | EMOTE_WINCE, 60);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTHEAST, 0);
    Actor_FaceDirection(ACTOR_IVAN, FACING_NORTHEAST, 0);
    // "Isaac, Mia... Don't make me look like a fool..."
    Event_ShowMessageThenWait(ACTOR_GARET, 0, 20);
  }

  // Hama wishes them well and goes back into her room.
  Actor_RunRepeatedMotion(ACTOR_TEMPLE_HAMA, 2);
  Event_Wait(20);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 0);
  Actor_FaceDirection(ACTOR_GARET, FACING_NORTHWEST, 0);
  Actor_FaceDirection(ACTOR_IVAN, FACING_NORTH, 0);
  Actor_FaceDirection(ACTOR_MIA, FACING_NORTHEAST, 0);
  Event_Wait(20);
  Actor_FaceDirection(ACTOR_TEMPLE_HAMA, FACING_SOUTH_SOUTHEAST, 20);
  // "Do not worry, Garet. I do not mind."
  Event_ShowMessageThenWait(ACTOR_TEMPLE_HAMA, 0, 20);
  Actor_RunRepeatedMotion(ACTOR_GARET, 2);
  Event_Wait(20);
  Actor_SetAnimationAndWait(ACTOR_GARET, ANIM_NOD);
  Event_Wait(30);
  Actor_SetAnimationAndWait(ACTOR_TEMPLE_HAMA, ANIM_NOD);
  Event_Wait(20);
  // "Travel well, my friends."
  Event_ShowMessageThenWait(ACTOR_TEMPLE_HAMA, 0, 20);
  Actor_RunRepeatedMotion(ACTOR_TEMPLE_HAMA, 2);
  Event_Wait(20);
  // "I hope to see you soon."
  Event_ShowMessageThenWait(ACTOR_TEMPLE_HAMA, 0, 20);
  Actor_SetAnimation(ACTOR_PARTY_LEADER, ANIM_NOD);
  Actor_SetAnimation(ACTOR_GARET, ANIM_NOD);
  Actor_SetAnimation(ACTOR_IVAN, ANIM_NOD);
  Actor_SetAnimationAndWait(ACTOR_MIA, ANIM_NOD);
  Event_Wait(20);
  Actor_SetAnimationAndWait(ACTOR_TEMPLE_HAMA, ANIM_NOD);
  Event_Wait(20);
  Actor_FaceDirection(ACTOR_TEMPLE_HAMA, FACING_NORTH, 30);
  // The door slides open with SE_THUMP, Hama walks through, and it slides shut.
  Sound_Play(SE_THUMP);
  MapGrid_AnimateCells(gLamaTemple_DoorOpenCells, DOOR_X, DOOR_Y);
  Actor_SetSpeed(ACTOR_TEMPLE_HAMA, 0.8);
  Actor_WalkToAndWait(ACTOR_TEMPLE_HAMA, 136, 136);
  Actor_SetPosition(ACTOR_TEMPLE_HAMA, 0, 0);
  Sound_Play(SE_THUMP);
  MapGrid_AnimateCells(gLamaTemple_DoorCloseCells, DOOR_X, DOOR_Y);
  Event_Wait(60);
  Scene_PlayQueuedMusic();

  // Garet asks whether the leader means to leave Hama's words unasked.
  Actor_RunRepeatedMotion(ACTOR_GARET, 2);
  Event_Wait(20);
  Actor_FaceDirection(ACTOR_GARET, FACING_SOUTHWEST, 20);
  // "Weren't you going to ask Master Hama what she meant earlier?"
  Event_ShowMessageThenWait(ACTOR_GARET, 0, 20);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_WEST, 20);
  Actor_RunRepeatedMotion(ACTOR_IVAN, 2);
  Event_Wait(20);
  Actor_FaceDirection(ACTOR_MIA, FACING_SOUTHEAST, 20);
  // "We do not know when we will be able to see Master Hama again."
  Event_ShowMessageThenWait(ACTOR_MIA, 0, 20);
  Actor_RunRepeatedMotion(ACTOR_GARET, 1);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTHEAST, 20);
  // "Isaac, don't you think we should ask Master Hama what she meant?"
  // Yes: "See? If it's bothering you, you should just go ahead and ask..."
  // No: "Huh? You think we should just leave it as is?"
  Event_AskYesNo(ACTOR_GARET, 0);
  Event_Wait(20);

  // Ivan decides that Hama will tell him when their mission is done.
  Actor_RunRepeatedMotion(ACTOR_IVAN, 2);
  Event_Wait(20);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_WEST, 20);
  // "Thank you, everyone..."
  Event_ShowMessageThenWait(ACTOR_IVAN, 0, 20);
  Actor_StartRepeatedMotion(ACTOR_PARTY_LEADER, 1);
  Actor_StartRepeatedMotion(ACTOR_GARET, 1);
  Actor_RunRepeatedMotion(ACTOR_MIA, 1);
  Event_Wait(20);
  Actor_SetAnimationAndWait(ACTOR_IVAN, ANIM_SHAKE_HEAD);
  Event_Wait(20);
  // "but it is better that we leave now."
  Event_ShowMessageThenWait(ACTOR_IVAN, 0, 20);
  Actor_RunRepeatedMotion(ACTOR_MIA, 2);
  Event_Wait(20);
  // "What do you mean?"
  Event_ShowMessageThenWait(ACTOR_MIA, 0, 20);
  Actor_FaceDirection(ACTOR_IVAN, FACING_NORTH_NORTHWEST, 20);
  Actor_SetAnimationAndWait(ACTOR_IVAN, ANIM_NOD);
  Event_Wait(20);
  Actor_SetAnimationAndWait(ACTOR_IVAN, ANIM_NOD);
  Event_Wait(20);
  // "Master Hama hasn't looked me in the eye since she said those..."
  Event_ShowMessageThenWait(ACTOR_IVAN, 0, 20);
  Actor_ShowEmote(ACTOR_GARET, EMOTE_IN_FRONT | EMOTE_WINCE, 60);
  // "Well, that just makes me even more curious!"
  Event_ShowMessageThenWait(ACTOR_GARET, 0, 20);
  Actor_FaceDirection(ACTOR_IVAN, FACING_NORTHEAST, 20);
  Actor_SetAnimationAndWait(ACTOR_IVAN, ANIM_SHAKE_HEAD);
  Event_Wait(20);
  // "Don't worry about it. I doubt she would even tell me if I asked."
  Event_ShowMessageThenWait(ACTOR_IVAN, 0, 20);
  Actor_ShowEmote(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | EMOTE_WINCE, 0);
  Actor_ShowEmote(ACTOR_MIA, EMOTE_IN_FRONT | EMOTE_WINCE, 60);
  // "Well, that's just..."
  Event_ShowMessageThenWait(ACTOR_MIA, 0, 20);
  Actor_FaceDirection(ACTOR_IVAN, FACING_NORTHWEST, 20);
  Actor_SetAnimationAndWait(ACTOR_IVAN, ANIM_NOD);
  Event_Wait(20);
  // "Master Hama will tell me when we have completed our mission."
  Event_ShowMessageThenWait(ACTOR_IVAN, 0, 20);
  Actor_StartRepeatedMotion(ACTOR_PARTY_LEADER, 1);
  Actor_StartRepeatedMotion(ACTOR_GARET, 1);
  Actor_RunRepeatedMotion(ACTOR_MIA, 1);
  Event_Wait(20);
  Actor_SetSpeed(ACTOR_IVAN, 0.5);
  Actor_WalkToAndWait(ACTOR_IVAN, 128, 184);
  Actor_FaceDirection(ACTOR_IVAN, FACING_SOUTH, 20);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 0);
  Actor_FaceDirection(ACTOR_GARET, FACING_NORTHWEST, 0);
  Actor_FaceDirection(ACTOR_MIA, FACING_NORTHEAST, 20);
  Actor_SetAnimationAndWait(ACTOR_IVAN, ANIM_NOD);
  Event_Wait(20);
  // "I have to believe that..."
  Event_ShowMessageThenWait(ACTOR_IVAN, 0, 20);
  Actor_SetAnimation(ACTOR_PARTY_LEADER, ANIM_NOD);
  Actor_SetAnimation(ACTOR_GARET, ANIM_NOD);
  Actor_SetAnimationAndWait(ACTOR_MIA, ANIM_NOD);
  Event_Wait(20);

  // Garet, Ivan and Mia walk to the leader and rejoin the party.
  Actor_SetSpeed(ACTOR_GARET, 0.5);
  Actor_SetSpeed(ACTOR_MIA, 0.5);
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

/// Searching the writing on the cliff east of the waterfall (cells (20, 6) and
/// (21, 6)) reads "He who has the power to see that which cannot be seen shall
/// see the truth...", a hint to use Reveal on the mossy rock below it.
static void LamaTemple_ReadWallWriting(void) {
  Event_Start();
  Actor_SetAnimation(ACTOR_PARTY_LEADER, ANIM_STAND);
  FieldMessage_Show(MsgMountainHeWhoHasPowerSee, 1);
  Event_End();
}

/// When Reveal wears off in Lama Temple, the mossy rock east of the waterfall
/// stands over the chest again (cell (21, 9)), and the chest can no longer be
/// opened. The same happens on every entry. The chest's spot is hidden and its
/// cell takes the plain ground's attributes from the cell kept at (85, 9).
static void LamaTemple_HideChest(void) {
  MapGrid_CopyCellAttributes(85, 9, 1, 1, 21, 9);
  TreasureSpot_SetActorPosition(TREASURE_SPOT_WATER_OF_LIFE, 0, 0);
  Actor_SetPosition(ACTOR_MOSSY_ROCK, 344, 152);
}

/// Reveal in Lama Temple makes the mossy rock east of the waterfall vanish,
/// showing the chest of Water of Life under it (cell (21, 9)). The chest's
/// spot goes back to its cell, and the cell gets back the chest's attributes,
/// which Scene_Initialize kept at (21, 73).
static void LamaTemple_RevealChest(void) {
  MapGrid_CopyCellAttributes(21, 73, 1, 1, 21, 9);
  TreasureSpot_SetPosition(TREASURE_SPOT_WATER_OF_LIFE, -1, -1);
  Actor_SetPosition(ACTOR_MOSSY_ROCK, 0, 0);
}

/// The traveling healer at the east end of the bridge in Lama Temple offers the
/// Sanctum's services when spoken to from the south; from any other side he
/// only says he travels the world to spread good will.
static void LamaTemple_TalkToHealer(void) {
  u32 heading = Actor_Get(ACTOR_PARTY_LEADER)->facing;

  Event_Start();
  if (FACING_IS_NORTH(heading)) {
    Sanctum_Run(ACTOR_TRAVELING_HEALER);
  } else {
    Event_SetMessageId(MsgMountainAmTravelingAroundWorldSpread);
    // "I am traveling around the world to spread good will."
    Event_ShowMessage(ACTOR_TRAVELING_HEALER, 0);
  }
  Event_End();
}

/// The row Feizhi runs along, from (536, 216) west to (408, 216).
#define FEIZHI_ROW 13

/// Feizhi runs in behind Isaac as he walks onto Silk Road a few steps inside
/// the east entrance (cells (27, 12) to (27, 14)), cries out at the boulders
/// blocking the road, and runs down past the puddle to the rockslide calling
/// for Hsu, with Isaac following. Nothing happens here unless the party has
/// seen her leave Xian, and it plays once.
static void AlpineCrossing_FeizhiArrives(void) {
  struct Actor *leader;

  if (GameFlag_Get(FLAG_FEIZHI_WENT_TO_HSU)) {
    Event_Start();
    Actor_SetPosition(ACTOR_FEIZHI, 536, 216);
    Event_SetMessageId(MsgMountainYahhSilkRoadBouldersBlock);
    // "Yahh! Silk Road! Boulders block the road!"
    Event_ShowMessageThenWait(ACTOR_FEIZHI, 0, 20);
    Actor_RunRepeatedMotion(ACTOR_PARTY_LEADER, 2);
    Event_Wait(20);

    // The leader turns to follow Feizhi as she runs past; one standing in
    // her row steps aside first.
    leader = Actor_Get(ACTOR_PARTY_LEADER);
#ifdef UBFIX
    leader->update = AlpineCrossing_LeaderFaceFeizhi;
#else
    leader->update = (void (*)(struct Actor *))AlpineCrossing_LeaderFaceFeizhi;
#endif
    leader = Actor_Get(ACTOR_PARTY_LEADER);
    if (FIELD_CELL(leader->z.fixed) == FEIZHI_ROW) {
      Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 440, 200);
    }
    Actor_SetSpeed(ACTOR_FEIZHI, 2);
    Actor_SetSpritePriority(ACTOR_FEIZHI, 2);
    Actor_WalkToAndWait(ACTOR_FEIZHI, 408, 216);
    Actor_Get(ACTOR_FEIZHI)->priorityFlags |= ACTOR_PRIORITY_AUTOMATIC;
    Event_Wait(10);
    Actor_FaceDirection(ACTOR_FEIZHI, FACING_WEST, 20);
    // "My vision... It came true."
    Event_ShowMessageThenWait(ACTOR_FEIZHI, 0, 20);
    Actor_StartRepeatedMotion(ACTOR_FEIZHI, 2);
    Actor_SetAttachedEffect(ACTOR_FEIZHI,
                            ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
    Event_Wait(60);
    // "Hsu, Hsu!"
    Event_ShowMessageThenWait(ACTOR_FEIZHI, 0, 20);

    // Feizhi runs down the path with the camera; the leader follows her
    // until the camera arrives.
    Actor_SetScript(ACTOR_FEIZHI, gAlpineCrossing_FeizhiPath);
    FieldCamera_MoveTo(296, CAMERA_KEEP, 344, 1);
    GameFlag_Set(FLAG_FEIZHI_SAW_ROCKSLIDE);
    Actor_WaitForScriptEndById(ACTOR_FEIZHI);
    FieldCamera_WaitForArrival();
    Actor_SetSpeed(ACTOR_PARTY_LEADER, 1);
    Actor_SetScript(ACTOR_PARTY_LEADER, gAlpineCrossing_LeaderPath);
    Actor_WaitForScriptEndById(ACTOR_PARTY_LEADER);
    Event_Wait(10);
    Actor_Get(ACTOR_PARTY_LEADER)->update = NULL;
    Event_Wait(30);

    // She cannot hear Hsu's voice.
    Actor_RunRepeatedMotion(ACTOR_FEIZHI, 2);
    Event_Wait(20);
    Actor_FaceDirection(ACTOR_FEIZHI, FACING_SOUTH_SOUTHWEST, 120);
    Actor_ShowEmote(ACTOR_FEIZHI, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 60);
    Actor_ShowEmote(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | EMOTE_QUESTION, 60);
    Actor_SetAnimationAndWait(ACTOR_FEIZHI, ANIM_SHAKE_HEAD);
    Event_Wait(20);
    // "It is no good. I cannot hear Hsu's voice!"
    Event_ShowMessageThenWait(ACTOR_FEIZHI, 0, 20);
    Event_End();
  }
}

/// Feizhi, waiting by the rockslide, asks whether she should return to Xian,
/// and whatever Isaac answers decides to keep looking for Hsu.
static void AlpineCrossing_TalkToFeizhi(void) {
  Event_Start();
  Event_SetMessageId(MsgMountainDoDoWarriorShouldReturn);
  Actor_ShowEmote(ACTOR_FEIZHI, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 60);
  // "What do I do? Warrior, should I return to Xian?"
  Event_OpenMessage(ACTOR_FEIZHI, 0);
  if (Event_AskYesNoWithActorAnimation(ACTOR_PARTY_LEADER, 0) == 1) {
    Event_SkipMessages(1);
  }
  Event_Wait(20);
  Actor_SetAnimationAndWait(ACTOR_FEIZHI, ANIM_SHAKE_HEAD);
  Event_Wait(20);
  Event_ShowMessageThenWait(ACTOR_FEIZHI, 0, 20);
  Event_End();
}

/// The first time the party comes out of the cave mouth north-east of Hsu,
/// the camera finds him pinned under the boulder; Hama asks if he is okay, and
/// none of them knows how to free him.
static void AlpineCrossing_HamaReachesHsu(void) {
  Event_Start();
  Event_StartScreenTransition();
  Event_WaitTransitionFrames();
  Event_Wait(30);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_SOUTHWEST, 0);
  FieldCamera_MoveToActorById(ACTOR_HSU, 1);
  FieldCamera_WaitForArrival();
  Actor_RunRepeatedMotion(ACTOR_HAMA, 2);
  Event_Wait(20);
  Event_SetMessageId(MsgMountainHsuOkay);
  // "Hsu, are you okay?"
  Event_ShowMessageThenWait(ACTOR_HAMA, 0, 20);
  Actor_RunRepeatedMotion(ACTOR_HSU, 1);
  Event_Wait(20);
  // "Master Hama, I am okay. But the boulder... I cannot move."
  Event_ShowMessageThenWait(ACTOR_HSU, 0, 20);
  Actor_SetAnimationAndWait(ACTOR_HAMA, ANIM_NOD);
  Event_Wait(20);
  // "Hsu seems unhurt. You must be glad, Feizhi."
  Event_ShowMessageThenWait(ACTOR_HAMA, 0, 20);
  Actor_FaceDirection(ACTOR_FEIZHI, FACING_NORTH_NORTHEAST, 20);
  // "Yes. Thank you, Master. But how can we free him?"
  Event_ShowMessageThenWait(ACTOR_FEIZHI, 0, 20);
  Actor_SetAnimationAndWait(ACTOR_HAMA, ANIM_NOD);
  Event_Wait(60);
  Actor_ShowEmote(ACTOR_HAMA, EMOTE_IN_FRONT | EMOTE_WINCE, 60);
  Actor_SetAnimationAndWait(ACTOR_HAMA, ANIM_SHAKE_HEAD);
  Event_Wait(20);
  // "I don't know what we can do."
  Event_ShowMessageThenWait(ACTOR_HAMA, 0, 20);
  Actor_StartRepeatedMotion(ACTOR_FEIZHI, 2);
  Actor_SetAttachedEffect(ACTOR_FEIZHI,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  Event_Wait(60);
  Actor_FaceDirection(ACTOR_FEIZHI, FACING_NORTH_NORTHWEST, 20);
  Actor_SetAnimation(ACTOR_HSU, ANIM_HSU_STRUGGLE);
  Event_End();
  GameFlag_Set(FLAG_HAMA_REACHED_HSU);
}

/// Lift on the boulder pinning Hsu at the Alpine Crossing raises it, glowing,
/// while Hama and Feizhi pull him out in two heaves. Hama then sends for Master
/// Feh and the party is taken to Lama Temple for her farewell.
static void AlpineCrossing_LiftBoulder(void) {
  Event_Start();
  Actor_SetAttachedEffect(ACTOR_HAMA,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  Actor_StartRepeatedMotion(ACTOR_HAMA, 2);
  Event_Wait(60);
  Event_SetMessageId(MsgMountainIsaacDidLiftBoulder);
  // "Isaac, did you lift the boulder?"
  Event_ShowMessageThenWait(ACTOR_HAMA, 0, 20);
  Actor_SetAttachedEffect(ACTOR_FEIZHI,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  Actor_Jump(ACTOR_FEIZHI, 4, 0);
  Event_Wait(60);
  // "Oh my! The boulder! It floats!"
  Event_ShowMessageThenWait(ACTOR_FEIZHI, 0, 20);
  Actor_RunRepeatedMotion(ACTOR_HAMA, 1);
  Event_Wait(20);
  // "Quickly, Feizhi. We must pull Hsu free!"
  Event_ShowMessageThenWait(ACTOR_HAMA, 0, 20);
  Actor_FaceDirection(ACTOR_FEIZHI, FACING_NORTH_NORTHEAST, 20);
  Actor_SetAnimationAndWait(ACTOR_FEIZHI, ANIM_NOD);
  Event_Wait(20);

  // Hama and Feizhi take hold of Hsu.
  Actor_WalkTo(ACTOR_HAMA, 178, 276);
  Actor_WalkToAndWait(ACTOR_FEIZHI, 172, 284);
  Actor_WaitForMoveById(ACTOR_HAMA);
  Actor_FaceDirection(ACTOR_HAMA, FACING_SOUTH_SOUTHWEST, 0);
  Actor_FaceDirection(ACTOR_FEIZHI, FACING_NORTH_NORTHWEST, 0);
  Event_Wait(20);
  Actor_RunRepeatedMotion(ACTOR_HAMA, 2);
  Event_Wait(20);
  // "Ready yourself, Hsu. I am going to pull. One, two, three!"
  Event_ShowMessageThenWait(ACTOR_HAMA, 0, 20);
  Actor_Get(ACTOR_HAMA)->steeringFlags &= (u8)~ACTOR_FACES_MOTION;
  Actor_Get(ACTOR_FEIZHI)->steeringFlags &= (u8)~ACTOR_FACES_MOTION;
  Actor_SetSpeed(ACTOR_HAMA, 0.2);
  Actor_SetSpeed(ACTOR_FEIZHI, 0.2);

  // The first heave: two tugs draw Hsu halfway out.
  Actor_SetAnimation(ACTOR_HAMA, ANIM_HAMA_PULL);
  Actor_SetAnimation(ACTOR_FEIZHI, ANIM_FEIZHI_PULL);
  Event_Wait(20);
  Sound_Play(SE_CHEST_CLOSE);
  Actor_SetDestinationOffset(ACTOR_HAMA, 2, 0);
  Actor_SetDestinationOffset(ACTOR_HSU, 2, 0);
  Actor_SetDestinationOffset(ACTOR_FEIZHI, 2, 0);
  Actor_WaitForMoveById(ACTOR_FEIZHI);
  Event_Wait(30);
  Actor_SetAnimation(ACTOR_HAMA, ANIM_HAMA_PULL);
  Actor_SetAnimation(ACTOR_FEIZHI, ANIM_FEIZHI_PULL);
  Event_Wait(20);
  Sound_Play(SE_CHEST_CLOSE);
  Actor_SetDestinationOffset(ACTOR_HAMA, 4, 0);
  Actor_SetDestinationOffset(ACTOR_HSU, 4, 0);
  Actor_SetDestinationOffset(ACTOR_FEIZHI, 4, 0);
  Actor_WaitForMoveById(ACTOR_FEIZHI);
  Actor_Stop(ACTOR_HSU);
  Actor_SetAnimation(ACTOR_HAMA, ANIM_STAND);
  Actor_SetAnimation(ACTOR_FEIZHI, ANIM_STAND);
  Event_Wait(50);
  Actor_Jump(ACTOR_FEIZHI, 2, 0);
  Event_Wait(20);
  // "He is halfway out. Just a little further."
  Event_ShowMessageThenWait(ACTOR_FEIZHI, 0, 20);

  // Hama asks the leader to hold the boulder a moment longer.
  Actor_SetAnimationAndWait(ACTOR_HAMA, ANIM_NOD);
  Event_Wait(20);
  // "Can you hang on just a moment longer, Isaac?"
  // Yes: "Thank you, Isaac!"
  // No: "You cannot back out now! You must be responsible!"
  Event_AskYesNo(ACTOR_HAMA, 0);
  // "Ready? I am going to pull! One, two, three!"
  Event_ShowMessageThenWait(ACTOR_HAMA, 0, 20);

  // The second heave pulls Hsu free.
  Actor_SetAnimation(ACTOR_HAMA, ANIM_HAMA_PULL);
  Actor_SetAnimation(ACTOR_FEIZHI, ANIM_FEIZHI_PULL);
  Event_Wait(20);
  Sound_Play(SE_CHEST_CLOSE);
  Actor_SetDestinationOffset(ACTOR_HAMA, 2, 0);
  Actor_SetDestinationOffset(ACTOR_HSU, 2, 0);
  Actor_SetDestinationOffset(ACTOR_FEIZHI, 2, 0);
  Actor_WaitForMoveById(ACTOR_FEIZHI);
  Event_Wait(30);
  Actor_SetAnimation(ACTOR_HAMA, ANIM_HAMA_PULL);
  Actor_SetAnimation(ACTOR_FEIZHI, ANIM_FEIZHI_PULL);
  Event_Wait(20);
  Sound_Play(SE_CHEST_CLOSE);
  Actor_SetDestinationOffset(ACTOR_HAMA, 4, 0);
  Actor_SetDestinationOffset(ACTOR_HSU, 4, 0);
  Actor_SetDestinationOffset(ACTOR_FEIZHI, 4, 0);
  Actor_WaitForMoveById(ACTOR_FEIZHI);
  Event_Wait(40);
  Actor_SetAnimation(ACTOR_HAMA, ANIM_STAND);
  Actor_SetAnimation(ACTOR_FEIZHI, ANIM_STAND);
  Actor_Jump(ACTOR_FEIZHI, 2, 0);
  Event_Wait(20);
  Actor_FaceDirection(ACTOR_FEIZHI, FACING_NORTH_NORTHEAST, 20);
  // "We did it! Master Hama, Hsu is free!"
  Event_ShowMessageThenWait(ACTOR_FEIZHI, 0, 20);
  Actor_SetAnimationAndWait(ACTOR_HAMA, ANIM_NOD);
  Event_Wait(20);
  // "We did it, Feizhi!"
  Event_ShowMessageThenWait(ACTOR_HAMA, 0, 30);
  // "Thank you, Isaac!"
  Event_ShowMessageThenWait(ACTOR_HAMA, 0, 20);

  // Feizhi steps back; they agree to send for Master Feh.
  Actor_Get(ACTOR_FEIZHI)->steeringFlags |= ACTOR_FACES_MOTION;
  Actor_SetSpeed(ACTOR_FEIZHI, 0.8);
  Actor_WalkToAndWait(ACTOR_FEIZHI, 168, 296);
  Actor_FaceDirection(ACTOR_FEIZHI, FACING_NORTH_NORTHEAST, 20);
  Actor_SetAnimation(ACTOR_FEIZHI, ANIM_FEIZHI_CROUCH);
  Actor_SetAnimationAndWait(ACTOR_HAMA, ANIM_NOD);
  Event_Wait(20);
  // "We must contact Master Feh and ask him to tend to Hsu's..."
  Event_ShowMessageThenWait(ACTOR_HAMA, 0, 20);
  Actor_SetAnimationAndWait(ACTOR_FEIZHI, ANIM_NOD);
  Event_Wait(20);
  // "I will ask him to clean up the rocks, too."
  Event_ShowMessageThenWait(ACTOR_FEIZHI, 0, 20);
  Actor_SetAnimationAndWait(ACTOR_HAMA, ANIM_NOD);
  Event_Wait(20);
  // "Excellent. If Silk Road remains blocked, everyone will suffer."
  Event_ShowMessageThenWait(ACTOR_HAMA, 0, 20);
  Actor_SetAnimationAndWait(ACTOR_PARTY_LEADER, ANIM_NOD);
  Event_Wait(20);
  Event_End();
  GameFlag_Set(FLAG_HSU_RESCUED);
  gFieldWork->transition =
      SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_SHAPE);
  Import_EventSetExitRequest(ALPINE_CROSSING_TO_LAMA_TEMPLE);
}

/// Searching the signpost beside the road west of the east entrance (cell
/// (25, 11)) reads "North: Altin Mine West: Lama Temple".
static void AlpineCrossing_ReadSignpost(void) {
  Event_Start();
  Actor_SetAnimation(ACTOR_PARTY_LEADER, ANIM_STAND);
  FieldMessage_Show(MsgMountainNorthAltinMineWestLama, 1);
  Event_End();
}

/// Reveal at the Alpine Crossing opens the mouth of a cave in the cliff west of
/// Hsu (cells (5, 13) and (5, 14)). The cave inside comes out again just
/// north-east of him. The cave mouth's pictures and cells are kept off the map
/// at (5, 28).
static void AlpineCrossing_RevealPath(void) {
  MapGrid_CopyCellsTo(5, 28, 5, 13, 1, 2);
  MapGrid_CopyCellAttributes(5, 28, 1, 2, 5, 13);
  Event_Wait(1);
}

/// When Reveal wears off at the Alpine Crossing, the cave mouth in the cliff
/// west of Hsu is solid rock again (cells (5, 13) and (5, 14)), from the cells
/// kept at (6, 28).
static void AlpineCrossing_HidePath(void) {
  MapGrid_CopyCellsTo(6, 28, 5, 13, 1, 2);
  MapGrid_CopyCellAttributes(6, 28, 1, 2, 5, 13);
  Event_Wait(1);
}

/// Update routine of the ice pillar: Isaac is drawn in front of it while he
/// stands higher than its top, so that he shows standing on it, and behind it
/// otherwise.
static UNREAD_RESULT Pillar_SortAgainstLeader(struct Actor *pillar) {
  if (Actor_Get(ACTOR_PARTY_LEADER)->y.fixed > pillar->y.fixed) {
    pillar->priorityFlags |= ACTOR_PRIORITY_UNDERFOOT;
  } else {
    pillar->priorityFlags &= (u8)~ACTOR_PRIORITY_UNDERFOOT;
  }
}

/// Nothing visible happens as Frost reaches the puddle at the foot of the
/// gully. When Isaac casts it from north of the puddle, the game draws the
/// puddle in front of the scenery, so that the pillar about to rise from it is
/// not hidden behind the ground he stands on.
static void AlpineCrossing_PrepareFrost(void) {
  struct Actor *leader = Actor_Get(ACTOR_PARTY_LEADER);
  struct Actor *puddle = Actor_Get(ACTOR_WATER_FOR_FROST);

  if (leader->z.fixed <= puddle->z.fixed) {
    Actor_SetSpritePriority(ACTOR_WATER_FOR_FROST, 1);
  }
}

/// Frost on the puddle at the foot of the gully (cell (20, 18)) freezes it into
/// a tall ice pillar, which the party can jump onto from either side. The
/// pillar's cell, and its twin in the map's second layer at (56, 18), become
/// jump cells like the one kept at (55, 16).
static void AlpineCrossing_FreezePuddle(void) {
  Event_Start();

  Actor_Get(ACTOR_WATER_FOR_FROST)->priorityFlags &=
      (u8)~ACTOR_PRIORITY_UNDERFOOT;
  Actor_Get(ACTOR_WATER_FOR_FROST)->collisionFlags &= (u8)~ACTOR_PUSHABLE;
  Actor_Get(ACTOR_WATER_FOR_FROST)->motionFlags = 0;
  Actor_Get(ACTOR_WATER_FOR_FROST)->update =
      (void (*)(struct Actor *))Pillar_SortAgainstLeader;

  MapGrid_CopyCellAttributes(55, 16, 1, 1, 56, 18);
  MapGrid_CopyCellAttributes(55, 16, 1, 1, 20, 18);

  wait(1);
  GameFlag_Set(FLAG_ALPINE_CROSSING_PUDDLE_FROZEN);
  Actor_SetSpritePriority(ACTOR_WATER_FOR_FROST, 2);
  Event_End();
}

/// Growth on the sprout at the foot of the cliff below the signpost (cell
/// (21, 12)) grows it into a vine up the cliff, which the party can climb. Its
/// two cells take the climbable cells kept at (14, 6).
static void AlpineCrossing_GrowSprout(void) {
  Event_Start();
  MapGrid_CopyCellAttributes(14, 6, 1, 2, 21, 11);
  Actor_Get(ACTOR_PLANT)->collisionFlags = (u8)~ACTOR_SOLID;
  GameFlag_Set(FLAG_ALPINE_CROSSING_SPROUT_GROWN);
  Event_End();
}

const struct Trigger *Scene_GetTriggers(void) {
  if (gMainRoutineWork.next == MAP_ALPINE_CROSSING) {
    return gAlpineCrossing_Triggers;
  }
  return gLamaTemple_Triggers;
}

/// Sets up whichever scene is being entered and runs its arrival events.
s32 Scene_Initialize(void) {
  s32 entrance;

  gFieldWork->transition = SCENE_TRANSITION(TRANSITION_BACKDROP_FADE, 0);
  if (gMainRoutineWork.next == MAP_LAMA_TEMPLE) {
    // Lama Temple: the wind blows (SE_AMBIENT_WIND), two monks meditate, and
    // the mossy rock stands over the chest. The chest cell's attributes are
    // kept at (21, 73) for Reveal to restore.
    Scene_QueueMusic(SE_AMBIENT_WIND);
    Actor_SetAnimation(ACTOR_MEDITATING_MONK, ANIM_MONK_MEDITATE);
    Actor_SetAnimation(ACTOR_MONK_SENSING_ANXIETY, ANIM_MONK_MEDITATE);
    Actor_SetAnimation(ACTOR_MOSSY_ROCK, ANIM_TURNING_ROCK_MOSSY);
    MapGrid_CopyCellAttributes(21, 9, 1, 1, 21, 73);
    LamaTemple_HideChest();
    if (GameFlag_Get(FLAG_HSU_RESCUED)) {
      Actor_SetPosition(ACTOR_CHI_MONK, 136, 256);
      Actor_FaceDirection(ACTOR_CHI_MONK, FACING_EAST, 0);
    }
    entrance = gMainRoutineWork.entrance;
    if (entrance == LAMA_TEMPLE_FROM_LACQUER_ROOMS_A_HAMAS_ROOM) {
      GameFlag_Clear(FLAG_SHOW_AREA_NAME);
    } else if (entrance == LAMA_TEMPLE_FROM_ALPINE_CROSSING &&
               !GameFlag_Get(FLAG_KEEP_PARTY_POSITION)) {
      LamaTemple_RunHamasFarewell();
    }
  } else if (gMainRoutineWork.next == MAP_ALPINE_CROSSING) {
    // Alpine Crossing: restore the Psynergy puzzles already solved.
    Actor_SetSpriteFlags(Actor_Get(ACTOR_WATER_FOR_FROST), 0);
    Actor_Get(ACTOR_WATER_FOR_FROST)->priorityFlags |= ACTOR_PRIORITY_UNDERFOOT;
    if (GameFlag_Get(FLAG_ALPINE_CROSSING_PUDDLE_FROZEN)) {
      Actor_SetAnimation(ACTOR_WATER_FOR_FROST, PUDDLE_ANIMATION_PILLAR);
      AlpineCrossing_FreezePuddle();
    }
    if (GameFlag_Get(FLAG_ALPINE_CROSSING_SPROUT_GROWN)) {
      Actor_SetAnimation(ACTOR_PLANT, SPROUT_ANIMATION_GROWN);
      AlpineCrossing_GrowSprout();
    }
    if (gMainRoutineWork.entrance ==
            ALPINE_CROSSING_FROM_ALTIN_PEAK_SOUTH_ENTRANCE_WEST ||
        gMainRoutineWork.entrance ==
            ALPINE_CROSSING_FROM_ALTIN_PEAK_SOUTH_ENTRANCE_EAST) {
      GameFlag_Clear(FLAG_SHOW_AREA_NAME);
    }

    // Feizhi is here only once she has set out after Hsu.
    if (!GameFlag_Get(FLAG_FEIZHI_WENT_TO_HSU) &&
        !GameFlag_Get(FLAG_XIAN_HEARD_HSU_TRAPPED) &&
        !GameFlag_Get(FLAG_HSU_RESCUED)) {
      Actor_SetPosition(ACTOR_FEIZHI, 0, 0);
    }

    // Arriving from the north after Hama's lesson, before the party has freed
    // Hsu: he has been freed without it and the boulder is gone.
    if (!Event_GetFlag(FLAG_HSU_RESCUED) &&
        GameFlag_Get(FLAG_XIAN_HEARD_HSU_TRAPPED) &&
        gMainRoutineWork.entrance == ALPINE_CROSSING_FROM_WORLD_MAP_NORTH) {
      Actor_SetPosition(ACTOR_BOULDER, 0, 0);
      GameFlag_Set(FLAG_HSU_RESCUED);
      GameFlag_Set(FLAG_HSU_RESCUED_WITHOUT_PARTY);
      Actor_SetPosition(ACTOR_FEIZHI, 0, 0);
    }

    // Once Hsu is free, the rockslide and a rock west of him are cleared
    // away.
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
      Actor_SetPosition(ACTOR_FIRST_WORKER, 0, 0);
      Actor_SetPosition(ACTOR_SECOND_WORKER, 0, 0);
      Actor_SetPosition(ACTOR_HAMA, 192, 264);
      Actor_SetPosition(ACTOR_HSU, 164, 280);
      Actor_SetPosition(ACTOR_FEIZHI, 184, 304);
      Actor_FaceDirection(ACTOR_HAMA, FACING_SOUTH_SOUTHWEST, 0);
      Actor_FaceDirection(ACTOR_FEIZHI, FACING_NORTH_NORTHWEST, 0);
      Actor_SetScript(ACTOR_HSU, gAlpineCrossing_HsuStruggle);
      Actor_Get(ACTOR_HSU)->scaleX = -FIXED_ONE;
    }
    if (!Event_GetFlag(FLAG_HSU_RESCUED)) {
      Actor_SetPosition(ACTOR_HSU, 164, 280);
      Actor_SetScript(ACTOR_HSU, gAlpineCrossing_HsuStruggle);
      Actor_Get(ACTOR_HSU)->scaleX = -FIXED_ONE;
    }
    if (gMainRoutineWork.entrance ==
            ALPINE_CROSSING_FROM_ALTIN_PEAK_SOUTH_ENTRANCE_EAST &&
        !GameFlag_Get(FLAG_HAMA_REACHED_HSU) &&
        !GameFlag_Get(FLAG_KEEP_PARTY_POSITION) &&
        !GameFlag_Get(FLAG_HSU_RESCUED)) {
      AlpineCrossing_HamaReachesHsu();
    }
  }
  return 0;
}

/// The first worker's actor placement script: he is drawn behind the scenery.
s32 gAlpineCrossing_WorkerActorPlacementScript[] = {
    SC_CALL(Actor_DrawBehindScenery),
    SC_END,
};

/// Feizhi's run down the path toward Hsu, walking, then standing.
s32 gAlpineCrossing_FeizhiPath[] = {
    SC_ANIMATION(ANIM_WALK),
    SC_MOVE(392, 0, 232),
    SC_SYNC,
    SC_MOVE(376, 0, 232),
    SC_SYNC,
    SC_MOVE(344, 0, 264),
    SC_SYNC,
    SC_MOVE(344, 0, 312),
    SC_SYNC,
    SC_MOVE(312, 0, 344),
    SC_SYNC,
    SC_MOVE(296, 0, 344),
    SC_SYNC,
    SC_ANIMATION(ANIM_STAND),
    SC_END,
};

/// The party leader follows Feizhi most of the way.
s32 gAlpineCrossing_LeaderPath[] = {
    SC_ANIMATION(ANIM_WALK),
    SC_MOVE(392, 0, 232),
    SC_SYNC,
    SC_MOVE(376, 0, 232),
    SC_SYNC,
    SC_MOVE(344, 0, 264),
    SC_SYNC,
    SC_MOVE(344, 0, 312),
    SC_SYNC,
    SC_MOVE(328, 0, 328),
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
    ACTOR_PLACEMENT(ACTOR_GARET, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        0, 0, 0, FACING_NORTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(ACTOR_IVAN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        0, 0, 0, FACING_NORTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(ACTOR_MIA, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        0, 0, 0, FACING_NORTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_HAMA, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        0, 0, 0, FACING_NORTH, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_MONK_IN_ORANGE_ROBE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        168, 0, 336, FACING_EAST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_MONK_IN_ORANGE_ROBE, CONDITION_ALWAYS, MOVEMENT_TYPE_WANDER,
        216, 0, 240, FACING_SOUTH, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_MONK_IN_ORANGE_ROBE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        312, 0, 320, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_MONK_IN_ORANGE_ROBE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        344, 0, 320, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_MONK_IN_ORANGE_ROBE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        120, 0, 176, FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_TURNING_ROCK, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        344, 0, 152, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_MAN_IN_TALL_HAT, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        232, 0, 328, FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, 0),
    { SCENE_TABLE_END },
};

struct ActorPlacement gAlpineCrossing_ActorPlacements[] = {
    ACTOR_PLACEMENT(SPRITE_HAMA, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        0, 0, 0, FACING_NORTH, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_HSU, CONDITION_ALWAYS, (s32)gAlpineCrossing_HsuStruggle,
        0, 0, 0, FACING_NORTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_FEIZHI, FLAG_HSU_RESCUED, MOVEMENT_TYPE_STAND,
        296, 0, 344, FACING_WEST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_ROUND_BOULDER, FLAG_HSU_RESCUED, MOVEMENT_TYPE_STAND,
        152, 0, 276, FACING_WEST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_YOUTH_WITH_HEADBAND, IF_FLAG_SET(FLAG_XIAN_HEARD_HSU_TRAPPED), (s32)gAlpineCrossing_WorkerActorPlacementScript,
        456, 0, 200, FACING_SOUTH, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_YOUTH_WITH_TIED_HAIR, IF_FLAG_SET(FLAG_XIAN_HEARD_HSU_TRAPPED), MOVEMENT_TYPE_STAND,
        184, 0, 264, FACING_SOUTH, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_PUDDLE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        328, 0, 296, FACING_SOUTH, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_SPROUT, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        344, 0, 194, FACING_SOUTH, TALK_FACING_PARTY, 0),
    { SCENE_TABLE_END },
};

struct Trigger gLamaTemple_Triggers[] = {
    { TRIGGER_EXIT, CELL_BEHAVIOR_LAMA_TEMPLE_SOUTH_ROAD, CONDITION_ALWAYS, LAMA_TEMPLE_TO_WORLD_MAP },
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
    { PSYNERGY_ANYWHERE(ABILITY_REVEAL, PSYNERGY_PHASE_ACTS), 0, CONDITION_ALWAYS, EVENT(LamaTemple_RevealChest) },
    { PSYNERGY_ANYWHERE(ABILITY_REVEAL, PSYNERGY_PHASE_FADES), 0, CONDITION_ALWAYS, EVENT(LamaTemple_HideChest) },
    { EXAMINE_TRIGGER(EXAMINE_SILENT), CELL_BEHAVIOR_LAMA_TEMPLE_WALL_WRITING, CONDITION_ALWAYS, EVENT(LamaTemple_ReadWallWriting) },
    { EXAMINE_TRIGGER_FACING(FACING_NORTH), CELL_BEHAVIOR_LAMA_TEMPLE_PSYNERGY_STONE, FLAG_LAMA_TEMPLE_PSYNERGY_STONE, FIND_PSYNERGY_STONE },
    { EXAMINE_TRIGGER_FACING(FACING_NORTHEAST), CELL_BEHAVIOR_LAMA_TEMPLE_PSYNERGY_STONE, FLAG_LAMA_TEMPLE_PSYNERGY_STONE, FIND_PSYNERGY_STONE },
    { EXAMINE_TRIGGER_FACING(FACING_NORTHWEST), CELL_BEHAVIOR_LAMA_TEMPLE_PSYNERGY_STONE, FLAG_LAMA_TEMPLE_PSYNERGY_STONE, FIND_PSYNERGY_STONE },
    { EXAMINE_TRIGGER_FACING(FACING_WEST), CELL_BEHAVIOR_LAMA_TEMPLE_PSYNERGY_STONE, FLAG_LAMA_TEMPLE_PSYNERGY_STONE, FIND_PSYNERGY_STONE },
    { EXAMINE_TRIGGER(EXAMINE_CHEST), CELL_BEHAVIOR_LAMA_TEMPLE_CHEST, FLAG_LAMA_TEMPLE_WATER_OF_LIFE, FIND_ITEM(ITEM_WATER_OF_LIFE) },
    { TRIGGER_TABLE_END },
};

struct Trigger gAlpineCrossing_Triggers[] = {
    { TRIGGER_EXIT, CELL_BEHAVIOR_ALPINE_CROSSING_EAST_ROAD, CONDITION_ALWAYS, ALPINE_CROSSING_TO_WORLD_MAP_EAST },
    { TRIGGER_EXIT, CELL_BEHAVIOR_ALPINE_CROSSING_NORTH_ROAD, CONDITION_ALWAYS, ALPINE_CROSSING_TO_WORLD_MAP_NORTH },
    { TRIGGER_EXIT, CELL_BEHAVIOR_ALPINE_CROSSING_WEST_ROAD, CONDITION_ALWAYS, ALPINE_CROSSING_TO_WORLD_MAP_WEST },
    { TRIGGER_EXIT, CELL_BEHAVIOR_ALPINE_CROSSING_WEST_CAVE_MOUTH, CONDITION_ALWAYS, ALPINE_CROSSING_TO_ALTIN_PEAK_SOUTH_ENTRANCE_WEST },
    { TRIGGER_EXIT, CELL_BEHAVIOR_ALPINE_CROSSING_EAST_CAVE_MOUTH, CONDITION_ALWAYS, ALPINE_CROSSING_TO_ALTIN_PEAK_SOUTH_ENTRANCE_EAST },
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
    { TRIGGER_TABLE_END },
};
// clang-format on
