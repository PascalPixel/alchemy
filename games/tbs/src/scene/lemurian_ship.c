/// @file
/// The gangway out over the sea behind the shack in the north-west of
/// Lalivero, where the Lemurian ship lies sunk (SceneId_LemurianShip, the area
/// name Lemurian ship), and the game's last scene.
///
/// The party comes in from the shack (timber_rooms_bc.c) once Iodem has given
/// it the Black Orb and the guard lets it by. A long wooden gangway with rails
/// runs north over the sea, and the ship lies beneath the water beside it, seen
/// through the waves. Walking back off the gangway's south end goes back into
/// the shack.
///
/// Using the Black Orb at the gangway's north end raises the ship
/// (LemurianShip_RaiseTheShip): the sea glows, spray rises, the screen shakes,
/// and the ship comes up out of the water beside the gangway and rides the
/// swell with a glow along its hull. The party then talks the journey over,
/// with three yes-or-no questions, climbs aboard and sails away west as black
/// bars close in from above and below (LemurianShip_SetSail). The game then
/// fades to the staff credits and the clear screen (TITLE_FROM_LEMURIAN_SHIP),
/// keeping the music (FLAG_KEEP_MUSIC_ONCE).
///
/// The sunk ship is the map's two upper layers drawn 90 pixels low, under the
/// sea's layer, which ripples by a line scroll (Ripple_Start). Reveal stops the
/// ripple while it shows. Arriving by LEMURIAN_SHIP_WITH_BLACK_ORB, which only
/// the debug scene selector reaches, first gives the leader the Black Orb.

#include "ability_ids.h"
#include "flags.h"
#include "item_ids.h"
#include "types.h"
#include "battle/battle.h"
#include "debug/debug.h"
#include "field/actor_script.h"
#include "field/event.h"
#include "field/event_commands.h"
#include "field/event_inline.h"
#include "field/field.h"
#include "field/line_scroll.h"
#include "field/line_scroll_wave_sine.h"
#include "field/map_grid_scroll.h"
#include "field/scene_tables.h"
#include "field/script_operands.h"
#include "game/game.h"
#include "graphics/graphics.h"
#include "menu/menu.h"
#include "scene/lemurian_ship_ripple.h"
#include "shop/shop.h"
#include "sound/sound.h"
#include "sound/sound_driver_symbols.h"
#include "system/dma.h"
#include "system/point.h"
#include "system/heap_slots.h"
#include "system/io_write.h"
#include "system/iwram_call.h"
#include "system/main_routine_state.h"
#include "system/system.h"
#include "system/vram_cache.h"
#include "text/msg_ids.h"

s32 Scene_Initialize(void);
extern u16 gShipGlowPalettes[];
extern s32 gSprayFallScript[];
extern s16 gRippleSine[LINE_SCROLL_WAVE_ENTRIES];
extern struct SceneEntrance gLemurianShipEntrances[];
extern u32 gLemurianShipExits[];
extern struct ActorPlacement gLemurianShipActorPlacements[];
extern struct Trigger gLemurianShipTriggers[];
extern s32 gOrbGlowActive;
extern s32 gOrbGlowPhase;
extern s32 gOrbGlowStrength;
extern s32 gOrbGlowFadeFrames;
extern s32 gSprayActive;
extern s32 gSwellActive;
extern s32 gSwellPhase;
extern s32 gShipHullY;
extern s32 gShipDeckY;
extern s32 gLeaderDeckY;
extern s32 gGaretDeckY;
extern s32 gMiaDeckY;
extern s32 gIvanDeckY;
extern u16 gShipGlowStep;
extern s32 gLeaderBoardsShip[];
extern s32 gGaretBoardsShip[];
extern s32 gIvanBoardsShip[];
extern s32 gMiaBoardsShip[];

/// DMA 0's control, DMA0CNT_H, as a halfword after REG_DMA0's source,
/// destination and count (GBATEK "GBA DMA Transfers").
#define DMA_CNT_H_INDEX 5

/// The ship's glow (ShipGlow_CyclePalette): SHIP_GLOW_COLORS of
/// gShipGlowPalettes at a time, from a start that moves one colour every
/// SHIP_GLOW_STEP_FRAMES frames, through as many starts as colours shown.
#define SHIP_GLOW_COLORS 6
#define SHIP_GLOW_STEP_FRAMES 6

/// The curtains (Curtain_Close): three rows of CURTAIN_ROW_PARTS sprites, 32
/// by 16, one row sliding down from above the screen and two up from below,
/// CURTAIN_START_GAP pixels in all, until they rest at the top and at
/// CURTAIN_BOTTOM_Y.
#define CURTAIN_PART_WIDTH 32
#define CURTAIN_PART_HEIGHT 16
#define CURTAIN_ROW_PARTS 8
#define CURTAIN_BOTTOM_Y 136
#define CURTAIN_START_GAP 48

/// The frames from the curtains starting to close to the ship sailing west: 30
/// from patch 3, 80 before.
#if TBS_PATCH_3
#define LEMURIAN_SHIP_DEPARTURE_DELAY_FRAMES 30
#else
#define LEMURIAN_SHIP_DEPARTURE_DELAY_FRAMES 80
#endif

static void Ripple_Stop(void);
static void Curtain_Start(void);

enum {
  /// The gangway, in two pieces (SPRITE_GANGWAY): its northern half.
  ACTOR_GANGWAY_NORTH = ACTOR_FIRST_PLACED,
  ACTOR_GANGWAY_SOUTH
};

/// The deck height of a party member who has not yet boarded.
/// The swell (Ship_UpdateSwell) moves only those who have.
#define SHIP_NOT_ON_DECK (-toFIXED(1))

/// The sprite priority of an actor's picture and of its shadow.
#define SET_IMAGE_PRIORITY(actor, level)                                       \
  ((void)(actor->sprite->image.priority = (level)))
#define SET_SHADOW_PRIORITY(actor, level)                                      \
  ((void)(actor->sprite->shadow.priority = (level)))

const struct SceneEntrance *Scene_GetEntrances(void) {
  return gLemurianShipEntrances;
}

const struct SceneExitRegion *Scene_GetExitRegions(void) { return NULL; }

const u32 *Scene_GetExits(void) { return gLemurianShipExits; }

/// The gangway's two pieces.
/// The party's own actors fill the first three rows.
const struct ActorPlacement *Scene_GetActorPlacements(void) {
  return gLemurianShipActorPlacements;
}

/// Walking off the south end of the gangway to the Lemurian ship (cells (18,
/// 20) to (20, 20)) goes back into the shack in Lalivero. The backdrop colour
/// is cleared to black for the way out.
static void LemurianShip_LeaveTheShip(void) {
  Import_EventSetExitRequest(gFieldState->stepTrigger);
  Io_Write16(0, &PLTT_BACKDROP);
}

