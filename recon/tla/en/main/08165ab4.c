// Draft, differ 78 of 804: BattleEffect_RunRamses, summon 1 (BattleEffect_RunSceneEffect's case 1,
// actflags 0x8001), The Broken Seal's BattleEffect_RunRamses.

// Twin: ☀️ BattleEffect_RunRamses
// (games/tbs/src/effect/effect.c), ported: Func_0813ba50 sets the display,
// the six rock objects are resource 653 at objects 9 to 14 (☀️ 390), the
// formation is resource 648 (☀️ 379), the blitters come from
// BattleEffect_LoadBlitter(104, 19) and (188, 3), the effect stays an argument for the
// hit loop, callback BattleEffect_ProcessPendingGraphicsTransfer at 0xc80.

// Left: the 78 differing rows are only reordered (as a multiset they are the
// native's): loads and stores ahead of independent constants, and the literal
// pools that shift with them, the kind Func_08164b2c's header traces to a
// load costing 3 and a store 2.
#include "types.h"
#include "ram_buffer.h"
#include "io_reg.h"
#include "system.h"
#include "fixed_math.h"
#include "callback_scheduler.h"
#include "input.h"
#include "battle_effect.h"
#include "battle_effect_work.h"

// The pixel half of a 16.16 fixed-point value.
#define HI(v) (((s16 *)&(v))[1])

// The heap blocks the dispatcher hands every canvas effect, as byte offsets
// into Ram_HeapSlots (☀️ uses slot indices).
#define HEAP_BLITTER 104
#define HEAP_BLITTER_ALTERNATE 188

// BattleEffect_LoadBlitter's argument: The Lost Age's blitter mode (☀️'s counted down
// from 3) in the high nibble, the COMPOSITE_ flags in the low one.
#define BLITTER(mode, flags) ((mode) << 4 | (flags))
#define BLIT_ADDITIVE 0
#define BLIT_MAX 1

// The map cell buffer holds the sparks of the final blow.
#define SPARKS ((struct EffectStep *)Ram_MapCellBuffer)
#define SPARK_COUNT 512

// The objects: the formation's seven, the orb's two halves, six rocks.
#define FORMATION 648
#define FORMATION_SIZE 9
#define ORB_LEFT 7
#define ORB_RIGHT 8
#define ROCK_OBJECT 653
#define FIRST_ROCK 9
#define ROCK_COUNT 6
#define PUFF_COUNT 58 // particles 6 to 63

#define SCENE_FRAMES 320
#define CIRCLE_FRAMES 90  // the orb circles, then drops three times
#define FIRST_DROP 91     // each drop 40 frames after the last
#define ROCKS_FALL 96     // the rocks start falling
#define WIND_UP_FRAME 244 // the orb draws back, then flies left
#define THROW_FRAME 252
#define HIT_FRAME 260

#define PHASE_SUMMON_RESULTS 134
#define SE_DROP 156
#define SE_HIT 145

struct FxAnimObject {
  u8 unk00[9];
  u8 unk09_0 : 2;
  u8 priority : 2;
  u8 unk09_4 : 4;
  u8 unk0a[16];
  u8 flags;
};

struct FxScale {
  s32 x;
  s32 y;
};

struct ScaledSprite;

extern char ResourceId_DesertBackdrop;
extern char ResourceId_Data134; // ☀️ ParticleSpritesA
extern char ResourceId_BlastSheet;
extern struct FxScale gData_08196e6c; // one to one
extern u8 gData_08198a64[];           // ☀️ gRamses_ObjectColumns
extern u8 gData_08198a6d[];           // ☀️ gRamses_ObjectRows
extern u16 gData_08198a76[];          // ☀️ gEruption_PuffCells
extern u16 gData_08198a84[];          // ☀️ gCybele_EmberSizes
extern u16 gData_08197410[];          // ☀️ gBattleEffect_MoteCells

void BattleEffect_BeginCanvasLayer(s32 bgControl);
void Func_0813ba50(void); // BattleEffect_ConfigureEffectDisplay
void BattleEffect_ProcessPendingGraphicsTransfer(void); // BattleEffect_ProcessPendingGraphicsTransfer
void Func_08163c2c(s32 mode, s32 value);
void BattleEffect_SelectLivingTargets(
    struct BattleEffectArgument *effect); // BattleEffect_SelectLivingTargets
void BattleEffect_SpawnObjects(s32 count, s32 resource,
                   s32 priority); // BattleEffect_SpawnObjects
