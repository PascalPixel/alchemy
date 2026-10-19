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

#if TBS_PATCH_3
#define LEMURIAN_SHIP_DEPARTURE_DELAY_FRAMES 30
#else
#define LEMURIAN_SHIP_DEPARTURE_DELAY_FRAMES 80
#endif

// The Lemurian ship's hidden harbor at Lalivero, and the end of the game.
//
// The leader steps up with the Black Orb: it glows, spray rises and the
// ancient ship comes up out of the water. The party boards, talks over the
// journey so far and sails; curtains close and the game goes to the clear
// screen.

static void Ripple_Stop(void);
static void Curtain_Start(void);

enum {
  SHIP_NORTH = 8, // the ship, in two pieces of sprite 0x1f6
  SHIP_SOUTH = 9
};

// An entrance the table does not list; arriving by it hands the leader the
// Black Orb.
enum { ENTRANCE_99 = 99 };

// Set before a scene change whose next scene should not start its music.
enum { FLAG_SKIP_SCENE_MUSIC = 0x11a };

// The sprite's two priority bits of a ship piece or a party actor: 9 sets
// the first, any other the second. ACTOR_SPRITE reads actor->sprite through
// an s32, so each store reloads it as the native does; read as the field,
// the two stores share one load.
#define ACTOR_SPRITE(actor)                                                    \
  ((struct AnimationObject *)*(s32 *)((u8 *)(actor) + 0x50))
#define SET_PRIORITY(actor, n)                                                 \
  ((n) == 9 ? (ACTOR_SPRITE(actor)->image.priority = 1)                        \
            : (ACTOR_SPRITE(actor)->shadow.priority = 1))

s32 Math_DivideUnsigned();

// Where the party can enter the harbor.
const struct SceneEntrance *LemurianShip_GetEntrances(void) {
  return gLemurianShipEntrances;
}

// The harbor has no regions.
const struct SceneRegion *LemurianShip_GetRegions(void) { return NULL; }

// Where its one exit leads.
const u32 *LemurianShip_GetExits(void) { return gLemurianShipExits; }

// The ship's two pieces; the party's own actors fill the first three rows.
const struct ScenePlacement *LemurianShip_GetPlacements(void) {
  return gLemurianShipPlacements;
}

// Trigger 1, the way out: leave, clearing the backdrop color.
static void LeaveTheShip(void) {
  Engine_EventRequestExit(gEventWork->touchedTrigger);
  Io_Write16(0, &PLTT_BACKDROP);
}

// Reveal (phase 5) stops the water's ripple; its end (phase 4) starts it
// again.
static void Ripple_StopForReveal(void) { Ripple_Stop(); }

// Starts the two-axis ripple with its original frequencies and amplitudes.
static void Ripple_Start(void) {
  DisplayScroll_InitializeHBlankDma(0, 0x40000, 0x10000, 0x2000, 0x10000,
                                    0x8000, 0x4000);
}

// The exit, Reveal and trigger 10 before the water.
const struct SceneEvent *LemurianShip_GetEvents(void) {
  return gLemurianShipEvents;
}

// Every frame once the ship has risen: steps the ship's glowing colors
// (6 palettes of 6 frames each) into palette 7.
static void ShipGlow_CyclePalette(void) {
  u16 *frame;
  s32 paletteIndex;

  frame = &gShipGlowStep;
  paletteIndex = Math_DivideUnsigned(*frame, 6);
  Dma_Set((const void *)(((u32)(paletteIndex << 16) >> 15) +
                         (u32)gShipGlowPalettes),
          (void *)0x050000e8, (DMA_ENABLE << 16) | 6, REG_DMA3);
  (*frame)++;
  if (*frame > 35)
    *frame = 0;
}

