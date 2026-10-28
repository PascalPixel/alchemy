/// @file
/// Kolima Forest (SceneId_KolimaForestTree), the last of the forest's five
/// scenes, where Tret and Laurel, the spirits of the holy trees, stand. The
/// party has come from Kolima to reawaken Tret's gentle side and save the
/// villagers he turned into trees, and here the curse is lifted.
///
/// At first Tret is silent and his wrathful side answers for him
/// (ACTOR_TRETS_WRATHFUL_SIDE, with a portrait): "I control all in Tret's
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
/// Switch's scene (SceneId_KolimaForestFloodgate).

#include "speaker.h"
#include "main_routine_work.h"
#include "module/scene/kolima_forest_tree.h"
#include "module/scene/shared/kolima_palette.h"
#include "module/overworld/overworld.h"
#include "module/overworld/event_commands.h"
#include "module/game/game.h"
#include "module/system/system.h"
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
#include "actor.h"
#include "field_effect.h"
#include "battle_unit.h"
#include "types.h"
#include "call.h"
#include "event.h"
#include "system_call.h"
#include "scene_ids.h"
#include "palette_map.h"
#include "io_reg.h"
#include "scene_tables.h"
#include "text/msg_ids.h"
#include "ability_ids.h"
#include "actor_script.h"
#include "fixed_math.h"
#include "palbuf.h"
#include "flags.h"
#include "character_ids.h"
#include "item_ids.h"
#include "macros.h"
#include "sounds.h"
#include "sound_driver_symbols.h"

