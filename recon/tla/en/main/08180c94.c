// Matches: differ 0 (tla-en, 2026-11-08)
// tla-ja (at 08180c98), tla-de, tla-es, tla-fr and tla-it also differ 0. From
// differ 2.
// Of BattleEffect_RunDaedalusAlt, decompiled from the listing. The frame loop
// is a real do-while, as the listing's code shows loop.c ran: its single-use
// replacement moves &source's `add r2, sp, #44` down to the argument moves,
// after request's `adds r1, r4, #0`, the tie every earlier round left with the
// label-and-goto loop. The same replacement would fold the disc call's
// gMapCellBuffer load into its r0 copy (r0 then free for discY's reload, which
// turns the reload rotation: 56 of 575 with gMapCellBuffer passed directly);
// `cells`, set before each of the two calls, is set twice in the loop, so
// loop.c leaves it (set once before the `if`: 69; for one call only: 56).
// With the real loop these goto-era forms went, each measured at differ 0:
// the inline wrappers on the palette copy and the fill (both tagged
// FAKEMATCH(wrapper) before), the disc's scale local (0x10000 is passed),
// the place pointer (request is used directly) and the two-step spark.
// Still needed, measured against this match: `i = discX` (discX passed
// directly: 10), mask = 15 (the literal: 42), speed = 31 then &= (as
// Random16() & 31: 23). `cells` and `i = discX` are tagged temporaries. The
// frame-0 arm's constants (discX, discY, ringX, ringOffset) are REG_EQUIV and
// reload at each use, as the listing's do. For adoption: source is
// Render_ApplyProjectedPlacement's scale ({2.0, 2.0}, struct SpriteScale in
// graphics/render_placement.h) and request its struct SpritePlacement (x,
// height, y, groundHeight); not renamed here.

#include "types.h"
#include "battle_effect.h"
#include "battle_effect_work.h"
#include "effect_step.h"
#include "io_reg.h"
#include "iwram_call.h"
#include "heap_release.h"
#include "ram_buffer.h"

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
  s32 *vertices;
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
void ResourceObject_ReleaseFar(void *object);
void Func_08118088(s32 actor, s32 motion); // BattleMotion_ApplyVariantMotionFar
void Func_081180e8(s32 phase);             // BattlePlayback_StartFar
void Func_08158ce0(s32 randomMask, u32 shakeRange); // Camera_ApplyShake
void BattleEffectKernels_IntegrateVector2(struct EffectStep *step, s32 gravity,
                                          s32 drag);
void **GetBattleObjectSlotFar(s32 objectId);
void *Heap_AllocEwram(s32 size);
void Resource_LoadAndDecompress(s32 resource, void *destination, s32 a, s32 b);
void *Resource_GetTableEntry(s32 resource);
s32 Task_Add(void (*func)(void), s32 order);
void Task_Remove(void (*func)(void));
void HeapSlot_Release(s32 slot);
void Sound_PlayFar(s32 cue);
void wait(s32 frames);
u32 Random16(void);
s32 Trig_Sin(s32 angle);
s32 Trig_Cos(s32 angle);

extern char ResourceId_FlashBurstSheet;
extern char ResourceId_OrangePaletteB;
extern char ResourceId_Data0C9;
extern u8 gMapCellBuffer[];
extern const u16 gData_08197410[]; // gBattleEffect_MoteCells
extern const u8 Data_081995ec[];   // the seven strike frames
struct PlacementSource {
  s32 x;
  s32 y;
};
extern const struct PlacementSource
    Data_08196ee8;                         // the summon's placement source
extern const u8 Data_08199220[];           // the rings' mesh
extern const u8 gData_081991b0[];          // their corners
extern struct EffectStep gData_02014000[]; // the sparks
extern const u8 gData_08199210[];          // its four corners
extern u32 gCameraSceneParameters[];

#define BLITTER ((DrawRectangle)Ram_HeapSlots->unk64[1])
enum { BLIT_ADDITIVE, BLIT_MAX, BLIT_TRANSPARENT };
#define BLIT_MODE(mode, flags) ((mode) << 4 | (flags))
#define BLIT_CLIP (COMPOSITE_CLIP_X | COMPOSITE_CLIP_Y)
#define HI(v) (((s16 *)&(v))[1])
#define DAEDALUS_FRAMES 108
#define DAEDALUS_SPARKS 64

// The summoned object's display fields that the scene sets.
struct SummonObject {
  u8 unk00[18];
  u16 unk12;
  u8 unk14[2];
  u8 unk16;
  u8 unk17;
};