// The update of a drop of spray: it rises and wobbles for its timer, then
// falls back by gSprayFallScript.
static void Spray_Update(union FieldObject *fieldObject) {
  u8 *obj = (u8 *)fieldObject;
  u16 *wait = &((struct FieldActor *)obj)->timer;
  s32 left = *wait;

  if (*(s16 *)wait != 0) {
    s32 drift;

    *wait = left - 1;
    drift = Engine_RandomNext();
    drift -= Engine_RandomNext();
    ((struct FieldActor *)obj)->x.fixed += drift;
    ((struct FieldActor *)obj)->y.fixed += 0xcccc;
  } else if (*(s16 *)&((struct FieldActor *)obj)->work != 0) {
    *(s16 *)&((struct FieldActor *)obj)->work = 0;
    Object_SetMode(obj, 1);
    {
      u16 *p = (u16 *)(obj + 94);
      s32 delay = 20;

      *p = delay;
    }
    Engine_ObjectSetScript(obj, gSprayFallScript);
  }
}

// Every 40 frames, the Black Orb's glow fades a step, down to 4.
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

// The map's scrolling layers, as the swell moves them.
struct WaveMapLayer {
  s32 unk00[3];
  s32 y;
  s32 unk10[8];
};

struct WaveMap {
  u8 unk00[20];
  struct WaveMapLayer layers[8];
};

// Every frame while the ship rises and floats: the Black Orb's glow
// pulses in the blend, the ship (layers 6 and 7) and everyone on deck bob
// with the swell, and spray rises from the water.
static void Ship_UpdateSwell(void) {
  /* FAKEMATCH: volatile restores the form the bytes prove: blend lives at sp+18
   * and is loaded again with ldrh for the register write (a 20-byte frame for
   * 16). Without it differ 24, 223 instructions for 225. */
  volatile u16 blend;
  struct FixedPointPosition place;
  struct FixedPointPosition *pos;
  struct WaveMap *map;
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
    map->layers[6].y = gShipHullY + swell;
    map->layers[7].y = gShipDeckY + swell;
    if (gLeaderDeckY != -0x10000) {
      actor = Object_GetById(0);
      actor->y.fixed = gLeaderDeckY + swell;
      actor->terrainHeight = gLeaderDeckY + swell;
      actor->motionFlags = 0;
    }
    if (gGaretDeckY != -0x10000) {
      actor = Object_GetById(1);
      actor->y.fixed = gGaretDeckY + swell;
      actor->terrainHeight = gLeaderDeckY + swell;
      actor->motionFlags = 0;
    }
    if (gMiaDeckY != -0x10000) {
      actor = Object_GetById(3);
      actor->y.fixed = gMiaDeckY + swell;
      actor->terrainHeight = gLeaderDeckY + swell;
      actor->motionFlags = 0;
    }
    if (gIvanDeckY != -0x10000) {
      actor = Object_GetById(2);
      actor->y.fixed = gIvanDeckY + swell;
      actor->terrainHeight = gLeaderDeckY + swell;
      actor->motionFlags = 0;
    }
    gSwellPhase++;
  }
  if (gSprayActive != 0 && (gFrameCount & 1) != 0) {
    x = map->layers[4].unk10[0] & -0x10000;
    z = map->layers[4].unk10[1] & -0x10000;
    x += Random_Next() * 240;
    pos = &place;
    pos->x = x;
    pos->y = 0;
    z += Random_Next() * 160;
    z += 0x1e0000;
    pos->z = z;
    actor = Object_Create(0x1f7, pos->x, pos->y, pos->z);
    if (actor != 0) {
      actor->update = Spray_Update;
      actor->timer = 60;
      actor->work = 1;
      actor->motionFlags = 0;
      actor->priorityFlags = 2;
      actor->sprite->image.priority = 2;
      Engine_ActorSetSpriteFlags(actor, OBJECT_BLEND_NORMAL);
      Object_SetMode(actor, 0);
    }
  }
}

// Called by each member's boarding script as they step on deck: from then
// on they bob with the ship.
static s32 Leader_StepOnDeck(struct ScriptInterpreter *interpreter) {
  s32 *deckY;
  struct FieldActor *actor;

  deckY = &gLeaderDeckY;
  actor = Object_GetById(ACTOR_PARTY_LEADER);
  *deckY = actor->y.fixed;
  return 0;
}

// Remembers Garet's boarding height for the swell callback.
static s32 Garet_StepOnDeck(struct ScriptInterpreter *interpreter) {
  s32 *deckY;
  struct FieldActor *actor;

  deckY = &gGaretDeckY;
  actor = Object_GetById(GARET);
  *deckY = actor->y.fixed;
  return 0;
}

