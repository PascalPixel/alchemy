/// @file
/// Kolima Forest (SceneId_KolimaForestTree), the last of the forest's five
/// maps, where Tret and Laurel, the spirits of the holy trees, stand. The party
/// has come from Kolima to reawaken Tret's gentle side and save the villagers
/// he turned into trees, and here the curse is lifted.
///
/// At first Tret is silent and his wrathful side answers for him
/// (LOCALID_TRETS_WRATHFUL_SIDE, with a portrait): "I control all in Tret's
/// realm!" Laurel, on the first talk (FLAG_MET_LAUREL), knows the party as the
/// ones who resisted Tret's glamour, but says they are too late: Tret will die
/// and the forest with him. Mind Read hears Laurel think that the healing
/// waters of Mercury Lighthouse might save Tret, though its fountain has dried
/// up, and Tret ask Kolima's forgiveness. Using Hermes' Water on Tret now, the
/// wrathful side throws the leader back. The way on is into Tret himself: the
/// region at his foot is KOLIMA_FOREST_TREE_TO_TREE_ROOMS, into the
/// Tret Tree (SceneId_TreeRooms), where the party defeats the evil
/// within him (FLAG_TRET_DEFEATED, set there) and finds him too weak to save
/// Kolima. Back here, with Hermes' Water, which Mercury Lighthouse's fountain
/// gives in an Empty Bottle, talking to Tret opens the party's items, and using
/// the water plays KolimaForestTree_RestoreTret: the forest's colours return,
/// Tret frees Kolima's people (FLAG_KOLIMA_CURSE_LIFTED), he and Laurel tell
/// how the gems that fell from the sky, the Psynergy Stones, twisted their
/// hearts, and Laurel warns of an evil forest risen south of the river. The
/// bottle is left empty. Afterwards Tret and Laurel thank the party and worry
/// about what lies beyond the river.
///
/// Until the curse is lifted the forest keeps its cursed colours
/// (KolimaPalette_Curse), which keeps the healthy set and the cursed set
/// both. As the water heals Tret, KolimaForestTree_RestoreTret fades
/// background palettes 0 to 13 to the healthy set, then pulses them towards
/// the healthy set and back towards the cursed one every 30 frames
/// (KolimaForestTree_UpdateHealingPalette) while the camera pans to three
/// places, flashes between the two sets four times and settles on the healthy
/// one, from the line that the water has seeped into Tret until Tret speaks.
/// Retreat from here always leads back to the
/// forest's way in (SceneId_KolimaForestEntrance,
/// KOLIMA_FOREST_ENTRANCE_FROM_WORLD_MAP), and the way back,
/// KOLIMA_FOREST_TREE_TO_KOLIMA_FOREST_FLOODGATE, leads to the Floodgate
/// Switch's map (SceneId_KolimaForestFloodgate).

#include "speaker.h"
#include "save_block.h"
#include "unit/scene/kolima_forest_tree.h"
#include "unit/scene/common/kolima_palette.h"
#include "unit/overworld/overworld.h"
#include "unit/overworld/field_services.h"
#include "unit/game/game.h"
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
#include "resource.h"
#include "object_event.h"
#include "field_effect.h"
#include "battle_unit.h"
#include "types.h"
#include "call.h"
#include "script_layer.h"
#include "engine_call.h"
#include "scene_ids.h"
#include "palette_map.h"
#include "io_reg.h"
#include "map_tables.h"
#include "text/msg_ids.h"
#include "ability_ids.h"
#include "objcmd.h"
#include "fixed_math.h"
#include "palbuf.h"
#include "flags.h"
#include "character_ids.h"
#include "item_ids.h"
#include "macros.h"
#include "songs.h"
#include "audio_engine_symbols.h"

/// The object events, numbering gKolimaForestTreeObjectEventTemplates' rows
/// from LOCALID_FIRST_PLACED.
enum {
  /// Tret's face, drawn in SPRITE_TRET_RAISED; its animations are his face's
  /// gestures (KolimaForestTree_PlayGesture).
  /// ★ Named from the game: "The Water of Hermes seeped into Tret."
  /// (MsgKolimaWaterHermesSeepedIntoTret)
  LOCALID_TRET = LOCALID_FIRST_PLACED,
  /// ★ Named from the game: "Oh! Laurel, you were given powers, too?"
  /// (MsgKolimaWasIndeedAngryPeopleHad)
  LOCALID_LAUREL,
  // XXX (no evidence): why these exist; likely left from a version where
  // they were drawn, but the ROM keeps no such version.
  /// Blank object events 28 pixels up at Tret's and Laurel's spots, never
  /// targeted;
  /// nothing addresses them but KolimaForestTree_PlayGesture, which takes one
  /// to say whose face to move.
  /// ★ Named from the game: "Tret" (MsgEnemyName)
  LOCALID_INVISIBLE_TRET,
  /// ★ Named from the game: "Laurel" (MsgDebugGlyphName)
  LOCALID_INVISIBLE_LAUREL,
  /// An invisible object event on Tret's spot that answers for him:
  /// talking to it, Mind Read on it and Hermes' Water used on it reach Tret.
  /// ★ Named from the game: "The Water of Hermes seeped into Tret."
  /// (MsgKolimaWaterHermesSeepedIntoTret)
  LOCALID_TRET_STAND_IN,
  /// The invisible object event on Laurel's spot that answers for her when she
  /// is
  /// talked to or her mind is read.
  /// ★ Named from the game: "I am sorry to have worried you so, Laurel."
  /// (MsgKolimaFeelGreatPowerSpreadingThrough)
  LOCALID_LAUREL_STAND_IN,
  /// Tret's wrathful side, which speaks with a portrait, drawn in SPRITE_TRET
  /// with child value 15 over his face.
  /// ★ Named from the game: "His wrathful side turned the people of Kolima
  /// into trees." (MsgKolimaWatchOutItsHappeningAgain)
  LOCALID_TRETS_WRATHFUL_SIDE
};

static void KolimaForestTree_RejectHermesWater(void);
static void KolimaForestTree_RestoreTret(void);
static void KolimaForestTree_SpawnRejectionSpark(void);
static void KolimaForestTree_UpdateHealingPalette(void);
/// The face gestures KolimaForestTree_PlayGesture plays, each from the face
/// with its eyes open. Laurel has only the first six, and the wrathful ones,
/// on Tret's wrathful face, are never asked for here.
enum KolimaForestTreeGesture {
  FACE_GESTURE_CLOSE_EYES = 0,
  FACE_GESTURE_OPEN_EYES = 1,
  FACE_GESTURE_LOOK_LEFT = 2,
  FACE_GESTURE_LOOK_RIGHT = 3,
  FACE_GESTURE_BLINK = 4, ///< twice
  FACE_GESTURE_HALF_CLOSE_EYES = 5,
  FACE_GESTURE_WRATHFUL_CLOSE_EYES = 6,
  FACE_GESTURE_WRATHFUL_OPEN_EYES = 7,
  FACE_GESTURE_WRATHFUL_LOOK_RIGHT = 8,
  FACE_GESTURE_WRATHFUL_LOOK_LEFT = 9,
  FACE_GESTURE_WRATHFUL_BLINK = 10,
  FACE_GESTURE_WRATHFUL_STARE = 11 ///< as FACE_GESTURE_WRATHFUL_OPEN_EYES
};

static void KolimaForestTree_PlayGesture(s32 blankLocalId, s32 gesture);
static void KolimaForestTree_SpawnOrbitSparks(void);
static void KolimaForestTree_ReplaceInventoryItem(s32 oldItemId, s32 newItemId);

s32 gKolimaForestTreeSparkOrigin[3] __attribute__((section(".bss")));
s32 gKolimaForestTreeSparkFrame __attribute__((section(".bss")));
s32 gKolimaForestTreePlaySparkSound __attribute__((section(".bss")));
s32 gKolimaForestTreePaletteFrame __attribute__((section(".bss")));

const struct MapWarpDest *KolimaForestTree_GetWarpDests(void) {
  return gKolimaForestTreeWarpDests;
}

/// The region at Tret's foot that leads into the Tret Tree.
const struct MapWarpRegion *KolimaForestTree_GetRegions(void) {
  return gKolimaForestTreeRegions;
}

/// The ways back (SceneId_KolimaForestFloodgate) and into the Tret Tree
/// (SceneId_TreeRooms).
const u32 *KolimaForestTree_GetWarps(void) { return gKolimaForestTreeWarps; }

/// Tret and Laurel, the blank object events at their spots, the ones the party
/// talks to, Tret's wrathful side, and Garet, Ivan and Mia for the healing.
const struct ObjectEventTemplate *
KolimaForestTree_GetObjectEventTemplates(void) {
  return gKolimaForestTreeObjectEventTemplates;
}

/// Unused: takes the touched warp after the leader's current movement; no
/// map event row names it.
UNUSED static void KolimaForestTree_ExitTouchedRegion(void) {
  struct FieldWork *work;

  work = gFieldWork;
  FieldScript_Start();
  ObjectMotion_CommitPositionAndActivate(LOCALID_PARTY_LEADER, 0, 0);
  Import_ScriptRequestWarp(work->coordEvent);
  FieldScript_End();
}

