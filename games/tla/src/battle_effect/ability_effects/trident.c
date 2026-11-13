/// @file
/// Trident, the leaping strikes of Planet Diver and Planetary, and Meteor
/// Blow's palette DMA.
// Part of ability_effects.c.

#include "../ability_effects_internal.h"

struct FxPartyList;
struct Model;
struct StripQuad;

void BattleEffect_StartCanvasLayer(s32 bgControl);
void BattleEffect_EndCanvasLayer(void);
void BattleEffect_CountDownBattlerTints(void);
s32 BattleEffect_LoadBlitter(s32 slot,
                             s32 modeAndFlags); // BattleEffect_BuildBlitter
void BattleEffect_TintBattler(s32 actor, s32 palette, s32 pose, s32 timerSlot,
                              s32 timerFrames);
void Func_0815b434(u8 *map, s32 size, s32 scale); // shaded circles
void Func_0816ae40(u8 *src, u8 *dst, s32 width, s32 height);
void BattleEffect_RunDjinniEntrance(struct BattleEffectArgument *effect,
                                    u32 packed, s32 *screen, s32 *world);
void Func_0818caa8(void *cells, s32 x, s32 y, s32 size);
struct PoolEntry *Render_AllocateModelList(s32 count);
void Render_TransformVertices(const void *corners, void *vertices, s32 count);
void Func_08015160(s32 x, s32 y, s32 z);
void Func_0801521c(s32 scale);
void Transform_ApplyPitch(s32 angle);
void Transform_ApplyYaw(s32 angle);
void SceneTransform_ApplyScale3(s32 x, s32 y, s32 z);
void EffectPosition_ProjectBattler(s32 id, struct EffectPosition *out);
void BattleEffect_SpawnObjects(s32 entryCount, s32 kind,
                               u32 variant); // SpawnObjects
void BattleEffect_SelectLivingTargets(
    struct FxPartyList *party); // SelectLivingTargets
void Transform_ApplyRoll(s32 angle);
void BattleEffect_StartActorMotion(s32 id, s32 force, s32 acceleration,
                                   s32 speedLimit, s32 velocityY, s32 percent);

void BattleEffect_FetchRectangleBlitters(s32 flip, void **out);
void BattleMotion_ApplyKnockbackFar(
    s32 actor, s32 motion); // BattleMotion_ApplyVariantMotionFar
void BattlePlayback_StartFar(s32 phase);
void BattleCamera_Shake(s32 rangeX, u32 rangeY);
void BattleEffectKernels_IntegrateVector2(struct EffectStep *step, s32 gravity,
                                          s32 drag);

void Render_BuildQuadStrip(struct StripQuad *quad, s32 width, s32 height,
                           s32 count);
void BattleEffectKernels_IntegrateVector3(struct EffectStep *step, s32 damping,
                                          s32 gravity);
s32 BattleUnit_GetEffectHeightFar(s32 unit);
void BattleEffect_StartTiledCanvas(s32 bgControl);
void BattleEffect_StartTiledCanvasFilled(s32 bgControl);
void Transform_Pop(void);

// The blitter's heap slot, as a byte offset into Ram_HeapSlots.
#define SLOT_BLITTER 104 // unk64[1]

#define BLITTER(mode, flags) ((mode) << 4 | (flags))
#define HI(v) (((s16 *)&(v))[1])

void Func_0815e22c(
    struct EffectPosition *position); // the targets' centre on screen
void BattleEffect_StepPaletteToResource(s32 palette);
s32 BattleEffect_BuildBlitter(s32 slot, s32 widthShift, s32 heightShift,
                              s32 flags, u32 mode);

extern char ResourceId_TornadoSheet;
extern char ResourceId_Data0F8;
extern char ResourceId_FlashBurstSheet;
extern char ResourceId_Data0C1;
extern char ResourceId_Data0FA;
extern char ResourceId_Data0DA;
extern char ResourceId_Data134;
extern char ResourceId_EffectPalette02;
extern char ResourceId_OrangePaletteB;
extern u8 gMapCellBuffer[];
extern u8 Data_02011000[];
extern u8 Data_02011400[];
extern u8 Data_02012964[]; // 128 sparks
extern const struct PlacementSource Data_08196f18;
extern u16 gBattleEffect_MoteCells[];
extern u16 gBattleEffect_GlintCells[]; // the glints' cells
extern u8 gData_08197492[];            // widths
extern u8 gData_08197498[];            // heights
extern const u8 Data_08198cac[];
extern const u8 Data_08198ec4[];
extern const u8 gData_08199268[];
extern const u8 Data_08199dcb[];  // each ring's start
extern const u8 Data_08199dd3[];  // growth
extern const s8 Data_08199ddb[];  // height
extern const s32 Data_08199de4[]; // each trident's yaw and turn
extern const u8 Data_08199e24[];
extern const u8 Data_08199e54[];