/// Casting Reveal on the gangway to the Lemurian ship stops the sea rippling
/// while the grey circle of Reveal shows; there is nothing hidden here.
static void Ripple_StopForReveal(void) { Ripple_Stop(); }

/// The sea around the gangway to the Lemurian ship ripples, from the moment the
/// party arrives and again as Reveal ends. The ripple sways the sea's layer
/// across and up and down at once (LineScroll_InitWave).
static void Ripple_Start(void) {
  LineScroll_InitWave(0, FIXED_ONE * 4, FIXED_ONE, FIXED_ONE / 8, FIXED_ONE,
                      FIXED_ONE / 2, FIXED_ONE / 4);
}

/// The way back, Reveal, and the Black Orb at the gangway's north end.
const struct Trigger *Scene_GetTriggers(void) { return gLemurianShipTriggers; }

/// Runs the glow along the risen ship's hull, a colour at a time.
static void ShipGlow_CyclePalette(void) {
  u16 paletteIndex;

  paletteIndex = Math_DivideUnsigned(gShipGlowStep, SHIP_GLOW_STEP_FRAMES);
  Dma_Set(&gShipGlowPalettes[paletteIndex], (void *)&BG_PLTT_COLOR(7, 4),
          DMA_COPY_HALFWORDS(SHIP_GLOW_COLORS), REG_DMA3);
  gShipGlowStep++;
  if (gShipGlowStep > SHIP_GLOW_COLORS * SHIP_GLOW_STEP_FRAMES - 1)
    gShipGlowStep = 0;
}

/// The water drop's burst into a ring of spray (water_drop.png), beyond enum
/// ActorAnimation.
enum { ANIM_DROP_BURST = 1 };

/// A drop of spray rises and wobbles for its timer, then bursts into a ring
/// of spray and is released 20 frames later by gSprayFallScript.
static void LemurianShip_SprayUpdate(struct Actor *drop) {
  if (drop->timer != 0) {
    s32 drift;

    drop->timer--;
    drift = Random_Next() - Random_Next();
    drop->x.fixed += drift;
    drop->y.fixed += toFIXED(4) / 5;
  } else if ((s16)drop->userValue != 0) {
    drop->userValue = 0;
    Actor_SelectAnimation(drop, ANIM_DROP_BURST);
    drop->wait = 20;
    Actor_StartScript(drop, gSprayFallScript);
  }
}

/// Every 40 frames, the Black Orb's glow fades a step, down to 4.
static void OrbGlow_Fade(void) {
  s32 frame = gOrbGlowFadeFrames + 1;

  gOrbGlowFadeFrames = frame;
  if (frame == 40) {
    if (gOrbGlowStrength > 4) {
      gOrbGlowStrength -= 1;
      gOrbGlowFadeFrames = 0;
    }
  }
}

/// Keeps the raised ship alive with a glow, a swell and spray.
static void Ship_UpdateSwell(void) {
  struct MapGridScrollState *grid;
  struct Actor *actor;
  FIXED swell;
  s32 x, z;

  grid = gMapGridState[0];
  if (gOrbGlowActive != 0) {
    /* FAKEMATCH(temporary): blend, stored through its address; plainly,
       blend = .... */
    u16 blend;

    swell = Iwram_MulFX(Trig_Sin(gOrbGlowPhase << 9), 3);
    *&blend = gOrbGlowStrength + ((swell + 8) << 8);
    Io_Write16(blend, &REG_BLDALPHA);
    gOrbGlowPhase++;
  }
  if (gSwellActive != 0) {
    swell = toFIXED(Iwram_MulFX(Trig_Sin(gSwellPhase << 9), 2));
    grid->layers[MAPGRID_LAYER_MIDDLE].offsetY = gShipHullY + swell;
    grid->layers[MAPGRID_LAYER_OVERHEAD].offsetY = gShipDeckY + swell;
    if (gLeaderDeckY != SHIP_NOT_ON_DECK) {
      actor = Actor_Get(ACTOR_PARTY_LEADER);
      actor->y.fixed = gLeaderDeckY + swell;
      actor->groundHeight = gLeaderDeckY + swell;
      actor->motionFlags = 0;
    }
    if (gGaretDeckY != SHIP_NOT_ON_DECK) {
      actor = Actor_Get(ACTOR_GARET);
      actor->y.fixed = gGaretDeckY + swell;
      actor->groundHeight = gLeaderDeckY + swell;
      actor->motionFlags = 0;
    }
    if (gMiaDeckY != SHIP_NOT_ON_DECK) {
      actor = Actor_Get(ACTOR_MIA);
      actor->y.fixed = gMiaDeckY + swell;
      actor->groundHeight = gLeaderDeckY + swell;
      actor->motionFlags = 0;
    }
    if (gIvanDeckY != SHIP_NOT_ON_DECK) {
      actor = Actor_Get(ACTOR_IVAN);
      actor->y.fixed = gIvanDeckY + swell;
      actor->groundHeight = gLeaderDeckY + swell;
      actor->motionFlags = 0;
    }
    gSwellPhase++;
  }
  if (gSprayActive != 0 && (gFrameCount & 1) != 0) {
    struct Point sprayPosition;

    x = grid->view[0] & FIELD_PIXEL_MASK;
    z = grid->view[1] & FIELD_PIXEL_MASK;
    x += Random_Next() * DISPLAY_WIDTH;
    sprayPosition.x = x;
    sprayPosition.y = 0;
    z += Random_Next() * DISPLAY_HEIGHT;
    z += toFIXED(30);
    sprayPosition.z = z;
    actor = FieldActor_Create(SPRITE_WATER_DROP, sprayPosition.x,
                              sprayPosition.y, sprayPosition.z);
    if (actor != NULL) {
      actor->update = LemurianShip_SprayUpdate;
      actor->timer = 60;
      actor->userValue = 1;
      actor->motionFlags = 0;
      actor->priorityFlags = ACTOR_PRIORITY_UNDERFOOT;
      actor->sprite->image.priority = 2;
      Actor_SetSpriteFlags(actor, ACTOR_BLEND_NORMAL);
      Actor_SelectAnimation(actor, ANIM_STILL);
    }
  }
}

/// Remembers the leader's height on deck, so that from then on he bobs with
/// the ship.
static s32 LemurianShip_LeaderStepOnDeck(struct Actor *interpreter) {
  gLeaderDeckY = Actor_Get(ACTOR_PARTY_LEADER)->y.fixed;
  return 0;
}

