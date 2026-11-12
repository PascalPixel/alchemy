#include "types.h"
#include "system.h"
#include "motobj.h"
#include "battle_action.h"
#include "battle_unit.h"
#include "actor.h"

void *Unit_Get(s32 owner);

/// BattleEffect_StartActorMotion with fixed speeds and a random distance:
/// unless the unit is one of four enemies, the actor leaps back to 130-193% of
/// its anchor's x, then takes mode 5.
void BattleEffect_StartActorMotionRandomDistance(s32 id) {
  struct BattleObj *slot;
  struct MotionObject *object;
  u32 enemy;

  slot = GetBattleObjectSlotFar(id);
  object = slot->object;
  enemy = ((struct BattleUnit *)Unit_Get(id))->enemyId;
  if (enemy != 357 && enemy != 369 && enemy != 373 && enemy != 376) {
    s32 percent = (Random16() & 63) + 130;

    object->acceleration = 0x80000;
    object->speedLimit = 0xa0000;
    object->velocityY = 0x18000;
    Actor_ResetMotion(object);
    Actor_SetTargetPosition(object, slot->anchorX * percent / 100, 0,
                            slot->anchorZ);
  }
  object->verticalMotionStrength = 0x9999;
  object->verticalMotionPhase = 0;
  object->autoFaceMotion = 0;
  Actor_SetMode(object, 5);
}

// Unused: a second copy of Nereid's summon effect.

#include "ram_buffer.h"
#include "io_reg.h"
#include "dma.h"

#include "fixed_math.h"
#include "task.h"
#include "resource.h"
#include "input.h"

#include "battle_effect.h"
#include "battle_effect_work.h"

struct FxPartyList;

// The pixel half of a 16.16 fixed-point value.
#define HI(v) (((s16 *)&(v))[1])

// The heap blocks the dispatcher hands every canvas effect, as byte offsets
// into Ram_HeapSlots (☀️ uses slot indices).
#define HEAP_BLITTER 104
#define HEAP_BLITTER_ALTERNATE 188

// The map cell buffer holds the embers.
#define EMBERS ((struct EffectStep *)Ram_MapCellBuffer)
#define EMBER_COUNT 1024
#define PILLAR_SHEET 0x6e4 // the fire pillar after the spray in the sheet

#define FORMATION 649
#define FORMATION_SIZE 12

#define SCENE_FRAMES 124
#define SKIP_FROM_FRAME 32 // A or B jumps from here...
#define SKIP_TO_FRAME 98   // ...to here
#define FIRST_SPOUT_FRAME 28
#define SPOUT_FRAME 32 // the embers spout and the pillars rise until frame 79
#define SPOUT_END_FRAME 80
#define RESULTS_FRAME 120

#define PHASE_SUMMON_RESULTS 134
#define SE_SUMMON 164
#define SE_SPOUT 145
#define SE_FADE 144

extern char ResourceId_WaterSpraySheet;
extern char ResourceId_FirePillarSheetA;
extern char ResourceId_Data134;    // ☀️ ParticleSpritesA
extern u8 gData_0819896a[];        // the pillars' feet, x and y
extern u16 gData_08197410[];       // ☀️ gBattleEffect_MoteCells
extern const s32 gData_08198974[]; // ☀️ gNereid_Gravity

void BattleEffect_StartCanvasLayer(s32 bgControl);
void BattleEffect_ProcessPendingGraphicsTransfer(void);
void BattleEffect_FetchRectangleBlitters(s32 flip, void **out);
void BattleEffect_SelectLivingTargets(struct FxPartyList *party);
void BattleEffect_SpawnObjects(s32 entryCount, s32 kind, u32 variant);
void Func_081180e8(s32 phase); // BattlePlayback_StartFar
void Func_0814cd48(s32 actor, s32 palette, s32 a, s32 timer,
                   s32 frames); // BattleEffect_TintBattler
void Func_0816442c(s32 formation, s32 x,
                   s32 y); // BattleEffect_PlaceFormationObjects
void Func_0816467c(s32 formation, s32 x,
                   s32 y); // BattleEffect_RunImpactBurst
void BattleEffect_EndCanvasLayer(void);

struct Point2D {
  s32 x;
  s32 y;
};

/// Unused: nothing reaches it (no handler, summon case or call). A second
/// copy of The Broken Seal's BattleEffect_RunNereid, which this game keeps
/// for summon 2 as BattleEffect_RunNereid. As written:
/// a formation of twelve sweeps in from the right; three pillars of fire
/// stream upward and a fountain of embers spouts from the ground, falls, and
/// at the end rises away, while the targets jolt every twelve frames.
///
/// Setup: open the canvas, load the spray and pillar pictures and the ember
/// cells, empty the particle pools, and spawn the formation. Each frame (124,
/// A or B skipping from 33 to 98): the formation glides in and slows; 32
/// embers spout at frame 28 and 16 more a frame from 32 to 79; the spray rolls
/// along the ground until frame 95; the pillars stream up from 32 to 79; each
/// ember grows as it falls (the brighter blitter once it falls) and grows
/// again toward the end; the targets flash and jump every 12 frames from 30 to
/// 79. At the end the formation bursts and its objects are released.
UNUSED void Func_08163e14(struct BattleEffectArgument *effect) {
  DrawRectangle draw[2];
  void *canvas;
  struct BattleEffectWork *work;
  u8 *cells;
  u8 *palette;

  canvas = Ram_HeapSlots->battleFxWork[1];
  work = (struct BattleEffectWork *)Ram_HeapSlots->battleFxWork[0];
  cells = Ram_HeapSlots->unk64[0];
  BattleEffect_StartCanvasLayer(0);
  REG_BLDALPHA = 0x1010;
  BattleEffect_FetchRectangleBlitters(0, draw);

  // The spray's palette and picture, the pillar's picture, the ember cells.
  palette = Resource_GetTableEntry((s32)&ResourceId_WaterSpraySheet);
  Dma_Set(palette, (void *)BG_PLTT, 0x84000020, REG_DMA3);
  palette += 128;
  Resource_DecodeType01(palette, work->sheet);
  palette = Resource_GetTableEntry((s32)&ResourceId_FirePillarSheetA);
  palette += 128;
  Resource_DecodeType01(palette, work->sheet + PILLAR_SHEET);
  Resource_DecodeType01(Resource_GetTableEntry((s32)&ResourceId_Data134),
                        cells);
  work->transferMode = 2;
  work->transferValue = 75;
  Task_Add(BattleEffect_ProcessPendingGraphicsTransfer, TASK_PASS_FRAME);
  {
    struct Point2D focus;
    struct Point2D velocity;
    s32 rise;
    s32 frame;
    s32 k;
    struct EffectStep *point;
    struct EffectStep *ember;
    struct EffectStep *embers;
    s32 *life;

    // The formation starts off the right edge, gliding left.
    rise = 0;
    focus.x = 256 << 16;
    focus.y = 88 << 16;
    velocity.x = -(16 << 16);
    velocity.y = -(4 << 16);
    for (k = 0; k != 64; k++)
      work->particles[k].variant = -1;
    for (k = 0; k != 16; k++) {
      point = &work->particles[24 + k];
      point->x = Random16() & 127;
      point->y = (Random16() & 7) + 56;
      point->variant = -(Random16() & 15);
    }
    life = &EMBERS->variant;
    for (k = 0; k != EMBER_COUNT; k++) {
      *life = -1;
      life += sizeof(struct EffectStep) / sizeof(s32);
    }
    embers = EMBERS;
    BattleEffect_SelectLivingTargets(work->effect);
    wait(1);
    BattleEffect_SpawnObjects(FORMATION_SIZE, FORMATION, 2);

    for (frame = 0; frame != SCENE_FRAMES; frame++) {
      if ((gInput.repeat & 3) && frame > SKIP_FROM_FRAME &&
          frame < SKIP_TO_FRAME)
        frame = SKIP_TO_FRAME;
      if (frame == RESULTS_FRAME)
        Func_081180e8(PHASE_SUMMON_RESULTS);
      if (frame <= 15)
        rise += 2;

      // The formation glides in, slowing, and drifts back right as it
      // passes x = 120.
      if (frame <= 99) {
        focus.x += velocity.x;
        focus.y += velocity.y;
        velocity.x = velocity.x * 58 / 64;
        velocity.y = velocity.y * 56 / 64;
        if (focus.x < 120 << 16)
          velocity.x += 0x8000;
      }
      Func_0816442c(1, focus.x, focus.y);

      // The first spout fills the first 256 slots; then 16 embers a frame
      // from the ground at (32, 96).
      if (frame == FIRST_SPOUT_FRAME) {
        for (k = 0; k != 256; k++) {
          ember = &embers[k];
          if (ember->variant == -1) {
            s32 radius = Random16() & 63;
            s32 angle = Random16() & 0xffff;

            ember->x = ((Trig_Sin(angle) * radius) >> 3) + (32 << 16);
            ember->y = ((Trig_Cos(angle) * radius) >> 2) + (96 << 16);
            ember->velocityX = ((Random16() & 63) - 32) << 14;
            ember->velocityY = (-(Random16() & 63) - 8) << 13;
            ember->variant = 0;
          }
        }
      }
      if (frame >= SPOUT_FRAME && frame < SPOUT_END_FRAME) {
        s32 spawned = 0;

        for (k = 0; k != EMBER_COUNT; k++) {
          ember = &embers[k];
          if (ember->variant == -1) {
            s32 radius = Random16() & 63;
            s32 angle = Random16() & 0xffff;

            ember->x = ((Trig_Sin(angle) * radius) >> 3) + (32 << 16);
            ember->y = ((Trig_Cos(angle) * radius) >> 2) + (96 << 16);
            ember->velocityX = ((Random16() & 63) - 32) << 14;
            ember->velocityY = (-(Random16() & 63) - 8) << 13;
            ember->variant = 0;
            spawned++;
            if (spawned == 16)
              break;
          }
        }
      }
      if (frame == 0)
        Sound_PlayFar(SE_SUMMON);
      if (frame == SPOUT_FRAME)
        Sound_PlayFar(SE_SPOUT);
      if (frame == SPOUT_END_FRAME)
        Sound_PlayFar(SE_FADE);

      // The pillars stream upward, each a 104-pixel picture drawn twice to
      // wrap.
      if (frame >= SPOUT_FRAME && frame < SPOUT_END_FRAME) {
        for (k = 0; k != 3; k++) {
          s32 span = (frame * 16 - 256 + k * 25) % 104;

          draw[0](canvas, work->sheet + PILLAR_SHEET,
                  gData_0819896a[k * 2] - 17,
                  gData_0819896a[k * 2 + 1] - span - 104, 34, 104);
          draw[0](canvas, work->sheet + PILLAR_SHEET,
                  gData_0819896a[k * 2] - 17, gData_0819896a[k * 2 + 1] - span,
                  34, span);
        }
      }

      // The spray rolls along the ground, rising for the first 16 frames.
      if (frame <= 95) {
        for (k = 0; k != 5; k++)
          draw[0](canvas, work->sheet, k * 32 + ((frame / 4) & 31) - 32,
                  120 - rise, 32, 32);
      }

      // The embers: bigger when falling and toward the end, pulled down by
      // one of four gravities, then up after frame 80; gone below y = 104.
      for (k = 0; k != EMBER_COUNT; k++) {
        ember = &embers[k];
        if (ember->variant >= 0) {
          s32 size = k % 3 + 2;

          if (ember->velocityY > 0)
            size += 2;
          if (frame > 68 && size <= 5)
            size = 6;
          if (frame > 70 && size <= 6)
            size = 7;
          if (frame > 72 && size <= 7)
            size = 8;
          if (frame > 74 && size <= 8)
            size = 9;
          if (frame > 76)
            size = 10;
          draw[ember->velocityY > 0](canvas, cells + gData_08197410[size - 1],
                                     HI(ember->x) - size / 2,
                                     HI(ember->y) - size, size, size * 2);
          ember->x += ember->velocityX;
          ember->y += ember->velocityY;
          if (frame > SPOUT_END_FRAME)
            ember->velocityY += -0x8000;
          else
            ember->velocityY += gData_08198974[k & 3];
          ember->velocityX = ember->velocityX * 62 / 64;
          ember->velocityY = ember->velocityY * 62 / 64;
          ember->variant++;
          if (ember->velocityY > 0 && HI(ember->y) > 104)
            ember->variant = -1;
        }
      }

      // The targets flash and jump every 12 frames while the embers spout.
      if (frame < SPOUT_END_FRAME) {
        for (k = 0; k != effect->count; k++) {
          if (frame > 29) {
            s32 phase = frame % 12;

            if (phase == 0) {
              struct MotionObject *target =
                  GetBattleObjectSlotFar(effect->actors[k])->object;

              Func_0814cd48(effect->actors[k], 7, 5, -1, 0);
              target->velocityY = 0x48000;
              target->verticalMotionStrength = 0xab85;
            }
            if (phase == 6)
              Func_0814cd48(effect->actors[k], 0, 5, -1, 0);
          }
        }
      }
      work->transferPending = 1;
      wait(1);
    }

    Task_Remove(BattleEffect_ProcessPendingGraphicsTransfer);
    HeapSlot_Release(HEAP_BLITTER_ALTERNATE);
    HeapSlot_Release(HEAP_BLITTER);
    Func_0816467c(1, focus.x, focus.y);
    for (k = 0; k != FORMATION_SIZE; k++)
      ResourceObject_ReleaseFar(work->objects[k]);
  }
  BattleEffect_EndCanvasLayer();
}

#include "battle_fx_work.h"

struct FormationScale {
  s32 x;
  s32 y;
};

struct ScaledSprite;

extern const struct FormationScale gData_08196e5c;
extern const struct FormationScale gData_08196e64;
extern const u8 gData_08198984[];
extern const u8 gData_08198990[];
extern const u8 gData_0819899c[];
extern const u8 gData_081989a4[];
extern const u8 gData_081989ac[];
extern const u8 gData_081989b4[];
extern const u8 gData_081989bc[];
extern const u8 gData_081989c4[];
extern const s8 gData_081989cc[];
extern const u8 gData_081989d7[];

void Render_ApplyProjectedPlacementFar(struct ScaledSprite *sprite,
                                       s32 *position, s32 *scale, s32 mode);

/// The Broken Seal's BattleEffect_PlaceFormationObjects with The Lost Age's
/// three more formations (4: eleven objects from signed columns, 5: twelve in
/// a 4x3 grid, 6: eight more), in the order 0, 5, 1, 2, 6, 3, 4.
void Func_0816442c(s32 formation, s32 x, s32 z) {
  struct BattleFxWork *work =
      (struct BattleFxWork *)Ram_HeapSlots->battleFxWork[0];
  struct FormationScale normal = gData_08196e5c;
  struct FormationScale big = gData_08196e64;
  s32 pos[4];
  s32 i;
  void **objects;

  pos[3] = 0;
  pos[1] = 255 << 16;
  switch (formation) {
  case 0:
    objects = work->objects;
    for (i = 0; i != 9; i++) {
      pos[0] = (i % 3 << 21) + x;
      pos[2] = (i / 3 << 21) + z;
      Render_ApplyProjectedPlacementFar(*objects++, pos, &normal.x, 0);
    }
    break;
  case 5:
    for (i = 0; i != 12; i++) {
      pos[0] = (i % 4 << 21) + x;
      pos[2] = (i / 4 << 21) + z;
      Render_ApplyProjectedPlacementFar(work->objects[i], pos, &normal.x, 0);
    }
    break;
  case 1:
    for (i = 0; i != 12; i++) {
      pos[0] = (gData_08198984[i] << 16) + x - (16 << 16);
      pos[2] = (gData_08198990[i] << 16) + z - (32 << 16);
      Render_ApplyProjectedPlacementFar(work->objects[i], pos, &normal.x, 0);
    }
    break;
  case 2:
    for (i = 0; i != 8; i++) {
      pos[0] = (gData_0819899c[i] << 16) + x + (16 << 16);
      pos[2] = (gData_081989a4[i] << 16) + z;
      Render_ApplyProjectedPlacementFar(work->objects[i], pos, &normal.x, 0);
    }
    break;
  case 6:
    for (i = 0; i != 8; i++) {
      pos[0] = (gData_081989ac[i] << 16) + x;
      pos[2] = (gData_081989b4[i] << 16) + z;
      Render_ApplyProjectedPlacementFar(work->objects[i], pos, &normal.x, 0);
    }
    break;
  case 3:
    for (i = 0; i != 8; i++) {
      pos[0] = (gData_081989bc[i] << 16) + x;
      pos[2] = (gData_081989c4[i] << 16) + z;
      Render_ApplyProjectedPlacementFar(work->objects[i], pos, &big.x, 0);
    }
    break;
  case 4:
    for (i = 0; i != 11; i++) {
      pos[0] = (gData_081989cc[i] << 16) + x;
      pos[2] = (gData_081989d7[i] << 16) + z;
      Render_ApplyProjectedPlacementFar(work->objects[i], pos, &normal.x, 0);
    }
    break;
  }
}

#include "heap_release.h"

#include "iwram_call.h"

#define SHARD_E 0x59d8

