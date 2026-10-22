/// @file The wharf at the edge of Lalivero where Babi's ship lies sunk, the
/// game's last scene: SceneId_LemurianShip, with the area name Lemurian ship.
/// It is entered from Faran's house (lalivero_interiors.c) once Iodem has given
/// the party the Black Orb, and stepping back on cell code 1
/// (Event_LeaveTheShip) returns there.
///
/// LemurianShip_SetupScene sinks the ship's two map layers 90 pixels, puts the
/// two pieces of the gangway and the party at sprite priority 1, and starts the
/// water's ripple (Ripple_Start), an HBlank DMA of per-line scroll offsets for
/// BG3 that Reveal stops while it shows the scene. Arriving by
/// LEMURIAN_SHIP_WITH_BLACK_ORB, which only the debug scene selector reaches,
/// first gives the leader the Black Orb.
///
/// Using the Black Orb at the water's edge (cell code 10) plays
/// Event_RaiseTheShip: the orb glows, spray rises and the ship comes up out of
/// the water 90 pixels. It adds Ship_UpdateSwell and ShipGlow_CyclePalette,
/// which run to the end of the game, and then LemurianShip_SetSail plays the
/// departure: three yes-or-no questions each choose one of two replies, the
/// party boards, the ship sails east as the curtains close (Curtain_Start), and
/// the game goes to the clear screen by the title's TITLE_FROM_LEMURIAN_SHIP,
/// keeping the music (FLAG_KEEP_MUSIC).
///
/// Open questions: none.

#include "script.h"
#include "version.h"
#include "game_state.h"
#include "vram_tab.h"
#include "unit/scene/lemurian_ship.h"
#include "unit/scene/common/scroll_initialize_hblank_dma.h"
#include "unit/object/object.h"
#include "unit/object/psynergy_effects.h"
#include "unit/game/game.h"
#include "unit/shop/shop.h"
#include "unit/sound/sound.h"
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
#include "global_cells.h"
#include "types.h"
#include "field_event.h"
#include "dma.h"
#include "io_reg.h"
#include "iwram_call.h"
#include "fixed_point_position.h"
#include "tbs_edition.h"
#include "scene_ids.h"
#include "field_obj.h"
#include "iowrite.h"
#include "map_scroll.h"
#include "field_service.h"
#include "text/msg_ids.h"
#include "callback_scheduler.h"
#include "mem.h"
#include "scroll.h"
#include "resource.h"
#include "vram_block.h"
#include "ability_ids.h"
#include "field_scene.h"
#include "objcmd.h"
#include "item_ids.h"
#include "songs.h"
#include "audio_engine_symbols.h"
#include "flags.h"

#if TBS_PATCH_3
#define LEMURIAN_SHIP_DEPARTURE_DELAY_FRAMES 30
#else
#define LEMURIAN_SHIP_DEPARTURE_DELAY_FRAMES 80
#endif

static void Ripple_Stop(void);
static void Curtain_Start(void);

enum {
  GANGWAY_NORTH =
      ACTOR_FIRST_PLACED, ///< the gangway, in two pieces (SPRITE_GANGWAY)
  GANGWAY_SOUTH
};

/// The deck height of a party member who has not yet boarded: the swell
/// (Ship_UpdateSwell) moves only those who have.
#define SHIP_NOT_ON_DECK (-0x10000)

/// The sprite of a gangway piece or a party actor.
// ACTOR_SPRITE reads actor->sprite through an s32, so each store reloads it as
// the native does; read as the field, the two stores share one load.
// UB: the s32 read aliases the pointer field. UBFIX reads the field itself.
#ifdef UBFIX
#define ACTOR_SPRITE(actor) ((actor)->sprite)
#else
#define ACTOR_SPRITE(actor) ((struct AnimationObject *)*(s32 *)&(actor)->sprite)
#endif
/// The sprite priority of an actor's picture and of its shadow.
#define SET_IMAGE_PRIORITY(actor, level)                                       \
  ((void)(ACTOR_SPRITE(actor)->image.priority = (level)))
#define SET_SHADOW_PRIORITY(actor, level)                                      \
  ((void)(ACTOR_SPRITE(actor)->shadow.priority = (level)))

s32 Math_DivideUnsigned();

const struct SceneEntrance *LemurianShip_GetEntrances(void) {
  return gLemurianShipEntrances;
}

const struct SceneRegion *LemurianShip_GetRegions(void) { return NULL; }

const u32 *LemurianShip_GetExits(void) { return gLemurianShipExits; }

/// The gangway's two pieces; the party's own actors fill the first three rows.
const struct ScenePlacement *LemurianShip_GetPlacements(void) {
  return gLemurianShipPlacements;
}

/// Lemurian ship, the way back to Lalivero: leaves and clears the backdrop
/// colour. Reached by stepping on cell code 1 (gLemurianShipEvents); takes
/// LEMURIAN_SHIP_TO_LALIVERO_FARANS_HOUSE.
static void Event_LeaveTheShip(void) {
  Engine_EventRequestExit(gEventWork->touchedTrigger);
  Io_Write16(0, &PLTT_BACKDROP);
}

/// Lemurian ship: stops the water's ripple while Reveal shows the scene.
/// Reached by Reveal anywhere in the scene; Ripple_Start, as Reveal's light
/// fades, starts the ripple again.
static void Ripple_StopForReveal(void) { Ripple_Stop(); }

/// Starts the two-axis ripple with its original frequencies and amplitudes.
/// Called by LemurianShip_SetupScene, and reached by Reveal as the spell ends.
static void Ripple_Start(void) {
  DisplayScroll_InitializeHBlankDma(0, 0x40000, 0x10000, 0x2000, 0x10000,
                                    0x8000, 0x4000);
}

/// The exit, Reveal and the Black Orb at the water's edge (cell code 10).
const struct SceneEvent *LemurianShip_GetEvents(void) {
  return gLemurianShipEvents;
}

/// Slides a window of six colours along gShipGlowPalettes into palette 7's
/// colours 4 to 9, one step every 6 frames, so the ship's glow ripples.
/// Run each frame once Event_RaiseTheShip has brought the ship up.
static void ShipGlow_CyclePalette(void) {
  u16 *frame;
  s32 paletteIndex;

  frame = (u16 *)gShipGlowStep;
  paletteIndex = Math_DivideUnsigned(*frame, 6);
  Dma_Set((const void *)(((u32)(paletteIndex << 16) >> 15) +
                         (u32)gShipGlowPalettes),
          (void *)&BG_PLTT_COLOR(7, 4), (DMA_ENABLE << 16) | 6, REG_DMA3);
  (*frame)++;
  if (*frame > 35)
    *frame = 0;
}

