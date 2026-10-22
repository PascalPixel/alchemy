/// @file
/// Kolima Forest (SceneId_KolimaForestTree), the last of the forest's five
/// maps, where Tret and Laurel, the spirits of the holy trees, stand. The party
/// has come from Kolima to reawaken Tret's gentle side and save the villagers
/// he turned into trees, and here the curse is lifted.
///
/// At first Tret is silent and his wrathful side answers for him
/// (ACTOR_TRETS_WRATHFUL_SIDE, with a portrait): "I control all in Tret's
/// realm!" Laurel, on the first talk (FLAG_MET_LAUREL), knows the party as the
/// ones who resisted Tret's glamour, but says they are too late: Tret will die
/// and the forest with him. Mind Read hears Laurel think that the healing
/// waters of Mercury Lighthouse might save Tret, though its fountain has dried
/// up, and Tret ask Kolima's forgiveness. Using Hermes' Water on Tret now, the
/// wrathful side throws the leader back. The way on is into Tret himself: the
/// region at his foot is KOLIMA_FOREST_TREE_TO_TRET_TREE_INTERIORS, into the
/// Tret Tree (SceneId_TretTreeInteriors), where the party defeats the evil
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
/// forest's way in (SceneId_KolimaForestEntrance,
/// KOLIMA_FOREST_ENTRANCE_FROM_WORLD_MAP), and the way back,
/// KOLIMA_FOREST_TREE_TO_KOLIMA_FOREST_FLOODGATE, leads to the Floodgate
/// Switch's map (SceneId_KolimaForestFloodgate).
///
/// Open questions:
/// - XXX (no evidence): why Tret and Laurel each have a blank actor that only
///   selects whose face KolimaForestTree_PlayGesture moves, likely left from a
///   version where those actors were drawn; the ROM keeps no such version, and
///   nothing reads the actors but that function.

#include "battle_effect_runtime.h"
#include "game_state.h"
#include "unit/scene/kolima_forest_tree.h"
#include "unit/scene/common/kolima_palette.h"
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
#include "field_obj.h"
#include "field_effect.h"
#include "battle_unit.h"
#include "types.h"
#include "call.h"
#include "field_event.h"
#include "engine_call.h"
#include "scene_ids.h"
#include "palette_scene.h"
#include "io_reg.h"
#include "field_scene.h"
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