/// Talking to Tret and Laurel, reading their minds, and Hermes' Water on Tret.
const struct MapEvent *KolimaForestTree_GetEvents(void) {
  return gKolimaForestTreeEvents;
}

/// Kolima Forest (SceneId_KolimaForestTree), talking to Laurel
/// (LOCALID_LAUREL_STAND_IN): the first time, Laurel asks who the party is,
/// recognises the ones who resisted Tret's glamour and senses a great warmth in
/// them, but says they are too late to rescue Tret, and tells them to leave
/// before the forest is destroyed (sets FLAG_MET_LAUREL); then only that
/// warning. Once the curse is lifted, the forest is alive again thanks to the
/// party, and Laurel says, "We shall protect the forest from now on."
static void KolimaForestTree_TalkToLaurel(void) {
  FieldScript_Start();
  KolimaForestTree_PlayGesture(LOCALID_INVISIBLE_LAUREL,
                               FACE_GESTURE_OPEN_EYES);
  if (GameFlag_Get(FLAG_KOLIMA_CURSE_LIFTED) != 0) {
    FieldScript_SetMessageId(MsgKolimaForestKolimaAlive);
    FieldScript_ShowMessageAndWait(LOCALID_LAUREL, 0);
  } else if (Script_GetFlag(FLAG_MET_LAUREL) != 0) {
    FieldScript_SetMessageId(MsgKolimaLeaveBeforeForest);
    FieldScript_ShowMessageAndWait(LOCALID_LAUREL, 0);
  } else {
    FieldScript_SetMessageId(MsgKolimaWho);
    FieldScript_ShowMessageThenWait(LOCALID_LAUREL, 0, 20);
    KolimaForestTree_PlayGesture(LOCALID_INVISIBLE_LAUREL,
                                 FACE_GESTURE_CLOSE_EYES);
    FieldScript_WaitFrames(60);
    KolimaForestTree_PlayGesture(LOCALID_INVISIBLE_LAUREL,
                                 FACE_GESTURE_OPEN_EYES);
    FieldScript_ShowMessageThenWait(LOCALID_LAUREL, 0, 10);
    ObjectMotion_SetAnimationAndWait(LOCALID_PARTY_LEADER, ANIM_NOD);
    FieldScript_WaitFrames(40);
    FieldScript_ShowMessageAndWait(LOCALID_LAUREL, 0);
    KolimaForestTree_PlayGesture(LOCALID_INVISIBLE_LAUREL,
                                 FACE_GESTURE_CLOSE_EYES);
    FieldScript_WaitFrames(80);
    FieldScript_ShowMessageThenWait(LOCALID_LAUREL, 0, 20);
    KolimaForestTree_PlayGesture(LOCALID_INVISIBLE_LAUREL,
                                 FACE_GESTURE_OPEN_EYES);
    FieldScript_ShowMessageThenWait(LOCALID_LAUREL, 0, 20);
    GameFlag_Set(FLAG_MET_LAUREL);
  }
  KolimaForestTree_PlayGesture(LOCALID_INVISIBLE_LAUREL,
                               FACE_GESTURE_CLOSE_EYES);
  FieldScript_End();
}

/// Kolima Forest (SceneId_KolimaForestTree), talking to Tret
/// (LOCALID_TRET_STAND_IN); until FLAG_TRET_DEFEATED Mind Read on him runs this
/// too, with no effect of its own (trigger bit 10). Before the Tret Tree, he is
/// silent and his wrathful side answers, the screen darkening around its words;
/// after it he is silent, and if anyone carries Hermes' Water the main menu
/// opens. Once the curse is lifted, no axe shall touch him again.
static void KolimaForestTree_TalkToTret(void) {
  s32 waterHolder;

  FieldScript_Start();
  if (GameFlag_Get(FLAG_KOLIMA_CURSE_LIFTED) != 0) {
    KolimaForestTree_PlayGesture(LOCALID_INVISIBLE_TRET,
                                 FACE_GESTURE_OPEN_EYES);
    FieldScript_SetMessageId(MsgKolimaNowHaveSuchPowerAxe);
    Script_ShowMessage(LOCALID_TRET, 0);
    KolimaForestTree_PlayGesture(LOCALID_INVISIBLE_TRET,
                                 FACE_GESTURE_CLOSE_EYES);
  } else {
    if (GameFlag_Get(FLAG_TRET_DEFEATED) != 0) {
      KolimaForestTree_PlayGesture(LOCALID_INVISIBLE_TRET,
                                   FACE_GESTURE_OPEN_EYES);
      FieldScript_SetMessageId(MsgKolimaSilence2);
      Script_ShowMessage(LOCALID_TRET, 0);
      KolimaForestTree_PlayGesture(LOCALID_INVISIBLE_TRET,
                                   FACE_GESTURE_CLOSE_EYES);
      waterHolder = PartyInventory_FindMember(ITEM_HERMES_WATER);
      if (waterHolder != -1) {
        u16 *menuRequest = &gFieldWork->menu;
        u16 open = 1;

        *menuRequest = open;
      }
    } else {
      FieldScript_SetMessageId(MsgKolimaSilence);
      Script_ShowMessage(LOCALID_TRET, 0);
      PaletteFade_ApplyTarget(PALETTE_TINT_BLEND | RGB(24, 16, 24),
                              PALETTES_BG);
      PaletteFade_StartInterpolation(20);
      WaitFrames(40);
      Script_ShowMessageAndWait(
          SPEAKER_PORTRAIT_RIGHT | LOCALID_TRETS_WRATHFUL_SIDE, 0, 10);
      ObjectMotion_SetVarCbAndRefresh(LOCALID_PARTY_LEADER, 2);
      Script_ShowMessage(SPEAKER_PORTRAIT_RIGHT | LOCALID_TRETS_WRATHFUL_SIDE,
                         0);
      PaletteFade_ApplyTarget(PALETTE_TINT_NONE, PALETTES_BG);
      PaletteFade_StartInterpolation(20);
      WaitFrames(40);
    }
  }
  FieldScript_End();
}

/// Kolima Forest (SceneId_KolimaForestTree), Mind Read on Laurel: before the
/// curse is lifted, the healing waters of Mercury Lighthouse might save Tret,
/// but its fountain has dried up; afterwards, it must be horrible beyond the
/// river.
static void KolimaForestTree_ReadLaurelThoughts(void) {
  FieldScript_Start();
  if (GameFlag_Get(FLAG_KOLIMA_CURSE_LIFTED) != 0) {
    FieldScript_SetMessageId(MsgKolimaMustHorribleBeyondRiverAm);
  } else {
    FieldScript_SetMessageId(MsgKolimaHealingWatersMercuryLighthouseMight);
  }
  FieldScript_ShowMessageAndWait(LOCALID_LAUREL, 0);
  FieldScript_End();
}

/// Kolima Forest (SceneId_KolimaForestTree), Mind Read on Tret: before the
/// curse is lifted, "People of Kolima... Forgive me..."; afterwards, a plea to
/// be careful beyond the river.
static void KolimaForestTree_ReadTretThoughts(void) {
  FieldScript_Start();
  if (GameFlag_Get(FLAG_KOLIMA_CURSE_LIFTED) != 0) {
    FieldScript_SetMessageId(MsgKolimaKnowCannotStopButPlease);
  } else {
    FieldScript_SetMessageId(MsgKolimaPeopleKolimaForgiveMe);
  }
  FieldScript_ShowMessageAndWait(LOCALID_TRET, 0);
  FieldScript_End();
}

/// Kolima Forest (SceneId_KolimaForestTree), using Hermes' Water on Tret while
/// the curse lasts: before the evil within him is defeated in the Tret Tree
/// (FLAG_TRET_DEFEATED), his wrathful side repels it
/// (KolimaForestTree_RejectHermesWater); afterwards it heals him
/// (KolimaForestTree_RestoreTret).
static void KolimaForestTree_UseHermesWater(void) {
  FieldScript_Start();
  Field_EndBusyAndReveal();
  if (GameFlag_Get(FLAG_TRET_DEFEATED) == 0) {
    KolimaForestTree_RejectHermesWater();
  } else {
    KolimaForestTree_RestoreTret();
  }
  FieldScript_End();
}

