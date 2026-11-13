/// @file
/// The body of Thunder Mine and Earth Force, then Megaera and Void Beam.
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
void Func_0814cd48(s32 actor, s32 palette, s32 pose, s32 timerSlot,
                   s32 timerFrames);              // BattleEffect_TintBattler
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
void SceneTransform_ApplyYaw(s32 angle);
void Func_080151e4(s32 x, s32 y, s32 z); // SceneTransform_ApplyScale3
void EffectPosition_ProjectBattler(s32 id, struct EffectPosition *out);
void BattleEffect_SpawnObjects(s32 entryCount, s32 kind,
                               u32 variant); // SpawnObjects
void BattleEffect_SelectLivingTargets(
    struct FxPartyList *party); // SelectLivingTargets
void Func_080150e4(s32 angle);  // SceneTransform_ApplyRoll
void BattleEffect_StartActorMotion(s32 id, s32 force, s32 acceleration,
                                   s32 speedLimit, s32 velocityY, s32 percent);

void BattleEffect_FetchRectangleBlitters(s32 flip, void **out);
void BattleMotion_ApplyKnockbackFar(
    s32 actor, s32 motion);    // BattleMotion_ApplyVariantMotionFar
void Func_081180e8(s32 phase); // BattlePlayback_StartFar
void BattleCamera_Shake(s32 rangeX, u32 rangeY);
void BattleEffectKernels_IntegrateVector2(struct EffectStep *step, s32 gravity,
                                          s32 drag);

void *Resource_GetTableEntry(s32 resource);

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

s32 Func_08196404(s32 slot, s32 widthShift, s32 heightShift, s32 flags,
                  u32 mode);

extern char ResourceId_TornadoSheet;
extern char ResourceId_Data134;
extern char ResourceId_LightningBoltSheet;
extern char ResourceId_VioletPaletteC;
/* VIEW(f16fca8): variable gData_02014000 as extern struct EffectStep
 * gData_02014000[]; f16e65c has extern u8 gData_02014000[]; tried as the rest
 * of the object declares it, it does not compile in tla-ja:
 * games/tla/src/battle_effect/ability_effects.c:19186: request for member
 * `variant' in something not a structure or union. */
extern struct EffectStep
    gData_02014000__f16fca8[] __asm__("gData_02014000"); // the sparks
extern u16 gData_08197410[]; // gBattleEffect_MoteCells
// Heights not const: a const table's second height read issues two places
// early (6).
extern u16 gData_08197486[];      // the flashes' cells: offsets,
extern u8 gData_08197492[];       // widths
extern u8 gData_08197498[];       // and heights
extern const u8 Data_08198bee[4]; // their flips
extern const u8 gData_08199268[]; // a bolt's mesh
extern const u8 gData_081991b0[]; // its corners (Thunder Mine)
extern const u8 Data_081991c0[];  // and (Earth Force)

