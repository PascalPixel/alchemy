// 2026-11-05: differ 20 of 1748 (tla-en). Wrappers on the three palette copies
// gain 6. Left: mode 4's giv order and the sched2 rows below; pin and
// statement-move searches found nothing.
// Draft, differ 36 of 1748, skeleton gap 13 (tla-en, 2026-11-01)
// Same instruction count as the listing; the frame (176 bytes), every stack
// slot and all the loops' registers match. What is left, about 18 pairs:
// - Scheduling only, same instructions (K2's load and split-constant model, a
//   compiler question): ldr effect before frames = 38's store; the three
//   BG_PLTT copies (the listing's ldr of IwramCopyWords comes before
//   lsls #19); the third particles[63] copy of each of the three blocks (its
//   three-insn constant outranks the store here, lsls r2 comes after it in the
//   listing); mov r2, fp in mode 2; str [sp, #28] in mode 3; mode 3's
//   mov r7, sl; model->shade's store against the two scroll stores.
// - Mode 4's tail: x (sp 32) and the row (sp 24) update in the other order,
//   so r3 and r5 swap, and x's zero comes from r4 instead of r1. x as
//   j * step in the loop is the nearest form.
// What the listing showed (each moved differ by tens or hundreds):
// - resource ids load from the pool, so they are ResourceId_ symbols;
// - buffers are Ram_MapCellBuffer plus a constant, never symbols: as symbols
//   load_mems hoists the mirror's 0x02014208 into lr, which the listing rebuilds
//   in the outer loop, and the rows as a cast of the constant give j * 8 its
//   own running value;
// - vertices, mesh and sheet are one-constant locals of the whole function:
//   they get no register, so they rematerialize at each use (the pool order
//   and the reload registers of the model stores follow);
// - a vertex is s8[4]: a struct's field stores hoist a zero that lengthens
//   r and w and swaps them; mode 2 needs that zero back as its own local;
// - s32 *out in mode <= 1 keeps point's address out of r5, so k gets it;
// - the wave loops' outer counter is its own variable, the inner one is the
//   i of every other loop (r8), and p is a separate variable per loop;
// - j * step with step a variable, here x and mode 4's x, are givs worth
//   reducing, which gives the native's stack slots and its copies; -4 * j is
//   not (loop.c: not worth while), and -j * 3 written in the loop is mode 3's;
// - h is i * w * 2, a giv: as a running sum gcse hoists w * 2;
// - (frame + i * 4) << 11 is reduced where (i << 13) - (frame << 11) is not;
//   mode 3's wave and scroll are running values, i = 0 set before them;
// - mode 4's angle is (frame + j * 2) << 10 and r and w plain j * 2 (a running
//   y lets gcse hoist the shift, two as a variable merges the givs);
// - frame * 630 written inline, not phase * 2, keeps its last shift inside
//   the bar loop, and the shade of the two rings reuses shade (r10);
// - Trig_Cos(a) * r, not r * Trig_Cos(a), and the model's stores in the order
//   texture, mesh, vertices, shade.
// Tried without gain: step in mode 3, two in mode 3 (frame 180), an address or
// pointer for the rows instead of [j][i], x in other places in mode 4, every
// order of the second model block's stores, of wave, scroll and i, and of
// q->variant, a local for the packed side, a local for the palette.

#include "battle_effect.h"
#include "battle_effect_work.h"
#include "effect_step.h"
#include "io_reg.h"
#include "iwram_call.h"
#include "heap_release.h"
#include "ram_buffer.h"
#include "types.h"

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

void BattleEffect_StartCanvasLayer(s32 bgControl);
void BattleEffect_EndCanvasLayer(void);
void BattleEffect_ProcessPendingGraphicsTransfer(
    void);
void BattleEffect_CountDownBattlerTints(void);
void BattleEffect_LoadBlitter(s32 slot, s32 modeAndFlags); // BattleEffect_BuildBlitter
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
void BattleEffect_SpawnObjects(s32 count, s32 resource, s32 mode); // SpawnObjects
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
// The ring's rows of vertices, two (x, y, z, pad) each. The buffers in this
// function are constants built from the fixed buffer, not their own symbols.
#define MAP_CELLS ((s8 (*)[2][4])Ram_MapCellBuffer)