// Abilities: Daedalus (the second scene).
//
// The summoned object glides in from the right and down over 92 frames
// while, from frame 56, a shaded disc swells at the centre and six rings of
// light spin out from it. At the seven frames of Data_081995ec every target
// is knocked back and flashes; from frame 80 eight bursts of eight sparks,
// one a frame, scatter over the canvas.
void BattleEffect_RunDaedalusAlt(struct BattleEffectArgument *effect) {
  struct BattleEffectWork *work;
  void *canvas;
  // Unused: the second blitter gives the stack frame its sp+24 (as one, every
  // slot above it moves: 36 of 575 differ in tla-en).
  DrawRectangle draw[2];
  struct ModelTexture texture;
  s32 request[4];
  struct PlacementSource source;
  s32 frame;
  s32 i;
  s32 j;
  s32 centerX;
  s32 centerY;
  s32 burst;
  struct EffectStep *spark;
  s32 mask;
  s32 discX;
  s32 ringOffset;
  s32 ringX;
  s32 discY;

  canvas = Ram_HeapSlots->battleFxWork[1];
  work = (struct BattleEffectWork *)Ram_HeapSlots->battleFxWork[0];
  Func_0813ba50();
  REG_WIN0V = 0x1080;
  REG_WININ = 0x2737;
  BattleEffect_StartCanvasLayer(0);
  REG_BLDALPHA = 0x1010;
  BattleEffect_SpawnObjects(1, 0x296, 3);
  BattleEffect_SelectLivingTargets(work->effect);
  BattleEffect_LoadBlitter(104, BLIT_MODE(BLIT_ADDITIVE, BLIT_CLIP));
  draw[0] = BLITTER;
  Resource_LoadAndDecompress((s32)&ResourceId_FlashBurstSheet, work->sheet, 1,
                             0);
  work->transferMode = 2;
  work->transferValue = 50;
  Task_Add(BattleEffect_ProcessPendingGraphicsTransfer, 0xc80);
  work->fadeFrames = 24;
  work->fadeStep = 0;

  frame = 0;
  burst = -80 * 8;
  do {
    if (frame == 0) {
      discX = 0x400000;
      ringOffset = -0x400000;
      ringX = 0x420000;
      discY = 0x500000;
      work->transferMode = 2;
      work->transferValue = 50;
      Iwram_CopyWords((void *)BG_PLTT,
                      Resource_GetTableEntry((s32)&ResourceId_OrangePaletteB),
                      128);
    }
    if (frame == 56) {
      Sound_PlayFar(140);
    }
    for (i = 0; i != 7; i++) {
      if (frame == Data_081995ec[i]) {
        for (j = 0; j != effect->count; j++) {
          BattleEffect_StartActorMotion(effect->actors[j], 1, 0x20000, 0x80000,
                                        0x20000, 120);
          Func_0814cd48(effect->actors[j], 7, 5, j, 7);
        }
        work->shakeFrames = 4;
        Sound_PlayFar(145);
      }
    }

    // From frame 80: eight bursts of sparks, one a frame.
    if (frame > 79) {
      // One pointer to the sparks for the burst and the draw loop below.
      struct EffectStep *sparks;

      if (frame == 80) {
        // UB: the lives are cleared through one s32 pointer stepped a whole
        // step at a time, past the step's scalar members.
        s32 *life = &gData_02014000[0].variant;

        for (i = 0; i != DAEDALUS_SPARKS; i++) {
          *life = -1;
          life += sizeof(struct EffectStep) / sizeof(s32);
        }
        work->shakeFrames = 8;
        Iwram_FillWords(canvas, 0x3c00, 0x3f3f3f3f);
      }
      sparks = gData_02014000;
      if ((u32)(frame - 80) <= 7) {
        centerX = ((Random16() & 63) + 28) << 16;
        centerY = ((Random16() & 31) + 80) << 16;

        j = 0;
        mask = 15;
        spark = sparks + burst;
        for (; j != 8; j++, spark++) {
          s32 speed;
          s32 angle;
          // The mask set first and the random number masked in (as
          // Random16() & 31, 23 of 575 instructions differ in tla-en).
          speed = 31;
          speed &= Random16();
          angle = Random16();

          spark->x = centerX + (((Random16() & mask) - 8) << 16);
          spark->y = centerY + (((Random16() & mask) - 8) << 16);
          speed += 32;
          spark->velocityX = (Trig_Sin(angle) * speed) >> 3;
          spark->velocityY = (Trig_Cos(angle) * speed) >> 3;
          spark->variant = 0;
        }
      }
      {
        struct EffectStep *mote;

        for (i = 0, mote = sparks; i != DAEDALUS_SPARKS; i++, mote++) {
          if ((u32)mote->variant <= 17) {
            draw[0](canvas, work->sheet + ((mote->variant / 3) << 11),
                    HI(mote->x) - 16, HI(mote->y) - 48, 32, 64);
            BattleEffectKernels_IntegrateVector2(mote, 62, -0x800);
            mote->variant++;
          }
        }
      }
    }

    // The summoned object glides in over 92 frames.
    if (frame <= 91) {
      struct SummonObject *object;

      source = Data_08196ee8;
      object = work->objects[0];
      object->unk17 = 32;
      object->unk16 = 0;
      request[3] = 0;
      object->unk12 = 0x8800;
      request[1] = 0xff0000;
      if (frame <= 63) {
        request[0] = (152 - frame / 4) << 16;
        request[2] = (frame << 17) - 0x400000;
      } else if ((u32)(frame - 64) <= 63) {
        s32 t = frame - 64;

        request[0] = (136 - t / 16) << 16;
        request[2] = ((t / 2) << 16) + 0x400000;
      } else {
        request[0] = 136 << 16;
        request[2] = 0x400000;
      }
      Render_ApplyProjectedPlacementFar(work->objects[0], request, &source.x,
                                        0);
    }

    // Frames 56 to 91: the disc and the rings.
    if ((u32)(frame - 56) <= 35) {
      s32 size;
      s32 grow = frame * 2 - 104;
      struct RenderModel *model;
      void *vertices;
      struct EffectStep *ring;
      u8 *cells;

      /* FAKEMATCH(temporary): the disc's x copied into the loop counter i;
       * as discX passed directly, 10 of 575 instructions differ in tla-en. */
      i = discX;
      size = 128;
      /* FAKEMATCH(temporary): the cell-buffer local set before each call; as
       * gMapCellBuffer passed directly, 56 of 575 instructions differ in
       * tla-en. */
      if (grow <= 64) {
        size = grow * 2;
        cells = gMapCellBuffer;
        Func_0815b434(cells, size, 0x10000);
      }
      cells = gMapCellBuffer;
      Func_0818caa8(cells, i >> 16, discY >> 16, size);
      if (frame == 56) {
        s32 delay;

        i = 0;
        delay = 0;
        for (ring = work->particles; i != 16; i++, ring++) {
          ring->x = Random16() & 0xffff;
          ring->y = (Random16() & 255) + 128;
          if (Random16() & 1) {
            ring->y = -ring->y;
          }
          ring->variant = delay;
          delay -= 4;
        }
        Resource_LoadAndDecompress((s32)&ResourceId_Data0C9,
                                   work->sheet + 0x4000, 0, 0);
      }
      vertices = Heap_AllocEwram(32);
      model = Render_AllocateModelList(1);
      model->shade = 0;
      texture.logWidth = 5;
      texture.logHeight = 2;
      model->vertices = vertices;
      texture.pixels = work->sheet + 0x4000;
      model->kind = 7;
      model->texture = &texture;
      model->mesh = Data_08199220;

      for (i = 0; i != 6; i++) {
        ring = &work->particles[i];
        ring->variant += 2;
        ring->x += ring->y * 2;
        if ((u32)ring->variant <= 47) {
          s32 k;

          Render_ResetTransformState();
          k = i & 3;
          Func_08015160((k << 17) + ringX + ringOffset,
                        discY - ((k + 2) << 16) + ringOffset, 0);
          Func_080151e4(0x8000, 0x10000, 0x10000);
          Func_080150e4(ring->x);
          Func_080151e4(Trig_Sin(ring->variant * 682) + 0x10000, 0x20000,
                        0x10000);
          Render_TransformVertices(gData_081991b0, vertices, 4);
          Render_DrawModelList(model);
        }
      }
      Heap_Release(model);
      Heap_Release(vertices);
    }
    Func_08158ce0(8, 8);
    BattleEffect_CountDownBattlerTints();
    work->transferPending = 1;
    wait(1);
    burst += 8;
    frame++;
  } while (frame != DAEDALUS_FRAMES);
  Task_Remove(BattleEffect_ProcessPendingGraphicsTransfer);
  HeapSlot_Release(104);
  ResourceObject_ReleaseFar(work->objects[0]);
  BattleEffect_EndCanvasLayer();
}
