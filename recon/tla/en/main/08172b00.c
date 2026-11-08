// Matches: differ 0 (tla-en, 2026-11-08)
// Plain C, no FAKEMATCH device. The last tie was the turn: the native writes
// 2560 after frame << 5 (the order of 2560 - (frame << 5)) yet reloads frame
// into r4 instead of keeping it in r3 from the tests. That reload comes from
// the if/else with the frame <= 71 arm first (the arm order alone does it;
// a speed local copied into turn, as before, changes nothing): the else arm
// opens on a label while reload runs, so frame is reloaded fresh; jump2
// later folds the jumps into the one ble. The two tests with both arms alike
// are the ROM's and marked DEAD. resource_directory.s's
// unnamed row for BattleEffect_PetalSheet (0xc5) must become
// ResourceId_PetalSheet before it links. The
// palette copy also drops the CopyWords wrapper: a source local, then a copy
// local holding Iwram_CopyWords, loads the call address where the native does
// (the direct call measured 2 of 1216). Earlier rounds, kept as history:
// 2026-11-08: differ 2 of 1216. The turn was spelled with a block constant
// full = 2560 set before the shift; full - (frame << 5) in one expression lost
// frame's reload (33); an asm clobber of r3 (a barrier) also matched.
// 2026-11-05: differ 4, then 51 and 75 (2026-11-01). What settled the frame
// (144) and every stack slot: blit is an 8-byte struct, a DImode pseudo whose
// spill slot leaves the unused word at sp 84; target, savedYaw, caster and
// frames sit in a block opened after Func_0815585c, so the screen and focus
// pointers take their slots first; focus is declared before screen. The debris
// loop indexes work->particles[i] (p would be set before the count test), and
// the sphere writes vertex[row * 8 + k] through a struct, which keeps radius's
// spill store once. cells, projected and mesh are set once at the top.

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
void BattleEffect_ProcessPendingGraphicsTransfer(void);
void BattleEffect_CountDownBattlerTints(void);
void BattleEffect_LoadBlitter(s32 slot,
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
typedef DrawRectangle BlitterPair[2];

#define SLOT_BLITTER 104 // unk64[1]

// The Lost Age's blitter modes run the other way from The Broken Seal's.
enum { BLIT_ADDITIVE, BLIT_MAX, BLIT_TRANSPARENT, BLIT_COPY };
#define BLITTER(mode, flags) ((mode) << 4 | (flags))

void Actor_SetAnimationSpeedFar(void *object, s32 value);
void Func_0818de3c(void *triangles, s32 columns, s32 rows, s32 cellWidth,
                   s32 cellHeight); // builds a grid's mesh
void Render_ProjectVertices(const u8 *vertices, void *projected, s32 count);
void SceneTransform_ApplyPosition(void *position);

extern char ResourceId_Data134;
extern char ResourceId_MarsDjinnSmallSheet;
extern char ResourceId_DustBlobSheet;
extern char ResourceId_GlowOrbSheet;
extern char ResourceId_OrangePaletteB;
extern char ResourceId_PetalSheet;
extern char ResourceId_FlashBurstSheet;
extern u32 gCameraSceneParameters[];
extern u8 gMapCellBuffer[];
extern u8 gData_02014000[];
extern u8 Data_02012000[];
extern const u8 Data_08198c14[];   // each mode's flags for Func_0815585c
extern const s32 Data_08198c18[4]; // the debris's gravities
extern const u16 gData_08197410[];

// The body of Core (mode 0) and Cannon (mode 1): the Mars Djinni flies at
// the target. For Core it pulls back, then drives in and strikes four times
// before a glowing orb (a sphere of 8 by 8 vertices, its mesh built on the
// first frame) swells over the target and bursts; for Cannon it shoots
// straight in and strikes once, behind a gust of petals.
struct ModelVertex {
  s8 x;
  s8 y;
  s8 z;
  s8 unk03;
};

void Func_08172b00(struct BattleEffectArgument *effect, s32 mode) {
  u8 *mesh;
  u8 *projected;
  u8 *cells;
  void *canvas;
  struct BattleEffectWork *work;
  // Unused: blit[1] gives the stack frame its 4 bytes.
  BlitterPair blit;
  s32 frame;
  s16 *camera;
  u8 *sprites;
  s32 focus[3];
  struct EffectPosition screen;
  struct EffectPosition debris;
  struct ModelTexture texture;
  struct RenderModel *model;
  struct EffectStep *p;
  s32 count;
  s32 i;

  mesh = gData_02014000;
  projected = Data_02012000;
  cells = gMapCellBuffer;
  work = (struct BattleEffectWork *)Ram_HeapSlots->battleFxWork[0];
  canvas = Ram_HeapSlots->battleFxWork[1];
  camera = (s16 *)Ram_HeapSlots->cameraWork;
  sprites = Ram_HeapSlots->unk64[0];
  BattleEffect_StartCanvasLayer(0);
  Func_0815585c(effect, (effect->side << 4) | (Data_08198c14[mode] << 5) | 2,
                (s32 *)&screen, focus);
  {
    s32 *target;
    s16 savedYaw;
    void *caster;
    s32 frames;
    REG_BG2CNT = 0x785;
    if (effect->side == 0) {
      BattleEffect_LoadBlitter(
          SLOT_BLITTER,
          BLITTER(BLIT_ADDITIVE, COMPOSITE_CLIP_X | COMPOSITE_CLIP_Y));
    } else {
      BattleEffect_LoadBlitter(SLOT_BLITTER,
                               BLITTER(BLIT_ADDITIVE, COMPOSITE_CLIP_X |
                                                          COMPOSITE_CLIP_Y |
                                                          COMPOSITE_FLIP_X));
    }
    blit[0] = (DrawRectangle)Ram_HeapSlots->unk64[1];
    REG_BLDALPHA = 0x1010;
    work->transferMode = 2;
    work->transferValue = 50;
    Task_Add(BattleEffect_ProcessPendingGraphicsTransfer, 0xc80);
    target = *GetBattleObjectSlotFar(effect->actors[0]);
    savedYaw = camera[27];
    caster = *GetBattleObjectSlotFar(effect->actor);

    // The last two particles keep the Djinni's start and its step.
    work->particles[62].x = focus[0];
    work->particles[62].y = focus[1];
    work->particles[62].z = focus[2];
    Resource_LoadAndDecompress((s32)&ResourceId_Data134, sprites, 0, 0);
    Resource_LoadAndDecompress((s32)&ResourceId_MarsDjinnSmallSheet,
                               work->sheet + 0x1000, 1, 1);
    if (mode == 0) {
      Resource_LoadAndDecompress((s32)&ResourceId_DustBlobSheet, work->sheet, 1,
                                 1);
      Resource_LoadAndDecompress((s32)&ResourceId_GlowOrbSheet,
                                 work->sheet + 0x2000, 1, 1);
      {
        const void *source =
            Resource_GetTableEntry((s32)&ResourceId_OrangePaletteB);
        // A local for the routine: loads its address where the listing does.
        s32 (*copy)(void *, const void *, s32) = Iwram_CopyWords;

        copy((void *)BG_PLTT, source, 128);
      }
      frames = 102;
    } else {
      Resource_LoadAndDecompress((s32)&ResourceId_PetalSheet, cells, 1, 0);
      Resource_LoadAndDecompress((s32)&ResourceId_FlashBurstSheet,
                                 work->sheet + 0x2000, 1, 0);
      frames = 50;
    }
    REG_BG2PA = 0x80;
    Actor_SetAnimationSpeedFar(caster, 240);

    for (frame = 0; frame != frames; frame++) {
      if (frame == 0) {
        camera[27] = savedYaw;
        focus[0] = work->particles[62].x;
        focus[1] = work->particles[62].y;
        focus[2] = work->particles[62].z;
        work->particles[63].x = 0;
        work->particles[63].y = 0;
        work->particles[63].z = 0;
        work->particles[63].velocityX = target[2] - focus[0];
        // DEAD: both arms alike; the ROM keeps the test.
        if (mode == 0) {
          work->particles[63].velocityY = target[3] - focus[1] + 0x1e0000;
        } else {
          work->particles[63].velocityY = target[3] - focus[1] + 0x1e0000;
        }
        work->particles[63].velocityZ = target[4] - focus[2];
        for (i = 0; i != 62; i++) {
          work->particles[i].variant = -1;
        }
        if (mode == 1) {
          Sound_PlayFar(145);
        }
      }
      Render_ResetTransformState();
      Transform_SetLookAt((s32 *)camera, (s32 *)camera + 3);

      if (mode == 0) {
        if ((u32)frame <= 15) {
          s32 turn;

          // DEAD: frame is at most 15 here, so the second arm never runs. This
          // arm order gives frame's reload into r4.
          if (frame <= 71) {
            turn = 256;
          } else {
            turn = 2560 - (frame << 5);
          }
          if (effect->side == 0) {
            camera[27] -= turn;
          } else {
            camera[27] += turn;
          }
        }
        if (frame <= 31) {
          // Pull back...
          focus[0] -= work->particles[63].x;
          focus[1] -= work->particles[63].y;
          focus[2] -= work->particles[63].z;
          if (frame <= 15) {
            work->particles[63].x += work->particles[63].velocityX >> 10;
            work->particles[63].y += work->particles[63].velocityY >> 10;
            work->particles[63].z += work->particles[63].velocityZ >> 10;
          }
          work->particles[63].x = work->particles[63].x * 56 / 64;
          work->particles[63].y = work->particles[63].y * 56 / 64;
          work->particles[63].z = work->particles[63].z * 56 / 64;
          if (frame == 31) {
            Sound_PlayFar(138);
          }
        } else if (frame <= 45) {
          // ...drive in...
          focus[0] += work->particles[63].x;
          focus[1] += work->particles[63].y;
          focus[2] += work->particles[63].z;
          work->particles[63].x += work->particles[63].velocityX >> 7;
          work->particles[63].y += work->particles[63].velocityY >> 7;
          work->particles[63].z += work->particles[63].velocityZ >> 7;
        } else if (frame <= 72 && !(frame & 3)) {
          s32 thrown = 0;

          // ...and strike, every fourth frame.
          focus[0] += work->particles[63].x / 2;
          focus[1] += work->particles[63].y / 2;
          focus[2] += work->particles[63].z / 2;
          if (frame == 72) {
            Func_081180e8(134);
            work->shakeFrames = 8;
          } else {
            Sound_PlayFar(138);
            work->shakeFrames = 2;
          }
          Func_0814cd48(effect->actors[0], 7, 5, 0, 6);
          for (i = 0, p = work->particles; i != 63; i++, p++) {
            if (p->variant != -1) {
              continue;
            }
            p->x = target[2];
            p->y = target[3] + 0x50000;
            p->z = target[4];
            p->velocityX = ((Random16() & 511) + 256) << 10;
            if (effect->side == 1) {
              p->velocityX = -p->velocityX;
            }
            p->velocityY = ((Random16() & 511) + 256) << 9;
            p->velocityZ = 0;
            p->variant = (Random16() & 15) + 16;
            if (++thrown == 16) {
              break;
            }
          }
        }
      } else {
        if ((u32)frame <= 22) {
          focus[0] += work->particles[63].x;
          focus[1] += work->particles[63].y;
          focus[2] += work->particles[63].z;
          work->particles[63].x += work->particles[63].velocityX >> 7;
          work->particles[63].y += work->particles[63].velocityY >> 7;
          work->particles[63].z += work->particles[63].velocityZ >> 7;
        }
        if (frame == 15) {
          work->shakeFrames = 6;
          Func_081180e8(134);
          Func_0814cd48(effect->actors[0], 7, 5, 0, 6);
          Func_08118088(effect->actors[0], 0);
          for (i = 0; i != 63; i++) {
            s32 angle;
            s32 speed;

            p = &work->particles[i];
            p->x = target[2];
            p->y = target[3] + 0xa0000;
            p->z = target[4];
            angle = (Random16() & 0x7fff) - 0x4000;
            // DEAD: both arms alike; the ROM keeps the test and both calls.
            if (i > 53) {
              speed = (Random16() & 511) + 256;
            } else {
              speed = (Random16() & 511) + 256;
            }
            p->velocityX = (Trig_Sin(angle) * speed) >> 6;
            p->velocityY = (Trig_Cos(angle) * speed) >> 6;
            p->velocityZ = 0;
            p->variant = (Random16() & 15) + 16;
          }
        }
      }

      // The Djinni.
      EffectPosition_ProjectPoint(focus, &screen);
      if (mode == 0) {
        if (frame <= 73) {
          blit[0](canvas, work->sheet + 0x1000, screen.x / 2 - 10,
                  screen.y - 20, 20, 40);
        } else if (frame <= 93) {
          blit[0](canvas, work->sheet + 0x2000 + ((frame - 74) / 4) * 1600,
                  screen.x / 2 - 20, screen.y - 20, 40, 40);
        }
      } else if (frame <= 22) {
        blit[0](canvas, work->sheet + 0x1000, screen.x / 2 - 10, screen.y - 20,
                20, 40);
        if (mode == 1 && frame > 1) {
          s32 picture = (frame / 4) & 3;
          s32 x;

          if (effect->side == 0) {
            x = screen.x / 2 - 14;
          } else {
            x = screen.x / 2 - 46;
          }
          // The picture's address summed as integers: as cells + picture *
          // 6000, 2 differ.
          blit[0](canvas, (u8 *)(picture * 6000 + (s32)cells), x, screen.y - 50,
                  60, 100);
        }
      }

      // Debris, bouncing.
      count = 62;
      if (mode != 0) {
        count = 54;
      }
      for (i = 0; i != count; i++) {
        s32 size = work->particles[i].variant;

        if (size < 0) {
          continue;
        }
        EffectPosition_ProjectPoint(&work->particles[i], &debris);
        size >>= 3;
        size += 2;
        blit[0](canvas, sprites + gData_08197410[size - 1],
                debris.x / 2 - size / 2, debris.y - size, size, size * 2);
        BattleEffectKernels_IntegrateVector3(&work->particles[i], 62,
                                             Data_08198c18[i & 3]);
        if (work->particles[i].y < 0) {
          work->particles[i].velocityY = -work->particles[i].velocityY / 2;
        }
        work->particles[i].variant--;
      }
      if (mode == 1) {
        for (i = 0; i != 8; i++) {
          s32 picture;

          p = &work->particles[54 + i];

          if (p->variant < 0) {
            continue;
          }
          picture = 5 - p->variant / 6;
          EffectPosition_ProjectPoint(p, &debris);
          blit[0](canvas, work->sheet + 0x2000 + (picture << 11),
                  debris.x / 2 - 16, debris.y - 32, 32, 64);
          BattleEffectKernels_IntegrateVector3(p, 62, -0x400);
          p->variant--;
        }
      }
      if (mode == 0) {
        if (frame == 24) {
          Func_0814cd48(effect->actor, -1, 2, 0, -1);
        }
      } else if (frame == 0) {
        Func_0814cd48(effect->actor, -1, 2, 0, -1);
      }

      // Core's glowing orb.
      if (mode == 0 && frame <= 89) {
        s32 scale;

        model = Render_AllocateModelList(1);
        if (frame == 0) {
          s32 row;
          s32 angle;

          Func_0818de3c(mesh, 8, 8, 8, 8);
          for (row = 0, angle = 0; row != 8; angle += 4681, row++) {
            s32 y = (-Trig_Cos(angle) * 48) >> 16;
            s32 radius = (Trig_Sin(angle) * 48) >> 16;
            struct ModelVertex *vertex = (struct ModelVertex *)cells;
            s32 k;

            for (k = 0; k != 8; k++) {
              s32 x = (Trig_Sin(k << 13) * radius) >> 16;

              vertex[row * 8 + k].y = y;
              vertex[row * 8 + k].x = x;
              vertex[row * 8 + k].z = (Trig_Cos(k << 13) * radius) >> 16;
            }
          }
        }
        texture.logWidth = 5;
        texture.logHeight = 5;
        texture.pixels = work->sheet;
        Func_080151e4(0x8000, 0x10000, 0x8000);
        gCameraSceneParameters[3] = 60;
        SceneTransform_ApplyPosition(focus);
        Func_08015160(0, 0x100000, 0);
        if (effect->side == 0) {
          Func_080150e4(20576);
        } else {
          Func_080150e4(44960);
        }
        scale = 0xa000;
        if (frame > 31) {
          scale = (frame << 10) + 0x2000;
        }
        Func_0801521c(scale);
        if (frame > 3) {
          s32 shade = frame * 4 - 80;

          if (frame > 73) {
            shade = (74 - frame) * 4;
          }
          if (shade > 0) {
            shade = 0;
          }
          model->kind = 6;
          model->texture = &texture;
          model->mesh = mesh;
          model->vertices = projected;
          model->scrollU = (frame / 4) & 63;
          model->scrollV = frame & 63;
          model->shade = shade;
          model->unk04 = 3;
          Render_ProjectVertices(cells, projected, 64);
          Render_DrawModelList(model);
        }
        gCameraSceneParameters[3] = 120;
        Heap_Release(model);
      }
      Func_08158ce0(8, 8);
      BattleEffect_CountDownBattlerTints();
      work->transferPending = 1;
      wait(1);
    }
    Actor_SetAnimationSpeedFar(caster, 16);
    HeapSlot_Release(SLOT_BLITTER);
    Task_Remove(BattleEffect_ProcessPendingGraphicsTransfer);
  }
  BattleEffect_EndCanvasLayer();
}
