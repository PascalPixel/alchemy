// Draft, differ 90 of 1034 (tla-en, 2026-11-05); 45 with slots and pool offsets normalized, 10 with registers blind too. Measured, not kept: s32 c = *color; c <<= 16; blue = ((u32)c >> 26) & 31; with (red << 10) | (red << 5) | blue gives the listing's shifts and clamp but one shared mask, no pooled 31 (168). The pooled 31 and the shifts have not been had together.
// Draft, differ 90 of 1034 (tla-en, 2026-11-05); 45 with slots and pool offsets normalized, 10 with registers blind too. Blue's mask as a halfword gives the pooled 31 (98 to 90); (u16)(*color >> 10) still compiles to lsrs 10 where the listing has lsls 16, lsrs 26: combine knows the load is zero-extended. Hiding that with a tied empty asm moves the rest to 131.
// Measured 2026-11-05 (tla-en): differ 98: the model's mesh and vertices are typed pointers, not void.
// Measured 2026-11-05 (tla-en): 106 of 1034, unchanged. 280 + 300 typings of the colour loop's
// blue: none gives lsls 16, lsrs 26 with ldrh and a pooled 31 without costing more elsewhere.
// 2026-11-05: differ 106 of 1034 (tla-en). Green read, then set to red before the
// clamps: count and later rows agree, but the colour loop still has lsrs 10 and a
// copy for lsls 16, lsrs 26, so that loop is not settled.
// 2026-11-05: differ 130 of 1034 (tla-en). Tried on the colour loop, none
// giving lsls 16, lsrs 26: u16 and s16 locals with narrowing casts (165 to
// 172), u16 red and blue (141, 168), a bitfield (148), a mask local (184), an
// s32 local (181). Judgment's flash loop has the same shape: ldrh, lsls, lsrs
// 26 and a pooled 31.
// Draft, differ 142 of 1034, skeleton gap 34 (tla-en, 2026-11-01)
//
// BattleEffect_RunRisingDragonEnemy. Same instruction count, calls, control
// flow and frame (116); no device. What is left is one wrong shift and a run of
// reload registers it starts.
// - The colour loop has asrs #26 where the listing has lsrs #26, and takes its
//   pointer in r0 where the listing takes r4 (constants r6, r5). The pointer's
//   r0 moves reload's rotation by one (mov r1, r8 for mov r0, r8), which
//   accounts for most register differences from there to frame 134. The
//   listing's value is read once as HImode: ldrh, lsls 16, lsrs 26, and a
//   pooled 31 (HImode AND). (u16)((s16)*color >> 10) & 31 gives all of that but
//   a signed shift. Tried for lsrs, none keeps the lsls: u16 or s16 or s32 c,
//   s16 *color, (u16)*color, (u32)(c << 16) >> 26, a multi-set s32 value,
//   BG_PLTT[i + 96]; a three-minute permute of the loop found nothing. In
//   allocation r0 is free for the pointer here, so the listing has some reason
//   to skip it (a lower-priority pseudo preferring r0 is the lead); nothing
//   found.
// - The model block stores vertices before kind and shade; the listing after.
//   Statement moves, a six local and a late tex change nothing.
// - The opening's second reload of victim, and the burst's adds r7, r3, r2
// (here
//   r2, r3), are ties: every cast spelling of the sum compiles the same.
// - The trail's zero and base come one reload early (r3, r4 for r4, r7);
// setting
//   trail before i, in one line or apart, gives it r7 and costs 34.
// Settled this round, each measured:
// - blit is draw[2], for the word at sp+68.
// - cells is a set-once constant, so reload fetches it into r0; indexing
// through
//   (struct Corner *)(cells + j) keeps j * 8 apart from y's j * 2, so y is a
//   giv the inner loop hoists: the mov r3, sl copy and its spill at sp+12,
//   which closes the frame. strip is set-once too.
// - The x expression stays one expression, which is what puts it in r0, frees
// r3
//   for y's copy and gives r1 and r4 to j * 8 and j * 2018; 370 to 193.
// - BattleEffect_LoadBlitter returns s32. The model's stores are in listing
// order.
// - vertices is a local set once at the top; zero is set before the model is
//   allocated; the sparks are separate pointers, one per loop; first is a
//   variable, so (x * 4) * 28 multiplies by 28 after the shift.