/// The actors, numbering gKolimaForestTreePlacements' rows from
/// ACTOR_FIRST_PLACED.
enum {
  /// Tret's face, drawn in SPRITE_TRET_RAISED; its animations are his face's
  /// gestures (KolimaForestTree_PlayGesture).
  /// ★ Named from the game: "The Water of Hermes seeped into Tret."
  /// (MsgKolimaWaterHermesSeepedIntoTret)
  ACTOR_TRET = ACTOR_FIRST_PLACED,
  /// ★ Named from the game: "Oh! Laurel, you were given powers, too?"
  /// (MsgKolimaWasIndeedAngryPeopleHad)
  ACTOR_LAUREL,
  /// Blank actors 28 pixels up at Tret's and Laurel's spots, never targeted;
  /// nothing addresses them but KolimaForestTree_PlayGesture, which takes one
  /// to say whose face to move.
  /// ★ Named from the game: "Tret" (MsgEnemyName)
  ACTOR_TRET_BLANK,
  /// ★ Named from the game: "Laurel" (MsgDebugGlyphName)
  ACTOR_LAUREL_BLANK,
  /// An invisible actor on Tret's spot that answers for him:
  /// talking to it, Mind Read on it and Hermes' Water used on it reach Tret.
  /// ★ Named from the game: "The Water of Hermes seeped into Tret."
  /// (MsgKolimaWaterHermesSeepedIntoTret)
  ACTOR_TRET_STAND_IN,
  /// The invisible actor on Laurel's spot that answers for her when she is
  /// talked to or her mind is read.
  /// ★ Named from the game: "I am sorry to have worried you so, Laurel."
  /// (MsgKolimaFeelGreatPowerSpreadingThrough)
  ACTOR_LAUREL_STAND_IN,
  /// Tret's wrathful side, which speaks with a portrait, drawn in SPRITE_TRET
  /// with child value 15 over his face.
  /// ★ Named from the game: "His wrathful side turned the people of Kolima
  /// into trees." (MsgKolimaWatchOutItsHappeningAgain)
  ACTOR_TRETS_WRATHFUL_SIDE
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

static void KolimaForestTree_PlayGesture(s32 blankActorId, s32 gesture);
static void KolimaForestTree_SpawnOrbitSparks(void);
static void KolimaForestTree_ReplaceInventoryItem(s32 oldItemId, s32 newItemId);

s32 gKolimaForestTreeSparkOrigin[3] __attribute__((section(".bss")));
s32 gKolimaForestTreeSparkFrame __attribute__((section(".bss")));
s32 gKolimaForestTreePlaySparkSound __attribute__((section(".bss")));
s32 gKolimaForestTreePaletteFrame __attribute__((section(".bss")));

const struct SceneEntrance *KolimaForestTree_GetEntrances(void) {
  return gKolimaForestTreeEntrances;
}

/// The region at Tret's foot that leads into the Tret Tree.
const struct SceneRegion *KolimaForestTree_GetRegions(void) {
  return gKolimaForestTreeRegions;
}

/// The ways back (SceneId_KolimaForestFloodgate) and into the Tret Tree
/// (SceneId_TretTreeInteriors).
const u32 *KolimaForestTree_GetExits(void) { return gKolimaForestTreeExits; }

/// Tret and Laurel, the blank actors at their spots, the ones the party talks
/// to, Tret's wrathful side, and Garet, Ivan and Mia for the healing.
const struct ScenePlacement *KolimaForestTree_GetPlacements(void) {
  return gKolimaForestTreePlacements;
}

/// Unused: takes the touched exit after the leader's current movement; no
/// event row names it.
UNUSED static void KolimaForestTree_ExitTouchedRegion(void) {
  struct EventWork *scene;

  scene = gEventWork;
  Engine_EventBegin();
  Engine_ActorWalkByAndWait(ACTOR_PARTY_LEADER, 0, 0);
  Engine_EventRequestExit(scene->touchedTrigger);
  Engine_EventEnd();
}

/// Talking to Tret and Laurel, reading their minds, and Hermes' Water on Tret.
const struct SceneEvent *KolimaForestTree_GetEvents(void) {
  return gKolimaForestTreeEvents;
}

/// Kolima Forest (SceneId_KolimaForestTree), talking to Laurel
/// (ACTOR_LAUREL_STAND_IN): the first time, Laurel asks who the party is,
/// recognises the ones who resisted Tret's glamour and senses a great warmth in
/// them, but says they are too late to rescue Tret, and tells them to leave
/// before the forest is destroyed (sets FLAG_MET_LAUREL); then only that
/// warning. Once the curse is lifted, the forest is alive again thanks to the
/// party, and Laurel says, "We shall protect the forest from now on."
static void KolimaForestTree_TalkToLaurel(void) {
  Engine_EventBegin();
  KolimaForestTree_PlayGesture(ACTOR_LAUREL_BLANK, FACE_GESTURE_OPEN_EYES);
  if (Engine_GameFlagIsSet(FLAG_KOLIMA_CURSE_LIFTED) != 0) {
    Engine_EventSetMessage(MsgKolimaForestKolimaAlive);
    Engine_EventShowMessage(ACTOR_LAUREL, 0);
  } else if (GameFlag_IsSet(FLAG_MET_LAUREL) != 0) {
    Engine_EventSetMessage(MsgKolimaLeaveBeforeForest);
    Engine_EventShowMessage(ACTOR_LAUREL, 0);
  } else {
    Engine_EventSetMessage(MsgKolimaWho);
    Engine_EventShowMessageAndWait(ACTOR_LAUREL, 0, 20);
    KolimaForestTree_PlayGesture(ACTOR_LAUREL_BLANK, FACE_GESTURE_CLOSE_EYES);
    Engine_EventWait(60);
    KolimaForestTree_PlayGesture(ACTOR_LAUREL_BLANK, FACE_GESTURE_OPEN_EYES);
    Engine_EventShowMessageAndWait(ACTOR_LAUREL, 0, 10);
    Engine_ActorSetAnimationAndWait(ACTOR_PARTY_LEADER, ANIMATION_NOD);
    Engine_EventWait(40);
    Engine_EventShowMessage(ACTOR_LAUREL, 0);
    KolimaForestTree_PlayGesture(ACTOR_LAUREL_BLANK, FACE_GESTURE_CLOSE_EYES);
    Engine_EventWait(80);
    Engine_EventShowMessageAndWait(ACTOR_LAUREL, 0, 20);
    KolimaForestTree_PlayGesture(ACTOR_LAUREL_BLANK, FACE_GESTURE_OPEN_EYES);
    Engine_EventShowMessageAndWait(ACTOR_LAUREL, 0, 20);
    Engine_GameFlagSet(FLAG_MET_LAUREL);
  }
  KolimaForestTree_PlayGesture(ACTOR_LAUREL_BLANK, FACE_GESTURE_CLOSE_EYES);
  Engine_EventEnd();
}

/// Kolima Forest (SceneId_KolimaForestTree), talking to Tret
/// (ACTOR_TRET_STAND_IN); until FLAG_TRET_DEFEATED Mind Read on him runs this
/// too, with no effect of its own (trigger bit 10). Before the Tret Tree, he is
/// silent and his wrathful side answers, the screen darkening around its words;
/// after it he is silent, and if anyone carries Hermes' Water the main menu
/// opens. Once the curse is lifted, no axe shall touch him again.
static void KolimaForestTree_TalkToTret(void) {
  s32 waterOwner;

  Engine_EventBegin();
  if (GameFlag_IsSet(FLAG_KOLIMA_CURSE_LIFTED) != 0) {
    KolimaForestTree_PlayGesture(ACTOR_TRET_BLANK, FACE_GESTURE_OPEN_EYES);
    Engine_EventSetMessage(MsgKolimaNowHaveSuchPowerAxe);
    Event_ShowMessage(ACTOR_TRET, 0);
    KolimaForestTree_PlayGesture(ACTOR_TRET_BLANK, FACE_GESTURE_CLOSE_EYES);
  } else {
    if (GameFlag_IsSet(FLAG_TRET_DEFEATED) != 0) {
      KolimaForestTree_PlayGesture(ACTOR_TRET_BLANK, FACE_GESTURE_OPEN_EYES);
      Engine_EventSetMessage(MsgKolimaSilence2);
      Event_ShowMessage(ACTOR_TRET, 0);
      KolimaForestTree_PlayGesture(ACTOR_TRET_BLANK, FACE_GESTURE_CLOSE_EYES);
      waterOwner = PartyInventory_FindOwner(ITEM_HERMES_WATER);
      if (waterOwner != -1) {
        u16 *menuRequest = &gEventWork->requestedMenu;
        u16 open = 1;

        *menuRequest = open;
      }
    } else {
      Engine_EventSetMessage(MsgKolimaSilence);
      Event_ShowMessage(ACTOR_TRET, 0);
      ColorBuffer_ApplyTarget(0x406218, 1);
      Engine_ColorBufferInterpolate(20);
      Engine_TaskWait(40);
      Event_ShowMessageAndWait(
          SPEAKER_PORTRAIT_RIGHT | ACTOR_TRETS_WRATHFUL_SIDE, 0, 10);
      Engine_ActorRunRepeatedMotion(ACTOR_PARTY_LEADER, 2);
      Event_ShowMessage(SPEAKER_PORTRAIT_RIGHT | ACTOR_TRETS_WRATHFUL_SIDE, 0);
      ColorBuffer_ApplyTarget(PALETTE_TRANSFORM_PLAIN, 1);
      Engine_ColorBufferInterpolate(20);
      Engine_TaskWait(40);
    }
  }
  Engine_EventEnd();
}

/// Kolima Forest (SceneId_KolimaForestTree), Mind Read on Laurel: before the
/// curse is lifted, the healing waters of Mercury Lighthouse might save Tret,
/// but its fountain has dried up; afterwards, it must be horrible beyond the
/// river.
static void KolimaForestTree_ReadLaurelThoughts(void) {
  Engine_EventBegin();
  if (Engine_GameFlagIsSet(FLAG_KOLIMA_CURSE_LIFTED) != 0) {
    Engine_EventSetMessage(MsgKolimaMustHorribleBeyondRiverAm);
  } else {
    Engine_EventSetMessage(MsgKolimaHealingWatersMercuryLighthouseMight);
  }
  Engine_EventShowMessage(ACTOR_LAUREL, 0);
  Engine_EventEnd();
}

/// Kolima Forest (SceneId_KolimaForestTree), Mind Read on Tret: before the
/// curse is lifted, "People of Kolima... Forgive me..."; afterwards, a plea to
/// be careful beyond the river.
static void KolimaForestTree_ReadTretThoughts(void) {
  Engine_EventBegin();
  if (Engine_GameFlagIsSet(FLAG_KOLIMA_CURSE_LIFTED) != 0) {
    Engine_EventSetMessage(MsgKolimaKnowCannotStopButPlease);
  } else {
    Engine_EventSetMessage(MsgKolimaPeopleKolimaForgiveMe);
  }
  Engine_EventShowMessage(ACTOR_TRET, 0);
  Engine_EventEnd();
}

/// Kolima Forest (SceneId_KolimaForestTree), using Hermes' Water on Tret while
/// the curse lasts: before the evil within him is defeated in the Tret Tree
/// (FLAG_TRET_DEFEATED), his wrathful side repels it
/// (KolimaForestTree_RejectHermesWater); afterwards it heals him
/// (KolimaForestTree_RestoreTret).
static void KolimaForestTree_UseHermesWater(void) {
  Engine_EventBegin();
  Field_EndBusyAndReveal();
  if (Engine_GameFlagIsSet(FLAG_TRET_DEFEATED) == 0) {
    KolimaForestTree_RejectHermesWater();
  } else {
    KolimaForestTree_RestoreTret();
  }
  Engine_EventEnd();
}

/// Tret's wrathful side repels Hermes' Water: sparks gather, the leader is
/// thrown back and lands, and the voice says "I control Tret's heart! I shall
/// not let you do anything to him!"
static void KolimaForestTree_RejectHermesWater(void) {
  struct FieldActor *leader;

  // The wrathful side throws the leader back before the water can heal Tret.
  leader = Actor_Get(ACTOR_PARTY_LEADER);
  Engine_ActorFaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 0);
  ColorBuffer_ApplyTarget(0x406218, 1);
  Engine_ColorBufferInterpolate(20);
  Engine_TaskWait(40);
  Event_PlayCue(AUDIO_CUE_FADE_BGM);
  gKolimaForestTreePlaySparkSound = 1;
  FieldScheduler_AddCallback(KolimaForestTree_SpawnRejectionSpark,
                             TASK_PRIORITY_SCENE);
  Engine_TaskWait(30);
  gKolimaForestTreePlaySparkSound = 0;
  Camera_MoveTo(PIXELS(328), -1, PIXELS(235), 1);
  Engine_ActorSetSpritePriority(ACTOR_PARTY_LEADER, 1);
  Object_GetById(ACTOR_PARTY_LEADER)->steeringFlags &= ~ACTOR_FACES_MOTION;
  Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, ANIMATION_LEADER_STRIDE);
  Actor_SetSpeed(ACTOR_PARTY_LEADER, PIXELS(2), PIXELS(2));
  Event_PlayCue(SE_IMPACT);
  leader->velocityY = PIXELS(5);
  leader->gravity = PIXELS(1) / 4;
  leader->bounce = 5 * Q16_ONE / 8;
  Actor_MoveToAndWait(ACTOR_PARTY_LEADER, 335, 258);
  while (leader->velocityY >= 0) {
    Engine_TaskWait(1);
  }
  do {
    Engine_TaskWait(1);
  } while (leader->velocityY <= 0);
  Event_PlayCue(SE_161);
  Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, ANIMATION_COLLAPSE);
  Engine_EventWait(120);
  Engine_TaskRemoveCallback(KolimaForestTree_SpawnRejectionSpark);
  Engine_TaskWait(40);
  leader->bounce = Q16_ONE / 4;
  {
    struct FieldActor *leader = Actor_Get(ACTOR_PARTY_LEADER);
    leader->steeringFlags = ACTOR_FACES_MOTION | leader->steeringFlags;
  }
  Engine_EventWait(80);
  // Hear the wrathful side's refusal, then restore the leader's motion.
  Engine_EventSetMessage(MsgKolimaControlTretsHeartShallNot);
  Event_ShowMessageAndWait(SPEAKER_PORTRAIT_RIGHT | ACTOR_TRETS_WRATHFUL_SIDE,
                           0, 20);
  Engine_ActorRunRepeatedMotion(ACTOR_PARTY_LEADER, 2);
  Engine_EventWait(20);
  Event_ShowMessage(SPEAKER_PORTRAIT_RIGHT | ACTOR_TRETS_WRATHFUL_SIDE, 0);
  Audio_PlayCueFromEventWork();
  ColorBuffer_ApplyTarget(PALETTE_TRANSFORM_PLAIN, 1);
  Engine_ColorBufferInterpolate(20);
  Engine_TaskWait(40);
  {
    s32 facing = FACING_NORTH;

    leader->facing = facing;
  }
  leader->gravity = PIXELS(1);
  leader->bounce = Q16_ONE / 4;
  Engine_ActorRunRepeatedMotion(ACTOR_PARTY_LEADER, 2);
  Engine_EventWait(40);
  Engine_ActorJump(ACTOR_PARTY_LEADER, 4, 0);
  Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, ANIMATION_STAND);
  Engine_EventWait(20);
}