// Abilities: Trident (Poseidon).
//
// Poseidon (four objects) rises behind the targets as rings of foam
// spread across the water; seven spinning tridents fly in from frame 18,
// spray bursts up around them, and at 64 the waters strike every target.
void BattleEffect_RunTrident(struct BattleEffectArgument *effect) {
  struct BattleEffectWork *work;
  void *canvas;
  DrawRectangle blit[2]; // Unused: [1] is the rest of its two-word pseudo.
  u8 *sprites;
  s32 centerX;
  s16 *circles;
  s32 center[3];
  s32 placement[4];
  struct PlacementSource source;
  struct ModelTexture texture;
  struct RenderModel *model;
  void *vertices;
  struct EffectStep *p;
  s32 frame;
  s32 i;
  s32 size;
  s32 offset;
  s32 x;
  s32 grow; // Also each trident's first frame: one local, reused.
  s32 palette;
  s32 cells; // The cell buffer's address, held for the whole function.
  s32 orange;
  s32 orangeBase;
  s32 paletteBase;
  DrawRectangle draw;

  work = (struct BattleEffectWork *)Ram_HeapSlots->battleFxWork[0];
  canvas = Ram_HeapSlots->battleFxWork[1];
  sprites = Ram_HeapSlots->unk64[0];
  BattleEffect_StartCanvasLayer(0);
  REG_BLDALPHA = 0x1010;
  BattleEffect_SelectLivingTargets(work->effect);
  BattleEffect_SpawnObjects(4, 664, 1);
  REG_WININ = 0x2737;
  REG_WINOUT = 0x3f21;
  REG_WIN0V = 0x107c;
  REG_WIN1V = 0x1088;
  BattleEffect_LoadBlitter(
      SLOT_BLITTER, BLITTER(BLIT_MAX, COMPOSITE_CLIP_X | COMPOSITE_CLIP_Y));
  draw = (DrawRectangle)Ram_HeapSlots->unk64[1];
  work->transferMode = 2;
  work->transferValue = 50;
  blit[0] = draw;
  /* FAKEMATCH(temporary): the orange palette's id in a local, summed after the
   * 0xF8 load from the address four below it; as the id at its two uses, 56 of
   * 1122 instructions differ in tla-en. */
  orangeBase = (s32)&ResourceId_OrangePaletteB - 4;
  Task_Add(BattleEffect_ProcessPendingGraphicsTransfer, 0xc80);
  circles = (s16 *)(work->sheet + 0x4c56);
  Resource_LoadAndDecompress((s32)&ResourceId_TornadoSheet, work->sheet, 1, 0);
  // offset first holds the cell buffer's address, for the FlashBurst load.
  offset = (s32)gMapCellBuffer;
  Resource_LoadAndDecompress((s32)&ResourceId_Data0F8, work->sheet + 0xc56, 0,
                             0);
  orange = orangeBase + 4;
  size = 2;
  /* FAKEMATCH(dead): a test on the blitter, which the routine calls through and
   * is never null; as its three statements alone, 77 of 1122 instructions
   * differ in tla-en. */
  if (draw) {
    /* FAKEMATCH(temporary): the effect palette's id in a local, summed from the
     * address four below it; as the id at its three uses, 6 of 1122
     * instructions differ in tla-en. */
    paletteBase = (s32)&ResourceId_EffectPalette02 - 4;
    cells = (s32)gMapCellBuffer;
    palette = paletteBase + 4;
  }
  Resource_LoadAndDecompress((s32)&ResourceId_FlashBurstSheet,
                             (u8 *)offset + 0x5000, 1, 0);
  offset = 0x2400;
  // Twelve shaded droplets, 2 to 24 wide.
  for (i = 0; i != 12; i++) {
    Func_0815b434((u8 *)(offset + cells), size, 0x10000);
    circles[i] = offset;
    offset += (i * 2 + 3) * size / 2;
    size += 2;
  }
  Resource_LoadAndDecompress((s32)&ResourceId_Data0C1, (u8 *)cells, 0, 0);
  Resource_LoadAndDecompress((s32)&ResourceId_Data0FA, Data_02011000, 0, 0);
  Resource_LoadAndDecompress((s32)&ResourceId_Data0DA, Data_02011400, 0, 0);
  Resource_LoadAndDecompress((s32)&ResourceId_Data134, sprites, 0, 0);
  Iwram_CopyWordsEntry((void *)BG_PLTT,
                       Resource_GetTableEntry((s32)&ResourceId_EffectPalette02),
                       128);
  for (i = 0, p = work->particles; i != 32; i++, p++) {
    s32 radius = i * 2;
    s32 angle = Random16() & 0xffff;

    p->x = Trig_Sin(angle) * radius;
    p->y = -(Trig_Cos(angle) * radius);
    p->variant = i / 2 + 25;
  }
  Func_0815e22c(center);
  centerX = center[0] / 2;

  for (frame = 0; frame != 104; frame++) {
    if (frame == 0) {
      Iwram_CopyWordsEntry((void *)BG_PLTT, Resource_GetTableEntry(palette),
                           128);
    }
    if (frame > 76) {
      BattleEffect_StepPaletteToResource(palette);
    } else if (frame > 64) {
      BattleEffect_StepPaletteToResource(orange);
    } else if (frame > 23) {
      BattleEffect_StepPaletteToResource(palette);
    } else if (frame > 8) {
      BattleEffect_StepPaletteToResource(orange);
    }
    if (frame == 8) {
      work->shakeFrames = frame;
      for (i = 0; i != effect->count; i++) {
        BattleEffect_TintBattler(effect->actors[i], 7, 5, i, 16);
      }
    }
    if (frame == 48) {
      work->shakeFrames = 8;
    }
    if (frame == 64) {
      work->shakeFrames = 16;
      BattlePlayback_StartFar(144);
      for (i = 0; i != effect->count; i++) {
        BattleEffect_StartActorMotion(effect->actors[i], 1, 0x20000, 0x80000,
                                      0x80000, 120);
      }
    }
    if (frame == 0) {
      Sound_PlayFar(212);
    }
    if (frame == 4) {
      Sound_PlayFar(144);
    }
    if (frame == 16) {
      Sound_PlayFar(164);
    }
    if (frame == 64) {
      Sound_PlayFar(145);
    }

    // Poseidon rises.
    if (frame <= 63) {
      s32 left = centerX - 8;
      s32 visible = 128;
      s32 rise;

      if (frame <= 9) {
        rise = frame * 16 - 144;
      } else {
        rise = 0;
      }
      if (rise + 128 > 108) {
        visible = visible - rise - 20;
      }
      if (visible > 0) {
        source = Data_08196f18;
        placement[3] = 0;
        placement[1] = 255 << 16;
        for (i = 0; i != 4; i++) {
          placement[0] = (left << 17) + 0x100000;
          placement[2] = (rise + 40 + i * 32) << 16;
          Render_ApplyProjectedPlacementFar(work->objects[i], placement,
                                            (s32 *)&source, 0);
        }
      }
    }

    // Spray: a pool of foam, then droplets thrown up.
    if ((u32)(frame - 12) <= 52) {
      if (frame == 12) {
        for (i = 0; i != 32; i++) {
          work->particles[i].variant = -2 * i;
        }
      }
      for (i = 0, p = work->particles, grow = frame * 4 - 156; i != 14;
           i++, p++) {
        p->variant++;
        if (i == 0) {
          if (frame > 39) {
            u8 *pool = work->sheet + 0x4c86;

            Func_0815b434(pool, grow, 0x10000);
            Func_0818caa8(pool, centerX, 100, grow);
          }
          continue;
        }
        if (p->variant == 0) {
          s32 angle = Random16() & 0x7fff;
          s32 heading = angle + 0x4000;
          s32 speed = (Random16() & 511) + 512;
          s32 sine;

          sine = Trig_Sin(heading) * speed;
          x = centerX << 16;
          p->x = (sine >> 4) + x;
          p->y = ((Trig_Cos(heading) * speed) >> 3) + 0x680000;
          angle += 0xc000;
          p->velocityX = (Trig_Sin(angle) * speed) >> 9;
          p->velocityY = (Trig_Cos(angle) * speed) >> 8;
          p->velocityZ = (Random16() & 7) + 4;
        }
        if ((u32)p->variant <= 31) {
          s32 drop = ((Trig_Sin(p->variant << 10) * p->velocityZ) >> 16) * 2;
          s32 index = drop - 2;

          if ((u32)index <= 61) {
            s32 dropX = HI(p->x);
            s32 dropY = HI(p->y);
            s32 tier = index >> 1;

            BattleEffectKernels_IntegrateVector2(p, 64, 0);
            Func_0818caa8((u8 *)(circles[tier] + cells), dropX, dropY, drop);
          }
        }
      }
    }

    if (frame > 7) {
      s32 scale;

      vertices = Heap_AllocEwram(0x400);
      model = Render_AllocateModelList(1);
      texture.logWidth = 7;
      texture.logHeight = 7;
      texture.pixels = (void *)cells;
      model->kind = 7;
      model->texture = &texture;
      model->unk04 = 0;
      model->mesh = Data_08198ec4;
      model->vertices = vertices;
      model->scrollU = 0;
      model->scrollV = 0;

      // Eight rings of foam.
      for (i = 0; i != 8; i++) {
        s32 start = Data_08199dcb[i];
        s32 shade;

        if (frame <= start) {
          continue;
        }
        scale = (frame - start) * Data_08199dd3[i] * 1500 + 0x4180;
        shade = (start - frame) * 8 + 56;
        if (shade > 0) {
          shade = 0;
        }
        if (shade <= -64) {
          continue;
        }
        model->shade = shade;
        Transform_Reset();
        Func_08015160((centerX - 64) << 16, Data_08199ddb[i] << 16, 0);
        SceneTransform_ApplyScale3(scale, scale * 2, scale);
        model->scrollU = ((frame + i * 5) & 7) << 4;
        Transform_ApplyPitch(3000);
        Render_TransformVertices(Data_08198cac, vertices, 32);
        Render_DrawModelList(model);
      }

      // The tridents.
      if (frame <= 71) {
        const s32 *spin;
        struct EffectStep *t;
        struct EffectStep *spinner;

        texture.logWidth = 5;
        texture.logHeight = 7;
        model->texture = &texture;
        model->kind = 7;
        model->mesh = gData_08199268;
        model->vertices = vertices;
        model->scrollU = 0;
        model->scrollV = 0;
        if (frame == 8) {
          for (i = 0, t = &work->particles[32], spin = Data_08199de4; i != 8;
               i++, t++, spin += 2) {
            t->z = spin[0];
            t->velocityZ = spin[1];
            t->variant = (i % 6) * 4;
          }
        }
        for (i = 0, grow = 18, spinner = &work->particles[32]; i != 7;
             grow += 4, i++, spinner++) {

          scale = i * 4;
          if (frame < grow || frame >= scale + 42) {
            continue;
          }
          x = (spinner->variant / 8) % 3;
          /* FAKEMATCH(dead): a test on x with both arms alike; as one
           * model->shade = -16, 192 of 1122 instructions differ in tla-en. */
          if (x) {
            model->shade = -16;
          } else {
            model->shade = -16;
          }
          texture.pixels = work->sheet + (x << 12) + 0xc56;
          Transform_Reset();
          Func_08015160((centerX - 64) << 16, 0x200000, 0);
          Transform_ApplyRoll(spinner->z);
          if (i & 1) {
            Transform_ApplyYaw(0x8000);
          }
          spinner->z += spinner->velocityZ;
          SceneTransform_ApplyScale3(0x20000, 0x12710, 0x10000);
          Render_TransformVertices(Data_08199e24 + x * 16, vertices, 4);
          Render_DrawModelList(model);
        }
      }
      Heap_Release(model);
      Heap_Release(vertices);
    }

    // The strike: flashes and a fountain of sparks.
    if (frame > 63) {
      struct EffectStep *drawn;
      const u8 *cellTable;

      if (frame == 64) {
        for (i = 0, p = work->particles, x = centerX << 16; i != 32; i++, p++) {
          s32 angle = Random16();
          s32 speed = (Random16() & 511) + 512;
          s32 velocityY;

          p->velocityX = (Trig_Sin(angle) * speed) >> 8;
          velocityY = (Trig_Cos(angle) * speed) >> 7;
          p->velocityY = velocityY;
          if (i > 15) {
            p->x = (p->velocityX << 2) + x;
            p->y = (velocityY << 2) + 0x640000;
            p->variant = (16 - i) / 4;
          } else {
            p->x = x;
            p->y = (Random16() % 104) << 16;
            p->variant = -i / 4;
          }
        }
        for (i = 0, p = (struct EffectStep *)Data_02012964; i != 128;
             i++, p++) {
          s32 angle = (Random16() & 0x7fff) + 0x4000;
          s32 speed = (Random16() & 511) + 512;

          p->x = x;
          p->y = 0x640000;
          p->velocityX = (Trig_Sin(angle) * speed) >> 6;
          p->velocityY = (Trig_Cos(angle) * speed) >> 5;
          p->variant = (Random16() & 15) + 16;
        }
      }
      for (i = 0, drawn = work->particles; i != 32; i++, drawn++) {
        if ((u32)drawn->variant <= 23) {
          blit[0](canvas,
                  Ram_MapCellBuffer + 0x5000 + ((drawn->variant / 4) << 11),
                  HI(drawn->x) - 16, HI(drawn->y) - 32, 32, 64);
          BattleEffectKernels_IntegrateVector2(drawn, 62, -0x4000);
        }
        drawn->variant++;
      }
      // The cell table's address is a local set before the walker, and each
      // spark's cell is read at its byte offset: the listing's pool order
      // and its ldrh by register offset.
      for (i = 0, cellTable = (const u8 *)gBattleEffect_MoteCells,
          drawn = (struct EffectStep *)Data_02012964;
           i != 128; i++, drawn++) {
        s32 spark;
        s32 at;

        if (drawn->variant < 0) {
          continue;
        }
        spark = (drawn->variant >> 3) + 1;
        at = (spark - 1) * 2;
        blit[0](canvas, sprites + *(const u16 *)(cellTable + at),
                HI(drawn->x) - spark / 2, HI(drawn->y) - spark, spark,
                spark * 2);
        drawn->variant--;
        BattleEffectKernels_IntegrateVector2(drawn, 56, 0x2000);
      }
    }

    // Glints over the water.
    if ((u32)(frame - 24) <= 39) {
      for (i = 0; i != 4; i++) {
        s32 k = i & 3;
        s32 angle = Random16();
        s32 x =
            centerX + ((Trig_Sin(angle) << 3) >> 16) - (gData_08197492[k] >> 1);
        s32 y = (i << 5) + ((Trig_Cos(angle) << 4) >> 16) -
                (gData_08197498[k] >> 1);

        {
          s32 c = Data_08199e54[Random16() & 3] | 3;

          /* FAKEMATCH(dead): an empty once-only block before the blitter call;
           * as no block, 277 of 1122 instructions differ in tla-en. */
          do {
          } while (0);
          BattleEffect_BuildBlitter(188, 7, 7, c, 1);
        }
        ((DrawRectangle)Ram_HeapSlots->unkb8[1])(
            canvas, work->sheet + gBattleEffect_GlintCells[k], x, y,
            gData_08197492[k], gData_08197498[k]);
        HeapSlot_Release(188);
      }
    }
    BattleCamera_Shake(16, 16);
    BattleEffect_CountDownBattlerTints();
    work->transferPending = 1;
    wait(1);
  }
  Task_Remove(BattleEffect_ProcessPendingGraphicsTransfer);
  HeapSlot_Release(104);
  for (i = 0; i != 4; i++) {
    Sprite_DestroyFar(work->objects[i]);
  }
  BattleEffect_EndCanvasLayer();
}