#include "types.h"
#include "battle_effect.h"
#include "battle_effect_work.h"
#include "effect_step.h"
#include "io_reg.h"
#include "iwram_call.h"
#include "heap_release.h"
#include "ram_buffer.h"

struct ModelTexture {
  u8 logWidth;
  u8 logHeight;
  const void *pixels;
};

struct RenderModel {
  s32 kind;
  s32 unk04;
  const u8 *mesh;
  s16 *vertices;
  struct ModelTexture *texture;
  s32 shade;
  u8 scrollU;
  u8 scrollV;
};

void BattleEffect_StartCanvasLayer(s32 bgControl);
void BattleEffect_EndCanvasLayer(void);
void BattleEffect_ProcessPendingGraphicsTransfer(void);
void BattleEffect_CountDownBattlerTints(void);
s32 BattleEffect_LoadBlitter(s32 slot,
                             s32 modeAndFlags); // BattleEffect_BuildBlitter
void Func_0814cd48(s32 actor, s32 palette, s32 pose, s32 timerSlot,
                   s32 timerFrames);              // BattleEffect_TintBattler
void Func_0815b434(u8 *map, s32 size, s32 scale); // shaded circles
void Func_0816ae40(u8 *src, u8 *dst, s32 width, s32 height);
void Func_0815585c(struct BattleEffectArgument *effect, s32 packed, s32 *screen,
                   s32 *otherPoint);
void Func_0818caa8(void *cells, s32 x, s32 y, s32 size);
struct RenderModel *Render_AllocateModelList(s32 count);
void Render_TransformVertices(const void *corners, void *vertices, s32 count);
void Render_DrawModelList(struct RenderModel *model);
void Func_08015160(s32 x, s32 y, s32 z);
void Func_0801521c(s32 scale);
void SceneTransform_ApplyPitch(s32 angle);
void SceneTransform_ApplyYaw(s32 angle);
void Render_ResetTransformState(void);
void Func_080151e4(s32 x, s32 y, s32 z); // SceneTransform_ApplyScale3
void EffectPosition_ProjectBattler(s32 id, struct EffectPosition *out);
void BattleEffect_SpawnObjects(s32 count, s32 resource,
                               s32 mode); // SpawnObjects
void BattleEffect_SelectLivingTargets(
    struct BattleEffectArgument *effect); // SelectLivingTargets
void Func_080150e4(s32 angle);            // SceneTransform_ApplyRoll
void BattleEffect_StartActorMotion(s32 id, s32 force, s32 acceleration,
                                   s32 speedLimit, s32 velocityY, s32 percent);
void Render_ApplyProjectedPlacementFar(void *object, s32 *request, s32 *source,
                                       s32 flags);
void BattleEffect_FetchRectangleBlitters(s32 flip, void **out);
void Func_08118088(s32 actor, s32 motion); // BattleMotion_ApplyVariantMotionFar
void Func_081180e8(s32 phase);             // BattlePlayback_StartFar
void Func_08158ce0(s32 randomMask, u32 shakeRange); // Camera_ApplyShake
void BattleEffectKernels_IntegrateVector2(struct EffectStep *step, s32 gravity,
                                          s32 drag);
void **GetBattleObjectSlotFar(s32 objectId);
void *Heap_AllocEwram(s32 size);
void Resource_LoadAndDecompress(s32 resource, void *destination, s32 a, s32 b);
void *Resource_GetTableEntry(s32 resource);
void Task_Add(void (*func)(void), s32 order);
void Task_Remove(void (*func)(void));
void HeapSlot_Release(s32 slot);
void Sound_PlayFar(s32 cue);
void wait(s32 frames);
u32 Random16(void);
s32 Trig_Sin(s32 angle);
s32 Trig_Cos(s32 angle);