// The body of Thunder Mine (mode 0) and Earth Force (mode 1): a ball of
// light (Thunder Mine) or a falling rock's glow (Earth Force) crosses to the
// target, trailed by 40 spinning bolts, and grows; when it arrives the
// target is hit, and in Thunder Mine 128 sparks scatter and bounce.
void Func_0816fca8(struct BattleEffectArgument *effect, s32 mode) {
  struct BattleEffectWork *work;
  void *canvas;
  s32 frame;
  // Unused: draw[1] gives the stack frame its 4 bytes.
  DrawRectangle draw[2];
  u8 *sprites;
  s32 radius;
  s32 frames;
  s32 x;
  s32 y;
  struct EffectPosition target;
  struct EffectPosition source;
  struct ModelTexture texture;
  struct RenderModel *model;
  struct EffectStep *ball;
  struct EffectStep *bolt;
  struct EffectStep *spark;
  struct EffectStep *dot;
  void *vertices;
  s32 i;
  s32 k;
  u8 *buffer;

  work = (struct BattleEffectWork *)Ram_HeapSlots->battleFxWork[0];
  canvas = Ram_HeapSlots->battleFxWork[1];
  sprites = Ram_HeapSlots->unk64[0];
  buffer = Ram_MapCellBuffer;
  BattleEffect_StartCanvasLayer(1);
  if (mode == 0) {
    REG_BLDALPHA = 0x1010;
  } else {
    REG_BLDALPHA = 0x810;
  }
  BattleEffect_LoadBlitter(
      SLOT_BLITTER, BLITTER(BLIT_MAX, COMPOSITE_CLIP_X | COMPOSITE_CLIP_Y));
  draw[0] = (DrawRectangle)Ram_HeapSlots->unk64[1];
  radius = 0;
  Resource_LoadAndDecompress((s32)&ResourceId_TornadoSheet, work->sheet + 0x800,
                             1, 0);
  Resource_LoadAndDecompress((s32)&ResourceId_Data134, sprites, 0, 0);
  Resource_LoadAndDecompress((s32)&ResourceId_LightningBoltSheet, buffer, 1, 1);
  if (mode == 1) {
    Iwram_CopyWords((void *)BG_PLTT,
                    Resource_GetTableEntry((s32)&ResourceId_VioletPaletteC),
                    128);
  }

  // The bolts' four pictures, 24 by 120, each in a 32-wide cell.
  Iwram_ClearWordsEntry(work->sheet + 0x1456, 0x4000);
  for (i = 0; i != 4; i++) {
    for (k = 0; k != 120; k++) {
      s32 j;

      for (j = 0; j != 24; j++) {
        work->sheet[0x145a + i * 0x1000 + k * 32 + j] =
            buffer[(i * 45 << 6) + k * 24 + j];
      }
    }
  }
  if (mode == 0) {
    work->transferMode = 2;
    work->transferValue = 50;
  } else {
    work->transferMode = 1;
    work->transferValue = 0;
  }
  Task_Add(BattleEffect_ProcessPendingGraphicsTransfer, 0xc80);
  if (mode == 0) {
    frames = 24;
    /* FAKEMATCH(barrier): an empty asm after Thunder Mine's 24, which puts
     * its arm over BRANCH_COST so if-conversion keeps both arms; as the bare
     * `frames = 24;`, 27 of 747 instructions differ in tla-en. */
    asm("");
  } else {
    frames = 40;
  }

  for (frame = 0; frame != 50; frame++) {
    ball = work->particles;
    if (frame == 0) {
      EffectPosition_ProjectBattler(effect->actor, &source);
      EffectPosition_ProjectBattler(effect->actors[0], &target);
      if (mode == 0) {
        ball->x = source.x << 16;
        ball->y = (source.y - 32) << 16;
        ball->velocityX = ((target.x << 16) - ball->x) / frames;
      } else {
        if (effect->side == 0) {
          ball->x = (source.x - 32) << 16;
        } else {
          ball->x = (source.x + 32) << 16;
        }
        ball->y = (source.y - 80) << 16;
        ball->velocityX = (((target.x - 32) << 16) - ball->x) / frames;
      }
      ball->velocityY = (((target.y - 32) << 16) - ball->y) / frames;
      Sound_PlayFar(221);
      for (i = 0, bolt = &work->particles[1]; i != 63; i++, bolt++) {
        bolt->z = Random16();
        bolt->velocityZ = (Random16() & 255) + 128;
        if (Random16() & 1) {
          bolt->velocityZ = -bolt->velocityZ;
        }
        bolt->variant = Random16() & 3;
      }
      // UB: the variants are cleared through one s32 pointer stepped a whole
      // step at a time, past the step's scalar members.
#ifdef UBFIX
      for (i = 0; i != 512; i++)
        gData_02014000__f16fca8[i].variant = -1;
#else
      {
        s32 *variant = &gData_02014000__f16fca8[0].variant;

        for (i = 0; i != 512; i++) {
          *variant = -1;
          variant += sizeof(struct EffectStep) / sizeof(s32);
        }
      }
#endif
    }

    // Arrival.
    if (frame == frames) {
      s32 n;

      for (n = 0, spark = gData_02014000__f16fca8; n != 128; n++, spark++) {
        s32 angle;
        s32 speed;

        spark->x = ball->x / 2;
        spark->y = ball->y;
        angle = Random16();
        speed = Random16() % 400 + 32;
        spark->velocityX = (Trig_Sin(angle) * speed) >> 7;
        spark->velocityY = (Trig_Cos(angle) * speed) >> 6;
        spark->variant = (Random16() & 7) + 16;
      }
      Iwram_FillWordsEntry(canvas, 0x3c00, 0x3f3f3f3f);
      work->shakeFrames = 16;
      BattleMotion_ApplyKnockbackFar(effect->actors[0], 4);
      Func_0814cd48(effect->actors[0], 7, 5, 0, 8);
      if (mode == 0) {
        Func_081180e8(145);
      } else {
        Func_081180e8(-1);
        Sound_PlayFar(145);
      }
    }

    // The ball, growing to 48 (64), with four flashes about it.
    if (frame <= frames) {
      s32 largest;

      ball->variant++;
      x = ball->x >> 17;
      y = ball->y >> 16;
      if (mode == 0) {
        radius = frame * 2 + 16;
        largest = 48;
      } else {
        radius = frame * 2 + 16;
        largest = 64;
      }
      if (radius > largest) {
        radius = largest;
      } else {
        Func_0815b434(buffer, radius, 85536);
        if (mode == 1) {
          for (i = 0; i != radius * radius / 2; i++) {
            if (buffer[i] != 0) {
              buffer[i] = 64 - buffer[i];
            }
          }
        }
      }
      Func_0818caa8(buffer, x, y, radius);
      for (i = 0; i != 4; i++) {
        // DEAD: i runs 0 to 3, so the mask changes nothing; the source keeps
        // it.
        s32 cell = i & 3;
        s32 half = radius / 2;
        s32 angle = Random16();
        s32 flashX =
            x + ((Trig_Sin(angle) * half) >> 16) - (gData_08197492[cell] >> 1);
        s32 flashY =
            y + ((Trig_Cos(angle) * half) >> 16) - (gData_08197498[cell] >> 1);
        s32 flags = 3 | Data_08198bee[Random16() & 3];

        // Once-only block: it ends CSE's block, so the width and height tables
        // stay in the flash loop.
        do {
        } while (0);
        Func_08196404(188, 7, 7, flags, 1);
        ((DrawRectangle)Ram_HeapSlots->unkb8[1])(
            canvas, work->sheet + 0x800 + gData_08197486[cell], flashX, flashY,
            gData_08197492[cell], gData_08197498[cell]);
        HeapSlot_Release(188);
      }
      BattleEffectKernels_IntegrateVector2(ball, 64, 0);
    }

    // Thunder Mine's sparks bounce.
    if (mode == 0 && frame >= frames) {
      for (i = 0, dot = gData_02014000__f16fca8; i != 512; i++, dot++) {
        if (dot->variant >= 0) {
          s32 size = (dot->variant >> 3) + 2;
          s32 step;

          for (step = 0; step != 8; step++) {
            draw[0](canvas, sprites + gData_08197410[size - 1],
                    (dot->x >> 16) - size / 2, (dot->y >> 16) - size, size,
                    size * 2);
            BattleEffectKernels_IntegrateVector2(dot, 62, 0x200);
          }
          if ((dot->y >> 16) > 111) {
            dot->velocityY = -dot->velocityY * 48 / 64;
          }
          dot->variant--;
        }
      }
    }

    // The trailing bolts, each eight frames long.
    if (frame <= frames) {
      vertices = Heap_AllocEwram(32);
      model = Render_AllocateModelList(1);
      texture.logWidth = 5;
      texture.logHeight = 7;
      // DEAD: both kinds are overwritten by the 6 below; the source keeps them.
      if (mode == 0) {
        model->kind = 6;
      } else {
        model->kind = 9;
      }
      model->kind = 6;
      model->texture = &texture;
      model->mesh = gData_08199268;
      model->vertices = vertices;
      for (i = 0, bolt = &work->particles[1]; i != 40; i++, bolt++) {
        s32 shade;

        if (frame < i || frame >= i + 8) {
          continue;
        }
        shade = 0;
        if (frame == i) {
          shade = -16;
        } else if (frame >= i + 4) {
          /* FAKEMATCH(temporary): the fade negated through a local; as shade =
           * 64 - (frame - i) * 16, 4 of 747 instructions differ in tla-en. */
          s32 fade = (frame - i) * 16 - 64;

          shade = -fade;
        }
        texture.pixels = work->sheet + 0x1456 + (bolt->variant << 12);
        model->shade = shade;
        Render_ResetTransformState();
        Func_08015160(ball->x / 2 - 0x400000, ball->y - 0x400000, 0);
        Func_080150e4(bolt->z);
        bolt->z += bolt->velocityZ;
        if (mode == 0) {
          Func_080151e4(0x40000, 0x10000, 0x10000);
          Render_TransformVertices(gData_081991b0, vertices, 4);
        } else {
          Func_080151e4(radius << 11, radius * 312, 0x10000);
          Render_TransformVertices(Data_081991c0, vertices, 4);
        }
        Render_DrawModelList(model);
      }
      Heap_Release(model);
      Heap_Release(vertices);
    }
    BattleCamera_Shake(4, 4);
    BattleEffect_CountDownBattlerTints();
    work->transferPending = 1;
    wait(1);
  }
  Task_Remove(BattleEffect_ProcessPendingGraphicsTransfer);
  HeapSlot_Release(SLOT_BLITTER);
  BattleEffect_EndCanvasLayer();
}