void Func_0818d5a8(struct BattleEffectArgument *effect, s32 mode);

/// Abilities: Planet Diver (kind 60).
DEFINE_EFFECT_KIND(BattleEffect_RunPlanetDiver, Func_0818d5a8, 0)

/// Abilities: Planetary (kind 128).
DEFINE_EFFECT_KIND(BattleEffect_RunPlanetary, Func_0818d5a8, 1)

#define LEAP_FRAMES 88
#define SPARKS ((struct EffectStep *)(Ram_MapCellBuffer + 0x6000))

extern char ResourceId_FlameballSheet;
extern char ResourceId_ClawSlashSheet;

extern const u8 gData_08199340[];
extern const u8 gData_081990d0[];

extern const u8 Data_081991e0[];
extern const u8 gData_08199090[];

extern const u8 Data_08199e58[];
extern const u8 Data_08199e5c[];
extern const s8 Data_08199e60[];
extern const u8 Data_08199e64[];
extern const u8 Data_08199e68[];
extern const s8 Data_08199e6c[];

void BattleEffect_ConfigureEffectDisplay(void);

// The leaping strike family (BattleEffect_RunPlanetDiver is mode 0,
// BattleEffect_RunPlanetary mode 1; effect kinds 60 and 128). The caster
// flashes and sixteen flames ring it (frames 2-15), leaps up at frame 4 and
// comes down on the target at frame 16, sliding to it over the frames after; on
// landing 256 sparks burst from it. In mode 0 the caster then shows a fireball
// and one target is struck; in mode 1 a claw slashes in 3D, three rings and two
// pillars spread, every target is struck and at frame 50 all are thrown back.
void Func_0818d5a8(struct BattleEffectArgument *effect, s32 mode) {
  struct EffectPosition pos;
  struct ModelTexture texture;
  struct BattleEffectWork *work;
  void *canvas;
  DrawRectangle draw[2];
  u8 *graphics;
  struct MotionObject *caster;
  struct MotionObject *target;
  s32 stepX;
  s32 stepY;
  s32 homeX;
  s32 homeZ;
  s32 landed;
  struct EffectStep *spark;
  s32 frame;
  s32 i;

  work = (struct BattleEffectWork *)Ram_HeapSlots->battleFxWork[0];
  canvas = Ram_HeapSlots->battleFxWork[1];
  graphics = Ram_HeapSlots->unk64[0];
  BattleEffect_ConfigureEffectDisplay();
  BattleEffect_StartCanvasLayer(0);
  REG_BLDALPHA = 0x1010;
  Resource_LoadAndDecompress((s32)&ResourceId_Data134, graphics, 0, 0);
  Resource_LoadAndDecompress((s32)&ResourceId_FlashBurstSheet, work->sheet, 1,
                             0);
  if (mode == 0) {
    Resource_LoadAndDecompress((s32)&ResourceId_FlameballSheet,
                               work->sheet + 0x3000, 1, 0);
  } else {
    Resource_LoadAndDecompress((s32)&ResourceId_ClawSlashSheet,
                               work->sheet + 0x3000, 1, 0);
    Resource_LoadAndDecompress((s32)&ResourceId_Data0DA, work->sheet + 0x4000,
                               0, 0);
    Resource_LoadAndDecompress((s32)&ResourceId_Data0C1, work->sheet + 0x5000,
                               0, 0);
  }
  Iwram_CopyWordsEntry((void *)BG_PLTT,
                       Resource_GetTableEntry((s32)&ResourceId_OrangePaletteB),
                       128);
  // The blitter faces the caster's side.
  if (effect->side == 0)
    BattleEffect_LoadBlitter(104, 1 << 4 | 3);
  else
    BattleEffect_LoadBlitter(104, 1 << 4 | 7);
  draw[0] = (DrawRectangle)Ram_HeapSlots->unk64[1];
  work->transferMode = 2;
  if (mode == 0)
    work->transferValue = 75;
  else
    work->transferValue = 50;
  Task_Add(BattleEffect_ProcessPendingGraphicsTransfer, 0xc80);
  for (i = 0; i != 256; i++)
    SPARKS[i].variant = 0;

  caster = GetBattleObjectSlotFar(effect->actor)->object;
  target = GetBattleObjectSlotFar(effect->actors[0])->object;
  stepX = 0;
  stepY = 0;
  homeX = caster->x;
  homeZ = caster->z;
  landed = 0;

  for (frame = 0; frame != LEAP_FRAMES; frame++) {
    if (frame == 0) {
      caster->x = homeX;
      caster->y = 0;
      caster->z = homeZ;
      landed = 0;
    }
    if (frame > 17 || frame == 0) {
      EffectPosition_ProjectBattler(effect->actor, &pos);
      pos.x /= 2;
    }

    // Frames 2-15: a flash, then sixteen flames ringing the caster.
    if ((u32)(frame - 2) <= 1)
      draw[0](canvas, work->sheet, pos.x - 16, pos.y - 64, 32, 64);
    if ((u32)(frame - 4) <= 11) {
      for (i = 0; i != 16; i++) {
        s32 x = pos.x + ((frame * Trig_Sin(i << 12)) >> 16);
        s32 y = pos.y + ((frame * Trig_Cos(i << 12)) >> 16) - frame;

        draw[0](canvas, work->sheet + ((frame - 4) / 2 << 11), x - 16, y - 64,
                32, 64);
      }
    }

    // Frame 4: the leap, away from the caster's own side.
    if (frame == 4) {
      caster->autoFaceMotion = 0;
      caster->verticalMotionStrength = 0x9999;
      caster->velocityY = 15 << 16;
      if (caster->x < 0)
        caster->velocityX = -8 << 16;
      else
        caster->velocityX = 8 << 16;
      if (mode == 1)
        caster->velocityX = -caster->velocityX;
      Actor_SetMode(caster, 2);
      work->shakeFrames = 4;
      Sound_PlayFar(136);
    }

    // Frame 16: down at the target's depth, then sliding to it over 16
    // frames.
    if (frame == 16) {
      s32 slide = 16;

      caster->verticalMotionStrength = 0;
      if (mode == 0)
        caster->z = target->z;
      else
        caster->z = 0;
      caster->z = target->z;
      caster->velocityX = 0;
      caster->velocityY = 0;
      caster->velocityZ = 0;
      stepX = (target->x * 60 / 100 - caster->x) / slide;
      stepY = (target->y - caster->y) / slide;
      Actor_ResetMotion(caster);
    }

    if (frame > 17) {
      // Sliding to the target while in the air.
      if (caster->y > 0) {
        caster->x += stepX;
        caster->y += stepY;
      }
      if (caster->y <= 0) {
        if (!landed) {
          // The landing: sparks burst from the caster.
          landed = 1;
          caster->y = 0;
          spark = SPARKS;
          for (i = 0; i != 256; i++) {
            s32 speed = 0x3ff;
            s32 angle;

            // Masked in place, the mask assigned first.
            speed &= Random16();
            angle = Random16() & 0xffff;

            speed += 32;
            spark->x = pos.x << 16;
            spark->y = (pos.y - 24) << 16;
            spark->velocityX = (Trig_Sin(angle) * speed) >> 6;
            spark->velocityY = -((Trig_Cos(angle) * speed) << 1) >> 6;
            spark->variant = (Random16() & 7) + 32;
            spark++;
          }
          work->shakeFrames = 4;
          if (mode == 0) {
            BattlePlayback_StartFar(145);
            BattleMotion_ApplyKnockbackFar(effect->actors[0], 4);
            BattleEffect_TintBattler(effect->actors[0], 7, 5, 0, 8);
          } else {
            Sound_PlayFar(145);
            for (i = 0; i != effect->count; i++) {
              BattleMotion_ApplyKnockbackFar(effect->actors[i], 3);
              BattleEffect_TintBattler(effect->actors[i], 7, 5, i, 16);
            }
          }
        }
      }

      if (mode == 0) {
        // The fireball over the caster while it is in the air.
        if (caster->y > 0)
          draw[0](canvas, work->sheet + 0x3000, pos.x - 20, pos.y - 52, 40, 64);
      } else {
        struct RenderModel *model;
        void *projected;
        s32 kind;
        s32 shade;

        projected = Heap_AllocEwram(1024);
        model = Render_AllocateModelList(1);
        shade = 0;
        if (frame == 40)
          work->shakeFrames = 14;

        // The claw, slashing until frame 45.
        // A set-once local: it gets no register and reloads at each use.
        kind = 6;
        texture.logWidth = 6;
        texture.logHeight = 6;
        texture.pixels = work->sheet + 0x3000;
        model->kind = kind;
        model->mesh = gData_08199340;
        model->texture = &texture;
        model->vertices = projected;
        model->shade = shade;
        if (frame <= 45) {
          Transform_Reset();
          Func_08015160((pos.x - 60) << 16, (pos.y - 80) << 16, 0);
          SceneTransform_ApplyScale3(0x8000, 0x10000, 0x8000);
          if (effect->side == 0) {
            Transform_ApplyRoll(-0x3388);
          } else {
            Transform_ApplyYaw(0x8000);
            Transform_ApplyRoll(-0x3388);
          }
          Func_0801521c(0x286a0);
          Render_TransformVertices(Data_081991e0, projected, 4);
          Render_DrawModelList(model);
        }

        // Three rings, each from its own frame, spinning as they spread.
        texture.logWidth = 6;
        texture.logHeight = 6;
        texture.pixels = work->sheet + 0x4000;
        model->kind = 7;
        model->mesh = gData_081990d0;
        for (i = 0; i != 3; i++) {
          s32 start = Data_08199e58[i] + 40;

          if (frame > start) {
            shade = (start - frame) * 8 + 56;

            if (shade > 0)
              shade = 0;
            if (shade > -64) {
              s32 scale;

              start = frame - start;
              scale = start * Data_08199e5c[i] * 2000 + 0x8000;

              model->shade = shade;
              Transform_Reset();
              model->scrollV = (frame * 8 + i * 16) & 63;
              Func_08015160((pos.x - 60) << 16, Data_08199e60[i] << 16, 0);
              SceneTransform_ApplyScale3(scale >> 1, scale << 2, scale / 4);
              Transform_ApplyPitch(1000);
              if (i & 1)
                Transform_ApplyYaw((i * 4 + frame) << 11);
              else
                Transform_ApplyYaw((i * 4 - frame) << 11);
              Render_TransformVertices(gData_08199090, projected, 32);
              Render_DrawModelList(model);
            }
          }
        }

        // Two pillars where the caster landed, from their own frames.
        model->scrollV = 0;
        model->mesh = Data_08198ec4;
        texture.pixels = work->sheet + 0x5000;
        texture.logWidth = 7;
        texture.logHeight = 7;
        for (i = 0; i != 2; i++) {
          s32 start = Data_08199e64[i] + 34;
          struct EffectStep *base = &work->particles[i];

          if (frame == start) {
            base->x = pos.x;
            base->y = pos.y;
          }
          if (frame > start) {
            shade = (start - frame) * 8 + 40;

            if (shade > 0)
              shade = 0;
            if (shade > -64) {
              s32 scale;
              s32 scroll;

              start = frame - start;
              scale = start * Data_08199e68[i] * 1500 + 0x4180;

              model->shade = shade;
              // Masked in place, the mask assigned first.
              scroll = 7;
              scroll &= frame + i * 5;
              model->scrollU = scroll << 4;
              Transform_Reset();
              Func_08015160((base->x - 60) << 16, Data_08199e6c[i] << 16, 0);
              SceneTransform_ApplyScale3(scale, scale * 2, scale);
              Transform_ApplyPitch(2000);
              Render_TransformVertices(Data_08198cac, projected, 32);
              Render_DrawModelList(model);
            }
          }
        }
        Heap_Release(model);
        Heap_Release(projected);
      }
    }

    if (mode == 1 && frame == 50) {
      BattlePlayback_StartFar(134);
      for (i = 0; i != effect->count; i++)
        BattleEffect_StartActorMotion(effect->actors[i], 1, 0x40000, 0x100000,
                                      0x10000, 250);
    }

    // The sparks fall, bounce off the ground at 112 and shrink.
    spark = SPARKS;
    for (i = 0; i != 256; i++) {
      if (spark->variant > 0) {
        spark->variant--;
        BattleEffectKernels_IntegrateVector2(spark, 56, 0x2000);
        if (spark->y > 112 << 16) {
          spark->velocityY = -spark->velocityY / 2;
        } else if ((u32)spark->x < 127 << 16 && spark->y >= 0) {
          s32 size = spark->variant / 8 + 1;

          draw[0](canvas, graphics + gBattleEffect_MoteCells[size - 1],
                  (spark->x >> 16) - size / 2, (spark->y >> 16) - size, size,
                  size * 2);
        }
      }
      spark++;
    }

    BattleCamera_Shake(12, 12);
    BattleEffect_CountDownBattlerTints();
    work->transferPending = 1;
    wait(1);
  }

  Task_Remove(BattleEffect_ProcessPendingGraphicsTransfer);
  HeapSlot_Release(104);
  BattleEffect_EndCanvasLayer();
}