/// Tret's wrathful side repels Hermes' Water: sparks gather, the leader is
/// thrown back and lands, and the voice says "I control Tret's heart! I shall
/// not let you do anything to him!"
static void KolimaForestTree_RejectHermesWater(void) {
  struct ObjectEvent *leader;

  // The wrathful side throws the leader back before the water can heal Tret.
  leader = ObjectEvent_Get(LOCALID_PARTY_LEADER);
  ObjectMotion_ArmCallback(LOCALID_PARTY_LEADER, FACING_NORTH, 0);
  PaletteFade_ApplyTarget(PALETTE_TINT_BLEND | RGB(24, 16, 24), PALETTES_BG);
  PaletteFade_StartInterpolation(20);
  WaitFrames(40);
  Script_PlaySong(SONG_FADE_OUT_BGM);
  gKolimaForestTreePlaySparkSound = 1;
  FieldScheduler_AddCallback(KolimaForestTree_SpawnRejectionSpark,
                             TASK_PRIORITY_MAP);
  WaitFrames(30);
  gKolimaForestTreePlaySparkSound = 0;
  FieldCamera_MoveTo(PIXELS(328), -1, PIXELS(235), 1);
  ObjectMotion_SetActionVariant(LOCALID_PARTY_LEADER, 1);
  ObjectEvent_GetById(LOCALID_PARTY_LEADER)->steeringFlags &=
      ~OBJ_EVENT_FACES_MOTION;
  Import_ObjectEventSetAnimation(LOCALID_PARTY_LEADER, ANIM_LEADER_STRIDE);
  ObjectEvent_SetSpeed(LOCALID_PARTY_LEADER, PIXELS(2), PIXELS(2));
  Script_PlaySong(SE_IMPACT);
  leader->velocityY = PIXELS(5);
  leader->gravity = PIXELS(1) / 4;
  leader->bounce = 5 * Q16_ONE / 8;
  ObjectEvent_MoveToAndWait(LOCALID_PARTY_LEADER, 335, 258);
  while (leader->velocityY >= 0) {
    WaitFrames(1);
  }
  do {
    WaitFrames(1);
  } while (leader->velocityY <= 0);
  Script_PlaySong(SE_RATTLE);
  Import_ObjectEventSetAnimation(LOCALID_PARTY_LEADER, ANIM_COLLAPSE);
  FieldScript_WaitFrames(120);
  Task_Remove(KolimaForestTree_SpawnRejectionSpark);
  WaitFrames(40);
  leader->bounce = Q16_ONE / 4;
  {
    struct ObjectEvent *leader = ObjectEvent_Get(LOCALID_PARTY_LEADER);
    leader->steeringFlags = OBJ_EVENT_FACES_MOTION | leader->steeringFlags;
  }
  FieldScript_WaitFrames(80);
  // Hear the wrathful side's refusal, then restore the leader's motion.
  FieldScript_SetMessageId(MsgKolimaControlTretsHeartShallNot);
  Script_ShowMessageAndWait(
      SPEAKER_PORTRAIT_RIGHT | LOCALID_TRETS_WRATHFUL_SIDE, 0, 20);
  ObjectMotion_SetVarCbAndRefresh(LOCALID_PARTY_LEADER, 2);
  FieldScript_WaitFrames(20);
  Script_ShowMessage(SPEAKER_PORTRAIT_RIGHT | LOCALID_TRETS_WRATHFUL_SIDE, 0);
  Scene_PlayMusic();
  PaletteFade_ApplyTarget(PALETTE_TINT_NONE, PALETTES_BG);
  PaletteFade_StartInterpolation(20);
  WaitFrames(40);
  {
    s32 facing = FACING_NORTH;

    leader->facing = facing;
  }
  leader->gravity = PIXELS(1);
  leader->bounce = Q16_ONE / 4;
  ObjectMotion_SetVarCbAndRefresh(LOCALID_PARTY_LEADER, 2);
  FieldScript_WaitFrames(40);
  ObjectMotion_Launch(LOCALID_PARTY_LEADER, 4, 0);
  Import_ObjectEventSetAnimation(LOCALID_PARTY_LEADER, ANIM_STAND);
  FieldScript_WaitFrames(20);
}

/// Gives all four companions the same nod, waiting on Ivan's animation.
static __inline__ void KolimaForestTree_NodTogether(void) {
  Import_ObjectEventSetAnimation(LOCALID_PARTY_LEADER, ANIM_NOD);
  Import_ObjectEventSetAnimation(LOCALID_GARET, ANIM_NOD);
  Import_ObjectEventSetAnimation(LOCALID_MIA, ANIM_NOD);
  ObjectMotion_SetAnimationAndWait(LOCALID_IVAN, ANIM_NOD);
}

/// Turns the companions together and pauses after the final facing change.
static __inline__ void KolimaForestTree_FaceParty(s32 facing, s32 waitFrames) {
  ObjectEvent_FaceDirection(LOCALID_PARTY_LEADER, facing, 0);
  ObjectEvent_FaceDirection(LOCALID_GARET, facing, 0);
  ObjectEvent_FaceDirection(LOCALID_MIA, facing, 0);
  ObjectEvent_FaceDirection(LOCALID_IVAN, facing, waitFrames);
}

