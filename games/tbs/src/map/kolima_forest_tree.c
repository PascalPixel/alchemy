/// @file
/// Kolima Forest (MapId_KolimaForestTree), the last of the forest's five
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
/// region at his foot is KOLIMA_FOREST_TREE_TO_TRET_TREE_INTERIORS, into the
/// Tret Tree (MapId_TretTreeInteriors), where the party defeats the evil
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
/// one
/// (https://alchemy.pascalpixel.com/places/kolima-forest-tree/healing-palette.gif:
/// the screen beside those palettes, from the line that the water has seeped
/// into Tret until Tret speaks). Retreat from here always leads back to the
/// forest's way in (MapId_KolimaForestEntrance,
/// KOLIMA_FOREST_ENTRANCE_FROM_WORLD_MAP), and the way back,
/// KOLIMA_FOREST_TREE_TO_KOLIMA_FOREST_FLOODGATE, leads to the Floodgate
/// Switch's map (MapId_KolimaForestFloodgate).
///
/// Open questions:
/// - XXX (no evidence): why Tret and Laurel each have a blank object event that
/// only
///   selects whose face KolimaForestTree_PlayGesture moves, likely left from a
///   version where those object events were drawn; the ROM keeps no such
///   version, and nothing reads the object events but that function.

#include "battle_effect_runtime.h"
#include "game_state.h"
#include "unit/map/kolima_forest_tree.h"
#include "unit/map/common/kolima_palette.h"
#include "unit/object/object.h"
#include "unit/object/psynergy_effects.h"
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
#include "map_ids.h"
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
  /// Blank object events 28 pixels up at Tret's and Laurel's spots, never
  /// targeted;
  /// nothing addresses them but KolimaForestTree_PlayGesture, which takes one
  /// to say whose face to move.
  /// ★ Named from the game: "Tret" (MsgEnemyName)
  LOCALID_TRET_BLANK,
  /// ★ Named from the game: "Laurel" (MsgDebugGlyphName)
  LOCALID_LAUREL_BLANK,
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

/// The ways back (MapId_KolimaForestFloodgate) and into the Tret Tree
/// (MapId_TretTreeInteriors).
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
  struct EventWork *scene;

  scene = gFieldWork;
  Engine_ScriptStart();
  Engine_ObjectEventWalkByAndWait(LOCALID_PARTY_LEADER, 0, 0);
  Engine_ScriptRequestWarp(scene->coordEvent);
  Engine_ScriptEnd();
}

/// Talking to Tret and Laurel, reading their minds, and Hermes' Water on Tret.
const struct MapEvent *KolimaForestTree_GetEvents(void) {
  return gKolimaForestTreeEvents;
}

/// Kolima Forest (MapId_KolimaForestTree), talking to Laurel
/// (LOCALID_LAUREL_STAND_IN): the first time, Laurel asks who the party is,
/// recognises the ones who resisted Tret's glamour and senses a great warmth in
/// them, but says they are too late to rescue Tret, and tells them to leave
/// before the forest is destroyed (sets FLAG_MET_LAUREL); then only that
/// warning. Once the curse is lifted, the forest is alive again thanks to the
/// party, and Laurel says, "We shall protect the forest from now on."
static void KolimaForestTree_TalkToLaurel(void) {
  Engine_ScriptStart();
  KolimaForestTree_PlayGesture(LOCALID_LAUREL_BLANK, FACE_GESTURE_OPEN_EYES);
  if (Engine_GameFlagIsSet(FLAG_KOLIMA_CURSE_LIFTED) != 0) {
    Engine_ScriptSetMessage(MsgKolimaForestKolimaAlive);
    Engine_ScriptShowMessage(LOCALID_LAUREL, 0);
  } else if (GameFlag_IsSet(FLAG_MET_LAUREL) != 0) {
    Engine_ScriptSetMessage(MsgKolimaLeaveBeforeForest);
    Engine_ScriptShowMessage(LOCALID_LAUREL, 0);
  } else {
    Engine_ScriptSetMessage(MsgKolimaWho);
    Engine_ScriptShowMessageAndWait(LOCALID_LAUREL, 0, 20);
    KolimaForestTree_PlayGesture(LOCALID_LAUREL_BLANK, FACE_GESTURE_CLOSE_EYES);
    Engine_ScriptWait(60);
    KolimaForestTree_PlayGesture(LOCALID_LAUREL_BLANK, FACE_GESTURE_OPEN_EYES);
    Engine_ScriptShowMessageAndWait(LOCALID_LAUREL, 0, 10);
    Engine_ObjectEventSetAnimationAndWait(LOCALID_PARTY_LEADER, ANIMATION_NOD);
    Engine_ScriptWait(40);
    Engine_ScriptShowMessage(LOCALID_LAUREL, 0);
    KolimaForestTree_PlayGesture(LOCALID_LAUREL_BLANK, FACE_GESTURE_CLOSE_EYES);
    Engine_ScriptWait(80);
    Engine_ScriptShowMessageAndWait(LOCALID_LAUREL, 0, 20);
    KolimaForestTree_PlayGesture(LOCALID_LAUREL_BLANK, FACE_GESTURE_OPEN_EYES);
    Engine_ScriptShowMessageAndWait(LOCALID_LAUREL, 0, 20);
    Engine_GameFlagSet(FLAG_MET_LAUREL);
  }
  KolimaForestTree_PlayGesture(LOCALID_LAUREL_BLANK, FACE_GESTURE_CLOSE_EYES);
  Engine_ScriptEnd();
}