#define HI(v) (((s16 *)&(v))[1])

void Func_0815f0a0(s32 palette);

void Func_0816442c(s32 formation, s32 x, s32 y); // ..PlaceFormationObjects
void Func_0816467c(s32 formation, s32 x, s32 y); // ..RunImpactBurst

extern char ResourceId_FlashBurstSheet;
extern char ResourceId_WhirlBeamSheet;

extern char ResourceId_SparkleDots;
extern char ResourceId_MarsDjinnSheet;
extern char ResourceId_OrangePaletteB;
extern char ResourceId_TanPalette;
#define SPARKS ((struct EffectStep *)Ram_MapCellBuffer) // 512 sparks
#define PICTURES (Ram_MapCellBuffer + 0x4000)
struct Point {
  s32 x;
  s32 y;
};
extern const struct Point Data_08196e94;

extern u16 Data_0819744c[]; // the sparkles' cells
extern u8 Data_0819745e[];  // and sizes

extern const u8 Data_08198bf4[];
extern const u8 Data_08198bf8[];

// Abilities: Megaera.
//
// Two bolts of lightning fall at frames 0 and 16, each with a burst of
// sparks; a whirl of flame turns over the field from 64 while smoke rises,
// and at 108 Megaera (eleven objects) swoops in across the targets in a
// trail of sparkles. Holding A or B after frame 10 skips to the swoop.
void BattleEffect_RunMegaera(struct BattleEffectArgument *effect) {
  void *canvas;
  struct BattleEffectWork *work;
  s32 frame;
  u8 *sprites;
  DrawRectangle *rects;
  struct Point position;
  struct Point velocity;
  DrawRectangle blitters[2];
  struct Point request;
  s32 placement[4];
  struct EffectStep *p;
  u8 *pictures;
  s32 i;
  s32 j;

  canvas = Ram_HeapSlots->battleFxWork[1];
  work = (struct BattleEffectWork *)Ram_HeapSlots->battleFxWork[0];
  sprites = Ram_HeapSlots->unk64[0];
  pictures = PICTURES;
  BattleEffect_ConfigureEffectDisplay();
  BattleEffect_StartCanvasLayer(0);
  REG_BLDALPHA = 0x1010;
  rects = blitters;
  BattleEffect_FetchRectangleBlitters(0, (void **)rects);
  Resource_LoadAndDecompress((s32)&ResourceId_FlashBurstSheet,
                             work->sheet + 0x3800, 1, 0);
  Resource_LoadAndDecompress((s32)&ResourceId_WhirlBeamSheet, work->sheet, 1,
                             1);
  Resource_LoadAndDecompress((s32)&ResourceId_Data134, sprites, 0, 0);
  Resource_LoadAndDecompress((s32)&ResourceId_TornadoSheet, pictures, 1, 0);
  Resource_LoadAndDecompress((s32)&ResourceId_LightningBoltSheet,
                             pictures + 0xc56, 1, 0);
  Resource_LoadAndDecompress((s32)&ResourceId_SparkleDots, pictures + 0x2e16, 0,
                             0);
  work->transferMode = 2;
  work->transferValue = 75;
  Task_Add(BattleEffect_ProcessPendingGraphicsTransfer, 0xc80);
  for (i = 0; i != 64; i++) {
    work->particles[i].variant = -1;
  }
  for (i = 0, p = &work->particles[24]; i != 16; i++, p++) {
    p->x = Random16() & 127;
    p->y = (Random16() & 7) + 56;
    p->variant = -(Random16() & 15);
  }
  for (i = 0, p = work->particles; i != 32; i++, p++) {
    p->x = ((Random16() & 31) + 28) << 16;
    p->y = ((Random16() % 48) + 56) << 16;
    p->velocityX = ((Random16() & 255) - 128) << 12;
    p->velocityY = (16 - (Random16() & 127)) << 10;
    p->variant = ~i;
  }
  for (i = 0; i != 512; i++) {
    SPARKS[i].variant = -1;
  }
  Iwram_CopyWordsEntry((void *)BG_PLTT,
                       Resource_GetTableEntry((s32)&ResourceId_MarsDjinnSheet),
                       128);
  BattleEffect_SelectLivingTargets(work->effect);
  wait(1);

  for (frame = 0; frame != 170; frame++) {
    if ((gInput.repeat & 3) && frame > 10) {
      if (frame > 107) {
        goto skipped;
      }
      frame = 107;
    }
    if (frame == 107) {
      Sound_PlayFar(212);
    }
  skipped:
    if (frame == 148) {
      Func_081180e8(134);
    }
    if (frame == 0) {
      position.x = 0x200000;
      position.y = 0x480000;
      velocity.x = 0x90000;
      velocity.y = -0x20000;
      BattleEffect_SpawnObjects(2, 657, 2);
    }
    if (frame == 108) {
      ResourceObject_ReleaseFar(work->objects[0]);
      ResourceObject_ReleaseFar(work->objects[1]);
      BattleEffect_SpawnObjects(11, 656, 2);
    }

    // Megaera swoops, trailing sparkles.
    if (frame > 107) {
      position.x += velocity.x;
      position.y += velocity.y;
      velocity.x = velocity.x * 59 / 64;
      velocity.y = velocity.y * 59 / 64;
      Func_0816442c(4, position.x, position.y);
      if (frame == 108) {
        for (i = 0; i != 512; i++) {
          SPARKS[i].variant = -1;
        }
      }
      {
        s32 t = frame - 108;
        if ((u32)t <= 23) {
          s32 count = 10 - t / 2;

          if (count > 0) {
            s32 half;

            for (i = 0, half = position.x / 2; i != count / 5; i++) {
              struct EffectStep *cell;
              s32 angle;
              s32 radius;

              cell = &SPARKS[((t << 5) + i) & 511];
              angle = Random16();
              radius = (Random16() & 31) + 16;
              cell->x = Trig_Sin(angle) * radius + half + 0x100000;
              cell->y = Trig_Cos(angle) * radius + position.y - 0x80000;
              cell->velocityX = 0;
              cell->velocityY = 0;
              cell->variant = Random16() & 15;
            }
          }
        }
      }
    }
    if ((u32)(frame - 64) <= 3) {
      Iwram_FillWordsEntry(canvas, 0x4000, 0x3f3f3f3f);
    }
    if (frame > 103) {
      Func_0815f0a0((s32)&ResourceId_TanPalette);
    }
    if (frame == 64) {
      Iwram_CopyWordsEntry(
          (void *)BG_PLTT,
          Resource_GetTableEntry((s32)&ResourceId_OrangePaletteB), 128);
    }

    // The whirl of flame, and smoke.
    if (frame > 63) {
      if (frame <= 111) {
        s32 picture = (frame / 4) % 3;
        s32 sway = ((Trig_Sin(frame << 11) << 3) >> 16) - 16;
        s32 which = frame == 108;
        DrawRectangle *draw = &blitters[which];
        u8 *cells = work->sheet + picture * 3 * 0x800;

        (*draw)(canvas, cells, sway, 0, 96, 64);
        (*draw)(canvas, cells, sway, 64, 96, 52);
        {
          s32 picture2 = (frame / 2) % 3;
          u8 *cells2;

          sway = (Trig_Sin((frame << 12) + 0x8000) << 3) >> 16;
          cells2 = work->sheet + picture2 * 3 * 0x800;
          (*draw)(canvas, cells2, sway, 0, 96, 64);
          (*draw)(canvas, cells2, sway, 64, 96, 52);
        }
      }
      for (i = 0, p = work->particles; i != 24; i++, p++) {
        if ((u32)p->variant <= 15) {
          s32 row = p->variant / 4;
          s32 which = i & 1;
          u8 *smoke = work->sheet + 0x4800 + (row << 11);

          rects[which](canvas, smoke, HI(p->x) - 16, HI(p->y) - 32, 32, 64);
          BattleEffectKernels_IntegrateVector2(p, 62, -0x4000);
        }
        p->variant++;
        if (frame == 108) {
          p->velocityX += 0x20000;
          p->velocityY /= 4;
        }
        if (frame <= 107) {
          s32 v = p->variant;

          if (v == 16) {
            p->variant = v = 0;
          }
          if (v == 0) {
            p->x = ((Random16() & 31) + 28) << 16;
            p->y = ((Random16() % 48) + 56) << 16;
            p->velocityX = ((Random16() & 255) - 128) << 12;
            p->velocityY = (16 - (Random16() & 127)) << 10;
          }
        }
      }
    }
    if (frame == 64) {
      Sound_PlayFar(163);
    }
    {
      s32 crackle;

      for (i = 0, crackle = 75; i != 8; i++, crackle += 4) {
        if (frame == crackle) {
          Sound_PlayFar(134);
        }
      }
    }
    if (frame == 10) {
      work->shakeFrames = i;
      for (i = 0; i != effect->count; i++) {
        Func_0814cd48(effect->actors[i], 7, 5, i, 16);
      }
    }
    if (frame > 107) {
      for (i = 0, p = SPARKS; i != 512; i++, p++) {
        s32 k;

        if ((u32)p->variant > 35) {
          continue;
        }
        k = (p->variant / 2) % 9;
        blitters[0](canvas, (PICTURES + 0x2e16) + Data_0819744c[k],
                    HI(p->x) - Data_0819745e[k] / 2,
                    HI(p->y) - Data_0819745e[k] / 2, Data_0819745e[k],
                    Data_0819745e[k]);
        BattleEffectKernels_IntegrateVector2(p, 58, 0);
        if (i & 1) {
          p->velocityX += -0x800;
          p->velocityY += 0x1000;
        } else {
          p->velocityX += -0x1000;
          p->velocityY += 0x800;
        }
        p->variant++;
      }
    }

    // Two bolts of lightning.
    if (frame <= 63) {
      HeapSlot_Release(188);
      for (i = 0; i != 2; i++) {
        s32 start = i * 16;
        s32 column = i * 32 + 24;

        if (frame == start + 5) {
          Sound_PlayFar(212);
        }
        if (frame == start + 9) {
          work->shakeFrames = 8;
          Iwram_FillWordsEntry(canvas, 0x3c00, 0x3f3f3f3f);
          Sound_PlayFar(144);
        }
        if (frame >= start) {
          s32 t = frame - start;
          s32 fall = t * 16 - 16;

          request = Data_08196e94;
          if (fall > 131) {
            fall = 132;
          }
          placement[3] = 0;
          placement[1] = 0;
          placement[0] = column << 17;
          placement[2] = fall << 16;
          Render_ApplyProjectedPlacementFar(work->objects[i], placement,
                                            &request.x, 0);
          if (t == 9) {
            Iwram_ClearWordsEntry(
                (u8 *)0x06010e00 +
                    ((u32)(((u16 *)work->objects[i])[4] << 22) >> 17),
                0x200);
          }
          if (frame == start + 10) {
            for (j = 0; j != 256; j++) {
              struct EffectStep *spark = &SPARKS[i * 256 + j];
              s32 angle = Random16() & 0x7fff;
              s32 speed = Random16() & 255;

              spark->x = column << 16;
              spark->y = 0x700000;
              spark->velocityY = (-Trig_Sin(angle) * speed) >> 5;
              spark->velocityX = (Trig_Cos(angle) * speed) >> 5;
              spark->variant = 0;
            }
          }
          if (frame >= start + 10 && frame < start + 18) {
            blitters[0](canvas,
                        (PICTURES + 0xc56) + (((frame + i) / 4) & 3) * 2880,
                        column - 12, 0, 24, 112);
          }
          if (frame >= start + 10) {
            for (j = 0; j != 2;) {
              s32 k = j & 3;
              s32 gx = column + (Random16() & 7) - (gData_08197492[k] >> 1);
              s32 gy = (Random16() & 63) - (gData_08197498[k] >> 1);

              // An empty once-only loop ends CSE's block, so the table reads
              // stay apart; without it, 188 of 1152 instructions differ in
              // tla-en.
              do {
              } while (0);
              Func_08196404(188, 7, 7, 3 | Data_08198bf4[Random16() & 3],
                            Data_08198bf8[effect->variant]);
              ((DrawRectangle)Ram_HeapSlots->unkb8[1])(
                  canvas, PICTURES + gData_08197486[k], gx - 4, gy + 40,
                  gData_08197492[k], gData_08197498[k]);
              j++;
              HeapSlot_Release(188);
            }
          }
        }
      }
      BattleEffect_LoadBlitter(188, 3);
      rects[1] = (DrawRectangle)Ram_HeapSlots->unkb8[1];
      if (frame <= 69) {
        for (i = 0, p = SPARKS; i != 512; i++, p++) {
          s32 size;

          if (p->variant < 0) {
            continue;
          }
          size = i % 3 + 1;
          blitters[p->velocityY > 0](canvas, sprites + gData_08197410[size - 1],
                                     HI(p->x) - size / 2, HI(p->y) - size, size,
                                     size * 2);
          BattleEffectKernels_IntegrateVector2(p, 62, 0x4000);
          p->variant++;
          if (p->velocityY > 0 && HI(p->y) > 104) {
            p->variant = -1;
          }
        }
      }
    }
    BattleCamera_Shake(8, 8);
    BattleEffect_CountDownBattlerTints();
    work->transferPending = 1;
    wait(1);
  }
  Task_Remove(BattleEffect_ProcessPendingGraphicsTransfer);
  HeapSlot_Release(188);
  HeapSlot_Release(104);
  Func_0816467c(4, position.x, position.y);
  for (i = 0; i != 11; i++) {
    ResourceObject_ReleaseFar(work->objects[i]);
  }
  BattleEffect_EndCanvasLayer();
}