/// Remembers Garet's height on deck, so that from then on he bobs with the
/// ship.
static s32 LemurianShip_GaretStepOnDeck(struct Actor *interpreter) {
  gGaretDeckY = Actor_Get(ACTOR_GARET)->y.fixed;
  return 0;
}

/// Remembers Mia's height on deck, so that from then on she bobs with the
/// ship.
static s32 LemurianShip_MiaStepOnDeck(struct Actor *interpreter) {
  gMiaDeckY = Actor_Get(ACTOR_MIA)->y.fixed;
  return 0;
}

/// Remembers Ivan's height on deck, so that from then on he bobs with the
/// ship.
static s32 LemurianShip_IvanStepOnDeck(struct Actor *interpreter) {
  gIvanDeckY = Actor_Get(ACTOR_IVAN)->y.fixed;
  return 0;
}

/// On the gangway beside the risen Lemurian ship, Garet, Mia and Ivan step out
/// and talk the journey over with Isaac, who answers three questions: whether
/// the journey ends when they find Jenna, whether to find her before Lemuria,
/// and whether they will be all right on the rough sea. Each answer picks one
/// of two replies. Then all four hop up onto the deck, Mia last after a look
/// back at the shore, and the ship sails away west as black bars close in
/// from above and below and the ending theme plays, until the screen fades to
/// the staff credits.
static void LemurianShip_SetSail(void) {
  struct MapGridScrollState *grid;
  struct Actor *ship;
  struct FieldState *state;

  Event_Start();
  FieldCamera_MoveTo(CAMERA_KEEP, CAMERA_KEEP, CAMERA_KEEP, 0);
  MapGrid_CopyCellAttributes(18, 0, 3, 1, 18, 12);
  // Garet, Mia and Ivan step out around the leader. Garet: "It looks like
  // it's finally time to leave Gondowan." Ivan and Mia say how much has
  // happened.
  Event_SetMessageId(MsgFieldLooksLikeFinally);
  FieldActor_LaunchFromLeader(ACTOR_GARET, -10, 16, FACING_NORTH);
  FieldActor_LaunchFromLeader(ACTOR_MIA, 0, 24, FACING_NORTH);
  Actor_LaunchFromLeader(ACTOR_IVAN, 10, 16, FACING_NORTH);
  Actor_WaitForMoveById(ACTOR_GARET);
  Event_Wait(50);
  Actor_RunRepeatedMotion(ACTOR_GARET, 2);
  Event_Wait(20);
  // "It looks like it's finally time to leave Gondowan."
  Event_ShowMessage(ACTOR_GARET, 0);
  Event_Wait(10);
  Actor_SetAnimationAndWait(ACTOR_IVAN, ANIM_NOD);
  Event_Wait(20);
  // "It's so strange... We've been traveling for so long, and yet..."
  Event_ShowMessage(ACTOR_IVAN, 0);
  Event_Wait(10);
  Actor_FaceDirection(ACTOR_MIA, FACING_SOUTH, 0);
  Event_Wait(40);
  // "Now that I think about it, so many things have happened."
  Event_ShowMessage(ACTOR_MIA, 0);
  Event_Wait(10);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_SOUTH, 0);
  Actor_FaceDirection(ACTOR_GARET, FACING_SOUTH, 0);
  Actor_FaceDirection(ACTOR_IVAN, FACING_SOUTH, 0);
  Event_Wait(60);
  Actor_SetAnimation(ACTOR_PARTY_LEADER, ANIM_NOD);
  Actor_SetAnimation(ACTOR_GARET, ANIM_NOD);
  Actor_SetAnimationAndWait(ACTOR_IVAN, ANIM_NOD);
  Event_Wait(60);
  // The first question, Garet's: will the journey be over once they find
  // Jenna? The answer picks which of his two replies he says.
  Actor_RunRepeatedMotion(ACTOR_GARET, 2);
  Event_Wait(30);
  Actor_FaceDirection(ACTOR_GARET, FACING_NORTHEAST, 0);
  Event_Wait(20);
  // "But our journey will be over as soon as we find Jenna, right?"
  Event_OpenMessage(ACTOR_GARET, 0);
  if (Event_AskYesNoWithActorAnimation(ACTOR_PARTY_LEADER, 0) == 0) {
    Event_Wait(30);
    Actor_ShowEmote(ACTOR_GARET, EMOTE_IN_FRONT | EMOTE_WINCE, 50);
    // "The end of one journey, but the beginning of another? No way!"
    Event_ShowMessage(ACTOR_GARET, 0);
    gFieldState->message++;
  } else {
    Event_Wait(30);
    Actor_ShowEmote(ACTOR_GARET, EMOTE_IN_FRONT | EMOTE_WINCE, 50);
    gFieldState->message++;
    // "What did you say? Our next journey? What journey is that?"
    Event_ShowMessage(ACTOR_GARET, 0);
  }
  Event_Wait(10);
  // Ivan and Mia remind Garet of their promise to Babi, and Lemuria comes up.
  Actor_FaceDirection(ACTOR_IVAN, FACING_WEST, 0);
  Event_Wait(30);
  Actor_ShowEmote(ACTOR_IVAN, EMOTE_IN_FRONT | EMOTE_QUESTION, 50);
  // "You didn't forget, did you, Garet?"
  Event_ShowMessage(ACTOR_IVAN, 0);
  Event_Wait(10);
  Actor_FaceDirection(ACTOR_GARET, FACING_EAST, 0);
  Event_Wait(30);
  Actor_ShowEmote(ACTOR_GARET, EMOTE_IN_FRONT | EMOTE_QUESTION, 60);
  Actor_RunRepeatedMotion(ACTOR_MIA, 2);
  Event_Wait(30);
  Actor_FaceDirection(ACTOR_MIA, FACING_NORTHWEST, 0);
  Event_Wait(30);
  // "Our promise to Babi."
  Event_ShowMessage(ACTOR_MIA, 0);
  Event_Wait(10);
  Actor_ShowEmote(ACTOR_GARET, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 50);
  // "Oh... Lemuria!"
  Event_ShowMessage(ACTOR_GARET, 0);
  Event_Wait(10);
  Actor_RunRepeatedMotion(ACTOR_IVAN, 2);
  Event_Wait(20);
  // "I wonder what kind of place Lemuria is."
  Event_ShowMessage(ACTOR_IVAN, 0);
  Event_Wait(10);
  Actor_FaceDirection(ACTOR_MIA, FACING_NORTH, 0);
  Event_Wait(20);
  Actor_SetAnimationAndWait(ACTOR_MIA, ANIM_SHAKE_HEAD);
  Event_Wait(20);
  // The second question, Mia's: must they find Jenna before going to
  // Lemuria? Garet answers either way.
  // "We have to find Jenna before going to Lemuria, don't we?"
  Event_OpenMessage(ACTOR_MIA, 0);
  if (Event_AskYesNoWithActorAnimation(ACTOR_PARTY_LEADER, 0) == 0) {
    Event_Wait(20);
    Actor_RunRepeatedMotion(ACTOR_GARET, 2);
    Event_Wait(20);
    Actor_FaceDirection(ACTOR_GARET, FACING_NORTHEAST, 0);
    Event_Wait(20);
    // "Yeah. That's important."
    Event_ShowMessage(ACTOR_GARET, 0);
    gFieldState->message++;
  } else {
    Event_Wait(20);
    Actor_RunRepeatedMotion(ACTOR_GARET, 2);
    Event_Wait(20);
    Actor_FaceDirection(ACTOR_GARET, FACING_NORTHEAST, 0);
    Event_Wait(20);
    gFieldState->message++;
    // "Huh? Jenna's going to be very sad when I tell her what you said!"
    Event_ShowMessage(ACTOR_GARET, 0);
  }
  Event_Wait(10);
  // Ivan: their first time on the ocean; Garet: the ocean Kraden wanted
  // him to see.
  Actor_ShowEmote(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | EMOTE_WINCE, 50);
  Event_Wait(10);
  Actor_FaceDirection(ACTOR_IVAN, FACING_NORTHWEST, 0);
  Event_Wait(20);
  Actor_SetAnimationAndWait(ACTOR_IVAN, ANIM_NOD);
  Event_Wait(20);
  // "At any rate, this will be our first time on the ocean."
  Event_ShowMessage(ACTOR_IVAN, 0);
  Event_Wait(20);
  // "So this is the ocean Kraden wanted me to see."
  Event_ShowMessage(ACTOR_GARET, 0);
  Event_Wait(20);
  Actor_SetAnimationAndWait(ACTOR_PARTY_LEADER, ANIM_NOD);
  Event_Wait(30);
  Event_Wait(10);
  Actor_SetAnimationAndWait(ACTOR_MIA, ANIM_NOD);
  Event_Wait(20);
  // The third question, Mia's: the seas are rough; will they be all right?
  // "The seas are very rough. Do you think we will be okay?"
  Event_OpenMessage(ACTOR_MIA, 0);
  Actor_FaceDirection(ACTOR_GARET, FACING_NORTHEAST, 0);
  Actor_FaceDirection(ACTOR_MIA, FACING_NORTH, 0);
  Actor_FaceDirection(ACTOR_IVAN, FACING_NORTHWEST, 0);
  Event_Wait(20);
  if (Event_AskYesNoWithActorAnimation(ACTOR_PARTY_LEADER, 0) == 0) {
    Event_Wait(30);
    Actor_SetAnimationAndWait(ACTOR_MIA, ANIM_NOD);
    Event_Wait(20);
    // "This is a Lemurian ship. We'll be fine."
    Event_ShowMessage(ACTOR_MIA, 0);
    gFieldState->message++;
  } else {
    Event_Wait(30);
    Actor_SetAnimationAndWait(ACTOR_MIA, ANIM_NOD);
    Event_Wait(20);
    gFieldState->message++;
    // "I guess it's hard to say. Still, we must go, just the same..."
    Event_ShowMessage(ACTOR_MIA, 0);
  }
  Event_Wait(10);
  // Garet: "Well... Shall we, then?" Ivan: "To sea..." Mia: Jenna is waiting
  // for them. The departure theme starts.
  Actor_FaceDirection(ACTOR_GARET, FACING_NORTH, 0);
  Event_Wait(30);
  // "Well... Shall we, then?"
  Event_ShowMessage(ACTOR_GARET, 0);
  Event_Wait(10);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 0);
  Event_Wait(30);
  Event_Wait(10);
  Actor_FaceDirection(ACTOR_IVAN, FACING_NORTH, 0);
  Event_Wait(30);
  Actor_SetAnimationAndWait(ACTOR_IVAN, ANIM_NOD);
  Event_Wait(20);
  // "To sea..."
  Event_ShowMessage(ACTOR_IVAN, 0);
  Event_Wait(10);
  Actor_SetAnimationAndWait(ACTOR_MIA, ANIM_NOD);
  Event_Wait(20);
  // "Jenna is waiting for us!"
  Event_ShowMessage(ACTOR_MIA, 0);
  Sound_Play(SOUND_FADE_OUT_BGM);
  Event_Wait(10);
  Actor_SetAnimation(ACTOR_PARTY_LEADER, ANIM_NOD);
  Actor_SetAnimation(ACTOR_GARET, ANIM_NOD);
  Actor_SetAnimationAndWait(ACTOR_IVAN, ANIM_NOD);
  Event_Wait(40);
  // Everyone boards by their own scripts, hopping up onto the deck
  // (SE_HIGH_JUMP each); the camera follows up to the ship.
  Actor_SetSpeed(ACTOR_PARTY_LEADER, 1.2);
  Actor_SetSpeed(ACTOR_GARET, 1.2);
  Actor_SetSpeed(ACTOR_IVAN, 1.2);
  Actor_SetSpeed(ACTOR_MIA, 1.2);
  Actor_SetScript(ACTOR_PARTY_LEADER, gLeaderBoardsShip);
  Event_Wait(50);
  FieldCamera_MoveTo(264, CAMERA_KEEP, 200, 1);
  Actor_SetScript(ACTOR_GARET, gGaretBoardsShip);
  Event_Wait(50);
  Actor_SetScript(ACTOR_IVAN, gIvanBoardsShip);
  Actor_WaitForScriptEndById(ACTOR_IVAN);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_SOUTH, 0);
  Actor_FaceDirection(ACTOR_GARET, FACING_SOUTH, 0);
  Actor_FaceDirection(ACTOR_IVAN, FACING_SOUTH, 0);
  // Mia, last, looks back at the shore before she boards.
  Actor_WalkByAndWait(ACTOR_MIA, 0, -32);
  Event_Wait(30);
  Actor_FaceDirection(ACTOR_MIA, FACING_SOUTH, 0);
  Event_Wait(60);
  Actor_FaceDirection(ACTOR_MIA, FACING_WEST, 0);
  Event_Wait(20);
  Actor_SetScript(ACTOR_MIA, gMiaBoardsShip);
  Actor_WaitForScriptEndById(ACTOR_MIA);
  Actor_FaceDirection(ACTOR_MIA, FACING_NORTH, 0);
  Event_Wait(20);
  // On deck, all four turn west together and nod.
  FieldCamera_MoveTo(216, CAMERA_KEEP, 168, 1);
  FieldCamera_WaitForMove();
  Event_Wait(20);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_WEST, 0);
  Actor_FaceDirection(ACTOR_GARET, FACING_WEST, 0);
  Actor_FaceDirection(ACTOR_IVAN, FACING_WEST, 0);
  Actor_FaceDirection(ACTOR_MIA, FACING_WEST, 0);
  Event_Wait(40);
  Actor_SetAnimation(ACTOR_PARTY_LEADER, ANIM_NOD);
  Actor_SetAnimation(ACTOR_GARET, ANIM_NOD);
  Actor_SetAnimation(ACTOR_MIA, ANIM_NOD);
  Actor_SetAnimationAndWait(ACTOR_IVAN, ANIM_NOD);
  Event_Wait(30);
  // The ship sails west and the curtains close: the sea's layer and the
  // gangway's north piece slide away east past it, ever faster, and the camera
  // pans along the deck to the stern.
  Sound_Play(MUS_THE_BROKEN_SEAL_ENDING);
  Sound_SetBgmTempo(SOUND_TEMPO_SCALE_NORMAL * 15 / 16);
  Curtain_Start();
  Event_Wait(LEMURIAN_SHIP_DEPARTURE_DELAY_FRAMES);
  grid = gMapGridState[0];
  ship = Actor_Get(ACTOR_GANGWAY_NORTH);
  ship->acceleration = toFIXED(0.002);
  ship->speedLimit = toFIXED(2);
  grid->layers[MAPGRID_LAYER_GROUND].speedX = -toFIXED(3) / 16;
  Actor_WalkByAndWait(ACTOR_GANGWAY_NORTH, 60, 0);
  grid->layers[MAPGRID_LAYER_GROUND].speedX = -toFIXED(3) / 8;
  Actor_WalkByAndWait(ACTOR_GANGWAY_NORTH, 60, 0);
  Event_Wait(80);
  Event_Wait(100);
  FieldCamera_SetSpeedAndAcceleration(2.2, 0.01);
  FieldCamera_MoveTo(808, CAMERA_KEEP, 168, 1);
  Event_Wait(300);
  state = gFieldState;
  state->transition = SCENE_TRANSITION(TRANSITION_BACKDROP_FADE, 0);
  Io_Write16(0, &PLTT_BACKDROP);
  state->transitionFrames = 96;
  Event_FinishScreenTransition();
  Event_WaitTransitionFrames();
  Event_Wait(30);
  GameFlag_Set(FLAG_KEEP_MUSIC_ONCE);
  Event_SetDestination(SCENE_TITLE, TITLE_FROM_LEMURIAN_SHIP);
}