/// Kolima Forest (MapId_KolimaForestTree), talking to Tret
/// (LOCALID_TRET_STAND_IN); until FLAG_TRET_DEFEATED Mind Read on him runs this
/// too, with no effect of its own (trigger bit 10). Before the Tret Tree, he is
/// silent and his wrathful side answers, the screen darkening around its words;
/// after it he is silent, and if anyone carries Hermes' Water the main menu
/// opens. Once the curse is lifted, no axe shall touch him again.
static void KolimaForestTree_TalkToTret(void) {
  s32 waterOwner;

  Engine_ScriptStart();
  if (GameFlag_IsSet(FLAG_KOLIMA_CURSE_LIFTED) != 0) {
    KolimaForestTree_PlayGesture(LOCALID_TRET_BLANK, FACE_GESTURE_OPEN_EYES);
    Engine_ScriptSetMessage(MsgKolimaNowHaveSuchPowerAxe);
    Script_ShowMessage(LOCALID_TRET, 0);
    KolimaForestTree_PlayGesture(LOCALID_TRET_BLANK, FACE_GESTURE_CLOSE_EYES);
  } else {
    if (GameFlag_IsSet(FLAG_TRET_DEFEATED) != 0) {
      KolimaForestTree_PlayGesture(LOCALID_TRET_BLANK, FACE_GESTURE_OPEN_EYES);
      Engine_ScriptSetMessage(MsgKolimaSilence2);
      Script_ShowMessage(LOCALID_TRET, 0);
      KolimaForestTree_PlayGesture(LOCALID_TRET_BLANK, FACE_GESTURE_CLOSE_EYES);
      waterOwner = PartyInventory_FindMember(ITEM_HERMES_WATER);
      if (waterOwner != -1) {
        u16 *menuRequest = &gFieldWork->menu;
        u16 open = 1;

        *menuRequest = open;
      }
    } else {
      Engine_ScriptSetMessage(MsgKolimaSilence);
      Script_ShowMessage(LOCALID_TRET, 0);
      PaletteFade_ApplyTarget(0x406218, 1);
      Engine_ColorBufferInterpolate(20);
      Engine_TaskWait(40);
      Script_ShowMessageAndWait(
          SPEAKER_PORTRAIT_RIGHT | LOCALID_TRETS_WRATHFUL_SIDE, 0, 10);
      Engine_ObjectEventRunRepeatedMotion(LOCALID_PARTY_LEADER, 2);
      Script_ShowMessage(SPEAKER_PORTRAIT_RIGHT | LOCALID_TRETS_WRATHFUL_SIDE,
                         0);
      PaletteFade_ApplyTarget(PALETTE_TINT_NONE, 1);
      Engine_ColorBufferInterpolate(20);
      Engine_TaskWait(40);
    }
  }
  Engine_ScriptEnd();
}

/// Kolima Forest (MapId_KolimaForestTree), Mind Read on Laurel: before the
/// curse is lifted, the healing waters of Mercury Lighthouse might save Tret,
/// but its fountain has dried up; afterwards, it must be horrible beyond the
/// river.
static void KolimaForestTree_ReadLaurelThoughts(void) {
  Engine_ScriptStart();
  if (Engine_GameFlagIsSet(FLAG_KOLIMA_CURSE_LIFTED) != 0) {
    Engine_ScriptSetMessage(MsgKolimaMustHorribleBeyondRiverAm);
  } else {
    Engine_ScriptSetMessage(MsgKolimaHealingWatersMercuryLighthouseMight);
  }
  Engine_ScriptShowMessage(LOCALID_LAUREL, 0);
  Engine_ScriptEnd();
}

/// Kolima Forest (MapId_KolimaForestTree), Mind Read on Tret: before the
/// curse is lifted, "People of Kolima... Forgive me..."; afterwards, a plea to
/// be careful beyond the river.
static void KolimaForestTree_ReadTretThoughts(void) {
  Engine_ScriptStart();
  if (Engine_GameFlagIsSet(FLAG_KOLIMA_CURSE_LIFTED) != 0) {
    Engine_ScriptSetMessage(MsgKolimaKnowCannotStopButPlease);
  } else {
    Engine_ScriptSetMessage(MsgKolimaPeopleKolimaForgiveMe);
  }
  Engine_ScriptShowMessage(LOCALID_TRET, 0);
  Engine_ScriptEnd();
}

/// Kolima Forest (MapId_KolimaForestTree), using Hermes' Water on Tret while
/// the curse lasts: before the evil within him is defeated in the Tret Tree
/// (FLAG_TRET_DEFEATED), his wrathful side repels it
/// (KolimaForestTree_RejectHermesWater); afterwards it heals him
/// (KolimaForestTree_RestoreTret).
static void KolimaForestTree_UseHermesWater(void) {
  Engine_ScriptStart();
  Field_EndBusyAndReveal();
  if (Engine_GameFlagIsSet(FLAG_TRET_DEFEATED) == 0) {
    KolimaForestTree_RejectHermesWater();
  } else {
    KolimaForestTree_RestoreTret();
  }
  Engine_ScriptEnd();
}

/// Tret's wrathful side repels Hermes' Water: sparks gather, the leader is
/// thrown back and lands, and the voice says "I control Tret's heart! I shall
/// not let you do anything to him!"
static void KolimaForestTree_RejectHermesWater(void) {
  struct ObjectEvent *leader;

  // The wrathful side throws the leader back before the water can heal Tret.
  leader = ObjectEvent_Get(LOCALID_PARTY_LEADER);
  Engine_ObjectEventFaceDirection(LOCALID_PARTY_LEADER, FACING_NORTH, 0);
  PaletteFade_ApplyTarget(0x406218, 1);
  Engine_ColorBufferInterpolate(20);
  Engine_TaskWait(40);
  Script_PlaySong(SONG_FADE_OUT_BGM);
  gKolimaForestTreePlaySparkSound = 1;
  FieldScheduler_AddCallback(KolimaForestTree_SpawnRejectionSpark,
                             TASK_PRIORITY_MAP);
  Engine_TaskWait(30);
  gKolimaForestTreePlaySparkSound = 0;
  FieldCamera_MoveTo(PIXELS(328), -1, PIXELS(235), 1);
  Engine_ObjectEventSetSpritePriority(LOCALID_PARTY_LEADER, 1);
  ObjectEvent_GetById(LOCALID_PARTY_LEADER)->steeringFlags &=
      ~OBJ_EVENT_FACES_MOTION;
  Engine_ObjectEventSetAnimation(LOCALID_PARTY_LEADER, ANIMATION_LEADER_STRIDE);
  ObjectEvent_SetSpeed(LOCALID_PARTY_LEADER, PIXELS(2), PIXELS(2));
  Script_PlaySong(SE_IMPACT);
  leader->velocityY = PIXELS(5);
  leader->gravity = PIXELS(1) / 4;
  leader->bounce = 5 * Q16_ONE / 8;
  ObjectEvent_MoveToAndWait(LOCALID_PARTY_LEADER, 335, 258);
  while (leader->velocityY >= 0) {
    Engine_TaskWait(1);
  }
  do {
    Engine_TaskWait(1);
  } while (leader->velocityY <= 0);
  Script_PlaySong(SE_161);
  Engine_ObjectEventSetAnimation(LOCALID_PARTY_LEADER, ANIMATION_COLLAPSE);
  Engine_ScriptWait(120);
  Engine_TaskRemoveCallback(KolimaForestTree_SpawnRejectionSpark);
  Engine_TaskWait(40);
  leader->bounce = Q16_ONE / 4;
  {
    struct ObjectEvent *leader = ObjectEvent_Get(LOCALID_PARTY_LEADER);
    leader->steeringFlags = OBJ_EVENT_FACES_MOTION | leader->steeringFlags;
  }
  Engine_ScriptWait(80);
  // Hear the wrathful side's refusal, then restore the leader's motion.
  Engine_ScriptSetMessage(MsgKolimaControlTretsHeartShallNot);
  Script_ShowMessageAndWait(
      SPEAKER_PORTRAIT_RIGHT | LOCALID_TRETS_WRATHFUL_SIDE, 0, 20);
  Engine_ObjectEventRunRepeatedMotion(LOCALID_PARTY_LEADER, 2);
  Engine_ScriptWait(20);
  Script_ShowMessage(SPEAKER_PORTRAIT_RIGHT | LOCALID_TRETS_WRATHFUL_SIDE, 0);
  Map_PlayMusic();
  PaletteFade_ApplyTarget(PALETTE_TINT_NONE, 1);
  Engine_ColorBufferInterpolate(20);
  Engine_TaskWait(40);
  {
    s32 facing = FACING_NORTH;

    leader->facing = facing;
  }
  leader->gravity = PIXELS(1);
  leader->bounce = Q16_ONE / 4;
  Engine_ObjectEventRunRepeatedMotion(LOCALID_PARTY_LEADER, 2);
  Engine_ScriptWait(40);
  Engine_ObjectEventJump(LOCALID_PARTY_LEADER, 4, 0);
  Engine_ObjectEventSetAnimation(LOCALID_PARTY_LEADER, ANIMATION_STAND);
  Engine_ScriptWait(20);
}

