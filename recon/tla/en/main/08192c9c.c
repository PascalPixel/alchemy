// Draft of BattleEffect_RunIris, decompiled from the listing: differ 4932 of 5364
// (draft 5319), the same 257 calls and control flow; allocation differs.

#include "types.h"
#include "battle_effect.h"
#include "battle_effect_work.h"
#include "effect_step.h"
#include "io_reg.h"
#include "iwram_call.h"
#include "mem.h"
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
void BattleEffect_EndCanvasLayer(void); // BattleEffect_EndCanvasLayer
void BattleEffect_ProcessPendingGraphicsTransfer(void); // BattleEffect_ProcessPendingGraphicsTransfer
void ObjectGroup_CountDownMemberTimers(void); // ObjectGroup_CountDownMemberTimers
void BattleEffect_LoadBlitter(s32 slot, s32 modeAndFlags); // BattleEffect_BuildBlitter
void Func_0814cd48(s32 actor, s32 palette, s32 pose, s32 timerSlot,
                   s32 timerFrames);              // ObjectGroup_UpdateMembers
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
void Func_081180e8(s32 phase);             // BattleEventRuntime_StartPhaseFar
void Func_08158ce0(s32 randomMask, u32 shakeRange); // Camera_ApplyShake
void BattleEffectKernels_IntegrateVector2(struct EffectStep *step, s32 gravity,
                                      s32 drag);
void **GetBattleObjectSlotFar(s32 objectId);
void *Runtime_BumpAllocateAlternatePool(s32 size);
void Resource_LoadAndDecompress(s32 resource, void *destination, s32 a, s32 b);
void *Resource_GetTableEntry(s32 resource);
void Scheduler_AddOrUpdateCallback(void (*callback)(void), s32 order);
void Scheduler_RemoveCallback(void (*callback)(void));
void Runtime_ReleaseHeapBlock(s32 slot);
void Audio_PlayCue(s32 cue);
void WaitFrames(s32 frames);
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
#include "dma.h"
#include "input.h"

struct ResourceTableEntry {
  u16 unk0;
  u16 tile;
};

struct WordPair {
  s32 a;
  s32 b;
};

struct Scale {
  s32 x;
  s32 y;
};

// The spark objects' work, in the map cell buffer at 0x1f40.
struct IrisSparks {
  s16 life[36]; // high byte: frames left; 0 frees the slot
  u16 tiles[15];
  u16 frames[36];
  s32 x[36];
  s32 y[36];
  s32 vx[36];
  s32 vy[36];
};

// A point of a streak: the head moves, the other nine trail it.
struct IrisPoint {
  s32 x;
  s32 y;
  s32 angle;
  s32 speed;
  s32 unk10;
};

// The opening's circle of light and its fifteen streaks, at 0x2ee0.
struct IrisScene {
  s32 x;
  s32 y;
  s32 unk08;
  s32 unk0c;
  s32 radius;
  struct IrisPoint streaks[15][10];
};

struct IrisSprite {
  u8 unk00[8];
  u16 tile : 10;
  u16 unk08_10 : 6;
  u8 unk0a[6];
  u8 resource;
};

struct IrisSpriteFlags {
  u8 unk00[9];
  u8 unk09_0 : 2;
  u8 priority : 2;
  u8 unk09_4 : 4;
};

struct Widths5 {
  s32 w[5];
};

struct Widths7 {
  s32 w[7];
};

struct Spans4 {
  struct WordPair p[4];
};

extern struct ResourceTableEntry gResourceTableEntries[];
extern u8 gMapCellBuffer[];
extern u8 Data_02010578[];
extern struct IrisSparks Data_02011f40;
extern struct IrisScene Data_02012ee0;
extern s32 gCameraSceneParameters[];
extern const u16 gData_08197410[];
extern const struct Scale Data_08196f48;
extern const struct Widths5 Data_08196f50;
extern const struct Widths7 Data_08196f64;
extern const struct Widths7 Data_08196f80;
extern const struct Spans4 Data_08196f9c;
extern const struct Spans4 Data_08196fbc;
extern const s32 Data_08199f40[];
extern const u8 Data_08199f48[];
extern const u8 Data_08199f88[];
extern const u8 Data_08199f98[];
extern const u8 Data_08199fbc[];
extern const u16 Data_08199fc8[];
extern const u16 Data_08199fd2[];
extern const s8 Data_08199fde[];
extern const u8 Data_08199fed[];
extern const u16 Data_08199ffc[];
extern const s16 Data_0819a01a[];
extern const u8 Data_0819a038[];
extern const s32 Data_0819a040[];
extern const u16 Data_0819a048[];
extern const u16 Data_0819a054[];
extern const u8 Data_0819a060[];
extern const u8 Data_0819a066[];
extern const u8 Data_0819a06c[];
extern const u8 Data_0819a072[];
extern const u8 Data_0819a078[];
extern const u8 Data_0819a07e[];
extern const u8 Data_0819a084[];
extern const u8 Data_0819a0ac[];
extern const u8 Data_0819a0d4[];
extern const u8 Data_0819a140[];
extern const u8 Data_0819a160[];

struct IrisSprite *Func_0815b290(s32 width, s32 height, u32 shape, s32 mode);
struct IrisSprite *Func_0815b3b0(s32 width, s32 height, u32 shape, s32 mode);
void ResourceObject_ReleaseFar(void *object);
void Func_0813ba50(void); // BattleEffect_ConfigureEffectDisplay
void BattleBackground_LoadFar(s32 layer, s32 background, s32 palette);
void Resource_DecodeType01(const void *source, void *destination);
void Palette_LoadBasicObjColors(void);
void Graphics_LoadGreyRamp(void);
void Func_0815b510(u8 *map, s32 size, s32 scale);
void Func_08164b2c(s32 red, s32 green, s32 blue);
void Func_08191958(u8 side, u8 speed, void (*midpoint)(void));
void Func_0819253c(void);
void Func_08191cc4(void);
void BattleEffect_BuildSteepShadeRampWave(u8 *table, u32 count, u32 repeats, s32 scale);
void Func_08192648(u32 frame, s32 x, s32 y, s32 small);
void Func_0819273c(s32 halfWidth, s32 x, s32 height);
void BattleEffect_DrawDiagonalCanvasLine(s32 x, s32 y, s32 w, u8 color);
void SceneTransform_ResetMatrix(void);
void SceneTransform_ApplyPosition(s32 *position);
void Func_080151ac(s32 *scale); // SceneTransform_ApplyScale
void MemFree(void *buffer);

#define SLOT_ALT 188 // unkb8[1]
#define OBJ_TILES(obj)                                                         \
  (0x06010000 + gResourceTableEntries[((u8 *)(obj))[16]].tile)