/// The water's ripple, this scene's view of the line scroll's state
/// (gLineScroll, struct LineScrollState).
///
/// It holds two pages of per-line scroll offsets, swapped each frame and
/// built from a sine table. Here a line is one word, BG3's x and y, where
/// LineScrollState has three.
struct RippleScrollState {
  u16 pages[2][960]; ///< per page: 160 lines of BG3 x, y; the rest unused
  u8 page;           ///< the page the HBlank DMA shows; the other is built
  u8 mode;
  u16 phase;          ///< frames built so far, added to BG3's y
  u8 unused[4];       ///< zeroed with the block; nothing reads or writes it
  FIXED frequency[2]; ///< x, y: 16.16 sine entries per frame and per row
  FIXED step[2];      ///< x, y: 16.16 sine entries a line
  FIXED amplitude[2]; ///< x, y: 16.16 scale on the sine, then divided by 256
};

/// One line's ripple offset.
static inline s32 LemurianShip_WaveOffset(s32 angle, FIXED amplitude,
                                          u16 base) {
  return (u16)(Iwram_MulFX(gRippleSine[FIXEDtoINT(angle) &
                                       (LINE_SCROLL_WAVE_ENTRIES - 1)],
                           amplitude) /
               LINE_SCROLL_WAVE_SCALE) +
         base;
}