/// Gives all four companions the same nod, waiting on Ivan's animation.
static __inline__ void KolimaForestTree_NodTogether(void) {
  Engine_ObjectEventSetAnimation(LOCALID_PARTY_LEADER, ANIMATION_NOD);
  Engine_ObjectEventSetAnimation(GARET, ANIMATION_NOD);
  Engine_ObjectEventSetAnimation(MIA, ANIMATION_NOD);
  Engine_ObjectEventSetAnimationAndWait(IVAN, ANIMATION_NOD);
}

/// Turns the companions together and pauses after the final facing change.
static __inline__ void KolimaForestTree_FaceParty(s32 facing, s32 waitFrames) {
  ObjectEvent_FaceDirection(LOCALID_PARTY_LEADER, facing, 0);
  ObjectEvent_FaceDirection(GARET, facing, 0);
  ObjectEvent_FaceDirection(MIA, facing, 0);
  ObjectEvent_FaceDirection(IVAN, facing, waitFrames);
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
  includeMia = GameFlag_IsSet(FLAG_IN_PARTY(MIA));
  ObjectEvent_WalkToAndWait(LOCALID_PARTY_LEADER, 328, 212);
  ObjectEvent_FaceDirection(LOCALID_PARTY_LEADER, FACING_NORTH, 20);
  Script_PlaySong(SONG_FADE_OUT_BGM);
  Engine_MessageShowCentered(MsgKolimaWaterHermesSeepedIntoTret, 1);
  ObjectEvent_SetSpeed(GARET, PIXELS(1), PIXELS(1) / 2);
  ObjectEvent_SetSpeed(IVAN, PIXELS(1), PIXELS(1) / 2);
  leader = ObjectEvent_Get(LOCALID_PARTY_LEADER);
  if (leader != 0) {
    ObjectEvent_SetPosition(GARET, leader->x.fixed, leader->z.fixed);
  }
  leader = ObjectEvent_Get(LOCALID_PARTY_LEADER);
  if (leader != 0) {
    ObjectEvent_SetPosition(IVAN, leader->x.fixed, leader->z.fixed);
  }
  ObjectEvent_SetActionScript(GARET, gKolimaForestTree_GaretApproachActions);
  ObjectEvent_SetActionScript(IVAN, gKolimaForestTree_IvanApproachActions);
  if (includeMia != 0) {
    ObjectEvent_SetSpeed(MIA, PIXELS(1), PIXELS(1) / 2);
    leader = ObjectEvent_Get(LOCALID_PARTY_LEADER);
    if (leader != 0) {
      ObjectEvent_SetPosition(MIA, leader->x.fixed, leader->z.fixed);
    }
    ObjectEvent_SetActionScript(MIA, gKolimaForestTree_MiaApproachActions);
  }
  ObjectEvent_RefreshSelectorById(IVAN);
  Engine_ScriptWait(40);
  KolimaPalette_Restore(0);
  Engine_ColorBufferInterpolate(32);
  Engine_TaskWait(40);
  // Carry the healing across the forest as its healthy colours return.
  paletteFrame = &gKolimaForestTreePaletteFrame;
  *paletteFrame = 0;
  FieldScheduler_AddCallback(KolimaForestTree_UpdateHealingPalette,
                             TASK_PRIORITY_MAP);
  Engine_ScriptWait(40);
  ObjectEvent_FaceDirection(GARET, FACING_SOUTHWEST, 20);
  FieldCamera_SetSpeed(PIXELS(16) / 5, PIXELS(2) / 5);
  FieldCamera_MoveTo(PIXELS(256), -1, PIXELS(254), 1);
  Engine_CameraWaitForMove();
  Script_PlaySong(SE_246);
  Engine_ScriptWait(40);
  ObjectEvent_FaceDirection(IVAN, FACING_SOUTHEAST, 20);
  FieldCamera_MoveTo(PIXELS(413), -1, PIXELS(261), 1);
  Engine_CameraWaitForMove();
  Script_PlaySong(SE_246);
  Engine_ScriptWait(40);
  ObjectEvent_FaceDirection(LOCALID_PARTY_LEADER, FACING_SOUTH, 0);
  ObjectEvent_FaceDirection(MIA, FACING_SOUTH, 20);
  FieldCamera_MoveTo(PIXELS(326), -1, PIXELS(384), 1);
  Engine_CameraWaitForMove();
  Script_PlaySong(SE_246);
  if (*paletteFrame != 24) {
    do {
      Engine_TaskWait(1);
    } while (*paletteFrame != 24);
  }
  Engine_TaskRemoveCallback(KolimaForestTree_UpdateHealingPalette);
  Engine_TaskWait(10);
  // Flash the two saved palettes before settling on the healthy forest.
  flash = 0;
  do {
    KolimaPalette_Restore(0);
    Engine_ColorBufferInterpolate(6);
    Engine_TaskWait(6);
    KolimaPalette_Restore(1);
    Engine_ColorBufferInterpolate(6);
    flash = (flash + 1);
    Engine_TaskWait(6);
  } while ((u32)flash <= 3);
  KolimaPalette_Restore(0);
  Engine_ColorBufferInterpolate(40);
  Engine_TaskWait(80);
  FieldCamera_MoveTo(PIXELS(328), PIXELS(8), PIXELS(212), 1);
  Engine_CameraWaitForMove();
  Engine_ScriptWait(40);
  KolimaForestTree_PlayGesture(LOCALID_TRET_BLANK, FACE_GESTURE_OPEN_EYES);
  Engine_ScriptWait(40);
  Script_PlaySong(MUS_007);
  // Tret and Laurel discover that their powers have returned.
  Engine_ScriptSetMessage(MsgKolimaFeelGreatPowerSpreadingThrough);
  Script_ShowMessage(LOCALID_TRET, 0);
  Engine_ObjectEventStartRepeatedMotion(LOCALID_PARTY_LEADER, 2);
  Engine_ObjectEventStartRepeatedMotion(GARET, 2);
  Engine_ObjectEventStartRepeatedMotion(MIA, 2);
  Engine_ObjectEventRunRepeatedMotion(IVAN, 2);
  KolimaForestTree_FaceParty(FACING_NORTH, 20);
  KolimaForestTree_PlayGesture(LOCALID_TRET_BLANK, FACE_GESTURE_LOOK_LEFT);
  Engine_ScriptWait(20);
  KolimaForestTree_PlayGesture(LOCALID_TRET_BLANK, FACE_GESTURE_LOOK_RIGHT);
  Engine_ScriptWait(40);
  KolimaForestTree_PlayGesture(LOCALID_TRET_BLANK, FACE_GESTURE_OPEN_EYES);
  Engine_ScriptWait(20);
  Script_ShowMessage(LOCALID_TRET, 0);
  ObjectEvent_ShowEmote(LOCALID_PARTY_LEADER, EMOTE_IN_FRONT | EMOTE_ELLIPSIS,
                        0);
  ObjectEvent_ShowEmote(GARET, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 0);
  ObjectEvent_ShowEmote(MIA, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 0);
  ObjectEvent_ShowEmote(IVAN, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 40);
  FieldCamera_MoveTo(PIXELS(234), 0, PIXELS(232), 1);
  Engine_CameraWaitForMove();
  Engine_ScriptWait(40);
  KolimaForestTree_PlayGesture(LOCALID_LAUREL_BLANK, FACE_GESTURE_OPEN_EYES);
  Engine_ScriptWait(40);
  KolimaForestTree_PlayGesture(LOCALID_LAUREL_BLANK, FACE_GESTURE_LOOK_RIGHT);
  Engine_ScriptWait(20);
  Script_ShowMessageAndWait(SPEAKER_WINDOW_ABOVE | LOCALID_LAUREL, 0, 20);
  KolimaForestTree_PlayGesture(LOCALID_LAUREL_BLANK, FACE_GESTURE_LOOK_LEFT);
  Engine_ScriptWait(10);
  ObjectEvent_FaceDirection(LOCALID_PARTY_LEADER, FACING_SOUTHWEST, 0);
  ObjectEvent_FaceDirection(GARET, FACING_SOUTHWEST, 0);
  ObjectEvent_FaceDirection(IVAN, FACING_SOUTHWEST, 0);
  ObjectEvent_FaceDirection(MIA, FACING_SOUTHWEST, 20);
  KolimaForestTree_PlayGesture(LOCALID_LAUREL_BLANK, FACE_GESTURE_LOOK_RIGHT);
  Engine_ScriptWait(20);
  KolimaForestTree_PlayGesture(LOCALID_LAUREL_BLANK, FACE_GESTURE_LOOK_LEFT);
  Engine_ScriptWait(20);
  KolimaForestTree_PlayGesture(LOCALID_LAUREL_BLANK, FACE_GESTURE_LOOK_RIGHT);
  Script_ShowMessageAndWait(SPEAKER_WINDOW_ABOVE | LOCALID_LAUREL, 0, 10);
  KolimaForestTree_PlayGesture(LOCALID_TRET_BLANK, FACE_GESTURE_CLOSE_EYES);
  Engine_ScriptWait(20);
  Script_ShowMessage(SPEAKER_WINDOW_BELOW | LOCALID_TRET, 0);
  KolimaForestTree_PlayGesture(LOCALID_TRET_BLANK, FACE_GESTURE_OPEN_EYES);
  Engine_ScriptWait(20);
  Script_OpenMessage(SPEAKER_WINDOW_BELOW | LOCALID_TRET, 0);
  ObjectEvent_FaceDirection(LOCALID_PARTY_LEADER, FACING_NORTH, 0);
  ObjectEvent_FaceDirection(GARET, FACING_NORTHEAST, 0);
  ObjectEvent_FaceDirection(IVAN, FACING_NORTHWEST, 0);
  ObjectEvent_FaceDirection(MIA, FACING_NORTH, 0);
  if (Engine_ScriptChooseYesNo(0, 0) == 0) {
    Script_ShowMessage(SPEAKER_WINDOW_ABOVE | LOCALID_LAUREL, 0);
    Script_ShowMessage(SPEAKER_WINDOW_BELOW | LOCALID_TRET, 0);
  } else {
    gFieldWork->message += 2;
    ObjectEvent_ShowEmote(MIA, EMOTE_IN_FRONT | EMOTE_ANGER_VEIN, 0);
    ObjectEvent_ShowEmote(GARET, EMOTE_IN_FRONT | EMOTE_ANGER_VEIN, 0);
    ObjectEvent_ShowEmote(IVAN, EMOTE_IN_FRONT | EMOTE_ANGER_VEIN, 40);
    Engine_ObjectEventSetAnimation(GARET, ANIMATION_SHAKE_HEAD);
    Script_ShowMessage(GARET, 0);
    if (includeMia != 0) {
      Engine_ObjectEventRunRepeatedMotion(MIA, 2);
      Script_ShowMessage(MIA, 0);
    } else {
      gFieldWork->message += 1;
    }
    Engine_ObjectEventSetAnimationAndWait(IVAN, ANIMATION_NOD);
    Script_ShowMessage(IVAN, 0);
    Script_ShowMessage(SPEAKER_WINDOW_ABOVE | LOCALID_LAUREL, 0);
    Script_ShowMessage(SPEAKER_WINDOW_BELOW | LOCALID_TRET, 0);
  }
  KolimaForestTree_NodTogether();
  FieldCamera_MoveTo(PIXELS(328), PIXELS(8), PIXELS(212), 1);
  Engine_CameraWaitForMove();
  Engine_ScriptWait(20);
  KolimaForestTree_PlayGesture(LOCALID_TRET_BLANK, FACE_GESTURE_CLOSE_EYES);
  Engine_ScriptWait(20);
  KolimaPalette_Restore(0);
  Engine_ColorBufferInterpolate(1);
  Engine_TaskWait(1);
  PaletteFade_ApplyTarget(0x406218, 1);
  Engine_ColorBufferInterpolate(40);
  Engine_ScriptWait(60);
  gKolimaForestTreeSparkFrame = 0;
  // Tret sends out the power that frees Kolima's villagers.
  gKolimaForestTreeSparkOrigin[0] = PIXELS(328);
  gKolimaForestTreeSparkOrigin[1] = PIXELS(48);
  spawnOrbitSparks = KolimaForestTree_SpawnOrbitSparks;
  gKolimaForestTreeSparkOrigin[2] = PIXELS(205);
  FieldScheduler_AddCallback(spawnOrbitSparks, TASK_PRIORITY_MAP);
  Engine_ScriptWait(100);
  Engine_TaskRemoveCallback(spawnOrbitSparks);
  PaletteFade_ApplyTarget(0x7fff, 0);
  Engine_ColorBufferInterpolate(60);
  Engine_ScriptWait(100);
  KolimaPalette_Restore(0);
  Engine_ColorBufferInterpolate(20);
  Engine_ScriptWait(40);
  KolimaForestTree_PlayGesture(LOCALID_TRET_BLANK, FACE_GESTURE_OPEN_EYES);
  Engine_ScriptWait(10);
  Engine_ScriptSetMessage(MsgKolimaShouldDoPeopleKolimaCursed);
  Script_ShowMessage(SPEAKER_WINDOW_BELOW | LOCALID_TRET, 0);
  KolimaForestTree_NodTogether();
  FieldCamera_MoveTo(PIXELS(234), 0, PIXELS(232), 1);
  Engine_CameraWaitForMove();
  Engine_ScriptWait(20);
  Script_ShowMessage(SPEAKER_WINDOW_ABOVE | LOCALID_LAUREL, 0);
  Script_ShowMessageAndWait(SPEAKER_WINDOW_BELOW | LOCALID_TRET, 0, 10);
  Engine_ObjectEventRunRepeatedMotion(GARET, 2);
  ObjectEvent_FaceDirection(LOCALID_PARTY_LEADER, FACING_SOUTHWEST, 0);
  ObjectEvent_FaceDirection(GARET, FACING_NORTHEAST, 10);
  Script_OpenMessage(GARET, 0);
  if (Engine_ScriptChooseYesNo(0, 0) == 0) {
    ObjectEvent_ShowEmote(GARET, EMOTE_IN_FRONT | EMOTE_WINCE, 40);
  } else {
    Engine_ObjectEventSetAnimationAndWait(GARET, ANIMATION_SHAKE_HEAD);
    gFieldWork->message += 1;
  }
  Script_ShowMessage(GARET, 0);
  KolimaForestTree_PlayGesture(LOCALID_TRET_BLANK, FACE_GESTURE_BLINK);
  Engine_ScriptWait(20);
  // Explain the curse and how the falling Psynergy Stones changed the forest.
  Engine_ScriptSetMessage(MsgKolimaWasIndeedAngryPeopleHad);
  Script_ShowMessage(SPEAKER_WINDOW_BELOW | LOCALID_TRET, 0);
  ObjectEvent_FaceDirection(LOCALID_PARTY_LEADER, FACING_NORTH, 0);
  ObjectEvent_FaceDirection(GARET, FACING_NORTH, 0);
  Script_ShowMessage(SPEAKER_WINDOW_BELOW | LOCALID_TRET, 0);
  KolimaForestTree_NodTogether();
  KolimaForestTree_PlayGesture(LOCALID_TRET_BLANK, FACE_GESTURE_BLINK);
  Engine_ScriptWait(20);
  Script_ShowMessageAndWait(SPEAKER_WINDOW_BELOW | LOCALID_TRET, 0, 20);
  KolimaForestTree_PlayGesture(LOCALID_LAUREL_BLANK, FACE_GESTURE_CLOSE_EYES);
  Script_ShowMessageAndWait(SPEAKER_WINDOW_ABOVE | LOCALID_LAUREL, 0, 20);
  KolimaForestTree_PlayGesture(LOCALID_LAUREL_BLANK, FACE_GESTURE_LOOK_RIGHT);
  Engine_ScriptWait(40);
  KolimaForestTree_PlayGesture(LOCALID_LAUREL_BLANK, FACE_GESTURE_OPEN_EYES);
  Engine_ScriptWait(20);
  Script_ShowMessageAndWait(SPEAKER_WINDOW_ABOVE | LOCALID_LAUREL, 0, 20);
  KolimaForestTree_PlayGesture(LOCALID_TRET_BLANK, FACE_GESTURE_LOOK_LEFT);
  Engine_ScriptWait(20);
  Script_ShowMessage(SPEAKER_WINDOW_BELOW | LOCALID_TRET, 0);
  ObjectEvent_ShowEmote(LOCALID_PARTY_LEADER, EMOTE_IN_FRONT | EMOTE_WINCE, 0);
  ObjectEvent_ShowEmote(GARET, EMOTE_IN_FRONT | EMOTE_WINCE, 0);
  ObjectEvent_ShowEmote(MIA, EMOTE_IN_FRONT | EMOTE_WINCE, 0);
  ObjectEvent_ShowEmote(IVAN, EMOTE_IN_FRONT | EMOTE_WINCE, 80);
  KolimaForestTree_PlayGesture(LOCALID_LAUREL_BLANK,
                               FACE_GESTURE_HALF_CLOSE_EYES);
  Engine_ScriptWait(60);
  KolimaForestTree_PlayGesture(LOCALID_LAUREL_BLANK, FACE_GESTURE_LOOK_RIGHT);
  Engine_ScriptWait(20);
  Script_ShowMessageAndWait(SPEAKER_WINDOW_ABOVE | LOCALID_LAUREL, 0, 20);
  KolimaForestTree_PlayGesture(LOCALID_TRET_BLANK,
                               FACE_GESTURE_HALF_CLOSE_EYES);
  Engine_ScriptWait(40);
  KolimaForestTree_PlayGesture(LOCALID_TRET_BLANK, FACE_GESTURE_LOOK_LEFT);
  Engine_ScriptWait(20);
  Script_ShowMessageAndWait(SPEAKER_WINDOW_ABOVE | LOCALID_TRET, 0, 20);
  Engine_ObjectEventRunRepeatedMotion(GARET, 2);
  ObjectEvent_FaceDirection(GARET, FACING_WEST, 10);
  Script_ShowMessage(GARET, 0);
  ObjectEvent_FaceDirection(IVAN, FACING_WEST, 20);
  Script_ShowMessage(SPEAKER_WINDOW_BELOW | IVAN, 0);
  KolimaForestTree_PlayGesture(LOCALID_LAUREL_BLANK, FACE_GESTURE_BLINK);
  Engine_ScriptWait(20);
  Script_ShowMessageAndWait(SPEAKER_WINDOW_ABOVE | LOCALID_LAUREL, 0, 20);
  ObjectEvent_FaceDirection(GARET, FACING_NORTHEAST, 0);
  ObjectEvent_FaceDirection(LOCALID_PARTY_LEADER, FACING_SOUTHWEST, 10);
  Script_ShowMessageAndWait(GARET, 0, 20);
  KolimaForestTree_PlayGesture(LOCALID_TRET_BLANK, FACE_GESTURE_OPEN_EYES);
  Script_ShowMessageAndWait(SPEAKER_WINDOW_BELOW | LOCALID_TRET, 0, 10);
  KolimaForestTree_PlayGesture(LOCALID_TRET_BLANK, FACE_GESTURE_LOOK_LEFT);
  Engine_ScriptWait(20);
  KolimaForestTree_PlayGesture(LOCALID_LAUREL_BLANK, FACE_GESTURE_LOOK_RIGHT);
  Engine_ScriptWait(40);
  KolimaForestTree_PlayGesture(LOCALID_LAUREL_BLANK, FACE_GESTURE_CLOSE_EYES);
  Engine_ScriptWait(20);
  KolimaPalette_Restore(0);
  Engine_ColorBufferInterpolate(1);
  Engine_TaskWait(1);
  PaletteFade_ApplyTarget(0x406218, 1);
  Engine_ColorBufferInterpolate(40);
  Engine_ScriptWait(60);
  gKolimaForestTreeSparkFrame = 0;
  // Laurel looks beyond the river and warns of the forest to the south.
  gKolimaForestTreeSparkOrigin[0] = PIXELS(136);
  gKolimaForestTreeSparkOrigin[1] = PIXELS(20);
  spawnOrbitSparks = KolimaForestTree_SpawnOrbitSparks;
  gKolimaForestTreeSparkOrigin[2] = PIXELS(258);
  FieldScheduler_AddCallback(spawnOrbitSparks, TASK_PRIORITY_MAP);
  Engine_ScriptWait(100);
  KolimaForestTree_FaceParty(FACING_SOUTHWEST, 40);
  Engine_ObjectEventStartRepeatedMotion(IVAN, 1);
  ObjectEvent_ShowEmote(IVAN, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 20);
  Script_ShowMessageAndWait(SPEAKER_WINDOW_BELOW | IVAN, 0, 10);
  Engine_ObjectEventRunRepeatedMotion(LOCALID_PARTY_LEADER, 2);
  ObjectEvent_FaceDirection(LOCALID_PARTY_LEADER, FACING_SOUTHEAST, 10);
  Engine_ObjectEventSetAnimationAndWait(LOCALID_PARTY_LEADER, ANIMATION_NOD);
  KolimaForestTree_PlayGesture(LOCALID_TRET_BLANK, FACE_GESTURE_BLINK);
  Engine_ScriptWait(20);
  Script_ShowMessage(SPEAKER_WINDOW_BELOW | LOCALID_TRET, 0);
  ObjectEvent_ShowEmote(IVAN, EMOTE_IN_FRONT | EMOTE_QUESTION, 60);
  ObjectEvent_FaceDirection(IVAN, FACING_NORTH, 10);
  Script_ShowMessageAndWait(SPEAKER_WINDOW_BELOW | IVAN, 0, 10);
  ObjectEvent_FaceDirection(LOCALID_PARTY_LEADER, FACING_NORTH, 0);
  ObjectEvent_FaceDirection(MIA, FACING_NORTH, 0);
  ObjectEvent_FaceDirection(GARET, FACING_NORTH, 20);
  Script_ShowMessageAndWait(SPEAKER_WINDOW_BELOW | LOCALID_TRET, 0, 10);
  KolimaForestTree_NodTogether();
  Engine_ScriptWait(10);
  KolimaForestTree_FaceParty(FACING_SOUTHWEST, 120);
  Engine_TaskRemoveCallback(spawnOrbitSparks);
  Engine_ScriptWait(60);
  KolimaPalette_Restore(0);
  Engine_ColorBufferInterpolate(40);
  KolimaForestTree_PlayGesture(LOCALID_TRET_BLANK, FACE_GESTURE_LOOK_LEFT);
  Engine_ScriptWait(20);
  Script_ShowMessageAndWait(SPEAKER_WINDOW_BELOW | LOCALID_TRET, 0, 20);
  KolimaForestTree_PlayGesture(LOCALID_LAUREL_BLANK, FACE_GESTURE_LOOK_RIGHT);
  Script_ShowMessage(SPEAKER_WINDOW_ABOVE | LOCALID_LAUREL, 0);
  Script_ShowMessage(SPEAKER_WINDOW_BELOW | LOCALID_TRET, 0);
  KolimaForestTree_PlayGesture(LOCALID_LAUREL_BLANK, FACE_GESTURE_BLINK);
  Engine_ScriptWait(20);
  Script_ShowMessageAndWait(SPEAKER_WINDOW_ABOVE | LOCALID_LAUREL, 0, 10);
  Engine_ObjectEventStartRepeatedMotion(LOCALID_PARTY_LEADER, 2);
  Engine_ObjectEventStartRepeatedMotion(GARET, 2);
  Engine_ObjectEventStartRepeatedMotion(MIA, 2);
  Engine_ObjectEventRunRepeatedMotion(IVAN, 2);
  KolimaForestTree_PlayGesture(LOCALID_TRET_BLANK, FACE_GESTURE_OPEN_EYES);
  Script_OpenMessage(SPEAKER_WINDOW_BELOW | LOCALID_TRET, 0);
  ObjectEvent_FaceDirection(LOCALID_PARTY_LEADER, FACING_NORTH, 0);
  ObjectEvent_FaceDirection(GARET, FACING_NORTHEAST, 0);
  ObjectEvent_FaceDirection(MIA, FACING_NORTH, 0);
  ObjectEvent_FaceDirection(IVAN, FACING_NORTHWEST, 0);
  if (Engine_ScriptChooseYesNo(0, 0) == 1) {
    gFieldWork->message += 1;
  }
  Engine_ScriptWait(10);
  KolimaForestTree_PlayGesture(LOCALID_TRET_BLANK, FACE_GESTURE_LOOK_LEFT);
  Engine_ScriptWait(20);
  KolimaForestTree_PlayGesture(LOCALID_LAUREL_BLANK, FACE_GESTURE_LOOK_RIGHT);
  Engine_ScriptWait(40);
  KolimaForestTree_PlayGesture(LOCALID_TRET_BLANK, FACE_GESTURE_OPEN_EYES);
  Engine_ScriptWait(20);
  Script_ShowMessageAndWait(SPEAKER_WINDOW_BELOW | LOCALID_TRET, 0, 10);
  ObjectEvent_FaceDirection(LOCALID_PARTY_LEADER, FACING_SOUTH, 0);
  ObjectEvent_FaceDirection(GARET, FACING_EAST, 0);
  ObjectEvent_FaceDirection(MIA, FACING_NORTH, 0);
  ObjectEvent_FaceDirection(IVAN, FACING_WEST, 10);
  KolimaForestTree_NodTogether();
  Script_PlaySong(SONG_FADE_OUT_BGM);
  // Rejoin the party and exchange the used water for its empty bottle.
  finalActions = gKolimaForestTree_RejoinPartyActions;
  ObjectEvent_SetActionScript(GARET, finalActions);
  if (includeMia != 0) {
    ObjectEvent_SetActionScript(MIA, finalActions);
  }
  ObjectEvent_RunActionScriptAndWait(IVAN, finalActions);
  KolimaForestTree_PlayGesture(LOCALID_TRET_BLANK, FACE_GESTURE_BLINK);
  KolimaForestTree_PlayGesture(LOCALID_TRET_BLANK, FACE_GESTURE_BLINK);
  Engine_ScriptWait(20);
  Engine_ScriptSetMessage(MsgKolimaOweGreatDebtHaveSaved);
  Script_ShowMessage(SPEAKER_WINDOW_BELOW | LOCALID_TRET, 0);
  KolimaForestTree_PlayGesture(LOCALID_LAUREL_BLANK, FACE_GESTURE_BLINK);
  KolimaForestTree_PlayGesture(LOCALID_LAUREL_BLANK, FACE_GESTURE_BLINK);
  Engine_ScriptWait(20);
  Script_ShowMessageAndWait(SPEAKER_WINDOW_ABOVE | LOCALID_LAUREL, 0, 10);
  Engine_ObjectEventSetAnimationAndWait(LOCALID_PARTY_LEADER, ANIMATION_NOD);
  GameFlag_Set(FLAG_KOLIMA_CURSE_LIFTED);
  Script_PlaySong(MUS_001);
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

  tretBlank = ObjectEvent_GetById(LOCALID_TRET_BLANK);
  tretsWrathfulSide = ObjectEvent_GetById(LOCALID_TRETS_WRATHFUL_SIDE);
  laurelBlank = ObjectEvent_GetById(LOCALID_LAUREL_BLANK);
  Engine_TaskWait(1);
  Engine_ObjectEventSetChildValue(LOCALID_TRETS_WRATHFUL_SIDE, 15);
  gFieldWork->transition =
      MAP_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_INTERLACED_WIPE);
  gGameState.retreatMap = (s32)&MapId_KolimaForestEntrance;
  gGameState.retreatWarpDest = KOLIMA_FOREST_ENTRANCE_FROM_WORLD_MAP;
  if (!Engine_GameFlagIsSet(FLAG_KOLIMA_CURSE_LIFTED))
    KolimaPalette_Curse(3);
  ObjectEvent_GetById(LOCALID_TRET)->radius = 6;
  ObjectEvent_GetById(LOCALID_LAUREL)->radius = 6;
  ObjectEvent_GetById(LOCALID_TRET_STAND_IN)->radius = 6;
  ObjectEvent_GetById(LOCALID_LAUREL_STAND_IN)->radius = 6;
  Engine_ObjectEventSetSpriteFlags(
      ObjectEvent_GetById(LOCALID_TRETS_WRATHFUL_SIDE), 0);
  Engine_ObjectEventSetSpriteFlags(ObjectEvent_GetById(LOCALID_TRET_BLANK), 0);
  Engine_ObjectEventSetSpriteFlags(ObjectEvent_GetById(LOCALID_LAUREL_BLANK),
                                   0);
  Engine_ObjectEventSetSpriteFlags(ObjectEvent_GetById(LOCALID_TRET), 0);
  Engine_ObjectEventSetSpriteFlags(ObjectEvent_GetById(LOCALID_LAUREL), 0);
  Engine_ObjectEventSetSpritePriority(LOCALID_TRET, 2);
  Engine_ObjectEventSetSpritePriority(LOCALID_TRETS_WRATHFUL_SIDE, 2);
  Engine_ObjectEventSetSpritePriority(LOCALID_LAUREL, 2);
  tretBlank->motionFlags = 0;
  tretBlank->y.fixed = PIXELS(28);
  laurelBlank->motionFlags = 0;
  laurelBlank->y.fixed = PIXELS(28);
  tretsWrathfulSide->motionFlags = 0;
  tretsWrathfulSide->y.fixed = PIXELS(28);
  Engine_ObjectEventSetAnimation(LOCALID_LAUREL, LAUREL_FACE_CLOSED);
  Engine_ObjectEventSetAnimation(LOCALID_TRET, TRET_FACE_CLOSED);
  ObjectEvent_GetById(LOCALID_TRET)->collisionFlags |= OBJ_EVENT_NOT_TARGETED;
  ObjectEvent_GetById(LOCALID_LAUREL)->collisionFlags |= OBJ_EVENT_NOT_TARGETED;
  ObjectEvent_GetById(LOCALID_TRET_BLANK)->collisionFlags |=
      OBJ_EVENT_NOT_TARGETED;
  ObjectEvent_GetById(LOCALID_LAUREL_BLANK)->collisionFlags |=
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
    Engine_AudioPlayCue(SE_200);
  effect = Engine_ObjectCreate(SPRITE_GOLDEN_RINGS, spawnX, spawnY, spawnZ);
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
  ObjectEvent_SetMode(effect, 2);
  Engine_ObjectSetPosition(effect, targetX, 0, targetZ);
  Engine_ObjectSetScript(effect, gKolimaForestTreeEffectScript);
}

