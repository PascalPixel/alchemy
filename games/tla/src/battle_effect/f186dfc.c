#include "types.h"
#include "battle_effect.h"
#include "battle_effect_work.h"
#include "effect_step.h"
#include "io_reg.h"
#include "iwram_call.h"
#include "heap_release.h"
#include "ram_buffer.h"
#include "field_sprite.h"

struct FxPartyList;
struct Model;
struct StripQuad;

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
void Render_ResetTransformState(void);
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
void Func_08118088(s32 actor, s32 motion); // BattleMotion_ApplyVariantMotionFar
void Func_081180e8(s32 phase);             // BattlePlayback_StartFar
void BattleCamera_Shake(s32 rangeX, u32 rangeY);
void BattleEffectKernels_IntegrateVector2(struct EffectStep *step, s32 gravity,
                                          s32 drag);

void *Resource_GetTableEntry(s32 resource);

void Sound_PlayFar(s32 cue);
void wait(s32 frames);
u32 Random16(void);
s32 Trig_Sin(s32 angle);
s32 Trig_Cos(s32 angle);

void Render_BuildQuadStrip(struct StripQuad *quad, s32 width, s32 height,
                           s32 count);
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

#define BLITTER(mode, flags) ((mode) << 4 | (flags))
#define HI(v) (((s16 *)&(v))[1])

#include "bg_scroll.h"
#include "input.h"
#include "battle.h"
#include "motobj.h"
#include "resource.h"
#include "task.h"
struct ResourceTableEntry {
  u16 unk0;
  u16 tile;
};

struct WordPair {
  s32 a;
  s32 b;
};

struct HalfPair {
  u16 x;
  u16 y;
};

extern struct ResourceTableEntry gResourceTableEntries[];
struct WingVertex {
  s8 x;
  s8 y;
  s8 z;
  s8 unused;
  s8 x2;
  s8 y2;
  s8 z2;
  s8 unused2;
};

extern u8 gMapCellBuffer[];
#define SCRATCH (Ram_MapCellBuffer + 0x4000)
extern u8 Data_02010ae8[];
extern u8 Data_02010be0[];
extern u8 Data_02010fa0[];
extern struct WingVertex Data_020104f0[];
extern u32 gCameraSceneParameters[];
extern const struct WordPair Data_08196f00;
extern const struct WordPair Data_08196f08;
extern const struct WordPair Data_08196f10;
extern u16 gData_08197410[];
extern u8 gData_08197467[];
extern u8 gData_0819746d[];
extern u8 gData_08197473[];
extern u16 gData_0819747a[];
extern const u8 Data_081991e0[];
extern const u8 gData_08199210[];
extern const u8 Data_08199244[];
extern const u8 Data_081992f8[];
extern const u8 gData_08199340[];
extern const u8 gData_08199364[];
extern const u8 Data_08199388[];
extern const struct HalfPair Data_08199a12[];
extern const u8 Data_08199a3e[];
extern s32 Data_08199a44[];
extern const u8 Data_08199a4c[];
extern u8 Data_08199bec[];
extern const u8 Data_08199bf9[];
extern const u8 Data_08199d2f[];

// Resource ids, named after their places in the resource directory.
extern char ResourceId_Data0F7;
extern char ResourceId_EmberStreakSheet;
extern char ResourceId_Data0B4;
extern char ResourceId_MossCaveBackdrop;
extern char ResourceId_BlueArcSheetB;
extern char ResourceId_MarsDjinnSheet;
extern char ResourceId_OrangePaletteB;
extern char ResourceId_SmokeBandSheet;
extern char ResourceId_Data134;

void Animation_ApplyChildArgumentFar(void *object, s32 argument);
struct SpriteObject *Func_0815b290(s32 width, s32 height, u32 shape,
                                   u32 attributes);
void BattleEffect_ConfigureEffectDisplay(void);
void Func_08163c2c(s32 a, s32 b);
void Func_0814cca8(void); // BattleEffect_SetupBlendedDisplay
void BattleBackground_LoadFar(s32 layer, s32 background, s32 palette);
void Func_08164abc(s32 r, s32 g, s32 b);
void Func_08164b2c(s32 r, s32 g, s32 b);
s32 Func_08196404(s32 slot, s32 widthShift, s32 heightShift, s32 flags,
                  u32 mode);
void BattleEffect_ProcessPendingStampTransfer(
    void); // copies the 0x6780 canvas to the stamp's tiles

#define SPARKS ((struct EffectStep *)SCRATCH)
#define SLOT_ALT 188 // unkb8[1]
#define STAMP_TILES(work) (*(u32 *)((u8 *)(work) + 0x7b74))
#define STAMP_CANVAS(work) ((u8 *)(work) + 0x6780)
#define HALF_HI(v) (((s16 *)&(v))[1])

