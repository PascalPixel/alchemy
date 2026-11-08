// Differ 38 of 1361 (tla-en, 2026-11-08, -mtune=gba). Rechecked: f and twice
// are still needed (167 and 129 without) and are now tagged; the once-only
// block still gains 2. No better: the req word through pair[0], texture2's
// fields before its pixels (46), the first model's vertices after its texture
// sizes (40), the closing transition store through zero (48), meshA's sine
// folded into r, d as (frame - 72) * 2.
// 2026-11-07: a once-only block around the first texture's pixels and mesh
// (permuter) took 40 to 38; frame * 2 - 144 spellings, texture2 and model
// store orders, and once-only blocks elsewhere did not help.
// Scheduling only: sched2 order in seven
// places (s * 48 in the meshA loop, m's increment, frame * 2 - 144 in r3, the
// first req word through r2, the first model's stores, texture2's fields and
// the closing transition store). texture2's two fields before its pixels, as
// the listing stores them, fix that row but add two insns at addressof: gcse's
// table grows from 551 to 553 buckets, sfp-120 hashes past sfp-96 and the
// &texture2/&pos slot swap returns (differ 48); three 0 defaults reach 555 and
// still 48. Measured 2026-11-05: differ 40; orders of the plain assignments
// searched. 2026-11-06: the req word store moved after place[3] (56),
// after place[1] (149) or spelled pair[0] (40, no change).
// Draft, differ 62 of 1361, skeleton gap 20/20 (tla-en, 2026-11-01)
// BattleEffect_RunMegiddo. Frame (220), control flow, calls, pools and nearly
// every register agree. What is left is two causes.
// 1. Eighteen lines are one swap: the listing keeps &texture2 in [sp, #24] and
// &pos in [sp, #20], the draft the reverse. Both are PRE's reaching registers,
// numbered in gcse's hash table order, and that table's size follows the
// function's insn count, so this draft is a few insns short before gcse. A dead
// if (zero) block of four stores at the end of the function flips it (differ
// 44, swap gone), but it is a device short of 0, so it came out. Natural
// rewrites tried without a flip: seven reshaped temporaries and zero spellings.
// 2. The other 44 are scheduling pairs, mostly a load the draft issues earlier
// than the listing (effect at the top and before the caster's slot, frame
// before k's loop, effect before list2), the known load-after-store gap, which
// is a model question for K2 and out of scope here. Also: frame * 2 - 144 takes
// its temporary in r3, the listing r2; the m loop's r2 restore lands after the
// strb; the first req word is stored through r2, not sp; the 0xffb40000 load.
// Forced locals, tagged 2026-11-08 (FAKEMATCH temporary): f (a copy
// of frame, so thread_jumps cannot join the two frame > 1 tests before cse2 and
// the label ends cse2's path: without it differ 195) and twice (without it 153).
// tilt and i = 0 before the loop put frame * 3 after i's reload. Plain finds:
// side's if/else order sets x before the test; v[2] and place use zero (spilled,
// reloaded) but the first model's shade and scrollV are literal; strip and
// sheet2 are one-constant locals the listing reloads; tint overlaps the 3 so -24
// lands in r2. RESET_AND_SCALE_HALF's do-while ends cse1's block, so the 5s are
// not shared with % 5 (without it 126).
// blit is two words: slot 76 is its unused half. req is a byte array so its slot
// comes before the first texture's: both must be BLKmode, as a DImode texture's
// address hoists out of the loop and a DImode req is placed after it.
// Permute, about five minutes on the dead-block state: inlining y's value, kept.
// The ResourceId_Data names are placeholders for ids with no symbol yet.

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
  void *vertices;
  struct ModelTexture *texture;
  s32 shade;
  u8 scrollU;
  u8 scrollV;
};

void BattleEffect_StartCanvasLayer(s32 bgControl);
void BattleEffect_EndCanvasLayer(void);
void BattleEffect_ProcessPendingGraphicsTransfer(void);
void BattleEffect_CountDownBattlerTints(void);
s32 BattleEffect_LoadBlitter(s32 slot, s32 modeAndFlags); // BattleEffect_BuildBlitter
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
void EffectPosition_ProjectBattler(s32 id,
                                                 struct EffectPosition *out);
