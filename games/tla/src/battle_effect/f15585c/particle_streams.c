/// @file
/// The particle streams of summon 11, the formation objects summons place,
/// palette brightening, and an unused second copy of the Nereid summon.
// Part of f15585c.c.

#include "../f15585c_internal.h"

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
extern char ResourceId_Data134; // ☀️ ParticleSpritesA
extern u8 gData_0819896a[];     // the pillars' feet, x and y
// ☀️ gBattleEffect_MoteCells: the Data134 sheet's spark cells, by size - 1
// (byte offsets).
extern u16 gData_08197410[];
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

#define SHARD_E 0x59d8

extern char ResourceId_FlareSheet;
extern char ResourceId_FlareImage;
extern u16 gData_081989f2[];
extern u8 gData_081989e2[];
extern u8 gData_081989ea[];

s32 BattleEffect_LoadBlitter(
    s32 slot,
    s32 modeAndFlags); // BattleEffect_BuildBlitter(slot, 7, 7, flags, mode)
void BattleEffectKernels_IntegrateVector2(struct EffectStep *step, s32 damping,
                                          s32 gravity);

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
/* VIEW(actor_move_random): static CopyWords_ as CopyWords___actor_move_random;
 * f15b3b0 has static __inline__ s32 CopyWords_(void *destination, const void
 * *source, s32 size) { return Iwram_CopyWords(destination, source, size); };
 * tried with the definition the rest of the object has, BattleEffect_RunMeteor
 * (gone) moves: 4 of 4 instructions in tla-ja. */
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
// kept as the argument, BattleEffect_WipeCanvas for the canvas wipe, the fills
// through IwramFillWords where ☀️ uses DMA, mode 1's two objects resource 638 +
// i * 0x2001 (☀️ 483), mode 0's spawn 650 (☀️ 381), the palette lines at 0x7640
// in the work (☀️ 0x1f80), the blitter modes counted down from 3, and the
// callback orders 0xc80, 0xcfe and 0xcff (☀️ 0x480, 0x4fe, 0x4ff).

static __inline__ s32
CopyWords___actor_move_random(void *destination, const void *source, s32 size) {
  return Iwram_CopyWords(destination, source, size);
}

// The map cell buffer: first a 256 x 128 picture the streams draw into (BG1's
// tiles), then, in the second act, the flashes and the embers.
#define FLASHES ((struct EffectStep *)Ram_MapCellBuffer)
#define EMBERS (&FLASHES[128])

// The scanline palette the HBlank DMA feeds into BG colour 0, in the work.
#define HEAT_LINES(work) ((u16 *)((u8 *)(work) + 0x7640))

// BattleEffect_LoadBlitter's argument: The Lost Age's blitter mode (☀️'s counted
// down from 3) in the high nibble, the COMPOSITE_ flags in the low one.
#define BLITTER(mode, flags) ((mode) << 4 | (flags))

#define STREAM_FRAMES 192
#define BLAST_FRAMES 54
#define BLAST_FLASH_FRAME 16 // the flashes rise from here
#define BLAST_HIT_FRAME 48

#define SE_RISE 145
#define SE_STREAMS 141
#define SE_BLAST 156

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

void BattleEffect_WipeCanvas(s32 mode, s32 value);
void Animation_ApplyChildArgumentFar(void *object, s32 animation);

void BattleEffect_AdvanceBg2OnInterval(void);
void BattleEffect_FlushPendingGraphicsTransfer(void);
s32 Func_08196404(s32 slot, s32 widthShift, s32 heightShift, s32 flags,
                  u32 mode); // BattleEffect_BuildBlitter

void SceneTransform_ApplyPosition(const s32 *pos);
void Func_080150e4(s32 roll); // SceneTransform_ApplyRoll
void SceneTransform_ApplyYaw(s32 yaw);
void Func_0814cca8(void); // BattleEffect_SetupBlendedDisplay
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
  BattleEffect_WipeCanvas(0, 0);
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
    CopyWords___actor_move_random(
        (void *)BG_PLTT,
        Resource_GetTableEntry((s32)&ResourceId_LightningBoltSheet), 128);

  // BG1 shows the ground: the map cell buffer filled with colour 1.
  FillWords_(Ram_MapCellBuffer, 0x8000, 0x01010101);
  // The copy in a once-only block, so its entry loads before the size; as
  // the plain call, 2 of 1591 instructions differ in tla-en.
  do {
    CopyWords___actor_move_random((void *)0x06008000, Ram_MapCellBuffer,
                                  0x7800);
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
  CopyWords___actor_move_random((void *)0x06004000, canvas, 0x4000);
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