/// Gives all four companions the same nod, waiting on Ivan's animation.
static __inline__ void KolimaForestTree_NodTogether(void) {
  Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, ANIMATION_NOD);
  Engine_ActorSetAnimation(GARET, ANIMATION_NOD);
  Engine_ActorSetAnimation(MIA, ANIMATION_NOD);
  Engine_ActorSetAnimationAndWait(IVAN, ANIMATION_NOD);
}

/// Turns the companions together and pauses after the final facing change.
static __inline__ void KolimaForestTree_FaceParty(s32 facing, s32 waitFrames) {
  Actor_FaceDirection(ACTOR_PARTY_LEADER, facing, 0);
  Actor_FaceDirection(GARET, facing, 0);
  Actor_FaceDirection(MIA, facing, 0);
  Actor_FaceDirection(IVAN, facing, waitFrames);
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
  struct FieldActor *leader;
  s32 *paletteFrame;
  s32 flash;
  void (*spawnOrbitSparks)(void);
  const s32 *finalActions;

  // Gather the companions while Hermes' Water seeps into Tret.
  includeMia = GameFlag_IsSet(FLAG_IN_PARTY(MIA));
  Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 328, 212);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 20);
  Event_PlayCue(AUDIO_CUE_FADE_BGM);
  Engine_MessageShowCentered(MsgKolimaWaterHermesSeepedIntoTret, 1);
  Actor_SetSpeed(GARET, PIXELS(1), PIXELS(1) / 2);
  Actor_SetSpeed(IVAN, PIXELS(1), PIXELS(1) / 2);
  leader = Actor_Get(ACTOR_PARTY_LEADER);
  if (leader != 0) {
    Actor_SetPosition(GARET, leader->x.fixed, leader->z.fixed);
  }
  leader = Actor_Get(ACTOR_PARTY_LEADER);
  if (leader != 0) {
    Actor_SetPosition(IVAN, leader->x.fixed, leader->z.fixed);
  }
  Actor_SetActionScript(GARET, gKolimaForestTree_GaretApproachActions);
  Actor_SetActionScript(IVAN, gKolimaForestTree_IvanApproachActions);
  if (includeMia != 0) {
    Actor_SetSpeed(MIA, PIXELS(1), PIXELS(1) / 2);
    leader = Actor_Get(ACTOR_PARTY_LEADER);
    if (leader != 0) {
      Actor_SetPosition(MIA, leader->x.fixed, leader->z.fixed);
    }
    Actor_SetActionScript(MIA, gKolimaForestTree_MiaApproachActions);
  }
  Object_RefreshSelectorById(IVAN);
  Engine_EventWait(40);
  KolimaPalette_Restore(0);
  Engine_ColorBufferInterpolate(32);
  Engine_TaskWait(40);
  // Carry the healing across the forest as its healthy colours return.
  paletteFrame = &gKolimaForestTreePaletteFrame;
  *paletteFrame = 0;
  FieldScheduler_AddCallback(KolimaForestTree_UpdateHealingPalette,
                             TASK_PRIORITY_SCENE);
  Engine_EventWait(40);
  Actor_FaceDirection(GARET, FACING_SOUTHWEST, 20);
  Camera_SetSpeed(PIXELS(16) / 5, PIXELS(2) / 5);
  Camera_MoveTo(PIXELS(256), -1, PIXELS(254), 1);
  Engine_CameraWaitForMove();
  Event_PlayCue(SE_246);
  Engine_EventWait(40);
  Actor_FaceDirection(IVAN, FACING_SOUTHEAST, 20);
  Camera_MoveTo(PIXELS(413), -1, PIXELS(261), 1);
  Engine_CameraWaitForMove();
  Event_PlayCue(SE_246);
  Engine_EventWait(40);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_SOUTH, 0);
  Actor_FaceDirection(MIA, FACING_SOUTH, 20);
  Camera_MoveTo(PIXELS(326), -1, PIXELS(384), 1);
  Engine_CameraWaitForMove();
  Event_PlayCue(SE_246);
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
  Camera_MoveTo(PIXELS(328), PIXELS(8), PIXELS(212), 1);
  Engine_CameraWaitForMove();
  Engine_EventWait(40);
  KolimaForestTree_PlayGesture(ACTOR_TRET_BLANK, FACE_GESTURE_OPEN_EYES);
  Engine_EventWait(40);
  Event_PlayCue(MUS_007);
  // Tret and Laurel discover that their powers have returned.
  Engine_EventSetMessage(MsgKolimaFeelGreatPowerSpreadingThrough);
  Event_ShowMessage(ACTOR_TRET, 0);
  Engine_ActorStartRepeatedMotion(ACTOR_PARTY_LEADER, 2);
  Engine_ActorStartRepeatedMotion(GARET, 2);
  Engine_ActorStartRepeatedMotion(MIA, 2);
  Engine_ActorRunRepeatedMotion(IVAN, 2);
  KolimaForestTree_FaceParty(FACING_NORTH, 20);
  KolimaForestTree_PlayGesture(ACTOR_TRET_BLANK, FACE_GESTURE_LOOK_LEFT);
  Engine_EventWait(20);
  KolimaForestTree_PlayGesture(ACTOR_TRET_BLANK, FACE_GESTURE_LOOK_RIGHT);
  Engine_EventWait(40);
  KolimaForestTree_PlayGesture(ACTOR_TRET_BLANK, FACE_GESTURE_OPEN_EYES);
  Engine_EventWait(20);
  Event_ShowMessage(ACTOR_TRET, 0);
  Actor_ShowEmote(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 0);
  Actor_ShowEmote(GARET, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 0);
  Actor_ShowEmote(MIA, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 0);
  Actor_ShowEmote(IVAN, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 40);
  Camera_MoveTo(PIXELS(234), 0, PIXELS(232), 1);
  Engine_CameraWaitForMove();
  Engine_EventWait(40);
  KolimaForestTree_PlayGesture(ACTOR_LAUREL_BLANK, FACE_GESTURE_OPEN_EYES);
  Engine_EventWait(40);
  KolimaForestTree_PlayGesture(ACTOR_LAUREL_BLANK, FACE_GESTURE_LOOK_RIGHT);
  Engine_EventWait(20);
  Event_ShowMessageAndWait(SPEAKER_WINDOW_ABOVE | ACTOR_LAUREL, 0, 20);
  KolimaForestTree_PlayGesture(ACTOR_LAUREL_BLANK, FACE_GESTURE_LOOK_LEFT);
  Engine_EventWait(10);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_SOUTHWEST, 0);
  Actor_FaceDirection(GARET, FACING_SOUTHWEST, 0);
  Actor_FaceDirection(IVAN, FACING_SOUTHWEST, 0);
  Actor_FaceDirection(MIA, FACING_SOUTHWEST, 20);
  KolimaForestTree_PlayGesture(ACTOR_LAUREL_BLANK, FACE_GESTURE_LOOK_RIGHT);
  Engine_EventWait(20);
  KolimaForestTree_PlayGesture(ACTOR_LAUREL_BLANK, FACE_GESTURE_LOOK_LEFT);
  Engine_EventWait(20);
  KolimaForestTree_PlayGesture(ACTOR_LAUREL_BLANK, FACE_GESTURE_LOOK_RIGHT);
  Event_ShowMessageAndWait(SPEAKER_WINDOW_ABOVE | ACTOR_LAUREL, 0, 10);
  KolimaForestTree_PlayGesture(ACTOR_TRET_BLANK, FACE_GESTURE_CLOSE_EYES);
  Engine_EventWait(20);
  Event_ShowMessage(SPEAKER_WINDOW_BELOW | ACTOR_TRET, 0);
  KolimaForestTree_PlayGesture(ACTOR_TRET_BLANK, FACE_GESTURE_OPEN_EYES);
  Engine_EventWait(20);
  Event_OpenMessage(SPEAKER_WINDOW_BELOW | ACTOR_TRET, 0);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 0);
  Actor_FaceDirection(GARET, FACING_NORTHEAST, 0);
  Actor_FaceDirection(IVAN, FACING_NORTHWEST, 0);
  Actor_FaceDirection(MIA, FACING_NORTH, 0);
  if (Engine_EventChooseYesNo(0, 0) == 0) {
    Event_ShowMessage(SPEAKER_WINDOW_ABOVE | ACTOR_LAUREL, 0);
    Event_ShowMessage(SPEAKER_WINDOW_BELOW | ACTOR_TRET, 0);
  } else {
    gEventWork->message += 2;
    Actor_ShowEmote(MIA, EMOTE_IN_FRONT | EMOTE_ANGER_VEIN, 0);
    Actor_ShowEmote(GARET, EMOTE_IN_FRONT | EMOTE_ANGER_VEIN, 0);
    Actor_ShowEmote(IVAN, EMOTE_IN_FRONT | EMOTE_ANGER_VEIN, 40);
    Engine_ActorSetAnimation(GARET, ANIMATION_SHAKE_HEAD);
    Event_ShowMessage(GARET, 0);
    if (includeMia != 0) {
      Engine_ActorRunRepeatedMotion(MIA, 2);
      Event_ShowMessage(MIA, 0);
    } else {
      gEventWork->message += 1;
    }
    Engine_ActorSetAnimationAndWait(IVAN, ANIMATION_NOD);
    Event_ShowMessage(IVAN, 0);
    Event_ShowMessage(SPEAKER_WINDOW_ABOVE | ACTOR_LAUREL, 0);
    Event_ShowMessage(SPEAKER_WINDOW_BELOW | ACTOR_TRET, 0);
  }
  KolimaForestTree_NodTogether();
  Camera_MoveTo(PIXELS(328), PIXELS(8), PIXELS(212), 1);
  Engine_CameraWaitForMove();
  Engine_EventWait(20);
  KolimaForestTree_PlayGesture(ACTOR_TRET_BLANK, FACE_GESTURE_CLOSE_EYES);
  Engine_EventWait(20);
  KolimaPalette_Restore(0);
  Engine_ColorBufferInterpolate(1);
  Engine_TaskWait(1);
  ColorBuffer_ApplyTarget(0x406218, 1);
  Engine_ColorBufferInterpolate(40);
  Engine_EventWait(60);
  gKolimaForestTreeSparkFrame = 0;
  // Tret sends out the power that frees Kolima's villagers.
  gKolimaForestTreeSparkOrigin[0] = PIXELS(328);
  gKolimaForestTreeSparkOrigin[1] = PIXELS(48);
  spawnOrbitSparks = KolimaForestTree_SpawnOrbitSparks;
  gKolimaForestTreeSparkOrigin[2] = PIXELS(205);
  FieldScheduler_AddCallback(spawnOrbitSparks, TASK_PRIORITY_SCENE);
  Engine_EventWait(100);
  Engine_TaskRemoveCallback(spawnOrbitSparks);
  ColorBuffer_ApplyTarget(0x7fff, 0);
  Engine_ColorBufferInterpolate(60);
  Engine_EventWait(100);
  KolimaPalette_Restore(0);
  Engine_ColorBufferInterpolate(20);
  Engine_EventWait(40);
  KolimaForestTree_PlayGesture(ACTOR_TRET_BLANK, FACE_GESTURE_OPEN_EYES);
  Engine_EventWait(10);
  Engine_EventSetMessage(MsgKolimaShouldDoPeopleKolimaCursed);
  Event_ShowMessage(SPEAKER_WINDOW_BELOW | ACTOR_TRET, 0);
  KolimaForestTree_NodTogether();
  Camera_MoveTo(PIXELS(234), 0, PIXELS(232), 1);
  Engine_CameraWaitForMove();
  Engine_EventWait(20);
  Event_ShowMessage(SPEAKER_WINDOW_ABOVE | ACTOR_LAUREL, 0);
  Event_ShowMessageAndWait(SPEAKER_WINDOW_BELOW | ACTOR_TRET, 0, 10);
  Engine_ActorRunRepeatedMotion(GARET, 2);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_SOUTHWEST, 0);
  Actor_FaceDirection(GARET, FACING_NORTHEAST, 10);
  Event_OpenMessage(GARET, 0);
  if (Engine_EventChooseYesNo(0, 0) == 0) {
    Actor_ShowEmote(GARET, EMOTE_IN_FRONT | EMOTE_WINCE, 40);
  } else {
    Engine_ActorSetAnimationAndWait(GARET, ANIMATION_SHAKE_HEAD);
    gEventWork->message += 1;
  }
  Event_ShowMessage(GARET, 0);
  KolimaForestTree_PlayGesture(ACTOR_TRET_BLANK, FACE_GESTURE_BLINK);
  Engine_EventWait(20);
  // Explain the curse and how the falling Psynergy Stones changed the forest.
  Engine_EventSetMessage(MsgKolimaWasIndeedAngryPeopleHad);
  Event_ShowMessage(SPEAKER_WINDOW_BELOW | ACTOR_TRET, 0);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 0);
  Actor_FaceDirection(GARET, FACING_NORTH, 0);
  Event_ShowMessage(SPEAKER_WINDOW_BELOW | ACTOR_TRET, 0);
  KolimaForestTree_NodTogether();
  KolimaForestTree_PlayGesture(ACTOR_TRET_BLANK, FACE_GESTURE_BLINK);
  Engine_EventWait(20);
  Event_ShowMessageAndWait(SPEAKER_WINDOW_BELOW | ACTOR_TRET, 0, 20);
  KolimaForestTree_PlayGesture(ACTOR_LAUREL_BLANK, FACE_GESTURE_CLOSE_EYES);
  Event_ShowMessageAndWait(SPEAKER_WINDOW_ABOVE | ACTOR_LAUREL, 0, 20);
  KolimaForestTree_PlayGesture(ACTOR_LAUREL_BLANK, FACE_GESTURE_LOOK_RIGHT);
  Engine_EventWait(40);
  KolimaForestTree_PlayGesture(ACTOR_LAUREL_BLANK, FACE_GESTURE_OPEN_EYES);
  Engine_EventWait(20);
  Event_ShowMessageAndWait(SPEAKER_WINDOW_ABOVE | ACTOR_LAUREL, 0, 20);
  KolimaForestTree_PlayGesture(ACTOR_TRET_BLANK, FACE_GESTURE_LOOK_LEFT);
  Engine_EventWait(20);
  Event_ShowMessage(SPEAKER_WINDOW_BELOW | ACTOR_TRET, 0);
  Actor_ShowEmote(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | EMOTE_WINCE, 0);
  Actor_ShowEmote(GARET, EMOTE_IN_FRONT | EMOTE_WINCE, 0);
  Actor_ShowEmote(MIA, EMOTE_IN_FRONT | EMOTE_WINCE, 0);
  Actor_ShowEmote(IVAN, EMOTE_IN_FRONT | EMOTE_WINCE, 80);
  KolimaForestTree_PlayGesture(ACTOR_LAUREL_BLANK,
                               FACE_GESTURE_HALF_CLOSE_EYES);
  Engine_EventWait(60);
  KolimaForestTree_PlayGesture(ACTOR_LAUREL_BLANK, FACE_GESTURE_LOOK_RIGHT);
  Engine_EventWait(20);
  Event_ShowMessageAndWait(SPEAKER_WINDOW_ABOVE | ACTOR_LAUREL, 0, 20);
  KolimaForestTree_PlayGesture(ACTOR_TRET_BLANK, FACE_GESTURE_HALF_CLOSE_EYES);
  Engine_EventWait(40);
  KolimaForestTree_PlayGesture(ACTOR_TRET_BLANK, FACE_GESTURE_LOOK_LEFT);
  Engine_EventWait(20);
  Event_ShowMessageAndWait(SPEAKER_WINDOW_ABOVE | ACTOR_TRET, 0, 20);
  Engine_ActorRunRepeatedMotion(GARET, 2);
  Actor_FaceDirection(GARET, FACING_WEST, 10);
  Event_ShowMessage(GARET, 0);
  Actor_FaceDirection(IVAN, FACING_WEST, 20);
  Event_ShowMessage(SPEAKER_WINDOW_BELOW | IVAN, 0);
  KolimaForestTree_PlayGesture(ACTOR_LAUREL_BLANK, FACE_GESTURE_BLINK);
  Engine_EventWait(20);
  Event_ShowMessageAndWait(SPEAKER_WINDOW_ABOVE | ACTOR_LAUREL, 0, 20);
  Actor_FaceDirection(GARET, FACING_NORTHEAST, 0);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_SOUTHWEST, 10);
  Event_ShowMessageAndWait(GARET, 0, 20);
  KolimaForestTree_PlayGesture(ACTOR_TRET_BLANK, FACE_GESTURE_OPEN_EYES);
  Event_ShowMessageAndWait(SPEAKER_WINDOW_BELOW | ACTOR_TRET, 0, 10);
  KolimaForestTree_PlayGesture(ACTOR_TRET_BLANK, FACE_GESTURE_LOOK_LEFT);
  Engine_EventWait(20);
  KolimaForestTree_PlayGesture(ACTOR_LAUREL_BLANK, FACE_GESTURE_LOOK_RIGHT);
  Engine_EventWait(40);
  KolimaForestTree_PlayGesture(ACTOR_LAUREL_BLANK, FACE_GESTURE_CLOSE_EYES);
  Engine_EventWait(20);
  KolimaPalette_Restore(0);
  Engine_ColorBufferInterpolate(1);
  Engine_TaskWait(1);
  ColorBuffer_ApplyTarget(0x406218, 1);
  Engine_ColorBufferInterpolate(40);
  Engine_EventWait(60);
  gKolimaForestTreeSparkFrame = 0;
  // Laurel looks beyond the river and warns of the forest to the south.
  gKolimaForestTreeSparkOrigin[0] = PIXELS(136);
  gKolimaForestTreeSparkOrigin[1] = PIXELS(20);
  spawnOrbitSparks = KolimaForestTree_SpawnOrbitSparks;
  gKolimaForestTreeSparkOrigin[2] = PIXELS(258);
  FieldScheduler_AddCallback(spawnOrbitSparks, TASK_PRIORITY_SCENE);
  Engine_EventWait(100);
  KolimaForestTree_FaceParty(FACING_SOUTHWEST, 40);
  Engine_ActorStartRepeatedMotion(IVAN, 1);
  Actor_ShowEmote(IVAN, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 20);
  Event_ShowMessageAndWait(SPEAKER_WINDOW_BELOW | IVAN, 0, 10);
  Engine_ActorRunRepeatedMotion(ACTOR_PARTY_LEADER, 2);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_SOUTHEAST, 10);
  Engine_ActorSetAnimationAndWait(ACTOR_PARTY_LEADER, ANIMATION_NOD);
  KolimaForestTree_PlayGesture(ACTOR_TRET_BLANK, FACE_GESTURE_BLINK);
  Engine_EventWait(20);
  Event_ShowMessage(SPEAKER_WINDOW_BELOW | ACTOR_TRET, 0);
  Actor_ShowEmote(IVAN, EMOTE_IN_FRONT | EMOTE_QUESTION, 60);
  Actor_FaceDirection(IVAN, FACING_NORTH, 10);
  Event_ShowMessageAndWait(SPEAKER_WINDOW_BELOW | IVAN, 0, 10);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 0);
  Actor_FaceDirection(MIA, FACING_NORTH, 0);
  Actor_FaceDirection(GARET, FACING_NORTH, 20);
  Event_ShowMessageAndWait(SPEAKER_WINDOW_BELOW | ACTOR_TRET, 0, 10);
  KolimaForestTree_NodTogether();
  Engine_EventWait(10);
  KolimaForestTree_FaceParty(FACING_SOUTHWEST, 120);
  Engine_TaskRemoveCallback(spawnOrbitSparks);
  Engine_EventWait(60);
  KolimaPalette_Restore(0);
  Engine_ColorBufferInterpolate(40);
  KolimaForestTree_PlayGesture(ACTOR_TRET_BLANK, FACE_GESTURE_LOOK_LEFT);
  Engine_EventWait(20);
  Event_ShowMessageAndWait(SPEAKER_WINDOW_BELOW | ACTOR_TRET, 0, 20);
  KolimaForestTree_PlayGesture(ACTOR_LAUREL_BLANK, FACE_GESTURE_LOOK_RIGHT);
  Event_ShowMessage(SPEAKER_WINDOW_ABOVE | ACTOR_LAUREL, 0);
  Event_ShowMessage(SPEAKER_WINDOW_BELOW | ACTOR_TRET, 0);
  KolimaForestTree_PlayGesture(ACTOR_LAUREL_BLANK, FACE_GESTURE_BLINK);
  Engine_EventWait(20);
  Event_ShowMessageAndWait(SPEAKER_WINDOW_ABOVE | ACTOR_LAUREL, 0, 10);
  Engine_ActorStartRepeatedMotion(ACTOR_PARTY_LEADER, 2);
  Engine_ActorStartRepeatedMotion(GARET, 2);
  Engine_ActorStartRepeatedMotion(MIA, 2);
  Engine_ActorRunRepeatedMotion(IVAN, 2);
  KolimaForestTree_PlayGesture(ACTOR_TRET_BLANK, FACE_GESTURE_OPEN_EYES);
  Event_OpenMessage(SPEAKER_WINDOW_BELOW | ACTOR_TRET, 0);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 0);
  Actor_FaceDirection(GARET, FACING_NORTHEAST, 0);
  Actor_FaceDirection(MIA, FACING_NORTH, 0);
  Actor_FaceDirection(IVAN, FACING_NORTHWEST, 0);
  if (Engine_EventChooseYesNo(0, 0) == 1) {
    gEventWork->message += 1;
  }
  Engine_EventWait(10);
  KolimaForestTree_PlayGesture(ACTOR_TRET_BLANK, FACE_GESTURE_LOOK_LEFT);
  Engine_EventWait(20);
  KolimaForestTree_PlayGesture(ACTOR_LAUREL_BLANK, FACE_GESTURE_LOOK_RIGHT);
  Engine_EventWait(40);
  KolimaForestTree_PlayGesture(ACTOR_TRET_BLANK, FACE_GESTURE_OPEN_EYES);
  Engine_EventWait(20);
  Event_ShowMessageAndWait(SPEAKER_WINDOW_BELOW | ACTOR_TRET, 0, 10);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_SOUTH, 0);
  Actor_FaceDirection(GARET, FACING_EAST, 0);
  Actor_FaceDirection(MIA, FACING_NORTH, 0);
  Actor_FaceDirection(IVAN, FACING_WEST, 10);
  KolimaForestTree_NodTogether();
  Event_PlayCue(AUDIO_CUE_FADE_BGM);
  // Rejoin the party and exchange the used water for its empty bottle.
  finalActions = gKolimaForestTree_RejoinPartyActions;
  Actor_SetActionScript(GARET, finalActions);
  if (includeMia != 0) {
    Actor_SetActionScript(MIA, finalActions);
  }
  Actor_RunActionScriptAndWait(IVAN, finalActions);
  KolimaForestTree_PlayGesture(ACTOR_TRET_BLANK, FACE_GESTURE_BLINK);
  KolimaForestTree_PlayGesture(ACTOR_TRET_BLANK, FACE_GESTURE_BLINK);
  Engine_EventWait(20);
  Engine_EventSetMessage(MsgKolimaOweGreatDebtHaveSaved);
  Event_ShowMessage(SPEAKER_WINDOW_BELOW | ACTOR_TRET, 0);
  KolimaForestTree_PlayGesture(ACTOR_LAUREL_BLANK, FACE_GESTURE_BLINK);
  KolimaForestTree_PlayGesture(ACTOR_LAUREL_BLANK, FACE_GESTURE_BLINK);
  Engine_EventWait(20);
  Event_ShowMessageAndWait(SPEAKER_WINDOW_ABOVE | ACTOR_LAUREL, 0, 10);
  Engine_ActorSetAnimationAndWait(ACTOR_PARTY_LEADER, ANIMATION_NOD);
  GameFlag_Set(FLAG_KOLIMA_CURSE_LIFTED);
  Event_PlayCue(MUS_001);
  KolimaForestTree_ReplaceInventoryItem(ITEM_HERMES_WATER, ITEM_EMPTY_BOTTLE);
}