/// Using the Black Orb at the north end of the gangway (cells (18, 12) to
/// (20, 14)) raises the Lemurian ship. Isaac walks to the end and faces the
/// sea; the water glows brighter and brighter, flashing three times, spray
/// rises all over it and the screen shakes harder, and the ship slowly comes
/// up out of the sea beside the gangway. A white flash, the puzzle-solved
/// jingle, and the departure follows (LemurianShip_SetSail).
static void LemurianShip_RaiseTheShip(void) {
  struct MapGridScrollState *grid;
  s32 i;
  s32 flash;
  s32 white;
  s32 dark;
  FIXED risen;
  // Each control is stored through its address, so it stays in the frame.
  /* FAKEMATCH(temporary): cnt, each register's new value stored through its
     address; plainly, cnt = .... */
  u16 cnt;

  grid = gMapGridState[0];

  Event_Start();
  Field_EndBusyAndReveal();
  // The leader steps to the water's edge; the backdrop brightens 16 steps.
  Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 312, 232);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 0);
  Event_Wait(40);
  Sound_Play(SE_SPIN);
  for (i = 0; i <= RGB_LEVEL_MAX / 2; i++) {
    Io_Write16(RGB(0, i, i * 2), &PLTT_BACKDROP);
    Event_Wait(10);
  }
  Io_Write16(RGB(0, 16, 31), &PLTT_BACKDROP);
  {
    u16 bright = BLEND_ALPHA(16, 16);

    for (flash = 0; flash < 3; flash++) {
      Sound_Play(SE_POWER_UP);
      REG_BLDALPHA = bright;
      Event_Wait(3);
      Io_Write16(BLEND_ALPHA(16, 8), &REG_BLDALPHA);
      Event_Wait(65);
    }
  }
  // The orb's glow and the spray begin, and the water lightens in three
  // steps.
  gOrbGlowActive = 1;
  gOrbGlowPhase = 0;
  Task_AddFrame(Ship_UpdateSwell, TASK_PASS_FRAME);
  gSprayActive = 1;
  Event_Wait(20);
  Sound_Play(SE_RUSH);
  MapGrid_SetShake(1, 1, 1);
  Event_Wait(60);
  gSprayActive = 1;
  MapGrid_SetShake(2, 2, 1);
  Event_Wait(60);
  MapGrid_SetShake(3, 3, 1);
  gOrbGlowFadeFrames = 0;
  Task_AddFrame(OrbGlow_Fade, TASK_PASS_FRAME);
  // The ship rises 90 pixels, 0.2 pixels a frame.
  risen = 0;
  do {
    grid->layers[MAPGRID_LAYER_MIDDLE].offsetY += toFIXED(1) / 5;
    grid->layers[MAPGRID_LAYER_OVERHEAD].offsetY += toFIXED(1) / 5;
    risen += toFIXED(1) / 5;
    wait(1);
  } while (risen < toFIXED(90));
  Task_Remove(OrbGlow_Fade);
  gSprayActive = 0;
  *&cnt = (REG_BG3CNT & ~BGCNT_PRIORITY(3)) | BGCNT_PRIORITY(3);
  REG_BG3CNT = cnt;
  *&cnt = (REG_BG2CNT & ~BGCNT_PRIORITY(3)) | BGCNT_PRIORITY(3);
  REG_BG2CNT = cnt;
  *&cnt = (REG_BG1CNT & ~BGCNT_PRIORITY(3)) | BGCNT_PRIORITY(2);
  REG_BG1CNT = cnt;
  gOrbGlowActive = 0;
  Sound_Play(SE_STOP_ACTION_SE);
  wait(1);
  // A flash of white; the swell starts as it clears.
  Sound_Play(SE_CRASH);
  Io_Write16(BLDCNT_TGT1_ALL | BLDCNT_EFFECT_LIGHTEN, &REG_BLDCNT);
  for (white = 0; white <= BLEND_FADE_MAX; white++) {
    REG_BLDY = white;
    Event_Wait(1);
  }
  Event_Wait(40);
  MapGrid_SetShake(SHAKE_KEEP, SHAKE_KEEP, 0.9);
  gShipHullY = grid->layers[MAPGRID_LAYER_MIDDLE].offsetY;
  gShipDeckY = grid->layers[MAPGRID_LAYER_OVERHEAD].offsetY;
  gSwellActive = 1;
  for (dark = BLEND_FADE_MAX; dark >= 0; dark--) {
    REG_BLDY = dark;
    Event_Wait(8);
  }
  Task_AddFrame(ShipGlow_CyclePalette, TASK_PASS_FRAME);
  Sound_Play(MUS_PUZZLE_SOLVED);
  Sound_WaitForJingle();
  Event_Wait(20);
  Event_End();
  LemurianShip_SetSail();
}