/// Hermes' Water heals Tret. The companions gather as the water seeps in and
/// the forest's healthy colours spread; Tret feels life flowing in him again
/// and, asked by Laurel and the party, frees the people he turned into trees.
/// He and Laurel tell how the gems fell from the sky into their branches and
/// twisted their anger and sorrow, and that only the elder trees were changed;
/// Laurel has seen that an evil forest has risen south of the river, and Tret,
/// asking whether the party will go there, warns them to hurry. Tret thanks
/// the party for saving Kolima, the forest and himself. Sets
/// FLAG_KOLIMA_CURSE_LIFTED and turns the Hermes' Water back into an Empty
/// Bottle.
static void KolimaForestTree_RestoreTret(void) {
  s32 includeMia;
  struct ObjectEvent *leader;
  s32 *paletteFrame;
  s32 flash;
  void (*spawnOrbitSparks)(void);
  const s32 *finalActions;

  // Gather the companions while Hermes' Water seeps into Tret.
  includeMia = GameFlag_Get(FLAG_IN_PARTY(MIA));
  ObjectEvent_WalkToAndWait(LOCALID_PARTY_LEADER, 328, 212);
  ObjectEvent_FaceDirection(LOCALID_PARTY_LEADER, FACING_NORTH, 20);
  Script_PlaySong(SONG_FADE_OUT_BGM);
  FieldMessage_Show(MsgKolimaWaterHermesSeepedIntoTret, 1);
  ObjectEvent_SetSpeed(LOCALID_GARET, PIXELS(1), PIXELS(1) / 2);
  ObjectEvent_SetSpeed(LOCALID_IVAN, PIXELS(1), PIXELS(1) / 2);
  leader = ObjectEvent_Get(LOCALID_PARTY_LEADER);
  if (leader != 0) {
    ObjectEvent_SetPosition(LOCALID_GARET, leader->x.fixed, leader->z.fixed);
  }
  leader = ObjectEvent_Get(LOCALID_PARTY_LEADER);
  if (leader != 0) {
    ObjectEvent_SetPosition(LOCALID_IVAN, leader->x.fixed, leader->z.fixed);
  }
  ObjectEvent_SetActionScript(LOCALID_GARET,
                              gKolimaForestTree_GaretApproachActions);
  ObjectEvent_SetActionScript(LOCALID_IVAN,
                              gKolimaForestTree_IvanApproachActions);
  if (includeMia != 0) {
    ObjectEvent_SetSpeed(LOCALID_MIA, PIXELS(1), PIXELS(1) / 2);
    leader = ObjectEvent_Get(LOCALID_PARTY_LEADER);
    if (leader != 0) {
      ObjectEvent_SetPosition(LOCALID_MIA, leader->x.fixed, leader->z.fixed);
    }
    ObjectEvent_SetActionScript(LOCALID_MIA,
                                gKolimaForestTree_MiaApproachActions);
  }
  ObjectEvent_RefreshSelectorById(LOCALID_IVAN);
  FieldScript_WaitFrames(40);
  KolimaPalette_Restore(0);
  PaletteFade_StartInterpolation(32);
  WaitFrames(40);
  // Carry the healing across the forest as its healthy colours return.
  paletteFrame = &gKolimaForestTreePaletteFrame;
  *paletteFrame = 0;
  FieldScheduler_AddCallback(KolimaForestTree_UpdateHealingPalette,
                             TASK_PRIORITY_MAP);
  FieldScript_WaitFrames(40);
  ObjectEvent_FaceDirection(LOCALID_GARET, FACING_SOUTHWEST, 20);
  FieldCamera_SetSpeed(PIXELS(16) / 5, PIXELS(2) / 5);
  FieldCamera_MoveTo(PIXELS(256), -1, PIXELS(254), 1);
  FieldCamera_WaitForArrival();
  Script_PlaySong(SE_GLITTER);
  FieldScript_WaitFrames(40);
  ObjectEvent_FaceDirection(LOCALID_IVAN, FACING_SOUTHEAST, 20);
  FieldCamera_MoveTo(PIXELS(413), -1, PIXELS(261), 1);
  FieldCamera_WaitForArrival();
  Script_PlaySong(SE_GLITTER);
  FieldScript_WaitFrames(40);
  ObjectEvent_FaceDirection(LOCALID_PARTY_LEADER, FACING_SOUTH, 0);
  ObjectEvent_FaceDirection(LOCALID_MIA, FACING_SOUTH, 20);
  FieldCamera_MoveTo(PIXELS(326), -1, PIXELS(384), 1);
  FieldCamera_WaitForArrival();
  Script_PlaySong(SE_GLITTER);
  if (*paletteFrame != 24) {
    do {
      WaitFrames(1);
    } while (*paletteFrame != 24);
  }
  Task_Remove(KolimaForestTree_UpdateHealingPalette);
  WaitFrames(10);
  // Flash the two saved palettes before settling on the healthy forest.
  flash = 0;
  do {
    KolimaPalette_Restore(0);
    PaletteFade_StartInterpolation(6);
    WaitFrames(6);
    KolimaPalette_Restore(1);
    PaletteFade_StartInterpolation(6);
    flash = (flash + 1);
    WaitFrames(6);
  } while ((u32)flash <= 3);
  KolimaPalette_Restore(0);
  PaletteFade_StartInterpolation(40);
  WaitFrames(80);
  FieldCamera_MoveTo(PIXELS(328), PIXELS(8), PIXELS(212), 1);
  FieldCamera_WaitForArrival();
  FieldScript_WaitFrames(40);
  KolimaForestTree_PlayGesture(LOCALID_INVISIBLE_TRET, FACE_GESTURE_OPEN_EYES);
  FieldScript_WaitFrames(40);
  Script_PlaySong(MUS_COLOSSO_INTRO);
  // Tret and Laurel discover that their powers have returned.
  FieldScript_SetMessageId(MsgKolimaFeelGreatPowerSpreadingThrough);
  Script_ShowMessage(LOCALID_TRET, 0);
  ObjectMotion_SetVariantCallback(LOCALID_PARTY_LEADER, 2);
  ObjectMotion_SetVariantCallback(LOCALID_GARET, 2);
  ObjectMotion_SetVariantCallback(LOCALID_MIA, 2);
  ObjectMotion_SetVarCbAndRefresh(LOCALID_IVAN, 2);
  KolimaForestTree_FaceParty(FACING_NORTH, 20);
  KolimaForestTree_PlayGesture(LOCALID_INVISIBLE_TRET, FACE_GESTURE_LOOK_LEFT);
  FieldScript_WaitFrames(20);
  KolimaForestTree_PlayGesture(LOCALID_INVISIBLE_TRET, FACE_GESTURE_LOOK_RIGHT);
  FieldScript_WaitFrames(40);
  KolimaForestTree_PlayGesture(LOCALID_INVISIBLE_TRET, FACE_GESTURE_OPEN_EYES);
  FieldScript_WaitFrames(20);
  Script_ShowMessage(LOCALID_TRET, 0);
  ObjectEvent_ShowEmote(LOCALID_PARTY_LEADER, EMOTE_IN_FRONT | EMOTE_ELLIPSIS,
                        0);
  ObjectEvent_ShowEmote(LOCALID_GARET, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 0);
  ObjectEvent_ShowEmote(LOCALID_MIA, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 0);
  ObjectEvent_ShowEmote(LOCALID_IVAN, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 40);
  FieldCamera_MoveTo(PIXELS(234), 0, PIXELS(232), 1);
  FieldCamera_WaitForArrival();
  FieldScript_WaitFrames(40);
  KolimaForestTree_PlayGesture(LOCALID_INVISIBLE_LAUREL,
                               FACE_GESTURE_OPEN_EYES);
  FieldScript_WaitFrames(40);
  KolimaForestTree_PlayGesture(LOCALID_INVISIBLE_LAUREL,
                               FACE_GESTURE_LOOK_RIGHT);
  FieldScript_WaitFrames(20);
  Script_ShowMessageAndWait(SPEAKER_WINDOW_ABOVE | LOCALID_LAUREL, 0, 20);
  KolimaForestTree_PlayGesture(LOCALID_INVISIBLE_LAUREL,
                               FACE_GESTURE_LOOK_LEFT);
  FieldScript_WaitFrames(10);
  ObjectEvent_FaceDirection(LOCALID_PARTY_LEADER, FACING_SOUTHWEST, 0);
  ObjectEvent_FaceDirection(LOCALID_GARET, FACING_SOUTHWEST, 0);
  ObjectEvent_FaceDirection(LOCALID_IVAN, FACING_SOUTHWEST, 0);
  ObjectEvent_FaceDirection(LOCALID_MIA, FACING_SOUTHWEST, 20);
  KolimaForestTree_PlayGesture(LOCALID_INVISIBLE_LAUREL,
                               FACE_GESTURE_LOOK_RIGHT);
  FieldScript_WaitFrames(20);
  KolimaForestTree_PlayGesture(LOCALID_INVISIBLE_LAUREL,
                               FACE_GESTURE_LOOK_LEFT);
  FieldScript_WaitFrames(20);
  KolimaForestTree_PlayGesture(LOCALID_INVISIBLE_LAUREL,
                               FACE_GESTURE_LOOK_RIGHT);
  Script_ShowMessageAndWait(SPEAKER_WINDOW_ABOVE | LOCALID_LAUREL, 0, 10);
  KolimaForestTree_PlayGesture(LOCALID_INVISIBLE_TRET, FACE_GESTURE_CLOSE_EYES);
  FieldScript_WaitFrames(20);
  Script_ShowMessage(SPEAKER_WINDOW_BELOW | LOCALID_TRET, 0);
  KolimaForestTree_PlayGesture(LOCALID_INVISIBLE_TRET, FACE_GESTURE_OPEN_EYES);
  FieldScript_WaitFrames(20);
  Script_OpenMessage(SPEAKER_WINDOW_BELOW | LOCALID_TRET, 0);
  ObjectEvent_FaceDirection(LOCALID_PARTY_LEADER, FACING_NORTH, 0);
  ObjectEvent_FaceDirection(LOCALID_GARET, FACING_NORTHEAST, 0);
  ObjectEvent_FaceDirection(LOCALID_IVAN, FACING_NORTHWEST, 0);
  ObjectEvent_FaceDirection(LOCALID_MIA, FACING_NORTH, 0);
  if (FieldScript_AskYesNoWithObjectEventAnimation(0, 0) == 0) {
    Script_ShowMessage(SPEAKER_WINDOW_ABOVE | LOCALID_LAUREL, 0);
    Script_ShowMessage(SPEAKER_WINDOW_BELOW | LOCALID_TRET, 0);
  } else {
    gFieldWork->message += 2;
    ObjectEvent_ShowEmote(LOCALID_MIA, EMOTE_IN_FRONT | EMOTE_ANGER_VEIN, 0);
    ObjectEvent_ShowEmote(LOCALID_GARET, EMOTE_IN_FRONT | EMOTE_ANGER_VEIN, 0);
    ObjectEvent_ShowEmote(LOCALID_IVAN, EMOTE_IN_FRONT | EMOTE_ANGER_VEIN, 40);
    Import_ObjectEventSetAnimation(LOCALID_GARET, ANIM_SHAKE_HEAD);
    Script_ShowMessage(LOCALID_GARET, 0);
    if (includeMia != 0) {
      ObjectMotion_SetVarCbAndRefresh(LOCALID_MIA, 2);
      Script_ShowMessage(LOCALID_MIA, 0);
    } else {
      gFieldWork->message += 1;
    }
    ObjectMotion_SetAnimationAndWait(LOCALID_IVAN, ANIM_NOD);
    Script_ShowMessage(LOCALID_IVAN, 0);
    Script_ShowMessage(SPEAKER_WINDOW_ABOVE | LOCALID_LAUREL, 0);
    Script_ShowMessage(SPEAKER_WINDOW_BELOW | LOCALID_TRET, 0);
  }
  KolimaForestTree_NodTogether();
  FieldCamera_MoveTo(PIXELS(328), PIXELS(8), PIXELS(212), 1);
  FieldCamera_WaitForArrival();
  FieldScript_WaitFrames(20);
  KolimaForestTree_PlayGesture(LOCALID_INVISIBLE_TRET, FACE_GESTURE_CLOSE_EYES);
  FieldScript_WaitFrames(20);
  KolimaPalette_Restore(0);
  PaletteFade_StartInterpolation(1);
  WaitFrames(1);
  PaletteFade_ApplyTarget(PALETTE_TINT_BLEND | RGB(24, 16, 24), PALETTES_BG);
  PaletteFade_StartInterpolation(40);
  FieldScript_WaitFrames(60);
  gKolimaForestTreeSparkFrame = 0;
  // Tret sends out the power that frees Kolima's villagers.
  gKolimaForestTreeSparkOrigin[0] = PIXELS(328);
  gKolimaForestTreeSparkOrigin[1] = PIXELS(48);
  spawnOrbitSparks = KolimaForestTree_SpawnOrbitSparks;
  gKolimaForestTreeSparkOrigin[2] = PIXELS(205);
  FieldScheduler_AddCallback(spawnOrbitSparks, TASK_PRIORITY_MAP);
  FieldScript_WaitFrames(100);
  Task_Remove(spawnOrbitSparks);
  PaletteFade_ApplyTarget(RGB(31, 31, 31), PALETTES_ALL);
  PaletteFade_StartInterpolation(60);
  FieldScript_WaitFrames(100);
  KolimaPalette_Restore(0);
  PaletteFade_StartInterpolation(20);
  FieldScript_WaitFrames(40);
  KolimaForestTree_PlayGesture(LOCALID_INVISIBLE_TRET, FACE_GESTURE_OPEN_EYES);
  FieldScript_WaitFrames(10);
  FieldScript_SetMessageId(MsgKolimaShouldDoPeopleKolimaCursed);
  Script_ShowMessage(SPEAKER_WINDOW_BELOW | LOCALID_TRET, 0);
  KolimaForestTree_NodTogether();
  FieldCamera_MoveTo(PIXELS(234), 0, PIXELS(232), 1);
  FieldCamera_WaitForArrival();
  FieldScript_WaitFrames(20);
  Script_ShowMessage(SPEAKER_WINDOW_ABOVE | LOCALID_LAUREL, 0);
  Script_ShowMessageAndWait(SPEAKER_WINDOW_BELOW | LOCALID_TRET, 0, 10);
  ObjectMotion_SetVarCbAndRefresh(LOCALID_GARET, 2);
  ObjectEvent_FaceDirection(LOCALID_PARTY_LEADER, FACING_SOUTHWEST, 0);
  ObjectEvent_FaceDirection(LOCALID_GARET, FACING_NORTHEAST, 10);
  Script_OpenMessage(LOCALID_GARET, 0);
  if (FieldScript_AskYesNoWithObjectEventAnimation(0, 0) == 0) {
    ObjectEvent_ShowEmote(LOCALID_GARET, EMOTE_IN_FRONT | EMOTE_WINCE, 40);
  } else {
    ObjectMotion_SetAnimationAndWait(LOCALID_GARET, ANIM_SHAKE_HEAD);
    gFieldWork->message += 1;
  }
  Script_ShowMessage(LOCALID_GARET, 0);
  KolimaForestTree_PlayGesture(LOCALID_INVISIBLE_TRET, FACE_GESTURE_BLINK);
  FieldScript_WaitFrames(20);
  // Explain the curse and how the falling Psynergy Stones changed the forest.
  FieldScript_SetMessageId(MsgKolimaWasIndeedAngryPeopleHad);
  Script_ShowMessage(SPEAKER_WINDOW_BELOW | LOCALID_TRET, 0);
  ObjectEvent_FaceDirection(LOCALID_PARTY_LEADER, FACING_NORTH, 0);
  ObjectEvent_FaceDirection(LOCALID_GARET, FACING_NORTH, 0);
  Script_ShowMessage(SPEAKER_WINDOW_BELOW | LOCALID_TRET, 0);
  KolimaForestTree_NodTogether();
  KolimaForestTree_PlayGesture(LOCALID_INVISIBLE_TRET, FACE_GESTURE_BLINK);
  FieldScript_WaitFrames(20);
  Script_ShowMessageAndWait(SPEAKER_WINDOW_BELOW | LOCALID_TRET, 0, 20);
  KolimaForestTree_PlayGesture(LOCALID_INVISIBLE_LAUREL,
                               FACE_GESTURE_CLOSE_EYES);
  Script_ShowMessageAndWait(SPEAKER_WINDOW_ABOVE | LOCALID_LAUREL, 0, 20);
  KolimaForestTree_PlayGesture(LOCALID_INVISIBLE_LAUREL,
                               FACE_GESTURE_LOOK_RIGHT);
  FieldScript_WaitFrames(40);
  KolimaForestTree_PlayGesture(LOCALID_INVISIBLE_LAUREL,
                               FACE_GESTURE_OPEN_EYES);
  FieldScript_WaitFrames(20);
  Script_ShowMessageAndWait(SPEAKER_WINDOW_ABOVE | LOCALID_LAUREL, 0, 20);
  KolimaForestTree_PlayGesture(LOCALID_INVISIBLE_TRET, FACE_GESTURE_LOOK_LEFT);
  FieldScript_WaitFrames(20);
  Script_ShowMessage(SPEAKER_WINDOW_BELOW | LOCALID_TRET, 0);
  ObjectEvent_ShowEmote(LOCALID_PARTY_LEADER, EMOTE_IN_FRONT | EMOTE_WINCE, 0);
  ObjectEvent_ShowEmote(LOCALID_GARET, EMOTE_IN_FRONT | EMOTE_WINCE, 0);
  ObjectEvent_ShowEmote(LOCALID_MIA, EMOTE_IN_FRONT | EMOTE_WINCE, 0);
  ObjectEvent_ShowEmote(LOCALID_IVAN, EMOTE_IN_FRONT | EMOTE_WINCE, 80);
  KolimaForestTree_PlayGesture(LOCALID_INVISIBLE_LAUREL,
                               FACE_GESTURE_HALF_CLOSE_EYES);
  FieldScript_WaitFrames(60);
  KolimaForestTree_PlayGesture(LOCALID_INVISIBLE_LAUREL,
                               FACE_GESTURE_LOOK_RIGHT);
  FieldScript_WaitFrames(20);
  Script_ShowMessageAndWait(SPEAKER_WINDOW_ABOVE | LOCALID_LAUREL, 0, 20);
  KolimaForestTree_PlayGesture(LOCALID_INVISIBLE_TRET,
                               FACE_GESTURE_HALF_CLOSE_EYES);
  FieldScript_WaitFrames(40);
  KolimaForestTree_PlayGesture(LOCALID_INVISIBLE_TRET, FACE_GESTURE_LOOK_LEFT);
  FieldScript_WaitFrames(20);
  Script_ShowMessageAndWait(SPEAKER_WINDOW_ABOVE | LOCALID_TRET, 0, 20);
  ObjectMotion_SetVarCbAndRefresh(LOCALID_GARET, 2);
  ObjectEvent_FaceDirection(LOCALID_GARET, FACING_WEST, 10);
  Script_ShowMessage(LOCALID_GARET, 0);
  ObjectEvent_FaceDirection(LOCALID_IVAN, FACING_WEST, 20);
  Script_ShowMessage(SPEAKER_WINDOW_BELOW | LOCALID_IVAN, 0);
  KolimaForestTree_PlayGesture(LOCALID_INVISIBLE_LAUREL, FACE_GESTURE_BLINK);
  FieldScript_WaitFrames(20);
  Script_ShowMessageAndWait(SPEAKER_WINDOW_ABOVE | LOCALID_LAUREL, 0, 20);
  ObjectEvent_FaceDirection(LOCALID_GARET, FACING_NORTHEAST, 0);
  ObjectEvent_FaceDirection(LOCALID_PARTY_LEADER, FACING_SOUTHWEST, 10);
  Script_ShowMessageAndWait(LOCALID_GARET, 0, 20);
  KolimaForestTree_PlayGesture(LOCALID_INVISIBLE_TRET, FACE_GESTURE_OPEN_EYES);
  Script_ShowMessageAndWait(SPEAKER_WINDOW_BELOW | LOCALID_TRET, 0, 10);
  KolimaForestTree_PlayGesture(LOCALID_INVISIBLE_TRET, FACE_GESTURE_LOOK_LEFT);
  FieldScript_WaitFrames(20);
  KolimaForestTree_PlayGesture(LOCALID_INVISIBLE_LAUREL,
                               FACE_GESTURE_LOOK_RIGHT);
  FieldScript_WaitFrames(40);
  KolimaForestTree_PlayGesture(LOCALID_INVISIBLE_LAUREL,
                               FACE_GESTURE_CLOSE_EYES);
  FieldScript_WaitFrames(20);
  KolimaPalette_Restore(0);
  PaletteFade_StartInterpolation(1);
  WaitFrames(1);
  PaletteFade_ApplyTarget(PALETTE_TINT_BLEND | RGB(24, 16, 24), PALETTES_BG);
  PaletteFade_StartInterpolation(40);
  FieldScript_WaitFrames(60);
  gKolimaForestTreeSparkFrame = 0;
  // Laurel looks beyond the river and warns of the forest to the south.
  gKolimaForestTreeSparkOrigin[0] = PIXELS(136);
  gKolimaForestTreeSparkOrigin[1] = PIXELS(20);
  spawnOrbitSparks = KolimaForestTree_SpawnOrbitSparks;
  gKolimaForestTreeSparkOrigin[2] = PIXELS(258);
  FieldScheduler_AddCallback(spawnOrbitSparks, TASK_PRIORITY_MAP);
  FieldScript_WaitFrames(100);
  KolimaForestTree_FaceParty(FACING_SOUTHWEST, 40);
  ObjectMotion_SetVariantCallback(LOCALID_IVAN, 1);
  ObjectEvent_ShowEmote(LOCALID_IVAN, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 20);
  Script_ShowMessageAndWait(SPEAKER_WINDOW_BELOW | LOCALID_IVAN, 0, 10);
  ObjectMotion_SetVarCbAndRefresh(LOCALID_PARTY_LEADER, 2);
  ObjectEvent_FaceDirection(LOCALID_PARTY_LEADER, FACING_SOUTHEAST, 10);
  ObjectMotion_SetAnimationAndWait(LOCALID_PARTY_LEADER, ANIM_NOD);
  KolimaForestTree_PlayGesture(LOCALID_INVISIBLE_TRET, FACE_GESTURE_BLINK);
  FieldScript_WaitFrames(20);
  Script_ShowMessage(SPEAKER_WINDOW_BELOW | LOCALID_TRET, 0);
  ObjectEvent_ShowEmote(LOCALID_IVAN, EMOTE_IN_FRONT | EMOTE_QUESTION, 60);
  ObjectEvent_FaceDirection(LOCALID_IVAN, FACING_NORTH, 10);
  Script_ShowMessageAndWait(SPEAKER_WINDOW_BELOW | LOCALID_IVAN, 0, 10);
  ObjectEvent_FaceDirection(LOCALID_PARTY_LEADER, FACING_NORTH, 0);
  ObjectEvent_FaceDirection(LOCALID_MIA, FACING_NORTH, 0);
  ObjectEvent_FaceDirection(LOCALID_GARET, FACING_NORTH, 20);
  Script_ShowMessageAndWait(SPEAKER_WINDOW_BELOW | LOCALID_TRET, 0, 10);
  KolimaForestTree_NodTogether();
  FieldScript_WaitFrames(10);
  KolimaForestTree_FaceParty(FACING_SOUTHWEST, 120);
  Task_Remove(spawnOrbitSparks);
  FieldScript_WaitFrames(60);
  KolimaPalette_Restore(0);
  PaletteFade_StartInterpolation(40);
  KolimaForestTree_PlayGesture(LOCALID_INVISIBLE_TRET, FACE_GESTURE_LOOK_LEFT);
  FieldScript_WaitFrames(20);
  Script_ShowMessageAndWait(SPEAKER_WINDOW_BELOW | LOCALID_TRET, 0, 20);
  KolimaForestTree_PlayGesture(LOCALID_INVISIBLE_LAUREL,
                               FACE_GESTURE_LOOK_RIGHT);
  Script_ShowMessage(SPEAKER_WINDOW_ABOVE | LOCALID_LAUREL, 0);
  Script_ShowMessage(SPEAKER_WINDOW_BELOW | LOCALID_TRET, 0);
  KolimaForestTree_PlayGesture(LOCALID_INVISIBLE_LAUREL, FACE_GESTURE_BLINK);
  FieldScript_WaitFrames(20);
  Script_ShowMessageAndWait(SPEAKER_WINDOW_ABOVE | LOCALID_LAUREL, 0, 10);
  ObjectMotion_SetVariantCallback(LOCALID_PARTY_LEADER, 2);
  ObjectMotion_SetVariantCallback(LOCALID_GARET, 2);
  ObjectMotion_SetVariantCallback(LOCALID_MIA, 2);
  ObjectMotion_SetVarCbAndRefresh(LOCALID_IVAN, 2);
  KolimaForestTree_PlayGesture(LOCALID_INVISIBLE_TRET, FACE_GESTURE_OPEN_EYES);
  Script_OpenMessage(SPEAKER_WINDOW_BELOW | LOCALID_TRET, 0);
  ObjectEvent_FaceDirection(LOCALID_PARTY_LEADER, FACING_NORTH, 0);
  ObjectEvent_FaceDirection(LOCALID_GARET, FACING_NORTHEAST, 0);
  ObjectEvent_FaceDirection(LOCALID_MIA, FACING_NORTH, 0);
  ObjectEvent_FaceDirection(LOCALID_IVAN, FACING_NORTHWEST, 0);
  if (FieldScript_AskYesNoWithObjectEventAnimation(0, 0) == 1) {
    gFieldWork->message += 1;
  }
  FieldScript_WaitFrames(10);
  KolimaForestTree_PlayGesture(LOCALID_INVISIBLE_TRET, FACE_GESTURE_LOOK_LEFT);
  FieldScript_WaitFrames(20);
  KolimaForestTree_PlayGesture(LOCALID_INVISIBLE_LAUREL,
                               FACE_GESTURE_LOOK_RIGHT);
  FieldScript_WaitFrames(40);
  KolimaForestTree_PlayGesture(LOCALID_INVISIBLE_TRET, FACE_GESTURE_OPEN_EYES);
  FieldScript_WaitFrames(20);
  Script_ShowMessageAndWait(SPEAKER_WINDOW_BELOW | LOCALID_TRET, 0, 10);
  ObjectEvent_FaceDirection(LOCALID_PARTY_LEADER, FACING_SOUTH, 0);
  ObjectEvent_FaceDirection(LOCALID_GARET, FACING_EAST, 0);
  ObjectEvent_FaceDirection(LOCALID_MIA, FACING_NORTH, 0);
  ObjectEvent_FaceDirection(LOCALID_IVAN, FACING_WEST, 10);
  KolimaForestTree_NodTogether();
  Script_PlaySong(SONG_FADE_OUT_BGM);
  // Rejoin the party and exchange the used water for its empty bottle.
  finalActions = gKolimaForestTree_RejoinPartyActions;
  ObjectEvent_SetActionScript(LOCALID_GARET, finalActions);
  if (includeMia != 0) {
    ObjectEvent_SetActionScript(LOCALID_MIA, finalActions);
  }
  ObjectEvent_RunActionScriptAndWait(LOCALID_IVAN, finalActions);
  KolimaForestTree_PlayGesture(LOCALID_INVISIBLE_TRET, FACE_GESTURE_BLINK);
  KolimaForestTree_PlayGesture(LOCALID_INVISIBLE_TRET, FACE_GESTURE_BLINK);
  FieldScript_WaitFrames(20);
  FieldScript_SetMessageId(MsgKolimaOweGreatDebtHaveSaved);
  Script_ShowMessage(SPEAKER_WINDOW_BELOW | LOCALID_TRET, 0);
  KolimaForestTree_PlayGesture(LOCALID_INVISIBLE_LAUREL, FACE_GESTURE_BLINK);
  KolimaForestTree_PlayGesture(LOCALID_INVISIBLE_LAUREL, FACE_GESTURE_BLINK);
  FieldScript_WaitFrames(20);
  Script_ShowMessageAndWait(SPEAKER_WINDOW_ABOVE | LOCALID_LAUREL, 0, 10);
  ObjectMotion_SetAnimationAndWait(LOCALID_PARTY_LEADER, ANIM_NOD);
  GameFlag_Set(FLAG_KOLIMA_CURSE_LIFTED);
  Script_PlaySong(MUS_TOWN);
  KolimaForestTree_ReplaceInventoryItem(ITEM_HERMES_WATER, ITEM_EMPTY_BOTTLE);
}