static inline s32 ClearWords(void *destination, s32 size) {
  /* FAKEMATCH(wrapper): the buffer clears through this inline; as direct
   * calls to Iwram_ClearWords, 4 of 2667 instructions of
   * BattleEffect_RunHaures differ in tla-en. */
  return Iwram_ClearWords(destination, size);
}
static __inline__ void PLACE(s32 x, s32 y) {
  Func_08015160(x << 16, y << 16, 0);
}
/// Haures: shadowed wings unfold over a dark pool, the demon climbs out of a
/// vortex and dives through the foes, scattering into black feathers.
void BattleEffect_RunHaures(struct BattleEffectArgument *effect) {
  u8 *battle;
  u8 *canvas;
  struct BattleEffectWork *work;
  DrawRectangle draw[2];
  s32 frame;
  u8 *sprites;
  s32 *transition;
  struct WordPair position;
  struct WordPair velocity;
  s32 morph;
  s32 morphStep;
  s32 dived;
  s32 i;
  s32 place[4];
  s32 place2[4];
  struct WordPair req;
  struct WordPair req2;
  struct WordPair req3;
  struct ModelTexture texture;
  struct ModelTexture texture2;
  struct ModelTexture texture3;
  struct ModelTexture texture4;
  struct FieldSprite *sprite;
  const u8 *q2;
  u8 *buffer;
  u8 *shades;
  u16 *dst;

  buffer = gMapCellBuffer;
  shades = Data_02010fa0;
  battle = Ram_HeapSlots->battleWork;
  canvas = Ram_HeapSlots->battleFxWork[1];
  work = (struct BattleEffectWork *)Ram_HeapSlots->battleFxWork[0];
  sprites = Ram_HeapSlots->unk64[0];
  transition = (s32 *)Ram_HeapSlots->transitionWork;
  BattleEffect_StartCanvasLayer(0);
  BattleEffect_ConfigureEffectDisplay();
  BG_PLTT[0] = 0;
  BG_PLTT[1] = 0;
  work->transferMode = 0;
  Task_Add(BattleEffect_ProcessPendingGraphicsTransfer, 3200);
  Func_08163c2c(1, 0);
  REG_BLDCNT = 0x3f40;
  REG_BG2X = 0x400;
  gCameraSceneParameters[4] = 240;
  BattleEffect_SelectLivingTargets(work->effect);
  transition[4] = 1;
  gBgScroll[1].x = 0;
  work->transferMode = 1;
  work->transferValue = 0;
  Task_Add(BattleEffect_ProcessPendingStampTransfer, 3200);
  morph = 0;
  morphStep = 0;
  dived = 0;
  BattleEffect_SpawnObjects(2, 0x293, 2);
  for (i = 0; i != 4; i++) {
    sprite = ResourceObject_CreateFar(0x294);
    work->objects[12 + i] = sprite;
    if (sprite != NULL) {
      sprite->nextFrame = 0;
      Animation_ApplyChildArgumentFar(sprite, 0);
      {
        struct FieldSprite *added = work->objects[12 + i];

        added->priority = 1;
        added->blendMode = 1;
      }
    }
  }
  for (i = 0; i != 2; i++) {
    sprite = ResourceObject_CreateFar(0x295);
    work->objects[16 + i] = sprite;
    if (sprite != NULL) {
      sprite->nextFrame = 0;
      Animation_ApplyChildArgumentFar(sprite, 0);
      {
        struct FieldSprite *added = work->objects[16 + i];

        added->priority = 1;
      }
    }
  }
  sprite = Func_0815b290(64, 64, 0xc0002000, 0);
  ((struct FieldSprite **)work->objects)[18] = sprite;
  sprite->blendMode = 1;
  STAMP_TILES(work) =
      gResourceTableEntries[sprite->vramBlock].tile + 0x06010000;
  {
    u8 *c = work->sheet;

    Resource_LoadAndDecompress((s32)&ResourceId_Data0F7, c, 0, 0);
    Func_0816ae40(c, SCRATCH, 64, 64);
    Resource_LoadAndDecompress((s32)&ResourceId_EmberStreakSheet, c, 1, 0);
    for (i = 0; i != 0x1781; i++, c++) {
      u8 v = *c;

      if (v != 0) {
        *c = (v >> 2) + 224;
      }
    }
  }
  {
    u32 *w;
    u32 mask;

    Resource_LoadAndDecompress((s32)&ResourceId_Data0B4,
                               w = (u32 *)(work->sheet + 0x4000), 0, 0);
    mask = 0x7f7f7f7f;
    for (i = 0; i != 1024; i++) {
      w[i] = (w[i] >> 1) & mask;
    }
  }
  for (i = 0; i != 31; i++) {
    Data_02010ae8[i * 8 + 0] = i * 2;
    Data_02010ae8[i * 8 + 1] = i * 2 + 1;
    Data_02010ae8[i * 8 + 2] = i * 2 + 2;
    Data_02010ae8[i * 8 + 4] = i * 2 + 1;
    Data_02010ae8[i * 8 + 5] = i * 2 + 3;
    Data_02010ae8[i * 8 + 6] = i * 2 + 2;
  }
  Data_02010be0[0] = 0;
  Data_02010be0[1] = 0;
  Data_02010be0[2] = 0;
  {
    dst = BG_PLTT + 1;

    for (i = 1; i != 64; i++) {
      s32 b;
      s32 r;
      s32 g;

      if (i <= 31) {
        b = 0;
        g = 0;
        r = i;
        if (i < 0) {
          r = 0;
        }
        *dst = r;
      } else {
        s32 t = i - 32;

        b = t * 4;
        g = t;
        r = b;
        if (r > 31) {
          r = 31;
        }
        if (b > 31) {
          b = 31;
        }
      }
      *dst++ = b << 10 | g << 5 | r;
    }
  }
  {
    dst = &OBJ_PLTT_COLOR(14, 0);

    for (i = 0; i != 16; i++, dst++) {
      s32 b = i * 3 + 2;
      s32 r = i * 2 + 2;

      if (r > 31) {
        r = 31;
      }
      if (b > 31) {
        b = 31;
      }
      *dst = b << 10 | (i + 2) << 5 | r;
    }
  }
  BattleBackground_LoadFar(1, (s32)&ResourceId_MossCaveBackdrop, 0);
  frame = 0;
  if (!(gInput.repeat & 3)) {
    do {
      s32 cut = frame - 267;

      if (frame == 0) {
        work->transferValue = frame;
        position.a = 0x6c0000;
        position.b = 0x780000;
        velocity.a = 0;
        velocity.b = 0;
        Iwram_CopyWords((void *)&BG_PLTT_COLOR(6, 0), battle + 0x56c, 256);
        {
          u8 *q = shades;

          for (i = 0; i != 31; i++, q += 2) {
            q[0] = 1;
            q[1] = 1;
          }
        }
        q2 = Data_08199a3e;
        {
          struct EffectStep *p = work->particles;

          for (i = 0; i != 4; i++, p++, q2++) {
            p->x = *q2 << 16;
            p->y = 0x100000;
            p->velocityX = 0;
            p->velocityY = 0;
            p->variant = 0;
          }
        }
        {
          for (i = 0; i != 8; i++) {
            struct EffectStep *p = &work->particles[i + 4];
            p->x = (Random16() & 15) + 24;
            p->y = Random16() & 7;
            p->variant = i * 3;
            p->velocityX = 0;
            p->velocityY = 0;
          }
        }
        {
          struct EffectStep *p = &work->particles[63];

          p->x = 0x3c0000;
          p->y = 0x700000;
          p->variant = 0;
        }
        for (i = 0; i != 32; i++) {
          Data_020104f0[i].x = -(Trig_Cos(i * 992) << 6) >> 16;
          Data_020104f0[i].y = -(Trig_Sin(i * 992) * 18) >> 16;
          Data_020104f0[i].x2 = i * 4 - 64;
          Data_020104f0[i].z = 0;
          Data_020104f0[i].x2 = -(Trig_Cos(i * 992) << 6) >> 16;
          Data_020104f0[i].y2 = (u32)(Trig_Sin(i * 992) * 9) >> 15;
          Data_020104f0[i].z2 = 0;
        }
      }
      if ((u32)frame <= 31) {
        s32 a = frame * 2 / 3 - 35;

        Func_08164b2c(a, a, frame - 35);
      }
      if ((u32)(frame - 242) <= 21 && (frame & 1)) {
        Func_08164abc(1, 0, 0);
      }
      if (frame == 277) {
        Func_08164b2c(8, -12, -12);
      }
      if (frame > 359) {
        Func_08164b2c(8 - (frame - 360) / 4, -12, (frame - 360) / 8 - 12);
      }
      if (frame == 48) {
        Sound_PlayFar(136);
      }
      if (frame == 74) {
        Sound_PlayFar(136);
      }
      if (frame == 100) {
        Sound_PlayFar(136);
      }
      if (frame == 126) {
        Sound_PlayFar(136);
      }
      if (frame == 140) {
        Sound_PlayFar(141);
      }
      if (frame <= 266) {
        if (frame <= 639) {
          place[1] = 0;
          place[3] = 0;
          {
            struct EffectStep *p = work->particles;

            for (i = 0; i != 4; i++, p++) {
              if (frame > i * 26) {
                if (p->variant == 0) {
                  place[0] = p->x;
                  place[2] = p->y;
                  Render_ApplyProjectedPlacementFar(work->objects[16 + (i & 1)],
                                                    place, Data_08199a44, 0);
                  BattleEffectKernels_IntegrateVector2(p, 63, 0x2000);
                  if (HALF_HI(p->y) > 119) {
                    p->variant = 1;
                  }
                } else if (p->variant <= 23) {
                  place[0] = p->x;
                  place[2] = p->y;
                  Animation_ApplyChildArgumentFar(work->objects[12 + (i & 1)],
                                                  (p->variant / 6) & 3);
                  Render_ApplyProjectedPlacementFar(work->objects[12 + (i & 1)],
                                                    place, Data_08199a44, 0);
                  p->variant++;
                }
              }
            }
          }
        }
        if (frame > 46) {
          struct RenderModel *model;
          void *verts;
          s32 t;
          s32 sh;
          s32 s;
          u8 *strip = Data_02010ae8;

          verts = Heap_AllocEwram(32);
          model = Render_AllocateModelList(1);
          texture.logWidth = 6;
          t = frame - 47;
          texture.logHeight = 6;
          texture.pixels = work->sheet + 0x4000;
          sh = frame - 111;
          if (sh > 0) {
            sh = 0;
          }
          model->kind = 6;
          model->shade = sh;
          model->texture = &texture;
          model->mesh = gData_08199340;
          model->vertices = verts;
          Render_ResetTransformState();
          Func_08015160(0, 0x240000, 0);
          SceneTransform_ApplyPitch(4000);
          SceneTransform_ApplyYaw(-t << 8);
          s = t << 10;
          if (s > 60000) {
            s = 60000;
          }
          Func_0801521c(s);
          Render_TransformVertices(gData_08199210, verts, 4);
          Render_DrawModelList(model);
          texture.logWidth = 5;
          texture.logHeight = 5;
          texture.pixels = work->sheet + 0x1000;
          model->kind = 9;
          model->texture = &texture;
          model->mesh = Data_08199244;
          model->vertices = verts;
          model->shade = sh;
          Render_ResetTransformState();
          Func_08015160(0, 0x240000, 0);
          s = (t * 3 << 8) + 1024;
          if (s > 38768) {
            s = 38768;
          }
          Func_0801521c(s);
          if (frame > 73) {
            s32 a;
            s32 b;
            s32 n;
            s32 next;
            struct WingVertex *wing = Data_020104f0;

            if (frame == 74) {
              morph = 0;
              morphStep = 0;
            }
            a = morph % 13;
            next = morph + 1;
            b = next % 13;
            n = morphStep;
            morphStep = n + 1;
            if (morphStep == Data_08199bec[a]) {
              morphStep = 0;
              morph = next;
            }
            for (i = 0; i != 32; i++) {
              wing[i].y = -(
                  Data_08199a4c[i + a * 32] +
                  n * (Data_08199a4c[i + b * 32] - Data_08199a4c[i + a * 32]) /
                      Data_08199bec[a]);
            }
            for (i = 0; i != 32; i++) {
              Data_020104f0[i].x = i * 4 - 64;
            }
          }
          model->unk04 = 0;
          model->kind = 1;
          model->mesh = strip;
          model->vertices = buffer;
          if ((u32)(frame - 236) <= 31) {
            s32 v = frame - 235;

            for (i = 0; i != 31; i++) {
              Data_02010fa0[i * 2] = v;
              Data_02010fa0[i * 2 + 1] = v;
            }
          }
          model->shade = (s32)shades;
          Render_TransformVertices(Data_020104f0, buffer, 64);
          Render_DrawModelList(model);
          Heap_Release(model);
          Heap_Release(verts);
        }
        req = Data_08196f00;
        place2[3] = 0;
        if (frame > 149) {

          position.a += velocity.a;
          position.b += velocity.b;
          if (frame <= 159) {
            velocity.b += -0x4000;
          } else if ((u32)(frame - 210) <= 13) {
            velocity.a += -0x800;
            velocity.b += -0x1000;
          } else if ((u32)(frame - 236) <= 3) {
            velocity.a += 0x2000;
            velocity.b += -0x4000;
          }
          velocity.a = velocity.a * 60 / 64;
          velocity.b = velocity.b * 60 / 64;
          req.a = 0x10000;
          req.b = 0x10000;
          for (i = 0; i != 2; i++) {
            place2[0] = position.a;
            place2[1] = 0x1100000 - (position.b + i * 0x200000);
            place2[2] = 0x1000000;
            Render_ApplyProjectedPlacementFar(work->objects[i], place2,
                                              (s32 *)&req, 0);
          }
          if ((u32)(frame - 150) <= 116) {
            Func_08196404(SLOT_ALT, 6, 6, 3, 1);
            draw[1] = Ram_HeapSlots->unkb8[1];
            {
              for (i = 0; i != 8; i++) {
                struct EffectStep *p = &work->particles[i + 4];
                s32 n = (p->variant / 4) % 6;
                u8 *src = work->sheet + gData_0819747a[n];
                u32 w = gData_08197467[n];

                draw[1](STAMP_CANVAS(work), src, p->x - (w >> 1),
                        p->y + gData_08197473[n], w, gData_0819746d[n]);
                if (++p->variant == 24) {
                  p->variant = 0;
                  p->x = (Random16() & 15) + 24;
                  p->y = Random16() & 7;
                }
              }
            }
            HeapSlot_Release(SLOT_ALT);
          }
          req.a = frame * 1128 - 103664;
          if (req.a > 0x40000) {
            req.a = 0x40000;
          }
          req.b = frame * 564 - 19064;
          if (req.b > 0x20000) {
            req.b = 0x20000;
          }
          place2[0] = position.a;
          place2[1] = 0x780000 - position.b;
          place2[2] = 0x800000;
          Render_ApplyProjectedPlacementFar(work->objects[18], place2,
                                            (s32 *)&req, 0);
        }
      }
      if (frame == 267) {
        Sound_PlayFar(212);
      }
      if (frame == 271) {
        Sound_PlayFar(144);
      }
      if ((u32)cut <= 9) {
        struct RenderModel *model;
        void *verts;
        s32 s;

        if (frame == 267) {
          u16 *dst = BG_PLTT + 1;

          for (i = 1; i != 64; i++) {
            s32 v = i;

            if (v > 31) {
              v = 31;
            }
            if (v < 0) {
              v = 0;
            }
            *dst++ = v;
          }
          Resource_LoadAndDecompress((s32)&ResourceId_BlueArcSheetB,
                                     work->sheet, 1, 0);
          Iwram_CopyWords(
              (void *)BG_PLTT,
              Resource_GetTableEntry((s32)&ResourceId_MarsDjinnSheet), 128);
          work->transferMode = 1;
          work->transferValue = 0x01010101;
        }
        verts = Heap_AllocEwram(32);
        model = Render_AllocateModelList(1);
        texture2.logWidth = 7;
        texture2.logHeight = 7;
        texture2.pixels = SCRATCH;
        model->shade = 0;
        model->kind = 7;
        model->texture = &texture2;
        model->mesh = gData_08199364;
        model->vertices = verts;
        Render_ResetTransformState();
        Func_08015160(0, -0x40000, 0);
        s = 0x1d8f0;
        if (frame > 268) {
          s = (frame << 12) - 0xef710;
        }
        Func_080151e4(s >> 1, s, s >> 1);
        Render_TransformVertices(Data_081991e0, verts, 4);
        Render_DrawModelList(model);
        if ((u32)cut <= 5) {
          s32 j;

          ClearWords(buffer, 0xd80);
          for (j = 0; j != 72; j++) {
            s32 m;

            for (m = 0; m != 48; m++) {
              buffer[j * 64 + m] = work->sheet[cut * 27 * 128 + j * 48 + m];
            }
          }
          if ((u32)(frame - 267) <= 5) {
            texture2.logWidth = 6;
            texture2.logHeight = 7;
            texture2.pixels = buffer;
            model->mesh = Data_08199388;
            Render_ResetTransformState();
            Func_08015160(0, -0x40000, 0);
            Func_080151e4(0x1b1e0, 0x29c40, 0x20000);
            Render_TransformVertices(Data_081991e0, verts, 4);
            Render_DrawModelList(model);
          }
        }
        Heap_Release(model);
        Heap_Release(verts);
      }
      if (frame > 276) {
        struct RenderModel *model;
        void *verts;
        u8 *cells;
        u8 *mesh;

        if (frame == 277) {
          const u8 *q;

          work->transferMode = 1;
          work->transferValue = 0;
          dived = 1;
          ResourceObject_ReleaseFar(work->objects[0]);
          ResourceObject_ReleaseFar(work->objects[1]);
          for (i = 0; i != 4; i++) {
            ResourceObject_ReleaseFar(work->objects[12 + i]);
          }
          for (i = 0; i != 2; i++) {
            ResourceObject_ReleaseFar(work->objects[16 + i]);
          }
          ResourceObject_ReleaseFar(work->objects[18]);
          BattleEffect_SpawnObjects(11, 0x293, 2);
          q = Data_08199bf9;
          {
            struct EffectStep *p = work->particles;

            for (i = 0; i != 31; i++, q += 10, p++) {
              s32 x = q[0] >> 1;
              s32 y = q[1];

              p->x = x << 16;
              p->y = y << 16;
              {
                s32 velocityY = (60 - y) << 10;

                p->velocityX = (x - 120) << 12;
                p->velocityY = velocityY;
              }
              p->z = 0;
              p->velocityX += ((Random16() & 31) - 16) << 13;
              p->velocityY += ((Random16() & 31) - 16) << 13;
              p->velocityZ = (Random16() & 127) * 5 + 640;
              if (Random16() & 1) {
                p->velocityZ = -p->velocityZ;
              }
            }
          }
          velocity.a = 0;
        }
        gBgScroll[1].x += 0xfff4;
        req3 = Data_08196f08;
        place2[3] = 0;
        position.a = (Trig_Sin(frame << 9) << 3) + 0x6c0000;
        position.b = 0x380000 - (Trig_Cos(frame << 9) << 3);
        if (frame > 396) {
          velocity.a -= (frame - 397) << 14;
          position.a += velocity.a;
          velocity.b -= (frame - 397) << 13;
          position.b +=
              velocity.a; // velocity.a, not velocity.b, as the ROM has it
        }
        for (i = 0; i != 11; i++) {
          place2[0] = (Data_08199a12[i].x << 16) + position.a;
          place2[1] = 0x1100000 - ((Data_08199a12[i].y << 16) + position.b);
          place2[2] = 0x1000000;
          Render_ApplyProjectedPlacementFar(work->objects[i], place2,
                                            (s32 *)&req3, 0);
        }
        verts = Heap_AllocEwram(32);
        model = Render_AllocateModelList(1);
        cells = work->sheet;
        mesh = work->sheet + 0x800;
        if (frame == 277) {
          s32 none;
          s32 j;

          for (j = 0, none = 0; j != 31; j++) {
            for (i = 0; i != 4; i++) {
              s8 *dst = (s8 *)cells + j * 16 + i * 4;

              dst[2] = none;
              dst[0] =
                  Data_08199bf9[j * 10 + i * 2 + 2] - Data_08199bf9[j * 10];
              dst[1] =
                  Data_08199bf9[j * 10 + i * 2 + 3] - Data_08199bf9[j * 10 + 1];
            }
            mesh[j * 12 + 1] = 1;
            mesh[j * 12 + 2] = 2;
            mesh[j * 12 + 5] = 2;
            mesh[j * 12 + 6] = 3;
            mesh[j * 12 + 0] = none;
            mesh[j * 12 + 4] = none;
            mesh[j * 12 + 8] = none;
            mesh[j * 12 + 9] = none;
            mesh[j * 12 + 10] = none;
          }
        }
        texture3.logWidth = 8;
        texture3.logHeight = 7;
        texture3.pixels = buffer;
        model->kind = 1;
        model->texture = &texture3;
        model->vertices = verts;
        model->shade = (s32)shades;
        {
          /* FAKEMATCH(dead): a never-true test of the byte scroll, which
           * combine folds after CSE, so its label ends post-reload CSE's run
           * (the fill pointer reloads its pool word, not r0's copy) and flow2
           * merges the blocks before sched2; without it, 2 of 2667 instructions
           * differ in tla-en. */
          s32 scroll = model->scrollU;

          if (scroll > 255) {
            i = 0;
          }
        }
        for (i = 0; i != 31; i++) {
          Data_02010fa0[i] = 1;
        }
        {
          for (i = 0; i != 31; i++) {
            struct EffectStep *p = &work->particles[i];
            model->mesh = mesh + i * 12;
            if ((u32)(HALF_HI(p->x) + 16) <= 152 && (p->y >> 16) >= -16 &&
                (p->y >> 16) <= 136) {
              Render_ResetTransformState();
              Func_08015160(p->x + -0x400000, p->y + -0x400000, 0);
              Func_080151e4(0x8000, 0x10000, 0x8000);
              switch (i & 3) {
              case 0:
                SceneTransform_ApplyPitch(p->z);
                Func_080150e4(p->z);
                break;
              case 1:
                SceneTransform_ApplyPitch(p->z);
                Func_080150e4(-p->z);
                break;
              case 2:
                SceneTransform_ApplyPitch(p->z);
                SceneTransform_ApplyYaw(p->z);
                break;
              case 3:
                Func_080150e4(p->z);
                SceneTransform_ApplyYaw(-p->z);
                break;
              }
              Render_TransformVertices(cells + i * 16, verts, 4);
              Render_DrawModelList(model);
            }
            BattleEffectKernels_IntegrateVector3(p, 62, 0);
            p->x += 0x40000;
          }
        }
        Heap_Release(model);
        Heap_Release(verts);
      }
      BattleEffect_CountDownBattlerTints();
      work->transferPending = 1;
      if (frame <= 266) {
        work->unk78ec = 1;
      }
      wait(1);
      frame++;
    } while (frame != 440 && !(gInput.repeat & 3));
  }
  if (dived == 0) {
    ResourceObject_ReleaseFar(work->objects[0]);
    ResourceObject_ReleaseFar(work->objects[1]);
    for (i = 0; i != 4; i++) {
      ResourceObject_ReleaseFar(work->objects[12 + i]);
    }
    for (i = 0; i != 2; i++) {
      ResourceObject_ReleaseFar(work->objects[16 + i]);
    }
    ResourceObject_ReleaseFar(work->objects[18]);
  } else {
    for (i = 0; i != 11; i++) {
      ResourceObject_ReleaseFar(work->objects[i]);
    }
  }
  transition[4] = 0;
  Task_Remove(BattleEffect_ProcessPendingStampTransfer);
  Func_0814cca8();
  gCameraSceneParameters[4] = 120;
  BattleEffect_SpawnObjects(11, 0x293, 2);
  Iwram_CopyWords((void *)BG_PLTT,
                  Resource_GetTableEntry((s32)&ResourceId_OrangePaletteB), 128);
  Resource_LoadAndDecompress((s32)&ResourceId_BlueArcSheetB, work->sheet, 1, 0);
  {
    ClearWords(buffer, 0x6c00);
    for (i = 0; i != 6; i++) {
      s32 j;

      for (j = 0; j != 72; j++) {
        s32 m;

        for (m = 0; m != 48; m++) {
          buffer[(i * 9 * 512) + j * 64 + m] =
              work->sheet[i * 27 * 128 + j * 48 + m];
        }
      }
    }
  }
  Resource_LoadAndDecompress((s32)&ResourceId_SmokeBandSheet, work->sheet, 1,
                             0);
  Resource_LoadAndDecompress((s32)&ResourceId_Data134, sprites, 0, 0);
  work->transferMode = 2;
  work->transferValue = 50;
  REG_BLDALPHA = 0x1010;
  BattleEffect_LoadBlitter(SLOT_BLITTER, 19);
  draw[0] = Ram_HeapSlots->unk64[1];
  for (frame = 0; frame != 92; frame++) {
    struct RenderModel *model;
    void *verts;

    if (frame == 0) {
      position.a = 0x10e0000;
      position.b = -0x160000;
      velocity.a = -0xc0000;
      velocity.b = 0x60000;
      {
        s32 *variant = &SPARKS->variant;

        for (i = 0; i != 512; i++, variant += 7) {
          *variant = -1;
        }
      }
    }
    if (frame == 20) {
      position.a = -0xa0000;
      position.b = -0x160000;
      velocity.a = 0xc0000;
      velocity.b = 0x60000;
    }
    if (frame == 40) {
      position.a = 0x1040000;
      position.b = 0x400000;
      velocity.a = -0x80000;
      velocity.b = 0;
    }
    {
      for (i = 0; i != 512; i++) {
        struct EffectStep *p = &((struct EffectStep *)SCRATCH)[i];
        if (p->variant >= 0) {
          s32 n = (p->variant >> 3) + 1;

          draw[0](canvas, sprites + gData_08197410[n - 1],
                  HALF_HI(p->x) - n / 2, HALF_HI(p->y) - n, n, n * 2);
          BattleEffectKernels_IntegrateVector2(p, 63, -0x8000);
          p->variant--;
        }
      }
    }
    if (frame <= 39) {
      struct EffectStep *p;

      // The spark row keeps frame * 2 + frame: frame * 3 folds with the row's
      // size.
      for (i = 0, p = ((struct EffectStep(*)[4])SCRATCH)[frame * 2 + frame];
           i != 12; i++, p++) {
        s32 angle;
        s32 speed;

        p->x = (((Random16() & 63) - 16) << 16) + position.a / 2;
        p->y = ((Random16() & 31) << 16) + position.b;
        angle = Random16();
        speed = (Random16() & 127) + 128;
        p->velocityX = Trig_Sin(angle) * speed >> 7;
        p->velocityY = Trig_Cos(angle) * speed >> 6;
        p->variant = 16;
      }
    }
    if (frame <= 73) {
      s32 t;

      req2 = Data_08196f10;
      place2[3] = 0;
      t = frame - 20;
      if ((u32)t <= 19) {
        req2.a = -0x10001;
        for (i = 0; i != 11; i++) {
          place2[0] = position.a - (Data_08199a12[i].x << 16);
          place2[1] = 0x1100000 - ((Data_08199a12[i].y << 16) + position.b);
          place2[2] = 0x1000000;
          Render_ApplyProjectedPlacementFar(work->objects[i], place2,
                                            (s32 *)&req2, 0);
        }
      } else {
        for (i = 0; i != 11; i++) {
          place2[0] = (Data_08199a12[i].x << 16) + position.a;
          place2[1] = 0x1100000 - ((Data_08199a12[i].y << 16) + position.b);
          place2[2] = 0x1000000;
          Render_ApplyProjectedPlacementFar(work->objects[i], place2,
                                            (s32 *)&req2, 0);
        }
      }
      position.a += velocity.a;
      position.b += velocity.b;
      if ((u32)frame <= 19) {
        velocity.a += -0x8000;
        velocity.b += 0x8000;
      } else if ((u32)t <= 19) {
        velocity.a += 0x8000;
        velocity.b += 0x4000;
      } else if ((u32)(frame - 40) <= 19) {
        velocity.a += -0x8000;
      }
    }
    if (frame == 10) {
      Sound_PlayFar(212);
    }
    if (frame == 14) {
      Sound_PlayFar(144);
      for (i = 0; i != effect->count; i++) {
        BattleEffect_StartActorMotion(effect->actors[i], 1, 0x40000, 0x80000,
                                      0x40000, 150);
        Func_0814cd48(effect->actors[i], 7, 5, i, 16);
        work->shakeFrames = 8;
      }
    }
    if (frame == 30) {
      Sound_PlayFar(212);
    }
    if (frame == 34) {
      Sound_PlayFar(144);
      for (i = 0; i != effect->count; i++) {
        BattleEffect_StartActorMotion(effect->actors[i], 1, 0x40000, 0x80000,
                                      0x40000, 50);
        Func_0814cd48(effect->actors[i], 7, 5, i, 16);
        work->shakeFrames = 8;
      }
    }
    if (frame == 58) {
      Sound_PlayFar(145);
      for (i = 0; i != effect->count; i++) {
        BattleEffect_StartActorMotion(effect->actors[i], 1, 0x40000, 0x80000,
                                      0x40000, 200);
        Func_0814cd48(effect->actors[i], 7, 5, i, 16);
        work->shakeFrames = 8;
      }
    }
    verts = Heap_AllocEwram(32);
    model = Render_AllocateModelList(1);
    model->shade = -8;
    model->kind = 7;
    model->texture = &texture4;
    texture4.logWidth = 6;
    texture4.logHeight = 5;
    model->mesh = Data_081992f8;
    model->vertices = verts;
    texture4.pixels = work->sheet;
    model->scrollU = (frame * 8) & 127;
    for (i = 0; i != 2; i++) {
      s32 start = Data_08199d2f[i];

      if (frame >= start && frame < start + 16) {
        s32 amp = Trig_Sin((frame - start) << 11) / 8;
        s32 x;
        s32 m;

        for (m = 0; m != 3; m++) {
          s32 y = -4;
          x = m * 20 - 16;
          Render_ResetTransformState();
          if (i == 0) {
            Func_08015160(x << 16, y << 16, 0);
            Func_080150e4(-0x2000);
          } else {
            Func_08015160((x - 16) << 16, y << 16, 0);
            Func_080150e4(0x27d0);
          }
          Func_080151e4(0x30000, amp, 0x20000);
          Render_TransformVertices(Data_081991e0, verts, 4);
          Render_DrawModelList(model);
        }
      }
    }
    texture4.logWidth = 6;
    texture4.logHeight = 5;
    model->mesh = Data_081992f8;
    model->kind = 7;
    if ((u32)(frame - 56) <= 31) {
      s32 sh = 0;
      s32 v;
      s32 left;
      s32 right;
      s16 *corner = verts;

      if (frame > 79) {
        sh = (80 - frame) * 8;
      }
      model->shade = sh;
      model->scrollU = (frame * 8) & 127;
      texture4.pixels = work->sheet;
      if (frame <= 71) {
        v = Trig_Sin((frame << 10) + -0xe000) * 80 >> 16;
      } else if (frame > 79) {
        v = Trig_Sin((frame << 10) + -0x10000) * 80 >> 16;
      } else {
        v = 80;
      }
      left = 4;
      corner[1] = 60 - v;
      corner[5] = 60 - v;
      right = 123;
      corner[0] = left;
      corner[4] = right;
      corner[8] = left;
      corner[9] = v + 60;
      corner[12] = right;
      corner[13] = v + 60;
      Render_DrawModelList(model);
    }
    Heap_Release(model);
    Heap_Release(verts);
    BattleCamera_Shake(8, 8);
    BattleEffect_CountDownBattlerTints();
    work->transferPending = 1;
    wait(1);
  }
  for (i = 0; i != 11; i++) {
    ResourceObject_ReleaseFar(work->objects[i]);
  }
  Task_Remove(BattleEffect_ProcessPendingGraphicsTransfer);
  HeapSlot_Release(SLOT_BLITTER);
  BattleEffect_EndCanvasLayer();
}