/// Lays the Lemurian ship under the sea: its two layers are lowered 90 pixels
/// and drawn behind the sea's, which is blended over them, while the gangway
/// and the party are drawn above. The sea starts rippling.
s32 Scene_Initialize(void) {
  struct MapGridScrollState *grid;
  // Each control is stored through its address, so it stays in the frame.
  /* FAKEMATCH(temporary): cnt, each register's new value stored through its
     address; plainly, cnt = .... */
  u16 cnt;

  grid = gMapGridState[0];
  if (gMainRoutineState.entrance == LEMURIAN_SHIP_WITH_BLACK_ORB) {
    Inventory_AddItem(ACTOR_PARTY_LEADER, ITEM_BLACK_ORB);
  }
  gFieldState->transition = SCENE_TRANSITION(TRANSITION_BACKDROP_FADE, 0);
  Actor_Get(ACTOR_GANGWAY_NORTH)->collisionFlags = 0;
  Actor_Get(ACTOR_GANGWAY_NORTH)->priorityFlags = ACTOR_PRIORITY_UNDERFOOT;
  Actor_Get(ACTOR_GANGWAY_SOUTH)->collisionFlags = 0;
  Actor_Get(ACTOR_GANGWAY_SOUTH)->priorityFlags = ACTOR_PRIORITY_UNDERFOOT;
  {
    struct Actor *actor = Actor_Get(ACTOR_GANGWAY_NORTH);

    SET_IMAGE_PRIORITY(actor, 1);
  }
  {
    struct Actor *actor = Actor_Get(ACTOR_GANGWAY_SOUTH);

    SET_IMAGE_PRIORITY(actor, 1);
  }
  {
    struct Actor *actor = Actor_Get(ACTOR_PARTY_LEADER);

    SET_IMAGE_PRIORITY(actor, 1);
    SET_SHADOW_PRIORITY(actor, 1);
  }
  {
    struct Actor *actor = Actor_Get(ACTOR_GARET);

    SET_IMAGE_PRIORITY(actor, 1);
    SET_SHADOW_PRIORITY(actor, 1);
  }
  {
    struct Actor *actor = Actor_Get(ACTOR_IVAN);

    SET_IMAGE_PRIORITY(actor, 1);
    SET_SHADOW_PRIORITY(actor, 1);
  }
  {
    struct Actor *actor = Actor_Get(ACTOR_MIA);

    SET_IMAGE_PRIORITY(actor, 1);
    SET_SHADOW_PRIORITY(actor, 1);
  }
  *&cnt = (REG_BG3CNT & ~BGCNT_PRIORITY(3)) | BGCNT_PRIORITY(2);
  REG_BG3CNT = cnt;
  *&cnt = (REG_BG2CNT & ~BGCNT_PRIORITY(3)) | BGCNT_PRIORITY(3);
  REG_BG2CNT = cnt;
  *&cnt = (REG_BG1CNT & ~BGCNT_PRIORITY(3)) | BGCNT_PRIORITY(3);
  REG_BG1CNT = cnt;
  Io_Write16(BLDCNT_TGT1_BG3 | BLDCNT_EFFECT_BLEND | BLDCNT_TGT2_BG1 |
                 BLDCNT_TGT2_BG2 | BLDCNT_TGT2_BD,
             &REG_BLDCNT);
  Io_Write16(BLEND_ALPHA(16, 8), &REG_BLDALPHA);
  {
    struct MapGridLayerScroll *layer = grid->layers + 1;

    layer->offsetY -= toFIXED(90);
    layer = grid->layers + 2;
    layer->offsetY -= toFIXED(90);
  }
  MapGrid_CenterOnFollowedPoint();
  Ripple_Start();
  return 0;
}

/// Arms DMA 0 to copy the shown ripple page into background 3's scroll
/// registers.
void LineScroll_ArmDma(void) {
  struct RippleScrollState *state;
  u32 *source;
  volatile u16 *dma0;
  volatile u32 *destination;

  state = (struct RippleScrollState *)gLineScroll;
  source = (u32 *)state->pages[state->page];

  dma0 = REG_DMA0;
  destination = (volatile u32 *)&REG_BG3HOFS;
  dma0[DMA_CNT_H_INDEX] &= ~DMA_START_TIMING_DRQ_REPEAT;
  dma0[DMA_CNT_H_INDEX] &= ~DMA_ENABLE;
  dma0[DMA_CNT_H_INDEX];

  *destination = *source++;
  Dma_Set(source, (void *)destination, DMA_HBLANK_WORD_PER_LINE,
          (volatile u32 *)dma0);
}