/// Meteor Blow's horizontal-blank callback, the same routine as
/// actor_move_random.c's BattleEffect_ArmPaletteHBlankDma: stops DMA 0,
/// then arms it to copy one BG palette word per scanline from the line table in
/// the effect work, so the backdrop's colour changes down the screen.
// The Broken Seal's BattleEffect_ArmPaletteHBlankDma.
void BattleEffect_ArmMeteorBlowPaletteDma(void) {
  u8 *work = Ram_HeapSlots->battleFxWork[0];
  volatile u16 *channel = REG_DMA0;
  u32 palette;

  channel[5] &= 0xc5ff; // clear the start timing and repeat
  channel[5] &= 0x7fff; // stop the channel
  palette = 0xa0;
  (void)channel[5];
  Dma_Set(work + 0x7640, (void *)(palette << 19), DMA_HBLANK_HALFWORD_PER_LINE,
          (volatile u32 *)channel);
}

// The triangles of a grid mesh.

/// Fills a grid of columns by rows vertices with two triangles a cell, each
/// cell cellWidth by cellHeight texels of the texture, and ends the list with
/// an all-zero triangle.
void Func_0818de3c(struct MeshTriangle *triangles, s32 columns, s32 rows,
                   s32 cellWidth, s32 cellHeight) {
  s32 row;
  s32 column;
  struct MeshTriangle *second;

  for (row = 0; row != rows; row++) {
    for (column = 0; column != columns; column++) {
      if (row < rows - 1) {
        // The address as an integer sum, which adds the offset to the base
        // as the native does.
        struct MeshTriangle *triangle =
            (struct MeshTriangle *)((row * columns + column) * 24 +
                                    (u32)triangles);
        s32 u0 = column * cellWidth;
        s32 u1 = u0 + cellWidth - 1;
        s32 v0 = row * cellHeight;
        s32 v1 = v0 + cellHeight - 1;

        triangle[0].texCoord[0][0] = u1;
        triangle[0].texCoord[0][1] = v0;
        triangle[0].texCoord[1][0] = u0;
        triangle[0].texCoord[1][1] = v1;
        triangle[0].texCoord[2][0] = u1;
        triangle[0].texCoord[2][1] = v1;
        second = triangle + 1;
        second->texCoord[0][0] = u0;
        second->texCoord[0][1] = v0;
        second->texCoord[1][0] = u0;
        second->texCoord[1][1] = v1;
        second->texCoord[2][0] = u1;
        second->texCoord[2][1] = v0;
        triangle->vertex[0] = (column + 1) % columns + row * columns;
        triangle->vertex[1] = column % columns + columns + row * columns;
        triangle->vertex[2] = (column + 1) % columns + columns + row * columns;
        second->vertex[0] = column % columns + row * columns;
        second->vertex[1] = column % columns + columns + row * columns;
        second->vertex[2] = (column + 1) % columns + row * columns;
      }
    }
  }
  triangles[(rows - 1) * columns * 2].vertex[0] = 0;
  triangles[(rows - 1) * columns * 2].vertex[1] = 0;
  triangles[(rows - 1) * columns * 2].vertex[2] = 0;
}
#undef SLOT_BLITTER
#undef BLITTER
#undef HI
#undef LEAP_FRAMES
#undef SPARKS