void Render_BuildQuadStrip(void *mesh, s32 columns, s32 width, s32 height);
void BattleEffectKernels_IntegrateVector3(struct EffectStep *step, s32 damping,
                                          s32 gravity);
s32 BattleUnit_GetEffectHeightFar(s32 unit);
void Transform_SetLookAt(s32 *eye, s32 *target);
void BattleEffect_StartTiledCanvas(s32 bgControl);
void BattleEffect_StartTiledCanvasFilled(s32 bgControl);
void Transform_Push(void);
void Transform_Pop(void);

// The blitter's heap slot, as a byte offset into Ram_HeapSlots.
#define SLOT_BLITTER 104 // unk64[1]

// ⚓️'s blitter modes run the other way from The Broken Seal's.
enum { BLIT_ADDITIVE, BLIT_MAX, BLIT_TRANSPARENT, BLIT_COPY };
#define BLITTER(mode, flags) ((mode) << 4 | (flags))
#define HI(v) (((s16 *)&(v))[1])

#include "bg_scroll.h"

void BattleBackground_LoadFar(s32 layer, s32 resourceId, s32 mode);
void Func_08164a4c(s32 red, s32 green, s32 blue); // Palette_BrightenBgEntries
void BattleEffect_BuildRepeatingTilemap(void);

extern char ResourceId_SmokeBandSheet;
extern char ResourceId_FlashBurstSheet;
extern char ResourceId_ClawSlashSheet;
extern char ResourceId_Data134;
extern char ResourceId_OrangePaletteB;
extern char ResourceId_RedHazeBackdrop;
extern u32 gCameraSceneParameters[];
struct Corner {
  s8 x;
  s8 y;
  s8 z;
  s8 pad;
};
struct CornerRow {
  struct Corner corners[2];
};
extern struct CornerRow gMapCellBuffer[];
// 256 sparks. The listing's pool holds this address plus each constant offset
// as one word (0x02014018, 0x02014e00, 0x02014e18), which only a constant
// address folds to; through the symbol, each offset becomes an extra add.
#define SPARKS FIXED_ADDRESS((struct EffectStep *), 0x02014000, gData_02014000)
extern u8 Data_02011000[];
extern u8 Data_02012000[];
extern const u16 gData_08197410[];
extern const u8 gData_08199340[];
extern const u8 Data_081991e0[];

#define SEGMENTS 33