void BattleEffect_SpawnObjects(s32 count, s32 resource, s32 mode);   // SpawnObjects
void BattleEffect_SelectLivingTargets(struct BattleEffectArgument *effect); // SelectLivingTargets
void Func_080150e4(s32 angle); // SceneTransform_ApplyRoll
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
s32 Task_Add(void (*func)(void), s32 order);
s32 Task_Remove(void (*func)(void));
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
#include "input.h"
#include "io_write_queue.h"

extern u8 Data_02011000[];
extern u8 Data_02012000[];
extern u8 gMapCellBuffer[];
extern u8 gData_02014000[];
extern const u16 gData_08197410[]; // gBattleEffect_MoteCells
extern const u8 gData_08199364[];
extern const u8 Data_081991e0[];
extern const u8 gData_081991f0[];
extern const u8 Data_081992b0[];
extern const u16 Data_08199ee0[]; // the five rings' start frames
extern const s16 Data_08199ecc[];
extern const s16 Data_08199ed6[];
extern const s16 Data_08199eea[];
extern const s16 Data_08199efa[];
extern const s16 Data_08199f0a[];
extern const s8 Data_08199f1a[];
extern const s8 Data_08199f22[];
extern s32 gCameraSceneParameters[];
extern char ResourceId_Data072;
extern char ResourceId_Data0C2;
extern char ResourceId_Data0C9;
extern char ResourceId_Data0CD;
extern char ResourceId_Data0E9;
extern char ResourceId_Data103;
extern char ResourceId_Data134;
extern char ResourceId_Data148;

void BattleMotion_ApproachTargetFar(s32 actor, s32 target, s32 frames,
                                    s32 speed);
void Actor_SetAnimationSpeedFar(void *object, s32 value);
void BattleActor_SpawnObjectsForListFar(s16 *targets, s32 mode);
void BattleActor_CommitPlacementFar(void);
void BattleEffect_BuildRepeatingTilemap(void);
void Func_08164abc(s32 red, s32 green, s32 blue);
void Func_08164a4c(s32 blue, s32 green, s32 red);
void Func_0815f0a0(s32 palette);
void Graphics_LoadGreyRamp(void);
s32 Render_ProjectPoint(const void *point, s32 *screen);
void ResourceObject_ReleaseFar(void *object);
void BattleBackground_LoadFar(s32 layer, s32 background, s32 palette);

#define RESET_AND_SCALE_HALF()                                                 \
  do {                                                                         \
    s32 half = 0x8000;                                                         \
                                                                               \
    Render_ResetTransformState();                                              \
    Func_080151e4(half, 0x10000, half);                                        \
  } while (0)