struct FxPartyList;
struct Model;
struct StripQuad;

void BattleCamera_SetViewFar(s32 a, s32 b, s32 c, s32 d);
void Camera_ApplyPhasedDelta(void); // the camera's sway

extern char ResourceId_EmberStreakSheet;
extern char ResourceId_CometSheetB;
extern char ResourceId_RuneSheet;
extern u8 gMapCellBuffer[];
extern const s8 Data_08196e9c[]; // the hexagon's corners (z, y)

#define VOID_BEAM_FRAMES 88
#define BEAM_SEGMENTS 10

// Abilities: Void Beam.
//
// A hexagon of motes is traced, side by side, around the caster while a
// ring of 64 gathers behind; at frame 72 a beam (a 32-line gradient,
// repeated ten times) shoots from the caster toward the targets' side,
// fading out from frame 80 with the blend.
void BattleEffect_RunVoidBeam(struct BattleEffectArgument *effect) {
  struct BattleEffectWork *work;
  void *canvas;
  s32 frame;
  DrawRectangle draw[2];
  s32 *camera;
  u8 *sprites;
  s32 point[3];
  struct EffectPosition corners[11];
  struct EffectPosition ring;
  struct EffectPosition segment;
  s32 flip;
  s32 count;
  u8 *row;
  s32 i;
  s32 k;
  u8 *buffer;
  s32 size;

  work = (struct BattleEffectWork *)Ram_HeapSlots->battleFxWork[0];
  canvas = Ram_HeapSlots->battleFxWork[1];
  camera = Ram_HeapSlots->cameraWork;
  sprites = Ram_HeapSlots->unk64[0];
  buffer = Ram_MapCellBuffer;
  BattleEffect_StartCanvasLayer(1);
  Resource_LoadAndDecompress((s32)&ResourceId_EmberStreakSheet, work->sheet, 1,
                             1);
  Resource_LoadAndDecompress((s32)&ResourceId_CometSheetB, work->sheet + 0x1000,
                             1, 0);
  Resource_LoadAndDecompress((s32)&ResourceId_Data134, sprites, 0, 0);

  // The motes, one shade dimmer.
  for (i = 0; i != 770; i++) {
    s32 pixel = sprites[i];

    if (pixel != 0) {
      pixel -= 8;
      if (pixel <= 0) {
        pixel = 1;
      }
      sprites[i] = pixel;
    }
  }
  Resource_LoadAndDecompress((s32)&ResourceId_RuneSheet, buffer + 0x1000, 1, 0);
  Iwram_ClearWordsEntry(buffer, 0x1000);

  // The beam's head: the rune, mirrored to 48 columns.
  for (i = 0; i != 48; i++) {
    s32 j;

    for (j = 0; j != 24; j++) {
      buffer[0x208 + i * 64 + j] = buffer[0x1000 + i * 24 + j];
      buffer[0x208 + i * 64 + 47 - j] = buffer[0x1000 + i * 24 + j];
    }
  }
  Iwram_CopyWords((void *)BG_PLTT,
                  Resource_GetTableEntry((s32)&ResourceId_LightningBoltSheet),
                  128);

  // The beam's body: a 32-line gradient.
  row = work->sheet + 0x1a20;
  for (i = 0; i != 32; i++) {
    s32 shade = (Trig_Sin(i << 9) * 63) >> 16;
    s32 j;

    for (j = 0; j != 32; j++) {
      *row++ = shade;
    }
  }
  REG_BLDALPHA = 0x1010;
  flip = 4;
  if (effect->side != 0) {
    flip = 0;
  }
  Func_08196404(SLOT_BLITTER, 7, 7, flip | 3, 1);
  Func_08196404(188, 7, 7, flip | 11, 1);
  draw[0] = (DrawRectangle)Ram_HeapSlots->unk64[1];
  draw[1] = (DrawRectangle)Ram_HeapSlots->unkb8[1];
  work->cameraDelta = 0;
  work->cameraPhase = 0;
  Task_Add(Camera_ApplyPhasedDelta, 0x480);
  work->transferMode = 2;
  work->transferValue = 50;
  Task_Add(BattleEffect_ProcessPendingGraphicsTransfer, 0xc80);
  Sound_PlayFar(140);
  size = 4;

  for (frame = 0; frame != VOID_BEAM_FRAMES; frame++) {
    if ((u32)(frame - 9) <= 54) {
      work->cameraDelta = -256;
    } else if ((u32)(frame - 64) <= 15) {
      work->cameraDelta = frame * 16 - 1280;
    } else {
      work->cameraDelta = 0;
    }
    if (effect->side == 1) {
      work->cameraDelta = -work->cameraDelta;
    }
    if (frame > 79) {
      s32 fade = frame * 2 - 160;

      REG_BLDALPHA = (16 - fade) | 0x1000;
    }
    BattleCamera_SetViewFar(0, 0, 0, 100);
    Render_ResetTransformState();
    Transform_SetLookAt(camera, camera + 3);
    {
      struct MotionObject *caster;
      s32 traced;

      caster = GetBattleObjectSlotFar(effect->actor)->object;
      if (caster->x > 0) {
        point[0] = caster->x - 0x80000;
      } else {
        point[0] = caster->x + 0x80000;
      }

      // The hexagon, one side every 36 frames.
      traced = frame * 8 - 336;
      if (traced >= 0) {
        for (i = 0; i != 6; i++) {
          s32 corner = (i + 4) % 6;

          point[1] = (Data_08196e9c[corner * 2 + 1] << 16) + 0x1e0000;
          point[2] = (Data_08196e9c[corner * 2] << 16) + caster->z;
          EffectPosition_ProjectPoint(point, &corners[i]);
          corners[i].x >>= 1;
        }
        for (i = 0; i != 6; i++) {
          s32 motes = traced - i * 36;

          if (motes > 36) {
            motes = 36;
          }
          if (motes < 0) {
            continue;
          }
          for (k = 0; k != motes; k++) {
            s32 x =
                corners[i].x +
                Iwram_MulQ16(k * (corners[(i + 1) % 6].x - corners[i].x), 1820);
            s32 y =
                corners[i].y +
                Iwram_MulQ16(k * (corners[(i + 1) % 6].y - corners[i].y), 1820);

            draw[0](canvas, sprites + gData_08197410[size - 1], x - size / 2,
                    y - size, size, size * 2);
          }
        }
      }

      // The ring behind the caster.
      count = frame * 3 / 2;
      if (count > 64) {
        count = 64;
      }
      for (i = 0; i != count; i++) {
        s32 angle = i << 10;

        point[1] = caster->y - Trig_Cos(angle) * 40 + 0x200000;
        point[2] = caster->z + Trig_Sin(angle) * 40;
        EffectPosition_ProjectPoint(point, &ring);
        ring.x >>= 1;
        if (ring.depth <= 159) {
          ring.depth = 160;
        }
        if (ring.depth > 799) {
          ring.depth = 799;
        }
        draw[1](canvas, sprites + gData_08197410[size - 1], ring.x - size / 2,
                ring.y - size, size, size * 2);
      }

      // The beam.
      if (frame > 71) {
        s32 fade;

        if (frame == 72) {
          work->particles[0].x = point[0];
          if (work->particles[0].x > 0) {
            work->particles[0].velocityX = -0x200000;
          } else {
            work->particles[0].velocityX = 0x200000;
          }
          REG_BG2CNT = 0x784;
        }
        if (frame == 72) {
          work->shakeFrames = 8;
          Func_081180e8(145);
          BattleMotion_ApplyKnockbackFar(effect->actors[0], 4);
          Func_0814cd48(effect->actors[0], 7, 5, 0, 8);
        }
        for (i = 0; i != BEAM_SEGMENTS; i++) {
          point[0] = work->particles[0].x + work->particles[0].velocityX * i;
          point[1] = 0x1e0000;
          point[2] = caster->z;
          EffectPosition_ProjectPoint(point, &segment);
          segment.x >>= 1;
          if (effect->side == 0) {
            segment.x -= 32;
          }
          if (i > 0) {
            draw[0](canvas, work->sheet + 0x1a20, segment.x, segment.y - 32, 32,
                    32);
            draw[1](canvas, work->sheet + 0x1a20, segment.x, segment.y, 32, 32);
          } else if (frame <= 81) {
            draw[0](canvas, work->sheet + 0x1000, segment.x, segment.y - 36, 32,
                    36);
            draw[1](canvas, work->sheet + 0x1000, segment.x, segment.y, 32, 36);
          }
        }
        row = work->sheet + 0x1a20;
        fade = 0;
        if (frame > 79) {
          fade = frame * 8 - 640;
        }
        for (i = 0; i != 32; i++) {
          s32 shade = ((Trig_Sin(i << 9) * 63) >> 16) - fade;
          s32 j;

          if (shade < 0) {
            shade = 0;
          }
          for (j = 0; j != 32; j++) {
            *row++ = shade;
          }
        }
      }
    }
    if (work->cameraPhase == 0) {
      work->cameraPhase = 1;
    }
    BattleCamera_Shake(8, 8);
    BattleEffect_CountDownBattlerTints();
    work->transferPending = 1;
    wait(1);
  }
  HeapSlot_Release(188);
  HeapSlot_Release(104);
  Task_Remove(BattleEffect_ProcessPendingGraphicsTransfer);
  Task_Remove(Camera_ApplyPhasedDelta);
  BattleEffect_EndCanvasLayer();
}
#undef SLOT_BLITTER
#undef BLITTER
#undef HI
#undef SPARKS
#undef PICTURES
#undef VOID_BEAM_FRAMES
#undef BEAM_SEGMENTS