/// Sets up Tret, Laurel and the forest's colours on entry. Tret, Laurel and the
/// wrathful side stand 28 pixels up, nothing passes through them, Retreat is
/// set to the forest's way in, and the forest takes its cursed colours until
/// the curse is lifted.
s32 KolimaForestTree_Initialize(void) {
  struct FieldActor *tretBlank;
  struct FieldActor *tretsWrathfulSide;
  struct FieldActor *laurelBlank;

  tretBlank = Object_GetById(ACTOR_TRET_BLANK);
  tretsWrathfulSide = Object_GetById(ACTOR_TRETS_WRATHFUL_SIDE);
  laurelBlank = Object_GetById(ACTOR_LAUREL_BLANK);
  Engine_TaskWait(1);
  Engine_ActorSetChildValue(ACTOR_TRETS_WRATHFUL_SIDE, 15);
  gEventWork->startTransition =
      SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_INTERLACED_WIPE);
  gGameState.retreatScene = (s32)&SceneId_KolimaForestEntrance;
  gGameState.retreatEntrance = KOLIMA_FOREST_ENTRANCE_FROM_WORLD_MAP;
  if (!Engine_GameFlagIsSet(FLAG_KOLIMA_CURSE_LIFTED))
    KolimaPalette_Curse(3);
  Object_GetById(ACTOR_TRET)->radius = 6;
  Object_GetById(ACTOR_LAUREL)->radius = 6;
  Object_GetById(ACTOR_TRET_STAND_IN)->radius = 6;
  Object_GetById(ACTOR_LAUREL_STAND_IN)->radius = 6;
  Engine_ActorSetSpriteFlags(Object_GetById(ACTOR_TRETS_WRATHFUL_SIDE), 0);
  Engine_ActorSetSpriteFlags(Object_GetById(ACTOR_TRET_BLANK), 0);
  Engine_ActorSetSpriteFlags(Object_GetById(ACTOR_LAUREL_BLANK), 0);
  Engine_ActorSetSpriteFlags(Object_GetById(ACTOR_TRET), 0);
  Engine_ActorSetSpriteFlags(Object_GetById(ACTOR_LAUREL), 0);
  Engine_ActorSetSpritePriority(ACTOR_TRET, 2);
  Engine_ActorSetSpritePriority(ACTOR_TRETS_WRATHFUL_SIDE, 2);
  Engine_ActorSetSpritePriority(ACTOR_LAUREL, 2);
  tretBlank->motionFlags = 0;
  tretBlank->y.fixed = PIXELS(28);
  laurelBlank->motionFlags = 0;
  laurelBlank->y.fixed = PIXELS(28);
  tretsWrathfulSide->motionFlags = 0;
  tretsWrathfulSide->y.fixed = PIXELS(28);
  Engine_ActorSetAnimation(ACTOR_LAUREL, LAUREL_FACE_CLOSED);
  Engine_ActorSetAnimation(ACTOR_TRET, TRET_FACE_CLOSED);
  Object_GetById(ACTOR_TRET)->collisionFlags |= ACTOR_NOT_TARGETED;
  Object_GetById(ACTOR_LAUREL)->collisionFlags |= ACTOR_NOT_TARGETED;
  Object_GetById(ACTOR_TRET_BLANK)->collisionFlags |= ACTOR_NOT_TARGETED;
  Object_GetById(ACTOR_LAUREL_BLANK)->collisionFlags |= ACTOR_NOT_TARGETED;
  Object_GetById(ACTOR_TRETS_WRATHFUL_SIDE)->collisionFlags |=
      ACTOR_NOT_TARGETED;
  return 0;
}