#define SCENE (&Data_02012ee0)
// The 128 background colours the scene fades (entries 96 to 223).
#define FADE_PLTT ((u16 *)(BG_PLTT + 0xc0))

// A splash where a drop lands: rings spread from (x, y) for twice the
// splash's duration, then a smaller pair of rings and a column of spray
// rises while the screen shakes; the drop itself is drawn last.
static inline void Iris_DrawSplash(s32 t, s32 x, s32 y, s32 small) {
  struct BattleEffectWork *work =
      (struct BattleEffectWork *)Ram_HeapSlots->battleFxWork[0];
  u8 *canvas = Ram_HeapSlots->battleFxWork[1];
  u8 *sprites = Ram_HeapSlots->unk64[0];
  DrawRectangle draw = Ram_HeapSlots->unk64[1];
  DrawRectangle drawAlt = Ram_HeapSlots->unkb8[1];
  s32 r;
  s32 cel;
  s32 j;
  s32 a;
  s32 px;
  s32 py;
  u8 *ring;

  if (t < Data_08199f40[small] * 2) {
    r = Data_08199f40[small] * 2 - t;
    cel = r / 6;
    for (j = 0; j != 64; j++) {
      a = j << 10;
      px = r * Trig_Sin(a) / 0x20000 + x;
      py = (r * Trig_Cos(a) >> 16) + y;
      draw(canvas, sprites + 444 + cel * 8, px, py - 2, 2, 4);
    }
    if (small == 0) {
      s32 offset = 0;

      if (t <= 11)
        offset = 196;
      else if (t <= 23)
        offset = 98;
      ring = sprites + offset;
      for (j = 0; j != 8; j++) {
        a = (j << 13) + (t * 5 << 9);
        px = (r * Trig_Sin(a) >> 16) + x;
        py = (r * Trig_Cos(a) * 2 >> 16) + y;
        draw(canvas, ring, px - 3, py - 7, 7, 14);
        a += a >> 1;
        px = (Iwram_MulQ16(0x28000, r * Trig_Sin(a) / 2) >> 16) + x;
        py = (Iwram_MulQ16(0x50000, r * Trig_Cos(a) / 2) >> 16) + y;
        draw(canvas, ring, px - 3, py - 7, 7, 14);
      }
      draw(canvas, work->sheet + gData_08197410[8] + cel * 0x302, x - 4, y - 9,
           9, 18);
    } else {
      s32 offset = 294;

      if (t <= 11)
        offset = 394;
      else if (t <= 23)
        offset = 344;
      ring = sprites + offset;
      for (j = 0; j != 8; j++) {
        a = (j << 13) + (t * 5 << 9);
        px = (r * Trig_Sin(a) >> 16) + x;
        py = (r * Trig_Cos(a) * 2 >> 16) + y;
        draw(canvas, ring, px - 2, py - 5, 5, 10);
        a += a >> 1;
        px = (Iwram_MulQ16(0x28000, r * Trig_Sin(a) / 2) >> 16) + x;
        py = (Iwram_MulQ16(0x50000, r * Trig_Cos(a) / 2) >> 16) + y;
        draw(canvas, ring, px - 2, py - 5, 5, 10);
      }
      draw(canvas, work->sheet + gData_08197410[6] + cel * 0x302, x - 3, y - 7,
           7, 14);
    }
  } else {
    if (t < Data_08199f40[small] * 2 + Data_08199f40[small]) {
      r = t - Data_08199f40[small] * 2;
      cel = r / 3 * 8;
      if (cel == 64)
        cel = 56;
      ring = sprites + cel + 444;
      for (j = 0; j != 64; j++) {
        a = j << 10;
        px = (r * Trig_Sin(a) >> 16) + x;
        py = (r * Trig_Cos(a) * 2 >> 16) + y;
        draw(canvas, ring, px - 1, py - 2, 2, 4);
      }
      for (j = 0; j != 64; j++) {
        a = j << 10;
        px = (r * Trig_Sin(a) * 2 >> 16) + x;
        py = (r * Trig_Cos(a) >> 16) + y + Data_08199f48[small] * 2;
        draw(canvas, ring, px - 1, py - 2, 2, 4);
      }
    }
    for (j = 0, py = y; j < t - Data_08199f40[small] * 2 && py <= 112;
         j++, py += 32)
      draw(canvas, sprites + 508, x - 4, py, 8, 32);
    if (t < Data_08199f40[small] * 2 + 8) {
      s32 shake = (Data_08199f40[small] * 2 - t + 8) >> 1;

      gBgScroll[1].y = 32;
      if (t & 1)
        gBgScroll[1].y = shake + 32;
      else
        gBgScroll[1].y = 32 - shake;
    }
    if (small == 0)
      drawAlt(canvas, work->sheet + 0x1b12, x - 10, y - 20, 20, 40);
    else
      drawAlt(canvas, work->sheet + 0x4c12, x - 6, y - 12, 12, 24);
  }
}

