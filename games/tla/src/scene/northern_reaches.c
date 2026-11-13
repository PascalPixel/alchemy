/// @file
/// The Northern Reaches, where an ice wall blocks the ship's way to Prox until
/// Felix fires the Magma Ball at it from a safe distance.

#include "shared/tables.h"
#include "battle.h"
#include "field.h"
#include "game.h"
#include "graphics.h"
#include "lib.h"
#include "menu.h"
#include "sound.h"
#include "system.h"
#include "types.h"
#include "scene_tables.h"
#include "actor.h"
#include "main_routine_work.h"
#include "ram_buffer.h"
#include "event_work.h"
#include "text/msg_ids.h"
#include "event_inline.h"
#include "system/dma.h"
#include "gamepak/far_call/system.h"
#include "item_icon.h"
#include "module/scene/northern_reaches.h"
#include "module/scene/scene.h"
#include "render_input.h"
#include "effect_sprite.h"
#include "resource.h"

/// The Magma Ball's ice shards and the explosion's sparks, as the ice wall
/// shatters (NorthernReaches_ShatterIceWall).
struct IceShard {
  s32 x;
  s32 y;
  s32 z;
  s32 velocityX;
  s32 velocityY;
  s32 velocityZ;
  u32 age;
};

struct IceShardSpark {
  struct IceShardSpark *next; ///< The sprite list's link.
  union SparkOam oam;
  s32 timer;
};

struct IceWallWork {
  u8 sheet[0x1000];
  struct EffectSprite sprites[54];
  struct IceShard particles[54];
};

s32 NorthernReaches_ResetAnimationFrame(s32 arg0) {
  *(u8 *)((u8 *)*(s32 *)((u8 *)arg0 + 80) + 26) = 0;
  *(u8 *)((u8 *)(arg0 + 34) + 0) = 1;
  return 1;
}

void Event_Start();
void Event_End();
void FieldCamera_WaitForArrival();

const struct SceneEntrance *Scene_GetEntrances(void) { return gSceneEntrances; }

const struct SceneRegion *Scene_GetRegions(void) { return 0; }

const u32 *Scene_GetExits(void) { return gSceneExits; }

const struct ActorPlacement *Scene_GetPlacements(void) {
  return gActorPlacements;
}

/// Draws the ice wall gone: its pictures and its cells, as the Magma Ball
/// leaves it and on every later visit (flag 0x90e).
void NorthernReaches_ClearIceWall(void) {
  MapGrid_CopyCellsLayered(47, 9, 9, 4, 25, 9);
  MapGrid_CopyCellsLayered(47, 69, 9, 10, 25, 69);
  MapGrid_CopyCellAttributes(111, 8, 9, 5, 89, 8);
  return;
}

/// The first time the ship nears the ice wall, once a visit, the crew calls
/// Kraden to see it.
/// "Kraden, come look at this... There's a huge ice wall blocking our path..."
void NorthernReaches_SightIceWall(void) {
  s32 result0;
  struct Actor *object1;

  result0 = Event_GetFlag(0x200);
  if (result0 != 0)
    goto done;
  Event_SetFlag(0x200);
  Event_Start();
  FieldEvent_ResolvePendingActions(0);
  FieldEvent_SetMessageId((s32)MsgShipIceWallFirstApproach);
  object1 = Actor_Get(8);
  FieldActor_WaitForMove((s32)object1);
  Actor_FaceDirectionAndWait(8, FACING_NORTH);
  FieldEvent_Wait(5);
  FieldEvent_ShowMessageThenWait(5, 0, 5);
  FieldEvent_ShowMessageThenWait(11, 0, 5);
  Event_End();
done:
  return;
}

/// The crew gathers at the bow to talk the ice wall over, once (flag 0x90c).
void NorthernReaches_DiscussIceWall(void) {
  s32 result0;
  struct Actor *object1;

  result0 = Event_GetFlag(0x90c);
  if (result0 != 0)
    goto done;
  Event_SetFlag(0x90c);
  Event_Start();
  FieldEvent_ResolvePendingActions(0);
  FieldEvent_SetMessageId((s32)MsgShipIceWallCrewReposition);
  object1 = Actor_Get(8);
  FieldActor_WaitForMove((s32)object1);
  FieldEvent_Wait(5);
  FieldEvent_ShowMessageThenWait(5, 0, 5);
  FieldCamera_MoveTo(PIXELS(464), -1, PIXELS(176), 1);
  FieldCamera_WaitForArrival();
  FieldEvent_Wait(5);
  Actor_SetPosition(11, PIXELS(408), PIXELS(288));
  Actor_SetPosition(5, PIXELS(408), PIXELS(288));
  Actor_SetPosition(6, PIXELS(408), PIXELS(288));
  FieldEvent_ShowMessageThenWait(11, 0, 5);
  FieldEvent_ShowMessageThenWait(6, 0, 5);
  FieldEvent_ShowMessageThenWait(11, 0, 5);
  Actor_SetPosition(11, 0, 0);
  Actor_SetPosition(5, 0, 0);
  Actor_SetPosition(6, 0, 0);
  Event_End();
done:
  return;
}