// Abilities: Rising Dragon, the enemy's.
//
// The dragon bursts up under the target and carries it into the sky: the
// backdrop changes to a red haze scrolling past as the target is lifted,
// swaying, then is dropped back to the field at frame 166 and lands at 179.
void BattleEffect_RunRisingDragonEnemy(struct BattleEffectArgument *effect) {
  struct BattleEffectWork *work;
  void *canvas;
  u8 *vertices;
  u8 *strip;
  struct CornerRow *cells;
  DrawRectangle blit[2];
  u8 *sprites;
  s32 *victim;
  u16 *camera;
  s16 savedYaw;
  s32 height;
  s32 savedMode;
  s32 savedX;
  s32 savedZ;
  struct EffectPosition source;
  struct EffectPosition target;
  struct ModelTexture texture;
  struct RenderModel *model;
  struct ModelTexture *tex;
  s32 zero;
  s32 burstFrame;
  s32 yaw;
  struct EffectStep *spawned;
  struct EffectStep *particle;
  struct EffectStep *scatter;
  s32 frame;
  s32 i;
  s32 j;

  vertices = Data_02011000;
  strip = Data_02012000;
  cells = (struct CornerRow *)Ram_MapCellBuffer;
  work = (struct BattleEffectWork *)Ram_HeapSlots->battleFxWork[0];
  canvas = Ram_HeapSlots->battleFxWork[1];
  sprites = Ram_HeapSlots->unk64[0];
  Func_0813ba50();
  BattleEffect_StartCanvasLayer(1);
  BattleEffect_LoadBlitter(
      SLOT_BLITTER, BLITTER(BLIT_MAX, COMPOSITE_CLIP_X | COMPOSITE_CLIP_Y));
  blit[0] = (DrawRectangle)Ram_HeapSlots->unk64[1];
  REG_BLDALPHA = 0x1010;
  work->transferMode = 2;
  work->transferValue = 50;
  Task_Add(BattleEffect_ProcessPendingGraphicsTransfer, 0xc80);
  victim = *GetBattleObjectSlotFar(effect->actors[0]);
  camera = (u16 *)Ram_HeapSlots->cameraWork;
  savedYaw = camera[27];
  height = 0;
  savedMode = victim[18];
  savedX = victim[2];
  savedZ = victim[4];
  EffectPosition_ProjectBattler(effect->actors[0], &source);
  EffectPosition_ProjectBattler(effect->actors[0], &target);
  Resource_LoadAndDecompress((s32)&ResourceId_SmokeBandSheet, work->sheet, 1,
                             1);
  Resource_LoadAndDecompress((s32)&ResourceId_FlashBurstSheet,
                             work->sheet + 0x800, 1, 0);
  Resource_LoadAndDecompress((s32)&ResourceId_ClawSlashSheet,
                             work->sheet + 0x4000, 1, 0);
  Resource_LoadAndDecompress((s32)&ResourceId_Data134, sprites, 0, 0);
  REG_BG2PA = 0x80;
  Render_BuildQuadStrip(strip, 4, 32, 32);

  for (frame = 0; frame != 186; frame++) {
    if (frame == 0) {
      Iwram_CopyWords((void *)BG_PLTT,
                      Resource_GetTableEntry((s32)&ResourceId_OrangePaletteB),
                      128);
      for (i = 0; i != 64; i++) {
        work->particles[i].variant = -1;
      }
      for (i = 0; i != 128; i++) {
        SPARKS[128 + i].variant = -1;
      }
      for (i = 0, spawned = SPARKS; i != 128; i++, spawned++) {
        s32 angle;
        s32 speed;

        spawned->x = (source.x / 2) << 16;
        spawned->y = 240 << 15;
        angle = (Random16() & 0x7fff) + 0x4000;
        speed = (Random16() & 1023) + 32;
        spawned->velocityX = (Trig_Sin(angle) * speed) >> 6;
        spawned->velocityY = (Trig_Cos(angle) * speed) >> 6;
        spawned->variant = (Random16() & 7) + 32;
      }
    }
    if (frame == 4) {
      Func_0814cd48(effect->actors[0], 7, 5, 0, 2);
      work->shakeFrames = 8;
      Sound_PlayFar(145);
    }
    if (frame == 163) {
      REG_BG2CNT = 0x784;
    }
    if (frame == 164) {
      BattleBackground_LoadFar(1, ((u16 *)Ram_HeapSlots->battleWork)[824], 24);
    }
    if (frame == 165) {
      gCameraSceneParameters[4] = 120;
      Iwram_CopyWords((void *)BG_PLTT,
                      Resource_GetTableEntry((s32)&ResourceId_OrangePaletteB),
                      128);
    }
    if (frame == 166) {
      camera[27] = savedYaw;
      victim[18] = savedMode;
      victim[2] = savedX;
      victim[3] = 200 << 15;
      victim[4] = savedZ;
      gBgScroll[1].y = 32;
      Func_0814cd48(effect->actors[0], 7, 5, 0, 16);
    }
    if (frame == 179) {
      Func_081180e8(134);
      work->shakeFrames = 8;
    }
    if (frame > 165) {
      work->transferMode = 3;
      work->transferValue = 0x04040404;
      goto next;
    }

    if (frame == 59) {
      Iwram_FillWords(canvas, 0x4000, 0x3f3f3f3f);
    }
    if (frame == 60) {
      u16 *color;

      // Into the sky: the red haze, its palette made grey-blue.
      BattleBackground_LoadFar(1, (s32)&ResourceId_RedHazeBackdrop, 0);
      for (i = 0, color = (u16 *)(BG_PLTT + 96); i != 128; i++, color++) {
        s32 red = *color & 31;
        s32 green = red;
        s32 blue = (u16)(*color >> 10) & (u16)31;
        if (red > 31) {
          red = 31;
        }
        if (blue > 31) {
          blue = 31;
        }
        *color = (red << 10) | (green << 5) | blue;
      }
      BattleEffect_BuildRepeatingTilemap();
      gBgScroll[1].y = 104;
      gCameraSceneParameters[4] = 240;
      if (effect->side == 0) {
        yaw = 0x4000;
        camera[27] = yaw;
      } else {
        yaw = 0xc000;
        camera[27] = yaw;
      }
      for (i = 0; i != 256; i++) {
        SPARKS[i].variant = -1;
      }
      height = 220;
    }
    if (frame == 134) {
      Sound_PlayFar(145);
      Iwram_FillWords(canvas, 0x4000, 0x3f3f3f3f);
    }
    if (frame > 59) {
      s32 scroll = gBgScroll[1].y;
      s32 climb;

      if (scroll > 104) {
        scroll -= 0x10000;
      }
      scroll -= 8;
      if (scroll < -16) {
        scroll += 120;
      }
      gBgScroll[1].y = scroll;
      climb = 4 - (frame - 60) / 8;
      if (climb < 0) {
        climb = 0;
      }
      if (frame > 119) {
        climb = frame - 120;
      }
      height -= climb;
    } else {
      height = 200 - frame * 8;
    }
    if (height <= 55) {
      height = 56;
    }
    victim[18] = 0;
    if ((u32)(frame - 4) <= 55) {
      victim[3] = (frame * 9 << 16) - (18 << 16);
    } else if (frame > 59) {
      victim[2] = 0;
      victim[3] = (294 - height) << 16;
      victim[4] = -(Trig_Sin(frame << 10) << 3);
      if (frame == 60) {
        EffectPosition_ProjectBattler(effect->actors[0], &source);
      }
      EffectPosition_ProjectBattler(effect->actors[0], &target);
    }
    if ((u32)(frame - 152) <= 15) {
      work->transferMode = 1;
      work->transferValue = 0x20202020;
      if (frame > 162) {
        work->transferValue = 0x3f3f3f3f;
      }
      Func_08164a4c(2, 2, 2);
    }
    if (frame < 0) {
      goto next;
    }

    // The dragon's body, swaying.
    for (j = 0; j != SEGMENTS; j++) {
      for (i = 0; i != 2; i++) {
        ((struct Corner *)(cells + j))[i].x =
            i * 20 + ((Trig_Sin(j * 2018 + (frame << 10)) * 2) >> 16) - 10;
        ((struct Corner *)(cells + j))[i].z = 0;
        ((struct Corner *)(cells + j))[i].y = j * 2 - 64;
      }
    }
    zero = 0;
    model = Render_AllocateModelList(1);
    tex = &texture;
    model->kind = 6;
    model->shade = zero;
    model->scrollV = zero;
    model->vertices = vertices;
    tex->logWidth = 6;
    tex->logHeight = 5;
    model->texture = tex;
    tex->pixels = work->sheet;
    model->scrollU = 127 & ~(frame << 2);
    model->mesh = strip;
    Render_ResetTransformState();
    Func_080151e4(0x8000, 0x10000, 0x8000);
    Transform_Push();
    Func_08015160((source.x - 128) << 16, height << 16, 0);
    if (effect->side == 1) {
      SceneTransform_ApplyYaw(0x8000);
    }
    Func_0801521c(0x21770);
    Render_TransformVertices(cells, vertices, 66);
    Render_DrawModelList(model);
    Transform_Pop();
    if (frame <= 133 && (u32)(frame - 17) > 42) {
      // The claw.
      tex->logWidth = 6;
      tex->logHeight = 6;
      model->mesh = gData_08199340;
      model->scrollU = zero;
      tex->pixels = work->sheet + 0x4000;
      Func_08015160((target.x - 124) << 16, (height - 144) << 16, 0);
      Func_080150e4(0x4000);
      Func_080151e4(0x19c40, 0x14e20, 0x10000);
      Render_TransformVertices(Data_081991e0, vertices, 4);
      Render_DrawModelList(model);
    }
    Heap_Release(model);

    if (frame == 134) {
      for (i = 0, scatter = SPARKS; i != 256; i++, scatter++) {
        s32 angle;
        s32 speed;

        scatter->x = (source.x / 2) << 16;
        scatter->y = 0;
        angle = (Random16() & 0x7fff) - 0x4000;
        speed = (Random16() & 255) + 32;
        scatter->velocityX = (Trig_Sin(angle) * speed) >> 5;
        scatter->velocityY = (Trig_Cos(angle) * speed) >> 5;
        scatter->variant = (Random16() & 7) + 32;
      }
    }
    burstFrame = frame - 134;
    if ((u32)burstFrame <= 7) {
      s32 first = burstFrame * 4;
      struct EffectStep *burst;

      for (i = 0, burst = &work->particles[first]; i != 4; i++, burst++) {
        s32 angle;
        s32 speed;

        burst->x = (source.x / 2) << 16;
        burst->y = 0;
        angle = (Random16() & 0x7fff) - 0x4000;
        speed = (Random16() & 255) + 32;
        burst->velocityX = (Trig_Sin(angle) * speed) >> 5;
        burst->velocityY = (Trig_Cos(angle) * speed) >> 5;
        burst->variant = 0;
      }
    }
    for (i = 0, particle = work->particles; i != 32; i++, particle++) {
      s32 picture = particle->variant;

      if ((u32)picture <= 23) {
        if (picture > 1) {
          blit[0](canvas, work->sheet + 0x800 + ((picture / 4) << 11),
                  HI(particle->x) - 16, HI(particle->y) - 32, 32, 64);
        }
        BattleEffectKernels_IntegrateVector2(particle, 60, -0x1000);
        particle->variant++;
      }
    }
    if ((u32)frame <= 133) {
      struct EffectStep *trail;
      s32 startY;
      s32 slot = (frame - 16) & 63;
      s32 first;
      s32 y = 120;

      if (frame > 59) {
        y = height - 80;
      }
      startY = y << 16;
      first = slot * 2;
      for (i = 0, trail = &SPARKS[128 + first]; i != 2; i++, trail++) {
        s32 random;
        s32 angle;
        s32 speed;

        trail->x = (source.x / 2) << 16;
        trail->y = startY;
        random = Random16() & 0x7fff;
        angle = random + 0x4000;
        if (frame > 59) {
          angle = random + 0xc000;
        }
        speed = (Random16() & 255) + 32;
        trail->velocityX = (Trig_Sin(angle) * speed) >> 6;
        trail->velocityY = (Trig_Cos(angle) * speed) >> 6;
        if (frame > 59) {
          trail->velocityX /= 2;
        }
        trail->variant = (Random16() & 7) + 32;
      }
    }
    {
      struct EffectStep *spark;
      s32 gravity = -0x1000;

      if (frame > 59) {
        gravity = 0x4000;
      }
      for (i = 0, spark = SPARKS; i != 256; i++, spark++) {
        s32 size;

        if (spark->variant < 0) {
          continue;
        }
        size = (spark->variant - 8) / 8;
        if (frame > 59) {
          size += 2;
        }
        if (size <= 0) {
          size = 1;
        }
        blit[0](canvas, sprites + gData_08197410[size - 1],
                HI(spark->x) - size / 2, HI(spark->y) - size, size, size * 2);
        BattleEffectKernels_IntegrateVector2(spark, 60, gravity);
        spark->variant--;
      }
    }
  next:
    BattleEffect_CountDownBattlerTints();
    if ((u32)(frame - 32) > 133) {
      Func_08158ce0(8, 8);
    }
    work->transferPending = 1;
    wait(1);
  }
  HeapSlot_Release(104);
  Task_Remove(BattleEffect_ProcessPendingGraphicsTransfer);
  BattleEffect_EndCanvasLayer();
}