/// Sets up Tret, Laurel and the forest's colours on entry. Tret, Laurel and the
/// wrathful side stand 28 pixels up, nothing passes through them, Retreat is
/// set to the forest's way in, and the forest takes its cursed colours until
/// the curse is lifted.
s32 KolimaForestTree_Initialize(void) {
  struct ObjectEvent *tretBlank;
  struct ObjectEvent *tretsWrathfulSide;
  struct ObjectEvent *laurelBlank;

  tretBlank = ObjectEvent_GetById(LOCALID_INVISIBLE_TRET);
  tretsWrathfulSide = ObjectEvent_GetById(LOCALID_TRETS_WRATHFUL_SIDE);
  laurelBlank = ObjectEvent_GetById(LOCALID_INVISIBLE_LAUREL);
  WaitFrames(1);
  ObjectEvent_ConfigureSpriteColor(LOCALID_TRETS_WRATHFUL_SIDE,
                                   SPRITE_COLOR_HIDDEN);
  gFieldWork->transition =
      SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_INTERLACED_WIPE);
  gSaveBlock.retreatScene = (s32)&SceneId_KolimaForestEntrance;
  gSaveBlock.retreatWarpDest = KOLIMA_FOREST_ENTRANCE_FROM_WORLD_MAP;
  if (!GameFlag_Get(FLAG_KOLIMA_CURSE_LIFTED))
    KolimaPalette_Curse(3);
  ObjectEvent_GetById(LOCALID_TRET)->radius = 6;
  ObjectEvent_GetById(LOCALID_LAUREL)->radius = 6;
  ObjectEvent_GetById(LOCALID_TRET_STAND_IN)->radius = 6;
  ObjectEvent_GetById(LOCALID_LAUREL_STAND_IN)->radius = 6;
  ObjectEvent_SetSpriteFlags(ObjectEvent_GetById(LOCALID_TRETS_WRATHFUL_SIDE),
                             0);
  ObjectEvent_SetSpriteFlags(ObjectEvent_GetById(LOCALID_INVISIBLE_TRET), 0);
  ObjectEvent_SetSpriteFlags(ObjectEvent_GetById(LOCALID_INVISIBLE_LAUREL), 0);
  ObjectEvent_SetSpriteFlags(ObjectEvent_GetById(LOCALID_TRET), 0);
  ObjectEvent_SetSpriteFlags(ObjectEvent_GetById(LOCALID_LAUREL), 0);
  ObjectMotion_SetActionVariant(LOCALID_TRET, 2);
  ObjectMotion_SetActionVariant(LOCALID_TRETS_WRATHFUL_SIDE, 2);
  ObjectMotion_SetActionVariant(LOCALID_LAUREL, 2);
  tretBlank->motionFlags = 0;
  tretBlank->y.fixed = PIXELS(28);
  laurelBlank->motionFlags = 0;
  laurelBlank->y.fixed = PIXELS(28);
  tretsWrathfulSide->motionFlags = 0;
  tretsWrathfulSide->y.fixed = PIXELS(28);
  Import_ObjectEventSetAnimation(LOCALID_LAUREL, LAUREL_FACE_CLOSED);
  Import_ObjectEventSetAnimation(LOCALID_TRET, TRET_FACE_CLOSED);
  ObjectEvent_GetById(LOCALID_TRET)->collisionFlags |= OBJ_EVENT_NOT_TARGETED;
  ObjectEvent_GetById(LOCALID_LAUREL)->collisionFlags |= OBJ_EVENT_NOT_TARGETED;
  ObjectEvent_GetById(LOCALID_INVISIBLE_TRET)->collisionFlags |=
      OBJ_EVENT_NOT_TARGETED;
  ObjectEvent_GetById(LOCALID_INVISIBLE_LAUREL)->collisionFlags |=
      OBJ_EVENT_NOT_TARGETED;
  ObjectEvent_GetById(LOCALID_TRETS_WRATHFUL_SIDE)->collisionFlags |=
      OBJ_EVENT_NOT_TARGETED;
  return 0;
}