// Summon 25 (Iris): a circle of light opens over a dark field while fifteen
// streaks swirl up out of it; the goddess's crystal rises in a 4 x 4 grid
// of sprites and bursts into sparks; drops fall and splash in rings; pillars
// of light climb, the scene fills with rising motes, rays turn about the
// centre, and the screen whitens as she heals the party.
void BattleEffect_RunIris(struct BattleEffectArgument *effect) {
  struct IrisSparks *sparks;
  struct IrisSprite **objects;
  s32 frame;
  struct BattleEffectWork *work;
  DrawRectangle draw[2];
  u8 *canvas;

  // The first spark slot with no life left, or -1.
  s32 FindFreeSpark(void) {
    s32 i;
    s32 slot = 36;

    for (i = 0; i != 36; i++) {
      if (sparks->life[i] == 0) {
        slot = i;
        break;
      }
    }
    if (slot == 36)
      slot = -1;
    return slot;
  }

  // Each live spark: placed at its size, moved, dropped when it leaves the
  // screen, otherwise animated and aged.
  void UpdateSparks(void) {
    struct Scale scale;
    s32 place[4];
    s32 i;

    for (i = 0; i != 36; i++) {
      if (sparks->life[i] != 0) {
        scale.x = scale.y = sparks->life[i] * 2;
        place[3] = 0;
        place[1] = 0xff0000;
        place[0] = sparks->x[i];
        place[2] = sparks->y[i];
        Render_ApplyProjectedPlacementFar(objects[i], place, (s32 *)&scale, 0);
        sparks->x[i] += sparks->vx[i];
        sparks->y[i] += sparks->vy[i];
        if (sparks->x[i] < -0x100000 || sparks->x[i] > 0xf00000 ||
            sparks->y[i] < 0 || sparks->y[i] > 0xa00000) {
          sparks->life[i] = 0;
          continue;
        }
        if (!(frame & 3))
          sparks->frames[i]++;
        if (sparks->frames[i] > 14)
          sparks->frames[i] = 0;
        objects[i]->tile = sparks->tiles[sparks->frames[i]];
        sparks->life[i] -= 0x100;
      }
    }
  }

  // The first free mote of 32, or -1.
  s32 FindFreeParticle(void) {
    s32 i;
    s32 slot = 32;

    for (i = 0; i != 32; i++) {
      if (work->particles[i].variant == 0) {
        slot = i;
        break;
      }
    }
    if (slot == 32)
      slot = -1;
    return slot;
  }

  // The motes: a sheet cell, fading after 32 frames with bit 4, or a
  // textured quad spinning up with bit 8.
  void DrawParticles(void) {
    struct ModelTexture texture;
    s32 place[3];
    s32 i;

    for (i = 0; i != 32; i++) {
      if (work->particles[i].variant != 0) {
        s32 n = work->particles[i].variant & 15;
        s32 px = work->particles[i].x >> 16;
        s32 py = work->particles[i].y >> 16;
        s32 offset = 0;

        if (work->particles[i].x < 0 || work->particles[i].x > 0x7f0000 ||
            work->particles[i].y < 0 || work->particles[i].y > 0x7f0000) {
          work->particles[i].variant = 0;
          continue;
        }
        if (work->particles[i].variant & 16) {
          s32 age = frame - work->particles[i].z;

          if (age > 32)
            offset = (age - 32) * 0x302;
          if (offset == 0x1810) {
            work->particles[i].variant = 0;
            continue;
          }
        }
        if (work->particles[i].variant & 256) {
          void *verts = Runtime_BumpAllocateAlternatePool(32);
          struct RenderModel *model = Render_AllocateModelList(1);
          s32 age;

          offset = 0;
          age = frame - work->particles[i].z;
          if (age > 24)
            offset = (age - 24) * 0x800;
          texture.logWidth = 5;
          texture.logHeight = 6;
          texture.pixels = work->sheet + 0x1810 + offset;
          SceneTransform_ResetMatrix();
          place[0] = work->particles[i].x - 0x400000;
          place[1] = work->particles[i].y - 0x400000;
          place[2] = 0;
          SceneTransform_ApplyPosition(place);
          place[1] = place[0] = work->particles[i].velocityZ;
          place[2] = 0;
          Func_080151ac(place);
          work->particles[i].velocityZ += 0x2000;
          model->kind = 4;
          model->mesh = Data_08199f98;
          model->texture = &texture;
          model->vertices = verts;
          Render_TransformVertices(Data_08199f88, verts, 4);
          Render_DrawModelList(model);
          MemFree(model);
          MemFree(verts);
          if (offset == 0x3800)
            work->particles[i].variant = 0;
        } else {
          draw[0](canvas, work->sheet + (gData_08197410[n - 1] + offset),
                  px - n / 2, py - n, n, n * 2);
        }
        work->particles[i].x += work->particles[i].velocityX;
        work->particles[i].y += work->particles[i].velocityY;
      }
    }
  }

  u8 *sprites;
  s32 pos[4];
  u8 *battle;
  u16 savedScroll;
  s16 *camera;
  s16 savedZoom;
  struct Scale scale;
  struct Scale shrink;
  struct ModelTexture texture;
  struct ModelTexture texture2;
  s32 lift;
  s32 i;
  s32 j;
  s32 k;
  s32 m;
  s32 x;
  s32 y;
  s32 off;
  s32 top;
  u8 *dst;

  canvas = Ram_HeapSlots->battleFxWork[1];
  work = (struct BattleEffectWork *)Ram_HeapSlots->battleFxWork[0];
  objects = (struct IrisSprite **)gMapCellBuffer;
  sprites = Ram_HeapSlots->unk64[0];
  sparks = &Data_02011f40;
  battle = (u8 *)Ram_HeapSlots->battleWork;
  savedScroll = gBgScroll[1].x;
  camera = Ram_HeapSlots->cameraWork;
  work->effect = effect;
  BattleEffect_StartCanvasLayer(0x2000);
  Func_0813ba50();
  *(u16 *)BG_PLTT = 0;
  *(u16 *)(BG_PLTT + 2) = 0;
  work->transferMode = 0;
  work->transferValue = 75;
  Scheduler_AddOrUpdateCallback(BattleEffect_ProcessPendingGraphicsTransfer, 0xc80);
  REG_DISPCNT = 0x7741;
  REG_BG2CNT = 0x784;
  REG_BG2PA = 0x80;
  REG_BLDALPHA = 0x1010;
  REG_BLDCNT = 0x3f44;
  Func_08191958(0, 4, Func_0819253c);
  Iwram_FillWords(canvas, 0x4000, 0);
  while (*(volatile u32 *)(0x040000d4 + 8) & 0x80000000) {
  }
  work->transferPending = 1;
  WaitFrames(1);
  REG_BG2PA = 0x80;
  *(volatile u32 *)0x04000028 = 0;
  REG_BLDALPHA = 0x1010;
  REG_BLDCNT = 0x3f44;
  work->transferMode = 2;
  work->transferValue = 75;
  savedZoom = camera[27];
  if (effect->side == 0)
    camera[27] = 0x4000;
  else
    camera[27] = 0xc000;
  BattleEffect_LoadBlitter(SLOT_BLITTER, 19);
  BattleEffect_LoadBlitter(SLOT_ALT, 3);
  draw[0] = Ram_HeapSlots->unk64[1];
  draw[1] = Ram_HeapSlots->unkb8[1];
  Resource_LoadAndDecompress(0x134, Data_02010578, 0, 0);
  Resource_LoadAndDecompress(0x134, sprites, 0, 0);
  for (i = 0; i != 0x302; i++)
    sprites[i] >>= 1;
  Func_0815b434(work->sheet, 32, 0x10000);
  Graphics_LoadGreyRamp();
  frame = 0;
  Runtime_ReleaseHeapBlock(SLOT_ALT);
  Render_ResetTransformState();

  for (;;) {
    // A or B: skips the opening, or the rest of the scene.
    if (gInput.repeat & 3) {
      if (frame <= 175) {
        frame = 176;
      } else if ((u32)(frame - 177) <= 268) {
        REG_BLDALPHA = 0x1010;
        Iwram_FillWords(canvas, 0x4000, 0x3f3f3f3f);
        work->transferPending = 1;
        WaitFrames(1);
        if (frame <= 386) {
          for (i = 0; i != 16; i++)
            ResourceObject_ReleaseFar(work->objects[i]);
          BattleEffect_SelectLivingTargets(work->effect);
          Palette_LoadBasicObjColors();
          for (i = 0; i != work->effect->count; i++)
            Func_0814cd48(work->effect->actors[i], -1, 5, -1, 0);
        }
        frame = 446;
      } else {
        Iwram_FillWords(canvas, 0x4000, 0x3f3f3f3f);
        work->transferPending = 1;
        WaitFrames(1);
        break;
      }
    }
    if (frame == 850)
      break;

    if (frame == 0)
      Audio_PlayCue(206);
    for (i = 0; i != 11; i++)
      if (frame == Data_08199fbc[i])
        Audio_PlayCue(130);
    if (frame == 175)
      Audio_PlayCue(212);
    for (i = 0; i != 5; i++)
      if (frame == Data_08199fc8[i])
        Audio_PlayCue(209);
    for (i = 0; i != 6; i++)
      if (frame == Data_08199fd2[i])
        Audio_PlayCue(212);
    if (frame == 369)
      Audio_PlayCue(206);
    for (i = 0; i != 6; i++)
      if (frame == 644 + i * 12)
        Audio_PlayCue(104);
    if (frame == 755)
      Audio_PlayCue(212);
    for (i = 0; i != 6; i++)
      if (frame == 759 + i * 9)
        Audio_PlayCue(145);
    if (frame == 820)
      Audio_PlayCue(208);

    // The opening: the circle and the streaks.
    if (frame == 0) {
      SCENE->x = 0x3c0000;
      SCENE->y = 0x800000;
      SCENE->radius = 0xc0000;
      for (i = 0; i != 15; i++) {
        struct IrisPoint *s = SCENE->streaks[i];

        s->x = Data_08199fde[i] << 16;
        s->y = Data_08199fed[i] << 16;
        s->angle = Data_08199ffc[i];
        s->speed = Data_0819a01a[i];
        s->unk10 = 0;
        for (j = 1; j != 10; j++)
          s[j].x = s[j].y = 0x960000;
      }
      work->transferMode = 2;
      work->transferValue = 50;
    }
    if (frame <= 175) {
      BattleEffect_LoadBlitter(SLOT_ALT, 3);
      draw[1] = Ram_HeapSlots->unkb8[1];
      for (i = 0; i != 15; i++) {
        struct IrisPoint *s = SCENE->streaks[i];

        if (!(frame & 1)) {
          for (j = 9; j != 0; j--) {
            s[j].x = s[j - 1].x;
            s[j].y = s[j - 1].y;
          }
        }
        if (frame >= i * 8 + 24 && HI(s->y) > 59) {
          s32 size;

          s->x += Trig_Sin(s->angle) * 3 / 2;
          s->y -= Trig_Cos(s->angle) * 3 / 2;
          s->angle += s->speed * 2 / 3;
          size = (i & 3) + 5;
          for (j = 0; j != 10; j++)
            draw[1](canvas, sprites + gData_08197410[size - 1],
                    HI(s[j].x) - (size >> 1), HI(s[j].y) - size, size,
                    size * 2);
        }
      }
      if (frame <= 71) {
        SCENE->y -= 0x10000;
        SCENE->radius -= 0x1000;
        if (!(frame & 15)) {
          for (i = 0; i != 128; i++) {
            u16 c = FADE_PLTT[i];
            s32 r = c & 31;
            s32 g = c >> 5 & 31;
            s32 b = c >> 10 & 31;

            FADE_PLTT[i] = (b + 1) << 10 | (g + 1) << 5 | (r + 1);
          }
        }
      } else {
        SCENE->radius += 0x2000;
        if (!(frame & 7)) {
          for (i = 0; i != 128; i++) {
            u16 c = FADE_PLTT[i];
            s32 r = c & 31;
            s32 g = c >> 5 & 31;
            s32 b = c >> 10 & 31;

            if (r < 23)
              r++;
            if (g > 1)
              g--;
            if (b < 15)
              b++;
            FADE_PLTT[i] = b << 10 | g << 5 | r;
          }
        }
      }
      Runtime_ReleaseHeapBlock(SLOT_ALT);
      {
        s32 size = HI(SCENE->radius) * 2;

        Func_0815b434(work->sheet, size, 0x10000);
        Func_0818caa8(work->sheet, HI(SCENE->x), HI(SCENE->y), size);
      }
      if (frame == 175) {
        REG_BG2CNT = 0x785;
        Iwram_FillWords(canvas, 0x4000, 0x3f3f3f3f);
        Iwram_FillWords(FADE_PLTT, 0x100, 0x7fff7fff);
      }
    }

    // The crystal: sixteen sprites, the sparks and the first motes.
    if (frame == 176) {
      void *colors;

      Iwram_FillWords(canvas, 0x4000, 0x3f3f3f3f);
      work->transferPending = 1;
      WaitFrames(1);
      battle[0x867] = 1;
      BattleBackground_LoadFar(1, 0x76, 0);
      colors = Resource_GetTableEntry(0xa5);
      Dma_Set(colors, (void *)(BG_PLTT + 0x200), 0x84000080, REG_DMA3);
      Resource_DecodeType01((u8 *)colors + 0x200, Data_02010578);
      for (i = 0; i != 16; i++) {
        struct IrisSprite *obj = Func_0815b290(32, 32, 0x80002000, 0);

        work->objects[i] = obj;
        ((struct IrisSpriteFlags *)obj)->priority = 1;
        Dma_Set(Data_02010578 + i * 0x400, (void *)OBJ_TILES(obj), 0x84000100,
                REG_DMA3);
      }
      colors = Resource_GetTableEntry(0x9a);
      Iwram_FillWords((void *)(BG_PLTT + 0x3e0), 32, -1);
      Resource_DecodeType01((u8 *)colors + 32, work->sheet);
      for (i = 0; i != 15; i++) {
        struct IrisSprite *obj = Func_0815b290(16, 16, 0x40000000, 0xf000);

        objects[i] = obj;
        ((struct IrisSpriteFlags *)obj)->priority = 1;
        sparks->tiles[i] = obj->tile;
        sparks->frames[i] = i;
        Dma_Set(work->sheet + i * 128, (void *)OBJ_TILES(obj), 0x84000020,
                REG_DMA3);
      }
      for (; i != 36; i++) {
        struct IrisSprite *obj = Func_0815b3b0(16, 16, 0x40000000, 0xf000);

        objects[i] = obj;
        Dma_Set(objects[0], obj, 0x84000006, REG_DMA3);
        sparks->frames[i] = Random16() % 15;
        objects[i]->tile = sparks->tiles[sparks->frames[i]];
      }
      for (i = 0; i != 18; i++) {
        sparks->x[i] = (Random16() % 15 << 16) + 0x390000;
        sparks->y[i] = (Random16() % 15 << 16) + 0x4f0000;
        sparks->vx[i] = ((Random16() & 63) - 32) << 12;
        sparks->vy[i] = ((Random16() & 63) - 32) << 12;
      }
      for (; i != 36; i++) {
        sparks->x[i] = (Random16() % 15 << 16) + 0xa90000;
        sparks->y[i] = (Random16() % 15 << 16) + 0x4f0000;
        sparks->vx[i] = ((Random16() & 63) - 32) << 12;
        sparks->vy[i] = ((Random16() & 63) - 32) << 12;
      }
      Resource_LoadAndDecompress(0x134, work->sheet, 0, 0);
      for (i = 0; i != 6; i++) {
        work->particles[i].x = (Random16() % 15 << 16) + 0x210000;
        work->particles[i].y = (Random16() % 15 << 16) + 0x390000;
        work->particles[i].velocityX = -(Random16() & 31) << 12;
      }
      for (; i != 12; i++) {
        work->particles[i].x = (Random16() % 15 << 16) + 0x510000;
        work->particles[i].y = (Random16() % 15 << 16) + 0x390000;
        work->particles[i].velocityX = (Random16() & 31) << 12;
      }
      work->transferMode = 2;
      work->transferValue = 70;
    }
    if ((u32)(frame - 177) <= 216) {
      scale = Data_08196f48;
      pos[3] = 0;
      pos[1] = 0xff0000;
      lift = 0;
      if (frame > 350) {
        u16 *pal = (u16 *)(BG_PLTT + 0x200);

        for (i = 0; i != 255; i++) {
          s32 r = pal[i] & 31;
          s32 g = pal[i] >> 5 & 31;
          s32 b = pal[i] >> 10 & 31;

          if (r != 31)
            r++;
          if (g != 31)
            g++;
          if (b != 31)
            b++;
          pal[i] = b << 10 | g << 5 | r;
        }
      }
      if (frame <= 385) {
        if ((u32)(frame - 351) <= 14)
          lift = Trig_Sin((frame - 350) << 10) << 6;
        else if (frame > 365)
          lift = ((366 - frame) * 5 << 17) + 0x400000;
        if (frame <= 229)
          lift = (Trig_Sin((frame << 6) - 0x2c00) << 6) - 0x147200;
        for (i = 0; i != 4; i++) {
          for (j = 0; j != 4; j++) {
            pos[0] = 0x480000 + j * 0x20000;
            pos[2] = lift + 0x380000 + i * 0x20000;
            Render_ApplyProjectedPlacementFar(work->objects[i * 4 + j], pos,
                                              (s32 *)&scale, 0);
          }
        }
      }
      if (frame == 386) {
        for (i = 0; i != 16; i++)
          ResourceObject_ReleaseFar(work->objects[i]);
        BattleEffect_SelectLivingTargets(work->effect);
        Palette_LoadBasicObjColors();
        for (i = 0; i != work->effect->count; i++)
          Func_0814cd48(work->effect->actors[i], -1, 5, -1, 0);
      }
      if (frame <= 191) {
        for (k = 0; k != 3; k++) {
          u8 v = frame + (k & 1) * 47 + 80;

          y = k + 62;
          off = ((y & 7) << 3) + ((y >> 3) << 10);
          for (x = 0; x != 128; x += 8) {
            for (m = x; m != x + 8; m++) {
              if (canvas[off] < v)
                canvas[off] = v;
              off++;
            }
            off += 56;
          }
        }
      }
    }
    if ((u32)(frame - 177) <= 46) {
      for (i = 0; i != 36; i++) {
        shrink.x = shrink.y = 0x10000 - ((frame - 177) >> 1 << 11);
        pos[3] = 0;
        pos[1] = 0xff0000;
        pos[0] = sparks->x[i];
        pos[2] = sparks->y[i];
        Render_ApplyProjectedPlacementFar(objects[i], pos, (s32 *)&shrink, 0);
        sparks->x[i] += sparks->vx[i];
        sparks->y[i] += sparks->vy[i];
        if (!(frame & 3))
          sparks->frames[i]++;
        if (sparks->frames[i] > 14)
          sparks->frames[i] = 0;
        objects[i]->tile = sparks->tiles[sparks->frames[i]];
      }
    }
    if ((u32)(frame - 179) <= 38) {
      s32 radius = frame - 170;

      for (i = 0; i != 256; i++) {
        s32 a = i << 8;
        s32 px = radius * Trig_Sin(a);
        s32 py = radius * Trig_Cos(a);

        draw[0](canvas, work->sheet + gData_08197410[3], (px >> 16) + 58,
                (py * 2 >> 16) + 48, 4, 8);
      }
      for (i = 0; i != 12; i++) {
        draw[0](canvas, work->sheet + gData_08197410[1],
                (work->particles[i].x >> 16) - 2,
                (work->particles[i].y >> 16) - 2, 2, 4);
        work->particles[i].x += work->particles[i].velocityX;
      }
    }
    if ((u32)(frame - 179) <= 31)
      REG_BLDALPHA = (210 - frame) >> 1 | 0x1000;

    // The drops: their frames, the sparks' cells and the spray.
    if (frame == 230) {
      REG_BLDALPHA = 0x1010;
      REG_BG2CNT = 0x784;
      for (k = 1; k != 8; k++) {
        for (j = 0; j != 10; j++) {
          u8 *src = work->sheet + gData_08197410[j];
          u8 *out = work->sheet + gData_08197410[j] + k * 0x302;
          s32 n = (j + 1) * 2 * (j + 1);

          for (m = 0; m != n; m++) {
            u8 v = src[m] - k * 8;

            if (v > 63)
              v = 0;
            out[m] = v;
          }
        }
      }
      for (i = 0; i != 8; i++) {
        Func_0815b510(work->sheet + 0x1b12 + i * 800, 20, 0x10000 - i * 0x2000);
        Func_0815b510(work->sheet + 0x4c12 + i * 288, 12, 0x10000 - i * 0x2000);
      }
      for (i = 0; i != 36; i++)
        sparks->life[i] = 0;
      Resource_LoadAndDecompress(0x134, sprites, 0, 0);
      for (i = 0; i != 32; i++)
        for (j = 0; j != 8; j++)
          sprites[508 + i * 8 + j] = Data_0819a038[j];
      dst = sprites;
      for (i = 0; i != 2; i++) {
        for (j = 0; j != 3; j++) {
          u8 *src = work->sheet + gData_08197410[Data_0819a040[i] - 1];

          for (m = 0; m != Data_0819a040[i] * Data_0819a040[i] * 2; m++) {
            s32 v = src[m] - j * 16;

            if (v < 0)
              v = 0;
            *dst++ = v;
          }
        }
      }
      for (j = 0; j != 8; j++) {
        u8 *src = work->sheet + gData_08197410[1];

        for (m = 0; m != 8; m++) {
          s32 v = src[m] - j * 8;

          if (v < 0)
            v = 0;
          *dst++ = v;
        }
      }
    }
    BattleEffect_LoadBlitter(SLOT_ALT, 3);
    if ((u32)(frame - 240) <= 127 && !(frame & 15)) {
      s32 v = (240 - frame) / 16;

      Func_08164b2c(v, v, v);
    }
    for (k = 0; k != 6; k++) {
      if (frame > Data_0819a048[k]) {
        if (frame < Data_0819a054[k]) {
          Iris_DrawSplash(frame - Data_0819a048[k], Data_0819a060[k],
                          Data_0819a066[k], Data_0819a06c[k]);
          if (Data_0819a048[k] - Data_0819a06c[k] * 16 + 48 == frame) {
            for (j = 0; j != 6; j++) {
              s32 s = FindFreeSpark();

              sparks->life[s] = 0x7900;
              sparks->x[s] = Data_0819a060[k] << 17;
              sparks->y[s] = (Data_0819a066[k] << 16) + 0x200000;
              sparks->vx[s] = ((Random16() & 31) - 16) << 12;
              sparks->vy[s] = -(Random16() & 31) << 12;
            }
          }
        } else if (frame < Data_0819a054[k] + Data_0819a078[k]) {
          s32 t = frame - Data_0819a054[k];

          Func_08192648(t / Data_0819a07e[k], Data_0819a060[k],
                        Data_0819a066[k] - Data_0819a072[k] * t / 2,
                        Data_0819a06c[k]);
        }
      }
    }
    if ((u32)(frame - 279) <= 132 || frame > 446)
      UpdateSparks();
    Runtime_ReleaseHeapBlock(SLOT_ALT);

    // The pillars of light.
    if (frame == 370) {
      Func_0815b434(work->sheet + 0x3e8, 16, 0x10000);
      for (i = 0; i != 16; i++) {
        Dma_Set(work->sheet + 0x3e8 + i * 8, work->sheet + 0x302 + i * 16,
                0x80000008, REG_DMA3);
        for (j = 0; j != 8; j++)
          work->sheet[0x30a + i * 16 + j] = work->sheet[0x3ef + i * 8 - j];
      }
      REG_BLDALPHA = 0x1008;
    }
    if ((u32)(frame - 414) <= 7)
      REG_BLDALPHA = (frame - 406) | 0x1000;
    if (frame > 385)
      gCameraSceneParameters[4] = 2556 - frame * 6;
    if (frame <= 422) {
      if (frame > 370) {
        s32 h = (frame - 370) * 8;

        if (h > 127)
          h = 127;
        Func_0819273c(3, 34, h);
      }
      if (frame > 375) {
        struct Widths5 widths = Data_08196f50;
        s32 h = (frame - 375) * 8;

        if (h > 127)
          h = 127;
        Func_0819273c(widths.w[h >> 5], (h >> 5) * 2 + 94, h);
      }
      if (frame > 380) {
        struct Widths7 widths = Data_08196f64;
        s32 h = (frame - 380) * 4;
        s32 q;

        if (h > 127)
          h = 127;
        q = h / 19;
        Func_0819273c(widths.w[q], q + 12, h);
      }
      if (frame > 382) {
        struct Widths7 widths = Data_08196f80;
        s32 h = (frame - 382) * 8;
        s32 q;

        if (h > 127)
          h = 127;
        q = h / 21;
        Func_0819273c(widths.w[q], q * 2 + 70, h);
      }
      if (frame > 385) {
        s32 h = (frame - 385) * 4;

        if (h > 127)
          h = 127;
        Func_0819273c(14, (h >> 3) + 48, h);
      }
      if (frame == 422) {
        work->transferMode = 0;
        work->transferValue = 0;
        REG_BLDALPHA = 0x1010;
      }
    }
    if ((u32)(frame - 423) <= 19) {
      top = 120 - (frame - 422) * 8;
      for (y = top; y != top + 40 && y <= 127; y++) {
        u8 v = (y - top) * 2;

        if (y >= 0) {
          if (v > 63)
            v = 63;
          off = ((y & 7) << 3) + ((y >> 3) << 10);
          for (x = 0; x != 128; x += 8) {
            for (m = 0; m != 8; m++) {
              if (canvas[off] < v)
                canvas[off] = v;
              off++;
            }
            off += 56;
          }
        }
      }
    }
    if ((u32)(frame - 401) <= 30 && (frame & 1)) {
      Resource_GetTableEntry(0x45);
      for (i = 0; i != 128; i++) {
        u16 c = FADE_PLTT[i];
        s32 r = c & 31;
        s32 g = c >> 5 & 31;
        s32 b = c >> 10 & 31;

        if (r > 3)
          r -= 2;
        if (g > 3)
          g -= 2;
        if (b > 3)
          b -= 2;
        FADE_PLTT[i] = b << 10 | g << 5 | r;
      }
    }

    // The fountain: the second backdrop, the motes and the light shafts.
    if (frame == 446) {
      Iwram_FillWords(canvas, 0x4000, 0x3f3f3f3f);
      Render_ResetTransformState();
      BattleBackground_LoadFar(1, 0x77, 0);
      for (i = 0; i != 128; i++) {
        u16 c = FADE_PLTT[i];
        s32 r = c & 31;
        s32 g = c >> 5 & 31;
        s32 b = c >> 10 & 31;

        FADE_PLTT[i] = (b + 1) << 10 | (g + 1) << 5 | (r + 1);
      }
      Graphics_LoadGreyRamp();
      for (i = 0, off = 0x302; i != 6; i += 2) {
        Func_0815b434(work->sheet + off, i + 14, 0x10000);
        off += (i + 14) * (i + 14) * 2;
      }
      Iwram_FillWords(work->sheet + 0x1810, 0x1000, 0);
      BattleEffect_BuildSteepShadeRampWave(work->sheet + 0x2010, 16, 64, 0x10000);
      BattleEffect_BuildSteepShadeRampWave(work->sheet + 0x2810, 16, 64, 0x10000);
      dst = work->sheet + 0x2010;
      for (i = 0; i != 64; i++) {
        for (j = 0; j != 32; j++) {
          if (*dst > i)
            *dst = i;
          dst++;
        }
      }
      for (i = 0; i != 36; i++)
        sparks->life[i] = 0;
      for (i = 0; i != 32; i++)
        work->particles[i].variant = 0;
      for (k = 1; k != 8; k++) {
        for (j = 0; j != 10; j++) {
          u8 *src = work->sheet + gData_08197410[j];
          u8 *out = work->sheet + gData_08197410[j] + k * 0x302;
          s32 n = (j + 1) * 2 * (j + 1);

          for (m = 0; m != n; m++) {
            u8 v = src[m] - k * 8;

            if (v > 63)
              v = 0;
            out[m] = v;
          }
        }
      }
      work->transferMode = 2;
      work->transferValue = 70;
    }
    if ((u32)(frame - 447) <= 15) {
      top = 136 - (frame - 446) * 8;
      for (y = top; y != 0; y--) {
        u8 v = (top - y) * 2;

        if (y <= 127) {
          if (v > 63)
            v = 63;
          if (y > top - 8)
            v = 0;
          off = ((y & 7) << 3) + ((y >> 3) << 10);
          for (x = 0; x != 128; x += 8) {
            for (m = 0; m != 8; m++) {
              if (canvas[off] < v)
                canvas[off] = v;
              off++;
            }
            off += 56;
          }
        }
      }
    }
    if (frame > 446)
      DrawParticles();
    if ((u32)(frame - 447) <= 194) {
      void *verts = Runtime_BumpAllocateAlternatePool(80);
      struct RenderModel *model = Render_AllocateModelList(1);
      s32 scroll;

      texture.logWidth = 5;
      texture.logHeight = 6;
      SceneTransform_ResetMatrix();
      model->kind = 4;
      model->mesh = Data_0819a0d4;
      model->vertices = verts;
      if (frame <= 563) {
        scroll = 0x1000;
        if (frame > 500) {
          scroll = (s16)(0x1000 - (frame - 500) * 64);
          if (scroll < 0)
            scroll = 0;
        }
        texture.pixels = work->sheet + 0x1810 + scroll;
        if (!(frame & 7)) {
          struct Spans4 spans = Data_08196f9c;
          s32 s = FindFreeSpark();

          if (s != -1) {
            s32 r = Random16() & 127;

            sparks->life[s] = -0x7c00;
            sparks->x[s] =
                (spans.p[(r + 1) >> 5].a + Random16() % spans.p[(r + 1) >> 5].b)
                << 16;
            sparks->y[s] = (r << 16) + 0x100000;
            sparks->vx[s] = (Random16() & 15) * 3 << 11;
            sparks->vy[s] = -0x8000 - ((Random16() & 31) * 3 << 11);
          }
        }
        model->texture = &texture;
        Render_TransformVertices(Data_0819a084, verts, 10);
        Render_DrawModelList(model);
      }
      if (frame <= 563) {
        scroll = 0x1000;
        if (frame > 470) {
          scroll = (s16)(0x1000 - (frame - 470) * 64);
          if (scroll < 0)
            scroll = 0;
        }
        texture.pixels = work->sheet + 0x1810 + scroll;
        if (!(frame & 7) && frame <= 501) {
          struct Spans4 spans = Data_08196fbc;
          s32 s = FindFreeSpark();

          if (s != -1) {
            s32 r = Random16() & 127;

            sparks->life[s] = -0x7c00;
            sparks->x[s] =
                (spans.p[(r + 1) >> 5].a + Random16() % spans.p[(r + 1) >> 5].b)
                << 16;
            sparks->y[s] = (r << 16) + 0x100000;
            sparks->vx[s] = (Random16() & 15) * 3 << 11;
            sparks->vy[s] = -0x8000 - ((Random16() & 31) * 3 << 11);
          }
        }
        if (!(frame & 3)) {
          s32 n = FindFreeParticle();

          if (n != -1) {
            work->particles[n].x = 0x200000;
            if (frame <= 509)
              work->particles[n].x += (Random16() & 63) << 16;
            else
              work->particles[n].x += (Random16() & 31) << 16;
            work->particles[n].y = 0x7f0000;
            work->particles[n].velocityX = (Random16() & 15) << 12;
            work->particles[n].velocityY =
                -0x8000 - ((Random16() & 31) * 7 << 11);
            work->particles[n].variant = (Random16() & 3) + 1;
          }
        }
        model->texture = &texture;
        Render_TransformVertices(Data_0819a0ac, verts, 10);
        Render_DrawModelList(model);
      }
      MemFree(model);
      MemFree(verts);
    }

    // The five light bars sweeping in from the edges.
    if ((u32)(frame - 513) <= 209) {
      s16 shade = 723 - frame;

      if (shade < 0 || shade > 63)
        shade = 63;
      BattleEffect_DrawDiagonalCanvasLine(0, 127, frame - 512, shade);
    }
    if ((u32)(frame - 522) <= 209) {
      s16 shade = 732 - frame;

      if (shade < 0 || shade > 63)
        shade = 63;
      BattleEffect_DrawDiagonalCanvasLine(0,
                    (Iwram_MulQ16((frame - 521) << 16, 0x400) + 0x7b0000) >> 16,
                    frame - 521, shade);
    }
    if ((u32)(frame - 531) <= 209) {
      s16 shade = 741 - frame;

      if (shade < 0 || shade > 63)
        shade = 63;
      BattleEffect_DrawDiagonalCanvasLine((0x40000 - Iwram_MulQ16((frame - 530) << 16, 0x400)) >> 16,
                    127, frame - 530, shade);
    }
    if ((u32)(frame - 535) <= 209) {
      s16 shade = 745 - frame;

      if (shade < 0 || shade > 63)
        shade = 63;
      BattleEffect_DrawDiagonalCanvasLine(0,
                    (Iwram_MulQ16((frame - 534) << 16, 0x600) + 0x780000) >> 16,
                    frame - 534, shade);
    }
    if ((u32)(frame - 554) <= 209) {
      s16 shade = 764 - frame;

      if (shade < 0 || shade > 63)
        shade = 63;
      BattleEffect_DrawDiagonalCanvasLine((0x60000 - Iwram_MulQ16((frame - 553) << 16, 0x600)) >> 16,
                    127, frame - 553, shade);
    }

    // The healing: rings turning about the centre, then the great glow.
    if ((u32)(frame - 639) <= 110) {
      s32 sinT = Trig_Sin(0x7800);
      s32 cosT = Trig_Cos(0x7800);

      for (k = 0; k != 80; k += 8) {
        if (frame > 638 + k && frame < 670 + k) {
          s32 t = frame - k - 638;
          s32 ry = t >> 1;
          s32 tile = gData_08197410[0] + t / 4 * 0x302;
          s32 rx = t >> 2;
          s32 shift = k >> 1;

          for (j = 0; j != 128; j++) {
            s32 a = j << 9;
            s32 sx = rx * Trig_Sin(a);
            s32 sy = ry * Trig_Cos(a) * 2;
            s32 px = Iwram_MulQ16(sx, cosT) - Iwram_MulQ16(sy, sinT);
            s32 py = Iwram_MulQ16(sx, sinT) + Iwram_MulQ16(sy, cosT);

            draw[0](canvas, work->sheet + tile, (px >> 16) + shift + 64,
                    (py >> 16) - shift + 63, 1, 2);
          }
        }
      }
    }
    if ((u32)(frame - 755) <= 2 || frame == 781)
      Iwram_FillWords(canvas, 0x4000, 0x3f3f3f3f);
    if (frame == 754) {
      BattleEffect_BuildSteepShadeRampWave(work->sheet + 0x5810, 16, 32, 0x10000);
      Func_0815b434(work->sheet + 0x1c10, 32, 0x10000);
      for (i = 0; i != 32; i++) {
        Dma_Set(work->sheet + 0x1c10 + i * 16, work->sheet + 0x1810 + i * 32,
                0x84000008, REG_DMA3);
        for (j = 0; j != 16; j++)
          work->sheet[0x1820 + i * 32 + j] = work->sheet[0x181f + i * 32 - j];
      }
      for (i = 0; i != 32; i++)
        Dma_Set(work->sheet + 0x1bf0 - i * 32, work->sheet + 0x1c10 + i * 32,
                0x84000010, REG_DMA3);
      for (k = 1; k != 8; k++) {
        u8 *src = work->sheet + 0x1810;
        u8 *out = work->sheet + 0x1810 + k * 0x800;
        u8 v = 64 - k * 8;

        for (m = 0; m != 0x800; m++) {
          if (v < src[m])
            out[m] = v;
          else
            out[m] = src[m];
        }
      }
    }
    if ((u32)(frame - 759) <= 30) {
      s32 sinT = Trig_Sin(0x5800);
      s32 cosT = Trig_Cos(0x5800);
      s32 t = frame - 758;
      s32 tile = gData_08197410[5] + t / 4 * 0x302;
      s32 rx = t * 2;

      for (j = 0; j != 96; j++) {
        s32 a = j * 384;
        s32 sx = rx * Trig_Sin(a);
        s32 sy = t * Trig_Cos(a);
        s32 px = Iwram_MulQ16(sx, cosT) - Iwram_MulQ16(sy, sinT);
        s32 py = Iwram_MulQ16(sx, sinT) + Iwram_MulQ16(sy, cosT);

        draw[0](canvas, work->sheet + tile, (px >> 16) + 101, (py >> 16) + 26,
                6, 12);
      }
    }
    if ((u32)(frame - 783) <= 46) {
      s32 sinT = Trig_Sin(0xe800);
      s32 cosT = Trig_Cos(0xe000);
      s32 t = frame - 782;
      s32 tile = gData_08197410[5] + t / 6 * 0x302;
      s32 rx = t >> 1;
      s32 ry = t * 2;

      for (j = 0; j != 197; j++) {
        s32 a = j * 384;
        s32 sx = rx * Trig_Sin(a);
        s32 sy = ry * Trig_Cos(a);
        s32 px = Iwram_MulQ16(sx, cosT) - Iwram_MulQ16(sy, sinT);
        s32 py = Iwram_MulQ16(sx, sinT) + Iwram_MulQ16(sy, cosT);

        draw[0](canvas, work->sheet + tile, (px >> 16) + 93, (py >> 16) + 32, 6,
                12);
      }
    }
    if (frame > 758) {
      void *verts = Runtime_BumpAllocateAlternatePool(64);
      struct RenderModel *model = Render_AllocateModelList(1);
      s32 place[3];
      s32 grow[3];
      s32 s;

      texture2.logWidth = 5;
      texture2.logHeight = 5;
      texture2.pixels = work->sheet + 0x5810;
      SceneTransform_ResetMatrix();
      place[0] = 0x200000;
      place[1] = -0x180000;
      place[2] = 0;
      SceneTransform_ApplyPosition(place);
      Func_080150e4(0x9800);
      grow[0] = 0x10000;
      grow[1] = (frame - 758) << 10;
      if (grow[1] > 0x10000)
        grow[1] = 0x10000;
      grow[2] = 0;
      Func_080151ac(grow);
      model->kind = 4;
      model->mesh = Data_0819a160;
      model->texture = &texture2;
      model->vertices = verts;
      Render_TransformVertices(Data_0819a140, verts, 8);
      Render_DrawModelList(model);
      MemFree(model);
      MemFree(verts);
      s = FindFreeSpark();
      if (s != -1) {
        sparks->life[s] = (Random16() & 15) << 15;
        sparks->x[s] = 0x770000;
        sparks->y[s] = (Random16() & 255) << 16;
        sparks->vx[s] = -(Random16() & 31) << 12;
        sparks->vy[s] = ((Random16() & 31) - 16) << 12;
      }
      if (!(frame & 3)) {
        s32 n = FindFreeParticle();

        if (n != -1) {
          s32 spin = Random16() & 3;

          work->particles[n].x = 0x600000;
          work->particles[n].y = ((Random16() & 7) << 16) + 0x480000;
          work->particles[n].z = frame;
          work->particles[n].velocityX =
              Iwram_MulQ16((spin + 2) << 15, Trig_Sin(0x9800));
          work->particles[n].velocityY =
              -Iwram_MulQ16((spin + 2) << 15, Trig_Cos(0x9800));
          work->particles[n].variant = (Random16() & 3) | 16;
        }
        n = FindFreeParticle();
        if (n != -1) {
          work->particles[n].x = ((Random16() & 31) << 16) + 0x600000;
          work->particles[n].y = (Random16() & 31) << 16;
          work->particles[n].z = frame;
          work->particles[n].velocityX = -((Random16() & 15) << 13);
          work->particles[n].velocityY = (Random16() & 15) << 14;
          work->particles[n].velocityZ = 0x8000;
          work->particles[n].variant = ((Random16() & 3) + 1) | 0x110;
        }
      }
    }
    if (frame > 782) {
      if (frame & 2)
        gBgScroll[1].y = gBgScroll[1].y + (frame & 1) + 2;
      else
        gBgScroll[1].y = gBgScroll[1].y - (frame & 1) - 2;
    }
    if (frame > 816) {
      for (i = 0; i != 128; i++) {
        u16 c = FADE_PLTT[i];
        s32 r = c & 31;
        s32 g = c >> 5 & 31;
        s32 b = c >> 10 & 31;

        if (r <= 30)
          r++;
        if (g <= 30)
          g++;
        if (b <= 30)
          b++;
        FADE_PLTT[i] = b << 10 | g << 5 | r;
      }
    }
    frame++;
    work->transferPending = 1;
    WaitFrames(1);
  }

  camera[27] = savedZoom;
  Runtime_ReleaseHeapBlock(SLOT_BLITTER);
  Func_08191cc4();
  Iwram_FillWords(canvas, 0x4000, 0);
  while (*(volatile u32 *)(0x040000d4 + 8) & 0x80000000) {
  }
  work->transferPending = 1;
  WaitFrames(1);
  REG_BG2PA = 0x80;
  *(volatile u32 *)0x04000028 = 0;
  gBgScroll[1].x = savedScroll;
  for (i = 0; i != 36; i++)
    ResourceObject_ReleaseFar(objects[i]);
  Scheduler_RemoveCallback(BattleEffect_ProcessPendingGraphicsTransfer);
  BattleEffect_EndCanvasLayer();
}