#include "common/kolima_grow.h"

/// Spawns the growing sparks that repel the party leader.
static void KolimaForestTree_SpawnRejectionSpark(void) {
  struct FieldActor *effect;
  struct AnimationObject *sprite;
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
  effectFlags = 0xff & ~ACTOR_PRIORITY_AUTOMATIC;
  effectFlags &= effect->priorityFlags;
  effect->priorityFlags = effectFlags;
  sprite->image.priority = 1;
  effect->scaleX = Q16_ONE / 10;
  effect->speed = PIXELS(4);
  effect->acceleration = PIXELS(4);
  effect->motionFlags = phase;
  Object_SetMode(effect, 2);
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
static __inline__ void KolimaForestTree_AlternateFaceFrames(s32 actorId,
                                                            s32 firstFrame,
                                                            s32 secondFrame) {
  Engine_ActorSetAnimation(actorId, firstFrame);
  Engine_TaskWait(6);
  Engine_ActorSetAnimation(actorId, secondFrame);
  Engine_TaskWait(6);
  Engine_ActorSetAnimation(actorId, firstFrame);
  Engine_TaskWait(6);
  Engine_ActorSetAnimation(actorId, secondFrame);
  Engine_TaskWait(6);
  Engine_ActorSetAnimation(actorId, firstFrame);
}

/// Plays one of Tret's or Laurel's face gestures, chosen by the blank actor at
/// their spot.
static void KolimaForestTree_PlayGesture(s32 blankActorId, s32 gesture) {
  if (blankActorId == ACTOR_TRET_BLANK) {
    switch (gesture) {
    case FACE_GESTURE_CLOSE_EYES:
      Engine_ActorSetAnimation(ACTOR_TRET, TRET_FACE_OPEN);
      Engine_TaskWait(6);
      Engine_ActorSetAnimation(ACTOR_TRET, TRET_FACE_CLOSED);
      break;
    case FACE_GESTURE_OPEN_EYES:
      Engine_ActorSetAnimation(ACTOR_TRET, TRET_FACE_OPEN);
      break;
    case FACE_GESTURE_LOOK_LEFT:
      Engine_ActorSetAnimation(ACTOR_TRET, TRET_FACE_OPEN);
      Engine_TaskWait(6);
      Engine_ActorSetAnimation(ACTOR_TRET, TRET_FACE_LOOK_LEFT);
      break;
    case FACE_GESTURE_LOOK_RIGHT:
      Engine_ActorSetAnimation(ACTOR_TRET, TRET_FACE_OPEN);
      Engine_TaskWait(6);
      Engine_ActorSetAnimation(ACTOR_TRET, TRET_FACE_LOOK_RIGHT);
      break;
    case FACE_GESTURE_BLINK:
      KolimaForestTree_AlternateFaceFrames(ACTOR_TRET, TRET_FACE_OPEN,
                                           TRET_FACE_CLOSED);
      break;
    case FACE_GESTURE_HALF_CLOSE_EYES:
      Engine_ActorSetAnimation(ACTOR_TRET, TRET_FACE_OPEN);
      Engine_TaskWait(6);
      Engine_ActorSetAnimation(ACTOR_TRET, TRET_FACE_HALF_CLOSED);
      break;
    case FACE_GESTURE_WRATHFUL_CLOSE_EYES:
      Engine_ActorSetAnimation(ACTOR_TRET, TRET_FACE_WRATHFUL_OPEN);
      Engine_TaskWait(6);
      Engine_ActorSetAnimation(ACTOR_TRET, TRET_FACE_WRATHFUL_CLOSED);
      break;
    case FACE_GESTURE_WRATHFUL_LOOK_RIGHT:
      Engine_ActorSetAnimation(ACTOR_TRET, TRET_FACE_WRATHFUL_OPEN);
      Engine_TaskWait(6);
      Engine_ActorSetAnimation(ACTOR_TRET, TRET_FACE_WRATHFUL_LOOK_RIGHT);
      break;
    case FACE_GESTURE_WRATHFUL_LOOK_LEFT:
      Engine_ActorSetAnimation(ACTOR_TRET, TRET_FACE_WRATHFUL_OPEN);
      Engine_TaskWait(6);
      Engine_ActorSetAnimation(ACTOR_TRET, TRET_FACE_WRATHFUL_LOOK_LEFT);
      break;
    case FACE_GESTURE_WRATHFUL_BLINK:
      KolimaForestTree_AlternateFaceFrames(ACTOR_TRET, TRET_FACE_WRATHFUL_OPEN,
                                           TRET_FACE_WRATHFUL_CLOSED);
      break;
    case FACE_GESTURE_WRATHFUL_OPEN_EYES:
    case FACE_GESTURE_WRATHFUL_STARE:
      Engine_ActorSetAnimation(ACTOR_TRET, TRET_FACE_WRATHFUL_OPEN);
      break;
    }
  } else {
    switch (gesture) {
    case FACE_GESTURE_CLOSE_EYES:
      Engine_ActorSetAnimation(ACTOR_LAUREL, LAUREL_FACE_OPEN);
      Engine_TaskWait(6);
      Engine_ActorSetAnimation(ACTOR_LAUREL, LAUREL_FACE_CLOSED);
      break;
    case FACE_GESTURE_OPEN_EYES:
      Engine_ActorSetAnimation(ACTOR_LAUREL, LAUREL_FACE_OPEN);
      break;
    case FACE_GESTURE_LOOK_LEFT:
      Engine_ActorSetAnimation(ACTOR_LAUREL, LAUREL_FACE_OPEN);
      Engine_TaskWait(6);
      Engine_ActorSetAnimation(ACTOR_LAUREL, LAUREL_FACE_LOOK_LEFT);
      break;
    case FACE_GESTURE_LOOK_RIGHT:
      Engine_ActorSetAnimation(ACTOR_LAUREL, LAUREL_FACE_OPEN);
      Engine_TaskWait(6);
      Engine_ActorSetAnimation(ACTOR_LAUREL, LAUREL_FACE_LOOK_RIGHT);
      break;
    case FACE_GESTURE_BLINK:
      KolimaForestTree_AlternateFaceFrames(ACTOR_LAUREL, LAUREL_FACE_OPEN,
                                           LAUREL_FACE_CLOSED);
      break;
    case FACE_GESTURE_HALF_CLOSE_EYES:
      Engine_ActorSetAnimation(ACTOR_LAUREL, LAUREL_FACE_OPEN);
      Engine_TaskWait(6);
      Engine_ActorSetAnimation(ACTOR_LAUREL, LAUREL_FACE_HALF_CLOSED);
      break;
    }
  }
  Engine_TaskWait(12);
}

/// Expands a spark along its spiral for 120 frames, then releases its cache
/// slot.
static void KolimaForestTree_UpdateOrbitSpark(union FieldObject *fieldObject) {
  struct FieldActor *spark = &fieldObject->actor;
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
    Resource_ResetEntry(spark->sprite->slot);
    Engine_ObjectDispatchRelease(spark);
  }
}