#include "common/kolima_grow.h"

/// Spawns the growing sparks that repel the party leader.
static void KolimaForestTree_SpawnRejectionSpark(void) {
  struct ObjectEvent *effect;
  struct Sprite *sprite;
  s32 phase;
  s32 effectFlags;
  s32 spawnX = PIXELS(326);
  s32 spawnY = PIXELS(32);
  s32 spawnZ = PIXELS(192);
  s32 targetX = PIXELS(326);
  s32 targetZ = PIXELS(240);

  phase = gFrameCount & 3;
  if (phase != 0)
    return;
  if (gKolimaForestTreePlaySparkSound != 0)
    Sound_PlaySong(SE_SPARKLE_FALL);
  effect = ObjectEvent_Create(SPRITE_GOLDEN_RINGS, spawnX, spawnY, spawnZ);
  if (effect == 0)
    return;
  sprite = effect->sprite;
  sprite->flags = phase;
  effectFlags = 0xff & ~OBJ_EVENT_PRIORITY_AUTOMATIC;
  effectFlags &= effect->priorityFlags;
  effect->priorityFlags = effectFlags;
  sprite->image.priority = 1;
  effect->scaleX = Q16_ONE / 10;
  effect->speed = PIXELS(4);
  effect->acceleration = PIXELS(4);
  effect->motionFlags = phase;
  Object_SelectAnimation(effect, 2);
  Object_SetPosition(effect, targetX, 0, targetZ);
  ObjectEvent_SetScript(effect, gKolimaForestTreeEffectScript);
}