/// The actors, numbering gKolimaForestTreeActorPlacements' rows
/// from ACTOR_FIRST_PLACED.
enum {
  /// Tret's face, drawn in SPRITE_TRET_RAISED; its animations are his face's
  /// gestures (KolimaForestTree_PlayGesture).
  /// ★ Named from the game: "The Water of Hermes seeped into Tret."
  /// (MsgKolimaWaterHermesSeepedIntoTret)
  ACTOR_TRET = ACTOR_FIRST_PLACED,
  /// ★ Named from the game: "Oh! Laurel, you were given powers, too?"
  /// (MsgKolimaWasIndeedAngryPeopleHad)
  ACTOR_LAUREL,
  // XXX (no evidence): why these exist; likely left from a version where
  // they were drawn, but the ROM keeps no such version.
  /// Blank actors 28 pixels up at Tret's and Laurel's spots, never
  /// targeted;
  /// nothing addresses them but KolimaForestTree_PlayGesture, which takes one
  /// to say whose face to move.
  /// ★ Named from the game: "Tret" (MsgEnemyName)
  ACTOR_INVISIBLE_TRET,
  /// ★ Named from the game: "Laurel" (MsgDebugGlyphName)
  ACTOR_INVISIBLE_LAUREL,
  /// An invisible actor on Tret's spot that answers for him:
  /// talking to it, Mind Read on it and Hermes' Water used on it reach Tret.
  /// ★ Named from the game: "The Water of Hermes seeped into Tret."
  /// (MsgKolimaWaterHermesSeepedIntoTret)
  ACTOR_TRET_STAND_IN,
  /// The invisible actor on Laurel's spot that answers for her when she
  /// is
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
const struct SceneExitRegion *KolimaForestTree_GetRegions(void) {
  return gKolimaForestTreeRegions;
}

/// The ways back (SceneId_KolimaForestFloodgate) and into the Tret Tree
/// (SceneId_TreeRooms).
const u32 *KolimaForestTree_GetExits(void) { return gKolimaForestTreeExits; }

/// Tret and Laurel, the blank actors at their spots, the ones the party
/// talks to, Tret's wrathful side, and Garet, Ivan and Mia for the healing.
const struct ActorPlacement *KolimaForestTree_GetActorPlacements(void) {
  return gKolimaForestTreeActorPlacements;
}

/// Unused: takes the touched exit after the leader's current movement; no
/// trigger row names it.
UNUSED static void KolimaForestTree_ExitTouchedRegion(void) {
  struct FieldWork *work;

  work = gFieldWork;
  Event_Start();
  Actor_CommitPositionAndActivate(ACTOR_PARTY_LEADER, 0, 0);
  Import_EventSetExitRequest(work->stepTrigger);
  Event_End();
}

/// Talking to Tret and Laurel, reading their minds, and Hermes' Water on Tret.
const struct Trigger *KolimaForestTree_GetTriggers(void) {
  return gKolimaForestTreeTriggers;
}

/// Kolima Forest (SceneId_KolimaForestTree), talking to Laurel
/// (ACTOR_LAUREL_STAND_IN): the first time, Laurel asks who the party is,
/// recognises the ones who resisted Tret's glamour and senses a great warmth in
/// them, but says they are too late to rescue Tret, and tells them to leave
/// before the forest is destroyed (sets FLAG_MET_LAUREL); then only that
/// warning. Once the curse is lifted, the forest is alive again thanks to the
/// party, and Laurel says, "We shall protect the forest from now on."
static void KolimaForestTree_TalkToLaurel(void) {
  Event_Start();
  KolimaForestTree_PlayGesture(ACTOR_INVISIBLE_LAUREL, FACE_GESTURE_OPEN_EYES);
  if (GameFlag_Get(FLAG_KOLIMA_CURSE_LIFTED) != 0) {
    Event_SetMessageId(MsgKolimaForestKolimaAlive);
    Event_ShowMessageAndWait(ACTOR_LAUREL, 0);
  } else if (Event_GetFlag(FLAG_MET_LAUREL) != 0) {
    Event_SetMessageId(MsgKolimaLeaveBeforeForest);
    Event_ShowMessageAndWait(ACTOR_LAUREL, 0);
  } else {
    Event_SetMessageId(MsgKolimaWho);
    Event_ShowMessageThenWait(ACTOR_LAUREL, 0, 20);
    KolimaForestTree_PlayGesture(ACTOR_INVISIBLE_LAUREL,
                                 FACE_GESTURE_CLOSE_EYES);
    Event_Wait(60);
    KolimaForestTree_PlayGesture(ACTOR_INVISIBLE_LAUREL,
                                 FACE_GESTURE_OPEN_EYES);
    Event_ShowMessageThenWait(ACTOR_LAUREL, 0, 10);
    Actor_SetAnimationAndWait(ACTOR_PARTY_LEADER, ANIM_NOD);
    Event_Wait(40);
    Event_ShowMessageAndWait(ACTOR_LAUREL, 0);
    KolimaForestTree_PlayGesture(ACTOR_INVISIBLE_LAUREL,
                                 FACE_GESTURE_CLOSE_EYES);
    Event_Wait(80);
    Event_ShowMessageThenWait(ACTOR_LAUREL, 0, 20);
    KolimaForestTree_PlayGesture(ACTOR_INVISIBLE_LAUREL,
                                 FACE_GESTURE_OPEN_EYES);
    Event_ShowMessageThenWait(ACTOR_LAUREL, 0, 20);
    GameFlag_Set(FLAG_MET_LAUREL);
  }
  KolimaForestTree_PlayGesture(ACTOR_INVISIBLE_LAUREL, FACE_GESTURE_CLOSE_EYES);
  Event_End();
}

/// Kolima Forest (SceneId_KolimaForestTree), talking to Tret
/// (ACTOR_TRET_STAND_IN); until FLAG_TRET_DEFEATED Mind Read on him runs this
/// too, with no effect of its own (subject bit 10). Before the Tret Tree, he is
/// silent and his wrathful side answers, the screen darkening around its words;
/// after it he is silent, and if anyone carries Hermes' Water the File Screen
/// opens. Once the curse is lifted, no axe shall touch him again.
static void KolimaForestTree_TalkToTret(void) {
  s32 waterHolder;

  Event_Start();
  if (GameFlag_Get(FLAG_KOLIMA_CURSE_LIFTED) != 0) {
    KolimaForestTree_PlayGesture(ACTOR_INVISIBLE_TRET, FACE_GESTURE_OPEN_EYES);
    Event_SetMessageId(MsgKolimaNowHaveSuchPowerAxe);
    Event_ShowMessage(ACTOR_TRET, 0);
    KolimaForestTree_PlayGesture(ACTOR_INVISIBLE_TRET, FACE_GESTURE_CLOSE_EYES);
  } else {
    if (GameFlag_Get(FLAG_TRET_DEFEATED) != 0) {
      KolimaForestTree_PlayGesture(ACTOR_INVISIBLE_TRET,
                                   FACE_GESTURE_OPEN_EYES);
      Event_SetMessageId(MsgKolimaSilence2);
      Event_ShowMessage(ACTOR_TRET, 0);
      KolimaForestTree_PlayGesture(ACTOR_INVISIBLE_TRET,
                                   FACE_GESTURE_CLOSE_EYES);
      waterHolder = PartyInventory_FindMember(ITEM_HERMES_WATER);
      if (waterHolder != -1) {
        u16 *menuRequest = &gFieldWork->fieldMenu;
        u16 open = 1;

        *menuRequest = open;
      }
    } else {
      Event_SetMessageId(MsgKolimaSilence);
      Event_ShowMessage(ACTOR_TRET, 0);
      PaletteFade_ApplyTarget(PALETTE_TINT_BLEND | RGB(24, 16, 24),
                              PALETTES_BG);
      PaletteFade_StartInterpolation(20);
      wait(40);
      FieldEvent_ShowMessageThenWait(
          SPEAKER_PORTRAIT_RIGHT | ACTOR_TRETS_WRATHFUL_SIDE, 0, 10);
      Actor_RunRepeatedMotion(ACTOR_PARTY_LEADER, 2);
      Event_ShowMessage(SPEAKER_PORTRAIT_RIGHT | ACTOR_TRETS_WRATHFUL_SIDE, 0);
      PaletteFade_ApplyTarget(PALETTE_TINT_NONE, PALETTES_BG);
      PaletteFade_StartInterpolation(20);
      wait(40);
    }
  }
  Event_End();
}

/// Kolima Forest (SceneId_KolimaForestTree), Mind Read on Laurel: before the
/// curse is lifted, the healing waters of Mercury Lighthouse might save Tret,
/// but its fountain has dried up; afterwards, it must be horrible beyond the
/// river.
static void KolimaForestTree_ReadLaurelThoughts(void) {
  Event_Start();
  if (GameFlag_Get(FLAG_KOLIMA_CURSE_LIFTED) != 0) {
    Event_SetMessageId(MsgKolimaMustHorribleBeyondRiverAm);
  } else {
    Event_SetMessageId(MsgKolimaHealingWatersMercuryLighthouseMight);
  }
  Event_ShowMessageAndWait(ACTOR_LAUREL, 0);
  Event_End();
}

/// Kolima Forest (SceneId_KolimaForestTree), Mind Read on Tret: before the
/// curse is lifted, "People of Kolima... Forgive me..."; afterwards, a plea to
/// be careful beyond the river.
static void KolimaForestTree_ReadTretThoughts(void) {
  Event_Start();
  if (GameFlag_Get(FLAG_KOLIMA_CURSE_LIFTED) != 0) {
    Event_SetMessageId(MsgKolimaKnowCannotStopButPlease);
  } else {
    Event_SetMessageId(MsgKolimaPeopleKolimaForgiveMe);
  }
  Event_ShowMessageAndWait(ACTOR_TRET, 0);
  Event_End();
}

/// Kolima Forest (SceneId_KolimaForestTree), using Hermes' Water on Tret while
/// the curse lasts: before the evil within him is defeated in the Tret Tree
/// (FLAG_TRET_DEFEATED), his wrathful side repels it
/// (KolimaForestTree_RejectHermesWater); afterwards it heals him
/// (KolimaForestTree_RestoreTret).
static void KolimaForestTree_UseHermesWater(void) {
  Event_Start();
  Field_EndBusyAndReveal();
  if (GameFlag_Get(FLAG_TRET_DEFEATED) == 0) {
    KolimaForestTree_RejectHermesWater();
  } else {
    KolimaForestTree_RestoreTret();
  }
  Event_End();
}

/// Tret's wrathful side repels Hermes' Water: sparks gather, the leader is
/// thrown back and lands, and the voice says "I control Tret's heart! I shall
/// not let you do anything to him!"
static void KolimaForestTree_RejectHermesWater(void) {
  struct Actor *leader;

  // The wrathful side throws the leader back before the water can heal Tret.
  leader = Actor_Get(ACTOR_PARTY_LEADER);
  Actor_ArmCallback(ACTOR_PARTY_LEADER, FACING_NORTH, 0);
  PaletteFade_ApplyTarget(PALETTE_TINT_BLEND | RGB(24, 16, 24), PALETTES_BG);
  PaletteFade_StartInterpolation(20);
  wait(40);
  Event_PlaySound(SOUND_FADE_OUT_BGM);
  gKolimaForestTreePlaySparkSound = 1;
  Task_AddFrame(KolimaForestTree_SpawnRejectionSpark, TASK_PASS_FRAME);
  wait(30);
  gKolimaForestTreePlaySparkSound = 0;
  FieldCamera_MoveTo(PIXELS(328), -1, PIXELS(235), 1);
  Actor_SetSpritePriority(ACTOR_PARTY_LEADER, 1);
  Actor_GetById(ACTOR_PARTY_LEADER)->steeringFlags &= ~ACTOR_FACES_MOTION;
  Import_ActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_LEADER_STRIDE);
  Actor_SetSpeed(ACTOR_PARTY_LEADER, PIXELS(2), PIXELS(2));
  Event_PlaySound(SE_IMPACT);
  leader->velocityY = PIXELS(5);
  leader->gravity = PIXELS(1) / 4;
  leader->bounce = 5 * Q16_ONE / 8;
  Actor_MoveToAndWait(ACTOR_PARTY_LEADER, 335, 258);
  while (leader->velocityY >= 0) {
    wait(1);
  }
  do {
    wait(1);
  } while (leader->velocityY <= 0);
  Event_PlaySound(SE_RATTLE);
  Import_ActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_COLLAPSE);
  Event_Wait(120);
  Task_Remove(KolimaForestTree_SpawnRejectionSpark);
  wait(40);
  leader->bounce = Q16_ONE / 4;
  {
    struct Actor *leader = Actor_Get(ACTOR_PARTY_LEADER);
    leader->steeringFlags = ACTOR_FACES_MOTION | leader->steeringFlags;
  }
  Event_Wait(80);
  // Hear the wrathful side's refusal, then restore the leader's motion.
  Event_SetMessageId(MsgKolimaControlTretsHeartShallNot);
  FieldEvent_ShowMessageThenWait(
      SPEAKER_PORTRAIT_RIGHT | ACTOR_TRETS_WRATHFUL_SIDE, 0, 20);
  Actor_RunRepeatedMotion(ACTOR_PARTY_LEADER, 2);
  Event_Wait(20);
  Event_ShowMessage(SPEAKER_PORTRAIT_RIGHT | ACTOR_TRETS_WRATHFUL_SIDE, 0);
  Scene_PlayMusic();
  PaletteFade_ApplyTarget(PALETTE_TINT_NONE, PALETTES_BG);
  PaletteFade_StartInterpolation(20);
  wait(40);
  {
    s32 facing = FACING_NORTH;

    leader->facing = facing;
  }
  leader->gravity = PIXELS(1);
  leader->bounce = Q16_ONE / 4;
  Actor_RunRepeatedMotion(ACTOR_PARTY_LEADER, 2);
  Event_Wait(40);
  Actor_Jump(ACTOR_PARTY_LEADER, 4, 0);
  Import_ActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_STAND);
  Event_Wait(20);
}