// Megiddo: the caster rises in a column of light, a winged figure gathers
// above and dives in a storm of feathers and rings onto the target.
void BattleEffect_RunMegiddo(struct BattleEffectArgument *effect) {
  struct BattleEffectWork *work;
  u8 *canvas;
  s32 frame;
  s32 f;
  s32 k;
  s32 zero;
  u8 *ring;
  DrawRectangle blit[2];
  u8 *sprites;
  s32 *transition;
  s16 *camera;
  s16 savedYaw;
  u8 *meshA;
  u8 *meshB;
  s32 casterValue;
  s32 *caster;
  s32 x;
  s32 z;
  s32 scale;
  u8 *transformed;
  u8 *strip;
  u8 *sheet2;
  s32 i;
  s32 j;
  s32 m;
  struct RenderModel *model;
  void *verts;
  struct EffectStep *p;
  struct EffectPosition pos;
  s16 list[14];
  s16 list2[14];
  s32 place[4];
  s32 screen[3];
  u8 req[8];
  struct ModelTexture texture2;

  work = (struct BattleEffectWork *)Ram_HeapSlots->battleFxWork[0];
  canvas = Ram_HeapSlots->battleFxWork[1];
  sprites = Ram_HeapSlots->unk64[0];
  transition = Ram_HeapSlots->transitionWork;
  camera = Ram_HeapSlots->cameraWork;
  savedYaw = 0;
  zero = 0;
  ring = gMapCellBuffer;
  transformed = Data_02011000;
  strip = Data_02012000;
  sheet2 = gData_02014000;
  BattleMotion_ApproachTargetFar(effect->actor, effect->actor, 150, 0x50000);
  BattleEffect_StartCanvasLayer(0);
  REG_BLDALPHA = 0x1010;
  Resource_LoadAndDecompress((s32)&ResourceId_Data134, sprites, 0, 0);
  work->transferMode = 2;
  work->transferValue = 50;
  Task_Add(BattleEffect_ProcessPendingGraphicsTransfer, 3200);
  BattleEffect_LoadBlitter(SLOT_BLITTER, 19);
  blit[0] = (DrawRectangle)Ram_HeapSlots->unk64[1];
  meshA = work->sheet + 0x5f40;
  meshB = work->sheet + 0x6340;
  caster = *GetBattleObjectSlotFar(effect->actor);
  x = 0;
  z = 0;
  scale = 0x10000;
  Render_BuildQuadStrip(strip, 8, 16, 32);
  Render_BuildQuadStrip(meshB, 2, 32, 15);
  for (j = 0; j != 16; j++) {
    s32 c = -Trig_Cos(j * 1092) * 3;
    s32 s = Trig_Sin(j * 1092);
    s32 r = (s * 48) >> 16;

    for (m = 0; m != 2; m++) {
      u8 *v = meshA + j * 8 + m * 4;

      v[0] = (Trig_Sin(m << 13) * r) >> 16;
      v[1] = ((c * 16) >> 16) + 24;
      v[2] = (Trig_Cos(m << 13) * r) >> 16;
    }
  }
  casterValue = caster[18];
  Actor_SetAnimationSpeedFar(caster, 0);
  caster[9] = 0;
  caster[10] = 0;
  caster[11] = 0;
  caster[13] = 0;
  caster[18] = 0;

  for (frame = 0; frame != 188; frame++) {
    if (frame == 4) {
      Sound_PlayFar(212);
    }
    if (frame == 28) {
      Sound_PlayFar(140);
    }
    if (frame == 84) {
      Sound_PlayFar(104);
    }
    if (frame == 143) {
      Sound_PlayFar(191);
    }
    if (frame == 171) {
      Sound_PlayFar(145);
    }
    if ((gInput.repeat & 3) && frame > 3 && frame <= 142) {
      frame = 143;
    }
    if (frame == 0) {
      Iwram_FillWords(canvas, 0x3c00, 0x3f3f3f3f);
      Resource_LoadAndDecompress((s32)&ResourceId_Data0C2, gData_02014000, 0, 0);
      Resource_LoadAndDecompress((s32)&ResourceId_Data0E9, work->sheet, 0, 0);
      Resource_LoadAndDecompress((s32)&ResourceId_Data0C9, work->sheet + 0x800, 0, 0);
      Resource_LoadAndDecompress((s32)&ResourceId_Data103, work->sheet + 0x1000, 1, 1);
      Resource_LoadAndDecompress((s32)&ResourceId_Data0CD, work->sheet + 0x2400, 1, 0);
      Graphics_LoadGreyRamp();
      REG_BLDALPHA = 0x1010;
      if (!effect->side) {
        x = 0xa00000;
      } else {
        x = 0x500000;
      }
      z = -0x600000;
      {
        volatile u16 *ime;
        struct IoWriteQueue *q;
        u32 saved;
        s32 count;

        q = &gIoWriteQueue;
        ime = &REG_IME;
        IME_SAVE(ime, saved);
        IME_DISABLE(ime);
        count = *&q->count;
        if (count <= 31) {
          u32 *entry = q->entries[count];

          *&q->count = count + 1;
          *entry++ = 0x784;
          *entry++ = (u32)&REG_BG2CNT;
          *entry = IO_WRITE_16;
        }
        IME_RESTORE(ime, saved);
      }
    }
    if (frame == 1) {
      transition[4] = 1;
      list[0] = effect->actor;
      list[1] = 255;
      BattleActor_SpawnObjectsForListFar(list, 0);
      BattleBackground_LoadFar(1, (s32)&ResourceId_Data072, 0);
      BattleEffect_BuildRepeatingTilemap();
      Func_08164abc(8, 2, 2);
      BattleEffect_SpawnObjects(1, 0x299, 2);
    }
    EffectPosition_ProjectScaledBattler(effect->actor, &pos);
    if (frame == 3) {
      savedYaw = camera[27];
    }
    if ((u32)(frame - 16) <= 61) {
      if (!effect->side) {
        camera[27] -= 256;
      } else {
        camera[27] += 256;
      }
    }

    for (k = 0; k != 33; k++) {
      for (i = 0; i != 2; i++) {
        u8 *v = ring + k * 8 + i * 4;
        s32 r = 80;

        if (i != 1) {
          r = ((Trig_Sin((frame + k * 4) << 12) * 2) >> 16) + 120;
        }
        v[0] = (Trig_Sin(k << 11) * r) >> 16;
        v[1] = (Trig_Cos(k << 11) * r) >> 16;
        v[2] = zero;
      }
    }

    {
      s32 y = gBgScroll[1].y;
      s32 d;

      if (y > 104) {
        y -= 0x10000;
      }
      if (frame <= 63) {
        d = -16;
      } else if ((u32)(frame - 64) <= 7) {
        d = frame * 2 - 144;
      } else if ((u32)(frame - 72) <= 11) {
        d = 0;
      } else if (frame > 154) {
        d = 16 - (frame - 155) / 2;
        if (d < 0) {
          d = 0;
        }
      } else {
        d = frame - 84;
        if (d > 16) {
          d = 16;
        }
        gCameraSceneParameters[4] -= d;
      }
      y += d;
      if (y < -16) {
        y += 120;
      }
      if (y > 104) {
        y -= 120;
      }
      gBgScroll[1].y = y;
    }
    if ((u32)(frame - 42) <= 37) {
      z += 0x20000;
      Func_0815f0a0((s32)&ResourceId_Data148);
    }
    if ((u32)(frame - 84) <= 58) {
      if (!effect->side) {
        x -= 0x20000;
      } else {
        x += 0x20000;
      }
      z += 0x40000;
    }
    if (frame == 143) {
      s32 *target;

      Func_08164abc(-12, -2, -2);
      camera[27] = savedYaw;
      x = 0x780000;
      z = 0x300000;
      list2[0] = effect->actors[0];
      list2[1] = 255;
      BattleActor_SpawnObjectsForListFar(list2, 0);
      gCameraSceneParameters[4] = 556;
      target = *GetBattleObjectSlotFar(effect->actors[0]);
      target[2] = zero;
      target[4] = zero;
      work->transferMode = 2;
      work->transferValue = 50;
      Iwram_CopyWords((void *)BG_PLTT, Resource_GetTableEntry((s32)&ResourceId_Data148), 128);
    }
    if (frame > 154) {
      s32 d = 16 - (frame - 155) / 2;

      if (d < 0) {
        d = 0;
      }
      gCameraSceneParameters[4] -= d;
    }
    if (frame == 84) {
      {
        volatile u16 *ime;
        struct IoWriteQueue *q;
        u32 saved;
        s32 count;

        q = &gIoWriteQueue;
        ime = &REG_IME;
        IME_SAVE(ime, saved);
        IME_DISABLE(ime);
        count = *&q->count;
        if (count <= 31) {
          u32 *entry = q->entries[count];

          *&q->count = count + 1;
          *entry++ = 0x785;
          *entry++ = (u32)&REG_BG2CNT;
          *entry = IO_WRITE_16;
        }
        IME_RESTORE(ime, saved);
      }
      Actor_SetAnimationSpeedFar(caster, 128);
      Iwram_FillWords(canvas, 0x3c00, 0x3f3f3f3f);
      work->transferMode = 3;
      work->transferValue = 0x06060606;
    }
    if (frame == 171) {
      work->shakeFrames = 8;
      Sound_PlayFar(145);
      Iwram_FillWords(canvas, 0x3c00, 0x3f3f3f3f);
      Func_08118088(effect->actors[0], 0);
      Func_0814cd48(effect->actors[0], 7, 5, 0, 16);
      work->transferMode = 1;
      work->transferValue = 0x10101010;
    }
    /* FAKEMATCH(temporary): f, a copy of frame tested below; as frame > 1, 167 of 1361 instructions differ in tla-en. */
    f = frame;
    if (frame > 174) {
      Func_08164a4c(2, 2, 2);
    }

    if (frame > 1) {
      u8 *obj;
      s32 *pair;
      s32 v;

      if (frame > 142) {
        s32 n = frame - 143;

        scale = n * 1236 + 0x4000;
        if (scale > 0xc000) {
          scale = 0xc000;
        }
      }
      pair = (s32 *)req;
      v = scale * 2;
      pair[1] = v;
      *(s32 *)req = v;
      place[3] = zero;
      place[1] = 0xff0000;
      place[0] = x;
      place[2] = z;
      obj = work->objects[0];
      obj[23] = 32;
      obj[22] = zero;
      if ((u32)(place[2] + 0x340000) <= 0xfbffff) {
        Render_ApplyProjectedPlacementFar(obj, place, (s32 *)req, 0);
      }
    }
    if (f > 1) {
      u8 textureBytes[8];
      struct ModelTexture *t;
      s32 tint;

      model = Render_AllocateModelList(1);
      t = (struct ModelTexture *)textureBytes;
      model->kind = 6;
      model->vertices = transformed;
      t->logWidth = 6;
      t->logHeight = 4;
      model->texture = t;
      model->shade = 0;
      model->scrollV = 0;
      // A once-only block: it ends CSE's block before the transform.
      do {
        t->pixels = work->sheet + 0x1000 + ((frame / 2) % 5) * 0x400;
        model->mesh = strip;
      } while (0);
      RESET_AND_SCALE_HALF();
      Func_08015160(x - 0x7e0000, z - 0x4c0000, 0);
      Func_0801521c(46536);
      Func_0801521c(scale);
      Render_TransformVertices(ring, transformed, 66);
      Render_DrawModelList(model);

      t->logWidth = 5;
      t->logHeight = 5;
      t->pixels = work->sheet + 0x2400;
      model->kind = 7;
      model->mesh = meshB;
      tint = -24;
      model->unk04 = 3;
      model->shade = tint;
      model->scrollV = zero;
      if (frame > 142) {
        s32 n = frame - 143;
        s32 scroll;
        s32 angle;
        s32 tilt;
        s32 twice;

        if (n > 32) {
          n = 32;
        }
        i = 0;
        /* FAKEMATCH(temporary): twice, frame * 2 kept apart; as scroll = frame * 3, 129 of 1361 instructions differ in tla-en. */
        twice = frame * 2;
        angle = -0x4000;
        tilt = n * 384;
        scroll = twice + frame;
        for (; i != 10; i++) {
          model->scrollU = ~scroll & 127;
          Render_ResetTransformState();
          Func_08015160(-0x20000, -0x200000, 0);
          Func_080151e4(82000, 0x35208, 0x10000);
          Func_0801521c(scale);
          Func_080150e4(0x8000);
          scroll += 20;
          SceneTransform_ApplyPitch(tilt - 0x3000);
          SceneTransform_ApplyYaw(angle);
          Render_TransformVertices(meshA, transformed, 32);
          Render_DrawModelList(model);
          angle += 0x1800;
        }
      }
      Heap_Release(model);
    }

    if (frame == 8) {
      Iwram_FillWords(canvas, 0x3c00, 0x3f3f3f3f);
      p = work->particles;
      for (i = 0; i != 64; i++, p++) {
        p->x = Random16() & 127;
        p->y = Random16() & 127;
      }
    }
    if ((u32)(frame - 8) <= 79) {
      s32 off = (frame * 2 - 16) & 127;

      p = work->particles;
      for (i = 0; i != 64; i++, p++) {
        if (p->variant >= 0) {
          s32 size = 1;
          s32 mx = p->x;
          s32 y = p->y - off;

          if (y > 127) {
            y -= 128;
          }
          if (y < 0) {
            y += 128;
          }
          blit[0](canvas, sprites + gData_08197410[size - 1], mx, y, size, 2);
        }
      }
    }
    if (frame > 142) {
      s32 n = frame - 143;

      if (n > 32) {
        n = 32;
      }
      if (frame == 143) {
        p = work->particles;
        for (i = 0; i != 64; i++, p++) {
          p->x = Random16() & 127;
          p->y = Random16() % 192 - 64;
          p->z = Random16() & 127;
        }
      }
      Render_ResetTransformState();
      SceneTransform_ApplyPitch(n * 384 - 0x3000);
      p = work->particles;
      for (i = 0; i != 64; i++, p++) {
        s32 size = 1;

        Render_ProjectPoint(p, screen);
        blit[0](canvas, sprites + gData_08197410[size - 1], screen[0], screen[1],
             size, 4);
        p->y -= 2;
      }
    }

    verts = Heap_AllocEwram(32);
    model = Render_AllocateModelList(1);
    texture2.pixels = sheet2;
    texture2.logWidth = 7;
    texture2.logHeight = 7;
    model->kind = 7;
    model->mesh = gData_08199364;
    model->texture = &texture2;
    model->vertices = verts;
    for (i = 0; i != 5; i++) {
      s32 start = Data_08199ee0[i];

      if (frame >= start && frame < start + 20) {
        s32 t = frame - start;
        s32 shade = 64 - t * 8;
        s32 s;

        if (shade > 0) {
          shade = 0;
        }
        if (shade > -64) {
          s = (t << 14) + 0x4000;
          model->shade = shade;
          Render_ResetTransformState();
          if (i <= 1) {
            Func_08015160((pos.x / 2 - 64) << 16, (pos.y - 48) << 16, 0);
          } else {
            Func_08015160(0, 0x100000, 0);
          }
          Func_080151e4(s, s * 2, s);
          SceneTransform_ApplyPitch(Data_08199ecc[i]);
          SceneTransform_ApplyYaw(Data_08199ed6[i]);
          Render_TransformVertices(Data_081991e0, verts, 4);
          Render_DrawModelList(model);
        }
      }
    }
    model->kind = 7;
    texture2.logWidth = 5;
    texture2.logHeight = 6;
    texture2.pixels = work->sheet;
    model->mesh = Data_081992b0;
    for (i = 0; i != 8; i++) {
      s32 start = 16 + i * 2;

      if (frame >= start && frame < i * 2 + 80) {
        s32 t = frame - start;
        s32 shade = t * 8 - 64;
        s32 s = t << 11;

        if (shade > 0) {
          shade = 0;
        }
        if (s > 0x10000) {
          s = 0x10000;
        }
        if (t > 47) {
          s = s - (t << 12) + 0x30000;
        }
        model->shade = shade;
        Render_ResetTransformState();
        if (!effect->side) {
          Func_08015160((pos.x / 2 + Data_08199f1a[i] - 64) << 16,
                        (Data_08199f22[i] + pos.y - 54) << 16, 0);
        } else {
          Func_08015160((pos.x / 2 - Data_08199f1a[i] - 62) << 16,
                        (Data_08199f22[i] + pos.y - 54) << 16, 0);
        }
        Func_080151e4(0x8000, 0x10000, 0x8000);
        if (!effect->side) {
          Func_080150e4(Data_08199f0a[i]);
        } else {
          Func_080150e4(-Data_08199f0a[i]);
        }
        SceneTransform_ApplyPitch(Data_08199eea[i]);
        SceneTransform_ApplyYaw(Data_08199efa[i]);
        Func_080151e4(s / 4, s * 4, s / 2);
        Render_TransformVertices(gData_081991f0, verts, 4);
        Render_DrawModelList(model);
      }
    }
    Heap_Release(model);
    Heap_Release(verts);

    {
      volatile u32 *vblanks = (volatile u32 *)&work->vblanksSinceTransfer;

      do {
        (void)*vblanks;
      } while (*vblanks <= 1);
    }
    BattleEffect_CountDownBattlerTints();
    work->transferPending = 1;
    wait(1);
  }

  ResourceObject_ReleaseFar(work->objects[0]);
  caster[18] = casterValue;
  BattleActor_CommitPlacementFar();
  transition[4] = 0;
  BattleBackground_LoadFar(1, *(u16 *)((u8 *)Ram_HeapSlots->battleWork + 0x670),
                           24);
  Task_Remove(BattleEffect_ProcessPendingGraphicsTransfer);
  HeapSlot_Release(SLOT_BLITTER);
  BattleEffect_EndCanvasLayer();
}