/// Emits six-spark rings at ten-frame intervals during each burst.
static void KolimaForestTree_SpawnOrbitSparks(void) {
  struct FieldActor *spark;
  struct AnimationObject *previousSprite;
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
        previousSprite = Sprite_ShareVramCache(spark->sprite, previousSprite);
        spark->motionFlags = 0;
        spark->sprite->image.priority = 1;
        Engine_ActorSetSpriteFlags(spark, 0);
        Object_SetMode(spark, 1);
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
  s32 owner = PartyInventory_FindOwner(oldItemId);

  if (owner != -1) {
    s32 inventorySlot = Inventory_Find(owner, oldItemId);

    if (inventorySlot != -1) {
      UnitGet(owner)->inventory[inventorySlot] = newItemId;
    }
  }
}

#include "common/kolima_palette.h"

s32 gKolimaForestTree_GaretApproachActions[] = {
    SC_ARG(2), SC_MOVE(PIXELS(312), 0, PIXELS(230)), SC_SYNC,
    SC_ARG(1), SC_SET(SW_FACING, FACING_NORTH),      SC_WAIT(1),
    SC_END,
};

s32 gKolimaForestTree_IvanApproachActions[] = {
    SC_ARG(2), SC_MOVE(PIXELS(346), 0, PIXELS(224)), SC_SYNC,
    SC_ARG(1), SC_SET(SW_FACING, FACING_NORTH),      SC_WAIT(1),
    SC_END,
};

