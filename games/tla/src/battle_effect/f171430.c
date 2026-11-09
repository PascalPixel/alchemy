#include "battle_display.h"
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
s32 BattleEffect_LoadBlitter(s32 slot, s32 modeAndFlags);
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
s32 Task_Add(void (*func)(void), s32 order);
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

void Func_0815f0a0(s32 palette);

struct SurgeTarget {
  u8 unk00[8];
  s32 x;
  s32 y;
  s32 z;
};

extern char ResourceId_Data0C4;
extern char ResourceId_Data0C9;
extern char ResourceId_Data134;
extern char ResourceId_EffectPalette02;
extern char ResourceId_MercuryDjinnSheet;
extern u8 gMapCellBuffer[];
extern const u8 Data_08199244[];   // a quad's mesh
extern const u8 Data_081991e0[];   // its corners
extern const s32 Data_08198bfc[4]; // per pillar: its size,
extern const s8 Data_08198c0c[4];  // its shade
extern s8 Data_08198c10[4];        // and its offset

#define LIGHT_SURGE_FRAMES 74
#define SURGE_BOLTS 14

// Abilities: Light Surge.
//
// The camera swings around the target while 14 bolts of light rain onto
// it, four frames apart; each lands with a flash and a ring of light (two
// rings of shaded circles), and four pillars of light rise and sway over
// the target.
void BattleEffect_RunLightSurge(struct BattleEffectArgument *effect) {
  struct BattleEffectWork *work;
  void *canvas;
  s32 frame;
  s32 i;
  /* FAKEMATCH(dead): seven locals nothing uses, which raise camera's
   * register number so gcse's hash puts camera->pos after frame << 8; as no
   * locals, 8 of 828 instructions differ in tla-en. */
  s32 unused1;
  s32 unused2;
  s32 unused3;
  s32 unused4;
  s32 unused5;
  s32 unused6;
  s32 unused7;
  struct BattleCamera *camera;
  s16 savedYaw;
  struct SurgeTarget *target;
  s16 *circles;
  s32 x;
  s32 y;
  struct EffectPosition screen;
  struct EffectPosition *pos;
  s32 head[3];
  struct EffectPosition center;
  struct ModelTexture texture;
  struct RenderModel *model;
  struct EffectStep *fall;
  struct EffectStep *bolt;
  struct EffectStep *ring;
  struct EffectStep *flash;
  struct EffectStep *glow;
  s16 *vertices;
  s32 offset;
  s32 size;
  s32 shade;
  u8 *sprites;
  s32 cells;

  work = (struct BattleEffectWork *)Ram_HeapSlots->battleFxWork[0];
  canvas = Ram_HeapSlots->battleFxWork[1];
  sprites = Ram_HeapSlots->unk64[0];
  BattleEffect_StartCanvasLayer(1);
  REG_BLDALPHA = 0x1010;
  BattleEffect_LoadBlitter(
      SLOT_BLITTER, BLITTER(BLIT_MAX, COMPOSITE_CLIP_X | COMPOSITE_CLIP_Y));
  Resource_LoadAndDecompress((s32)&ResourceId_Data0C4, work->sheet + 0x1000, 0,
                             0);
  Resource_LoadAndDecompress((s32)&ResourceId_Data0C9, work->sheet + 0x4000, 0,
                             0);
  Resource_LoadAndDecompress((s32)&ResourceId_Data134, sprites, 0, 0);
  Iwram_CopyWords((void *)BG_PLTT,
                  Resource_GetTableEntry((s32)&ResourceId_EffectPalette02),
                  128);
  work->transferMode = 2;
  work->transferValue = 50;
  Task_Add(BattleEffect_ProcessPendingGraphicsTransfer, 0xc80);
  camera = Ram_HeapSlots->cameraWork;
  savedYaw = camera->yaw;
  target = *GetBattleObjectSlotFar(effect->actors[0]);

  // Shaded circles of 2 to 64 pixels, packed in the map buffer.
  cells = (s32)gMapCellBuffer;
  circles = (s16 *)work->sheet;
  offset = 0;
  size = 2;
  for (i = 0; i != 32; i++) {
    Func_0815b434((u8 *)(offset + cells), size, 0x10000);
    circles[i] = offset;
    offset += (i * 2 + 3) * size / 2;
    size += 2;
  }

  frame = 0;
  /* FAKEMATCH(dead): pos first takes the camera's look-at point, a store the
   * loop's pos = &screen overwrites, so gcse copies camera->pos from here
   * instead of inserting it after pos in the loop's preheader; without it, 16
   * of 828 instructions differ in tla-en. */
  pos = (struct EffectPosition *)camera->pos;
  /* FAKEMATCH(dead): the texture's address, computed as center's address
   * less 8 so the bolts still take texture's own address first (their
   * bit-field stores then stay one word), stored in vertices, which is set
   * again before any read, so gcse copies it from here ahead of pos in the
   * loop's preheader; without it, 8 of 828 instructions differ in tla-en. */
  vertices = (s16 *)((s32)&center - 8);
  for (pos = &screen; frame != LIGHT_SURGE_FRAMES; frame++) {
    if (frame == 64) {
      Func_081180e8(134);
    }
    if (frame > 31 && (frame & 3) == 0) {
      Func_0815f0a0((s32)&ResourceId_MercuryDjinnSheet);
    }
    if (frame == 0) {
      camera->yaw = savedYaw;
      for (i = 0; i != 32; i++) {
        work->particles[i].variant = -1;
      }
      for (i = 0; i != 32; i++) {
        s32 radius;
        s32 angle;
        s32 z;

        fall = &work->particles[32 + i];
        radius = (Random16() & 63) + 32;
        angle = Random16() & 0xffff;

        fall->x = Trig_Sin(angle) * radius;
        fall->z = Trig_Cos(angle) * radius;
        fall->y = 0x6e0000;
        radius = (Random16() & 31) + 4;
        fall->velocityX = target->x + Trig_Sin(angle) * radius;
        z = target->z + Trig_Cos(angle) * radius;
        fall->velocityZ = z;
        fall->velocityY = -0x80000;
        fall->velocityX = (fall->velocityX - fall->x) / 6;
        fall->velocityY = (-0x80000 - fall->y) / 6;
        fall->velocityZ = (z - fall->z) / 6;
        fall->variant = -i * 4;
      }
    }
    if ((u32)(frame - 20) <= 59) {
      s32 turn;

      if (frame <= 71) {
        turn = 0x100;
      } else {
        turn = 0xa00 - (frame << 5);
      }
      if (effect->side == 0) {
        camera->yaw += turn;
      } else {
        camera->yaw -= turn;
      }
    }
    Render_ResetTransformState();
    Transform_SetLookAt(camera->eye, camera->pos);

    // The rings: four quarter circles each, upper halves for the first 16.
    for (i = 0, ring = work->particles; i != 28; ring++, i++) {
      void *cel;
      s32 half;
      s32 index;
      s16 *entry;
      s32 left;
      s32 size;

      if ((u32)ring->variant > 15) {
        continue;
      }
      size = ((Trig_Sin(ring->variant << 11) * ring->velocityX) >> 16) * 2;
      index = size - 2;
      ring->variant++;
      if ((u32)index > 61) {
        continue;
      }
      EffectPosition_ProjectPoint(ring, pos);
      index >>= 1;
      x = pos->x / 2;
      y = pos->y;
      BattleEffect_LoadBlitter(
          188, BLITTER(BLIT_MAX, COMPOSITE_CLIP_X | COMPOSITE_CLIP_Y));
      index <<= 1;
      index += (s32)circles;
      half = size >> 1;
      cel = (void *)(*(s16 *)index + cells);
      entry = (s16 *)index;
      left = x - half;
      index = y - size;
      ((DrawRectangle)Ram_HeapSlots->unkb8[1])(canvas, cel, left, index, half,
                                               size);
      HeapSlot_Release(188);
      BattleEffect_LoadBlitter(188, BLITTER(BLIT_MAX, COMPOSITE_CLIP_X |
                                                          COMPOSITE_CLIP_Y |
                                                          COMPOSITE_FLIP_X));
      ((DrawRectangle)Ram_HeapSlots->unkb8[1])(canvas, (void *)(*entry + cells),
                                               x, index, half, size);
      HeapSlot_Release(188);
      if (i > 15) {
        BattleEffect_LoadBlitter(188, BLITTER(BLIT_MAX, COMPOSITE_CLIP_X |
                                                            COMPOSITE_CLIP_Y |
                                                            COMPOSITE_FLIP_Y));
        ((DrawRectangle)Ram_HeapSlots->unkb8[1])(
            canvas, (void *)(*entry + cells), x - half, y, half, size);
        HeapSlot_Release(188);
        BattleEffect_LoadBlitter(188, BLITTER(BLIT_MAX, COMPOSITE_CLIP_X |
                                                            COMPOSITE_CLIP_Y |
                                                            COMPOSITE_FLIP));
        ((DrawRectangle)Ram_HeapSlots->unkb8[1])(
            canvas, (void *)(*entry + cells), x, y, half, size);
        HeapSlot_Release(188);
      }
    }

    // The bolts: a streak from the sky to the ground, six frames long.
    vertices = Heap_AllocEwram(32);
    model = Render_AllocateModelList(1);
    shade = 0;
    texture.logWidth = 5;
    texture.logHeight = 2;
    model->texture = &texture;
    model->mesh = Data_08199244;
    model->vertices = vertices;
    model->shade = shade;
    model->kind = 7;
    texture.pixels = work->sheet + 0x4000;
    for (i = 0, flash = work->particles, glow = &work->particles[16],
        bolt = &work->particles[32];
         i != SURGE_BOLTS; flash++, glow++, bolt++, i++) {
      s32 t = ++bolt->variant;
      struct EffectPosition *out;

      if ((u32)t > 17) {
        continue;
      }
      if (t > 11) {
        head[0] = bolt->x + bolt->velocityX * (t - 12);
        head[1] = bolt->y + bolt->velocityY * (bolt->variant - 12);
        head[2] = bolt->z + bolt->velocityZ * (bolt->variant - 12);
        if (bolt->variant == 16) {
          glow->x = head[0];
          glow->y = head[1];
          glow->z = head[2];
          glow->velocityX = (Random16() & 15) + 8;
          glow->variant = 0;
        }
        out = pos;
        EffectPosition_ProjectPoint((struct EffectStep *)head, out);
      } else {
        out = &screen;
        EffectPosition_ProjectPoint(bolt, out);
      }
      vertices[0] = out->x / 2 - 2;
      vertices[1] = out->y;
      vertices[4] = out->x / 2 + 2;
      vertices[5] = out->y;
      if (bolt->variant <= 5) {
        head[0] = bolt->x + bolt->velocityX * bolt->variant;
        head[1] = bolt->y + bolt->velocityY * bolt->variant;
        head[2] = bolt->z + bolt->velocityZ * bolt->variant;
      } else {
        head[0] = bolt->x + bolt->velocityX * 6;
        head[1] = bolt->y + bolt->velocityY * 6;
        head[2] = bolt->z + bolt->velocityZ * 6;
      }
      if (bolt->variant == 6) {
        flash->x = head[0];
        flash->y = head[1];
        flash->z = head[2];
        flash->velocityX = (Random16() & 7) + 24;
        flash->variant = 0;
        work->shakeFrames = 4;
        Sound_PlayFar(145);
        Func_08118088(effect->actors[0], 0);
        Func_0814cd48(effect->actors[0], 7, 5, 0, 2);
      }
      EffectPosition_ProjectPoint((struct EffectStep *)head, out);
      vertices[8] = out->x / 2 - 2;
      vertices[9] = out->y;
      vertices[12] = out->x / 2 + 2;
      vertices[13] = out->y;
      Render_DrawModelList(model);
    }

    // The pillars.
    if ((u32)(frame - 16) <= 63) {
      EffectPosition_ProjectBattler(effect->actors[0], &center);
      texture.logWidth = 5;
      texture.logHeight = 5;
      texture.pixels = work->sheet + 0x1000;
      model->kind = 6;
      model->mesh = Data_08199244;
      for (i = 0; i != 4; i++) {
        s32 sway = Trig_Sin((frame << 8) + 0x1000);
        s32 x =
            ((center.x / 2 - 64) << 16) -
            (Data_08198c10[i] * sway / 0x10000) * Trig_Sin((s16)camera->yaw);
        s32 z =
            -((Data_08198c10[i] * sway / 0x10000) * Trig_Cos((s16)camera->yaw));
        s32 growth;
        u32 scale;

        /* FAKEMATCH(dead): a store both arms below overwrite; its test builds
         * 300 in a reload register, the one pick before frame's reload that
         * sends it to r1; without it, 10 of 828 instructions differ in
         * tla-en. */
        if (i == 300)
          growth = 0;
        if (frame <= 47) {
          growth = Trig_Sin((frame << 9) - 0x2000);
        } else {
          growth = 0x10000;
        }
        scale = (u32)(Data_08198bfc[i] * growth) >> 16;
        if (frame > 63) {
          shade = Data_08198c0c[i] - frame * 8 + 512;
        } else if (frame <= 23) {
          shade = Data_08198c0c[i] + frame * 8 - 192;
        } else {
          shade = Data_08198c0c[i];
        }
        model->shade = shade;
        Render_ResetTransformState();
        Func_08015160(x, z, 0);
        Func_080151e4(scale, scale * 2, scale);
        Render_TransformVertices(Data_081991e0, vertices, 4);
        Render_DrawModelList(model);
      }
    }
    Heap_Release(model);
    Heap_Release(vertices);
    /* FAKEMATCH(dead): a read of shade the compiler folds to nothing, which
     * keeps shade in its stack slot; as a plain 4, 283 of 828 instructions
     * differ in tla-en. */
    Func_08158ce0(4 + ((shade & 255) >> 8), 4);
    BattleEffect_CountDownBattlerTints();
    work->transferPending = 1;
    wait(1);
  }
  Task_Remove(BattleEffect_ProcessPendingGraphicsTransfer);
  HeapSlot_Release(104);
  BattleEffect_EndCanvasLayer();
}