void *ResourceObject_CreateFar(s32 resource);
void ResourceObject_ReleaseFar(void *object);
void Animation_ApplyChildArgumentFar(void *object, s32 animation);
void BattleEffect_LoadBlitter(s32 slot, s32 modeAndFlags); // BattleEffect_BuildBlitter(slot,
                                                // 7, 7, flags, mode)
void BattleBackground_LoadFar(s32 layer, s32 resourceId, s32 mode);
void Audio_PlayCue(s32 cue);
void Render_ApplyProjectedPlacementFar(struct ScaledSprite *sprite,
                                       s32 *position, s32 *scale, s32 mode);
void BattleEffectKernels_IntegrateVector2(struct EffectStep *step, s32 damping,
                                      s32 gravity);
void Func_08118088(s32 actor,
                   s32 variant); // BattleMotion_ApplyVariantMotionFar
void Func_0814cd48(s32 actor, s32 palette, s32 a, s32 timer,
                   s32 frames);   // ObjectGroup_UpdateMembers
void Func_08158ce0(s32 x, s32 y); // Camera_ApplyShake
void ObjectGroup_TickMemberTimers(void);         // ObjectGroup_TickMemberTimers
void Func_081180e8(s32 phase);    // BattleEventRuntime_BeginPhaseFar
void Func_0814cca8(void);         // BattleEffect_SetupBlendedDisplay
void BattleEffect_EndCanvasLayer(void);         // BattleEffect_EndCanvasLayer