s32 gKolimaForestTree_MiaApproachActions[] = {
    SC_ARG(2), SC_MOVE(PIXELS(333), 0, PIXELS(240)), SC_SYNC,
    SC_ARG(1), SC_SET(SW_FACING, FACING_NORTH),      SC_WAIT(1),
    SC_END,
};

s32 gKolimaForestTree_RejoinPartyActions[] = {
    SC_ARG(2),  SC_MOVE(PIXELS(328), 0, PIXELS(212)),
    SC_SYNC,    SC_POS(0, 0, 0),
    SC_WAIT(1), SC_END,
};

struct SceneEntrance gKolimaForestTreeEntrances[] = {
    {ENTRANCE_DEFAULT, CONDITION_ALWAYS, 209, 0, 279, FACING_SOUTH, 0, 0, 0,
     512, 448, 0},
    {KOLIMA_FOREST_TREE_FROM_KOLIMA_FOREST_FLOODGATE, CONDITION_ALWAYS, 136, 0,
     424, FACING_NORTH, 0, 0, 0, 512, 448, 0},
    {KOLIMA_FOREST_TREE_FROM_TRET_TREE_INTERIORS, CONDITION_ALWAYS, 328, 16,
     200, FACING_SOUTH, 0, 0, 0, 512, 448, 0},
    {SCENE_TABLE_END},
};