/// Gives all four companions the same nod, waiting on Ivan's animation.
static __inline__ void KolimaForestTree_NodTogether(void) {
  Import_ActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_NOD);
  Import_ActorSetAnimation(ACTOR_GARET, ANIM_NOD);
  Import_ActorSetAnimation(ACTOR_MIA, ANIM_NOD);
  Actor_SetAnimationAndWait(ACTOR_IVAN, ANIM_NOD);
}

/// Turns the companions together and pauses after the final facing change.
static __inline__ void KolimaForestTree_FaceParty(s32 facing, s32 waitFrames) {
  Actor_FaceDirection(ACTOR_PARTY_LEADER, facing, 0);
  Actor_FaceDirection(ACTOR_GARET, facing, 0);
  Actor_FaceDirection(ACTOR_MIA, facing, 0);
  Actor_FaceDirection(ACTOR_IVAN, facing, waitFrames);
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
  struct Actor *leader;
  s32 *paletteFrame;
  s32 flash;
  void (*spawnOrbitSparks)(void);
  const s32 *finalActions;

  // Gather the companions while Hermes' Water seeps into Tret.
  includeMia = GameFlag_Get(FLAG_IN_PARTY(MIA));
  Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 328, 212);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 20);
  Event_PlaySound(SOUND_FADE_OUT_BGM);
  FieldMessage_Show(MsgKolimaWaterHermesSeepedIntoTret, 1);
  Actor_SetSpeed(ACTOR_GARET, PIXELS(1), PIXELS(1) / 2);
  Actor_SetSpeed(ACTOR_IVAN, PIXELS(1), PIXELS(1) / 2);
  leader = Actor_Get(ACTOR_PARTY_LEADER);
  if (leader != 0) {
    Actor_SetPosition(ACTOR_GARET, leader->x.fixed, leader->z.fixed);
  }
  leader = Actor_Get(ACTOR_PARTY_LEADER);
  if (leader != 0) {
    Actor_SetPosition(ACTOR_IVAN, leader->x.fixed, leader->z.fixed);
  }
  Actor_SetScript(ACTOR_GARET, gKolimaForestTree_GaretApproachActions);
  Actor_SetScript(ACTOR_IVAN, gKolimaForestTree_IvanApproachActions);
  if (includeMia != 0) {
    Actor_SetSpeed(ACTOR_MIA, PIXELS(1), PIXELS(1) / 2);
    leader = Actor_Get(ACTOR_PARTY_LEADER);
    if (leader != 0) {
      Actor_SetPosition(ACTOR_MIA, leader->x.fixed, leader->z.fixed);
    }
    Actor_SetScript(ACTOR_MIA, gKolimaForestTree_MiaApproachActions);
  }
  Actor_RefreshSelectorById(ACTOR_IVAN);
  Event_Wait(40);
  KolimaPalette_Restore(0);
  PaletteFade_StartInterpolation(32);
  wait(40);
  // Carry the healing across the forest as its healthy colours return.
  paletteFrame = &gKolimaForestTreePaletteFrame;
  *paletteFrame = 0;
  Task_AddFrame(KolimaForestTree_UpdateHealingPalette, TASK_PASS_FRAME);
  Event_Wait(40);
  Actor_FaceDirection(ACTOR_GARET, FACING_SOUTHWEST, 20);
  FieldCamera_SetSpeed(PIXELS(16) / 5, PIXELS(2) / 5);
  FieldCamera_MoveTo(PIXELS(256), -1, PIXELS(254), 1);
  FieldCamera_WaitForArrival();
  Event_PlaySound(SE_GLITTER);
  Event_Wait(40);
  Actor_FaceDirection(ACTOR_IVAN, FACING_SOUTHEAST, 20);
  FieldCamera_MoveTo(PIXELS(413), -1, PIXELS(261), 1);
  FieldCamera_WaitForArrival();
  Event_PlaySound(SE_GLITTER);
  Event_Wait(40);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_SOUTH, 0);
  Actor_FaceDirection(ACTOR_MIA, FACING_SOUTH, 20);
  FieldCamera_MoveTo(PIXELS(326), -1, PIXELS(384), 1);
  FieldCamera_WaitForArrival();
  Event_PlaySound(SE_GLITTER);
  if (*paletteFrame != 24) {
    do {
      wait(1);
    } while (*paletteFrame != 24);
  }
  Task_Remove(KolimaForestTree_UpdateHealingPalette);
  wait(10);
  // Flash the two saved palettes before settling on the healthy forest.
  flash = 0;
  do {
    KolimaPalette_Restore(0);
    PaletteFade_StartInterpolation(6);
    wait(6);
    KolimaPalette_Restore(1);
    PaletteFade_StartInterpolation(6);
    flash = (flash + 1);
    wait(6);
  } while ((u32)flash <= 3);
  KolimaPalette_Restore(0);
  PaletteFade_StartInterpolation(40);
  wait(80);
  FieldCamera_MoveTo(PIXELS(328), PIXELS(8), PIXELS(212), 1);
  FieldCamera_WaitForArrival();
  Event_Wait(40);
  KolimaForestTree_PlayGesture(ACTOR_INVISIBLE_TRET, FACE_GESTURE_OPEN_EYES);
  Event_Wait(40);
  Event_PlaySound(MUS_COLOSSO_INTRO);
  // Tret and Laurel discover that their powers have returned.
  Event_SetMessageId(MsgKolimaFeelGreatPowerSpreadingThrough);
  Event_ShowMessage(ACTOR_TRET, 0);
  Actor_StartRepeatedMotion(ACTOR_PARTY_LEADER, 2);
  Actor_StartRepeatedMotion(ACTOR_GARET, 2);
  Actor_StartRepeatedMotion(ACTOR_MIA, 2);
  Actor_RunRepeatedMotion(ACTOR_IVAN, 2);
  KolimaForestTree_FaceParty(FACING_NORTH, 20);
  KolimaForestTree_PlayGesture(ACTOR_INVISIBLE_TRET, FACE_GESTURE_LOOK_LEFT);
  Event_Wait(20);
  KolimaForestTree_PlayGesture(ACTOR_INVISIBLE_TRET, FACE_GESTURE_LOOK_RIGHT);
  Event_Wait(40);
  KolimaForestTree_PlayGesture(ACTOR_INVISIBLE_TRET, FACE_GESTURE_OPEN_EYES);
  Event_Wait(20);
  Event_ShowMessage(ACTOR_TRET, 0);
  Actor_ShowEmote(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 0);
  Actor_ShowEmote(ACTOR_GARET, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 0);
  Actor_ShowEmote(ACTOR_MIA, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 0);
  Actor_ShowEmote(ACTOR_IVAN, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 40);
  FieldCamera_MoveTo(PIXELS(234), 0, PIXELS(232), 1);
  FieldCamera_WaitForArrival();
  Event_Wait(40);
  KolimaForestTree_PlayGesture(ACTOR_INVISIBLE_LAUREL, FACE_GESTURE_OPEN_EYES);
  Event_Wait(40);
  KolimaForestTree_PlayGesture(ACTOR_INVISIBLE_LAUREL, FACE_GESTURE_LOOK_RIGHT);
  Event_Wait(20);
  FieldEvent_ShowMessageThenWait(SPEAKER_WINDOW_ABOVE | ACTOR_LAUREL, 0, 20);
  KolimaForestTree_PlayGesture(ACTOR_INVISIBLE_LAUREL, FACE_GESTURE_LOOK_LEFT);
  Event_Wait(10);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_SOUTHWEST, 0);
  Actor_FaceDirection(ACTOR_GARET, FACING_SOUTHWEST, 0);
  Actor_FaceDirection(ACTOR_IVAN, FACING_SOUTHWEST, 0);
  Actor_FaceDirection(ACTOR_MIA, FACING_SOUTHWEST, 20);
  KolimaForestTree_PlayGesture(ACTOR_INVISIBLE_LAUREL, FACE_GESTURE_LOOK_RIGHT);
  Event_Wait(20);
  KolimaForestTree_PlayGesture(ACTOR_INVISIBLE_LAUREL, FACE_GESTURE_LOOK_LEFT);
  Event_Wait(20);
  KolimaForestTree_PlayGesture(ACTOR_INVISIBLE_LAUREL, FACE_GESTURE_LOOK_RIGHT);
  FieldEvent_ShowMessageThenWait(SPEAKER_WINDOW_ABOVE | ACTOR_LAUREL, 0, 10);
  KolimaForestTree_PlayGesture(ACTOR_INVISIBLE_TRET, FACE_GESTURE_CLOSE_EYES);
  Event_Wait(20);
  Event_ShowMessage(SPEAKER_WINDOW_BELOW | ACTOR_TRET, 0);
  KolimaForestTree_PlayGesture(ACTOR_INVISIBLE_TRET, FACE_GESTURE_OPEN_EYES);
  Event_Wait(20);
  Event_OpenMessage(SPEAKER_WINDOW_BELOW | ACTOR_TRET, 0);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 0);
  Actor_FaceDirection(ACTOR_GARET, FACING_NORTHEAST, 0);
  Actor_FaceDirection(ACTOR_IVAN, FACING_NORTHWEST, 0);
  Actor_FaceDirection(ACTOR_MIA, FACING_NORTH, 0);
  if (Event_AskYesNoWithActorAnimation(0, 0) == 0) {
    Event_ShowMessage(SPEAKER_WINDOW_ABOVE | ACTOR_LAUREL, 0);
    Event_ShowMessage(SPEAKER_WINDOW_BELOW | ACTOR_TRET, 0);
  } else {
    gFieldWork->message += 2;
    Actor_ShowEmote(ACTOR_MIA, EMOTE_IN_FRONT | EMOTE_ANGER_VEIN, 0);
    Actor_ShowEmote(ACTOR_GARET, EMOTE_IN_FRONT | EMOTE_ANGER_VEIN, 0);
    Actor_ShowEmote(ACTOR_IVAN, EMOTE_IN_FRONT | EMOTE_ANGER_VEIN, 40);
    Import_ActorSetAnimation(ACTOR_GARET, ANIM_SHAKE_HEAD);
    Event_ShowMessage(ACTOR_GARET, 0);
    if (includeMia != 0) {
      Actor_RunRepeatedMotion(ACTOR_MIA, 2);
      Event_ShowMessage(ACTOR_MIA, 0);
    } else {
      gFieldWork->message += 1;
    }
    Actor_SetAnimationAndWait(ACTOR_IVAN, ANIM_NOD);
    Event_ShowMessage(ACTOR_IVAN, 0);
    Event_ShowMessage(SPEAKER_WINDOW_ABOVE | ACTOR_LAUREL, 0);
    Event_ShowMessage(SPEAKER_WINDOW_BELOW | ACTOR_TRET, 0);
  }
  KolimaForestTree_NodTogether();
  FieldCamera_MoveTo(PIXELS(328), PIXELS(8), PIXELS(212), 1);
  FieldCamera_WaitForArrival();
  Event_Wait(20);
  KolimaForestTree_PlayGesture(ACTOR_INVISIBLE_TRET, FACE_GESTURE_CLOSE_EYES);
  Event_Wait(20);
  KolimaPalette_Restore(0);
  PaletteFade_StartInterpolation(1);
  wait(1);
  PaletteFade_ApplyTarget(PALETTE_TINT_BLEND | RGB(24, 16, 24), PALETTES_BG);
  PaletteFade_StartInterpolation(40);
  Event_Wait(60);
  gKolimaForestTreeSparkFrame = 0;
  // Tret sends out the power that frees Kolima's villagers.
  gKolimaForestTreeSparkOrigin[0] = PIXELS(328);
  gKolimaForestTreeSparkOrigin[1] = PIXELS(48);
  spawnOrbitSparks = KolimaForestTree_SpawnOrbitSparks;
  gKolimaForestTreeSparkOrigin[2] = PIXELS(205);
  Task_AddFrame(spawnOrbitSparks, TASK_PASS_FRAME);
  Event_Wait(100);
  Task_Remove(spawnOrbitSparks);
  PaletteFade_ApplyTarget(RGB(31, 31, 31), PALETTES_ALL);
  PaletteFade_StartInterpolation(60);
  Event_Wait(100);
  KolimaPalette_Restore(0);
  PaletteFade_StartInterpolation(20);
  Event_Wait(40);
  KolimaForestTree_PlayGesture(ACTOR_INVISIBLE_TRET, FACE_GESTURE_OPEN_EYES);
  Event_Wait(10);
  Event_SetMessageId(MsgKolimaShouldDoPeopleKolimaCursed);
  Event_ShowMessage(SPEAKER_WINDOW_BELOW | ACTOR_TRET, 0);
  KolimaForestTree_NodTogether();
  FieldCamera_MoveTo(PIXELS(234), 0, PIXELS(232), 1);
  FieldCamera_WaitForArrival();
  Event_Wait(20);
  Event_ShowMessage(SPEAKER_WINDOW_ABOVE | ACTOR_LAUREL, 0);
  FieldEvent_ShowMessageThenWait(SPEAKER_WINDOW_BELOW | ACTOR_TRET, 0, 10);
  Actor_RunRepeatedMotion(ACTOR_GARET, 2);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_SOUTHWEST, 0);
  Actor_FaceDirection(ACTOR_GARET, FACING_NORTHEAST, 10);
  Event_OpenMessage(ACTOR_GARET, 0);
  if (Event_AskYesNoWithActorAnimation(0, 0) == 0) {
    Actor_ShowEmote(ACTOR_GARET, EMOTE_IN_FRONT | EMOTE_WINCE, 40);
  } else {
    Actor_SetAnimationAndWait(ACTOR_GARET, ANIM_SHAKE_HEAD);
    gFieldWork->message += 1;
  }
  Event_ShowMessage(ACTOR_GARET, 0);
  KolimaForestTree_PlayGesture(ACTOR_INVISIBLE_TRET, FACE_GESTURE_BLINK);
  Event_Wait(20);
  // Explain the curse and how the falling Psynergy Stones changed the forest.
  Event_SetMessageId(MsgKolimaWasIndeedAngryPeopleHad);
  Event_ShowMessage(SPEAKER_WINDOW_BELOW | ACTOR_TRET, 0);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 0);
  Actor_FaceDirection(ACTOR_GARET, FACING_NORTH, 0);
  Event_ShowMessage(SPEAKER_WINDOW_BELOW | ACTOR_TRET, 0);
  KolimaForestTree_NodTogether();
  KolimaForestTree_PlayGesture(ACTOR_INVISIBLE_TRET, FACE_GESTURE_BLINK);
  Event_Wait(20);
  FieldEvent_ShowMessageThenWait(SPEAKER_WINDOW_BELOW | ACTOR_TRET, 0, 20);
  KolimaForestTree_PlayGesture(ACTOR_INVISIBLE_LAUREL, FACE_GESTURE_CLOSE_EYES);
  FieldEvent_ShowMessageThenWait(SPEAKER_WINDOW_ABOVE | ACTOR_LAUREL, 0, 20);
  KolimaForestTree_PlayGesture(ACTOR_INVISIBLE_LAUREL, FACE_GESTURE_LOOK_RIGHT);
  Event_Wait(40);
  KolimaForestTree_PlayGesture(ACTOR_INVISIBLE_LAUREL, FACE_GESTURE_OPEN_EYES);
  Event_Wait(20);
  FieldEvent_ShowMessageThenWait(SPEAKER_WINDOW_ABOVE | ACTOR_LAUREL, 0, 20);
  KolimaForestTree_PlayGesture(ACTOR_INVISIBLE_TRET, FACE_GESTURE_LOOK_LEFT);
  Event_Wait(20);
  Event_ShowMessage(SPEAKER_WINDOW_BELOW | ACTOR_TRET, 0);
  Actor_ShowEmote(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | EMOTE_WINCE, 0);
  Actor_ShowEmote(ACTOR_GARET, EMOTE_IN_FRONT | EMOTE_WINCE, 0);
  Actor_ShowEmote(ACTOR_MIA, EMOTE_IN_FRONT | EMOTE_WINCE, 0);
  Actor_ShowEmote(ACTOR_IVAN, EMOTE_IN_FRONT | EMOTE_WINCE, 80);
  KolimaForestTree_PlayGesture(ACTOR_INVISIBLE_LAUREL,
                               FACE_GESTURE_HALF_CLOSE_EYES);
  Event_Wait(60);
  KolimaForestTree_PlayGesture(ACTOR_INVISIBLE_LAUREL, FACE_GESTURE_LOOK_RIGHT);
  Event_Wait(20);
  FieldEvent_ShowMessageThenWait(SPEAKER_WINDOW_ABOVE | ACTOR_LAUREL, 0, 20);
  KolimaForestTree_PlayGesture(ACTOR_INVISIBLE_TRET,
                               FACE_GESTURE_HALF_CLOSE_EYES);
  Event_Wait(40);
  KolimaForestTree_PlayGesture(ACTOR_INVISIBLE_TRET, FACE_GESTURE_LOOK_LEFT);
  Event_Wait(20);
  FieldEvent_ShowMessageThenWait(SPEAKER_WINDOW_ABOVE | ACTOR_TRET, 0, 20);
  Actor_RunRepeatedMotion(ACTOR_GARET, 2);
  Actor_FaceDirection(ACTOR_GARET, FACING_WEST, 10);
  Event_ShowMessage(ACTOR_GARET, 0);
  Actor_FaceDirection(ACTOR_IVAN, FACING_WEST, 20);
  Event_ShowMessage(SPEAKER_WINDOW_BELOW | ACTOR_IVAN, 0);
  KolimaForestTree_PlayGesture(ACTOR_INVISIBLE_LAUREL, FACE_GESTURE_BLINK);
  Event_Wait(20);
  FieldEvent_ShowMessageThenWait(SPEAKER_WINDOW_ABOVE | ACTOR_LAUREL, 0, 20);
  Actor_FaceDirection(ACTOR_GARET, FACING_NORTHEAST, 0);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_SOUTHWEST, 10);
  FieldEvent_ShowMessageThenWait(ACTOR_GARET, 0, 20);
  KolimaForestTree_PlayGesture(ACTOR_INVISIBLE_TRET, FACE_GESTURE_OPEN_EYES);
  FieldEvent_ShowMessageThenWait(SPEAKER_WINDOW_BELOW | ACTOR_TRET, 0, 10);
  KolimaForestTree_PlayGesture(ACTOR_INVISIBLE_TRET, FACE_GESTURE_LOOK_LEFT);
  Event_Wait(20);
  KolimaForestTree_PlayGesture(ACTOR_INVISIBLE_LAUREL, FACE_GESTURE_LOOK_RIGHT);
  Event_Wait(40);
  KolimaForestTree_PlayGesture(ACTOR_INVISIBLE_LAUREL, FACE_GESTURE_CLOSE_EYES);
  Event_Wait(20);
  KolimaPalette_Restore(0);
  PaletteFade_StartInterpolation(1);
  wait(1);
  PaletteFade_ApplyTarget(PALETTE_TINT_BLEND | RGB(24, 16, 24), PALETTES_BG);
  PaletteFade_StartInterpolation(40);
  Event_Wait(60);
  gKolimaForestTreeSparkFrame = 0;
  // Laurel looks beyond the river and warns of the forest to the south.
  gKolimaForestTreeSparkOrigin[0] = PIXELS(136);
  gKolimaForestTreeSparkOrigin[1] = PIXELS(20);
  spawnOrbitSparks = KolimaForestTree_SpawnOrbitSparks;
  gKolimaForestTreeSparkOrigin[2] = PIXELS(258);
  Task_AddFrame(spawnOrbitSparks, TASK_PASS_FRAME);
  Event_Wait(100);
  KolimaForestTree_FaceParty(FACING_SOUTHWEST, 40);
  Actor_StartRepeatedMotion(ACTOR_IVAN, 1);
  Actor_ShowEmote(ACTOR_IVAN, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 20);
  FieldEvent_ShowMessageThenWait(SPEAKER_WINDOW_BELOW | ACTOR_IVAN, 0, 10);
  Actor_RunRepeatedMotion(ACTOR_PARTY_LEADER, 2);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_SOUTHEAST, 10);
  Actor_SetAnimationAndWait(ACTOR_PARTY_LEADER, ANIM_NOD);
  KolimaForestTree_PlayGesture(ACTOR_INVISIBLE_TRET, FACE_GESTURE_BLINK);
  Event_Wait(20);
  Event_ShowMessage(SPEAKER_WINDOW_BELOW | ACTOR_TRET, 0);
  Actor_ShowEmote(ACTOR_IVAN, EMOTE_IN_FRONT | EMOTE_QUESTION, 60);
  Actor_FaceDirection(ACTOR_IVAN, FACING_NORTH, 10);
  FieldEvent_ShowMessageThenWait(SPEAKER_WINDOW_BELOW | ACTOR_IVAN, 0, 10);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 0);
  Actor_FaceDirection(ACTOR_MIA, FACING_NORTH, 0);
  Actor_FaceDirection(ACTOR_GARET, FACING_NORTH, 20);
  FieldEvent_ShowMessageThenWait(SPEAKER_WINDOW_BELOW | ACTOR_TRET, 0, 10);
  KolimaForestTree_NodTogether();
  Event_Wait(10);
  KolimaForestTree_FaceParty(FACING_SOUTHWEST, 120);
  Task_Remove(spawnOrbitSparks);
  Event_Wait(60);
  KolimaPalette_Restore(0);
  PaletteFade_StartInterpolation(40);
  KolimaForestTree_PlayGesture(ACTOR_INVISIBLE_TRET, FACE_GESTURE_LOOK_LEFT);
  Event_Wait(20);
  FieldEvent_ShowMessageThenWait(SPEAKER_WINDOW_BELOW | ACTOR_TRET, 0, 20);
  KolimaForestTree_PlayGesture(ACTOR_INVISIBLE_LAUREL, FACE_GESTURE_LOOK_RIGHT);
  Event_ShowMessage(SPEAKER_WINDOW_ABOVE | ACTOR_LAUREL, 0);
  Event_ShowMessage(SPEAKER_WINDOW_BELOW | ACTOR_TRET, 0);
  KolimaForestTree_PlayGesture(ACTOR_INVISIBLE_LAUREL, FACE_GESTURE_BLINK);
  Event_Wait(20);
  FieldEvent_ShowMessageThenWait(SPEAKER_WINDOW_ABOVE | ACTOR_LAUREL, 0, 10);
  Actor_StartRepeatedMotion(ACTOR_PARTY_LEADER, 2);
  Actor_StartRepeatedMotion(ACTOR_GARET, 2);
  Actor_StartRepeatedMotion(ACTOR_MIA, 2);
  Actor_RunRepeatedMotion(ACTOR_IVAN, 2);
  KolimaForestTree_PlayGesture(ACTOR_INVISIBLE_TRET, FACE_GESTURE_OPEN_EYES);
  Event_OpenMessage(SPEAKER_WINDOW_BELOW | ACTOR_TRET, 0);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 0);
  Actor_FaceDirection(ACTOR_GARET, FACING_NORTHEAST, 0);
  Actor_FaceDirection(ACTOR_MIA, FACING_NORTH, 0);
  Actor_FaceDirection(ACTOR_IVAN, FACING_NORTHWEST, 0);
  if (Event_AskYesNoWithActorAnimation(0, 0) == 1) {
    gFieldWork->message += 1;
  }
  Event_Wait(10);
  KolimaForestTree_PlayGesture(ACTOR_INVISIBLE_TRET, FACE_GESTURE_LOOK_LEFT);
  Event_Wait(20);
  KolimaForestTree_PlayGesture(ACTOR_INVISIBLE_LAUREL, FACE_GESTURE_LOOK_RIGHT);
  Event_Wait(40);
  KolimaForestTree_PlayGesture(ACTOR_INVISIBLE_TRET, FACE_GESTURE_OPEN_EYES);
  Event_Wait(20);
  FieldEvent_ShowMessageThenWait(SPEAKER_WINDOW_BELOW | ACTOR_TRET, 0, 10);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_SOUTH, 0);
  Actor_FaceDirection(ACTOR_GARET, FACING_EAST, 0);
  Actor_FaceDirection(ACTOR_MIA, FACING_NORTH, 0);
  Actor_FaceDirection(ACTOR_IVAN, FACING_WEST, 10);
  KolimaForestTree_NodTogether();
  Event_PlaySound(SOUND_FADE_OUT_BGM);
  // Rejoin the party and exchange the used water for its empty bottle.
  finalActions = gKolimaForestTree_RejoinPartyActions;
  Actor_SetScript(ACTOR_GARET, finalActions);
  if (includeMia != 0) {
    Actor_SetScript(ACTOR_MIA, finalActions);
  }
  Actor_RunScriptAndWait(ACTOR_IVAN, finalActions);
  KolimaForestTree_PlayGesture(ACTOR_INVISIBLE_TRET, FACE_GESTURE_BLINK);
  KolimaForestTree_PlayGesture(ACTOR_INVISIBLE_TRET, FACE_GESTURE_BLINK);
  Event_Wait(20);
  Event_SetMessageId(MsgKolimaOweGreatDebtHaveSaved);
  Event_ShowMessage(SPEAKER_WINDOW_BELOW | ACTOR_TRET, 0);
  KolimaForestTree_PlayGesture(ACTOR_INVISIBLE_LAUREL, FACE_GESTURE_BLINK);
  KolimaForestTree_PlayGesture(ACTOR_INVISIBLE_LAUREL, FACE_GESTURE_BLINK);
  Event_Wait(20);
  FieldEvent_ShowMessageThenWait(SPEAKER_WINDOW_ABOVE | ACTOR_LAUREL, 0, 10);
  Actor_SetAnimationAndWait(ACTOR_PARTY_LEADER, ANIM_NOD);
  GameFlag_Set(FLAG_KOLIMA_CURSE_LIFTED);
  Event_PlaySound(MUS_TOWN);
  KolimaForestTree_ReplaceInventoryItem(ITEM_HERMES_WATER, ITEM_EMPTY_BOTTLE);
}