extern char ResourceId_FlareSheet;
extern char ResourceId_FlareImage;
extern u16 gData_081989f2[];
extern u8 gData_081989e2[];
extern u8 gData_081989ea[];

s32 BattleEffect_LoadBlitter(s32 slot, s32 kind);
void BattleEffectKernels_IntegrateVector2(struct EffectStep *step, s32 damping,
                                          s32 gravity);

static __inline__ s32 ClearWords_(void *destination, s32 size) {
  return Iwram_ClearWords(destination, size);
}

void Func_0816467c(s32 formation, s32 x, s32 y) {
  struct BattleFxWork *work;
  void *canvas;
  DrawRectangle additive;
  DrawRectangle maxBlend;
  u8 *sheet;
  s32 orgX;
  struct EffectStep *p;
  s32 frame;
  s32 i;

  work = (struct BattleFxWork *)Ram_HeapSlots->battleFxWork[0];
  canvas = Ram_HeapSlots->battleFxWork[1];
  sheet = Ram_HeapSlots->unk64[0];
  orgX = x;
  x += 40 << 16;
  x /= 2;
  REG_BG2PA = 0x80;
  REG_BG2X = 0;
  REG_BLDCNT = 0x3f46;
  BattleEffect_LoadBlitter(104, 19);
  maxBlend = (DrawRectangle)Ram_HeapSlots->unk64[1];
  BattleEffect_LoadBlitter(188, 3);
  additive = (DrawRectangle)Ram_HeapSlots->unkb8[1];
  Resource_LoadAndDecompress((s32)&ResourceId_Data134, sheet, 0, 0);
  Resource_LoadAndDecompress((s32)&ResourceId_FlareSheet, work->sheet, 1, 0);
  Resource_LoadAndDecompress((s32)&ResourceId_FlareImage, work->sheet + SHARD_E,
                             0, 0);
  work->transferMode = 2;
  work->transferValue = 50;
  Task_Add(BattleEffect_ProcessPendingGraphicsTransfer, TASK_PASS_FRAME);

  for (i = 0; i != 64; i++) {
    s32 speed = (Random16() & 255) + 256;
    s32 angle = Random16() & 0xffff;

    work->particles[i].x = x;
    work->particles[i].y = y;
    work->particles[i].velocityX = (Trig_Sin(angle) * speed) >> 7;
    work->particles[i].velocityY = -((Trig_Cos(angle) * speed) >> 6);
    work->particles[i].variant = (Random16() & 15) + 16;
  }

  for (i = 0; i != 3; i++) {
    work->particles[61 + i].x = x;
    work->particles[61 + i].y = y;
    work->particles[61 + i].velocityX = (Trig_Sin(i * 0x5555) * 32) >> 6;
    work->particles[61 + i].velocityY = -((Trig_Cos(i * 0x5555) * 32) >> 5);
  }

  for (i = 0; i != 64; i++) {
    s32 speed = (Random16() & 255) + 32;
    s32 angle = Random16() & 0xffff;
    struct EffectStep *cells = (struct EffectStep *)Ram_MapCellBuffer;

    cells[i].x = x;
    cells[i].y = y;
    cells[i].velocityX = (Trig_Sin(angle) * speed) >> 6;
    cells[i].velocityY = -((Trig_Cos(angle) * speed) >> 5);
    cells[i].variant = (Random16() & 15) + 20;
  }

  for (frame = 0; frame != 72; frame++) {
    if (frame == 4)
      Sound_PlayFar(154);
    if (frame == 32)
      Sound_PlayFar(212);

    if (frame <= 47) {
      s32 cell = (frame - 8) / 5;
      u32 width;
      u32 height;

      if (cell < 0)
        cell = 0;
      maxBlend(canvas, work->sheet + gData_081989f2[cell],
               (x >> 16) - ((width = gData_081989e2[cell]) >> 1),
               (y >> 16) - ((height = gData_081989ea[cell]) >> 1), width,
               height);
    }

    for (i = 0, p = work->particles; i != 30; i++, p++) {
      if (frame > i / 2 && p->variant > 0) {
        s32 size;
        s32 px;
        s32 py;

        p->variant--;
        BattleEffectKernels_IntegrateVector2(p, 60, 0);
        size = p->variant / 16 + 3;
        px = HI(p->x);
        py = HI(p->y);
        additive(canvas, sheet + gData_08197410[size - 1], px - size / 2,
                 py - size, size, size * 2);
      }
    }

    for (i = 0, p = (struct EffectStep *)Ram_MapCellBuffer; i != 60; i++, p++) {
      if (frame > 35 && p->variant > 0) {
        s32 size;
        s32 px;
        s32 py;

        p->variant--;
        BattleEffectKernels_IntegrateVector2(p, 60, 0);
        size = p->variant / 16 + 1;
        px = HI(p->x);
        py = HI(p->y);
        additive(canvas, sheet + gData_08197410[size - 1], px - size / 2,
                 py - size, size, size * 2);
      }
    }

    for (i = 0; i != 3; i++) {
      struct EffectStep *shard = &work->particles[61 + i];

      if (frame >= 36 && frame <= 63) {
        s32 cell;
        s32 px;
        s32 py;

        BattleEffectKernels_IntegrateVector2(shard, 64, 0);
        cell = (frame - 36) / 7;
        px = HI(shard->x);
        py = HI(shard->y);
        maxBlend(canvas, work->sheet + cell * 0x120 + SHARD_E, px - 6, py - 12,
                 12, 24);
      }
    }

    if (frame <= 35)
      Func_0816442c(formation, orgX, y);
    work->transferPending = 1;
    wait(1);
  }

  Task_Remove(BattleEffect_ProcessPendingGraphicsTransfer);
  /* FAKEMATCH(wrapper): the clear through ClearWords_, which loads its address
   * before the size's shift; as a direct Iwram_ClearWords call, 2 of 430
   * instructions differ in tla-en. */
  ClearWords_((void *)0x06004000, 0x4000);
  HeapSlot_Release(188);
  HeapSlot_Release(104);
}

/// The Broken Seal's Palette_BrightenBgEntries over BG palette entries 1 to
/// 63, with The Lost Age's clamp at 0.
void Func_08164a4c(s32 blue_delta, s32 green_delta, s32 red_delta) {
  u16 *color = (u16 *)0x05000002;
  s32 i;

  for (i = 0; i != 63; i++) {
    s32 blue = (*color >> 10) & 0x1f;
    s32 green = (*color >> 5) & 0x1f;
    s32 red = *color & 0x1f;

    blue += blue_delta;
    green += green_delta;
    red += red_delta;
    if (blue > 31)
      blue = 31;
    if (green > 31)
      green = 31;
    if (red > 31)
      red = 31;
    if (blue < 0)
      blue = 0;
    if (green < 0)
      green = 0;
    if (red < 0)
      red = 0;
    *color++ = (blue << 10) | (green << 5) | red;
  }
}

/// As Func_08164a4c over BG palette entries 96 to 223, the deltas red first.
void Func_08164abc(s32 red_delta, s32 green_delta, s32 blue_delta) {
  u16 *color = (u16 *)0x050000c0;
  s32 i;

  for (i = 0; i != 128; i++) {
    s32 blue = (*color >> 10) & 0x1f;
    s32 green = (*color >> 5) & 0x1f;
    s32 red = *color & 0x1f;

    red += red_delta;
    green += green_delta;
    blue += blue_delta;
    if (red > 31)
      red = 31;
    if (green > 31)
      green = 31;
    if (blue > 31)
      blue = 31;
    if (red < 0)
      red = 0;
    if (green < 0)
      green = 0;
    if (blue < 0)
      blue = 0;
    *color++ = (blue << 10) | (green << 5) | red;
  }
}

#include "battle_work.h"

/// As Func_08164abc, from the battle work's saved palette into BG palette
/// entries 96 to 223.
void Func_08164b2c(s32 redDelta, s32 greenDelta, s32 blueDelta) {
  u16 *color = (u16 *)0x050000c0;
  u16 *source = Ram_HeapSlots->battleWork->palette;
  s32 i;

  for (i = 0; i != 128; i++) {
    s32 blue = (*source >> 10) & 0x1f;
    s32 green = (*source >> 5) & 0x1f;
    s32 red = *source & 0x1f;

    red += redDelta;
    green += greenDelta;
    blue += blueDelta;
    if (red > 31)
      red = 31;
    if (green > 31)
      green = 31;
    if (blue > 31)
      blue = 31;
    if (red < 0)
      red = 0;
    if (green < 0)
      green = 0;
    if (blue < 0)
      blue = 0;
    *color++ = (blue << 10) | (green << 5) | red;
    source++;
  }
}

#include "battle_effect_argument.h"

/// Stops DMA 0, then arms it to copy one palette line from the battle effect
/// work into the BG palette at each horizontal blank.
// The Broken Seal's BattleEffect_ArmPaletteHBlankDma.
void BattleEffect_ArmPaletteHBlankDma(void) {
  u8 *work = Ram_HeapSlots->battleFxWork[0];
  volatile u16 *channel = REG_DMA0;
  u32 palette;

  channel[5] &= 0xc5ff;
  channel[5] &= 0x7fff;
  palette = 0xa0;
  (void)channel[5];
  Dma_Set(work + 0x7640, (void *)(palette << 19), DMA_HBLANK_HALFWORD_PER_LINE,
          (volatile u32 *)channel);
}

/// The particle streams in mode 0.
DEFINE_EFFECT_KIND(BattleEffect_RunMeteor, BattleEffect_RunParticleStreams, 0)

/// The particle streams in mode 1.
DEFINE_EFFECT_KIND(BattleEffect_RunOuterSpace, BattleEffect_RunParticleStreams,
                   1)

#undef EMBERS

// BattleEffect_RunParticleStreams, summon 11 and summon 0's default
// (BattleEffect_RunSceneEffect's default case, through BattleEffect_RunMeteor
// and BattleEffect_RunOuterSpace), with its nested
// helpers AddFlame (Func_08164c0c) and AddMote (Func_08164c70, unused).

// Twin: ☀️ BattleEffect_RunParticleStreams (games/tbs/src/effect/effect.c),
// ported: the heap blocks read from Ram_HeapSlots (canvas first), the effect
// kept as the argument, Func_08163c2c for the canvas wipe, the fills through
// IwramFillWords where ☀️ uses DMA, mode 1's two objects resource 638 + i *
// 0x2001 (☀️ 483), mode 0's spawn 650 (☀️ 381), the palette lines at 0x7640 in
// the work (☀️ 0x1f80), the blitter modes counted down from 3, and the callback
// orders 0xc80, 0xcfe and 0xcff (☀️ 0x480, 0x4fe, 0x4ff).

#include "battle_display.h"

#include "bg_scroll.h"

#include "battle.h"
#include "graphics.h"

static __inline__ s32 CopyWords_(void *destination, const void *source,
                                 s32 size) {
  return Iwram_CopyWords(destination, source, size);
}

static __inline__ s32 FillWords_(void *destination, s32 size, u32 value) {
  return Iwram_FillWords(destination, size, value);
}

// The heap blocks the dispatcher hands every canvas effect, as byte offsets
// into Ram_HeapSlots (☀️ uses slot indices).

// The map cell buffer: first a 256 x 128 picture the streams draw into (BG1's
// tiles), then, in the second act, the flashes and the embers.
#define FLASHES ((struct EffectStep *)Ram_MapCellBuffer)
#define EMBERS (&FLASHES[128])

// The scanline palette the HBlank DMA feeds into BG colour 0, in the work.
#define HEAT_LINES(work) ((u16 *)((u8 *)(work) + 0x7640))

// BattleEffect_LoadBlitter's argument: The Lost Age's blitter mode (☀️'s counted
// down from 3) in the high nibble, the COMPOSITE_ flags in the low one.
#define BLITTER(mode, flags) ((mode) << 4 | (flags))

// ⚓️'s blitter modes run the other way from The Broken Seal's.
enum { BLIT_ADDITIVE, BLIT_MAX, BLIT_TRANSPARENT, BLIT_COPY };

#define STREAM_FRAMES 192
#define BLAST_FRAMES 54
#define BLAST_FLASH_FRAME 16 // the flashes rise from here
#define BLAST_HIT_FRAME 48

#define SE_RISE 145
#define SE_STREAMS 141
#define SE_BLAST 156

struct Scale {
  s32 x;
  s32 y;
};

struct FxAnimObject {
  u8 unk00[9];
  u8 unk09_0 : 2;
  u8 priority : 2;
  u8 unk09_4 : 4;
  u8 unk0a[16];
  u8 flags;
};

struct EffectCamera {
  s32 eye[3];
  s32 pos[3];
};

extern char ResourceId_FlameSheetB;
extern char ResourceId_LightningBoltSheet;
extern char ResourceId_BlastSheet;

extern s16 Data_08198a02[][3]; // ☀️ gParticleStreams_OrbitPoints
extern s16 Data_08198a2c[][2]; // ☀️ gParticleStreams_DropPoints
extern u16 Data_08198a48[];    // ☀️ gParticleStreams_FlashCells
extern u16 Data_08198a56[];    // ☀️ gParticleStreams_FlashSizes

void BattleEffect_ConfigureEffectDisplay(void);

void Func_08163c2c(s32 mode, s32 value);
void Animation_ApplyChildArgumentFar(void *object, s32 animation);

void BattleEffect_AdvanceBg2OnInterval(void);
void BattleEffect_FlushPendingGraphicsTransfer(void);
s32 Func_08196404(s32 slot, s32 widthShift, s32 heightShift, s32 flags,
                  u32 mode); // BattleEffect_BuildBlitter
s32 BattleEffect_LoadBlitter(
    s32 slot, s32 modeAndFlags); // BattleEffect_BuildBlitter(slot,
                                 // 7, 7, flags, mode)

void SceneTransform_ApplyPosition(const s32 *pos);
void Func_080150e4(s32 roll); // SceneTransform_ApplyRoll
void SceneTransform_ApplyYaw(s32 yaw);
void Func_0814cca8(void); // BattleEffect_SetupBlendedDisplay
void Func_08164a4c(s32 red, s32 green, s32 blue); // Palette_BrightenBgEntries
void BattleActor_CommitPlacementFar(void);

void Transform_SetLookAt(s32 *eye, s32 *position);
void BattleEffectKernels_IntegrateVector3(struct EffectStep *step, s32 damping,
                                          s32 gravity);
void BattleCamera_Shake(s32 rangeX, u32 rangeY);