// clang-format off
struct SceneRegion gKolimaForestTreeRegions[] = {
    {320, 88, 176, 336, 104, 192, -1, KOLIMA_FOREST_TREE_TO_TRET_TREE_INTERIORS},
    {-1, 0, 0, 0, 0, 0, 0, 0},
};

u32 gKolimaForestTreeExits[] = {
    SCENE_EXITS(KolimaForestTree),
    SCENE_EXIT(KOLIMA_FOREST_TREE_TO_KOLIMA_FOREST_FLOODGATE, KolimaForestFloodgate, KOLIMA_FOREST_FLOODGATE_FROM_KOLIMA_FOREST_TREE),
    SCENE_EXIT(KOLIMA_FOREST_TREE_TO_TRET_TREE_INTERIORS, TretTreeInteriors, TRET_TREE_INTERIORS_FROM_KOLIMA_FOREST_TREE),
    SCENE_EXITS_END,
};

struct ScenePlacement gKolimaForestTreePlacements[] = {
    { SPRITE_TRET_RAISED, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(327), 0, PIXELS(192), FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { SPRITE_LAUREL, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(136), 0, PIXELS(270), FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { SPRITE_BLANK, CONDITION_ALWAYS, ACTOR_RESET_ACTION,
        PIXELS(327), 0, PIXELS(192), FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { SPRITE_BLANK, CONDITION_ALWAYS, ACTOR_RESET_ACTION,
        PIXELS(136), 0, PIXELS(270), FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { SPRITE_BLANK, CONDITION_ALWAYS, ACTOR_RESET_ACTION,
        PIXELS(327), 0, PIXELS(194), FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { SPRITE_BLANK, CONDITION_ALWAYS, ACTOR_RESET_ACTION,
        PIXELS(136), 0, PIXELS(272), FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { SPRITE_TRET, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(327), 0, PIXELS(192), FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { GARET, CONDITION_ALWAYS, ACTOR_STAND,
        0, 0, 0, FACING_EAST, TALK_KEEP_FACING, 0 },
    { IVAN, CONDITION_ALWAYS, ACTOR_STAND,
        0, 0, 0, FACING_EAST, TALK_KEEP_FACING, 0 },
    { MIA, CONDITION_ALWAYS, ACTOR_STAND,
        0, 0, 0, FACING_EAST, TALK_KEEP_FACING, 0 },
    { SCENE_TABLE_END },
};

struct SceneEvent gKolimaForestTreeEvents[] = {
    { EVENT_EXIT, 1, CONDITION_ALWAYS, KOLIMA_FOREST_TREE_TO_KOLIMA_FOREST_FLOODGATE },
    { USE_ITEM_ON_ACTOR(ITEM_HERMES_WATER), ACTOR_TRET_STAND_IN, FLAG_KOLIMA_CURSE_LIFTED, EVENT_SCRIPT(KolimaForestTree_UseHermesWater) },
    { EVENT_TALK, ACTOR_LAUREL_STAND_IN, CONDITION_ALWAYS, EVENT_SCRIPT(KolimaForestTree_TalkToLaurel) },
    { EVENT_TALK, ACTOR_TRET_STAND_IN, CONDITION_ALWAYS, EVENT_SCRIPT(KolimaForestTree_TalkToTret) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_LAUREL_STAND_IN, CONDITION_ALWAYS, EVENT_SCRIPT(KolimaForestTree_ReadLaurelThoughts) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 0x400 | ACTOR_TRET_STAND_IN, FLAG_TRET_DEFEATED, EVENT_SCRIPT(KolimaForestTree_TalkToTret) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_TRET_STAND_IN, CONDITION_ALWAYS, EVENT_SCRIPT(KolimaForestTree_ReadTretThoughts) },
    { SCENE_EVENTS_END },
};
// clang-format on

s32 gKolimaForestTreeEffectScript[] = {
    SC_ADD(SW_SCALE_X, Q16_ONE / 10),
    SC_CALL(FieldEffect_StretchUntilArrived),
    SC_LOOP(SC_FOREVER, SC_TOP),
};