// Summon 1: the battlefield turns to desert; an orb circles over a standing
// formation and slams down three times, raising dust, while six rocks rain
// and bounce; then the orb draws back and is hurled at the enemies, bursting
// into 512 sparks.
//
// Setup: open the canvas, spawn the formation and the six rocks, build the
// blitters, set the windows, load the desert backdrop, the dust and spark
// pictures, and drop the rocks from staggered heights. Each frame (320, or
// until A or B): place the formation, the orb and the rocks; the orb circles
// for 90 frames, then drops at frames 91, 131 and 171 (dust puffs on each
// landing) and rises again; the rocks fall from frame 96 and bounce once in
// dust; the orb draws back at frame 244 and flies left from 252; at frame 260
// the targets are struck and the sparks burst. At the end the results phase
// begins and every object is released.
void BattleEffect_RunRamses(struct BattleEffectArgument *effect) {
  s32 position[4];
  DrawRectangle draw[2];
  struct FxScale scale;
  void *canvas;
  struct BattleEffectWork *work;
  s32 frame;
  u8 *cells;
  struct FxScale base;
  struct FxScale orb;
  s32 i;
  s32 k;

  canvas = Ram_HeapSlots->battleFxWork[1];
  work = (struct BattleEffectWork *)Ram_HeapSlots->battleFxWork[0];
  cells = Ram_HeapSlots->unk64[0];

  // Open the canvas over a black backdrop and spawn the objects.
  BattleEffect_BeginCanvasLayer(0);
  Func_0813ba50();
  *(u16 *)BG_PLTT = 0;
  *(u16 *)(BG_PLTT + 1) = 0;
  work->transferMode = 0;
  Scheduler_AddOrUpdateCallback(BattleEffect_ProcessPendingGraphicsTransfer, 0xc80);
  Func_08163c2c(1, 0);
  BattleEffect_SelectLivingTargets(work->effect);
  BattleEffect_SpawnObjects(FORMATION_SIZE, FORMATION, 2);
  for (i = 0; i != ROCK_COUNT; i++) {
    struct FxAnimObject *rock = ResourceObject_CreateFar(ROCK_OBJECT);

    work->objects[FIRST_ROCK + i] = rock;
    if (rock != 0) {
      rock->flags = 0;
      Animation_ApplyChildArgumentFar(rock, i % 3);
      ((struct FxAnimObject *)work->objects[FIRST_ROCK + i])->priority = 1;
    }
  }
  BattleEffect_LoadBlitter(HEAP_BLITTER,
                BLITTER(BLIT_MAX, COMPOSITE_CLIP_X | COMPOSITE_CLIP_Y));
  draw[0] = (DrawRectangle)Ram_HeapSlots->unk64[1];
  BattleEffect_LoadBlitter(HEAP_BLITTER_ALTERNATE,
                BLITTER(BLIT_ADDITIVE, COMPOSITE_CLIP_X | COMPOSITE_CLIP_Y));
  draw[1] = (DrawRectangle)Ram_HeapSlots->unkb8[1];
  REG_WININ = 0x2737;
  REG_WIN0H = 0xf0;
  REG_WIN1V = 0x1088;
  WaitFrames(1);

  // The desert, the dust and spark pictures, and the blend.
  BattleBackground_LoadFar(1, (s32)&ResourceId_DesertBackdrop, 0);
  Func_08163c2c(1, 1);
  Resource_LoadAndDecompress((s32)&ResourceId_Data134, cells, 0, 0);
  Resource_LoadAndDecompress((s32)&ResourceId_BlastSheet, work->sheet, 1, 1);
  REG_DISPCNT = 0x7741;
  REG_BG2PA = 0x80;
  REG_BLDALPHA = 0x1010;
  REG_BLDCNT = 0x3f44;
  work->transferMode = 2;
  work->transferValue = 50;

  // The formation's corner and the orb's start; the rocks wait above the
  // screen one after another; every puff and spark is idle.
  base.x = 0xbc0000;
  base.y = 0x5c0000;
  orb.x = 0xa00000;
  orb.y = 0x5c0000;
  for (i = 0; i != ROCK_COUNT; i++) {
    struct EffectStep *rock = &work->particles[i];

    rock->x = (Random16() & 127) << 16;
    rock->y = i * -0x100000;
    rock->velocityX = 0;
    rock->velocityY = 0;
    rock->variant = 0;
  }
  for (i = 0; i != PUFF_COUNT; i++)
    work->particles[ROCK_COUNT + i].variant = 24;
  for (i = 0; i != 1024; i++)
    SPARKS[i].variant = -1;
  work->fadeFrames = 24;
  work->fadeStep = 0;

  for (frame = 0; frame != SCENE_FRAMES && !(gInput.repeat & 3); frame++) {
    if (frame == FIRST_DROP + 3)
      Audio_PlayCue(SE_DROP);
    if (frame == FIRST_DROP + 45)
      Audio_PlayCue(SE_DROP);
    if (frame == FIRST_DROP + 87)
      Audio_PlayCue(SE_DROP);
    if (frame == HIT_FRAME)
      Audio_PlayCue(SE_HIT);
    scale = gData_08196e6c;
    if (frame >= ROCKS_FALL && frame <= 251)
      work->shakeFrames = 1;
    else if (frame >= HIT_FRAME && frame <= HIT_FRAME + 3)
      work->shakeFrames = 1;

    // The formation, a 3 x 3 block behind the orb.
    position[3] = 0;
    position[1] = 0;
    for (i = 0; i != 7; i++) {
      position[0] = (gData_08198a64[i] << 16) + base.x - 0x200000;
      position[2] = (gData_08198a6d[i] << 16) + base.y - 0x200000;
      Render_ApplyProjectedPlacementFar(work->objects[i], position, &scale.x,
                                        0);
    }

    // The orb circles, then drops three times: down 8 pixels a frame for
    // four frames, a puff of dust, and back up 2 a frame 20 frames later.
    if (frame <= CIRCLE_FRAMES) {
      orb.x = Trig_Sin(frame << 9) * 16 + 0x9c0000;
      orb.y = Trig_Cos(frame << 9) * 16 + 0x5c0000;
    }
    if (frame <= 196) {
      s32 start;

      for (i = 0, start = FIRST_DROP; i != 3; start += 40, i++) {
        if (frame >= start && frame < start + 4)
          orb.y += 0x80000;
        if (frame == start + 3) {
          for (k = 0; k != 4; k++) {
            struct EffectStep *puff = &work->particles[ROCK_COUNT + i * 8 + k];

            puff->x = 0x400000;
            puff->y = 0x600000;
            puff->velocityX = ((Random16() & 255) - 127) << 10;
            puff->velocityY = ((Random16() & 255) - 127) << 10;
            puff->variant = Random16() & 15;
          }
        }
        if (frame >= start + 20 && frame < start + 36)
          orb.y -= 0x20000;
      }
    }

    // The throw: back a pixel a frame, then left ever faster.
    if (frame >= WIND_UP_FRAME && frame < THROW_FRAME)
      orb.x -= 0x10000;
    if (frame >= THROW_FRAME && frame <= 275)
      orb.x -= (frame - 250) << 16;
    if (frame < HIT_FRAME) {
      position[0] = orb.x;
      position[1] = -0x1000000;
      position[2] = orb.y - 0x1000000;
      Render_ApplyProjectedPlacementFar(work->objects[ORB_LEFT], position,
                                        &scale.x, 0);
      position[0] = orb.x + 0x200000;
      Render_ApplyProjectedPlacementFar(work->objects[ORB_RIGHT], position,
                                        &scale.x, 0);
    }

    // The rocks fall, bounce once at y = 120 in two puffs of dust, and are
    // dropped again until frame 199.
    position[1] = 0;
    for (i = 0; i != ROCK_COUNT; i++) {
      struct EffectStep *rock = &work->particles[i];

      if (rock->variant != 2) {
        position[0] = rock->x;
        position[2] = rock->y;
        Render_ApplyProjectedPlacementFar(work->objects[FIRST_ROCK + i],
                                          position, &scale.x, 0);
        rock->x += rock->velocityX;
        rock->y += rock->velocityY;
        if (frame > ROCKS_FALL)
          rock->velocityY += 0x4000;
        if (rock->y > 0x780000) {
          rock->variant++;
          if (rock->variant == 1) {
            rock->velocityY = -rock->velocityY / 2;
            for (k = 0; k != 2; k++) {
              struct EffectStep *puff = &work->particles[30 + i * 2 + k];

              puff->x = rock->x / 2;
              puff->y = rock->y - 0x200000;
              puff->velocityX = ((Random16() & 255) - 127) << 10;
              puff->velocityY = ((Random16() & 255) - 127) << 10;
              puff->variant = Random16() & 15;
            }
          } else if (frame <= 199) {
            rock->y = 0;
            rock->velocityY = 0;
            rock->variant = 0;
          }
        }
      }
    }

    // The dust: four pictures, six frames each, drifting up.
    for (i = 0; i != 56; i++) {
      struct EffectStep *puff = &work->particles[ROCK_COUNT + i];

      if (puff->variant >= 0) {
        if (puff->variant >= 0 && puff->variant <= 23) {
          s32 cell = puff->variant / 6 + 3;

          draw[0](canvas, work->sheet + gData_08198a76[cell],
                  HI(puff->x) - gData_08198a84[cell] / 2,
                  HI(puff->y) - gData_08198a84[cell] / 2, gData_08198a84[cell],
                  gData_08198a84[cell]);
        }
        BattleEffectKernels_IntegrateVector2(puff, 60, -0x4000);
        puff->variant++;
      }
    }

    // The blow: every target struck and flashed, and the sparks burst from
    // (32, 92).
    if (frame == HIT_FRAME) {
      for (i = 0; i != effect->count; i++) {
        Func_08118088(effect->actors[i], 4);
        Func_0814cd48(effect->actors[i], 7, -1, i, 8);
      }
      work->shakeFrames = 8;
    }
    if (frame == HIT_FRAME) {
      for (i = 0; i != SPARK_COUNT; i++) {
        struct EffectStep *spark = &SPARKS[i];
        s32 speed;
        s32 angle;

        speed = 0x3ff;
        speed &= Random16();
        angle = Random16() & 0xffff;
        spark->x = 0x200000;
        spark->y = 0x5c0000;
        spark->velocityX = Trig_Sin(angle) * (speed + 32) >> 7;
        spark->velocityY = -(Trig_Cos(angle) * (speed + 32) << 1) >> 7;
        spark->variant = (Random16() & 15) + 32;
      }
    }
    for (i = 0; i != SPARK_COUNT; i++) {
      struct EffectStep *spark = &SPARKS[i];

      if (spark->variant >= 0) {
        s32 size = (spark->variant >> 3) + 1;

        draw[i & 1](canvas, cells + gData_08197410[size - 1],
                    HI(spark->x) - size / 2, HI(spark->y) - size, size,
                    size * 2);
        BattleEffectKernels_IntegrateVector2(spark, 62, 0x1000);
        spark->variant--;
      }
    }
    Func_08158ce0(8, 8);
    ObjectGroup_TickMemberTimers();
    work->transferPending = 1;
    WaitFrames(1);
  }

  Func_081180e8(PHASE_SUMMON_RESULTS);
  Func_0814cca8();
  for (i = 0; i != FIRST_ROCK + ROCK_COUNT; i++)
    ResourceObject_ReleaseFar(work->objects[i]);
  Scheduler_RemoveCallback(BattleEffect_ProcessPendingGraphicsTransfer);
  Runtime_ReleaseHeapBlock(HEAP_BLITTER_ALTERNATE);
  Runtime_ReleaseHeapBlock(HEAP_BLITTER);
  BattleEffect_EndCanvasLayer();
}