// Summon 11: the screen goes dark and the ground dissolves away from the top;
// a sun (or, in mode 1, two great flames the user throws) climbs over a
// spinning ring of seven orbs while burning drops rain and the sky glows from
// a heat palette under every scanline; then the scene bursts: embers fountain
// up, flashes rise from the target, and the sun swells over the enemies.
//
// Act one (192 frames, A or B ends it after frame 4, or 16 in mode 1): the
// dark backdrop, a noise of 256 bright pixels on the canvas, the ground
// eaten away row by row, the HBlank heat palette and the BG2 scroll; each
// frame the heat lines, the objects, a new mote, the orbs, the drops (each
// becomes a flame where it lands) and the motes. Act two (54 frames): the
// blast pictures, 32 motes, 128 flashes and 512 embers; the palette
// brightens from frame 36, the hit at 48.
void BattleEffect_RunParticleStreams(struct BattleEffectArgument *effect,
                                     s32 mode) {
  u8 offsets[128];
  void *canvas;
  DrawRectangle draw;
  s32 direction;
  struct BattleEffectWork *work;

  // A flame on the ground where a drop lands, in the first free of 16
  // slots after the motes.
  void AddFlame(s32 x, s32 y, s32 velocityX) {
    s32 i;

    for (i = 0; i != 16; i++) {
      struct EffectStep *flame = &work->particles[32 + i];

      if (flame->variant == -1) {
        flame->variant = 0;
        flame->x = x;
        flame->y = y;
        flame->velocityX = velocityX;
        break;
      }
    }
  }

  // A mote in the first free of the 32 slots (unused).
  void AddMote(s32 x, s32 y) {
    s32 i;

    for (i = 0; i != 32; i++) {
      struct EffectStep *mote = &work->particles[i];

      if (mote->variant == -1) {
        mote->variant = 0;
        mote->x = x;
        mote->y = y;
        break;
      }
    }
  }

  s32 position[4];
  s32 vector[3];
  struct EffectPosition spot;
  struct EffectPosition *pos;
  struct EffectPosition point;
  struct Scale scale;
  s32 frame;
  s32 i;

  canvas = Ram_HeapSlots->battleFxWork[1];
  work = (struct BattleEffectWork *)Ram_HeapSlots->battleFxWork[0];
  BattleEffect_StartCanvasLayer(0x2000);
  REG_BG2PA = 0x100;

  // Mode 1: the user leaps and flashes; the streams lean the user's way.
  /* FAKEMATCH(temporary): mode tested through its copy in direction, so CSE
   * keeps the mode reload in this arm; as if (mode == 1), 2 of 1591
   * instructions differ in tla-en. */
  direction = mode;
  if (direction == 1) {
    struct MotionObject *user = GetBattleObjectSlotFar(effect->actor)->object;

    user->velocityY = 0xa0000;
    user->verticalMotionStrength = 0x91eb;
    Func_0814cd48(effect->actor, -1, 2, -1, 0);
    Sound_PlayFar(SE_RISE);
    direction = effect->side == 1 ? mode : -1;
  } else {
    direction = -1;
  }

  // A dark backdrop and an empty canvas.
  BattleEffect_ConfigureEffectDisplay();
  *(u16 *)BG_PLTT = 0;
  *(u16 *)(BG_PLTT + 1) = 0;
  work->transferMode = 0;
  Task_Add(BattleEffect_ProcessPendingGraphicsTransfer, TASK_PASS_FRAME);
  Func_08163c2c(0, 0);
  Task_Remove(BattleEffect_ProcessPendingGraphicsTransfer);

  // Mode 1's two flames, or mode 0's sun.
  if (mode == 1) {
    for (i = 0; i != 2; i++) {
      struct FxAnimObject *flame = ResourceObject_CreateFar(638 + i * 0x2001);

      work->objects[i] = flame;
      if (flame != 0) {
        flame->flags = 0;
        Animation_ApplyChildArgumentFar(flame, 2);
        ((struct FxAnimObject *)work->objects[i])->priority = 3;
      }
    }
  } else {
    BattleEffect_SpawnObjects(1, 650, 3);
  }
  Resource_LoadAndDecompress((s32)&ResourceId_FlameSheetB, work->sheet, 1, 1);
  if (mode == 1)
    CopyWords_((void *)BG_PLTT,
               Resource_GetTableEntry((s32)&ResourceId_LightningBoltSheet),
               128);

  // BG1 shows the ground: the map cell buffer filled with colour 1.
  FillWords_(Ram_MapCellBuffer, 0x8000, 0x01010101);
  // The copy in a once-only block, so its entry loads before the size; as
  // the plain call, 2 of 1591 instructions differ in tla-en.
  do {
    CopyWords_((void *)0x06008000, Ram_MapCellBuffer, 0x7800);
  } while (0);
  wait(1);
  REG_BLDCNT = 0;
  REG_BG2PA = 0x100;
  REG_BG1CNT = 0x1f80;
  REG_BG2CNT = 0x2787;

  // 63 pale colours, and 256 of them scattered over the canvas.
  for (i = 0; i != 63; i++) {
    s32 red = (Random16() & 15) + 16;
    s32 green = (Random16() & 15) + 16;
    s32 blue = (Random16() & 15) + 16;

    ((u16 *)0x05000100)[i] = (blue << 10) | (green << 5) | red;
  }
  // The clear in a once-only block, so its entry and the canvas load before
  // the size's shift; as the plain call, 4 of 1591 instructions differ in
  // tla-en.
  do {
    FillWords_(canvas, 0x4000, 0);
  } while (0);

  for (i = 0; i != 256; i++) {
    s32 x = Random16() & 127;
    s32 y = Random16() & 127;
    s32 shade = (Random16() & 63) + 64;

    ((u8 *)canvas)[((y / 8 * 16 + x / 8) * 8 + (y & 7)) * 8 + (x & 7)] = shade;
  }
  CopyWords_((void *)0x06004000, canvas, 0x4000);
  gCameraSceneParameters[4] = 240;
  BattleEffect_SelectLivingTargets(work->effect);

  // BG2 scrolls a pixel every other frame, down and toward the user's side.
  work->bg2x = 0;
  work->bg2y = 0;
  work->scrollTimer = 0;
  work->scrollInterval = 2;
  work->scrollStepX = direction << 7;
  work->scrollStepY = 256;
  Task_Add(BattleEffect_AdvanceBg2OnInterval, TASK_PASS_FRAME + 127);
  Task_Add(BattleEffect_FlushPendingGraphicsTransfer, TASK_PASS_FRAME);

  // The ground dissolves: each column starts at its own random row, and the
  // rows go faster and faster.
  for (i = 0; i != 128; i++)
    offsets[i] = Random16() & 63;
  {
    s32 limit = 0;
    s32 step = 1;
    s32 row = 0;

    do {
      limit += step / 2;
      step += 4;
      for (; row != limit; row++) {
        s32 x;

        for (x = 0; x != 256; x++) {
          s32 y = row - offsets[x & 127];

          if (y >= 0)
            if (y <= 127)
              Ram_MapCellBuffer[((y / 8 * 32 + x / 8) * 8 + (y & 7)) * 8 +
                                (x & 7)] = 0;
        }
      }
      work->transferPending = 1;
      wait(1);
    } while (limit <= 191);
  }
  REG_BLDCNT = 0x3f42;
  REG_BLDALPHA = 0x1010;
  {
    s32 savedX = gBgScroll[1].x;
    s32 savedY = gBgScroll[1].y;
    struct BattlePresentationTransition *transition =
        Ram_HeapSlots->transitionWork;
    s32 transferMode = 3;

    /* FAKEMATCH(temporary): the transfer mode's 3 set in transferMode before
     * the blitter build, so it stays in r6 across the call; as
     * work->transferMode = 3, 16 of 1591 instructions differ in tla-en. */
    gBgScroll[1].x = 0;
    gBgScroll[1].y = 32;
    Func_08196404(HEAP_BLITTER, 8, 7, COMPOSITE_CLIP_X | COMPOSITE_CLIP_Y,
                  BLIT_MAX);
    draw = (DrawRectangle)Ram_HeapSlots->unk64[1];
    work->transferMode = transferMode;
    work->transferValue = 0x02020202;
    Task_Add(BattleEffect_ArmPaletteHBlankDma, TASK_PASS_FRAME + 126);
    for (i = 0; i != 64; i++)
      work->particles[i].variant = -1;
    transition->active = 1;
    work->frame = 0;
    frame = 0;
    do {
      s32 angle = frame << 8;
      s32 count = work->frame;
      u8 *base = (u8 *)work;
      s32 dividend;
      s32 heat;
      u16 *line;
      s32 size;
      s32 x;
      s32 y;

      /* FAKEMATCH(temporary): the division written out on a copy of count,
       * which it tests and biases; as heat = count / 4, 4 of 1591 instructions
       * differ in tla-en. */
      dividend = count;
      /* FAKEMATCH(dead): updates that cancel out end CSE's knowledge of count,
       * so the test reads the copy; without them, 4 of 1591 instructions
       * differ in tla-en. */
      if (base) {
        count++;
        count--;
      }
      if (dividend < 0)
        dividend += 3;
      heat = dividend >> 2;
      /* FAKEMATCH(barrier): count and base through an empty asm clobbering r1
       * and r3, so count keeps r2 and base takes r0; without it, 67 of 1591
       * instructions differ in tla-en. */
      asm("" : "=r"(base) : "0"(base), "r"(count) : "r1", "r3");
      line = HEAT_LINES(base);

      if (mode == 1) {
        if ((gInput.repeat & 3) && frame > 16)
          break;
      } else {
        if ((gInput.repeat & 3) && frame > 4)
          break;
      }
      {
        s32 hold;

        /* FAKEMATCH(barrier): hold set by an empty asm takes r7 across the
         * call, so heat takes r4 and its sp+8 save; without hold, 336 of 1591
         * instructions differ in tla-en. */
        asm("" : "=r"(hold));
        /* FAKEMATCH(barrier): an empty asm that clobbers r6 before the call;
         * without it, 336 of 1591 instructions differ in tla-en. */
        asm("" : : : "r6");
        if (frame == 0)
          Sound_PlayFar(SE_STREAMS);
        /* FAKEMATCH(dead): updates of hold that cancel out, under a test of
         * hold, keep it live across the call; without them, 336 of 1591
         * instructions differ in tla-en. */
        if (hold) {
          hold++;
          hold--;
        }
      }

      // The heat palette: black at the top and bottom, rising through red
      // to yellow as the frames pass.
      for (i = 0; i != 15; i++)
        *line++ = 0;
      for (; i != 135; i++) {
        s32 green = i - 16;
        s32 quarter = green / 4;
        s32 level = quarter + heat;
        s32 red;

        /* FAKEMATCH(dead): updates of quarter that cancel out, under a test of
         * quarter, keep it live past level's sum, so level takes r2; as level =
         * quarter + heat alone, 18 of 1591 instructions differ in tla-en. */
        if (quarter) {
          quarter++;
          quarter--;
        }
        red = level - 32;
        green = level - 80;

        if (red < 0)
          red = 0;
        if (red > 31)
          red = 31;
        if (green < 0)
          green = 0;
        if (green > 31)
          green = 31;
        *line++ = (red << 10) | (green << 5) | (green >> 1);
      }
      for (; i != 160; i++)
        *line++ = 0;

      // The sun (or the flames) climbs, growing.
      if (direction == 1)
        x = frame / 4;
      else
        x = 64 - frame / 4;
      y = 96 - frame;
      position[3] = 0;
      position[1] = 0xff0000;
      if (mode == 1) {
        size = angle + 0xa000;
        scale.x = size;
        scale.y = size;
        position[0] = (x << 16) + 0x500000;
        position[2] = (64 - y) << 16;
        Render_ApplyProjectedPlacementFar(work->objects[0], position, &scale,
                                          0);
        Render_ApplyProjectedPlacementFar(work->objects[1], position, &scale,
                                          0);
      } else {
        size = angle + 0x10000;
        scale.x = size;
        scale.y = size;
        position[0] = (x << 16) + 0x600000;
        position[2] = (96 - y) << 16;
        Render_ApplyProjectedPlacementFar(work->objects[0], position, &scale,
                                          0);
      }

      // A new mote on the sun's rim.
      for (i = 0, y = 32 - y; i != 32; i++) {
        struct EffectStep *mote = &work->particles[i];

        if (mote->variant == -1) {
          s32 angle = (Random16() & 0x7fff) + 0x4000;

          mote->variant = 0;
          mote->x = ((x + 96) << 16) + Trig_Sin(angle) * 30 / 65536 * size;
          mote->y = (y << 16) - Trig_Cos(angle) * 30 / 65536 * size;
          break;
        }
      }

      // The ring of seven orbs, turning.
      point.x = 0;
      point.y = 0;
      point.depth = 0x2000000;
      Render_ResetTransformState();
      SceneTransform_ApplyPosition(&point.x);
      Func_080150e4(0x800);
      SceneTransform_ApplyYaw(frame << 8);
      for (i = 0; i != 7; i++) {
        vector[0] = (Data_08198a02[i][0] / 2) << 16;
        vector[1] = (Data_08198a02[i][1] + frame) << 16;
        vector[2] = (Data_08198a02[i][2] / 2) << 16;
        EffectPosition_ProjectPoint(vector, pos = &spot);
        pos->x = HI(pos->x) + 128;
        pos->y = HI(pos->y) + 60;
        draw(Ram_MapCellBuffer, work->sheet + 0x1f40, pos->x - 4, pos->y - 4, 8,
             8);
      }

      // The drops fall, and each becomes a flame where it lands.
      if (direction == 1)
        x = frame / 4 - 16;
      else
        x = 16 - frame / 4;
      y = frame - 96;
      for (i = 0; i != 7; i++) {
        s32 drop = Data_08198a2c[i][1] + y;

        if (drop <= 93)
          draw(Ram_MapCellBuffer, work->sheet + 0x1c80,
               Data_08198a2c[i][0] + x - 12, drop - 12, 24, 24);
        else if (drop <= 95)
          AddFlame((Data_08198a2c[i][0] + x) << 16, drop << 16, 1);
      }

      // The motes drift up and away, six pictures each.
      for (i = 0; i != 32; i++) {
        struct EffectStep *mote = &work->particles[i];

        if (mote->variant >= 0) {
          draw(Ram_MapCellBuffer, work->sheet + (mote->variant << 10),
               HI(mote->x) - 16, HI(mote->y) - 16, 32, 32);
          mote->x -= direction * 0x14000;
          mote->y -= 0x50000;
          mote->variant++;
          if (mote->variant == 6)
            mote->variant = -1;
        }
      }
      work->transferPending = 1;
      wait(1);
      frame++;
      work->frame++;
    } while (frame != STREAM_FRAMES);
    wait(1);
    transition->active = 0;
    Task_Remove(BattleEffect_AdvanceBg2OnInterval);
    Task_Remove(BattleEffect_ArmPaletteHBlankDma);
    Task_Remove(BattleEffect_FlushPendingGraphicsTransfer);
    gBgScroll[1].x = savedX;
    gBgScroll[1].y = savedY;
  }

  // Act two: the blast.
  HeapSlot_Release(HEAP_BLITTER);
  Func_0814cca8();
  REG_BG2PA = 0x80;
  REG_BG2X = 0;
  REG_BG2Y = 0xfffff000;
  REG_BLDALPHA = 0x1010;
  REG_BG2CNT = 0x2784;
  BattleEffect_LoadBlitter(
      HEAP_BLITTER, BLITTER(BLIT_MAX, COMPOSITE_CLIP_X | COMPOSITE_CLIP_Y));
  draw = (DrawRectangle)Ram_HeapSlots->unk64[1];
  Resource_LoadAndDecompress((s32)&ResourceId_BlastSheet, work->sheet, 1, 0);
  for (i = 0; i != 32; i++) {
    struct EffectStep *mote = &work->particles[i];

    mote->x = Random16() & 127;
    mote->y = (Random16() & 127) + 127;
  }
  for (i = 0; i != 128; i++) {
    struct EffectStep *flash = &FLASHES[i];

    flash->x = 0;
    flash->y = 0;
    flash->z = 0;
    flash->velocityX = ((Random16() & 255) - 127) << 12;
    flash->velocityY = (Random16() & 255) << 11;
    flash->velocityZ = ((Random16() & 255) - 127) << 12;
    flash->variant = 0;
  }
  for (i = 0; i != 512; i++) {
    struct EffectStep *ember = &EMBERS[i];

    ember->x = 0;
    ember->y = 0;
    ember->z = 0;
    ember->velocityX = ((Random16() & 255) - 128) << 13;
    ember->velocityY = (Random16() & 255) << 11;
    ember->velocityZ = ((Random16() & 255) - 128) << 13;
    ember->variant = 0;
  }
  work->transferMode = 1;
  work->transferValue = 0x10101010;
  Task_Add(BattleEffect_ProcessPendingGraphicsTransfer, TASK_PASS_FRAME);

  for (frame = 0; frame != BLAST_FRAMES; frame++) {
    s32 size = (frame + 0x1d0) << 8;
    struct EffectCamera *camera = Ram_HeapSlots->cameraWork;
    s32 elapsed = frame - BLAST_FLASH_FRAME;
    struct Scale scale;
    s32 x;
    s32 y;
    s32 grow;

    if (elapsed > 19)
      Func_08164a4c(2, 2, 2);
    if (frame == 0)
      Sound_PlayFar(SE_BLAST);
    if (frame == 40)
      Sound_PlayFar(SE_RISE);
    if (frame == BLAST_HIT_FRAME) {
      if (mode == 1) {
        ResourceObject_ReleaseFar(work->objects[0]);
        ResourceObject_ReleaseFar(work->objects[1]);
        BattleActor_CommitPlacementFar();
      }
      Func_081180e8(PHASE_SUMMON_RESULTS);
    }

    // The embers fountain up from the ground in front of the camera.
    Render_ResetTransformState();
    Transform_SetLookAt(camera->eye, camera->pos);
    for (i = 0; i != 64; i++) {
      struct EffectStep *ember = &EMBERS[i];

      if (ember->y >= 0) {
        s32 size;

        EffectPosition_ProjectPoint(&ember->x, &point);
        point.x >>= 1;
        if (point.depth <= 159)
          point.depth = 160;
        if (point.depth > 799)
          point.depth = 799;
        size = 9 - (point.depth - 160) / 64;
        draw(canvas,
             work->sheet + (gData_08197410[size - 1] + (i & 1) * 770) + 0x3200,
             point.x - size / 2, point.y - size, size, size * 2);
        BattleEffectKernels_IntegrateVector3(ember, 64, -0x2000);
        if (ember->y <= 0x140000) {
          ember->x = 0;
          ember->z = 0;
          ember->y = 0x140000;
          ember->velocityX = ((Random16() & 63) - 32) << 15;
          ember->velocityY = (Random16() & 63) << 13;
          ember->velocityZ = ((Random16() & 63) - 32) << 15;
        }
      }
    }

    // The motes rise.
    for (i = 0; i != 64; i++) {
      struct EffectStep *mote = &work->particles[i];
      u32 rise = i & 7;
      s32 size = rise + 3;

      draw(canvas,
           work->sheet + (gData_08197410[size - 1] + (i & 1) * 770) + 0x3200,
           mote->x - size / 2, mote->y - size, size, size * 2);
      mote->y = mote->y - rise - 8;
      if (mote->y < -10)
        mote->y = 128;
    }

    // The flashes rise from the target, three more every frame.
    for (i = 0; i != 64; i++) {
      struct EffectStep *flash = &FLASHES[i];

      if (i / 3 < elapsed && flash->y >= 0) {
        EffectPosition_ProjectPoint(&flash->x, &point);
        point.x >>= 1;
        if ((u32)flash->variant <= 13) {
          s32 cell = flash->variant / 2;

          draw(canvas, work->sheet + Data_08198a48[cell],
               point.x - Data_08198a56[cell] / 2,
               point.y - Data_08198a56[cell] / 2, Data_08198a56[cell],
               Data_08198a56[cell]);
        }
        flash->variant++;
        if (flash->variant == 14) {
          flash->y = 0x140000;
          flash->x = 0;
          flash->z = ((Random16() & 255) - 127) << 16;
          flash->velocityX = 0;
          flash->velocityY = (Random16() & 255) << 11;
          flash->velocityZ = 0;
          flash->variant = 0;
        } else {
          BattleEffectKernels_IntegrateVector3(flash, 64, 1);
        }
      }
    }

    // The sun swells over the enemies.
    if (direction == 1)
      x = frame / 2 + 24;
    else
      x = 56 - frame / 2;
    y = 64 - frame * 2;
    grow = (frame << 8) + 0x20000;
    position[3] = 0;
    position[1] = 0xff0000;
    if (mode == 1) {
      scale.x = size;
      scale.y = size;
      position[0] = (x << 16) + 0x600000;
      position[2] = (96 - y) << 16;
      Render_ApplyProjectedPlacementFar(work->objects[0], position, &scale, 0);
      Render_ApplyProjectedPlacementFar(work->objects[1], position, &scale, 0);
    } else {
      scale.x = grow;
      scale.y = grow;
      position[0] = (x << 16) + 0x600000;
      position[2] = (96 - y) << 16;
      Render_ApplyProjectedPlacementFar(work->objects[0], position, &scale, 0);
    }
    work->shakeFrames = 1;
    BattleCamera_Shake(8, 8);
    work->transferPending = 1;
    wait(1);
  }

  Task_Remove(BattleEffect_ProcessPendingGraphicsTransfer);
  HeapSlot_Release(HEAP_BLITTER);
  if (mode == 0)
    ResourceObject_ReleaseFar(work->objects[0]);
  BattleEffect_EndCanvasLayer();
}