/// Alternates the saved healthy and withered palettes over thirty frames.
static void KolimaForestTree_UpdateHealingPalette(void) {
  s32 step = gKolimaForestTreePaletteFrame;

  if (step == 0) {
    KolimaPalette_Restore(0);
    Engine_ColorBufferInterpolate(20);
  } else if (step == 20) {
    KolimaPalette_Restore(1);
    Engine_ColorBufferInterpolate(8);
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
  Engine_ObjectEventSetAnimation(localId, firstFrame);
  Engine_TaskWait(6);
  Engine_ObjectEventSetAnimation(localId, secondFrame);
  Engine_TaskWait(6);
  Engine_ObjectEventSetAnimation(localId, firstFrame);
  Engine_TaskWait(6);
  Engine_ObjectEventSetAnimation(localId, secondFrame);
  Engine_TaskWait(6);
  Engine_ObjectEventSetAnimation(localId, firstFrame);
}

/// Plays one of Tret's or Laurel's face gestures, chosen by the blank object
/// event at their spot.
static void KolimaForestTree_PlayGesture(s32 blankLocalId, s32 gesture) {
  if (blankLocalId == LOCALID_TRET_BLANK) {
    switch (gesture) {
    case FACE_GESTURE_CLOSE_EYES:
      Engine_ObjectEventSetAnimation(LOCALID_TRET, TRET_FACE_OPEN);
      Engine_TaskWait(6);
      Engine_ObjectEventSetAnimation(LOCALID_TRET, TRET_FACE_CLOSED);
      break;
    case FACE_GESTURE_OPEN_EYES:
      Engine_ObjectEventSetAnimation(LOCALID_TRET, TRET_FACE_OPEN);
      break;
    case FACE_GESTURE_LOOK_LEFT:
      Engine_ObjectEventSetAnimation(LOCALID_TRET, TRET_FACE_OPEN);
      Engine_TaskWait(6);
      Engine_ObjectEventSetAnimation(LOCALID_TRET, TRET_FACE_LOOK_LEFT);
      break;
    case FACE_GESTURE_LOOK_RIGHT:
      Engine_ObjectEventSetAnimation(LOCALID_TRET, TRET_FACE_OPEN);
      Engine_TaskWait(6);
      Engine_ObjectEventSetAnimation(LOCALID_TRET, TRET_FACE_LOOK_RIGHT);
      break;
    case FACE_GESTURE_BLINK:
      KolimaForestTree_AlternateFaceFrames(LOCALID_TRET, TRET_FACE_OPEN,
                                           TRET_FACE_CLOSED);
      break;
    case FACE_GESTURE_HALF_CLOSE_EYES:
      Engine_ObjectEventSetAnimation(LOCALID_TRET, TRET_FACE_OPEN);
      Engine_TaskWait(6);
      Engine_ObjectEventSetAnimation(LOCALID_TRET, TRET_FACE_HALF_CLOSED);
      break;
    case FACE_GESTURE_WRATHFUL_CLOSE_EYES:
      Engine_ObjectEventSetAnimation(LOCALID_TRET, TRET_FACE_WRATHFUL_OPEN);
      Engine_TaskWait(6);
      Engine_ObjectEventSetAnimation(LOCALID_TRET, TRET_FACE_WRATHFUL_CLOSED);
      break;
    case FACE_GESTURE_WRATHFUL_LOOK_RIGHT:
      Engine_ObjectEventSetAnimation(LOCALID_TRET, TRET_FACE_WRATHFUL_OPEN);
      Engine_TaskWait(6);
      Engine_ObjectEventSetAnimation(LOCALID_TRET,
                                     TRET_FACE_WRATHFUL_LOOK_RIGHT);
      break;
    case FACE_GESTURE_WRATHFUL_LOOK_LEFT:
      Engine_ObjectEventSetAnimation(LOCALID_TRET, TRET_FACE_WRATHFUL_OPEN);
      Engine_TaskWait(6);
      Engine_ObjectEventSetAnimation(LOCALID_TRET,
                                     TRET_FACE_WRATHFUL_LOOK_LEFT);
      break;
    case FACE_GESTURE_WRATHFUL_BLINK:
      KolimaForestTree_AlternateFaceFrames(
          LOCALID_TRET, TRET_FACE_WRATHFUL_OPEN, TRET_FACE_WRATHFUL_CLOSED);
      break;
    case FACE_GESTURE_WRATHFUL_OPEN_EYES:
    case FACE_GESTURE_WRATHFUL_STARE:
      Engine_ObjectEventSetAnimation(LOCALID_TRET, TRET_FACE_WRATHFUL_OPEN);
      break;
    }
  } else {
    switch (gesture) {
    case FACE_GESTURE_CLOSE_EYES:
      Engine_ObjectEventSetAnimation(LOCALID_LAUREL, LAUREL_FACE_OPEN);
      Engine_TaskWait(6);
      Engine_ObjectEventSetAnimation(LOCALID_LAUREL, LAUREL_FACE_CLOSED);
      break;
    case FACE_GESTURE_OPEN_EYES:
      Engine_ObjectEventSetAnimation(LOCALID_LAUREL, LAUREL_FACE_OPEN);
      break;
    case FACE_GESTURE_LOOK_LEFT:
      Engine_ObjectEventSetAnimation(LOCALID_LAUREL, LAUREL_FACE_OPEN);
      Engine_TaskWait(6);
      Engine_ObjectEventSetAnimation(LOCALID_LAUREL, LAUREL_FACE_LOOK_LEFT);
      break;
    case FACE_GESTURE_LOOK_RIGHT:
      Engine_ObjectEventSetAnimation(LOCALID_LAUREL, LAUREL_FACE_OPEN);
      Engine_TaskWait(6);
      Engine_ObjectEventSetAnimation(LOCALID_LAUREL, LAUREL_FACE_LOOK_RIGHT);
      break;
    case FACE_GESTURE_BLINK:
      KolimaForestTree_AlternateFaceFrames(LOCALID_LAUREL, LAUREL_FACE_OPEN,
                                           LAUREL_FACE_CLOSED);
      break;
    case FACE_GESTURE_HALF_CLOSE_EYES:
      Engine_ObjectEventSetAnimation(LOCALID_LAUREL, LAUREL_FACE_OPEN);
      Engine_TaskWait(6);
      Engine_ObjectEventSetAnimation(LOCALID_LAUREL, LAUREL_FACE_HALF_CLOSED);
      break;
    }
  }
  Engine_TaskWait(12);
}

/// Expands a spark along its spiral for 120 frames, then releases its cache
/// slot.
static void KolimaForestTree_UpdateOrbitSpark(union FieldObject *fieldObject) {
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
    Engine_ObjectDispatchRelease(spark);
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
    Engine_AudioPlayCue(SE_220);
    for (sparkIndex = 0; sparkIndex <= 5; sparkIndex++) {
      spark = Engine_ObjectCreate(
          SPRITE_SPARKLE, gKolimaForestTreeSparkOrigin[0],
          gKolimaForestTreeSparkOrigin[1], gKolimaForestTreeSparkOrigin[2]);
      if (spark != 0) {
        previousSprite =
            Sprite_ShareVramCacheEntry(spark->sprite, previousSprite);
        spark->motionFlags = 0;
        spark->sprite->image.priority = 1;
        Engine_ObjectEventSetSpriteFlags(spark, 0);
        ObjectEvent_SetMode(spark, 1);
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
  s32 owner = PartyInventory_FindMember(oldItemId);

  if (owner != -1) {
    s32 inventorySlot = Inventory_Find(owner, oldItemId);

    if (inventorySlot != -1) {
      Unit_Get(owner)->inventory[inventorySlot] = newItemId;
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
    {KOLIMA_FOREST_TREE_FROM_TRET_TREE_INTERIORS, CONDITION_ALWAYS, 328, 16,
     200, FACING_SOUTH, 0, 0, 0, 512, 448, 0},
    {MAP_TABLE_END},
};

// clang-format off
struct MapWarpRegion gKolimaForestTreeRegions[] = {
    {320, 88, 176, 336, 104, 192, -1, KOLIMA_FOREST_TREE_TO_TRET_TREE_INTERIORS},
    {-1, 0, 0, 0, 0, 0, 0, 0},
};

u32 gKolimaForestTreeWarps[] = {
    MAP_WARPS(KolimaForestTree),
    MAP_WARP(KOLIMA_FOREST_TREE_TO_KOLIMA_FOREST_FLOODGATE, KolimaForestFloodgate, KOLIMA_FOREST_FLOODGATE_FROM_KOLIMA_FOREST_TREE),
    MAP_WARP(KOLIMA_FOREST_TREE_TO_TRET_TREE_INTERIORS, TretTreeInteriors, TRET_TREE_INTERIORS_FROM_KOLIMA_FOREST_TREE),
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
    { GARET, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        0, 0, 0, FACING_EAST, TALK_FACING_KEEP, 0 },
    { IVAN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        0, 0, 0, FACING_EAST, TALK_FACING_KEEP, 0 },
    { MIA, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
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