// Remembers Mia's boarding height for the swell callback.
static s32 Mia_StepOnDeck(struct ScriptInterpreter *interpreter) {
  s32 *deckY;
  struct FieldActor *actor;

  deckY = &gMiaDeckY;
  actor = Object_GetById(MIA);
  *deckY = actor->y.fixed;
  return 0;
}

// Remembers Ivan's boarding height for the swell callback.
static s32 Ivan_StepOnDeck(struct ScriptInterpreter *interpreter) {
  s32 *deckY;
  struct FieldActor *actor;

  deckY = &gIvanDeckY;
  actor = Object_GetById(IVAN);
  *deckY = actor->y.fixed;
  return 0;
}

// After the ship rises: the others step out and talk over the journey
// while three questions shape their replies, everyone boards, and the ship
// sails east as the curtains close and the game goes to the clear screen.
static void SetSail(void) {
  struct MapScrollWork *scroll;
  struct FieldActor *ship;
  struct EventWork *event;

  Engine_EventBegin();
  Camera_MoveTo(-1, -1, -1, 0);
  Map_CopyCellAttributes(18, 0, 3, 1, 18, 12);
  // The others step out and talk about leaving Gondowan.
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
  Engine_ActorSetAnimationAndWait(IVAN, ANIM_NOD);
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
  Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_NOD);
  Engine_ActorSetAnimation(GARET, ANIM_NOD);
  Engine_ActorSetAnimationAndWait(IVAN, ANIM_NOD);
  Engine_EventWait(60);
  Engine_ActorRunRepeatedMotion(GARET, 2);
  Engine_EventWait(30);
  Engine_ActorFaceDirection(GARET, FACING_NORTHEAST, 0);
  Engine_EventWait(20);
  UiText_OpenMessageAtObject(1, 0);
  if (Engine_EventChooseYesNo(ACTOR_PARTY_LEADER, 0) == 0) {
    Engine_EventWait(30);
    Actor_ShowEmote(GARET, EMOTE_IN_FRONT | 2, 50);
    Engine_EventShowMessage(GARET, 0);
    gEventWork->message++;
  } else {
    Engine_EventWait(30);
    Actor_ShowEmote(GARET, EMOTE_IN_FRONT | 2, 50);
    gEventWork->message++;
    Engine_EventShowMessage(GARET, 0);
  }
  Engine_EventWait(10);
  Engine_ActorFaceDirection(IVAN, FACING_WEST, 0);
  Engine_EventWait(30);
  Actor_ShowEmote(IVAN, EMOTE_IN_FRONT | 1, 50);
  Engine_EventShowMessage(IVAN, 0);
  Engine_EventWait(10);
  Engine_ActorFaceDirection(GARET, FACING_EAST, 0);
  Engine_EventWait(30);
  Actor_ShowEmote(GARET, EMOTE_IN_FRONT | 1, 60);
  Engine_ActorRunRepeatedMotion(MIA, 2);
  Engine_EventWait(30);
  Engine_ActorFaceDirection(MIA, FACING_NORTHWEST, 0);
  Engine_EventWait(30);
  Engine_EventShowMessage(MIA, 0);
  Engine_EventWait(10);
  Actor_ShowEmote(GARET, EMOTE_IN_FRONT, 50);
  Engine_EventShowMessage(GARET, 0);
  Engine_EventWait(10);
  Engine_ActorRunRepeatedMotion(IVAN, 2);
  Engine_EventWait(20);
  Engine_EventShowMessage(IVAN, 0);
  Engine_EventWait(10);
  Engine_ActorFaceDirection(MIA, FACING_NORTH, 0);
  Engine_EventWait(20);
  Engine_ActorSetAnimationAndWait(MIA, ANIM_SHAKE_HEAD);
  Engine_EventWait(20);
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
  Engine_ActorShowEmote(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | 2, 50);
  Engine_EventWait(10);
  Actor_FaceDirection(IVAN, FACING_NORTHWEST, 0);
  Engine_EventWait(20);
  Engine_ActorSetAnimationAndWait(IVAN, ANIM_NOD);
  Engine_EventWait(20);
  Engine_EventShowMessage(IVAN, 0);
  Engine_EventWait(20);
  Engine_EventShowMessage(GARET, 0);
  Engine_EventWait(20);
  Engine_ActorSetAnimationAndWait(ACTOR_PARTY_LEADER, ANIM_NOD);
  Engine_EventWait(30);
  Engine_EventWait(10);
  Engine_ActorSetAnimationAndWait(MIA, ANIM_NOD);
  Engine_EventWait(20);
  UiText_OpenMessageAtObject(3, 0);
  Actor_FaceDirection(GARET, FACING_NORTHEAST, 0);
  Actor_FaceDirection(MIA, FACING_NORTH, 0);
  Engine_ActorFaceDirection(IVAN, FACING_NORTHWEST, 0);
  Engine_EventWait(20);
  if (Engine_EventChooseYesNo(ACTOR_PARTY_LEADER, 0) == 0) {
    Engine_EventWait(30);
    Engine_ActorSetAnimationAndWait(MIA, ANIM_NOD);
    Engine_EventWait(20);
    Engine_EventShowMessage(MIA, 0);
    gEventWork->message++;
  } else {
    Engine_EventWait(30);
    Engine_ActorSetAnimationAndWait(MIA, ANIM_NOD);
    Engine_EventWait(20);
    gEventWork->message++;
    Engine_EventShowMessage(MIA, 0);
  }
  Engine_EventWait(10);
  Actor_FaceDirection(GARET, FACING_NORTH, 0);
  Engine_EventWait(30);
  Engine_EventShowMessage(GARET, 0);
  Engine_EventWait(10);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 0);
  Engine_EventWait(30);
  Engine_EventWait(10);
  Actor_FaceDirection(IVAN, FACING_NORTH, 0);
  Engine_EventWait(30);
  Engine_ActorSetAnimationAndWait(IVAN, ANIM_NOD);
  Engine_EventWait(20);
  Engine_EventShowMessage(IVAN, 0);
  Engine_EventWait(10);
  Engine_ActorSetAnimationAndWait(MIA, ANIM_NOD);
  Engine_EventWait(20);
  Engine_EventShowMessage(MIA, 0);
  Engine_AudioPlayCue(MUS_017);
  Engine_EventWait(10);
  Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_NOD);
  Engine_ActorSetAnimation(GARET, ANIM_NOD);
  Engine_ActorSetAnimationAndWait(IVAN, ANIM_NOD);
  Engine_EventWait(40);
  // Everyone boards (gLeaderBoardsShip and the others).
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
  Camera_MoveTo(PIXELS(216), -1, PIXELS(168), 1);
  Engine_CameraWaitForMove();
  Engine_EventWait(20);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_WEST, 0);
  Actor_FaceDirection(GARET, FACING_WEST, 0);
  Actor_FaceDirection(IVAN, FACING_WEST, 0);
  Engine_ActorFaceDirection(MIA, FACING_WEST, 0);
  Engine_EventWait(40);
  Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_NOD);
  Engine_ActorSetAnimation(GARET, ANIM_NOD);
  Engine_ActorSetAnimation(MIA, ANIM_NOD);
  Engine_ActorSetAnimationAndWait(IVAN, ANIM_NOD);
  Engine_EventWait(30);
  // The ship sails east and the curtains close.
  Engine_AudioPlayCue(MUS_067);
  MusicCommand_SetPitchAndUpdateFrequency(240);
  Curtain_Start();
  Engine_EventWait(LEMURIAN_SHIP_DEPARTURE_DELAY_FRAMES);
  scroll = gMapWork[0];
  ship = Object_GetById(SHIP_NORTH);
  ship->acceleration = 131;
  ship->speed = 0x20000;
  scroll->layers[0].speedX = -0x3000;
  ObjectMotion_CommitPositionAndActivate(8, 60, 0);
  scroll->layers[0].speedX = -0x6000;
  ObjectMotion_CommitPositionAndActivate(8, 60, 0);
  Engine_EventWait(80);
  Engine_EventWait(100);
  Engine_CameraSetSpeed(0x23333, 0x28f);
  Engine_CameraMoveTo(PIXELS(808), -1, PIXELS(168), 1);
  Engine_EventWait(300);
  // Fade out to the clear screen, the title's entrance 9.
  event = gMapWork[19];
  event->startTransition = SCENE_TRANSITION(TRANSITION_BACKDROP_FADE, 0);
  Io_Write16(0, &PLTT_BACKDROP);
  event->transitionFrames = 96;
  Engine_EventCloseScreen();
  Engine_EventWaitForScreen();
  Engine_EventWait(30);
  Engine_GameFlagSet(FLAG_SKIP_SCENE_MUSIC);
  Event_SetDestination((s32)&SceneId_Title, 9);
}