/// A drop of spray rises and wobbles for its timer, then falls back by
/// gSprayFallScript, which frees it.
/// The update hook Ship_UpdateSwell gives each drop it creates.
static void Spray_Update(union FieldObject *fieldObject) {
  struct FieldActor *drop = (struct FieldActor *)fieldObject;

  if (drop->timer != 0) {
    s32 drift;

    drop->timer--;
    drift = Engine_RandomNext();
    drift -= Engine_RandomNext();
    drop->x.fixed += drift;
    drop->y.fixed += 0xcccc;
  } else if ((s16)drop->work != 0) {
    drop->work = 0;
    Object_SetMode(drop, 1);
    drop->wait = 20;
    Engine_ObjectSetScript(drop, gSprayFallScript);
  }
}

/// Every 40 frames, the Black Orb's glow fades a step, down to 4.
/// Run each frame while the ship rises; Event_RaiseTheShip adds and removes it.
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

/// The Black Orb's glow pulses in the blend, the ship (the hull on BG2, the
/// deck on BG1) and everyone on deck bob with the swell, and spray rises from
/// the water in a screen-sized box (240 by 160) at the view.
/// Run each frame from Event_RaiseTheShip to the end of the game; each part
/// runs while its flag (gOrbGlowActive, gSwellActive, gSprayActive) is set.
static void Ship_UpdateSwell(void) {
  /* FAKEMATCH(volatile): blend kept at sp+18 and loaded again with ldrh for
   * the register write; as a plain u16, 5 of 225 instructions differ in
   * tbs-en. */
  volatile u16 blend;
  struct FixedPointPosition place;
  struct FixedPointPosition *pos;
  struct MapScrollWork *map;
  struct FieldActor *actor;
  s32 swell;
  s32 x, z;

  map = gMapWork[0];
  if (gOrbGlowActive != 0) {
    swell = Iwram_MulQ16(Engine_MathSin(gOrbGlowPhase << 9), 3);
    blend = gOrbGlowStrength + ((swell + 8) << 8);
    Io_Write16(blend, &REG_BLDALPHA);
    gOrbGlowPhase++;
  }
  if (gSwellActive != 0) {
    swell = Iwram_MulQ16(Engine_MathSin(gSwellPhase << 9), 2) << 16;
    map->layers[MAP_LAYER_MIDDLE].offsetY = gShipHullY + swell;
    map->layers[MAP_LAYER_OVERHEAD].offsetY = gShipDeckY + swell;
    if (gLeaderDeckY != SHIP_NOT_ON_DECK) {
      actor = Object_GetById(ACTOR_PARTY_LEADER);
      actor->y.fixed = gLeaderDeckY + swell;
      actor->terrainHeight = gLeaderDeckY + swell;
      actor->motionFlags = 0;
    }
    if (gGaretDeckY != SHIP_NOT_ON_DECK) {
      actor = Object_GetById(GARET);
      actor->y.fixed = gGaretDeckY + swell;
      actor->terrainHeight = gLeaderDeckY + swell;
      actor->motionFlags = 0;
    }
    if (gMiaDeckY != SHIP_NOT_ON_DECK) {
      actor = Object_GetById(MIA);
      actor->y.fixed = gMiaDeckY + swell;
      actor->terrainHeight = gLeaderDeckY + swell;
      actor->motionFlags = 0;
    }
    if (gIvanDeckY != SHIP_NOT_ON_DECK) {
      actor = Object_GetById(IVAN);
      actor->y.fixed = gIvanDeckY + swell;
      actor->terrainHeight = gLeaderDeckY + swell;
      actor->motionFlags = 0;
    }
    gSwellPhase++;
  }
  if (gSprayActive != 0 && (gFrameCount & 1) != 0) {
    x = map->viewX & -0x10000;
    z = map->viewY & -0x10000;
    x += Random_Next() * 240;
    pos = &place;
    pos->x = x;
    pos->y = 0;
    z += Random_Next() * 160;
    z += 0x1e0000;
    pos->z = z;
    actor = Object_Create(SPRITE_WATER_DROP, pos->x, pos->y, pos->z);
    if (actor != 0) {
      actor->update = Spray_Update;
      actor->timer = 60;
      actor->work = 1;
      actor->motionFlags = 0;
      actor->priorityFlags = ACTOR_PRIORITY_UNDERFOOT;
      actor->sprite->image.priority = 2;
      Engine_ActorSetSpriteFlags(actor, OBJECT_BLEND_NORMAL);
      Object_SetMode(actor, 0);
    }
  }
}

/// Remembers the leader's height on deck, so that from then on he bobs with
/// the ship.
/// Called by gLeaderBoardsShip as he steps on deck; the other members'
/// boarding scripts call their own copies below.
static s32 Leader_StepOnDeck(struct ScriptInterpreter *interpreter) {
  s32 *deckY;
  struct FieldActor *actor;

  deckY = &gLeaderDeckY;
  actor = Object_GetById(ACTOR_PARTY_LEADER);
  *deckY = actor->y.fixed;
  return 0;
}

/// Remembers Garet's boarding height for the swell callback.
static s32 Garet_StepOnDeck(struct ScriptInterpreter *interpreter) {
  s32 *deckY;
  struct FieldActor *actor;

  deckY = &gGaretDeckY;
  actor = Object_GetById(GARET);
  *deckY = actor->y.fixed;
  return 0;
}

/// Remembers Mia's boarding height for the swell callback.
static s32 Mia_StepOnDeck(struct ScriptInterpreter *interpreter) {
  s32 *deckY;
  struct FieldActor *actor;

  deckY = &gMiaDeckY;
  actor = Object_GetById(MIA);
  *deckY = actor->y.fixed;
  return 0;
}

/// Remembers Ivan's boarding height for the swell callback.
static s32 Ivan_StepOnDeck(struct ScriptInterpreter *interpreter) {
  s32 *deckY;
  struct FieldActor *actor;

  deckY = &gIvanDeckY;
  actor = Object_GetById(IVAN);
  *deckY = actor->y.fixed;
  return 0;
}