/// Alternates the saved healthy and withered palettes over thirty frames.
static void KolimaForestTree_UpdateHealingPalette(void) {
  s32 step = gKolimaForestTreePaletteFrame;

  if (step == 0) {
    KolimaPalette_Restore(0);
    PaletteFade_StartInterpolation(20);
  } else if (step == 20) {
    KolimaPalette_Restore(1);
    PaletteFade_StartInterpolation(8);
  }
  step = gKolimaForestTreePaletteFrame + 1;
  gKolimaForestTreePaletteFrame = step;
  if (step == 30) {
    gKolimaForestTreePaletteFrame = 0;
  }
}

/// Alternates two facial frames twice, ending on the first frame.
static __inline__ void KolimaForestTree_AlternateFaceFrames(s32 localId,
                                                            s32 firstFrame,
                                                            s32 secondFrame) {
  Import_ObjectEventSetAnimation(localId, firstFrame);
  WaitFrames(6);
  Import_ObjectEventSetAnimation(localId, secondFrame);
  WaitFrames(6);
  Import_ObjectEventSetAnimation(localId, firstFrame);
  WaitFrames(6);
  Import_ObjectEventSetAnimation(localId, secondFrame);
  WaitFrames(6);
  Import_ObjectEventSetAnimation(localId, firstFrame);
}

/// Plays one of Tret's or Laurel's face gestures, chosen by the blank object
/// event at their spot.
static void KolimaForestTree_PlayGesture(s32 blankLocalId, s32 gesture) {
  if (blankLocalId == LOCALID_INVISIBLE_TRET) {
    switch (gesture) {
    case FACE_GESTURE_CLOSE_EYES:
      Import_ObjectEventSetAnimation(LOCALID_TRET, TRET_FACE_OPEN);
      WaitFrames(6);
      Import_ObjectEventSetAnimation(LOCALID_TRET, TRET_FACE_CLOSED);
      break;
    case FACE_GESTURE_OPEN_EYES:
      Import_ObjectEventSetAnimation(LOCALID_TRET, TRET_FACE_OPEN);
      break;
    case FACE_GESTURE_LOOK_LEFT:
      Import_ObjectEventSetAnimation(LOCALID_TRET, TRET_FACE_OPEN);
      WaitFrames(6);
      Import_ObjectEventSetAnimation(LOCALID_TRET, TRET_FACE_LOOK_LEFT);
      break;
    case FACE_GESTURE_LOOK_RIGHT:
      Import_ObjectEventSetAnimation(LOCALID_TRET, TRET_FACE_OPEN);
      WaitFrames(6);
      Import_ObjectEventSetAnimation(LOCALID_TRET, TRET_FACE_LOOK_RIGHT);
      break;
    case FACE_GESTURE_BLINK:
      KolimaForestTree_AlternateFaceFrames(LOCALID_TRET, TRET_FACE_OPEN,
                                           TRET_FACE_CLOSED);
      break;
    case FACE_GESTURE_HALF_CLOSE_EYES:
      Import_ObjectEventSetAnimation(LOCALID_TRET, TRET_FACE_OPEN);
      WaitFrames(6);
      Import_ObjectEventSetAnimation(LOCALID_TRET, TRET_FACE_HALF_CLOSED);
      break;
    case FACE_GESTURE_WRATHFUL_CLOSE_EYES:
      Import_ObjectEventSetAnimation(LOCALID_TRET, TRET_FACE_WRATHFUL_OPEN);
      WaitFrames(6);
      Import_ObjectEventSetAnimation(LOCALID_TRET, TRET_FACE_WRATHFUL_CLOSED);
      break;
    case FACE_GESTURE_WRATHFUL_LOOK_RIGHT:
      Import_ObjectEventSetAnimation(LOCALID_TRET, TRET_FACE_WRATHFUL_OPEN);
      WaitFrames(6);
      Import_ObjectEventSetAnimation(LOCALID_TRET,
                                     TRET_FACE_WRATHFUL_LOOK_RIGHT);
      break;
    case FACE_GESTURE_WRATHFUL_LOOK_LEFT:
      Import_ObjectEventSetAnimation(LOCALID_TRET, TRET_FACE_WRATHFUL_OPEN);
      WaitFrames(6);
      Import_ObjectEventSetAnimation(LOCALID_TRET,
                                     TRET_FACE_WRATHFUL_LOOK_LEFT);
      break;
    case FACE_GESTURE_WRATHFUL_BLINK:
      KolimaForestTree_AlternateFaceFrames(
          LOCALID_TRET, TRET_FACE_WRATHFUL_OPEN, TRET_FACE_WRATHFUL_CLOSED);
      break;
    case FACE_GESTURE_WRATHFUL_OPEN_EYES:
    case FACE_GESTURE_WRATHFUL_STARE:
      Import_ObjectEventSetAnimation(LOCALID_TRET, TRET_FACE_WRATHFUL_OPEN);
      break;
    }
  } else {
    switch (gesture) {
    case FACE_GESTURE_CLOSE_EYES:
      Import_ObjectEventSetAnimation(LOCALID_LAUREL, LAUREL_FACE_OPEN);
      WaitFrames(6);
      Import_ObjectEventSetAnimation(LOCALID_LAUREL, LAUREL_FACE_CLOSED);
      break;
    case FACE_GESTURE_OPEN_EYES:
      Import_ObjectEventSetAnimation(LOCALID_LAUREL, LAUREL_FACE_OPEN);
      break;
    case FACE_GESTURE_LOOK_LEFT:
      Import_ObjectEventSetAnimation(LOCALID_LAUREL, LAUREL_FACE_OPEN);
      WaitFrames(6);
      Import_ObjectEventSetAnimation(LOCALID_LAUREL, LAUREL_FACE_LOOK_LEFT);
      break;
    case FACE_GESTURE_LOOK_RIGHT:
      Import_ObjectEventSetAnimation(LOCALID_LAUREL, LAUREL_FACE_OPEN);
      WaitFrames(6);
      Import_ObjectEventSetAnimation(LOCALID_LAUREL, LAUREL_FACE_LOOK_RIGHT);
      break;
    case FACE_GESTURE_BLINK:
      KolimaForestTree_AlternateFaceFrames(LOCALID_LAUREL, LAUREL_FACE_OPEN,
                                           LAUREL_FACE_CLOSED);
      break;
    case FACE_GESTURE_HALF_CLOSE_EYES:
      Import_ObjectEventSetAnimation(LOCALID_LAUREL, LAUREL_FACE_OPEN);
      WaitFrames(6);
      Import_ObjectEventSetAnimation(LOCALID_LAUREL, LAUREL_FACE_HALF_CLOSED);
      break;
    }
  }
  WaitFrames(12);
}