// The water's ripple: two pages of per-line scroll offsets, swapped each
// frame, built from a sine table.
struct ScrollWork {
  u16 pages[2][0x3c0];
  u8 page;
  u8 mode;
  u16 phase;
  u8 unkf04[4];
  s32 frequency[2];
  s32 step[2];
  s32 amplitude[2];
};

// One line's ripple offset.
static __inline__ s32 Wave_Offset(s32 angle, s32 amplitude, u16 base) {
  return (u16)(Iwram_MulQ16(gRippleSine[(angle >> 16) & 0xff], amplitude) /
               256) +
         base;
}

// Trigger 10, the leader at the water: the Black Orb glows, the water
// lights up, spray rises and the ship comes up out of the sea 90 pixels,
// then the party sets sail.
static void RaiseTheShip(void) {
  struct WaveMap *map;
  s32 i;
  s32 flash;
  s32 white;
  s32 dark;
  s32 sunk;
  /* FAKEMATCH: volatile restores the form the bytes prove: cnt lives at sp+6
   * and each BGxCNT value is stored there with strh on its way to the register
   * (an 8-byte frame for 4). Without it differ 26, 225 instructions for 231. */
  volatile u16 cnt;

  map = gMapWork[0];

  Event_Begin();
  Battle_ResetEffectCounter();
  // The leader steps to the water's edge; the backdrop brightens 16 steps.
  Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 312, 232);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 0);
  Event_Wait(40);
  Event_PlayCue(SE_140);
  for (i = 0; i <= 15; i++) {
    Io_Write16((i << 11) | (i << 5), &PLTT_BACKDROP);
    Event_Wait(10);
  }
  {
    u16 color = 0x7e00;

    PLTT_BACKDROP = color;
  }
  {
    u16 bright = 0x1010;

    for (flash = 0; flash < 3; flash++) {
      Event_PlayCue(SE_212);
      REG_BLDALPHA = bright;
      Event_Wait(3);
      Io_Write16(0x810, &REG_BLDALPHA);
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
  sunk = 0;
  do {
    map->layers[6].y += 0x3333;
    map->layers[7].y += 0x3333;
    sunk += 0x3333;
    Task_Wait(1);
  } while (sunk <= 0x59ffff);
  Engine_TaskRemoveCallback(OrbGlow_Fade);
  gSprayActive = 0;
  REG_BG3CNT = cnt = (REG_BG3CNT & 0xfffc) | 3;
  REG_BG2CNT = cnt = (REG_BG2CNT & 0xfffc) | 3;
  REG_BG1CNT = cnt = (REG_BG1CNT & 0xfffc) | 2;
  gOrbGlowActive = 0;
  Event_PlayCue(SE_288);
  Task_Wait(1);
  // A flash of white; the swell starts as it clears.
  Event_PlayCue(SE_145);
  Io_Write16(191, &REG_BLDCNT);
  for (white = 0; white <= 16; white++) {
    REG_BLDY = white;
    Event_Wait(1);
  }
  Event_Wait(40);
  Map_SetShake(-1, -1, 0xe666);
  gShipHullY = map->layers[6].y;
  gShipDeckY = map->layers[7].y;
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
  SetSail();
}

// Sets the harbor up: the ship sunk 90 pixels, its pieces and the party
// drawn behind the water layers, and the ripple running.
s32 LemurianShip_SetupScene(void) {
  struct WaveMap *map;
  /* FAKEMATCH: volatile restores the form the bytes prove: cnt lives at sp+2
   * and each step is stored there with strh (a 4-byte frame for none). Without
   * it (2026-10-13, the BGxCNT writes chained as in RaiseTheShip) differ 23,
   * 158 instructions for 165. */
  volatile u16 cnt;

  map = gMapWork[0];
  if (gGameState.entrance == ENTRANCE_99) {
    Inventory_AddItem(ACTOR_PARTY_LEADER, ITEM_BLACK_ORB);
  }
  ((struct EventWork *)gMapWork[19])->startTransition =
      SCENE_TRANSITION(TRANSITION_BACKDROP_FADE, 0);
  Object_GetById(SHIP_NORTH)->collisionFlags = 0;
  Object_GetById(SHIP_NORTH)->priorityFlags = 2;
  Object_GetById(SHIP_SOUTH)->collisionFlags = 0;
  Object_GetById(SHIP_SOUTH)->priorityFlags = 2;
  {
    struct FieldActor *actor = Object_GetById(SHIP_NORTH);

    SET_PRIORITY(actor, 9);
  }
  {
    struct FieldActor *actor = Object_GetById(SHIP_SOUTH);

    SET_PRIORITY(actor, 9);
  }
  {
    struct FieldActor *actor = Object_GetById(ACTOR_PARTY_LEADER);

    SET_PRIORITY(actor, 9);
    SET_PRIORITY(actor, 21);
  }
  {
    struct FieldActor *actor = Object_GetById(GARET);

    SET_PRIORITY(actor, 9);
    SET_PRIORITY(actor, 21);
  }
  {
    struct FieldActor *actor = Object_GetById(IVAN);

    SET_PRIORITY(actor, 9);
    SET_PRIORITY(actor, 21);
  }
  {
    struct FieldActor *actor = Object_GetById(MIA);

    SET_PRIORITY(actor, 9);
    SET_PRIORITY(actor, 21);
  }
  REG_BG3CNT = cnt = (REG_BG3CNT & 0xfffc) | 2;
  REG_BG2CNT = cnt = (REG_BG2CNT & 0xfffc) | 3;
  REG_BG1CNT = cnt = (REG_BG1CNT & 0xfffc) | 3;
  Io_Write16(0x2648, &REG_BLDCNT);
  Io_Write16(0x810, &REG_BLDALPHA);
  {
    struct WaveMapLayer *layer = map->layers + 6;

    layer->y += -0x5a0000;
    layer = map->layers + 7;
    layer->y += -0x5a0000;
  }
  Engine_MapRedraw();
  Ripple_Start();
  return 0;
}

// Each HBlank: copies the next line of the current ripple page into the
// background 3 scroll registers by DMA 0.
void DisplayScroll_ArmHBlankDma(void) {
  struct ScrollWork *work;
  u32 *source;
  volatile u16 *dma0;
  volatile u32 *destination;

  work = gHBlankScrollWork;
  source = (u32 *)work->pages[work->page];

  dma0 = REG_DMA0;
  destination = (volatile u32 *)&REG_BG3HOFS;
  dma0[5] &= ~DMA_START_TIMING_DRQ_REPEAT;
  dma0[5] &= ~DMA_ENABLE;
  dma0[5];

  *destination = *source++;
  Dma_Set(source, destination, DMA_HBLANK_WORD_PER_LINE, (volatile u32 *)dma0);
}

// Each frame: builds the other ripple page and swaps it in.
void DisplayScroll_BuildAndSwapHBlankPage(void) {
  struct ScrollWork *work;
  u16 *line;
  s32 angle;
  u16 base;
  s32 amplitude;
  s32 step;
  u16 y;
  s32 i;

  work = gHBlankScrollWork;
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

// Shared scene code, included where the ROM places it.
#include "common/scroll_initialize_hblank_dma.h"

// Takes the ripple's callbacks off and stops DMA 0.
static void Ripple_Stop(void) {
  volatile u16 *reg;

  Engine_TaskRemoveCallback(DisplayScroll_ArmHBlankDma);
  Engine_TaskRemoveCallback(DisplayScroll_BuildAndSwapHBlankPage);
  reg = REG_DMA0;
  reg[5] = ~DMA_START_TIMING_DRQ_REPEAT & reg[5];
  reg[5] = ~DMA_ENABLE & reg[5];
  reg[5];
}

// The curtains: three columns of eight 32 by 8 sprites that close in from
// the sides as the ship sails.
struct CurtainSprite {
  u32 unused;
  u32 attributes;
  u32 tile;
};

static struct CurtainSprite sCurtainSprites[24];
static s16 sCurtainGap;
static s16 sCurtainVramSlot;

// Every frame: moves the curtains a step closer.
static void Curtain_Close(void) {
  u32 *entry;
  u32 tile;
  s32 step;
  s32 x;
  u32 i;

  tile = gVramBlockCache[sCurtainVramSlot].offset >> 5;
  entry = (u32 *)sCurtainSprites;
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
  for (i = 0; i < 24; i++)
    Runtime_PushSlotEntry(&sCurtainSprites[i], 255);
}

// Fills a 256-byte tile with color 1 and starts the curtains 48 pixels
// open.
static void Curtain_Start(void) {
  u8 *tiles;

  tiles = Runtime_CallAlternatePool(Runtime_BumpAllocateAlternatePool, 0x100);
  sCurtainVramSlot = Resource_FindFreeEntry();
  Dma_Fill32(0x11111111, tiles, 0x100);
  Engine_VramLoad(sCurtainVramSlot, 0x100, (s32)tiles);

  sCurtainGap = 0x30;
  FieldScheduler_AddCallback(Curtain_Close, 0xc80);
}

struct SceneEvent gShipGlowPalettes[] = {
    {0x69c05860, 0x7f20, 0x69c0, 0x48005860},
    {0x69c05860, 0x7f20, 0x69c0, 0x5860},
};

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
    {0, CONDITION_ALWAYS, 408, 0, 200, FACING_NORTH, 0, -1, -1, -1, -1, 0},
    {1, CONDITION_ALWAYS, 312, 0, 296, FACING_NORTH, 0, -1, -1, -1, 328, 0},
    {SCENE_TABLE_END},
};

u32 gLemurianShipExits[] = {
    SCENE_EXITS(188),
    SCENE_EXIT(1, 180, 31),
    SCENE_EXITS_END,
};

struct ScenePlacement gLemurianShipPlacements[] = {
    {GARET, CONDITION_ALWAYS, ACTOR_STAND, 0, 0, 0, FACING_EAST,
     TALK_FACE_PARTY, 0},
    {IVAN, CONDITION_ALWAYS, ACTOR_STAND, 0, 0, 0, FACING_EAST, TALK_FACE_PARTY,
     0},
    {MIA, CONDITION_ALWAYS, ACTOR_STAND, 0, 0, 0, FACING_EAST, TALK_FACE_PARTY,
     0},
    {0x1f6, CONDITION_ALWAYS, ACTOR_STAND, PIXELS(312), 0, PIXELS(248),
     FACING_SOUTH, TALK_KEEP_FACING, 0},
    {0x1f6, CONDITION_ALWAYS, ACTOR_STAND, PIXELS(312), 0, PIXELS(312),
     FACING_SOUTH, TALK_KEEP_FACING, 0},
    {SCENE_TABLE_END},
};

struct SceneEvent gLemurianShipEvents[] = {
    {EVENT_TOUCH, 1, CONDITION_ALWAYS, EVENT_SCRIPT(LeaveTheShip)},
    {PSYNERGY_IN_AREA(ABILITY_REVEAL, 5), 0, CONDITION_ALWAYS,
     EVENT_SCRIPT(Ripple_StopForReveal)},
    {PSYNERGY_IN_AREA(ABILITY_REVEAL, 4), 0, CONDITION_ALWAYS,
     EVENT_SCRIPT(Ripple_Start)},
    {0xf204, 10, CONDITION_ALWAYS, EVENT_SCRIPT(RaiseTheShip)},
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

s32 gLeaderDeckY = -0x10000;

s32 gGaretDeckY = -0x10000;

s32 gMiaDeckY = -0x10000;

s32 gIvanDeckY = -0x10000;

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
    SC_SE(153),
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
    SC_SE(153),
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
    SC_SE(153),
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
    SC_SE(153),
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