/// Lemurian ship, the departure: the others step out and talk over the journey
/// while three questions shape their replies, everyone boards, and the ship
/// sails east as the curtains close and the game goes to the clear screen.
/// Called by Event_RaiseTheShip once the ship has risen. Sets FLAG_KEEP_MUSIC
/// and sends the party to the clear screen (SceneId_Title,
/// TITLE_FROM_LEMURIAN_SHIP).
static void LemurianShip_SetSail(void) {
  struct MapScrollWork *scroll;
  struct FieldActor *ship;
  struct EventWork *event;

  Engine_EventStart();
  Camera_MoveTo(-1, -1, -1, 0);
  Map_CopyCellAttributes(18, 0, 3, 1, 18, 12);
  // Garet, Mia and Ivan step out around the leader. Garet: "It looks like
  // it's finally time to leave Gondowan." Ivan and Mia say how much has
  // happened.
  Engine_EventSetMessage(MsgFieldLooksLikeFinally);
  FieldMotion_LaunchFromFocusedObject(GARET, -10, 16, FACING_NORTH);
  FieldMotion_LaunchFromFocusedObject(MIA, 0, 24, FACING_NORTH);
  Motion_LaunchFromFocusedObject(IVAN, 10, 16, FACING_NORTH);
  Engine_ActorWaitForMove(GARET);
  Engine_EventWait(50);
  Engine_ActorRunRepeatedMotion(GARET, 2);
  Engine_EventWait(20);
  Engine_EventShowMessage(GARET, 0);
  Engine_EventWait(10);
  Engine_ActorSetAnimationAndWait(IVAN, ANIMATION_NOD);
  Engine_EventWait(20);
  Engine_EventShowMessage(IVAN, 0);
  Engine_EventWait(10);
  Actor_FaceDirection(MIA, FACING_SOUTH, 0);
  Engine_EventWait(40);
  Engine_EventShowMessage(MIA, 0);
  Engine_EventWait(10);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_SOUTH, 0);
  Actor_FaceDirection(GARET, FACING_SOUTH, 0);
  Engine_ActorFaceDirection(IVAN, FACING_SOUTH, 0);
  Engine_EventWait(60);
  Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, ANIMATION_NOD);
  Engine_ActorSetAnimation(GARET, ANIMATION_NOD);
  Engine_ActorSetAnimationAndWait(IVAN, ANIMATION_NOD);
  Engine_EventWait(60);
  // The first question, Garet's: will the journey be over once they find
  // Jenna? The answer picks which of his two replies he says.
  Engine_ActorRunRepeatedMotion(GARET, 2);
  Engine_EventWait(30);
  Engine_ActorFaceDirection(GARET, FACING_NORTHEAST, 0);
  Engine_EventWait(20);
  UiText_OpenMessageAtObject(1, 0);
  if (Engine_EventChooseYesNo(ACTOR_PARTY_LEADER, 0) == 0) {
    Engine_EventWait(30);
    Actor_ShowEmote(GARET, EMOTE_IN_FRONT | EMOTE_WINCE, 50);
    Engine_EventShowMessage(GARET, 0);
    gEventWork->message++;
  } else {
    Engine_EventWait(30);
    Actor_ShowEmote(GARET, EMOTE_IN_FRONT | EMOTE_WINCE, 50);
    gEventWork->message++;
    Engine_EventShowMessage(GARET, 0);
  }
  Engine_EventWait(10);
  // Ivan and Mia remind Garet of their promise to Babi, and Lemuria comes up.
  Engine_ActorFaceDirection(IVAN, FACING_WEST, 0);
  Engine_EventWait(30);
  Actor_ShowEmote(IVAN, EMOTE_IN_FRONT | EMOTE_QUESTION, 50);
  Engine_EventShowMessage(IVAN, 0);
  Engine_EventWait(10);
  Engine_ActorFaceDirection(GARET, FACING_EAST, 0);
  Engine_EventWait(30);
  Actor_ShowEmote(GARET, EMOTE_IN_FRONT | EMOTE_QUESTION, 60);
  Engine_ActorRunRepeatedMotion(MIA, 2);
  Engine_EventWait(30);
  Engine_ActorFaceDirection(MIA, FACING_NORTHWEST, 0);
  Engine_EventWait(30);
  Engine_EventShowMessage(MIA, 0);
  Engine_EventWait(10);
  Actor_ShowEmote(GARET, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 50);
  Engine_EventShowMessage(GARET, 0);
  Engine_EventWait(10);
  Engine_ActorRunRepeatedMotion(IVAN, 2);
  Engine_EventWait(20);
  Engine_EventShowMessage(IVAN, 0);
  Engine_EventWait(10);
  Engine_ActorFaceDirection(MIA, FACING_NORTH, 0);
  Engine_EventWait(20);
  Engine_ActorSetAnimationAndWait(MIA, ANIMATION_SHAKE_HEAD);
  Engine_EventWait(20);
  // The second question, Mia's: must they find Jenna before going to
  // Lemuria? Garet answers either way.
  UiText_OpenMessageAtObject(3, 0);
  if (Engine_EventChooseYesNo(ACTOR_PARTY_LEADER, 0) == 0) {
    Engine_EventWait(20);
    Engine_ActorRunRepeatedMotion(GARET, 2);
    Engine_EventWait(20);
    Engine_ActorFaceDirection(GARET, FACING_NORTHEAST, 0);
    Engine_EventWait(20);
    Engine_EventShowMessage(GARET, 0);
    gEventWork->message++;
  } else {
    Engine_EventWait(20);
    Engine_ActorRunRepeatedMotion(GARET, 2);
    Engine_EventWait(20);
    Engine_ActorFaceDirection(GARET, FACING_NORTHEAST, 0);
    Engine_EventWait(20);
    gEventWork->message++;
    Engine_EventShowMessage(GARET, 0);
  }
  Engine_EventWait(10);
  // Ivan: their first time on the ocean; Garet: the ocean Kraden wanted
  // him to see.
  Engine_ActorShowEmote(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | EMOTE_WINCE, 50);
  Engine_EventWait(10);
  Actor_FaceDirection(IVAN, FACING_NORTHWEST, 0);
  Engine_EventWait(20);
  Engine_ActorSetAnimationAndWait(IVAN, ANIMATION_NOD);
  Engine_EventWait(20);
  Engine_EventShowMessage(IVAN, 0);
  Engine_EventWait(20);
  Engine_EventShowMessage(GARET, 0);
  Engine_EventWait(20);
  Engine_ActorSetAnimationAndWait(ACTOR_PARTY_LEADER, ANIMATION_NOD);
  Engine_EventWait(30);
  Engine_EventWait(10);
  Engine_ActorSetAnimationAndWait(MIA, ANIMATION_NOD);
  Engine_EventWait(20);
  // The third question, Mia's: the seas are rough; will they be all right?
  UiText_OpenMessageAtObject(3, 0);
  Actor_FaceDirection(GARET, FACING_NORTHEAST, 0);
  Actor_FaceDirection(MIA, FACING_NORTH, 0);
  Engine_ActorFaceDirection(IVAN, FACING_NORTHWEST, 0);
  Engine_EventWait(20);
  if (Engine_EventChooseYesNo(ACTOR_PARTY_LEADER, 0) == 0) {
    Engine_EventWait(30);
    Engine_ActorSetAnimationAndWait(MIA, ANIMATION_NOD);
    Engine_EventWait(20);
    Engine_EventShowMessage(MIA, 0);
    gEventWork->message++;
  } else {
    Engine_EventWait(30);
    Engine_ActorSetAnimationAndWait(MIA, ANIMATION_NOD);
    Engine_EventWait(20);
    gEventWork->message++;
    Engine_EventShowMessage(MIA, 0);
  }
  Engine_EventWait(10);
  // Garet: "Well... Shall we, then?" Ivan: "To sea..." Mia: Jenna is waiting
  // for them. The departure theme starts.
  Actor_FaceDirection(GARET, FACING_NORTH, 0);
  Engine_EventWait(30);
  Engine_EventShowMessage(GARET, 0);
  Engine_EventWait(10);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 0);
  Engine_EventWait(30);
  Engine_EventWait(10);
  Actor_FaceDirection(IVAN, FACING_NORTH, 0);
  Engine_EventWait(30);
  Engine_ActorSetAnimationAndWait(IVAN, ANIMATION_NOD);
  Engine_EventWait(20);
  Engine_EventShowMessage(IVAN, 0);
  Engine_EventWait(10);
  Engine_ActorSetAnimationAndWait(MIA, ANIMATION_NOD);
  Engine_EventWait(20);
  Engine_EventShowMessage(MIA, 0);
  Engine_AudioPlayCue(AUDIO_CUE_FADE_BGM);
  Engine_EventWait(10);
  Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, ANIMATION_NOD);
  Engine_ActorSetAnimation(GARET, ANIMATION_NOD);
  Engine_ActorSetAnimationAndWait(IVAN, ANIMATION_NOD);
  Engine_EventWait(40);
  // Everyone boards by their own scripts, hopping up onto the deck (SE_153
  // each); the camera follows up to the ship.
  Actor_SetSpeed(ACTOR_PARTY_LEADER, 0x13333, 0x9999);
  Actor_SetSpeed(GARET, 0x13333, 0x9999);
  Actor_SetSpeed(IVAN, 0x13333, 0x9999);
  Actor_SetSpeed(MIA, 0x13333, 0x9999);
  Actor_SetActionScript(ACTOR_PARTY_LEADER, gLeaderBoardsShip);
  Engine_EventWait(50);
  Camera_MoveTo(PIXELS(264), -1, PIXELS(200), 1);
  Actor_SetActionScript(GARET, gGaretBoardsShip);
  Engine_EventWait(50);
  Actor_SetActionScript(IVAN, gIvanBoardsShip);
  Object_RefreshSelectorById(2);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_SOUTH, 0);
  Actor_FaceDirection(GARET, FACING_SOUTH, 0);
  Actor_FaceDirection(IVAN, FACING_SOUTH, 0);
  // Mia, last, looks back at the shore before she boards.
  FieldObjectMotion_CommitPositionAndActivate(3, 0, -32);
  Engine_EventWait(30);
  Engine_ActorFaceDirection(MIA, FACING_SOUTH, 0);
  Engine_EventWait(60);
  Actor_FaceDirection(MIA, FACING_WEST, 0);
  Engine_EventWait(20);
  Actor_SetActionScript(MIA, gMiaBoardsShip);
  Object_RefreshSelectorById(3);
  Engine_ActorFaceDirection(MIA, FACING_NORTH, 0);
  Engine_EventWait(20);
  // On deck, all four turn west together and nod.
  Camera_MoveTo(PIXELS(216), -1, PIXELS(168), 1);
  Engine_CameraWaitForMove();
  Engine_EventWait(20);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_WEST, 0);
  Actor_FaceDirection(GARET, FACING_WEST, 0);
  Actor_FaceDirection(IVAN, FACING_WEST, 0);
  Engine_ActorFaceDirection(MIA, FACING_WEST, 0);
  Engine_EventWait(40);
  Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, ANIMATION_NOD);
  Engine_ActorSetAnimation(GARET, ANIMATION_NOD);
  Engine_ActorSetAnimation(MIA, ANIMATION_NOD);
  Engine_ActorSetAnimationAndWait(IVAN, ANIMATION_NOD);
  Engine_EventWait(30);
  // The ship sails east and the curtains close: the sea's layer scrolls
  // faster while the gangway's north piece moves off with the ship, and the
  // camera follows.
  Engine_AudioPlayCue(MUS_ENDING);
  MusicCommand_SetPitchAndUpdateFrequency(240);
  Curtain_Start();
  Engine_EventWait(LEMURIAN_SHIP_DEPARTURE_DELAY_FRAMES);
  scroll = gMapWork[0];
  ship = Object_GetById(GANGWAY_NORTH);
  ship->acceleration = 131;
  ship->speed = 0x20000;
  scroll->layers[MAP_LAYER_GROUND].speedX = -0x3000;
  ObjectMotion_CommitPositionAndActivate(GANGWAY_NORTH, 60, 0);
  scroll->layers[MAP_LAYER_GROUND].speedX = -0x6000;
  ObjectMotion_CommitPositionAndActivate(GANGWAY_NORTH, 60, 0);
  Engine_EventWait(80);
  Engine_EventWait(100);
  Engine_CameraSetSpeed(0x23333, 0x28f);
  Engine_CameraMoveTo(PIXELS(808), -1, PIXELS(168), 1);
  Engine_EventWait(300);
  // Fade out to the clear screen.
  event = gMapWork[19];
  event->startTransition = SCENE_TRANSITION(TRANSITION_BACKDROP_FADE, 0);
  Io_Write16(0, &PLTT_BACKDROP);
  event->transitionFrames = 96;
  Engine_EventCloseScreen();
  Engine_EventWaitForScreen();
  Engine_EventWait(30);
  Engine_GameFlagSet(FLAG_KEEP_MUSIC);
  Event_SetDestination((s32)&SceneId_Title, TITLE_FROM_LEMURIAN_SHIP);
}

