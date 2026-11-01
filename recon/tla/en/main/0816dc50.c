// Draft, differ 18 of 1101, skeleton gap 12 (tla-en, 2026-11-01)
// Two places are left, nothing else differs.
// 1. The opening's two Iwram_ClearWords calls (14 of the 18). The native builds
// 0x3c00 in r1 before each; a direct call's constant gets one pseudo, CSE
// shares it and it stays in r5. Sending the first call alone through
// ClearWords_ (static inline, as canvas.c) closes this and takes the draft to
// differ 4, gap 4. That is a FAKEMATCH(wrapper) device and it does not reach
// differ 0 on its own, so it is not here. Plain forms tried: block locals for
// the size and the address, a do/while round each call (worse, 50).
// 2. The falling Djinni's blitter (the last 4). After Render_ProjectPoint the
// native issues movs r1, #40 first and ldr r5, [sp, #48] after both stack
// stores; ours loads the two screen words first and that reload before the
// stores. sched2: all of these rank at priority 7 but the stores at 6, and
// the loads win on forward dependents (5 to 4). The native's order needs the
// stores at or above the reload, which no source change moved: this is the
// load-after-store issued too early that K2's -mthumb-slow-loads approximates,
// so it may be a question about the option and not about this C. Tried:
// locals for x, y and size, the blitter through a local, pointer or wrapper,
// a void cast of the call, byte and element forms of the sheet address, a
// pointer to dropScreen, dropScreen as an array or volatile, a do/while round
// the call or the projection, an empty asm after the projection.
// spireShade holds 0 and then -32, so it is a temporary device with no tag yet:
// without it CSE forms -32 as 7 - 39 (differ goes from 18 to 74); one-constant
// locals for -32, wherever set, are also worse. Tag it at adoption with N
// measured then. Ram_MeshBuffer belongs in ram_buffer.h at adoption.
// 2026-11-01, what closed the gap from 781: every reload rotates through a
// spill set, and r4 joins that set only where r0 to r3 are all busy, which is
// the first quad's stores with quad2 formed after them, left and right formed
// before. The rest followed: quad[4..9] and quad2's store order; the buffers at
// constant addresses (the mesh's second base is then +12 of the first, by
// reload_cse_move2add); speed and angle each one expression, so the add moves
// over the call only in sched2; mistSheet one constant across the function, so
// it gets no register and is rematerialized; mesh a local set first; x and y
// before the first blitter's sheet address; the mist's loop on the shared i;
// the turn an if/else; the shard angle a giv of (frame + i) << 10; the mist's
// size its own variable; Task_Add's real return type; pointer fields for shade
// and pixels with different alias sets, which orders their stores.
// 2026-10-31: frame is spilled and vertices kept in r11 as in the native once
// column is not caller-saved; camera, savedYaw and target live in a block after
// the blitter fetch, as the native's spill-slot order shows. The yaw is the
// tree's u16 (its -14000 clamp stores 0xc950), each particle loop takes its own
// &work->particles[i], and the spire is a second if under frame < 64, as the
// native's re-test shows.
// 2026-10-19: the native's arrays above the arguments' slots take 92 bytes:
// the falling Djinni is projected into dropScreen, not a point of its own;
// screenB is declared before screenA and frame after work, as the native's
// stack offsets show.

#include "battle_effect.h"
#include "battle_display.h"
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
  const u8 *pixels;
};