extern char ResourceId_RuneSheet, ResourceId_BlueFlameSheet, ResourceId_Data0B4;
extern char ResourceId_FlashBurstSheet, ResourceId_Data0D2, ResourceId_Data0D3;
extern char ResourceId_Data134, ResourceId_JupiterDjinnSmallSheet;
extern char ResourceId_MarsDjinnSheet, ResourceId_WaterSpraySheet;

extern u8 gMapCellBuffer[];
extern const u16 gData_08197410[]; // gBattleEffect_MoteCells
extern const u8 gData_08199340[];
extern const u8 gData_08199210[];
extern s32 gCameraSceneParameters[];

u8 *Unit_Get(s32 unit);
void Render_ProjectVertices(const u8 *vertices, void *projected, s32 count);
void SceneTransform_ApplyPosition(void *position);

static inline s32 CopyWords(void *destination, const void *source, s32 size) {
  /* FAKEMATCH(wrapper): the call through this inline; as a direct call to
   * Iwram_CopyWords, 6 of 1748 instructions differ in tla-en. */
  return Iwram_CopyWords(destination, source, size);
}

// The water-column scene shared by Whorl (0), Gale (1), Lethe Albion (2),
// Poison Flow (3) and Raging Flood (4): a twisting funnel of water (or wind)
// rises around the target, with spray, bubbles and a burst.
void Func_08171bb0(struct BattleEffectArgument *effect, u32 mode) {
  struct BattleEffectWork *work;
  u8 *canvas;
  /* FAKEMATCH(frame): the blitter is declared as The Broken Seal's pair
   * though only the first is used; its second word is the unused word at
   * sp 76 of the native's 176-byte stack frame. As one DrawRectangle the frame
   * is 172 bytes and every stack offset below it moves. */
  DrawRectangle blit[2];
  s32 frame;
  s16 *camera;
  u8 *sprites;
  s32 frames;
  u8 *unit;
  s32 *target;
  struct RenderModel *model;
  struct ModelTexture texture;
  s32 columns;
  s32 shade;
  s32 i;
  s32 j;
  void *vertices = Ram_MapCellBuffer + 0x1000;
  u8 *sheet = Ram_MapCellBuffer + 0x4000;
  void *mesh = Ram_MapCellBuffer + 0x2000;
  s32 point[3];
  s32 screen[3];
  s32 pos[3];
  struct EffectPosition proj;
  struct EffectPosition apos;
  struct EffectPosition spos;

  work = (struct BattleEffectWork *)Ram_HeapSlots->battleFxWork[0];
  canvas = Ram_HeapSlots->battleFxWork[1];
  camera = Ram_HeapSlots->cameraWork;
  sprites = Ram_HeapSlots->unk64[0];
  unit = Unit_Get(effect->actors[0]);
  BattleEffect_StartCanvasLayer(0);
  if (mode <= 1) {
    frames = 38;
    Func_0815585c(effect, (effect->side << 4) | 163, screen, point);
    REG_BG2CNT = 0x785;
  } else if (mode == 2 || mode == 4) {
    s32 *caster = *GetBattleObjectSlotFar(effect->actor);

    if (mode == 2) {
      s32 row;
      s32 k;
      s32 size = 0x1000;

      Resource_LoadAndDecompress((s32)&ResourceId_RuneSheet, sheet + size, 1, 0);
      Iwram_ClearWords(sheet, size);
      {
        u8 *base;

      for (row = 0; row != 48; row++) {
        base = Ram_MapCellBuffer + 0x4000;
        for (k = 0; k != 24; k++) {
          (Ram_MapCellBuffer + 0x4208)[row * 64 + k] = base[(row * 2 + row) * 8 + 0x1000 + k];
          base[row * 64 + 0x237 - k] = base[(row * 2 + row) * 8 + 0x1000 + k];
        }
      }
      }
      CopyWords((void *)BG_PLTT, Resource_GetTableEntry((s32)&ResourceId_BlueFlameSheet), 128);
    } else {
      Resource_LoadAndDecompress((s32)&ResourceId_Data0B4, sheet, 0, 0);
    }
    pos[0] = caster[2];
    pos[1] = caster[3];
    pos[2] = caster[4];
    Resource_LoadAndDecompress((s32)&ResourceId_FlashBurstSheet, Ram_MapCellBuffer + 0x5000, 1, 0);
    frames = 52;
  } else {
    REG_BG2CNT = 0x785;
    frames = 52;
  }
  if (!effect->side) {
    BattleEffect_LoadBlitter(SLOT_BLITTER, 19);
  } else {
    BattleEffect_LoadBlitter(SLOT_BLITTER, 23);
  }
  blit[0] = (DrawRectangle)Ram_HeapSlots->unk64[1];
  REG_BLDALPHA = 0x1010;
  work->transferMode = 2;
  work->transferValue = 50;
  Task_Add(BattleEffect_ProcessPendingGraphicsTransfer, 3200);
  target = *GetBattleObjectSlotFar(effect->actors[0]);
  work->particles[63].x = target[2];
  work->particles[63].y = target[3];
  work->particles[63].z = target[4];
  work->particles[63].velocityX = target[6];
  work->particles[63].velocityY = target[7];
  if (mode == 2 || mode == 4) {
    Resource_LoadAndDecompress((s32)&ResourceId_Data0D2, work->sheet, 0, 0);
  } else {
    Resource_LoadAndDecompress((s32)&ResourceId_Data0D3, work->sheet, 0, 0);
  }
  Resource_LoadAndDecompress((s32)&ResourceId_Data134, sprites, 0, 0);
  Resource_LoadAndDecompress((s32)&ResourceId_JupiterDjinnSmallSheet, work->sheet + 0x2000, 1, 1);
  if (mode == 3) {
    CopyWords((void *)BG_PLTT, Resource_GetTableEntry((s32)&ResourceId_MarsDjinnSheet), 128);
  }
  if (mode == 4 || mode == 2) {
    CopyWords((void *)BG_PLTT, Resource_GetTableEntry((s32)&ResourceId_WaterSpraySheet), 128);
  }
  columns = (mode == 2 || mode == 4) ? 8 : 4;
  Render_BuildQuadStrip(mesh, columns, 32, 32);

  frame = 0;
  if (frames != 0) {
    do {
      if (frame == 0) {
        Sound_PlayFar(199);
      }
      if (frame == 32) {
        Sound_PlayFar(206);
      }
      if (frame == 0) {
        target[2] = work->particles[63].x;
        target[3] = work->particles[63].y;
        target[4] = work->particles[63].z;
        target[6] = work->particles[63].velocityX;
        target[7] = work->particles[63].velocityY;
        if (mode == 0) {
          for (i = 0; i != 63; i++) {
            struct EffectStep *q = &work->particles[i];

            if (effect->side == 1) {
              q->x = 0xc80000;
            } else {
              q->x = -0xc80000;
            }
            q->y = ((Random16() & 127) - 64) << 16;
            q->z = ((Random16() & 127) - 64) << 16;
            q->velocityX = q->x;
            q->velocityZ = q->z;
            q->velocityY = q->y;
          }
        } else if (mode == 1) {
          {
            struct EffectStep *q = work->particles;

            for (i = 0; i != 63; i++, q++) {
              q->x = point[0];
              q->y = ((Random16() & 31) << 16) + 0x400000;
              q->z = 0;
              q->velocityX = ((Random16() & 63) + 64) << 13;
              if (q->x > 0) {
                q->velocityX = -q->velocityX;
              }
              q->velocityY = (Random16() | -128) << 11;
              q->velocityZ = 0;
            }
          }
        }
      }
      Render_ResetTransformState();
      Transform_SetLookAt((s32 *)camera, (s32 *)camera + 3);
      if (mode <= 1) {
        s32 k = 4;

        pos[0] = point[0] + (Trig_Sin(frame << 11) << 4);
        if (frame > 39) {
          k = frame - 36;
        }
        pos[1] = point[1] + (k << 16) - Trig_Cos(frame * 3304) * k;
        if (frame > 27) {
          pos[1] = pos[1] + (frame << 18) - 0x700000;
        }
        {
          s32 *out = screen;

          pos[2] = point[2];
          EffectPosition_ProjectPoint(pos, (struct EffectPosition *)out);
          blit[0](canvas, work->sheet + 0x2000, out[0] / 2 - 10,
                out[1] - 20, 20, 40);
        }
      }
      if (frame > 7) {
        if (mode <= 1 && frame <= 47) {
          work->shakeFrames = 3;
        }
        if (mode == 0) {
          if (frame == 32) {
            if (effect->variant == 1) {
              target[3] = 0x7c0000;
            } else {
              work->shakeFrames = 8;
              Func_081180e8(134);
              target[6] = work->particles[63].velocityX;
              target[7] = work->particles[63].velocityY;
              BattleEffect_StartActorMotion(effect->actors[0], 1, 0x20000, 0x80000,
                                        0x40000, 150);
            }
          } else if (frame <= 19) {
            target[18] = mode;
            target[2] += (pos[0] - work->particles[63].x) / 128;
            target[3] += (pos[1] - work->particles[63].y) / 128;
            target[4] += (pos[2] - work->particles[63].z) / 128;
          } else if (frame <= 31 && (effect->variant == 1 || frame <= 23)) {
            target[2] += (pos[0] - work->particles[63].x) / 16;
            target[3] += (pos[1] - work->particles[63].y) / 16;
            target[4] += (pos[2] - work->particles[63].z) / 16;
            target[6] += (0x4000 - work->particles[63].velocityX) / 16;
            target[7] += (0x4000 - work->particles[63].velocityY) / 16;
          }
        } else if (mode == 1) {
          target[18] = 0;
          if (frame > 55) {
            target[3] = 0x900000;
          } else if (frame <= 31) {
            if (target[2] > 0) {
              target[2] += 0x8000;
            } else {
              target[2] -= 0x8000;
            }
          } else {
            if (target[2] > 0) {
              target[2] += 0x100000;
            } else {
              target[2] -= 0x100000;
            }
          }
        }
      }
      if (frame > 0 && mode <= 1) {
        {
          struct EffectStep *q = work->particles;

          for (i = 0; i != 16; i++, q++) {
            if (frame >= i && frame < i + 30) {
              s32 n = (i & 3) + 1;

              if (mode == 0) {
                q->x += (pos[0] - q->velocityX) / 32;
                q->y += (pos[1] - q->velocityY) / 32;
                q->z += (pos[2] - q->velocityZ) / 32;
              } else {
                BattleEffectKernels_IntegrateVector3(q, 62, 1024);
              }
              EffectPosition_ProjectPoint(q, &proj);
              blit[0](canvas, sprites + gData_08197410[n - 1], proj.x / 2 - n / 2,
                      proj.y - n, n, n * 2);
            }
          }
        }
      }

      if (mode == 2 || mode == 4) {
        s32 step = -4;

        for (j = 0; j != 33; j++) {
          s32 r = j;
          s32 x;

          if (r > 28) {
            r = 28;
          }
          x = j * step + 64;
          for (i = 0; i != 2; i++) {
            s32 zero = 0;

            MAP_CELLS[j][i][0] = x;
            MAP_CELLS[j][i][1] = ((Trig_Cos((j << 10) - (frame << 11)) * r) >> 16) + i * 48 - 24;
            MAP_CELLS[j][i][2] = zero;
          }
        }
      } else if (mode == 3) {
        for (j = 0; j != 33; j++) {
          s32 r = j * 4;
          s32 w = j * 2 + 4;

          if (r > 64) {
            r = 64;
          }
          for (i = 0; i != 2; i++) {
            MAP_CELLS[j][i][0] = -j * 3;
            MAP_CELLS[j][i][1] = ((Trig_Cos((j << 10) - (frame << 11)) * r) >> 16) + (i * w << 1) - w;
            MAP_CELLS[j][i][2] = 0;
          }
        }
      } else {
        s32 step = -4;

        for (j = 0; j != 33; j++) {
          s32 r = j * 2 + j;
          s32 w = j * 2;

          if (r > 96) {
            r = 96;
          }
          if (w > 40) {
            w = 40;
          }
          for (i = 0; i != 2; i++) {
            MAP_CELLS[j][i][0] = j * step;
            MAP_CELLS[j][i][1] = ((Trig_Cos((frame + j * 2) << 10) * r) >> 16) + (i * w << 1) - w;
            MAP_CELLS[j][i][2] = 0;
          }
        }
      }

      model = Render_AllocateModelList(1);
      if (mode == 4) {
        if (frame <= 45) {
          shade = frame * 4 - 64;
        } else {
          shade = frame * 8 - 376;
        }
        if (shade > -8) {
          shade = -8;
        }
      } else {
        if (frame <= 45) {
          shade = frame * 4 - 64;
        } else {
          shade = 348 - frame * 8;
        }
        if (shade > -20) {
          shade = -20;
        }
      }
      if (shade < -64) {
        shade = -64;
      }
      if (mode == 2 || mode == 4) {
        texture.logWidth = 5;
      } else {
        texture.logWidth = 7;
      }
      texture.logHeight = 5;
      texture.pixels = work->sheet;
      model->kind = 7;
      model->texture = &texture;
      model->mesh = mesh;
      model->vertices = vertices;
      model->shade = shade;
      if (mode == 2 || mode == 4) {
        EffectPosition_ProjectBattler(effect->actors[0], &apos);
        if (frame == 16) {
          Func_081180e8(144);
          BattleEffect_StartActorMotion(effect->actors[0], 1, 0, 0, 0x70000, 100);
          work->shakeFrames = 8;
          Func_0814cd48(effect->actors[0], 7, 5, 0, 8);
        }
        if (frame == 0) {
          {
            struct EffectStep *q = work->particles;

            for (i = 0; i != 63; i++, q++) {
              s32 r = (Random16() & 255) + 256;
              s32 a = (Random16() & 0x7fff) + 0x4000;

              q->velocityX = (Trig_Sin(a) * r) >> 7;
              q->velocityY = (Trig_Cos(a) * r) >> 7;
              q->x = (apos.x / 2) << 16;
              q->y = (apos.y - 8) << 16;
              q->variant = 0;
            }
          }
        }
        if (frame > 15) {
          {
            struct EffectStep *q = work->particles;

            for (i = 0; i != 8; i++, q++) {
              if (q->variant <= 23) {
                blit[0](canvas, (Ram_MapCellBuffer + 0x5000) + ((q->variant / 4) << 11),
                        HI(q->x) - 16, HI(q->y) - 32, 32, 64);
                BattleEffectKernels_IntegrateVector2(q, 60, -0x2000);
                q->variant++;
              }
            }
          }
        }
        model->scrollU = ~(frame << 3) & 127;
        model->scrollV = 0;
        for (i = 0; i != 3; i++) {
          s32 y = 64 - ((Trig_Sin(frame * 630) << 6) >> 16);

          Transform_Push();
          Func_08015160(0, ((y - i * 8) << 16) + 0x300000, 0);
          if (effect->side == 1) {
            SceneTransform_ApplyYaw(0x8000);
          }
          Func_080150e4((i << 10) + 256);
          SceneTransform_ApplyPitch((i << 13) - (frame << 11));
          Func_080151e4(0x21770, 0x10000, 0x10000);
          Render_TransformVertices(gMapCellBuffer, vertices, 66);
          Render_DrawModelList(model);
          Transform_Pop();
        }
        texture.logWidth = 6;
        texture.logHeight = 6;
        model->mesh = gData_08199340;
        model->vertices = vertices;
        model->shade = shade;
        model->scrollU = 0;
        model->scrollV = 0;
        texture.pixels = sheet;
        for (i = 0; i != 2; i++) {
          s32 t = frame - i * 4 - 16;

          if (t >= 0) {
            s32 s = (t * 3) << 12;

            shade = 0;
            if (t > 7) {
              shade = (8 - t) * 8;
            }
            model->shade = shade;
            if (shade > -64) {
              Render_ResetTransformState();
              Func_08015160((apos.x / 2 - 64) << 16, (apos.y - 64) << 16, 0);
              Func_080151e4(0x8000, 0x10000, 0x8000);
              Func_080150e4((i << 14) + 0x2000);
              SceneTransform_ApplyPitch(-0x1000);
              SceneTransform_ApplyYaw(frame << 12);
              Func_0801521c(s);
              Render_TransformVertices(gData_08199210, vertices, 4);
              Render_DrawModelList(model);
            }
          }
        }
      } else if (mode == 3) {
        s32 scroll, wave;

        EffectPosition_ProjectScaledBattler(effect->actor, &spos);
        if (frame == 8) {
          Sound_PlayFar(144);
        }
        if (frame == 16) {
          Func_081180e8(134);
        }
        for (i = 0; i != effect->count; i++) {
          if (frame == i * 4 + 16) {
            Sound_PlayFar(134);
            BattleEffect_StartActorMotion(effect->actors[i], 1, 0, 0, 0x70000, 100);
            Func_0814cd48(effect->actors[i], 7, 5, i, 8);
            work->shakeFrames = 8;
          }
        }
        model->scrollV = 0;
        i = 0;
        wave = frame << 11;
        scroll = frame << 3;
        for (; i != 1; i++) {
          model->scrollU = ~scroll & 127;
          Render_ResetTransformState();
          Func_08015160((spos.x / 2 - 64) << 16, (spos.y << 16) - 0x340000, 0);
          if (effect->side == 1) {
            SceneTransform_ApplyYaw(0x8000);
          }
          SceneTransform_ApplyPitch(i << 15);
          Func_080150e4((Trig_Sin(wave) << 11) >> 16);
          Func_080151e4(0x14e20, 0x8000, 0x10000);
          Render_TransformVertices(gMapCellBuffer, vertices, 66);
          Render_DrawModelList(model);
          wave += 0x4000;
          scroll += 48;
        }
      } else {
        if (frame <= 29) {
          shade = frame * 4 - 64;
        } else {
          shade = 220 - frame * 8;
        }
        if (shade > -20) {
          shade = -20;
        }
        if (shade > -64) {
          model->shade = shade;
          Func_080151e4(0x8000, 0x10000, 0x10000);
          gCameraSceneParameters[3] = 60;
          if (mode == 0) {
            model->scrollU = (frame << 3) & 127;
          } else {
            model->scrollU = ~(frame << 3) & 127;
          }
          model->scrollV = 0;
          SceneTransform_ApplyPosition(pos);
          Func_08015160(0, 0x100000, 0);
          for (i = 0; i != 3; i++) {
            Transform_Push();
            if (effect->side == 1) {
              SceneTransform_ApplyYaw(0x8000);
            }
            Func_080150e4((i << 10) + 3096);
            if (mode != 1) {
              SceneTransform_ApplyPitch((frame + i * 4) << 11);
            } else {
              SceneTransform_ApplyPitch((i << 13) - (frame << 11));
            }
            Func_080151e4(0x40000, 0x10000, 0x10000);
            Render_ProjectVertices(gMapCellBuffer, vertices, 66);
            Render_DrawModelList(model);
            Transform_Pop();
          }
        }
        gCameraSceneParameters[3] = 120;
      }
      Heap_Release(model);
      if (mode == 2 || mode == 4) {
        Func_08158ce0(8, 8);
      } else {
        Func_08158ce0(4, 4);
      }
      BattleEffect_CountDownBattlerTints();
      work->transferPending = 1;
      wait(1);
      frame++;
    } while (frame != frames);
  }

  if (unit[0x129] && mode == 0 && effect->variant == 1) {
    target[2] = work->particles[63].x;
    target[3] = work->particles[63].y;
    target[4] = work->particles[63].z;
    target[6] = work->particles[63].velocityX;
    target[7] = work->particles[63].velocityY;
  }
  HeapSlot_Release(SLOT_BLITTER);
  Task_Remove(BattleEffect_ProcessPendingGraphicsTransfer);
  BattleEffect_EndCanvasLayer();
}