/// Builds the other ripple page and swaps it in.
void LineScroll_BuildPage(void) {
  struct RippleScrollState *state;
  u16 *line;
  s32 angle;
  u16 base;
  s32 amplitude;
  s32 step;
  u16 y;
  s32 i;

  state = (struct RippleScrollState *)gLineScroll;
  y = gBgScroll[3].y;
  line = state->pages[state->page ^ 1];
  step = state->step[0];
  angle = (state->phase + y) * state->frequency[0];
  amplitude = state->amplitude[0];
  base = gBgScroll[3].x;
  for (i = 0; i != DISPLAY_HEIGHT; i++) {
    *line = LemurianShip_WaveOffset(angle, amplitude, base);
    angle += step;
    line += 2;
  }
  line = state->pages[state->page ^ 1] + 1;
  step = state->step[1];
  angle = (state->phase + y) * state->frequency[1];
  amplitude = state->amplitude[1];
  for (i = 0; i != DISPLAY_HEIGHT; i++) {
    *line = LemurianShip_WaveOffset(angle, amplitude, y);
    angle += step;
    line += 2;
  }
  state->phase++;
  state->page ^= 1;
}

#include "shared/lemurian_ship_ripple.c"

/// Takes the ripple's tasks off and stops DMA 0.
static void Ripple_Stop(void) {
  volatile u16 *reg;

  Task_Remove(LineScroll_ArmDma);
  Task_Remove(LineScroll_BuildPage);
  reg = REG_DMA0;
  reg[DMA_CNT_H_INDEX] = ~DMA_START_TIMING_DRQ_REPEAT & reg[DMA_CNT_H_INDEX];
  reg[DMA_CNT_H_INDEX] = ~DMA_ENABLE & reg[DMA_CNT_H_INDEX];
  reg[DMA_CNT_H_INDEX];
}

/// The curtains' sprites, which close in from above and below as the ship
/// sails.
static struct MenuSprite sCurtainSprites[3 * CURTAIN_ROW_PARTS];
static s16 sCurtainGap;
static s16 sCurtainVramSlot;

/// Writes one curtain sprite: its queue link, its attributes 0 and 1, and its
/// attribute 2.
#ifdef UBFIX
#define CURTAIN_PART(attributes, tile)                                         \
  (part->next = NULL, part->oam.word.attr01 = (attributes),                    \
   part->oam.word.attr23 = (tile), part++)
#else
// UB: the parts are written through a word cursor, the link among them.
#define CURTAIN_PART(attributes, tile)                                         \
  (*word++ = 0, *word++ = (attributes), *word++ = (tile))
#endif

/// Each frame from Curtain_Start to the end of the game: closes the curtains a
/// pixel more, until they rest, and queues their 24 sprites.
static void Curtain_Close(void) {
#ifdef UBFIX
  struct MenuSprite *part;
#else
  u32 *word;
#endif
  u32 tile;
  s32 gap;
  s32 edge;
  u32 i;

  tile = gVramCacheEntries[sCurtainVramSlot].offset >> 5;
#ifdef UBFIX
  part = sCurtainSprites;
#else
  word = (u32 *)sCurtainSprites;
#endif
  if (sCurtainGap != 0)
    sCurtainGap--;
  // The top row, then the bottom two.
  for (i = 0; i < CURTAIN_ROW_PARTS; i++) {
    s32 topEdge;

    gap = sCurtainGap;
    topEdge = -gap / 2;
    CURTAIN_PART((topEdge & (OBJ_Y_RANGE - 1)) | OBJ_X(i * CURTAIN_PART_WIDTH) |
                     OBJ_SIZE_32X16,
                 tile);
  }
  edge = (gap / 2 + CURTAIN_BOTTOM_Y) & (OBJ_Y_RANGE - 1);
  for (i = 0; i < CURTAIN_ROW_PARTS; i++)
    CURTAIN_PART(OBJ_X(i * CURTAIN_PART_WIDTH) | edge | OBJ_SIZE_32X16, tile);
  i = 0;
  edge = (sCurtainGap / 2 + CURTAIN_BOTTOM_Y + CURTAIN_PART_HEIGHT) &
         (OBJ_Y_RANGE - 1);
  for (; i < CURTAIN_ROW_PARTS; i++)
    CURTAIN_PART(OBJ_X(i * CURTAIN_PART_WIDTH) | edge | OBJ_SIZE_32X16, tile);
  for (i = 0; i < 3 * CURTAIN_ROW_PARTS; i++)
    Oam_AddSpriteAtDepth(&sCurtainSprites[i], OAM_DEPTH_FRONT);
}

/// Fills a curtain sprite's tiles with colour 1 and starts the curtains
/// CURTAIN_START_GAP pixels apart from where they rest.
static void Curtain_Start(void) {
  u8 *tiles;

  tiles = Heap_CallAlternatePool(Heap_AllocEwram,
                                 CURTAIN_PART_WIDTH * CURTAIN_PART_HEIGHT / 2);
  sCurtainVramSlot = VramCache_FindFreeEntry();
  Dma_Fill32(PIXELS_4BPP(1), tiles,
             CURTAIN_PART_WIDTH * CURTAIN_PART_HEIGHT / 2);
  VramCache_Load(sCurtainVramSlot, CURTAIN_PART_WIDTH * CURTAIN_PART_HEIGHT / 2,
                 tiles);

  sCurtainGap = CURTAIN_START_GAP;
  Task_AddFrame(Curtain_Close, TASK_PASS_FRAME);
}

/// The ship's glow.
///
/// ShipGlow_CyclePalette shows six of these colours at a time, from a start
/// that moves one colour every 6 frames.
u16 gShipGlowPalettes[] = {
    RGB(0, 3, 22),  RGB(0, 14, 26), RGB(0, 25, 31), RGB(0, 14, 26),
    RGB(0, 3, 22),  RGB(0, 0, 18),  RGB(0, 3, 22),  RGB(0, 14, 26),
    RGB(0, 25, 31), RGB(0, 14, 26), RGB(0, 3, 22),  RGB(0, 0, 0),
};

/// A drop of spray, at the end of its rise: released.
s32 gSprayFallScript[] = {
    SC_RELEASE,
};

/// The ripple's sine, the line scroll's gLineScroll_WaveSine kept again in
/// this overlay.
s16 gRippleSine[LINE_SCROLL_WAVE_ENTRIES] = {LINE_SCROLL_WAVE_SINE};

struct SceneEntrance gLemurianShipEntrances[] = {
    {ENTRANCE_DEFAULT, CONDITION_ALWAYS, 408, 0, 200, FACING_NORTH, 0, -1, -1,
     -1, -1, 0},
    {LEMURIAN_SHIP_FROM_TIMBER_ROOMS_C, CONDITION_ALWAYS, 312, 0, 296,
     FACING_NORTH, 0, -1, -1, -1, 328, 0},
    {SCENE_TABLE_END},
};