struct RenderModel {
  s32 kind;
  s32 unk04;
  const void *mesh;
  void *vertices;
  struct ModelTexture *texture;
  const void *shade;
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

#include "io_write_queue.h"
#include "irq_lock.h"
#include "actor.h"
#include "motobj.h"

struct ProjectedPoint {
  s32 x, y, depth;
};

s32 Render_ProjectPoint(const void *point, struct ProjectedPoint *screen);
void Render_ProjectVertices(const u8 *vertices, void *projected, s32 count);

extern char ResourceId_Data0C1;
extern char ResourceId_DustBlobSheet;
extern char ResourceId_Data0CF;
extern char ResourceId_MercuryDjinnSheet;
extern char ResourceId_BlueFlameSheet;
extern u32 gCameraSceneParameters[];

// One model vertex: signed byte coordinates, padded to a word.
struct ModelVertex {
  s8 x;
  s8 y;
  s8 z;
  s8 pad;
};

// The cone's 64 vertices, stored at the buffer's constant address.
extern struct ModelVertex gMapCellBuffer[];
#define VERTICES ((struct ModelVertex *)Ram_MapCellBuffer)
extern u8 gData_02014000[];
extern u8 Data_02012000[];
#define Ram_MeshBuffer FIXED_ADDRESS((u8 *), 0x02012000, Data_02012000)
extern u8 Data_02012540[];       // the mesh's end, after 112 faces
extern const s8 Data_08198b7a[]; // the cone's ring heights
extern const s8 Data_08198b82[]; // and radii
extern const u8 Data_08198b8a[]; // each ripple's delay
extern const u8 Data_08198b8e[]; // its growth
extern const s8 Data_08198b92[]; // its height
extern const u8 Data_08198cac[];
extern const u8 Data_08198ec4[];
extern const u8 Data_0819919c[];
extern const u8 Data_081991a4[];

// Abilities: Serac.
//
// The Mercury Djinni drops onto the target, dust bursting where it lands;
// a spire of ice (a cone of 8 rings of 8 vertices, its mesh built on the
// first frame) grows up around the target in rings of mist while the
// camera circles, the caster closes in at frame 56, and at 64 the spire
// shatters into 64 tumbling shards.
void BattleEffect_RunSerac(struct BattleEffectArgument *effect) {
  void *canvas;
  struct BattleEffectWork *work;
  s32 frame;
  DrawRectangle blitters[2];
  s32 screenB[3];
  s32 screenA[3];
  u8 *mistSheet;

  canvas = Ram_HeapSlots->battleFxWork[1];
  work = (struct BattleEffectWork *)Ram_HeapSlots->battleFxWork[0];
  Func_0813ba50();
  if (effect->side == 0) {
    BattleEffect_StartTiledCanvasFilled(0x2000);
  } else {
    BattleEffect_StartTiledCanvas(0x2000);
  }
  Func_0815585c(effect, (effect->side << 4) | 0x101, screenA, screenB);
  Iwram_ClearWords((void *)0x06004000, 0x3c00);
  Iwram_ClearWords(canvas, 0x3c00);
  REG_BLDALPHA = 0x1010;
  REG_BG2PA = 0x100;
  BattleEffect_FetchRectangleBlitters(0, (void **)blitters);
  work->transferMode = 2;
  work->transferValue = 50;
  Task_Add(BattleEffect_ProcessPendingGraphicsTransfer, 0xc80);
  mistSheet = gData_02014000;
  Resource_LoadAndDecompress((s32)&ResourceId_Data0C1, mistSheet, 0, 0);
  Resource_LoadAndDecompress((s32)&ResourceId_DustBlobSheet, work->sheet, 1, 1);
  Resource_LoadAndDecompress((s32)&ResourceId_Data0CF, work->sheet + 0x400, 0,
                             0);
  Resource_LoadAndDecompress((s32)&ResourceId_MercuryDjinnSheet,
                             work->sheet + 0x4000, 1, 0);
  Iwram_CopyWords((void *)BG_PLTT,
                  Resource_GetTableEntry((s32)&ResourceId_BlueFlameSheet), 128);
  // A block of its own: camera, savedYaw and target take their stack slots
  // after the blitter pair's address.
  {
    struct BattleCamera *camera;
    s16 savedYaw;
    struct MotionObject *target;
    struct ProjectedPoint screen;
    s32 world[3];
    struct ProjectedPoint point;
    struct ProjectedPoint dropScreen;
    struct ModelTexture texture;
    struct RenderModel *model;
    void *vertices;
    u8 color[1];
    s32 savedScrollX;
    s32 savedScrollY;
    s32 row;
    s32 column;
    s32 i;

    camera = (struct BattleCamera *)Ram_HeapSlots->cameraWork;
    savedYaw = camera->yaw;
    target = GetBattleObjectSlotFar(effect->actors[0])->object;
    REG_BG2PA = 0x100;
    REG_BG2X = 0;

    for (frame = 0; frame != 88; frame++) {
      if (frame == 0) {
        camera->yaw = savedYaw;
        for (row = 0; row != 8; row++) {
          for (column = 0; column != 8; column++) {
            s32 height;
            s32 radius;
            s32 angle;

            if (row == 0) {
              height = Data_08198b7a[row];
              radius = Data_08198b82[row];
            } else {
              height = Data_08198b7a[row] + (Random16() & 3);
              radius = Data_08198b82[row] + (Random16() & 3);
            }
            angle = (column << 13) + 0x1000;
            VERTICES[row * 8 + column].x = (Trig_Sin(angle) * radius) >> 16;
            VERTICES[row * 8 + column].y = -height;
            VERTICES[row * 8 + column].z = (Trig_Cos(angle) * radius) >> 16;
            if (row <= 6) {
              u8 *quad = Ram_MeshBuffer + (row * 8 + column) * 24;
              u8 *quad2;
              s32 bottom = row * 8 + 7;
              s32 left = column * 16;
              s32 right = left + 15;
              s32 next;
              s32 here;

              quad[4] = right;
              quad[5] = row * 8;
              quad[6] = right;
              quad[9] = bottom;
              quad[7] = bottom;
              quad[8] = left;
              quad2 = Ram_MeshBuffer + 12 + (row * 8 + column) * 24;
              quad2[4] = left;
              quad2[8] = left;
              quad2[5] = row * 8;
              quad2[9] = bottom;
              quad2[6] = right;
              quad2[7] = row * 8;
              next = (column + 1) % 8 + row * 8;
              quad[0] = next;
              here = column % 8 + row * 8;
              quad[1] = here + 8;
              quad[2] = next + 8;
              quad2[0] = here;
              quad2[1] = here + 8;
              quad2[2] = next;
            }
          }
        }
        Data_02012540[0] = 0;
        Data_02012540[1] = 0;
        Data_02012540[2] = 0;
      }

      // The camera circles the spire.
      if ((u32)(frame - 4) <= 79) {
        s32 turn;

        if (frame <= 63) {
          turn = 320;
        } else {
          turn = 1600 - frame * 20;
        }
        if (effect->side == 0) {
          camera->yaw -= turn;
        } else {
          camera->yaw += turn;
        }
        if (effect->side == 0) {
          if ((s16)camera->yaw < -14000) {
            camera->yaw = -14000;
          }
        } else if ((s16)camera->yaw > 14000) {
          camera->yaw = 14000;
        }
      }

      vertices = Heap_AllocEwram(0x400);
      model = Render_AllocateModelList(1);
      savedScrollX = gCameraSceneParameters[3];
      savedScrollY = gCameraSceneParameters[4];
      gCameraSceneParameters[4] = savedScrollY - 16;
      Render_ResetTransformState();
      Transform_SetLookAt(camera->eye, camera->pos);
      world[0] = target->x;
      world[1] = 0;
      world[2] = target->z;
      Render_ProjectPoint(world, &screen);
      if (screen.x <= 63) {
        screen.x = 64;
      }
      if (screen.x > 176) {
        screen.x = 176;
      }
      gCameraSceneParameters[3] = 184 - screen.x;
      IO_WRITE_QUEUE_PUSH(0x04000028, (64 - screen.x) << 8, 0x30000);

      // Dust where the Djinni lands.
      if ((u32)frame <= 63) {
        if (frame == 0) {
          for (i = 0; i != 32; i++) {
            struct EffectStep *p = &work->particles[i];
            s32 speed = (Random16() & 1023) + 1024;
            s32 angle = Random16() & 0xffff;

            p->x = target->x;
            p->y = 0;
            p->z = target->z;
            p->velocityX = (Trig_Sin(angle) * speed) >> 9;
            p->velocityY = 0;
            p->velocityZ = (Trig_Cos(angle) * speed) >> 9;
            p->variant = -i;
          }
        }
        for (i = 0; i != 32; i++) {
          struct EffectStep *p = &work->particles[i];
          s32 picture;
          s32 x;
          s32 y;

          if ((u32)++p->variant > 23) {
            continue;
          }
          picture = p->variant / 6;
          Render_ProjectPoint(p, &point);
          BattleEffectKernels_IntegrateVector3(p, 60, 0x2000);
          x = point.x - 16;
          y = point.y - 16;
          blitters[0](canvas, work->sheet + 0x400 + (picture << 10), x, y, 32,
                      32);
        }
      }
      // The Djinni falls.
      if (frame <= 29) {
        point.x = target->x;
        point.y = 0x3c0000 - (frame << 17);
        point.depth = target->z + (Trig_Sin(frame << 9) << 5);
        Render_ProjectPoint(&point, &dropScreen);
        blitters[1](canvas, work->sheet + 0x4000, dropScreen.x - 20,
                    dropScreen.y - 30, 40, 40);
      }
      if (frame == 4) {
        Sound_PlayFar(156);
      }
      if (frame == 30) {
        Sound_PlayFar(139);
      }
      if (frame == 60) {
        Sound_PlayFar(212);
      }
      if (frame == 64) {
        Sound_PlayFar(134);
        Func_081180e8(-1);
      }
      if (frame == 30) {
        Func_0814cd48(effect->actors[0], 9, 5, -1, 0);
      }
      if (frame == 26) {
        work->shakeFrames = 8;
      }
      if (frame == 64) {
        work->shakeFrames = 4;
        Func_0814cd48(effect->actors[0], 7, 5, 0, 4);
      }
      if (frame == 70) {
        Func_0814cd48(effect->actors[0], 7, 5, 0, 4);
      }
      if (frame == 56) {
        // The caster closes four fifths of the way.
        struct Actor *caster =
            (struct Actor *)GetBattleObjectSlotFar(effect->actor)->object;
        struct Actor *to =
            (struct Actor *)GetBattleObjectSlotFar(effect->actors[0])->object;
        s32 x = caster->x;
        s32 z;
        s32 dx = (to->x - x) * 80 / 100;
        s32 dz;
        s32 speed;

        z = caster->z;
        dz = (to->z - z) * 80 / 100;
        x += dx;
        z += dz;
        dx >>= 8;
        dz >>= 8;
        speed = (Iwram_Sqrt(dx * dx + dz * dz) << 8) / 10;
        caster->acceleration = speed;
        caster->speedLimit = speed;
        caster->snapToTarget = 1;
        caster->velocityY = 0x60000;
        caster->gravity = 0x13333;
        caster->bounce = 0;
        caster->actionFlags = 1;
        Actor_ResetMotion(caster);
        Actor_SetTargetPosition(caster, x, 0, z);
        Actor_SetMode(caster, 2);
      }

      if (frame > 63) {
        if (frame == 64) {
          Iwram_FillWords(canvas, 0x4000, 0x3f3f3f3f);
          for (i = 0; i != 64; i++) {
            struct EffectStep *p = &work->particles[i];
            s32 angle = (i & 7) << 13;
            s32 ring;

            p->x = target->x + ((Trig_Sin(angle) * (ring = i / 8)) << 2);
            p->y = ((Random16() & 0xffff) << 2) - ((ring * 5) << 17) + 0x400000;
            p->z = target->z + ((Trig_Cos(angle) * ring) << 2);
            p->velocityX = ((Random16() & 0xffff) << 1) + 0x10000;
            p->velocityY = ((Random16() & 0xffff) - 12768) * 3;
            p->velocityZ = ((Random16() & 0xffff) - 0x8000) << 1;
            if (p->x < 0) {
              p->velocityX = -p->velocityX;
            }
            p->variant = (Random16() & 31) + 32;
          }
        } else {
          model->kind = 2;
          model->unk04 = 0;
          model->mesh = Data_0819919c;
          model->vertices = vertices;
          for (i = 0; i != 64; i++) {
            struct EffectStep *p = &work->particles[i];
            s32 angle = (frame + i) << 10;

            if (p->variant < 0) {
              continue;
            }
            color[0] = p->variant;
            p->variant -= 2;
            model->shade = color;
            BattleEffectKernels_IntegrateVector3(p, 62, 0x400);
            Transform_Push();
            Func_08015160(p->x, p->y, p->z);
            SceneTransform_ApplyPitch(angle);
            Func_080150e4(angle);
            Func_0801521c(0x10000);
            Render_ProjectVertices(Data_081991a4, vertices, 3);
            Render_DrawModelList(model);
            Transform_Pop();
          }
        }
      }
      // Two compares in the native, not one merged range check.
      if (frame < 64) {
        if (frame > 25) {
          s32 scale;
          s32 start;
          // Set before the branch below and stored after it; the second value
          // stays apart from the 7 beside it.
          s32 spireShade;
          const void *mesh = Data_02012000;

          // The spire, then the mist inside it.
          texture.logWidth = 5;
          texture.logHeight = 5;
          texture.pixels = (const u8 *)work->sheet;
          model->kind = 6;
          model->unk04 = 3;
          model->vertices = vertices;
          model->scrollU = 0;
          model->texture = &texture;
          model->mesh = mesh;
          model->scrollV = -((frame / 2) & 31);
          Transform_Push();
          spireShade = 0;
          if (frame <= 31) {
            scale = frame * 10000 - 260000;
          } else {
            scale = 60000;
          }
          model->shade = (const void *)spireShade;
          Func_08015160(target->x, -0x40000, target->z);
          Func_080151e4(scale, scale, scale);
          Render_ProjectVertices((const u8 *)gMapCellBuffer, vertices, 64);
          Render_DrawModelList(model);
          spireShade = -32;
          model->shade = (const void *)spireShade;
          model->kind = 7;
          model->scrollU = 0;
          model->scrollV = (frame / 2) & 31;
          model->unk04 = 2;
          Render_DrawModelList(model);
          Transform_Pop();

          // Four rings of mist spread from its foot.
          texture.logWidth = 7;
          texture.logHeight = 7;
          texture.pixels = mistSheet;
          model->kind = 7;
          model->texture = &texture;
          model->mesh = Data_08198ec4;
          model->unk04 = 0;
          model->vertices = vertices;
          model->scrollU = 0;
          model->scrollV = 0;
          for (i = 0; i != 4; i++) {
            s32 size;
            s32 shade;

            start = Data_08198b8a[i] + 22;
            if (frame <= start) {
              continue;
            }
            size = (frame - start) * Data_08198b8e[i] * 1500 + 0x4180;
            shade = (start - frame) * 8 + 56;
            if (shade > 0) {
              shade = 0;
            }
            if (shade <= -64) {
              continue;
            }
            Transform_Push();
            model->shade = (const void *)shade;
            Func_08015160(target->x, Data_08198b92[i] << 16, target->z);
            Func_080151e4(size, size, size);
            model->scrollU = ((frame + i * 5) & 7) << 4;
            Render_ProjectVertices(Data_08198cac, vertices, 32);
            Render_DrawModelList(model);
            Transform_Pop();
          }
        }
      }
      Heap_Release(model);
      Heap_Release(vertices);
      gCameraSceneParameters[3] = savedScrollX;
      gCameraSceneParameters[4] = savedScrollY;
      Func_08158ce0(8, 8);
      BattleEffect_CountDownBattlerTints();
      work->transferPending = 1;
      wait(1);
    }
  }
  Task_Remove(BattleEffect_ProcessPendingGraphicsTransfer);
  HeapSlot_Release(188);
  HeapSlot_Release(104);
  BattleEffect_EndCanvasLayer();
}