/// Expands a spark along its spiral for 120 frames, then releases its cache
/// slot.
static void
KolimaForestTree_UpdateOrbitSpark(union ObjectEventOrEffect *fieldObject) {
  struct ObjectEvent *spark = &fieldObject->objectEvent;
  s32 position[3];
  s32 age = spark->timer;
  s32 heading;

  if (age <= 119) {
    SET_VECTOR(position, spark->targetX, spark->targetY, spark->targetZ);
    heading = (s16)spark->work;
    Vector_AddPolarOffset(age * Q16_ONE, age * 768 + heading, position);
    spark->x.fixed = position[0];
    spark->y.fixed = position[1];
    spark->z.fixed = position[2];
    spark->scaleX += Q16_ONE / 200;
    spark->scaleY += Q16_ONE / 200;
    spark->timer++;
  } else {
    VramCache_FreeEntry(spark->sprite->slot);
    ObjectEvent_Release(spark);
  }
}

/// Emits six-spark rings at ten-frame intervals during each burst.
static void KolimaForestTree_SpawnOrbitSparks(void) {
  struct ObjectEvent *spark;
  struct Sprite *previousSprite;
  u32 sparkIndex;

  previousSprite = NULL;
  switch (gKolimaForestTreeSparkFrame) {
  case 0:
  case 10:
  case 20:
  case 30:
  case 40:
    Sound_PlaySong(SE_SHIMMER);
    for (sparkIndex = 0; sparkIndex <= 5; sparkIndex++) {
      spark = ObjectEvent_Create(
          SPRITE_SPARKLE, gKolimaForestTreeSparkOrigin[0],
          gKolimaForestTreeSparkOrigin[1], gKolimaForestTreeSparkOrigin[2]);
      if (spark != 0) {
        previousSprite =
            Sprite_ShareVramCacheEntry(spark->sprite, previousSprite);
        spark->motionFlags = 0;
        spark->sprite->image.priority = 1;
        ObjectEvent_SetSpriteFlags(spark, 0);
        Object_SelectAnimation(spark, 1);
        spark->timer = 0;
        spark->work = (sparkIndex * 60 * Q16_ONE) / 360;
        spark->targetX = gKolimaForestTreeSparkOrigin[0];
        spark->targetY = gKolimaForestTreeSparkOrigin[1];
        spark->targetZ = gKolimaForestTreeSparkOrigin[2];
        spark->speed = PIXELS(8) / 5;
        spark->update = KolimaForestTree_UpdateOrbitSpark;
      }
    }
    break;
  }
  if (++gKolimaForestTreeSparkFrame > 120) {
    gKolimaForestTreeSparkFrame = 0;
  }
}

/// Replaces the used water with its empty bottle in the same inventory slot.
static void KolimaForestTree_ReplaceInventoryItem(s32 oldItemId,
                                                  s32 newItemId) {
  s32 member = PartyInventory_FindMember(oldItemId);

  if (member != -1) {
    s32 inventorySlot = Inventory_Find(member, oldItemId);

    if (inventorySlot != -1) {
      Unit_Get(member)->inventory[inventorySlot] = newItemId;
    }
  }
}

#include "common/kolima_palette.h"

s32 gKolimaForestTree_GaretApproachActions[] = {
    SC_ANIMATION(2), SC_MOVE(PIXELS(312), 0, PIXELS(230)), SC_SYNC,
    SC_ANIMATION(1), SC_SET(SW_FACING, FACING_NORTH),      SC_WAIT(1),
    SC_END,
};

s32 gKolimaForestTree_IvanApproachActions[] = {
    SC_ANIMATION(2), SC_MOVE(PIXELS(346), 0, PIXELS(224)), SC_SYNC,
    SC_ANIMATION(1), SC_SET(SW_FACING, FACING_NORTH),      SC_WAIT(1),
    SC_END,
};

s32 gKolimaForestTree_MiaApproachActions[] = {
    SC_ANIMATION(2), SC_MOVE(PIXELS(333), 0, PIXELS(240)), SC_SYNC,
    SC_ANIMATION(1), SC_SET(SW_FACING, FACING_NORTH),      SC_WAIT(1),
    SC_END,
};

s32 gKolimaForestTree_RejoinPartyActions[] = {
    SC_ANIMATION(2), SC_MOVE(PIXELS(328), 0, PIXELS(212)),
    SC_SYNC,         SC_POSITION(0, 0, 0),
    SC_WAIT(1),      SC_END,
};

struct MapWarpDest gKolimaForestTreeWarpDests[] = {
    {WARP_DEST_DEFAULT, CONDITION_ALWAYS, 209, 0, 279, FACING_SOUTH, 0, 0, 0,
     512, 448, 0},
    {KOLIMA_FOREST_TREE_FROM_KOLIMA_FOREST_FLOODGATE, CONDITION_ALWAYS, 136, 0,
     424, FACING_NORTH, 0, 0, 0, 512, 448, 0},
    {KOLIMA_FOREST_TREE_FROM_TREE_ROOMS, CONDITION_ALWAYS, 328, 16, 200,
     FACING_SOUTH, 0, 0, 0, 512, 448, 0},
    {MAP_TABLE_END},
};

// clang-format off
struct MapWarpRegion gKolimaForestTreeRegions[] = {
    {320, 88, 176, 336, 104, 192, -1, KOLIMA_FOREST_TREE_TO_TREE_ROOMS},
    {-1, 0, 0, 0, 0, 0, 0, 0},
};

u32 gKolimaForestTreeWarps[] = {
    MAP_WARPS(KolimaForestTree),
    MAP_WARP(KOLIMA_FOREST_TREE_TO_KOLIMA_FOREST_FLOODGATE, KolimaForestFloodgate, KOLIMA_FOREST_FLOODGATE_FROM_KOLIMA_FOREST_TREE),
    MAP_WARP(KOLIMA_FOREST_TREE_TO_TREE_ROOMS, TreeRooms, TREE_ROOMS_FROM_KOLIMA_FOREST_TREE),
    MAP_WARPS_END,
};

struct ObjectEventTemplate gKolimaForestTreeObjectEventTemplates[] = {
    { SPRITE_TRET_RAISED, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(327), 0, PIXELS(192), FACING_SOUTH, TALK_FACING_KEEP, 0 },
    { SPRITE_LAUREL, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(136), 0, PIXELS(270), FACING_SOUTH, TALK_FACING_KEEP, 0 },
    { SPRITE_BLANK, CONDITION_ALWAYS, MOVEMENT_TYPE_RESET_ACTION,
        PIXELS(327), 0, PIXELS(192), FACING_SOUTH, TALK_FACING_KEEP, 0 },
    { SPRITE_BLANK, CONDITION_ALWAYS, MOVEMENT_TYPE_RESET_ACTION,
        PIXELS(136), 0, PIXELS(270), FACING_SOUTH, TALK_FACING_KEEP, 0 },
    { SPRITE_BLANK, CONDITION_ALWAYS, MOVEMENT_TYPE_RESET_ACTION,
        PIXELS(327), 0, PIXELS(194), FACING_SOUTH, TALK_FACING_KEEP, 0 },
    { SPRITE_BLANK, CONDITION_ALWAYS, MOVEMENT_TYPE_RESET_ACTION,
        PIXELS(136), 0, PIXELS(272), FACING_SOUTH, TALK_FACING_KEEP, 0 },
    { SPRITE_TRET, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(327), 0, PIXELS(192), FACING_SOUTH, TALK_FACING_KEEP, 0 },
    { LOCALID_GARET, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        0, 0, 0, FACING_EAST, TALK_FACING_KEEP, 0 },
    { LOCALID_IVAN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        0, 0, 0, FACING_EAST, TALK_FACING_KEEP, 0 },
    { LOCALID_MIA, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        0, 0, 0, FACING_EAST, TALK_FACING_KEEP, 0 },
    { MAP_TABLE_END },
};

struct MapEvent gKolimaForestTreeEvents[] = {
    { MAP_EVENT_WARP, 1, CONDITION_ALWAYS, KOLIMA_FOREST_TREE_TO_KOLIMA_FOREST_FLOODGATE },
    { USE_ITEM_ON_OBJ_EVENT(ITEM_HERMES_WATER), LOCALID_TRET_STAND_IN, FLAG_KOLIMA_CURSE_LIFTED, EVENT_SCRIPT(KolimaForestTree_UseHermesWater) },
    { MAP_EVENT_TALK, LOCALID_LAUREL_STAND_IN, CONDITION_ALWAYS, EVENT_SCRIPT(KolimaForestTree_TalkToLaurel) },
    { MAP_EVENT_TALK, LOCALID_TRET_STAND_IN, CONDITION_ALWAYS, EVENT_SCRIPT(KolimaForestTree_TalkToTret) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_LAUREL_STAND_IN, CONDITION_ALWAYS, EVENT_SCRIPT(KolimaForestTree_ReadLaurelThoughts) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), 0x400 | LOCALID_TRET_STAND_IN, FLAG_TRET_DEFEATED, EVENT_SCRIPT(KolimaForestTree_TalkToTret) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_TRET_STAND_IN, CONDITION_ALWAYS, EVENT_SCRIPT(KolimaForestTree_ReadTretThoughts) },
    { MAP_EVENTS_END },
};
// clang-format on

s32 gKolimaForestTreeEffectScript[] = {
    SC_ADD(SW_SCALE_X, Q16_ONE / 10),
    SC_CALL(FieldEffect_StretchUntilArrived),
    SC_LOOP(SC_FOREVER, SC_TOP),
};