#undef FORMATION
#undef FORMATION_SIZE
#undef SCENE_FRAMES

#include "render_placement.h"

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

#define SE_DROP 156
#define SE_HIT 145

struct FxScale {
  s32 x;
  s32 y;
};

extern char ResourceId_DesertBackdrop;

extern struct FxScale gData_08196e6c; // one to one
extern u8 gData_08198a64[];           // ☀️ gRamses_ObjectColumns
extern u8 gData_08198a6d[];           // ☀️ gRamses_ObjectRows
extern u16 gData_08198a76[];          // ☀️ gEruption_PuffCells
extern u16 gData_08198a84[];          // ☀️ gCybele_EmberSizes

void BattleBackground_LoadFar(s32 layer, s32 resourceId, s32 mode);

void Func_08118088(s32 actor,
                   s32 variant); // BattleMotion_ApplyVariantMotionFar

void BattleEffect_CountDownBattlerTints(void);

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
  struct SpritePlacement position;
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
  BattleEffect_StartCanvasLayer(0);
  BattleEffect_ConfigureEffectDisplay();
  *(u16 *)BG_PLTT = 0;
  *(u16 *)(BG_PLTT + 1) = 0;
  work->transferMode = 0;
  Task_Add(BattleEffect_ProcessPendingGraphicsTransfer, TASK_PASS_FRAME);
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
  BattleEffect_LoadBlitter(
      HEAP_BLITTER, BLITTER(BLIT_MAX, COMPOSITE_CLIP_X | COMPOSITE_CLIP_Y));
  draw[0] = (DrawRectangle)Ram_HeapSlots->unk64[1];
  BattleEffect_LoadBlitter(
      HEAP_BLITTER_ALTERNATE,
      BLITTER(BLIT_ADDITIVE, COMPOSITE_CLIP_X | COMPOSITE_CLIP_Y));
  draw[1] = (DrawRectangle)Ram_HeapSlots->unkb8[1];
  REG_WININ = 0x2737;
  REG_WIN0H = 0xf0;
  REG_WIN1V = 0x1088;
  wait(1);

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
      Sound_PlayFar(SE_DROP);
    if (frame == FIRST_DROP + 45)
      Sound_PlayFar(SE_DROP);
    if (frame == FIRST_DROP + 87)
      Sound_PlayFar(SE_DROP);
    if (frame == HIT_FRAME)
      Sound_PlayFar(SE_HIT);
    scale = gData_08196e6c;
    if (frame >= ROCKS_FALL && frame <= 251)
      work->shakeFrames = 1;
    else if (frame >= HIT_FRAME && frame <= HIT_FRAME + 3)
      work->shakeFrames = 1;

    // The formation, a 3 x 3 block behind the orb.
    position.groundHeight = 0;
    position.height = 0;
    for (i = 0; i != 7; i++) {
      position.x = (gData_08198a64[i] << 16) + base.x - 0x200000;
      position.y = (gData_08198a6d[i] << 16) + base.y - 0x200000;
      Render_ApplyProjectedPlacementFar(work->objects[i], &position, &scale.x,
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
      position.x = orb.x;
      position.height = -0x1000000;
      position.y = orb.y - 0x1000000;
      Render_ApplyProjectedPlacementFar(work->objects[ORB_LEFT], &position,
                                        &scale.x, 0);
      position.x = orb.x + 0x200000;
      Render_ApplyProjectedPlacementFar(work->objects[ORB_RIGHT], &position,
                                        &scale.x, 0);
    }

    // The rocks fall, bounce once at y = 120 in two puffs of dust, and are
    // dropped again until frame 199.
    position.height = 0;
    for (i = 0; i != ROCK_COUNT; i++) {
      struct EffectStep *rock = &work->particles[i];

      if (rock->variant != 2) {
        position.x = rock->x;
        position.y = rock->y;
        Render_ApplyProjectedPlacementFar(work->objects[FIRST_ROCK + i],
                                          &position, &scale.x, 0);
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
    BattleCamera_Shake(8, 8);
    BattleEffect_CountDownBattlerTints();
    work->transferPending = 1;
    wait(1);
  }

  Func_081180e8(PHASE_SUMMON_RESULTS);
  Func_0814cca8();
  for (i = 0; i != FIRST_ROCK + ROCK_COUNT; i++)
    ResourceObject_ReleaseFar(work->objects[i]);
  Task_Remove(BattleEffect_ProcessPendingGraphicsTransfer);
  HeapSlot_Release(HEAP_BLITTER_ALTERNATE);
  HeapSlot_Release(HEAP_BLITTER);
  BattleEffect_EndCanvasLayer();
}

#undef HIT_FRAME
#undef SE_HIT

#include "transform.h"

#define SHARD_COUNT 64
#define STREAK_PICTURE_BYTES (48 * 72)
#define BURST_PICTURE_BYTES (40 * 80)
#define BURST_STRIP_BYTES (3 * BURST_PICTURE_BYTES) // the stronger variant's

#define SHARDS_FRAMES 64
#define STREAK_FRAME 6 // the user's fire streak, six frames
#define HIT_FRAME 8
#define FLASH_FRAME 14
#define BURST_FRAME 16 // the burst on the target, until frame 47

#define PHASE_RESULTS 134
#define SE_HIT 212

extern char ResourceId_FireStreakSheet;
extern char ResourceId_FireBurstSheet;

void BattleEffect_CallDoNothing(s32 frames);

// The user lunges, a streak of fire leaves them, and the target bursts into
// flame, throwing 64 burning shards away from the user.
//
// Setup: open the canvas, load the streak, burst and shard pictures, dash
// the user four frames toward the target, and seed the shards at the target
// with random speeds (flipped to fly away from the user's side). Each frame:
// the streak's six pictures at the user from frame 6; the hit at frame 8
// (results phase, the stronger variant's cue, the canvas flooded, the target
// knocked back); the burst's three pictures (two frames each, then held) on
// the target from frame 16 to 47; from frame 4 the shards in 3D, their
// picture shrinking with their life.
void BattleEffect_RunAnnihilation(struct BattleEffectArgument *effect) {
  struct BattleEffectWork *work;
  void *canvas;
  s32 frame;
  u8 *cells;
  struct EffectCamera *view;
  struct MotionObject *target;
  struct EffectStep *shard;
  struct EffectPosition user;
  struct EffectPosition origin;
  struct EffectPosition screen;
  DrawRectangle draw[2];
  s32 i;
  s32 size;
  u8 *burst;

  work = (struct BattleEffectWork *)Ram_HeapSlots->battleFxWork[0];
  canvas = Ram_HeapSlots->battleFxWork[1];
  cells = Ram_HeapSlots->unk64[0];
  view = Ram_HeapSlots->cameraWork;
  burst = Ram_MapCellBuffer;

  // Open the canvas, load the pictures, and dash at the target.
  BattleEffect_StartCanvasLayer(0);
  Resource_LoadAndDecompress((s32)&ResourceId_FireStreakSheet, work->sheet, 1,
                             1);
  Resource_LoadAndDecompress((s32)&ResourceId_FireBurstSheet, burst, 1, 1);
  Resource_LoadAndDecompress((s32)&ResourceId_Data134, cells, 0, 0);
  BattleMotion_ApproachTargetFar(effect->actor, effect->actors[0], 4, 0);
  wait(1);
  target = GetBattleObjectSlotFar(effect->actors[0])->object;

  // The shards start inside the target; each group of four lives two frames
  // longer than the one before.
  i = 0;
  do {
    struct EffectStep *seed = &work->particles[i];

    seed->x = target->x;
    seed->y = target->y;
    seed->z = target->z;
    seed->velocityX = (Random16() & 255) << 11;
    seed->velocityY = ((Random16() & 255) - 127) << 12;
    seed->velocityZ = ((Random16() & 255) - 127) << 12;
    if (seed->x > 0)
      seed->velocityX = -seed->velocityX;
    seed->variant = i / 4 * 2 + 16;
    i++;
  } while (i != SHARD_COUNT);

  EffectPosition_ProjectScaledBattler(effect->actors[0], &origin);
  Task_Add(BattleEffect_ProcessPendingGraphicsTransfer, TASK_PASS_FRAME);
  work->transferMode = 2;
  work->transferValue = 75;

  for (frame = 0; frame != SHARDS_FRAMES; frame++) {
    if (frame == HIT_FRAME)
      Func_081180e8(PHASE_RESULTS);
    if (effect->variant != 0 && frame == HIT_FRAME)
      Sound_PlayFar(SE_HIT);

    // The streak, drawn from the user toward the target.
    EffectPosition_ProjectScaledBattler(effect->actor, &user);
    if (frame >= STREAK_FRAME && frame <= STREAK_FRAME + 5) {
      if (effect->side == 0)
        BattleEffect_LoadBlitter(
            HEAP_BLITTER,
            BLITTER(BLIT_ADDITIVE, COMPOSITE_CLIP_X | COMPOSITE_CLIP_Y));
      else
        BattleEffect_LoadBlitter(HEAP_BLITTER,
                                 BLITTER(BLIT_ADDITIVE, COMPOSITE_CLIP_X |
                                                            COMPOSITE_CLIP_Y |
                                                            COMPOSITE_FLIP_X));
      draw[0] = (DrawRectangle)Ram_HeapSlots->unk64[1];
      if (effect->side == 0)
        draw[0](canvas,
                work->sheet + (frame - STREAK_FRAME) * STREAK_PICTURE_BYTES,
                user.x / 2 - 24, user.y - 24, 48, 72);
      else
        draw[0](canvas,
                work->sheet + (frame - STREAK_FRAME) * STREAK_PICTURE_BYTES,
                user.x / 2, user.y - 24, 48, 72);
      HeapSlot_Release(HEAP_BLITTER);
    }

    // The burst on the target; the stronger variant has its own pictures.
    if (frame >= BURST_FRAME && frame <= 47) {
      s32 step = (frame - BURST_FRAME) / 2;
      s32 strip;

      BattleEffect_LoadBlitter(
          HEAP_BLITTER, BLITTER(BLIT_MAX, COMPOSITE_CLIP_X | COMPOSITE_CLIP_Y));
      draw[0] = (DrawRectangle)Ram_HeapSlots->unk64[1];
      if (step > 2)
        step = 2;
      if (effect->variant == 0)
        strip = 0;
      else
        strip = BURST_STRIP_BYTES;
      draw[0](canvas, Ram_MapCellBuffer + strip + step * BURST_PICTURE_BYTES,
              origin.x / 2 - 20, origin.y - 48, 40, 80);
      BattleEffect_CallDoNothing(10000);
      HeapSlot_Release(HEAP_BLITTER);
    }
    if (frame == HIT_FRAME)
      Iwram_FillWords(canvas, 0x4000, 0x3f3f3f3f);

    // The shards, projected from the battle camera.
    Render_ResetTransformState();
    Transform_SetLookAt(view->eye, view->pos);
    if (frame > 3) {
      BattleEffect_FetchRectangleBlitters(effect->side, draw);
      for (i = 0; i != SHARD_COUNT; i++) {
        s32 life;

        shard = &work->particles[i];
        life = shard->variant;
        if (life > 0) {
          EffectPosition_ProjectPoint((s32 *)shard, &screen);
          screen.x >>= 1;
          screen.y = screen.y + origin.y - 112;
          size = (life >> 3) + 2;
          draw[(i / 2) & 1](canvas, cells + gData_08197410[size - 1],
                            screen.x - size / 2, screen.y - size, size,
                            size * 2);
          BattleEffectKernels_IntegrateVector3(shard, 60, -0x400);
          shard->variant--;
        }
      }
      HeapSlot_Release(HEAP_BLITTER_ALTERNATE);
      HeapSlot_Release(HEAP_BLITTER);
    }

    // The hit's knock-back, and the target's flashes.
    if (frame == HIT_FRAME) {
      Func_08118088(effect->actors[0], 4);
      work->shakeFrames = 4;
    }
    if (frame == STREAK_FRAME)
      Func_0814cd48(effect->actors[0], 10, -1, -1, 0);
    if (frame == FLASH_FRAME)
      Func_0814cd48(effect->actors[0], 10, -1, -1, 0);
    BattleCamera_Shake(16, 16);
    BattleEffect_CountDownBattlerTints();
    work->transferPending = 1;
    wait(1);
  }

  Task_Remove(BattleEffect_ProcessPendingGraphicsTransfer);
  BattleEffect_EndCanvasLayer();
}

#undef RESULTS_FRAME

// The sheet holds the blade (64 x 128) and then the flash pictures.
#define FLASH_SHEET (64 * 128)
#define FLASH_PICTURE_BYTES (32 * 64)

// The map cell buffer holds each column's sparks.

#define SPARKS_PER_COLUMN 340
#define SPARKS_DRAWN 256
#define FLASHES_PER_COLUMN 16
#define FLASHES_DRAWN 12

#define BLADE_FRAMES 80
#define BLADE_SWING_FRAMES 10 // the blade comes down
#define BLADE_HOLD_FRAME 20   // then holds, then creeps on
#define BLADE_LAST_FRAME 39
#define FIRST_STRIKE_FRAME 16 // each column strikes 8 frames after the last
#define RESULTS_FRAME 40

#define SE_SWING 212
#define SE_STRIKE 145

extern char ResourceId_FlameBladeSheet;
extern char ResourceId_FlashBurstSheet;

extern u8 gData_08198a92[]; // ☀️ gRagnarok_StrikeColumns
extern u8 gData_08198a98[]; // ☀️ gPlasma_FlashCells

void EffectPosition_ProjectBattler(s32 id, struct EffectPosition *position);

void Func_0815f0a0(s32 resourceId); // BattleEffect_StepPaletteToResource

// A blade of fire swings down over the targets and strikes twice, each
// strike a flash and a fountain of sparks that bounce on the ground.
//
// Setup: open the canvas on BG1, pose the user, place the strike columns
// under the first target, load the blade, flash and spark pictures, and
// prepare each column's 16 flashes (a ring) and 340 sparks. Each frame: the
// blade comes down over 10 frames, holds until frame 20, then creeps on and is
// gone after frame 39; from frame 16 the palette steps toward the blast
// colours; each column strikes 8 frames after the last (shake, its flash, its
// targets flashed) and 6 frames later throws its sparks and knocks the
// targets back.
void BattleEffect_RunRagnarok(struct BattleEffectArgument *effect) {
  struct BattleEffectWork *work;
  void *canvas;
  s32 col;
  u8 *cells;
  struct MotionObject *user;
  DrawRectangle draw[2];
  struct EffectPosition target;
  s32 baseX;
  s32 frame;
  s32 i;

  work = (struct BattleEffectWork *)Ram_HeapSlots->battleFxWork[0];
  canvas = Ram_HeapSlots->battleFxWork[1];
  cells = Ram_HeapSlots->unk64[0];

  // Open the canvas, pose the user and place the columns.
  user = GetBattleObjectSlotFar(effect->actor)->object;
  BattleEffect_StartCanvasLayer(1);
  REG_BLDALPHA = 0x1010;
  BattleEffect_FetchRectangleBlitters(effect->side, draw);
  Actor_SetMode(user, 2);
  Actor_SetAnimationSpeedFar(user, 48);
  EffectPosition_ProjectBattler(effect->actors[0], &target);
  if (effect->side == 0)
    baseX = target.x / 2 - 24;
  else
    baseX = target.x / 2 - 92;

  Resource_LoadAndDecompress((s32)&ResourceId_FlameBladeSheet, work->sheet, 1,
                             1);
  Resource_LoadAndDecompress((s32)&ResourceId_FlashBurstSheet,
                             work->sheet + FLASH_SHEET, 1, 0);
  Resource_LoadAndDecompress((s32)&ResourceId_Data134, cells, 0, 0);

  // Each column's flashes (a ring, 2 pixels apart, living 25 to 32 frames)
  // and sparks (from y = 88, at random speeds and angles, living 32 to 39).
  col = 0;
  do {
    i = 0;
    do {
      struct EffectStep *flash = &work->particles[col * FLASHES_PER_COLUMN + i];
      s32 radius = i * 2;
      s32 angle = Random16() & 0xffff;

      flash->x = Trig_Sin(angle) * radius;
      flash->y = -(Trig_Cos(angle) * radius);
      flash->variant = i / 2 + 25;
      i++;
    } while (i != FLASHES_PER_COLUMN);

    i = 0;
    do {
      struct EffectStep *spark = &SPARKS[col * SPARKS_PER_COLUMN + i];
      s32 speed;
      s32 angle;

      speed = (Random16() & 0x1ff) + 32;
      angle = Random16() & 0xffff;
      spark->x = (gData_08198a92[col + effect->side * 3] + baseX) << 16;
      spark->y = 88 << 16;
      spark->velocityX = (Trig_Sin(angle) * speed) >> 6;
      spark->velocityY = -(Trig_Cos(angle) * speed * 2) >> 6;
      spark->variant = (Random16() & 7) + 32;
      i++;
    } while (i != SPARKS_PER_COLUMN);
    col++;
  } while (col != 3);

  work->transferMode = 2;
  work->transferValue = 75;
  Task_Add(BattleEffect_ProcessPendingGraphicsTransfer, TASK_PASS_FRAME);

  frame = 0;
  do {
    if (frame == 4)
      Sound_PlayFar(SE_SWING);
    if (frame == 8)
      work->shakeFrames = frame;
    if (frame == 18)
      Sound_PlayFar(SE_STRIKE);
    if (frame == RESULTS_FRAME)
      Func_081180e8(PHASE_RESULTS);

    // The blade: down 16 pixels a frame, held, then creeping on; nothing
    // below y = 104 is drawn.
    if (frame <= BLADE_LAST_FRAME) {
      s32 height;
      s32 x;
      s32 top;

      height = 128;
      if (effect->side == 1) {
        if (frame < BLADE_SWING_FRAMES) {
          x = frame * 10 - 8;
          top = frame * 16 - 128;
        } else if (frame > BLADE_HOLD_FRAME) {
          x = frame + 62;
          top = frame * 2 - 24;
        } else {
          x = 82;
          top = 16;
        }
      } else {
        if (frame < BLADE_SWING_FRAMES) {
          x = 128 - frame * 10;
          top = frame * 16 - 128;
        } else if (frame > BLADE_HOLD_FRAME) {
          x = 58 - frame;
          top = frame * 2 - 24;
        } else {
          x = 38;
          top = 16;
        }
      }
      if (top + 128 > 104)
        height = height - top - 24;
      if (height > 0)
        draw[0](canvas, work->sheet, x + baseX - 32, top, 64, height);
    }

    if (frame > FIRST_STRIKE_FRAME)
      Func_0815f0a0((s32)&ResourceId_BlastSheet);

    col = 0;
    do {
      s32 start = col * 8 + FIRST_STRIKE_FRAME;

      // The strike: a shake, two frames of the big flash, then the ring.
      if (frame == start)
        work->shakeFrames = 12;
      if (frame >= start) {
        if (frame < start + 2)
          draw[0](canvas, work->sheet + FLASH_SHEET,
                  gData_08198a92[col + effect->side * 3] + baseX - 16, 56, 32,
                  64);
        i = 0;
        do {
          struct EffectStep *flash =
              &work->particles[col * FLASHES_PER_COLUMN + i];
          s32 x;
          s32 life;
          s32 y;

          y = HI(flash->y);
          x = HI(flash->x) + gData_08198a92[col + effect->side * 3] + baseX;
          life = flash->variant;
          if (life >= 0 && life <= 17)
            draw[0](canvas,
                    work->sheet + (gData_08198a98[life / 3] << 11) +
                        FLASH_SHEET,
                    x - 16, y + 56, 32, 64);
          life = flash->variant;
          flash->variant = life > 0 ? life - 1 : -1;
          i++;
        } while (i != FLASHES_DRAWN);
      }

      // The sparks, from six frames after the strike, bouncing at y = 108.
      if (frame > start + 5) {
        i = 0;
        do {
          struct EffectStep *spark = &SPARKS[col * SPARKS_PER_COLUMN + i];

          if (spark->variant > 0) {
            s32 x;
            s32 y;
            s32 size;

            BattleEffectKernels_IntegrateVector2(spark, 64, 0x1000);
            spark->variant--;
            y = spark->y;
            if (y > (108 << 16)) {
              spark->velocityY = -spark->velocityY / 2;
            } else {
              x = spark->x;
              if ((x >= 0 && x <= 0x7effff) && y >= 0) {
                u8 *picture;
                s32 lane;
                s32 py = y >> 16;
                s32 px = x >> 16;

                size = spark->variant / 5 + 1;
                lane = i & 1;
                picture = cells + gData_08197410[size - 1];
                px -= size / 2;
                py -= size;
                draw[lane](canvas, picture, px, py, size, size * 2);
              }
            }
          }
          i++;
        } while (i != SPARKS_DRAWN);
      }

      // The column's targets flash at the strike and are struck by its
      // sparks.
      for (i = 0; i != effect->count; i++) {
        if (frame == start)
          Func_0814cd48(effect->actors[i], 7, 5, i, 10);
        if (frame == start + 6) {
          Func_0814cd48(effect->actors[i], 7, 5, i, 10);
          Func_08118088(effect->actors[i], 4);
        }
      }
      col++;
    } while (col != 2);

    BattleCamera_Shake(16, 16);
    BattleEffect_CountDownBattlerTints();
    work->transferPending = 1;
    wait(1);
    frame++;
  } while (frame != BLADE_FRAMES);

  Actor_SetAnimationSpeedFar(user, 16);
  Task_Remove(BattleEffect_ProcessPendingGraphicsTransfer);
  HeapSlot_Release(HEAP_BLITTER_ALTERNATE);
  HeapSlot_Release(HEAP_BLITTER);
  BattleEffect_EndCanvasLayer();
}

void Func_08166b10(struct BattleEffectArgument *effect, s32 mode);

/// Abilities: Titan Blade (kind 61).
DEFINE_EFFECT_KIND(BattleEffect_RunTitanBlade, Func_08166b10, 0)

/// Abilities: Acheron's Grief (kind 104).
DEFINE_EFFECT_KIND(BattleEffect_RunAcheronsGrief, Func_08166b10, 1)

#undef FLASH_SHEET

#include "effect_step.h"

// The sheet: the fire pillar, the flashes after it, the sword far after.
#define FLASH_SHEET (221 << 4)
#define SWORD_SHEET 20000

// The map cell buffer: 684 motes of dust, then the sword's sparks.
#define DUST ((struct EffectStep *)Ram_MapCellBuffer)
#define SPARK                                                                  \
  ((struct EffectStep *)(Ram_MapCellBuffer + 684 * sizeof(struct EffectStep)))

#define SE_SPRAY 164

extern char ResourceId_SwordSheet;

extern u8 gData_08198a9e[];  // ☀️ gPlasma_FlashCells
extern s32 gData_08198aa4[]; // the dust's four gravities

void Func_0815b634(void);

// A great sword falls from the sky onto the target, a pillar of fire and dust
// spouting under it, and drives into the ground in a burst of flashes and
// sparks.
//
// Setup: open the canvas on BG1, load the pillar, flash, sword and spark
// pictures (mode 1 turns the BG colours first), place 32 flash rings and 340
// sparks under the target, and empty the dust. Each frame (102): the sword
// comes down 16 pixels a frame for 10 frames, holds, sinks slowly from 33 and
// plunges after 60; a fountain of dust at frame 28 and 16 more a frame from 32
// to 63, while the pillar rises; the flashes from 56 and the sparks from 60,
// bouncing on the ground; the canvas floods at frames 9 and 60; every target
// is struck at frame 68.
void Func_08166b10(struct BattleEffectArgument *effect, s32 mode) {
  struct BattleEffectWork *work;
  void *canvas;
  s32 frame;
  DrawRectangle draw[2];
  u8 *cells;
  struct EffectPosition target;
  s32 centerX;
  s32 step;
  s32 origin;
  s32 i;

  work = (struct BattleEffectWork *)Ram_HeapSlots->battleFxWork[0];
  canvas = Ram_HeapSlots->battleFxWork[1];
  cells = Ram_HeapSlots->unk64[0];

  // Open the canvas on BG1 over the target and build the blitters.
  BattleEffect_StartCanvasLayer(1);
  REG_BLDALPHA = 0x1010;
  EffectPosition_ProjectBattler(effect->actors[0], &target);
  centerX = target.x / 2;

  BattleEffect_LoadBlitter(
      HEAP_BLITTER, BLITTER(BLIT_MAX, COMPOSITE_CLIP_X | COMPOSITE_CLIP_Y));
  draw[0] = (DrawRectangle)Ram_HeapSlots->unk64[1];
  BattleEffect_LoadBlitter(
      HEAP_BLITTER_ALTERNATE,
      BLITTER(BLIT_TRANSPARENT, COMPOSITE_CLIP_X | COMPOSITE_CLIP_Y));
  draw[1] = (DrawRectangle)Ram_HeapSlots->unkb8[1];

  if (mode != 2)
    Resource_LoadAndDecompress((s32)&ResourceId_SwordSheet,
                               work->sheet + SWORD_SHEET, 1, 1);
  Resource_LoadAndDecompress((s32)&ResourceId_FirePillarSheetA, work->sheet, 1,
                             0);
  Resource_LoadAndDecompress((s32)&ResourceId_FlashBurstSheet,
                             work->sheet + FLASH_SHEET, 1, 0);
  Resource_LoadAndDecompress((s32)&ResourceId_Data134, cells, 0, 0);

  // Mode 1 turns the first 64 BG colours: red to blue, blue to green, green
  // to red.
  if (mode == 1) {
    u16 *color = (u16 *)0x05000000;

    for (i = 0; i != 64; i++) {
      s32 red = *color & 0x1f;
      s32 green = (*color >> 5) & 0x1f;
      s32 blue = (*color >> 10) & 0x1f;

      *color++ = (red << 10) | (blue << 5) | green;
    }
  }

  work->transferMode = 2;
  work->transferValue = 75;
  Task_Add(BattleEffect_ProcessPendingGraphicsTransfer, TASK_PASS_FRAME);

  // 32 flashes on rings, two pixels apart.
  i = 0;
  do {
    struct EffectStep *flash = &work->particles[i];
    s32 scale = i * 2;
    s32 angle = Random16() & 0xFFFF;

    flash->x = Trig_Sin(angle) * scale;
    flash->y = -(Trig_Cos(angle) * scale);
    flash->variant = i / 2 + 25;
    i++;
  } while (i != 32);

  i = 0;
  do {
    DUST[i].variant = -1;
    i++;
  } while (i != 171 << 2);

  // The sparks wait at the target's feet.
  i = 0;
  origin = centerX << 16;
  do {
    struct EffectStep *spark = &SPARK[i];
    s32 speed;
    s32 angle;

    speed = 0x1FF;
    speed &= Random16();
    angle = Random16() & 0xFFFF;
    spark->x = origin;
    spark->y = 176 << 15;
    spark->velocityX = (Trig_Sin(angle) * (speed + 32)) >> 5;
    spark->velocityY = -(Trig_Cos(angle) * (speed + 32)) >> 6;
    spark->variant = (Random16() & 7) + 32;
    i++;
  } while (i != 170 << 1);

  frame = 0;
  do {
    if (mode == 0) {
      if (frame >= 41 && frame <= 63)
        Func_0815f0a0((s32)&ResourceId_BlastSheet);
    } else if (mode == 1) {
      if (frame == 61) {
        Iwram_CopyWords(
            (void *)0x05000000,
            Resource_GetTableEntry((s32)&ResourceId_LightningBoltSheet), 128);
        Func_0815b634();
      }
    }

    if (frame == 8)
      work->shakeFrames = frame;
    if (frame == 48)
      work->shakeFrames = 8;
    if (frame == 60)
      work->shakeFrames = 16;

    if (frame == 4)
      Sound_PlayFar(SE_SWING);
    if (frame == 32)
      Sound_PlayFar(SE_SPRAY);
    if (frame == 60) {
      if (mode == 1)
        Func_081180e8(144);
      else
        Func_081180e8(145);
    }

    // The flashes, from frame 56.
    if (frame > 55) {
      i = 0;
      do {
        struct EffectStep *flash = &work->particles[i];
        s32 x;
        s32 life;
        s32 y;

        y = HI(flash->y);
        x = HI(flash->x) + centerX;
        life = flash->variant;
        if (life >= 0 && life <= 17)
          draw[0](canvas,
                  work->sheet + (gData_08198a9e[life / 3] << 11) + FLASH_SHEET,
                  x - 16, y + 48, 32, 64);
        life = flash->variant;
        flash->variant = life > 0 ? life - 1 : -1;
        i++;
      } while (i != 16);
    }

    // The dust: a fountain at frame 28, then 16 more a frame until 63.
    if (frame == 28) {
      i = 0;
      do {
        struct EffectStep *dust = &DUST[i];

        if (dust->variant == -1) {
          s32 speed;
          s32 angle;

          speed = Random16() & 63;
          angle = Random16() & 0xFFFF;
          dust->x = ((Trig_Sin(angle) * speed) >> 3) + origin;
          dust->y = ((Trig_Cos(angle) * speed) >> 2) + (192 << 15);
          dust->velocityX = ((Random16() & 63) - 32) << 14;
          dust->velocityY = (-(Random16() & 63) - 8) << 13;
          dust->variant = 0;
        }
        i++;
      } while (i != 128 << 1);
    }

    step = frame - 32;
    if (step >= 0 && step <= 31) {
      s32 spawned;

      spawned = 0;
      i = 0;
      do {
        struct EffectStep *dust = &DUST[i];

        if (dust->variant == -1) {
          s32 speed;
          s32 angle;

          speed = Random16() & 63;
          angle = Random16() & 0xFFFF;
          dust->x = ((Trig_Sin(angle) * speed) >> 3) + origin;
          dust->y = ((Trig_Cos(angle) * speed) >> 2) + (192 << 15);
          dust->velocityX = ((Random16() & 63) - 32) << 14;
          dust->velocityY = (-(Random16() & 63) - 8) << 13;
          dust->variant = 0;
          spawned++;
          if (spawned == 16)
            break;
        }
        i++;
      } while (i != 171 << 2);
    }

    if (step >= 0 && step <= 31) {
      s32 phase = frame * 16;
      s32 height = 104;
      u8 *src = work->sheet;
      s32 x = centerX - 17;
      DrawRectangle blit = draw[0];
      s32 size = (phase - 256) % height;

      blit(canvas, src, x, 4 - size, 34, height);
      blit(canvas, src, x, 108 - size, 34, size);
    }

    // The dust grows as it falls and toward the end.
    if (frame <= 71) {
      i = 0;
      do {
        struct EffectStep *dust = &DUST[i];

        if (dust->variant >= 0) {
          s32 size;

          size = i % 3 + 2;
          if (dust->velocityY > 0)
            size += 2;
          if (frame > 68 && size <= 5)
            size = 6;
          if (frame > 70 && size <= 6)
            size = 7;
          if (frame > 72 && size <= 7)
            size = 8;
          if (frame > 74 && size <= 8)
            size = 9;
          if (frame > 76)
            size = 10;
          draw[0](canvas, cells + gData_08197410[size - 1],
                  HI(dust->x) - size / 2, HI(dust->y) - size, size, size * 2);

          dust->x += dust->velocityX;
          dust->y += dust->velocityY;
          if (frame > 80)
            dust->velocityY += -32768;
          else
            dust->velocityY += gData_08198aa4[i & 3];
          dust->velocityX = dust->velocityX * 62 / 64;
          dust->velocityY = dust->velocityY * 62 / 64;
          dust->variant++;
          if (dust->velocityY > 0 && HI(dust->y) > 108)
            dust->variant = -1;
        }
        i++;
      } while (i != 171 << 1);
    }

    // The sword: down for 10 frames, held, sinking from 33, plunging after
    // 60; nothing below y = 108 is drawn.
    if (frame <= 95) {
      s32 x;
      s32 high;
      s32 top;

      x = centerX - 18;
      high = 120;
      if (frame > 60)
        top = frame * 8 - 450;
      else if (frame > 32)
        top = step / 2 + 16;
      else if (frame <= 9)
        top = frame * 16 - 128;
      else
        top = 16;
      if (top + 120 > 108)
        high = high - top - 12;
      if (high > 0)
        draw[1](canvas, work->sheet + SWORD_SHEET, x, top, 36, high);
    }

    // The sparks bounce on the ground.
    if (frame > 59) {
      i = 0;
      do {
        struct EffectStep *spark = &SPARK[i];

        if (spark->variant > 0) {
          s32 x;
          s32 y;
          s32 size;

          BattleEffectKernels_IntegrateVector2(spark, 64, 128 << 6);
          y = spark->y;
          spark->variant--;
          if (y > (216 << 15)) {
            spark->velocityY = -spark->velocityY / 2;
          } else {
            x = spark->x;
            if ((x >= 0 && x <= 0x7effff) && y >= 0) {
              u8 *cell;

              size = spark->variant / 5 + 1;
              cell = cells + gData_08197410[size - 1];
              x >>= 16;
              y >>= 16;
              x -= size / 2;
              y -= size;
              draw[0](canvas, cell, x, y, size, size * 2);
            }
          }
        }
        i++;
      } while (i != 170 << 1);
    }

    // The hit, and the canvas flooded at frames 9 and 60.
    if (frame == 68) {
      for (i = 0; i != effect->count; i++) {
        Func_0814cd48(effect->actors[i], 7, 5, i, 16);
        Func_08118088(effect->actors[i], 7);
      }
    }

    if (frame == 9) {
      Func_0814cd48(effect->actors[0], 7, 5, 0, 8);
      Iwram_FillWords(canvas, 128 << 7, 0x3F3F3F3F);
    }
    if (frame == 60)
      Iwram_FillWords(canvas, 128 << 7, 0x3F3F3F3F);

    BattleCamera_Shake(16, 16);
    BattleEffect_CountDownBattlerTints();
    work->transferPending = 1;
    wait(1);
    frame++;
  } while (frame != 102);

  Task_Remove(BattleEffect_ProcessPendingGraphicsTransfer);
  HeapSlot_Release(HEAP_BLITTER_ALTERNATE);
  HeapSlot_Release(HEAP_BLITTER);
  BattleEffect_EndCanvasLayer();
}

struct Model;
struct StripQuad;

struct ModelTexture {
  u32 logWidth : 8;
  u32 logHeight : 8;
  u32 unk02 : 16;
  const void *pixels;
};

struct RenderModel {
  s32 kind;
  s32 unk04;
  const void *mesh;
  void *vertices;
  struct ModelTexture *texture;
  s32 shade;
  u8 scrollU;
  u8 scrollV;
};

void Func_0815b434(u8 *map, s32 size, s32 scale); // shaded circles
void Func_0816ae40(u8 *src, u8 *dst, s32 width, s32 height);
void Func_0815585c(struct BattleEffectArgument *effect, u32 packed, s32 *screen,
                   s32 *world);
void Func_0818caa8(void *cells, s32 x, s32 y, s32 size);
struct PoolEntry *Render_AllocateModelList(s32 count);
void Render_TransformVertices(const void *corners, void *vertices, s32 count);
void Render_DrawModelList(struct Model *model);
void Func_08015160(s32 x, s32 y, s32 z);
void Func_0801521c(s32 scale);
void SceneTransform_ApplyPitch(s32 angle);

void Func_080151e4(s32 x, s32 y, s32 z); // SceneTransform_ApplyScale3

void BattleEffect_StartActorMotion(s32 id, s32 force, s32 acceleration,
                                   s32 speedLimit, s32 velocityY, s32 percent);

void Render_BuildQuadStrip(struct StripQuad *quad, s32 width, s32 height,
                           s32 count);

s32 BattleUnit_GetEffectHeightFar(s32 unit);
void BattleEffect_StartTiledCanvas(s32 bgControl);
void BattleEffect_StartTiledCanvasFilled(s32 bgControl);
void Transform_Pop(void);

// The blitter's heap slot, as a byte offset into Ram_HeapSlots.
#define SLOT_BLITTER 104 // unk64[1]

#include "io_write_queue.h"

struct ResourceTableEntry {
  u16 unk0;
  u16 tile;
};

struct JudgmentObject {
  u8 unk0[9];
  u8 flagsLow : 2;
  u8 priority : 2;
  u8 flagsHigh : 4;
  u8 unk0a[6];
  u8 resource;
  u8 unk11[9];
  u8 unk1a;
};

extern char ResourceId_VioletSkyBackdrop;
extern char ResourceId_AerialViewBackdrop;
extern char ResourceId_BeachBackdrop;
extern char ResourceId_ForestBackdrop;
extern char ResourceId_WingedKnightATiles;
extern char ResourceId_WingedKnightBTiles;
extern char ResourceId_WingedKnightCTiles;
extern char ResourceId_PinkBurstSheet;
extern char ResourceId_TornadoSheet;
extern char ResourceId_Data0C2;

extern char ResourceId_BlueCrescentSheet;
extern char ResourceId_ExplosionSheet;
extern char ResourceId_OverlayImageA;

extern struct ResourceTableEntry gResourceTableEntries[];
extern u8 gData_02014000[];

struct JudgmentScale {
  s32 x;
  s32 y;
};

extern const struct JudgmentScale Data_08196e74;
extern const u8 Data_08198ab4[];
extern const u8 Data_08198ace[];
extern const u8 Data_08198ad4[];
extern const u16 Data_08198ac2[];
extern const u8 gData_08199364[];
extern const u8 gData_08199210[];

struct SpriteObject *Func_0815b290(s32 width, s32 height, u32 shape,
                                   u32 attributes);

void Palette_LoadBasicObjColors(void);
void BattleStaging_ConfigurePaletteFadeFar(s32 mode, s32 background, s32 speed);
void Graphics_LoadGreyRamp(void);

#define SLOT_ALT 188 // unkb8[1]
static inline void FILL(void *dst, s32 size, s32 value) {
  do {
    Iwram_FillWords(dst, size, value);
  } while (0);
}
static inline void FillWords(void *dst, s32 size, s32 value) {
  /* FAKEMATCH(wrapper): the closing fill of the gathering calls through this
   * inline with no block; as a direct call to Iwram_FillWords there, 14 of
   * 2680 instructions differ in tla-en. */
  Iwram_FillWords(dst, size, value);
}
#define DRAW_ALT(mode, sheet, x, y, w, h)                                      \
  BattleEffect_LoadBlitter(SLOT_ALT, mode);                                    \
  ((DrawRectangle)Ram_HeapSlots->unkb8[1])(canvas, sheet, x, y, w, h);         \
  HeapSlot_Release(SLOT_ALT)
// The same draw, the routine read through a pointer to its slot.
#define DRAW_ALT_THROUGH(slot, mode, sheet, x, y, w, h)                        \
  BattleEffect_LoadBlitter(SLOT_ALT, mode);                                    \
  (*(slot))(canvas, sheet, x, y, w, h);                                        \
  HeapSlot_Release(SLOT_ALT)
#define OBJ_TILES(obj)                                                         \
  (0x06010000 + gResourceTableEntries[(obj)->resource].tile)

// IO_WRITE_QUEUE_PUSH with an empty once-only block before the count store,
// the value store and the address store: the first tile queue matches only
// with each of the three parted from the one before.
#define QUEUE_PUSH_PARTED(address, value, control)                             \
  {                                                                            \
    struct IoWriteQueue *q = &gIoWriteQueue;                                   \
    u32 saved;                                                                 \
    s32 count;                                                                 \
                                                                               \
    IRQ_LOCK(saved);                                                           \
    count = *&q->count;                                                        \
    if (count <= 31) {                                                         \
      u32 *entry = q->entries[count];                                          \
                                                                               \
      do {                                                                     \
      } while (0);                                                             \
      *&q->count = count + 1;                                                  \
      do {                                                                     \
      } while (0);                                                             \
      *entry++ = (value);                                                      \
      do {                                                                     \
      } while (0);                                                             \
      *entry++ = (address);                                                    \
      *entry = (control);                                                      \
    }                                                                          \
    IRQ_UNLOCK(saved);                                                         \
  }

static inline s32 CopyWords(void *destination, const void *source, s32 size) {
  /* FAKEMATCH(wrapper): the first palette copy calls through this inline; as a
   * direct call to Iwram_CopyWords there, 103 of 2680 instructions differ in
   * tla-en. */
  return Iwram_CopyWords(destination, source, size);
}

static inline u8 *PassBuffer(u8 *buffer) {
  /* FAKEMATCH(wrapper): the second queue's source base passed through this
   * inline; as Ram_MapCellBuffer, 52 of 2680 instructions differ in tla-en. */
  return buffer;
}

// Judgment: the sky gathers sixteen tiles of light that fold into a sigil,
// then the sigil's beam strikes and the ground erupts in a ring of
// shockwaves and debris.
void BattleEffect_RunJudgment(struct BattleEffectArgument *effect) {
  s32 anchor[3];
  struct EffectPosition pos;
  s32 place[4];
  s32 drift[4];
  struct JudgmentScale req;
  struct ModelTexture texture;
  struct ModelTexture glare;
  s32 scale[2];
  u8 *canvas;
  struct BattleEffectWork *work;
  s32 *transition;
  DrawRectangle draw[2];
  s32 frame;
  u8 *sprites;
  s32 depth;
  s32 zoom;
  s32 addR, addG, addB;
  s32 step;
  s32 cx, cy, squash, sx, ring, rings, shift;
  s32 i;
  u8 *cells;
  u8 *pal;
  s32 width;

  canvas = Ram_HeapSlots->battleFxWork[1];
  work = (struct BattleEffectWork *)Ram_HeapSlots->battleFxWork[0];
  transition = Ram_HeapSlots->transitionWork;
  sprites = Ram_HeapSlots->unk64[0];
  /* FAKEMATCH(temporary): the cell buffer's address in a local; as
   * Ram_MapCellBuffer at the first tile copies and the first queue, 2 of
   * 2680 instructions differ in tla-en. */
  cells = Ram_MapCellBuffer;
  BattleEffect_StartCanvasLayer(0x2000);
  REG_BG2PA = 0x100;
  BattleEffect_ConfigureEffectDisplay();
  ((volatile u16 *)BG_PLTT)[0] = 0;
  ((volatile u16 *)BG_PLTT)[1] = 0;
  BattleEffect_LoadBlitter(SLOT_BLITTER, 3);
  work->transferMode = 0;
  draw[0] = (DrawRectangle)Ram_HeapSlots->unk64[1];
  Task_Add(BattleEffect_ProcessPendingGraphicsTransfer, 0xc80);
  Func_08163c2c(0, 0);
  BattleEffect_SelectLivingTargets(work->effect);
  BattleBackground_LoadFar(1, (s32)&ResourceId_VioletSkyBackdrop, 0);

  pal = Resource_GetTableEntry((s32)&ResourceId_WingedKnightATiles);
  CopyWords((void *)OBJ_PLTT, pal, 512);
  pal += 512;
  Resource_DecodeType01(pal, cells);
  for (i = 0; i != 16; i++) {
    struct JudgmentObject *obj = Func_0815b290(32, 32, 0x80002000, 0);

    obj->priority = 2;
    work->objects[i] = obj;
    Iwram_CopyWords((void *)OBJ_TILES(obj), cells + i * 0x400, 0x400);
  }
  gCameraSceneParameters[4] = 240;
  wait(1);
  transition[4] = 1;
  gBgScroll[1].x = 0;
  Func_08163c2c(0, 1);
  REG_DISPCNT = 0x7741;
  REG_BG2PA = 0x80;
  REG_BLDALPHA = 0x1010;
  REG_BLDCNT = 0x3f44;
  depth = -0x400000;
  zoom = 0x80000;
  Resource_LoadAndDecompress((s32)&ResourceId_PinkBurstSheet, work->sheet, 1,
                             1);
  Resource_LoadAndDecompress((s32)&ResourceId_TornadoSheet, work->sheet + 0x600,
                             1, 0);
  Resource_LoadAndDecompress((s32)&ResourceId_Data0C2, gData_02014000, 0, 0);
  Resource_LoadAndDecompress((s32)&ResourceId_Data134, sprites, 0, 0);
  pal = Resource_GetTableEntry((s32)&ResourceId_BlueCrescentSheet);
  Iwram_CopyWords((void *)BG_PLTT, pal, 128);
  work->transferMode = 2;
  work->transferValue = 50;
  REG_BG2CNT = 0x784;

  // The gathering: motes rise, the tiles fold in and flash.
  frame = 0;
  do {
    if (frame == 0) {
      struct EffectStep *p = work->particles;

      for (i = 0; i != 64; i++, p++) {
        p->x = (Random16() & 15) + 72;
        p->velocityX = Random16() & 0xffff;
        p->velocityY = Random16() & 0xffff;
        p->velocityY = Random16() & 0xffff;
      }
      pal = Resource_GetTableEntry((s32)&ResourceId_WingedKnightATiles);
      Iwram_CopyWords((void *)OBJ_PLTT, pal, 512);
      pal += 512;
      Resource_DecodeType01(pal, Ram_MapCellBuffer);
      for (i = 0; i != 16; i++) {
        struct JudgmentObject *obj = work->objects[i];

        Iwram_CopyWords((void *)OBJ_TILES(obj), Ram_MapCellBuffer + i * 0x400,
                        0x400);
      }
      depth = -0x400000;
      zoom = 0x80000;
    }
    if (frame == 143) {
      FILL(canvas, 0x4000, 0x2a2a2a2a);
      Sound_PlayFar(145);
    }
    if (frame == 80) {
      Sound_PlayFar(142);
    }
    if (frame == 72) {
      work->fadeFrames = 24;
      work->fadeStep = 0;
    }

    anchor[2] = 0;
    anchor[1] = 0;
    {
      struct EffectStep *p = work->particles;

      for (i = 0; i != 64; i++, p++) {
        s32 size;

        if (frame > i / 4 + 80 && p->x > 0) {
          Render_ResetTransformState();
          Func_080150e4(p->velocityY);
          SceneTransform_ApplyPitch(p->velocityX);
          SceneTransform_ApplyYaw(p->velocityY + (frame << 9));
          anchor[0] = p->x;
          p->x -= 2;
          if (p->x < 0) {
            p->x = 0;
          }
          EffectPosition_ProjectPoint(anchor, &pos);
          if (pos.depth < -60) {
            pos.depth = -60;
          }
          if (pos.depth > 60) {
            pos.depth = 60;
          }
          pos.depth += 60;
          size = pos.depth / 20 + 2;
          pos.x += 56;
          pos.y += 96;
          draw[0](canvas, sprites + gData_08197410[size - 1], pos.x - size / 2,
                  pos.y - size, size, size * 2);
        }
      }
    }

    req = Data_08196e74;
    place[3] = 0;
    place[1] = 0;
    for (i = 0; i != 16; i++) {
      if (frame > 75 && (i == 8 || i == 12 || i == 15)) {
        continue;
      }
      place[0] = ((i % 4) << 21) + (152 << 15);
      place[2] = ((i / 4) << 21) + depth;
      Render_ApplyProjectedPlacementFar(work->objects[i], place, (s32 *)&req,
                                        0);
    }
    depth += zoom;
    if (frame <= 47) {
      zoom -= 0x4000;
    }
    if (frame > 32) {
      zoom = zoom * 60 / 64;
    }
    if (frame == 144) {
      zoom = -0x80000;
    }
    if (frame == 146) {
      zoom = 0x10000;
    }

    if (frame == 72) {
      pal = Resource_GetTableEntry((s32)&ResourceId_WingedKnightBTiles);
      Iwram_CopyWords((void *)OBJ_PLTT, pal, 512);
      pal += 512;
      Resource_DecodeType01(pal, Ram_MapCellBuffer);
      for (i = 0; i != 16; i++) {
        QUEUE_PUSH_PARTED(OBJ_TILES((struct JudgmentObject *)work->objects[i]),
                          (u32)(cells + i * 0x400), 0x84000100);
      }
    }
    if (frame == 76) {
      pal = Resource_GetTableEntry((s32)&ResourceId_WingedKnightCTiles);
      Iwram_CopyWords((void *)OBJ_PLTT, pal, 512);
      pal += 512;
      Resource_DecodeType01(pal, Ram_MapCellBuffer);
      for (i = 0; i != 13; i++) {
        IO_WRITE_QUEUE_PUSH(
            OBJ_TILES((struct JudgmentObject *)work->objects[Data_08198ab4[i]]),
            (u32)(PassBuffer(Ram_MapCellBuffer) + i * 0x400), 0x84000100);
      }
    }

    if ((u32)(frame - 116) <= 27) {
      s32 n = frame * 3 - 346;
      if (n > 80) {
        n = 80;
      } else {
        Func_0815b434(work->sheet + 0x1256, n, 0x10000);
      }
      Func_0818caa8(work->sheet + 0x1256, 60, 80, n);
      /* FAKEMATCH(dead): the test of the block it stands in, made again; as a
       * plain block, 8 of 2680 instructions differ in tla-en. */
      if ((u32)(frame - 116) <= 27) {
        s32 size = (frame - 116) / 3 + 16;

        if (size > 32) {
          size = 32;
        }
        for (i = 0; i != 6; i++) {
          s32 k = i & 3;
          s32 a = Random16() & 0xffff;
          s32 x = ((size * Trig_Sin(a)) >> 16) - (Data_08198ace[k] >> 1);
          s32 y = ((size * Trig_Cos(a)) >> 16) - (Data_08198ad4[k] / 2);

          draw[0](canvas, work->sheet + Data_08198ac2[k], x + 60, y + 80,
                  Data_08198ace[k], Data_08198ad4[k]);
        }
      }
    }

    {
      s32 flag = 0;

      if ((u32)(frame - 108) <= 34) {
        s32 t = (frame - 108) / 2;

        addR = t / 4;
        addG = t / 2;
        addB = t;
        flag = 1;
      }
      if (frame == 143) {
        addB = addG = addR = 31;
        flag = 1;
      }
      if ((u32)(frame - 144) <= 8) {
        /* FAKEMATCH(temporary): the step taken through a local assigned twice;
         * as addB = addR = addG = 304 - frame * 2, 40 of 2680 instructions
         * differ in tla-en. */
        addB = addR = addG = step = (step = 304 - frame * 2);
        flag = 1;
      }
      if (flag == 1) {
        u16 *src = Resource_GetTableEntry((s32)&ResourceId_WingedKnightCTiles);
        volatile u16 *dst = (volatile u16 *)OBJ_PLTT;

        for (i = 0; i != 256; i++, src++, dst++) {
          s32 r = (*src & 31) + addR;
          s32 g = ((*src >> 5) & 31) + addG;
          s32 b = ((*src >> 10) & 31) + addB;

          if (r > 31)
            r = 31;
          if (g > 31)
            g = 31;
          if (b > 31)
            b = 31;
          if (r < 0)
            r = 0;
          if (g < 0)
            g = 0;
          if (b < 0)
            b = 0;
          *dst = b << 10 | g << 5 | r;
        }
      }
    }

    if (frame > 143) {
      s32 y = frame * 16 - 2272;

      DRAW_ALT(3, work->sheet, 36, y, 24, 64);
      /* FAKEMATCH(dead): a test that is never true, cells being the cell
       * buffer's address, kept between the two draws so that each builds
       * the sheet's address anew; as no test, 104 of 2680 instructions differ
       * in tla-en. */
      if (!cells && !canvas)
        y = 0;
      DRAW_ALT(7, work->sheet, 60, y, 24, 64);
    }

    if ((u32)(frame - 144) <= 15) {
      s32 s;
      s32 shade;
      void *verts;
      struct RenderModel *model;

      verts = Heap_AllocEwram(32);
      model = Render_AllocateModelList(1);
      shade = 0;
      s = (frame << 13) - 0x11e000;
      texture.logWidth = 7;
      texture.logHeight = 7;
      texture.pixels = gData_02014000;
      model->texture = &texture;
      model->kind = 7;
      model->mesh = gData_08199364;
      model->vertices = verts;
      if (frame > 151) {
        shade = (frame * -8) + 1216;
      }
      model->shade = shade;
      Render_ResetTransformState();
      Func_08015160(-0x40000, 0x180000, 0);
      Func_0801521c(s);
      SceneTransform_ApplyPitch(3000);
      Render_TransformVertices(gData_08199210, verts, 4);
      Render_DrawModelList(model);
      Heap_Release(model);
      Heap_Release(verts);
    }

    work->transferPending = 1;
    wait(1);
    frame++;
  } while (frame != 160 && (frame <= 4 || !(gInput.repeat & 3)));

  FillWords(canvas, 0x4000, 0);
  for (i = 0; i != 16; i++) {
    ResourceObject_ReleaseFar(work->objects[i]);
  }
  wait(1);
  Palette_LoadBasicObjColors();
  Dma_Set((const void *)OBJ_PLTT, (void *)&BG_PLTT_COLOR(5, 0), 0x80000010,
          REG_DMA3);
  BG_PLTT_COLOR(5, 14) = BG_PLTT_COLOR(15, 4);
  for (i = 0; i != 16; i++) {
    struct JudgmentObject *obj = ResourceObject_CreateFar(0x28d);

    work->objects[i] = obj;
    if (obj) {
      obj->unk1a = 0;
      Animation_ApplyChildArgumentFar(obj, i % 3);
      ((struct JudgmentObject *)work->objects[i])->priority = 0;
    }
  }
  HeapSlot_Release(SLOT_BLITTER);
  BattleEffect_LoadBlitter(SLOT_BLITTER, 19);
  draw[0] = (DrawRectangle)Ram_HeapSlots->unk64[1];
  Resource_LoadAndDecompress((s32)&ResourceId_BlueCrescentSheet,
                             work->sheet + 0x4000, 1, 1);
  {
    u8 *c = sprites;

    for (i = 0; i != 770; i++, c++) {
      if (*c > 32) {
        *c += 224;
      } else if (*c != 0) {
        *c = 1;
      }
    }
  }
  work->fadeFrames = 0;
  work->fadeStep = 0;
  BattleBackground_LoadFar(1, (s32)&ResourceId_AerialViewBackdrop, 0);
  REG_BG2PA = 0x100;
  REG_BG2X = -0x3c00;
  REG_BG2CNT = 0x784;
  FILL(canvas, 0x4000, 0);
  cx = 60;
  cy = 44;
  squash = 6;
  ring = 0;
  shift = 0;
  sx = 8;
  rings = 2;
  work->transferMode = 2;
  work->transferValue = 75;
  Iwram_FillWords(work->sheet, 0x3840, 0);

  // The strike: the beam, the shockwave rings and the debris.
  for (frame = 0; frame != 336; frame++) {
    if (frame == 66) {
      Sound_PlayFar(208);
    }
    if (frame == 88) {
      Sound_PlayFar(230);
    }
    if (frame == 155) {
      Sound_PlayFar(162);
    }
    if (frame == 217) {
      Sound_PlayFar(156);
    }
    if (frame == 280) {
      Sound_PlayFar(157);
    }
    if (frame == 300) {
      for (i = 0; i != effect->count; i++) {
        Func_0814cd48(effect->actors[i], 7, 5, i, 8);
        Func_08118088(effect->actors[i], 1);
      }
      Func_081180e8(145);
    }
    if (gInput.repeat & 3) {
      if ((u32)(frame - 5) <= 144) {
        frame = 150;
        IO_WRITE_QUEUE_PUSH(0x04000020, 128, 0x20000);
        IO_WRITE_QUEUE_PUSH(0x04000028, 0, 0x30000);
        Resource_LoadAndDecompress((s32)&ResourceId_ExplosionSheet,
                                   work->sheet + 0x4000, 1, 0);
      } else if ((u32)(frame - 155) <= 58) {
        frame = 214;
      } else if ((u32)(frame - 219) <= 60) {
        frame = 280;
      }
    }
    if (frame == 318) {
      FILL(canvas, 0x4000, 0x3f3f3f3f);
    }
    if (frame == 319) {
      Graphics_LoadGreyRamp();
      work->transferMode = 3;
      work->transferValue = 0x04040404;
    }
    if (frame == 64) {
      IO_WRITE_QUEUE_PUSH(0x04000020, 128, 0x20000);
      IO_WRITE_QUEUE_PUSH(0x04000028, 0, 0x30000);
      FILL(canvas, 0x4000, 0);
      Resource_LoadAndDecompress((s32)&ResourceId_ExplosionSheet,
                                 work->sheet + 0x4000, 1, 0);
      Resource_LoadAndDecompress((s32)&ResourceId_OverlayImageA,
                                 work->sheet + 0x6000, 0, 0);
    }
    if (frame == 66) {
      volatile u16 *c = &BG_PLTT_COLOR(6, 0);

      for (i = 0; i != 128; i++, c++) {
        *c ^= 0x7fff;
      }
    }
    if (frame == 69) {
      FILL(canvas, 0x4000, 0x3f3f3f3f);
    }
    if (frame == 70) {
      BattleStaging_ConfigurePaletteFadeFar(
          1, (s32)&ResourceId_AerialViewBackdrop, 7);
    }
    if (frame == 150) {
      cx = 112;
      cy = 32;
      ring = 0;
      rings = 4;
      squash = 8;
      FILL(work->sheet, 0x3840, 0);
      FILL(canvas, 0x4000, 0x3f3f3f3f);
      BattleBackground_LoadFar(1, (s32)&ResourceId_BeachBackdrop, 0);
    }
    if (frame == 214) {
      FILL(work->sheet, 0x3840, 0);
      FILL(canvas, 0x4000, 0x3f3f3f3f);
      BattleBackground_LoadFar(1, (s32)&ResourceId_ForestBackdrop, 0);
      {
        struct EffectStep *p = work->particles;

        for (i = 0; i != 16; i++, p++) {
          p->x = ((Random16() & 63) + 32) << 16;
          p->y = ((Random16() & 31) + 96) << 16;
          p->velocityY = ((Random16() & 15) + 16) << 15;
          p->z = 0x8000 - (p->x >> 7);
          p->variant = 1;
        }
      }
    }
    if (frame == 280) {
      Func_0814cca8();
      FILL(work->sheet, 0x3840, 0);
      FILL(canvas, 0x4000, 0x01010101);
      transition[4] = 0;
      REG_BLDALPHA = 0x1010;
      {
        struct EffectStep *p = work->particles;

        for (i = 0; i != 16; i++, p++) {
          p->x = ((Random16() & 63) + 256) << 16;
          p->y = ((Random16() & 31) + 96) << 16;
          p->velocityY = ((Random16() & 15) + 16) << 15;
          p->z = 0;
        }
      }
      shift = 272;
      work->transferMode = 0;
      work->transferValue = 0;
    }
    if ((u32)(frame - 280) <= 38) {
      volatile u16 *c = &BG_PLTT_COLOR(0, 1);

      for (i = 0; i != 63; i++, c++) {
        s32 v = *c;
        s32 b = ((u16)v >> 10) & 31;
        s32 g = ((u16)v >> 5) & 31;
        s32 r = v & 31;

        b++;
        g++;
        r++;

        if (b > 31)
          b = 31;
        if (g > 31)
          g = 31;
        if (r > 31)
          r = 31;
        *c = b << 10 | g << 5 | r;
      }
    }
    if (frame == 182) {
      FILL(canvas, 0x4000, 0x3f3f3f3f);
    }

    if (frame <= 63) {
      u8 *sheet;
      s32 top = frame - 4;
      DrawRectangle *slot;

      if ((frame & 7) < 4) {
        sheet = work->sheet + 0x4000;
      } else {
        sheet = work->sheet + 0x4240;
      }
      /* FAKEMATCH(temporary): a pointer to the routine's slot, set here after
       * the sheet is chosen and before the first blitter is loaded; as
       * Ram_HeapSlots->unkb8[1] at each draw, 990 of 2680 instructions differ
       * in tla-en. */
      slot = (DrawRectangle *)&Ram_HeapSlots->unkb8[1];
      DRAW_ALT_THROUGH(slot, 19, sheet, 36, top - 24, 24, 24);
      DRAW_ALT_THROUGH(slot, 23, sheet, 59, top - 24, 24, 24);
      DRAW_ALT_THROUGH(slot, 27, sheet, 36, top - 1, 24, 24);
      DRAW_ALT_THROUGH(slot, 31, sheet, 59, top - 1, 24, 24);
    }
    if ((u32)(frame - 64) <= 1) {
      draw[0](canvas, work->sheet + 0x6000, 52, 51, 16, 17);
    }
    if ((u32)(frame - 66) <= 1) {
      /* FAKEMATCH(temporary): the width through a local set at both draws of
       * this picture; as 24 at both, 116 of 2680 instructions differ in
       * tla-en. */
      width = 24;
      draw[0](canvas, work->sheet + 0x6110, 48, 40, width, 41);
    }
    if ((u32)(frame - 68) <= 7) {
      s32 d = 76 - frame;

      draw[0](canvas, work->sheet + 0x64e8, 49 - d / 2, 38 - d, 22, 44);
      draw[0](canvas, work->sheet + 0x64e8, 49 - d / 2, d + 38, 22, 44);
      draw[0](canvas, work->sheet + 0x64e8, d / 2 + 49, 38 - d, 22, 44);
      draw[0](canvas, work->sheet + 0x64e8, d / 2 + 49, d + 38, 22, 44);
    }
    if ((u32)(frame - 78) <= 1) {
      draw[0](canvas, work->sheet + 0x6000, 52, 51, 16, 17);
    }
    if ((u32)(frame - 80) <= 1) {
      width = 24;
      draw[0](canvas, work->sheet + 0x6110, 48, 40, width, 41);
    }
    if ((u32)(frame - 82) <= 3) {
      s32 d = frame * 2 - 164;

      draw[0](canvas, work->sheet + 0x64e8, 49 - d / 4, 38 - d, 22, 44);
      draw[0](canvas, work->sheet + 0x64e8, 49 - d / 4, d + 38, 22, 44);
      draw[0](canvas, work->sheet + 0x64e8, d / 4 + 49, 38 - d, 22, 44);
      draw[0](canvas, work->sheet + 0x64e8, d / 4 + 49, d + 38, 22, 44);
    }

    if ((u32)(frame - 72) <= 15) {
      s32 s;
      s32 shade;
      void *verts;
      struct RenderModel *model;

      verts = Heap_AllocEwram(32);
      model = Render_AllocateModelList(1);
      s = (frame << 13) - 0x8f800;
      glare.logWidth = 7;
      glare.logHeight = 7;
      glare.pixels = gData_02014000;
      shade = 0;
      model->kind = 7;
      model->texture = &glare;
      model->mesh = gData_08199364;
      model->vertices = verts;
      if (frame > 79) {
        shade = (frame * -8) + 640;
      }
      model->shade = shade;
      Render_ResetTransformState();
      Func_08015160(-0x40000, 0x80000, 0);
      Func_0801521c(s);
      SceneTransform_ApplyPitch(5000);
      Render_TransformVertices(gData_08199210, verts, 4);
      Render_DrawModelList(model);
      Heap_Release(model);
      Heap_Release(verts);
    }

    if (frame > 85) {
      if (frame <= 213) {
        for (i = 0; i != rings; i++, ring++) {
          s32 radius = ring;
          s32 slope = 0;
          s32 err = ring;

          while (radius >= slope) {
            s32 half, halfSlope, top, bottom, left, right, dx;

            half = radius / 2;
            dx = (half * sx) / 8;
            /* FAKEMATCH(dead): a store top's own line overwrites; its test
             * builds 1000 in a reload register before cx's reload, the one
             * pick that sends cx to r4; without it, 28 of 2680 instructions
             * differ in tla-en. */
            if (dx > 1000)
              top = 0;
            right = cx + dx;
            /* FAKEMATCH(dead): a branch whose store the next line overwrites
             * restores the label CSE and reload saw between the edges; without
             * it, 231 of 2680 instructions differ in tla-en. */
            if (dx > cx)
              left = 0;
            left = cx - dx;
            bottom = cy + (slope * squash) / 8;
            top = cy - (slope * sx) / 8;
            if (top < 0)
              top = 0;
            if (bottom > 119)
              bottom = 119;
            if (left < 0)
              left = 0;
            if (right > 119)
              right = 119;
            work->sheet[bottom * 120 + right] = 20;
            work->sheet[top * 120 + right] = 20;
            work->sheet[bottom * 120 + left] = 20;
            work->sheet[top * 120 + left] = 20;
            right = cx + half + 1;
            left = cx - half + 1;
            if (left < 0)
              left = 0;
            if (right > 119)
              right = 119;
            work->sheet[bottom * 120 + right] = 20;
            work->sheet[top * 120 + right] = 20;
            work->sheet[bottom * 120 + left] = 20;
            work->sheet[top * 120 + left] = 20;

            halfSlope = slope / 2;
            dx = (halfSlope * sx) / 8;
            /* FAKEMATCH(dead): as above, the pick before cx's reload in the
             * second half; without it, 28 of 2680 instructions differ in
             * tla-en. */
            if (dx > 1000)
              top = 0;
            right = cx + dx;
            /* FAKEMATCH(dead): a branch whose store the next line overwrites
             * restores the label CSE and reload saw between the edges; without
             * it, 205 of 2680 instructions differ in tla-en. */
            if (dx > cx)
              left = 0;
            left = cx - dx;
            bottom = cy + (radius * squash) / 8;
            top = cy - (radius * sx) / 8;
            if (left < 0)
              left = 0;
            if (right > 119)
              right = 119;
            if (top < 0)
              top = 0;
            if (bottom > 119)
              bottom = 119;
            work->sheet[bottom * 120 + right] = 20;
            work->sheet[top * 120 + right] = 20;
            work->sheet[bottom * 120 + left] = 20;
            work->sheet[top * 120 + left] = 20;
            right = cx + halfSlope + 1;
            left = cx - halfSlope + 1;
            if (left < 0)
              left = 0;
            if (right > 119)
              right = 119;
            work->sheet[bottom * 120 + right] = 20;
            work->sheet[top * 120 + right] = 20;
            work->sheet[bottom * 120 + left] = 20;
            work->sheet[top * 120 + left] = 20;

            err = err - slope * 2 - 1;
            if (err < 0) {
              err = err + radius * 2 - 2;
              radius--;
            }
            slope++;
          }
        }
      }
    }

    if ((u32)(frame - 86) <= 63) {
      s32 angle = -0x4000;

      for (i = 0; i != 9; i++, angle += 0x1000) {
        s32 x = ((frame - 86) * Trig_Sin(angle)) >> 16;
        s32 y = ((frame - 70) * Trig_Cos(angle)) >> 16;

        draw[0](canvas, work->sheet + ((frame / 2 + i) % 4) * 1728 + 0x4000,
                x + 44, y + 17, 32, 54);
      }
    }

    BattleEffect_LoadBlitter(SLOT_ALT, 3);
    draw[1] = (DrawRectangle)Ram_HeapSlots->unkb8[1];
    if (frame > 85) {
      draw[1](canvas, work->sheet, 0, 0, 120, 120);
    }
    if (frame > 279) {
      shift -= 8;
    }
    if (frame == 238) {
      FILL(canvas, 0x4000, 0x3f3f3f3f);
    }
    if ((u32)(frame - 214) <= 65) {
      gBgScroll[1].x += 8;
    }
    if ((u32)(frame - 246) <= 33) {
      shift += 8;
      work->transferMode = 0;
      work->transferValue = 75;
    }

    if (frame > 213) {
      struct EffectStep *p;

      drift[3] = 0;
      drift[1] = 0;
      for (i = 0; i != 16; i++) {
        p = &work->particles[i];
        if (p->variant != 0) {
          scale[0] = p->z + (p->y >> 7);
          if (scale[0] <= 0x7ff) {
            scale[0] = 0x800;
          }
          scale[1] = scale[0];
          drift[0] = p->x;
          drift[1] = 0xff0000;
          drift[2] = p->y + 0xff0000;
          Render_ApplyProjectedPlacementFar(work->objects[i], drift, scale, 0);
          p->y -= p->velocityY;
          if (p->y <= 0xfffff) {
            if (frame <= 279) {
              p->x = ((Random16() & 63) + shift + 32) << 16;
            } else {
              p->x = ((Random16() & 63) + shift - 32) << 16;
            }
            if (frame > 301) {
              p->variant = 0;
            } else if (frame > 245) {
              p->y = 0x600000;
            } else {
              p->y = 0xc00000 - p->x;
            }
          }
        }
      }
      if (frame <= 279) {
        for (i = 0; i != 8; i++) {
          s32 x =
              (i & 1) * 20 - (Random16() & 15) + (i / 2) * 4 + shift / 2 - 16;
          s32 y = (i / 2) * 32 - (Random16() & 15);

          draw[1](canvas, work->sheet + ((frame / 2 + i) % 4) * 1728 + 0x4000,
                  x, y, 32, 54);
        }
      } else {
        for (i = 0; i != 8; i++) {
          s32 x =
              (i & 1) * 20 + (Random16() & 15) - (i / 2) * 4 + shift / 2 - 16;
          s32 y = (i / 2) * 32 - (Random16() & 15);

          draw[1](canvas, work->sheet + ((frame / 2 + i) % 4) * 1728 + 0x4000,
                  x, y, 32, 54);
        }
      }
    }
    HeapSlot_Release(SLOT_ALT);
    if (frame > 63) {
      /* FAKEMATCH(temporary): the scroll value through a local; as stored
       * straight to gBgScroll[1].y, 2 of 2680 instructions differ in tla-en. */
      s32 y = Random16() % 4 + 30;

      gBgScroll[1].y = y;
    }
    BattleEffect_CountDownBattlerTints();
    work->transferPending = 1;
    wait(1);
  }

  for (i = 0; i != 16; i++) {
    ResourceObject_ReleaseFar(work->objects[i]);
  }
  Task_Remove(BattleEffect_ProcessPendingGraphicsTransfer);
  HeapSlot_Release(SLOT_BLITTER);
  BattleEffect_EndCanvasLayer();
}

#undef SE_FADE
#undef FORMATION
#undef SE_HIT

// The map cell buffer holds the hit bursts, 32 for each target.
#define PARTICLES ((struct EffectStep *)Ram_MapCellBuffer)

#define FORMATION 644

#define SE_GALLOP 136
#define SE_EMBERS 141
#define SE_FLAMES 154
#define SE_CHARGE 145
#define SE_FADE 137
#define SE_HIT 134

struct FxPair {
  s32 a;
  s32 b;
};

struct FxPlacement {
  s32 x;
  s32 y;
  s32 z;
  s32 unk0c;
};

struct FxControl {
  u8 unk00[16];
  s32 busy;
};

extern char ResourceId_FireBlobSheet;
extern struct FxPair gData_08196e7c; // ☀️ gObjectRow_SweepPair
extern struct FxPair gData_08196e84; // ☀️ gObjectRow_RisePair
extern u8 gData_08198ada[];          // the row's x offsets
extern u8 gData_08198ae3[];          // the row's heights

// Summon 3: the battlefield turns to forest; a row of nine riders gallops in
// from the right over a ring of embers, trailing fire, while flames climb at
// the left; then the row charges across, striking each target as it passes
// in a burst of sparks, and a wall of flame sweeps after it.
//
// Act one (120 frames, A or B ends it after frame 16): the backdrop scrolls
// faster from frame 24; the row bobs in, its riders on a sine wave, the
// screen shaking every 32 frames; the ember ring grows over the first 27
// frames; the fire trail from frame 18; the flames from 32. Act two (96
// frames): the row sweeps left 16 pixels a frame, rising; each target it
// passes takes a burst of 32 sparks; the flames slide across from frame 41.
void BattleEffect_RunKirin(struct BattleEffectArgument *effect) {
  struct FxPlacement place;
  struct EffectPosition unit;
  u8 jitterRow[16];
  u8 hitRow[14];
  struct FxPlacement place2;
  void *canvas;
  struct BattleEffectWork *work;
  DrawRectangle draw;
  u8 *sheet;
  s32 baseX;
  s32 grow;
  s32 savedX;
  struct FxControl *control;
  s32 shift;
  s32 wave;
  s32 frame;
  s32 i;

  canvas = Ram_HeapSlots->battleFxWork[1];
  work = (struct BattleEffectWork *)Ram_HeapSlots->battleFxWork[0];
  sheet = Ram_HeapSlots->unk64[0];

  // Open the canvas over the forest and spawn the row.
  BattleEffect_StartCanvasLayer(0);
  BattleEffect_ConfigureEffectDisplay();
  REG_BG2CNT = 0x784;
  *(u16 *)0x05000000 = 0;
  *(u16 *)0x05000002 = 0x18;
  work->transferMode = 0;
  Task_Add(BattleEffect_ProcessPendingGraphicsTransfer, TASK_PASS_FRAME);
  Func_08163c2c(1, 0);
  BattleEffect_SpawnObjects(9, 644, 1);
  gCameraSceneParameters[4] = 240;
  BattleEffect_SelectLivingTargets(work->effect);
  REG_WININ = 0x2737;
  REG_WIN0H = 0xca;
  wait(1);
  BattleBackground_LoadFar(1, (s32)&ResourceId_ForestBackdrop, 0);
  Func_08163c2c(1, 1);
  Resource_LoadAndDecompress((s32)&ResourceId_Data134, sheet, 0, 0);
  Resource_LoadAndDecompress((s32)&ResourceId_FireBlobSheet, work->sheet, 1, 1);
  REG_DISPCNT = 0x7741;
  REG_BG2PA = 0x80;
  REG_BLDALPHA = 0x100e;
  REG_BLDCNT = 0x3f44;

  // The fire trail's 64 flames wait at the left, each with its own delay.
  baseX = 0;
  grow = 0;
  savedX = gBgScroll[1].x;
  control = (struct FxControl *)Ram_HeapSlots->transitionWork;
  shift = 0;
  work->transferMode = 1;
  work->transferValue = baseX;
  control->busy = 1;

  i = 0;
  do {
    struct EffectStep *fire = &work->particles[i];

    fire->x = (Random16() & 31) + 16;
    fire->y = ((Random16() & 31) + 48) << 16;
    fire->velocityY = ((Random16() & 31) - 16) << 16;
    fire->variant = Random16() % 48 + 2;
    i++;
  } while (i != 64);

  BattleEffect_LoadBlitter(
      HEAP_BLITTER,
      BLITTER(BLIT_ADDITIVE, COMPOSITE_CLIP_X | COMPOSITE_CLIP_Y));
  draw = (DrawRectangle)Ram_HeapSlots->unk64[1];
  REG_BG2CNT = 0x786;

  for (frame = 0; frame != 120; frame++) {
    wave = 0;
    if (frame == 0)
      Sound_PlayFar(SE_GALLOP);
    if (frame == 26)
      Sound_PlayFar(SE_EMBERS);
    if (frame == 40)
      Sound_PlayFar(SE_FLAMES);
    if (frame == 72)
      Sound_PlayFar(SE_FLAMES);
    if (frame == 104)
      Sound_PlayFar(SE_FLAMES);
    if ((gInput.repeat & 3) != 0 && frame > 16)
      break;

    // The forest scrolls ever faster.
    if (frame >= 24 && frame <= 55)
      shift++;
    if (shift > 24)
      shift = 24;
    if (frame <= 135) {
      gBgScroll[1].x -= shift;
      grow += shift;
    }

    // The row bobs in on a sine wave, shaking the screen every 32 frames.
    if (frame <= 149) {
      struct FxPair pair = gData_08196e7c;
      s32 lift;

      lift = 0;
      if (frame > 103)
        lift = frame * 16 - 1664;
      if (frame >= 8 && frame <= 31)
        baseX = baseX + shift - 8;
      if (frame > 7) {
        s32 amp;
        s32 ang;

        amp = 96;
        if (frame <= 104)
          amp = 32;
        ang = (frame * 1024 - 0x2000) & 0xffff;
        if (ang > 0x8000)
          ang -= 0x8000;
        wave = (Trig_Sin(ang) * amp) >> 16;
        if ((frame & 31) == 8)
          work->shakeFrames = 4;
      }
      place.unk0c = 0;
      place.y = 255 << 16;
      i = 0;
      do {
        place.x = ((baseX + gData_08198ada[i] - lift) << 16) + (224 << 16);
        place.z = ((gData_08198ae3[i] - wave) << 16) + (144 << 15);
        Render_ApplyProjectedPlacementFar(work->objects[i], &place.x, &pair.a,
                                          0);
        i++;
      } while (i != 9);
    }

    // The ember ring, growing over the first 27 frames.
    if (frame <= 26) {
      s32 size = grow + 4;
      s32 count = frame * 8;

      if (size > 10)
        size = 10;
      if (count > 64)
        count = 64;
      for (i = 0; i != count; i++) {
        s32 ang = i << 10;
        s32 x = ((Trig_Sin(ang) * (grow * 2 + 8)) >> 16) + grow + 96;
        s32 y = ((Trig_Cos(ang) * (grow * 12 + 48)) >> 16) + 64;

        draw(canvas, sheet + gData_08197410[size - 1], x - size / 2, y - size,
             size, size * 2);
      }
    }

    if (frame == 24) {
      work->transferMode = 2;
      work->transferValue = 50;
    }
    if (frame == 28)
      REG_BG2CNT = 0x784;

    // The fire trail streams back from the riders.
    if (frame > 17) {
      i = 0;
      do {
        struct EffectStep *fire = &work->particles[i];

        if (fire->variant == 0) {
          s32 size = i % 3 + 1;

          draw(canvas, sheet + gData_08197410[size - 1], fire->x,
               HI(fire->y) - size, size, size * 2);
          fire->x += 2;
          fire->y += fire->velocityY;
          fire->velocityY = fire->velocityY * 48 / 64;
        } else {
          fire->variant--;
        }
        if (fire->x > 128 || fire->variant == 1) {
          fire->x = (Random16() & 31) + baseX + 172;
          fire->y = ((Random16() & 31) - wave + 56) << 16;
          fire->velocityY = ((Random16() & 31) - 16) << 15;
        }
        i++;
      } while (i != 48);
    }

    // The flames climb at the left.
    if (frame > 31) {
      s32 rise = (frame - 32) / 2;
      s32 y;

      if (rise > 40)
        rise = 40;
      i = 0;
      y = 0;
      do {
        draw(canvas, work->sheet + (Random16() & 3) * 0x600, 120 - rise, y, 48,
             32);
        i++;
        y += 18;
      } while (i != 6);
    }

    // The shake jolts BG1 alone.
    if (work->shakeFrames > 0) {
      work->shakeFrames--;
      gBgScroll[1].y = (Random16() & 7) + 28;
    } else {
      gBgScroll[1].y = 32;
    }
    work->transferPending = 1;
    wait(1);
  }

  // Act two: the charge.
  gBgScroll[1].x = savedX;
  control->busy = 0;
  Func_0814cca8();
  REG_WIN0H = 0xf0;

  i = 0;
  do {
    ((u8 *)work->objects[i])[9] |= 12;
    i++;
  } while (i != 9);

  {
    s32 rowX;
    u8 *hit;
    u8 *jitter;

    rowX = 224;
    hit = hitRow;
    i = 0;
    do {
      hit[i] = 0;
      i++;
    } while (i != 14);
    jitter = jitterRow;
    i = 0;
    do {
      jitter[i] = Random16() & 31;
      i++;
    } while (i != 16);
    i = 0;
    do {
      PARTICLES[i].variant = 0;
      i++;
    } while (i != 160 << 1);

    work->transferMode = 2;
    work->transferValue = 75;
    REG_BG2CNT = 0x784;
    REG_BLDALPHA = 0x1010;

    frame = 0;
    do {
      if (frame <= 23) {
        struct FxPair pair2 = gData_08196e84;
        s32 ang;
        s32 rise;

        rowX -= 16;
        if (frame <= 8) {
          ang = frame * 2048 + 0x4000;
          if (ang > 0x8000)
            ang -= 0x8000;
          rise = (Trig_Sin(ang) << 6) >> 16;
        } else {
          ang = frame * 2048 + 0x4000;
          if (ang > 0x8000)
            ang -= 0x8000;
          rise = (Trig_Sin(ang) << 5) >> 16;
        }
        place2.unk0c = 0;
        place2.y = 255 << 16;
        i = 0;
        do {
          place2.x = (rowX + gData_08198ada[i]) << 16;
          place2.z = ((gData_08198ae3[i] - rise) << 16) + (144 << 15);
          Render_ApplyProjectedPlacementFar(work->objects[i], &place2.x,
                                            &pair2.a, 0);
          i++;
        } while (i != 9);
      }

      if (frame == 8) {
        work->shakeFrames = frame;
        Sound_PlayFar(SE_CHARGE);
      }
      if (frame == 11)
        Sound_PlayFar(SE_CHARGE);
      if (frame == 46)
        Sound_PlayFar(SE_FADE);

      // Each target the row passes is struck once, in a burst of sparks.
      for (i = 0; i != effect->count; i++) {
        if (hit[i] == 0) {
          EffectPosition_ProjectBattler(effect->actors[i], &unit);
          if (unit.x > rowX) {
            s32 k;

            hit[i] = 1;
            k = 0;
            do {
              struct EffectStep *burst = &PARTICLES[i * 32 + k];
              s32 up;

              burst->x = unit.x << 15;
              burst->y = (unit.y - 16) << 16;
              burst->velocityX = ((Random16() & 255) - 128) << 10;
              up = (Random16() & 255) - 192;
              burst->velocityY = up << 11;
              burst->x += burst->velocityX * 4;
              burst->y += up << 13;
              burst->variant = (Random16() & 15) + 8;
              k++;
            } while (k != 32);
            Func_08118088(effect->actors[i], 1);
            Sound_PlayFar(SE_HIT);
          }
        }
      }

      i = 0;
      do {
        struct EffectStep *burst = &PARTICLES[i];

        if (burst->variant > 0) {
          s32 size = 3;

          draw(canvas, sheet + gData_08197410[size - 1],
               HI(burst->x) - size / 2, HI(burst->y) - size, size, size * 2);
          burst->x += burst->velocityX;
          burst->y += burst->velocityY;
          burst->variant--;
        }
        i++;
      } while (i != 192);

      if (frame == 48)
        Sound_PlayFar(SE_GALLOP);

      // The wall of flame slides across after the row.
      if (frame > 40) {
        s32 slide = (frame - 40) * 12;
        s32 y;

        work->transferMode = 0;
        work->transferValue = 75;
        i = 0;
        y = -8;
        do {
          draw(canvas, work->sheet + (Random16() & 3) * 0x600,
               jitter[i] - slide + 120, y, 48, 32);
          i++;
          y += 8;
        } while (i != 16);
      }
      if (frame > 64)
        work->transferMode = 2;
      if (frame == 58) {
        for (i = 0; i != effect->count; i++)
          Func_0814cd48(effect->actors[i], 14, 5, -1, 0);
      }

      BattleCamera_Shake(8, 8);
      BattleEffect_CountDownBattlerTints();
      work->transferPending = 1;
      wait(1);
      frame++;
    } while (frame != 96);
  }

  Func_081180e8(PHASE_SUMMON_RESULTS);
  i = 0;
  do {
    ResourceObject_ReleaseFar(work->objects[i]);
    i++;
  } while (i != 9);
  Task_Remove(BattleEffect_ProcessPendingGraphicsTransfer);
  HeapSlot_Release(HEAP_BLITTER);
  BattleEffect_EndCanvasLayer();
}

/// BG2 back to one texel per pixel across.
void Graphics_ResetBg2Pa(void) { REG_BG2PA = Q_8_8(1); }

/// BG2 at half a texel per pixel across: twice as wide.
void Graphics_SetBg2AffineScaleHalf(void) { REG_BG2PA = Q_8_8(0.5); }