u32 gLemurianShipExits[] = {
    SCENE_EXITS(LemurianShip),
    SCENE_EXIT(LEMURIAN_SHIP_TO_TIMBER_ROOMS_C, TimberRoomsC,
               TIMBER_ROOMS_C_FROM_LEMURIAN_SHIP),
    SCENE_EXITS_END,
};

struct ActorPlacement gLemurianShipActorPlacements[] = {
    ACTOR_PLACEMENT(ACTOR_GARET, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0,
                    FACING_EAST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(ACTOR_IVAN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0,
                    FACING_EAST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(ACTOR_MIA, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0,
                    FACING_EAST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_GANGWAY, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 312,
                    0, 248, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_GANGWAY, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 312,
                    0, 312, FACING_SOUTH, TALK_FACING_KEEP, 0),
    {SCENE_TABLE_END},
};

struct Trigger gLemurianShipTriggers[] = {
    {TRIGGER_STEP, 1, CONDITION_ALWAYS, EVENT(LemurianShip_LeaveTheShip)},
    {PSYNERGY_ANYWHERE(ABILITY_REVEAL, PSYNERGY_PHASE_ACTS), 0,
     CONDITION_ALWAYS, EVENT(Ripple_StopForReveal)},
    {PSYNERGY_ANYWHERE(ABILITY_REVEAL, PSYNERGY_PHASE_FADES), 0,
     CONDITION_ALWAYS, EVENT(Ripple_Start)},
    {USE_ITEM(ITEM_BLACK_ORB), 10, CONDITION_ALWAYS,
     EVENT(LemurianShip_RaiseTheShip)},
    {TRIGGER_TABLE_END},
};

/// The Black Orb's glow: whether it shows, its phase in frames, its strength
/// (BLDALPHA's first weight, a step weaker every 40 frames down to 4) and the
/// frames into that step.
s32 gOrbGlowActive = 0;
s32 gOrbGlowPhase = 0;
s32 gOrbGlowStrength = 16;
s32 gOrbGlowFadeFrames = 0;

/// Set while spray flies about the rising ship.
s32 gSprayActive = 0;

/// The raised ship's swell: whether it runs, its phase in frames, the hull's
/// and deck's layer heights it rocks about, and the height on the deck of each
/// of the party (16.16 pixels), SHIP_NOT_ON_DECK for one not on it.
s32 gSwellActive = 0;
s32 gSwellPhase = 0;
s32 gShipHullY = 0;
s32 gShipDeckY = 0;
s32 gLeaderDeckY = SHIP_NOT_ON_DECK;
s32 gGaretDeckY = SHIP_NOT_ON_DECK;
s32 gMiaDeckY = SHIP_NOT_ON_DECK;
s32 gIvanDeckY = SHIP_NOT_ON_DECK;

/// The frames into the glow's run along the risen ship's hull,
/// SHIP_GLOW_STEP_FRAMES for each of its SHIP_GLOW_COLORS colours.
u16 gShipGlowStep = 0;

s32 gLeaderBoardsShip[] = {
    SC_ANIMATION(2),
    SC_MOVE_BY(-8, 0, 0),
    SC_SYNC,
    SC_MOVE_BY(0, 0, -16),
    SC_SYNC,
    SC_SET(SW_VELOCITY_Y, toFIXED(8)),
    SC_SE(SE_HIGH_JUMP),
    SC_MOVE_BY(0, 0, -24),
    SC_SYNC,
    SC_ANIMATION(1),
    SC_WAIT(20),
    SC_ANIMATION(2),
    SC_MOVE_BY(-8, 0, 0),
    SC_SYNC,
    SC_MOVE_BY(0, 0, -56),
    SC_SYNC,
    SC_ANIMATION(1),
    SC_SET(SW_FACING, FACING_SOUTH),
    SC_WAIT(1),
    SC_CALL(LemurianShip_LeaderStepOnDeck),
    SC_END,
};

s32 gGaretBoardsShip[] = {
    SC_ANIMATION(2),
    SC_MOVE_BY(0, 0, -32),
    SC_SYNC,
    SC_SET(SW_VELOCITY_Y, toFIXED(8)),
    SC_SE(SE_HIGH_JUMP),
    SC_MOVE_BY(0, 0, -24),
    SC_SYNC,
    SC_ANIMATION(1),
    SC_WAIT(20),
    SC_ANIMATION(2),
    SC_MOVE_BY(-8, 0, 0),
    SC_SYNC,
    SC_MOVE_BY(0, 0, -40),
    SC_SYNC,
    SC_ANIMATION(1),
    SC_SET(SW_FACING, FACING_SOUTH),
    SC_WAIT(1),
    SC_CALL(LemurianShip_GaretStepOnDeck),
    SC_END,
};

s32 gIvanBoardsShip[] = {
    SC_ANIMATION(2),
    SC_MOVE_BY(-20, 0, 0),
    SC_SYNC,
    SC_MOVE_BY(0, 0, -32),
    SC_SYNC,
    SC_SET(SW_VELOCITY_Y, toFIXED(8)),
    SC_SE(SE_HIGH_JUMP),
    SC_MOVE_BY(0, 0, -24),
    SC_SYNC,
    SC_ANIMATION(1),
    SC_WAIT(20),
    SC_ANIMATION(2),
    SC_MOVE_BY(-8, 0, 0),
    SC_SYNC,
    SC_MOVE_BY(0, 0, -24),
    SC_SYNC,
    SC_ANIMATION(1),
    SC_SET(SW_FACING, FACING_SOUTH),
    SC_WAIT(1),
    SC_CALL(LemurianShip_IvanStepOnDeck),
    SC_END,
};

s32 gMiaBoardsShip[] = {
    SC_ANIMATION(2),
    SC_MOVE_BY(-8, 0, 0),
    SC_SYNC,
    SC_MOVE_BY(0, 0, -8),
    SC_SYNC,
    SC_SET(SW_VELOCITY_Y, toFIXED(8)),
    SC_SE(SE_HIGH_JUMP),
    SC_MOVE_BY(0, 0, -24),
    SC_SYNC,
    SC_ANIMATION(1),
    SC_WAIT(20),
    SC_ANIMATION(2),
    SC_MOVE_BY(-8, 0, 0),
    SC_SYNC,
    SC_MOVE_BY(0, 0, -8),
    SC_SYNC,
    SC_ANIMATION(1),
    SC_CALL(LemurianShip_MiaStepOnDeck),
    SC_END,
};