/// The water's ripple, this scene's view of the HBlank scroll work
/// (gHBlankScrollWork, struct DisplayScrollWork): two pages of per-line
/// scroll offsets, swapped each frame, built from a sine table. Here a line
/// is one word, BG3's x and y, where DisplayScrollWork has three.
struct RippleScrollWork {
  u16 pages[2][0x3c0]; ///< per page: 160 lines of BG3 x, y; the rest unused
  u8 page;             ///< the page the HBlank DMA shows; the other is built
  u8 mode;
  u16 phase;        ///< frames built so far, added to BG3's y
  u8 unused[4];     ///< zeroed with the block; nothing reads or writes it
  s32 frequency[2]; ///< x, y: 16.16 sine entries per frame and per row
  s32 step[2];      ///< x, y: 16.16 sine entries a line
  s32 amplitude[2]; ///< x, y: 16.16 scale on the sine, then divided by 256
};

/// One line's ripple offset.
static __inline__ s32 Wave_Offset(s32 angle, s32 amplitude, u16 base) {
  return (u16)(Iwram_MulQ16(gRippleSine[(angle >> 16) & 0xff], amplitude) /
               256) +
         base;
}

/// Lemurian ship: the Black Orb glows, the water lights up, spray rises and
/// the ship comes up out of the sea 90 pixels; then the party sets sail.
/// Reached by using the Black Orb at the water's edge (cell code 10). Adds
/// the swell and glow callbacks, which run to the end of the game, and calls
/// LemurianShip_SetSail.
static void Event_RaiseTheShip(void) {
  struct MapScrollWork *map;
  s32 i;
  s32 flash;
  s32 white;
  s32 dark;
  s32 risen;
  /* FAKEMATCH(volatile): each BGxCNT value stored at sp+6 with strh on its
   * way to the register; as a plain u16, 6 of 231 instructions differ in
   * tbs-en. */
  volatile u16 cnt;

  map = gMapWork[0];

  Event_Start();
  Field_EndBusyAndReveal();
  // The leader steps to the water's edge; the backdrop brightens 16 steps.
  Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 312, 232);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 0);
  Event_Wait(40);
  Event_PlayCue(SE_140);
  for (i = 0; i <= 15; i++) {
    Io_Write16(RGB(0, i, i * 2), &PLTT_BACKDROP);
    Event_Wait(10);
  }
  {
    u16 color = RGB(0, 16, 31);

    PLTT_BACKDROP = color;
  }
  {
    u16 bright = BLEND_ALPHA(16, 16);

    for (flash = 0; flash < 3; flash++) {
      Event_PlayCue(SE_212);
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
  FieldScheduler_AddCallback(Ship_UpdateSwell, TASK_PRIORITY_SCENE);
  gSprayActive = 1;
  Event_Wait(20);
  Event_PlayCue(SE_163);
  Map_SetShake(0x10000, 0x10000, 0x10000);
  Event_Wait(60);
  gSprayActive = 1;
  Map_SetShake(0x20000, 0x20000, 0x10000);
  Event_Wait(60);
  Map_SetShake(0x30000, 0x30000, 0x10000);
  gOrbGlowFadeFrames = 0;
  FieldScheduler_AddCallback(OrbGlow_Fade, TASK_PRIORITY_SCENE);
  // The ship rises 90 pixels, 0.2 pixels a frame.
  risen = 0;
  do {
    map->layers[MAP_LAYER_MIDDLE].offsetY += 0x3333;
    map->layers[MAP_LAYER_OVERHEAD].offsetY += 0x3333;
    risen += 0x3333;
    Task_Wait(1);
  } while (risen < PIXELS(90));
  Engine_TaskRemoveCallback(OrbGlow_Fade);
  gSprayActive = 0;
  REG_BG3CNT = cnt = (REG_BG3CNT & ~BGCNT_PRIORITY(3)) | BGCNT_PRIORITY(3);
  REG_BG2CNT = cnt = (REG_BG2CNT & ~BGCNT_PRIORITY(3)) | BGCNT_PRIORITY(3);
  REG_BG1CNT = cnt = (REG_BG1CNT & ~BGCNT_PRIORITY(3)) | BGCNT_PRIORITY(2);
  gOrbGlowActive = 0;
  Event_PlayCue(SE_STOP_SLOT3);
  Task_Wait(1);
  // A flash of white; the swell starts as it clears.
  Event_PlayCue(SE_145);
  Io_Write16(BLDCNT_TGT1_ALL | BLDCNT_EFFECT_LIGHTEN, &REG_BLDCNT);
  for (white = 0; white <= 16; white++) {
    REG_BLDY = white;
    Event_Wait(1);
  }
  Event_Wait(40);
  Map_SetShake(-1, -1, 0xe666);
  gShipHullY = map->layers[MAP_LAYER_MIDDLE].offsetY;
  gShipDeckY = map->layers[MAP_LAYER_OVERHEAD].offsetY;
  gSwellActive = 1;
  for (dark = 16; dark >= 0; dark--) {
    REG_BLDY = dark;
    Event_Wait(8);
  }
  FieldScheduler_AddCallback(ShipGlow_CyclePalette, TASK_PRIORITY_SCENE);
  Event_PlayCue(MUS_PUZZLE_SOLVED);
  AudioCommand_WaitForStateByteClear();
  Event_Wait(20);
  Event_End();
  LemurianShip_SetSail();
}

/// Sets the Lemurian ship's scene up: the ship sunk 90 pixels, the gangway and
/// the party at sprite priority 1 over the layers' new priorities, and the
/// ripple running. Arriving by LEMURIAN_SHIP_WITH_BLACK_ORB first gives the
/// leader the Black Orb.
s32 LemurianShip_SetupScene(void) {
  struct MapScrollWork *map;
  /* FAKEMATCH(volatile): each step stored at sp+2 with strh; as a plain u16, 2
   * of 165 instructions differ in tbs-en. */
  volatile u16 cnt;

  map = gMapWork[0];
  if (gGameState.entrance == LEMURIAN_SHIP_WITH_BLACK_ORB) {
    Inventory_AddItem(ACTOR_PARTY_LEADER, ITEM_BLACK_ORB);
  }
  ((struct EventWork *)gMapWork[19])->startTransition =
      SCENE_TRANSITION(TRANSITION_BACKDROP_FADE, 0);
  Object_GetById(GANGWAY_NORTH)->collisionFlags = 0;
  Object_GetById(GANGWAY_NORTH)->priorityFlags = ACTOR_PRIORITY_UNDERFOOT;
  Object_GetById(GANGWAY_SOUTH)->collisionFlags = 0;
  Object_GetById(GANGWAY_SOUTH)->priorityFlags = ACTOR_PRIORITY_UNDERFOOT;
  {
    struct FieldActor *actor = Object_GetById(GANGWAY_NORTH);

    SET_IMAGE_PRIORITY(actor, 1);
  }
  {
    struct FieldActor *actor = Object_GetById(GANGWAY_SOUTH);

    SET_IMAGE_PRIORITY(actor, 1);
  }
  {
    struct FieldActor *actor = Object_GetById(ACTOR_PARTY_LEADER);

    SET_IMAGE_PRIORITY(actor, 1);
    SET_SHADOW_PRIORITY(actor, 1);
  }
  {
    struct FieldActor *actor = Object_GetById(GARET);

    SET_IMAGE_PRIORITY(actor, 1);
    SET_SHADOW_PRIORITY(actor, 1);
  }
  {
    struct FieldActor *actor = Object_GetById(IVAN);

    SET_IMAGE_PRIORITY(actor, 1);
    SET_SHADOW_PRIORITY(actor, 1);
  }
  {
    struct FieldActor *actor = Object_GetById(MIA);

    SET_IMAGE_PRIORITY(actor, 1);
    SET_SHADOW_PRIORITY(actor, 1);
  }
  REG_BG3CNT = cnt = (REG_BG3CNT & ~BGCNT_PRIORITY(3)) | BGCNT_PRIORITY(2);
  REG_BG2CNT = cnt = (REG_BG2CNT & ~BGCNT_PRIORITY(3)) | BGCNT_PRIORITY(3);
  REG_BG1CNT = cnt = (REG_BG1CNT & ~BGCNT_PRIORITY(3)) | BGCNT_PRIORITY(3);
  Io_Write16(BLDCNT_TGT1_BG3 | BLDCNT_EFFECT_BLEND | BLDCNT_TGT2_BG1 |
                 BLDCNT_TGT2_BG2 | BLDCNT_TGT2_BD,
             &REG_BLDCNT);
  Io_Write16(BLEND_ALPHA(16, 8), &REG_BLDALPHA);
  {
    struct MapLayerScroll *layer = map->layers + 1;

    layer->offsetY += -PIXELS(90);
    layer = map->layers + 2;
    layer->offsetY += -PIXELS(90);
  }
  Engine_MapRedraw();
  Ripple_Start();
  return 0;
}

/// Arms DMA 0 to copy the shown ripple page, a line each HBlank, into
/// background 3's scroll registers, writing the first line itself.
/// Run each frame; DisplayScroll_InitializeHBlankDma adds it and Ripple_Stop
/// removes it.
void DisplayScroll_ArmHBlankDma(void) {
  struct RippleScrollWork *work;
  u32 *source;
  volatile u16 *dma0;
  volatile u32 *destination;

  work = (struct RippleScrollWork *)gHBlankScrollWork;
  source = (u32 *)work->pages[work->page];

  dma0 = REG_DMA0;
  destination = (volatile u32 *)&REG_BG3HOFS;
  dma0[5] &= ~DMA_START_TIMING_DRQ_REPEAT;
  dma0[5] &= ~DMA_ENABLE;
  dma0[5];

  *destination = *source++;
  Dma_Set(source, (void *)destination, DMA_HBLANK_WORD_PER_LINE,
          (volatile u32 *)dma0);
}

/// Builds the other ripple page and swaps it in.
/// Run each frame; DisplayScroll_InitializeHBlankDma adds it and Ripple_Stop
/// removes it.
void DisplayScroll_BuildAndSwapHBlankPage(void) {
  struct RippleScrollWork *work;
  u16 *line;
  s32 angle;
  u16 base;
  s32 amplitude;
  s32 step;
  u16 y;
  s32 i;

  work = (struct RippleScrollWork *)gHBlankScrollWork;
  y = gBgScroll[3].y;
  line = work->pages[work->page ^ 1];
  step = work->step[0];
  angle = (work->phase + y) * work->frequency[0];
  amplitude = work->amplitude[0];
  base = gBgScroll[3].x;
  for (i = 0; i != 160; i++) {
    *line = Wave_Offset(angle, amplitude, base);
    angle += step;
    line += 2;
  }
  line = work->pages[work->page ^ 1] + 1;
  step = work->step[1];
  angle = (work->phase + y) * work->frequency[1];
  amplitude = work->amplitude[1];
  for (i = 0; i != 160; i++) {
    *line = Wave_Offset(angle, amplitude, y);
    angle += step;
    line += 2;
  }
  work->phase++;
  work->page ^= 1;
}

#include "common/scroll_initialize_hblank_dma.h"

/// Takes the ripple's callbacks off and stops DMA 0.
/// Called by Ripple_StopForReveal.
static void Ripple_Stop(void) {
  volatile u16 *reg;

  Engine_TaskRemoveCallback(DisplayScroll_ArmHBlankDma);
  Engine_TaskRemoveCallback(DisplayScroll_BuildAndSwapHBlankPage);
  reg = REG_DMA0;
  reg[5] = ~DMA_START_TIMING_DRQ_REPEAT & reg[5];
  reg[5] = ~DMA_ENABLE & reg[5];
  reg[5];
}

/// The curtains: three columns of eight 32 by 8 sprites that close in from
/// the sides as the ship sails.
static struct AnimationSpritePart sCurtainSprites[24];
static s16 sCurtainGap;
static s16 sCurtainVramSlot;

/// Moves the curtains a step closer and queues their 24 sprites.
/// Run each frame from Curtain_Start to the end of the game.
static void Curtain_Close(void) {
#ifdef UBFIX
  u32 words[24 * sizeof(struct AnimationSpritePart) / sizeof(u32)];
#endif
  u32 *entry;
  u32 tile;
  s32 step;
  s32 x;
  u32 i;

  tile = gVramBlockCache[sCurtainVramSlot].offset >> 5;
  // Each part is written whole as three words: the queue link, attributes 0
  // and 1, and attribute 2.
  // UB: the native word cursor crosses the parts' members. UBFIX builds the
  // same words in a word array and copies them.
#ifdef UBFIX
  entry = words;
#else
  entry = (u32 *)sCurtainSprites;
#endif
  if (sCurtainGap != 0)
    sCurtainGap--;
  for (i = 0; i < 8; i++) {
    s32 left;

    step = sCurtainGap;
    left = -step / 2;
    *entry++ = 0;
    *entry++ = (left & 0xff) | (i << 21) | 0x80004000;
    *entry++ = tile;
  }
  x = (step / 2 + 136) & 0xff;
  for (i = 0; i < 8; i++) {
    *entry++ = 0;
    *entry++ = (i << 21) | x | 0x80004000;
    *entry++ = tile;
  }
  i = 0;
  x = (sCurtainGap / 2 + 152) & 0xff;
  for (; i < 8; i++) {
    *entry++ = 0;
    *entry++ = (i << 21) | x | 0x80004000;
    *entry++ = tile;
  }
#ifdef UBFIX
  memcpy(sCurtainSprites, words, sizeof(words));
#endif
  for (i = 0; i < 24; i++)
    Runtime_PushSlotEntry(&sCurtainSprites[i], 255);
}

/// Fills a 256-byte tile with colour 1 and starts the curtains 48 pixels
/// open.
/// Called by LemurianShip_SetSail as the ship leaves; adds Curtain_Close.
static void Curtain_Start(void) {
  u8 *tiles;

  tiles = Runtime_CallAlternatePool(Runtime_BumpAllocateAlternatePool, 0x100);
  sCurtainVramSlot = Resource_FindFreeEntry();
  Dma_Fill32(0x11111111, tiles, 0x100);
  Engine_VramLoad(sCurtainVramSlot, 0x100, tiles);

  sCurtainGap = 0x30;
  FieldScheduler_AddCallback(Curtain_Close, TASK_PRIORITY_SCENE);
}

/// The ship's glow: ShipGlow_CyclePalette shows six of these colours at a
/// time, from a start that moves one colour every 6 frames.
u16 gShipGlowPalettes[] = {
    0x5860, 0x69c0, 0x7f20, 0x69c0, 0x5860, 0x4800,
    0x5860, 0x69c0, 0x7f20, 0x69c0, 0x5860, 0x0000,
};

/// A drop of spray, at the end of its rise: freed.
s32 gSprayFallScript[] = {
    SC_FREE,
};

// clang-format off
s16 gRippleSine[256] = {
    0, 100, 200, 301, 401, 501, 601, 700,
    799, 897, 995, 1092, 1189, 1284, 1379, 1474,
    1567, 1659, 1751, 1841, 1930, 2018, 2105, 2191,
    2275, 2358, 2439, 2519, 2598, 2675, 2750, 2824,
    2896, 2966, 3034, 3101, 3166, 3229, 3289, 3348,
    3405, 3460, 3513, 3563, 3612, 3658, 3702, 3744,
    3784, 3821, 3856, 3889, 3919, 3947, 3973, 3996,
    4017, 4035, 4051, 4065, 4076, 4084, 4091, 4094,
    4096, 4094, 4091, 4084, 4076, 4065, 4051, 4035,
    4017, 3996, 3973, 3947, 3919, 3889, 3856, 3821,
    3784, 3744, 3702, 3658, 3612, 3563, 3513, 3460,
    3405, 3348, 3289, 3229, 3166, 3101, 3034, 2966,
    2896, 2824, 2750, 2675, 2598, 2519, 2439, 2358,
    2275, 2191, 2105, 2018, 1930, 1841, 1751, 1659,
    1567, 1474, 1379, 1284, 1189, 1092, 995, 897,
    799, 700, 601, 501, 401, 301, 200, 100,
    0, -100, -200, -301, -401, -501, -601, -700,
    -799, -897, -995, -1092, -1189, -1284, -1379, -1474,
    -1567, -1659, -1751, -1841, -1930, -2018, -2105, -2191,
    -2275, -2358, -2439, -2519, -2598, -2675, -2750, -2824,
    -2896, -2966, -3034, -3101, -3166, -3229, -3289, -3348,
    -3405, -3460, -3513, -3563, -3612, -3658, -3702, -3744,
    -3784, -3821, -3856, -3889, -3919, -3947, -3973, -3996,
    -4017, -4035, -4051, -4065, -4076, -4084, -4091, -4094,
    -4096, -4094, -4091, -4084, -4076, -4065, -4051, -4035,
    -4017, -3996, -3973, -3947, -3919, -3889, -3856, -3821,
    -3784, -3744, -3702, -3658, -3612, -3563, -3513, -3460,
    -3405, -3348, -3289, -3229, -3166, -3101, -3034, -2966,
    -2896, -2824, -2750, -2675, -2598, -2519, -2439, -2358,
    -2275, -2191, -2105, -2018, -1930, -1841, -1751, -1659,
    -1567, -1474, -1379, -1284, -1189, -1092, -995, -897,
    -799, -700, -601, -501, -401, -301, -200, -100,
};
// clang-format on

struct SceneEntrance gLemurianShipEntrances[] = {
    {ENTRANCE_DEFAULT, CONDITION_ALWAYS, 408, 0, 200, FACING_NORTH, 0, -1, -1,
     -1, -1, 0},
    {LEMURIAN_SHIP_FROM_LALIVERO_FARANS_HOUSE, CONDITION_ALWAYS, 312, 0, 296,
     FACING_NORTH, 0, -1, -1, -1, 328, 0},
    {SCENE_TABLE_END},
};

u32 gLemurianShipExits[] = {
    SCENE_EXITS(LemurianShip),
    SCENE_EXIT(LEMURIAN_SHIP_TO_LALIVERO_FARANS_HOUSE, LaliveroFaransHouse,
               LALIVERO_FARANS_HOUSE_FROM_LEMURIAN_SHIP),
    SCENE_EXITS_END,
};

struct ScenePlacement gLemurianShipPlacements[] = {
    {GARET, CONDITION_ALWAYS, ACTOR_STAND, 0, 0, 0, FACING_EAST,
     TALK_FACE_PARTY, 0},
    {IVAN, CONDITION_ALWAYS, ACTOR_STAND, 0, 0, 0, FACING_EAST, TALK_FACE_PARTY,
     0},
    {MIA, CONDITION_ALWAYS, ACTOR_STAND, 0, 0, 0, FACING_EAST, TALK_FACE_PARTY,
     0},
    {SPRITE_GANGWAY, CONDITION_ALWAYS, ACTOR_STAND, PIXELS(312), 0, PIXELS(248),
     FACING_SOUTH, TALK_KEEP_FACING, 0},
    {SPRITE_GANGWAY, CONDITION_ALWAYS, ACTOR_STAND, PIXELS(312), 0, PIXELS(312),
     FACING_SOUTH, TALK_KEEP_FACING, 0},
    {SCENE_TABLE_END},
};

struct SceneEvent gLemurianShipEvents[] = {
    {EVENT_TOUCH, 1, CONDITION_ALWAYS, EVENT_SCRIPT(Event_LeaveTheShip)},
    {PSYNERGY_IN_AREA(ABILITY_REVEAL, PSYNERGY_PHASE_EFFECT_ACTS), 0,
     CONDITION_ALWAYS, EVENT_SCRIPT(Ripple_StopForReveal)},
    {PSYNERGY_IN_AREA(ABILITY_REVEAL, PSYNERGY_PHASE_EFFECT_ENDS), 0,
     CONDITION_ALWAYS, EVENT_SCRIPT(Ripple_Start)},
    {USE_ITEM(ITEM_BLACK_ORB), 10, CONDITION_ALWAYS,
     EVENT_SCRIPT(Event_RaiseTheShip)},
    {SCENE_EVENTS_END},
};

s32 gOrbGlowActive = 0;

s32 gOrbGlowPhase = 0;

s32 gOrbGlowStrength = 16;

s32 gOrbGlowFadeFrames = 0;

s32 gSprayActive = 0;

s32 gSwellActive = 0;

s32 gSwellPhase = 0;

s32 gShipHullY = 0;

s32 gShipDeckY = 0;

s32 gLeaderDeckY = SHIP_NOT_ON_DECK;

s32 gGaretDeckY = SHIP_NOT_ON_DECK;

s32 gMiaDeckY = SHIP_NOT_ON_DECK;

s32 gIvanDeckY = SHIP_NOT_ON_DECK;

u32 gShipGlowStep[] = {
    0,
};

s32 gLeaderBoardsShip[] = {
    SC_ARG(2),
    SC_MOVE_BY(PIXELS(-8), 0, 0),
    SC_SYNC,
    SC_MOVE_BY(0, 0, PIXELS(-16)),
    SC_SYNC,
    SC_SET(SW_VELOCITY_Y, 0x80000),
    SC_SE(SE_HIGH_JUMP),
    SC_MOVE_BY(0, 0, PIXELS(-24)),
    SC_SYNC,
    SC_ARG(1),
    SC_WAIT(20),
    SC_ARG(2),
    SC_MOVE_BY(PIXELS(-8), 0, 0),
    SC_SYNC,
    SC_MOVE_BY(0, 0, PIXELS(-56)),
    SC_SYNC,
    SC_ARG(1),
    SC_SET(SW_FACING, FACING_SOUTH),
    SC_WAIT(1),
    SC_CALL(Leader_StepOnDeck),
    SC_END,
};

s32 gGaretBoardsShip[] = {
    SC_ARG(2),
    SC_MOVE_BY(0, 0, PIXELS(-32)),
    SC_SYNC,
    SC_SET(SW_VELOCITY_Y, 0x80000),
    SC_SE(SE_HIGH_JUMP),
    SC_MOVE_BY(0, 0, PIXELS(-24)),
    SC_SYNC,
    SC_ARG(1),
    SC_WAIT(20),
    SC_ARG(2),
    SC_MOVE_BY(PIXELS(-8), 0, 0),
    SC_SYNC,
    SC_MOVE_BY(0, 0, PIXELS(-40)),
    SC_SYNC,
    SC_ARG(1),
    SC_SET(SW_FACING, FACING_SOUTH),
    SC_WAIT(1),
    SC_CALL(Garet_StepOnDeck),
    SC_END,
};

s32 gIvanBoardsShip[] = {
    SC_ARG(2),
    SC_MOVE_BY(PIXELS(-20), 0, 0),
    SC_SYNC,
    SC_MOVE_BY(0, 0, PIXELS(-32)),
    SC_SYNC,
    SC_SET(SW_VELOCITY_Y, 0x80000),
    SC_SE(SE_HIGH_JUMP),
    SC_MOVE_BY(0, 0, PIXELS(-24)),
    SC_SYNC,
    SC_ARG(1),
    SC_WAIT(20),
    SC_ARG(2),
    SC_MOVE_BY(PIXELS(-8), 0, 0),
    SC_SYNC,
    SC_MOVE_BY(0, 0, PIXELS(-24)),
    SC_SYNC,
    SC_ARG(1),
    SC_SET(SW_FACING, FACING_SOUTH),
    SC_WAIT(1),
    SC_CALL(Ivan_StepOnDeck),
    SC_END,
};

s32 gMiaBoardsShip[] = {
    SC_ARG(2),
    SC_MOVE_BY(PIXELS(-8), 0, 0),
    SC_SYNC,
    SC_MOVE_BY(0, 0, PIXELS(-8)),
    SC_SYNC,
    SC_SET(SW_VELOCITY_Y, 0x80000),
    SC_SE(SE_HIGH_JUMP),
    SC_MOVE_BY(0, 0, PIXELS(-24)),
    SC_SYNC,
    SC_ARG(1),
    SC_WAIT(20),
    SC_ARG(2),
    SC_MOVE_BY(PIXELS(-8), 0, 0),
    SC_SYNC,
    SC_MOVE_BY(0, 0, PIXELS(-8)),
    SC_SYNC,
    SC_ARG(1),
    SC_CALL(Mia_StepOnDeck),
    SC_END,
};