#define FIELD_AT(base, type, off)                                              \
  (((struct {                                                                  \
     u8 pad[off];                                                              \
     type v;                                                                   \
   } *)(base))                                                                 \
       ->v)
s32 Actor_SetSpriteColorMode();

/// At a safe distance the crew asks Felix to fire the Magma Ball at the ice
/// wall; if he is ready the ship backs off, fires, and the wall shatters (flag
/// 0x90e).
/// "We should be safe if we fire from this distance."
void NorthernReaches_FireMagmaBall(void) {
  s32 from[3];
  s32 to[3];
  s32 v_r5, v_r8, v_r6, v_r7, v_r10, t0, t1, t2, t3, t10;
  s32 t11, t12, t13, t14, t16, t18, t20, t26, t31, t36;
  s32 t37, t40, t41, t45, t52, t53, t59, t60, t61, t66;
  s32 t70, t79, t82, t85, t88, t91, t96;

  t0 = (s32)Actor_Get(9);
  t1 = (s32)Actor_Get(10);
  t2 = (s32)Actor_Get(8);
  t3 = Event_GetFlag(0x90e);
  v_r7 = t2;
  v_r6 = t0;
  v_r8 = t1;
  v_r5 = t3;
  if (v_r5 == 0)
    goto fire;
  goto done;
fire:
  Event_Start();
  FieldEvent_ResolvePendingActions(0);
  FieldEvent_SetMessageId((s32)MsgShipSafeFiringDistance);
  Actor_MoveToAndWait(8, 0x1d0, 216);
  Actor_FaceDirectionAndWait(8, 0xc000);
  FieldEvent_Wait(5);
  t10 = (s32)Actor_Get(11);
  FIELD_AT(t10, s32, 24) = v_r5;
  t11 = (s32)Actor_Get(7);
  FIELD_AT(t11, s32, 24) = v_r5;
  t12 = (s32)Actor_Get(5);
  FIELD_AT(t12, s32, 24) = v_r5;
  t13 = (s32)Actor_Get(6);
  FIELD_AT(t13, s32, 24) = v_r5;
  t14 = (s32)Actor_Get(11);
  Actor_SetSpriteFlags(t14, 0);
  t16 = (s32)Actor_Get(7);
  Actor_SetSpriteFlags(t16, 0);
  t18 = (s32)Actor_Get(5);
  Actor_SetSpriteFlags(t18, 0);
  t20 = (s32)Actor_Get(6);
  Actor_SetSpriteFlags(t20, 0);
  Actor_SetPosition(11, 0x1d00000, 0xd80000);
  Actor_SetPosition(7, 0x1d00000, 0xd80000);
  Actor_SetPosition(5, 0x1d00000, 0xd80000);
  Actor_SetPosition(6, 0x1d00000, 0xd80000);
  t26 = Event_GetFlag(0x90d);
  v_r10 = (s32)MsgShipSafeFiringDistance;
  if (t26 != 0)
    goto ask;
  Event_SetFlag(0x90d);
  FieldEvent_ShowMessageThenWait(0x2007, 0, 5);
ask:
  FieldEvent_SetMessageId((v_r10 + 1));
  Event_OpenMessage(11, 0);
  t31 = FieldEvent_AskYesNoWithActorAnimation(4, 0);
  if (t31 == 0)
    goto ready;
  FieldEvent_ShowMessageThenWait(11, 0, 5);
  Actor_SetDestinationOffset(8, 0, 16);
  FieldActor_WaitForMove(v_r7);
  FieldEvent_Wait(5);
  goto putAway;
ready:
  t36 = (s32)Ram_HeapSlots->eventWork;
  t37 = FIELD_AT(t36, u16, 452);
  FIELD_AT(t36, u16, 452) = (t37 + 1);
  FieldEvent_ShowMessageThenWait(11, 0, 5);
  Actor_SetSpeed(8, 0x30000, 0x18000);
  t40 = (s32)Actor_Get(8);
  FIELD_AT(t40, u8, 90) &= 254;
  Actor_WalkBy(8, 0, 16);
  Event_PlaySound(144);
  FieldEvent_Wait(4);
  FIELD_AT(v_r8, s32, 24) = 0x18000;
  FIELD_AT(v_r8, s32, 28) = 0x18000;
  t45 = (s32)Actor_Get(10);
  Actor_SetSpriteColorMode(t45, 6);
  Actor_SetPosition(10, 0x1d00000, 0xc80000);
  Actor_SetAnimation(10, 1);
  FIELD_AT(v_r6, s32, 24) = 0x8000;
  FIELD_AT(v_r6, s32, 28) = 0x8000;
  Actor_SetSpeed(9, 0x20000, 0x10000);
  Actor_SetPosition(9, 0x1d00000, 0xd80000);
  Actor_WalkTo(9, 0x1d0, 144);
  v_r5 = 15;
growBall:
  t52 = FIELD_AT(v_r6, s32, 24);
  FIELD_AT(v_r6, s32, 24) = (t52 + 0x800);
  t53 = FIELD_AT(v_r6, s32, 28);
  FIELD_AT(v_r6, s32, 28) = (t53 + 0x800);
  System_Wait(1);
  v_r5 = (v_r5 - 1);
  if (v_r5 >= 0)
    goto growBall;
  FieldCamera_MoveTo(0x1d80000, -1, 0xb00000, 1);
  FieldCamera_WaitForArrival();
  FieldActor_WaitForMoveById(9);
  Event_PlaySound(145);
  t59 = FIELD_AT(v_r6, s32, 8);
  from[0] = (t59 - 0x20000);
  t60 = FIELD_AT(v_r6, s32, 12);
  from[1] = t60;
  t61 = FIELD_AT(v_r6, s32, 16);
  from[2] = (t61 - 0x80000);
  to[0] = 0x1900000;
  to[1] = 0;
  to[2] = 0x500000;
  NorthernReaches_ShatterIceWall((s32)from, to, NorthernReaches_ClearIceWall);
  FieldEvent_Wait(20);
  Actor_SetPosition(10, 0, 0);
  Actor_SetPosition(9, 0, 0);
  t66 = (s32)Ram_HeapSlots->eventWork;
  FIELD_AT(t66, s32, 436) = 100;
  PaletteFade_ApplyTarget(0x10000, 0);
  Event_StartScreenTransition();
  Event_WaitTransitionFrames();
  t70 = (s32)Ram_HeapSlots->eventWork;
  FIELD_AT(t70, s32, 436) = 8;
  FieldEvent_ShowMessageThenWait(0x2005, 0, 5);
  FieldEvent_ShowMessageThenWait(11, 0, 5);
  FieldEvent_ShowMessageThenWait(0x2006, 0, 5);
  Actor_SetSpeed(8, 0xcccc, 0x6666);
  Actor_MoveToAndWait(8, 0x1d0, 176);
  EventCamera_FollowActor(8, 1);
  Actor_MoveToAndWait(8, 0x1d0, 152);
  Actor_SetSpeed(8, 0x9999, 0x4ccc);
  t79 = FIELD_AT(v_r7, s32, 28);
  FIELD_AT(v_r7, s32, 28) = (t79 - 0x1000);
  FIELD_AT(v_r7, s32, 24) = (t79 - 0x1000);
  Actor_SetDestinationOffset(8, 0, -16);
  FieldActor_WaitForMoveById(8);
  t82 = FIELD_AT(v_r7, s32, 28);
  FIELD_AT(v_r7, s32, 28) = (t82 - 0x1000);
  FIELD_AT(v_r7, s32, 24) = (t82 - 0x1000);
  Actor_SetDestinationOffset(8, 0, -16);
  FieldActor_WaitForMoveById(8);
  t85 = FIELD_AT(v_r7, s32, 28);
  FIELD_AT(v_r7, s32, 28) = (t85 - 0x1000);
  FIELD_AT(v_r7, s32, 24) = (t85 - 0x1000);
  Actor_SetDestinationOffset(8, 0, -16);
  FieldActor_WaitForMoveById(8);
  t88 = FIELD_AT(v_r7, s32, 28);
  FIELD_AT(v_r7, s32, 28) = (t88 - 0x1000);
  FIELD_AT(v_r7, s32, 24) = (t88 - 0x1000);
  Actor_SetDestinationOffset(8, 0, -16);
  FieldActor_WaitForMoveById(8);
  t91 = FIELD_AT(v_r7, s32, 28);
  FIELD_AT(v_r7, s32, 28) = (t91 - 0x1000);
  FIELD_AT(v_r7, s32, 24) = (t91 - 0x1000);
  Actor_SetDestinationOffset(8, 0, -16);
  FieldActor_WaitForMoveById(8);
  Actor_SetSpeed(8, 0x3333, 0x1999);
  Actor_SetDestinationOffset(8, 0, -12);
  v_r5 = 19;
settle:
  t96 = FIELD_AT(v_r7, s32, 28);
  FIELD_AT(v_r7, s32, 28) = (t96 - 0x900);
  FIELD_AT(v_r7, s32, 24) = (t96 - 0x900);
  System_Wait(7);
  v_r5 = (v_r5 - 1);
  if (v_r5 >= 0)
    goto settle;
  FIELD_AT(v_r7, s32, 28) = 0;
  FIELD_AT(v_r7, s32, 24) = 0;
  FieldEvent_Wait(10);
  Event_SetFlag(0x90e);
  FieldEvent_SetExitRequest(4);
putAway:
  Actor_SetPosition(11, 0, 0);
  Actor_SetPosition(5, 0, 0);
  Actor_SetPosition(6, 0, 0);
  Actor_SetPosition(7, 0, 0);
  Event_End();
done:
  return;
}

