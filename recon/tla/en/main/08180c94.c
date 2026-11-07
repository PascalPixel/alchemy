// Differ 4 of 575 (tla-en, 2026-11-07).
// 2026-11-07: from 14. The model's vertices as s32 * (a void * store aliased
// the texture word and held its store back); one sparks pointer shared by the
// burst and the draw loop (the pool register r0); delay = 0 apart from i = 0;
// ring indexed from i. Left: two argument-move ties in sched2, `add r2, sp,
// #44` before the place and `lsls r2` (0x10000) before size; tried argument
// spellings, Q16_ONE, a struct source pointer, size set in the call.
// 2026-11-05: differ 14. Wrappers on the palette copy and the fill.
// Draft, differ 34 of 575, skeleton gap 8 (tla-en, 2026-11-01)
// Of BattleEffect_RunDaedalusAlt, decompiled from the listing. Same 51 calls
// and control flow; every difference left is scheduling or one register.
// What closed it, from 172: the disc's and rings' constants set once in the
// frame-0 arm (discX, discY, ringX, ringOffset). Not available on every path,
// so cprop leaves them, yet each is REG_EQUIV and reloads at each use: this
// is the unfolded x >> 16, the separate 0x420000 and the pooled -0x400000.
// The r6 of the disc's x is `i = discX`, the counters' own variable: a fresh
// local is replaced by discX in CSE and rematerialized (differ 38 with the
// reload rotation right, but no r6). The frame loop is label and goto: it
// keeps gMapCellBuffer's load ahead of the shifts (loop.c's single-use
// replacement otherwise moves it into the r0 copy), so r0 is busy and discY
// reloads into r3; that rotation sets every later reload.
// Others: request through a pointer (r4); place[3] = 0 before unk12 and unk16
// before 0x8800, or CSE takes the zero from 0x8800's low byte; speed = 31 then
// &= (r5); j, mask, then spark in two steps, the order the reload rotation
// gives; burst += 8 before frame++; the return types of LoadBlitter, Task_Add.
// Left: the native's pool register in the burst preheader is r0 (here r7: the
// two-step spark; &gData[burst] gives r2, a separate local reaches 166). Near
// lines of sched2 order where the native looks as if loop.c ran: the frame-0
// copy's `lsls r0`, shakeFrames = 8 (register pair swapped) and its fill's
// pools, `add r2, sp, #44`, the 0x10000 argument, the delay's movs r7, and the
// model's stores. These suggest the native is a real loop whose replacement
// skipped only gMapCellBuffer's load; why is the open question. Tried: for,
// do-while, for (;;) and while (each 66; the for loop now 64); an inline or
// one-pass around the disc call; a block-local buffer pointer; x as a local;
// model store orders; burst as (frame - 80) * 8 at the loop (184 to 298).
// `i = discX` reuses a local for two values: S2's `temporary`, to tag if it
// stays at adoption. No new device is in, and the old `frame` note on draw
// stands.

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
// The sparks' lives: gData_02014000's EffectSteps from their variant on.
extern s32 Data_02014018[];
extern const u8 gData_08199210[]; // its four corners
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

static inline s32 CopyWords(void *destination, const void *source, s32 size) {
  /* FAKEMATCH(wrapper): the call through this inline; as a direct call to
   * Iwram_CopyWords, 2 of 575 instructions differ in tla-en. */
  return Iwram_CopyWords(destination, source, size);
}

static inline s32 FillWords(void *destination, s32 size, u32 value) {
  /* FAKEMATCH(wrapper): the call through this inline; as a direct call to
   * Iwram_FillWords, 30 of 575 instructions differ in tla-en. */
  return Iwram_FillWords(destination, size, value);
}

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
  /* FAKEMATCH(frame): the blitter as a pair of which only the first is used;
   * as one two-word DImode pseudo its spill takes sp+20 and the unused sp+24
   * the native stack frame has. As a single DrawRectangle the stack frame is 64
   * bytes and every slot above it moves (differ 325 against 299). */
  DrawRectangle draw[2];
  struct ModelTexture texture;
  s32 request[4];
  s32 *place;
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
next_frame:
  if (frame == 0) {
    discX = 0x400000;
    ringOffset = -0x400000;
    ringX = 0x420000;
    discY = 0x500000;
    work->transferMode = 2;
    work->transferValue = 50;
    CopyWords((void *)BG_PLTT,
              Resource_GetTableEntry((s32)&ResourceId_OrangePaletteB), 128);
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
      s32 *life = Data_02014018;

      for (i = 0; i != DAEDALUS_SPARKS; i++) {
        *life = -1;
        life += 7;
      }
      work->shakeFrames = 8;
      FillWords(canvas, 0x3c00, 0x3f3f3f3f);
    }
    sparks = gData_02014000;
    if ((u32)(frame - 80) <= 7) {
      centerX = ((Random16() & 63) + 28) << 16;
      centerY = ((Random16() & 31) + 80) << 16;

      j = 0;
      mask = 15;
      spark = sparks;
      spark += burst;
      for (; j != 8; j++, spark++) {
        s32 speed;
        s32 angle;
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
    place = request;
    object->unk17 = 32;
    object->unk16 = 0;
    place[3] = 0;
    object->unk12 = 0x8800;
    place[1] = 0xff0000;
    if (frame <= 63) {
      place[0] = (152 - frame / 4) << 16;
      place[2] = (frame << 17) - 0x400000;
    } else if ((u32)(frame - 64) <= 63) {
      s32 t = frame - 64;

      place[0] = (136 - t / 16) << 16;
      place[2] = ((t / 2) << 16) + 0x400000;
    } else {
      place[0] = 136 << 16;
      place[2] = 0x400000;
    }
    Render_ApplyProjectedPlacementFar(work->objects[0], place, &source.x, 0);
  }

  // Frames 56 to 91: the disc and the rings.
  if ((u32)(frame - 56) <= 35) {
    s32 size;
    s32 grow = frame * 2 - 104;
    struct RenderModel *model;
    void *vertices;
    struct EffectStep *ring;

    // The disc's place shares the counters' variable, as the listing's r6 does.
    i = discX;
    size = 128;
    if (grow <= 64) {
      size = grow * 2;
      Func_0815b434(gMapCellBuffer, size, 0x10000);
    }
    Func_0818caa8(gMapCellBuffer, i >> 16, discY >> 16, size);
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
      Resource_LoadAndDecompress((s32)&ResourceId_Data0C9, work->sheet + 0x4000,
                                 0, 0);
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
  if (frame != DAEDALUS_FRAMES) {
    goto next_frame;
  }
  Task_Remove(BattleEffect_ProcessPendingGraphicsTransfer);
  HeapSlot_Release(104);
  ResourceObject_ReleaseFar(work->objects[0]);
  BattleEffect_EndCanvasLayer();
}