/// Sets up Tret, Laurel and the forest's colours on entry. Tret, Laurel and the
/// wrathful side stand 28 pixels up, nothing passes through them, Retreat is
/// set to the forest's way in, and the forest takes its cursed colours until
/// the curse is lifted.
s32 KolimaForestTree_Initialize(void) {
  struct Actor *tretBlank;
  struct Actor *tretsWrathfulSide;
  struct Actor *laurelBlank;

  tretBlank = Actor_GetById(ACTOR_INVISIBLE_TRET);
  tretsWrathfulSide = Actor_GetById(ACTOR_TRETS_WRATHFUL_SIDE);
  laurelBlank = Actor_GetById(ACTOR_INVISIBLE_LAUREL);
  wait(1);
  Actor_ConfigureSpriteColor(ACTOR_TRETS_WRATHFUL_SIDE, SPRITE_COLOR_HIDDEN);
  gFieldWork->transition =
      SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_INTERLACED_WIPE);
  gMainRoutineWork.retreatScene = (s32)&SceneId_KolimaForestEntrance;
  gMainRoutineWork.retreatEntrance = KOLIMA_FOREST_ENTRANCE_FROM_WORLD_MAP;
  if (!GameFlag_Get(FLAG_KOLIMA_CURSE_LIFTED))
    KolimaPalette_Curse(3);
  Actor_GetById(ACTOR_TRET)->radius = 6;
  Actor_GetById(ACTOR_LAUREL)->radius = 6;
  Actor_GetById(ACTOR_TRET_STAND_IN)->radius = 6;
  Actor_GetById(ACTOR_LAUREL_STAND_IN)->radius = 6;
  Actor_SetSpriteFlags(Actor_GetById(ACTOR_TRETS_WRATHFUL_SIDE), 0);
  Actor_SetSpriteFlags(Actor_GetById(ACTOR_INVISIBLE_TRET), 0);
  Actor_SetSpriteFlags(Actor_GetById(ACTOR_INVISIBLE_LAUREL), 0);
  Actor_SetSpriteFlags(Actor_GetById(ACTOR_TRET), 0);
  Actor_SetSpriteFlags(Actor_GetById(ACTOR_LAUREL), 0);
  Actor_SetSpritePriority(ACTOR_TRET, 2);
  Actor_SetSpritePriority(ACTOR_TRETS_WRATHFUL_SIDE, 2);
  Actor_SetSpritePriority(ACTOR_LAUREL, 2);
  tretBlank->motionFlags = 0;
  tretBlank->y.fixed = PIXELS(28);
  laurelBlank->motionFlags = 0;
  laurelBlank->y.fixed = PIXELS(28);
  tretsWrathfulSide->motionFlags = 0;
  tretsWrathfulSide->y.fixed = PIXELS(28);
  Import_ActorSetAnimation(ACTOR_LAUREL, LAUREL_FACE_CLOSED);
  Import_ActorSetAnimation(ACTOR_TRET, TRET_FACE_CLOSED);
  Actor_GetById(ACTOR_TRET)->collisionFlags |= ACTOR_NOT_TARGETED;
  Actor_GetById(ACTOR_LAUREL)->collisionFlags |= ACTOR_NOT_TARGETED;
  Actor_GetById(ACTOR_INVISIBLE_TRET)->collisionFlags |= ACTOR_NOT_TARGETED;
  Actor_GetById(ACTOR_INVISIBLE_LAUREL)->collisionFlags |= ACTOR_NOT_TARGETED;
  Actor_GetById(ACTOR_TRETS_WRATHFUL_SIDE)->collisionFlags |=
      ACTOR_NOT_TARGETED;
  return 0;
}