#include "fixed_math.h"

// The two rings sit at y = -36 and y = -18.
#define FIRST_RING_Y -36
#define RING_SPACING 18

// Each side of the band shows an 8 x 16 column of the texture.
#define COLUMN_WIDTH 8
#define COLUMN_HEIGHT 16

struct MeshVertex {
  s8 x;
  s8 y;
  s8 z;
  u8 unk03;
};

struct MeshTriangle {
  u8 vertex[3];
  u8 unk03;
  u8 texCoord[3][2];
  u8 unk0a[2];
};

// Builds the mesh of an open cylinder, the band summon 20 shows: two rings of
// `segments` vertices of radius 96 around the y axis, and two textured
// triangles for each side of the band, joining the first ring (texture row 0)
// to the second (row 15). A triangle whose three vertices are 0 ends the list.
// Summon 20 builds it with 32 segments into the buffers at 0x02014400 and
// 0x02014800.
void Func_081885f0(struct MeshVertex *vertices, struct MeshTriangle *triangles,
                   s32 segments) {
  s32 ring;
  s32 i;

  for (ring = 0; ring != 2; ring++) {
    for (i = 0; i != segments; i++) {
      // The vertex: a point on the ring, 96 units out (3 * 0x10000 >> 11).
      vertices[ring * segments + i].x =
          (u32)(Trig_Sin(i * (0x10000 / segments)) * 3) >> 11;
      vertices[ring * segments + i].y = ring * RING_SPACING + FIRST_RING_Y;
      vertices[ring * segments + i].z =
          (u32)(Trig_Cos(i * (0x10000 / segments)) * 3) >> 11;

      // The side from this vertex to the next, as two triangles.
      if (ring < 1) {
        // A byte offset added to the base: as
        // &triangles[(ring * segments + i) * 2], 4 of 141 differ (base +
        // offset, where the native adds offset + base).
        struct MeshTriangle *triangle =
            (struct MeshTriangle *)((ring * segments + i) * 24 +
                                    (u32)triangles);
        s32 left = i * COLUMN_WIDTH;
        s32 right = left + COLUMN_WIDTH - 1;

        triangle[0].texCoord[0][0] = right;
        triangle[0].texCoord[0][1] = 0;
        triangle[0].texCoord[1][0] = right;
        triangle[0].texCoord[1][1] = COLUMN_HEIGHT - 1;
        triangle[0].texCoord[2][0] = left;
        triangle[0].texCoord[2][1] = COLUMN_HEIGHT - 1;
        {
          struct MeshTriangle *second = triangle + 1;

          second->texCoord[0][0] = left;
          second->texCoord[0][1] = 0;
          second->texCoord[1][0] = right;
          second->texCoord[1][1] = 0;
          second->texCoord[2][0] = left;
          second->texCoord[2][1] = COLUMN_HEIGHT - 1;

          // (next, next on ring 1, this on ring 1), then (this, next, this on
          // ring 1).
          triangle->vertex[0] = (i + 1) % segments + ring * segments;
          triangle->vertex[1] = (i + 1) % segments + segments + ring * segments;
          triangle->vertex[2] = i % segments + segments + ring * segments;
          second->vertex[0] = i % segments + ring * segments;
          second->vertex[1] = (i + 1) % segments + ring * segments;
          second->vertex[2] = i % segments + segments + ring * segments;
        }
      }
    }
  }

  // The end of the list.
  triangles[segments * 2].vertex[0] = 0;
  triangles[segments * 2].vertex[1] = 0;
  triangles[segments * 2].vertex[2] = 0;
}