const struct Trigger *Scene_GetEvents(void) { return gSceneEvents; }

struct SceneWork {
  u8 unk000[0x1ac];
  s32 unk1ac;
};

s32 Scene_Initialize(void) {
  struct MainRoutineWork *state;

  ((struct SceneWork *)Ram_HeapSlots->eventWork)->unk1ac = 256;
  state = &gMainRoutineWork;
  Actor_Get(state->currentOwner)->priorityFlags |= 32;
  if (state->entrance == 90) {
    Event_SetFlag(0x90a);
    state->ownerOverride = 1;
    Event_SetDestination((s32)&SceneId_Ov6a7Room1, 4);
  }
  RisingParticles_Start();
  return 0;
}

s32 Scene_PrepareMap(void) {
  if (gMainRoutineWork.entrance == 3)
    NorthernReaches_ClearIceWall();
  if (Event_GetFlag(0x90e))
    NorthernReaches_ClearIceWall();
  return 0;
}

void BattleEffect_StepVector3(struct IceShard *particle, s32 a, s32 b);

/// The Magma Ball's flight and the wall's ice shards
/// (ResourceId_FieldEffectIceShard); `callback` draws the wall gone.
void NorthernReaches_ShatterIceWall(s32 unused, s32 *position,
                                    void (*callback)(void)) {
  struct IceWallWork *work;
  struct IceShard *particle;
  struct EffectSprite *sprite;
  struct EffectSprite *flySprite;
  struct IceShardSpark *sparks;
  struct IceShardSpark *spark;
  s32 *timer;
  // Unused: gives the stack frame its 4 bytes.
  u8 frameSlot[4];
  void *sheet;
  s32 i, frame, column, zero, entry, sparkEntry, tiles, flying, sparkTiles;

  work = HeapSlot_Alloc(220, 0x1e58);
  flying = 0;
  Decompress_TaggedLz(
      Resource_GetTableEntry((s32)&ResourceId_FieldEffectIceShard), work);
  entry = VramCache_FindFreeEntry();
  tiles = VramCache_Load(entry, 0x500, work);
  i = 0;
  particle = work->particles;
  sprite = work->sprites;
  for (; i <= 53; i++) {
    EffectSprite_Init(sprite, 0, 0, 0x40000000, (i & 3) * 8 + tiles);
    zero = 0;
    sprite->depth = 250;
    sprite->oam.priority = zero;
    sprite->oam.palette256 = 1;
    sprite->oam.palette = zero;
    column = i / 6;
    particle->x = position[0] + (column << 20);
    particle->y = position[1] - ((i % 6) << 20);
    column -= 4;
    sprite++;
    particle->z = position[2];
    particle->velocityX = (Random16() * 3 >> 1) * column;
    particle->velocityY = (Random16() << 2) + 0x20000;
    particle->velocityZ = zero;
    particle->age = zero;
    particle++;
  }
  sparks = Heap_AllocEwram(0x600);
  /* FAKEMATCH(temporary): the size held in the local that then takes the
   * buffer; as Heap_Alloc(0x1000), 19 of 323 instructions differ in tbs-en. */
  sheet = (void *)0x1000;
  sheet = Heap_Alloc((s32)sheet);
  Decompress_TaggedLz(
      Resource_GetTableEntry((s32)&ResourceId_FieldEffectExplosion), sheet);
  sparkEntry = VramCache_FindFreeEntry();
  sparkTiles = VramCache_Load(sparkEntry, 0x400, sheet);
  Heap_Release(sheet);
  timer = &sparks->timer;
  for (i = 95; i >= 0; i--) {
    *timer = -(Random16() << 5 >> 16);
    timer += 4;
  }
  Dma_Set(Field_Palette685, (void *)0x50003e0, 0x80000010,
          (volatile u32 *)0x40000d4);
  for (frame = 0; frame <= 149; frame++) {
    if ((frame & 31) == 0) {
      Event_PlaySound(145);
    }
    if (frame == 60) {
      BattleEffect_ApplyColorToTargetBuffer(0x7fff, 0);
      PaletteFade_StartInterpolation(90);
      Event_PlaySound(141);
    }
    if (frame == 105) {
      callback();
      flying = 1;
      MapGrid_SetShake(0x60000, 0x60000, 0x10000);
      Event_PlaySound(228);
    }
    if (frame == 130) {
      flying = 1;
    }
    if (flying != 0) {
      particle = work->particles;
      flySprite = work->sprites;
      for (i = 53; i >= 0; i--) {
        if (particle->age < 60) {
          EffectSprite_DrawAtWorldPosition(flySprite, particle);
          BattleEffect_StepVector3(particle, 63, -0x3000);
          particle->age++;
        }
        flySprite++;
        particle++;
      }
    }
    spark = sparks;
    do {
      if (spark->timer == 0) {
        spark->oam.words.first = 0x40000000;
        spark->oam.words.second = 0;
        spark->oam.oam.x = (Random16() << 7 >> 16) + 52;
        spark->oam.oam.y = (Random16() << 6 >> 16) + 16;
        spark->oam.oam.palette = 15;
        spark->oam.oam.priority = 0;
      }
      if (spark->timer >= 0) {
        spark->oam.oam.index = sparkTiles + (spark->timer >> 2) * 4;
        Oam_AddSpriteAtDepthClamped(spark, 100);
      }
      spark->timer++;
      if (spark->timer > 31) {
        spark->timer = -(Random16() * 7 >> 16);
      }
      spark++;
    } while ((s32)spark <= (s32)&sparks[95]);
    wait(1);
  }
  VramCache_FreeEntry(sparkEntry);
  Heap_Release(sparks);
  FieldEvent_Wait(40);
  MapGrid_SetShake(0, 0, 0);
  VramCache_FreeEntry(entry);
  HeapSlot_Release(220);
}