#include "shared/kolima_grow.h"

/// Spawns the growing sparks that repel the party leader.
static void KolimaForestTree_SpawnRejectionSpark(void) {
  struct Actor *effect;
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
    Sound_Play(SE_SPARKLE_FALL);
  effect = FieldActor_Create(SPRITE_GOLDEN_RINGS, spawnX, spawnY, spawnZ);
  if (effect == 0)
    return;
  sprite = effect->sprite;
  sprite->flags = phase;
  effectFlags = 0xff & ~ACTOR_PRIORITY_AUTOMATIC;
  effectFlags &= effect->priorityFlags;
  effect->priorityFlags = effectFlags;
  sprite->image.priority = 1;
  effect->scaleX = Q16_ONE / 10;
  effect->speedLimit = PIXELS(4);
  effect->acceleration = PIXELS(4);
  effect->motionFlags = phase;
  Actor_SelectAnimation(effect, 2);
  FieldActor_SetMoveTarget(effect, targetX, 0, targetZ);
  Actor_StartScript(effect, gKolimaForestTreeEffectScript);
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

/// Alternates two faces twice, ending on the first.
static __inline__ void
KolimaForestTree_AlternateFaces(s32 actorId, s32 firstFace, s32 secondFace) {
  Import_ActorSetAnimation(actorId, firstFace);
  wait(6);
  Import_ActorSetAnimation(actorId, secondFace);
  wait(6);
  Import_ActorSetAnimation(actorId, firstFace);
  wait(6);
  Import_ActorSetAnimation(actorId, secondFace);
  wait(6);
  Import_ActorSetAnimation(actorId, firstFace);
}

/// Plays one of Tret's or Laurel's face gestures, chosen by the blank
/// actor at their spot.
static void KolimaForestTree_PlayGesture(s32 blankActorId, s32 gesture) {
  if (blankActorId == ACTOR_INVISIBLE_TRET) {
    switch (gesture) {
    case FACE_GESTURE_CLOSE_EYES:
      Import_ActorSetAnimation(ACTOR_TRET, TRET_FACE_OPEN);
      wait(6);
      Import_ActorSetAnimation(ACTOR_TRET, TRET_FACE_CLOSED);
      break;
    case FACE_GESTURE_OPEN_EYES:
      Import_ActorSetAnimation(ACTOR_TRET, TRET_FACE_OPEN);
      break;
    case FACE_GESTURE_LOOK_LEFT:
      Import_ActorSetAnimation(ACTOR_TRET, TRET_FACE_OPEN);
      wait(6);
      Import_ActorSetAnimation(ACTOR_TRET, TRET_FACE_LOOK_LEFT);
      break;
    case FACE_GESTURE_LOOK_RIGHT:
      Import_ActorSetAnimation(ACTOR_TRET, TRET_FACE_OPEN);
      wait(6);
      Import_ActorSetAnimation(ACTOR_TRET, TRET_FACE_LOOK_RIGHT);
      break;
    case FACE_GESTURE_BLINK:
      KolimaForestTree_AlternateFaces(ACTOR_TRET, TRET_FACE_OPEN,
                                      TRET_FACE_CLOSED);
      break;
    case FACE_GESTURE_HALF_CLOSE_EYES:
      Import_ActorSetAnimation(ACTOR_TRET, TRET_FACE_OPEN);
      wait(6);
      Import_ActorSetAnimation(ACTOR_TRET, TRET_FACE_HALF_CLOSED);
      break;
    case FACE_GESTURE_WRATHFUL_CLOSE_EYES:
      Import_ActorSetAnimation(ACTOR_TRET, TRET_FACE_WRATHFUL_OPEN);
      wait(6);
      Import_ActorSetAnimation(ACTOR_TRET, TRET_FACE_WRATHFUL_CLOSED);
      break;
    case FACE_GESTURE_WRATHFUL_LOOK_RIGHT:
      Import_ActorSetAnimation(ACTOR_TRET, TRET_FACE_WRATHFUL_OPEN);
      wait(6);
      Import_ActorSetAnimation(ACTOR_TRET, TRET_FACE_WRATHFUL_LOOK_RIGHT);
      break;
    case FACE_GESTURE_WRATHFUL_LOOK_LEFT:
      Import_ActorSetAnimation(ACTOR_TRET, TRET_FACE_WRATHFUL_OPEN);
      wait(6);
      Import_ActorSetAnimation(ACTOR_TRET, TRET_FACE_WRATHFUL_LOOK_LEFT);
      break;
    case FACE_GESTURE_WRATHFUL_BLINK:
      KolimaForestTree_AlternateFaces(ACTOR_TRET, TRET_FACE_WRATHFUL_OPEN,
                                      TRET_FACE_WRATHFUL_CLOSED);
      break;
    case FACE_GESTURE_WRATHFUL_OPEN_EYES:
    case FACE_GESTURE_WRATHFUL_STARE:
      Import_ActorSetAnimation(ACTOR_TRET, TRET_FACE_WRATHFUL_OPEN);
      break;
    }
  } else {
    switch (gesture) {
    case FACE_GESTURE_CLOSE_EYES:
      Import_ActorSetAnimation(ACTOR_LAUREL, LAUREL_FACE_OPEN);
      wait(6);
      Import_ActorSetAnimation(ACTOR_LAUREL, LAUREL_FACE_CLOSED);
      break;
    case FACE_GESTURE_OPEN_EYES:
      Import_ActorSetAnimation(ACTOR_LAUREL, LAUREL_FACE_OPEN);
      break;
    case FACE_GESTURE_LOOK_LEFT:
      Import_ActorSetAnimation(ACTOR_LAUREL, LAUREL_FACE_OPEN);
      wait(6);
      Import_ActorSetAnimation(ACTOR_LAUREL, LAUREL_FACE_LOOK_LEFT);
      break;
    case FACE_GESTURE_LOOK_RIGHT:
      Import_ActorSetAnimation(ACTOR_LAUREL, LAUREL_FACE_OPEN);
      wait(6);
      Import_ActorSetAnimation(ACTOR_LAUREL, LAUREL_FACE_LOOK_RIGHT);
      break;
    case FACE_GESTURE_BLINK:
      KolimaForestTree_AlternateFaces(ACTOR_LAUREL, LAUREL_FACE_OPEN,
                                      LAUREL_FACE_CLOSED);
      break;
    case FACE_GESTURE_HALF_CLOSE_EYES:
      Import_ActorSetAnimation(ACTOR_LAUREL, LAUREL_FACE_OPEN);
      wait(6);
      Import_ActorSetAnimation(ACTOR_LAUREL, LAUREL_FACE_HALF_CLOSED);
      break;
    }
  }
  wait(12);
}

/// Expands a spark along its spiral for 120 frames, then releases its cache
/// slot.
static void KolimaForestTree_UpdateOrbitSpark(struct Actor *actor) {
  struct Actor *spark = actor;
  s32 position[3];
  s32 age = spark->timer;
  s32 heading;

  if (age <= 119) {
    SET_VECTOR(position, spark->targetX, spark->targetY, spark->targetZ);
    heading = (s16)spark->userValue;
    Vector_AddPolarOffset(age * Q16_ONE, age * 768 + heading, position);
    spark->x.fixed = position[0];
    spark->y.fixed = position[1];
    spark->z.fixed = position[2];
    spark->scaleX += Q16_ONE / 200;
    spark->scaleY += Q16_ONE / 200;
    spark->timer++;
  } else {
    VramCache_FreeEntry(spark->sprite->slot);
    Actor_Release(spark);
  }
}

/// Emits six-spark rings at ten-frame intervals during each burst.
static void KolimaForestTree_SpawnOrbitSparks(void) {
  struct Actor *spark;
  struct Sprite *previousSprite;
  u32 sparkIndex;

  previousSprite = NULL;
  switch (gKolimaForestTreeSparkFrame) {
  case 0:
  case 10:
  case 20:
  case 30:
  case 40:
    Sound_Play(SE_SHIMMER);
    for (sparkIndex = 0; sparkIndex <= 5; sparkIndex++) {
      spark = FieldActor_Create(SPRITE_SPARKLE, gKolimaForestTreeSparkOrigin[0],
                                gKolimaForestTreeSparkOrigin[1],
                                gKolimaForestTreeSparkOrigin[2]);
      if (spark != 0) {
        previousSprite =
            Sprite_ShareVramCacheEntry(spark->sprite, previousSprite);
        spark->motionFlags = 0;
        spark->sprite->image.priority = 1;
        Actor_SetSpriteFlags(spark, 0);
        Actor_SelectAnimation(spark, 1);
        spark->timer = 0;
        spark->userValue = (sparkIndex * 60 * Q16_ONE) / 360;
        spark->targetX = gKolimaForestTreeSparkOrigin[0];
        spark->targetY = gKolimaForestTreeSparkOrigin[1];
        spark->targetZ = gKolimaForestTreeSparkOrigin[2];
        spark->speedLimit = PIXELS(8) / 5;
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

#include "shared/kolima_palette.h"

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

struct SceneEntrance gKolimaForestTreeEntrances[] = {
    {ENTRANCE_DEFAULT, CONDITION_ALWAYS, 209, 0, 279, FACING_SOUTH, 0, 0, 0,
     512, 448, 0},
    {KOLIMA_FOREST_TREE_FROM_KOLIMA_FOREST_FLOODGATE, CONDITION_ALWAYS, 136, 0,
     424, FACING_NORTH, 0, 0, 0, 512, 448, 0},
    {KOLIMA_FOREST_TREE_FROM_TREE_ROOMS, CONDITION_ALWAYS, 328, 16, 200,
     FACING_SOUTH, 0, 0, 0, 512, 448, 0},
    {SCENE_TABLE_END},
};

// clang-format off
struct SceneExitRegion gKolimaForestTreeRegions[] = {
    {320, 88, 176, 336, 104, 192, -1, KOLIMA_FOREST_TREE_TO_TREE_ROOMS},
    {-1, 0, 0, 0, 0, 0, 0, 0},
};

u32 gKolimaForestTreeExits[] = {
    SCENE_EXITS(KolimaForestTree),
    SCENE_EXIT(KOLIMA_FOREST_TREE_TO_KOLIMA_FOREST_FLOODGATE, KolimaForestFloodgate, KOLIMA_FOREST_FLOODGATE_FROM_KOLIMA_FOREST_TREE),
    SCENE_EXIT(KOLIMA_FOREST_TREE_TO_TREE_ROOMS, TreeRooms, TREE_ROOMS_FROM_KOLIMA_FOREST_TREE),
    SCENE_EXITS_END,
};

struct ActorPlacement gKolimaForestTreeActorPlacements[] = {
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
    { ACTOR_GARET, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        0, 0, 0, FACING_EAST, TALK_FACING_KEEP, 0 },
    { ACTOR_IVAN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        0, 0, 0, FACING_EAST, TALK_FACING_KEEP, 0 },
    { ACTOR_MIA, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        0, 0, 0, FACING_EAST, TALK_FACING_KEEP, 0 },
    { SCENE_TABLE_END },
};

struct Trigger gKolimaForestTreeTriggers[] = {
    { TRIGGER_EXIT, 1, CONDITION_ALWAYS, KOLIMA_FOREST_TREE_TO_KOLIMA_FOREST_FLOODGATE },
    { USE_ITEM_ON_ACTOR(ITEM_HERMES_WATER), ACTOR_TRET_STAND_IN, FLAG_KOLIMA_CURSE_LIFTED, EVENT(KolimaForestTree_UseHermesWater) },
    { TRIGGER_TALK, ACTOR_LAUREL_STAND_IN, CONDITION_ALWAYS, EVENT(KolimaForestTree_TalkToLaurel) },
    { TRIGGER_TALK, ACTOR_TRET_STAND_IN, CONDITION_ALWAYS, EVENT(KolimaForestTree_TalkToTret) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_LAUREL_STAND_IN, CONDITION_ALWAYS, EVENT(KolimaForestTree_ReadLaurelThoughts) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), TRIGGER_SUBJECT_OWN_EFFECT | ACTOR_TRET_STAND_IN, FLAG_TRET_DEFEATED, EVENT(KolimaForestTree_TalkToTret) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_TRET_STAND_IN, CONDITION_ALWAYS, EVENT(KolimaForestTree_ReadTretThoughts) },
    { TRIGGERS_END },
};
// clang-format on

s32 gKolimaForestTreeEffectScript[] = {
    SC_ADD(SW_SCALE_X, Q16_ONE / 10),
    SC_CALL(FieldEffect_StretchUntilArrived),
    SC_LOOP(SC_FOREVER, SC_TOP),
};