s32 gNorthernReaches_ShipAtRest[] = {
    46, (s32)NorthernReaches_ResetAnimationFrame, 39, 7, 17,
};

struct SceneEntrance gSceneEntrances[] = {
    {0, CONDITION_ALWAYS, 440, 0, 520, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {2, CONDITION_ALWAYS, 470, 0, 192, FACING_NORTH, 0, -1, -1, -1, -1, 0},
    {3, CONDITION_ALWAYS, 470, 0, 192, FACING_NORTH, 0, -1, -1, -1, -1, 0},
    {SCENE_TABLE_END},
};

DEFINE_G_SCENE_EXITS_5(268, 60, 4, 61)

struct ActorPlacement gActorPlacements[] = {
    {8, CONDITION_ALWAYS, (s32)gNorthernReaches_ShipAtRest, PIXELS(120), 0,
     PIXELS(280), FACING_SOUTH, 2, 0},
    {263, CONDITION_ALWAYS, 1, -0x380000, 0, -0x380000, FACING_SOUTH, 2, 0},
    {285, CONDITION_ALWAYS, 1, -0x380000, 0, -0x380000, FACING_SOUTH, 2, 0},
    {56, CONDITION_ALWAYS, 1, -0x380000, 0, -0x380000, FACING_EAST, 2, 0},
    {5, CONDITION_ALWAYS, 1, -0x380000, 0, -0x380000, FACING_NORTHEAST, 2, 0},
    {6, CONDITION_ALWAYS, 1, -0x380000, 0, -0x380000, FACING_NORTHEAST, 2, 0},
    {7, CONDITION_ALWAYS, 1, -0x380000, 0, -0x380000, FACING_NORTHEAST, 2, 0},
    {SCENE_TABLE_END},
};

struct Trigger gSceneEvents[] = {
    {TRIGGER_EXIT, 1, CONDITION_ALWAYS, 1},
    {TRIGGER_EXIT, 4, CONDITION_ALWAYS, 4},
    {TRIGGER_STEP, 20, 0x190a, EVENT(NorthernReaches_DiscussIceWall)},
    {TRIGGER_STEP, 21, 0x190a, EVENT(NorthernReaches_FireMagmaBall)},
    {TRIGGER_STEP, 22, 0x90a, EVENT(NorthernReaches_SightIceWall)},
    {TRIGGER_TABLE_END},
};
