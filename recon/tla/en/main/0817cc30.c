// Draft, differ 4 of 598, skeleton gap 2 (tla-en, 2026-11-01)
//
// Left: two sched2 sites, one instruction each, and the RTL before sched2 is
// already the listing's order. At both blitter calls through blitters[n] the
// spill load of the blitters' address (ldr rN, [sp, #16]) is issued above the
// outgoing stack stores: above both at the column, above str r4, [sp, #4] at
// the drops. The sched2 dump gives that load priority 10 against the stores'
// 9: the two loads' link costs 3 with -mthumb-slow-loads' extra cycle, the
// store's 2, so the load always outranks by one whatever comes after it. A tie
// would keep LUID order, the listing's. That points at the load model, which
// is a compiler question (K2) and out of scope here; no device applies, since
// no C point lies between the stack stores and the reload, and a barrier inside
// an argument comes before the last store. Tried without gain: a blit local,
// a stored DrawRectangle pointer, the *(blitters + 1) spelling.
//
// What closed it from 448, for the next reader:
// - halfWidth, width and sheet are single-set constants on a path that does not
//   dominate their uses (the first frame). gcse's cprop then cannot fold them,
//   they get no register, and reload rematerializes them at each use: the
//   listing's movs r4, #48; subs r2, r3, r4, the r0 for 96 and the r2 for the
//   pool load. Set where they dominate, cprop folds them and 400 instructions
//   move, since the function pointer then takes r4 rather than ip.
// - sheet is added as integers so the shift stays the first operand.
// - spraySheet is one more constant held in r5 until the first load.
// - frame >= 0 && frame < N folds to the unsigned compare (bhi).
// - The bytes halve through an s32 local: asrs, where u8 / 2 gives lsrs.
// - -i - 4 reduces as a giv; -4 - i does not.
// - scroll is declared in a block after the address locals to take the lowest
//   slot (sp+8), and is set before frame, as the prologue's stores show.
// - One i throughout; drops, fountain and spray each their own pointer.

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
s32 Task_Add(void (*func)(void), s32 order);
void Task_Remove(void (*func)(void));
void HeapSlot_Release(s32 slot);
void Sound_PlayFar(s32 cue);
void wait(s32 frames);
u32 Random16(void);
s32 Trig_Sin(s32 angle);
s32 Trig_Cos(s32 angle);

extern char ResourceId_FlashBurstSheet;
extern char ResourceId_WhirlBeamSheet;
extern char ResourceId_Data134;
extern char ResourceId_BlueFlameSheet;
extern u8 gMapCellBuffer[];
extern const u16 gData_08197410[]; // gBattleEffect_MoteCells
extern const s32 Data_081994a8[4]; // the early drops' drags
extern u8 gData_02014000[];        // the spray sheet
// The drops' lives: gMapCellBuffer's EffectSteps from their variant on.
extern s32 Data_02010018[];

#define BLITTER ((DrawRectangle)Ram_HeapSlots->unk64[1])
enum { BLIT_ADDITIVE, BLIT_MAX, BLIT_TRANSPARENT };
#define BLIT_MODE(mode, flags) ((mode) << 4 | (flags))
#define BLIT_CLIP (COMPOSITE_CLIP_X | COMPOSITE_CLIP_Y)
#define HI(v) (((s16 *)&(v))[1])
#define OCEAN_FIST_FRAMES 92
#define OCEAN_DROPS 512

// Abilities: Ocean Fist.
//
// A column of water (three pictures of the work's sheet, each halved in
// brightness from frame 32 to 43) rises at the target and is drawn top-down
// for 42 frames; spray bursts at its foot, and drops of the map cell
// buffer's 512 fly up from the target for the first 32 frames, with a
// fountain of 256 at frame 28. The target is struck at frame 32 and pushed
// back.
void BattleEffect_RunOceanFist(struct BattleEffectArgument *effect) {
  u8 *spraySheet = gData_02014000;
  void *canvas;
  struct BattleEffectWork *work;
  u8 *sprites;
  DrawRectangle blitters[2];
  struct EffectPosition target;
  struct EffectStep *drops = (struct EffectStep *)gMapCellBuffer;
  struct EffectStep *drop;
  struct EffectStep *spray;
  s32 frame;
  s32 halfWidth;
  s32 width;
  u8 *sheet;
  s32 i;

  canvas = Ram_HeapSlots->battleFxWork[1];
  work = (struct BattleEffectWork *)Ram_HeapSlots->battleFxWork[0];
  sprites = Ram_HeapSlots->unk64[0];
  Func_0813ba50();
  BattleEffect_StartCanvasLayer(1);
  REG_BLDALPHA = 0x1010;
  BattleEffect_FetchRectangleBlitters(0, (void **)blitters);
  Resource_LoadAndDecompress((s32)&ResourceId_FlashBurstSheet, spraySheet, 1,
                             0);
  Resource_LoadAndDecompress((s32)&ResourceId_WhirlBeamSheet, work->sheet, 1,
                             0);
  Resource_LoadAndDecompress((s32)&ResourceId_Data134, sprites, 0, 0);
  Iwram_CopyWords((void *)BG_PLTT,
                  Resource_GetTableEntry((s32)&ResourceId_BlueFlameSheet), 128);
  work->transferMode = 2;
  work->transferValue = 50;
  Task_Add(BattleEffect_ProcessPendingGraphicsTransfer, 0xc80);
  EffectPosition_ProjectBattler(effect->actors[0], &target);

  {
    s32 scroll = 0;
    for (frame = 0; frame != OCEAN_FIST_FRAMES; scroll += 8, frame++) {
      if (frame == 0) {
        s32 *life = Data_02010018;

        for (i = 0; i != OCEAN_DROPS; i++) {
          *life = -1;
          life += 7;
        }
        Resource_LoadAndDecompress((s32)&ResourceId_WhirlBeamSheet, work->sheet,
                                   1, 0);
        Sound_PlayFar(145);
        work->shakeFrames = 4;
        // Set once: reload rebuilds each of these at its use (see the header).
        halfWidth = 48;
        width = 96;
        sheet = gData_02014000;
      }
      if (frame == 32) {
        Func_0814cd48(effect->actors[0], 7, 5, 0, 8);
        BattleEffect_StartActorMotion(effect->actors[0], 1, 0, 0, 0xf0000, 100);
      }
      if (frame == 38) {
        Func_081180e8(134);
        work->shakeFrames = 16;
      }

      // Frames 32 to 43: each in turn, the column's pictures dim.
      if ((u32)(frame - 32) <= 11) {
        u8 *pixel = work->sheet + (frame % 3) * 3 * 0x800;

        for (i = 0; i != 0x1800; i++) {
          s32 value = *pixel;

          *pixel = value >> 1;
          pixel++;
        }
      }

      // The column, three 64-line bands scrolling up from line 120.
      if (frame >= 0 && frame <= 41) {
        s32 top = 120 - scroll;
        s32 picture = frame % 3;
        s32 x = target.x / 2;
        s32 phase = scroll % 64;

        if (top < 0) {
          top = 0;
        }
        for (i = 0; i != 3; i++) {
          s32 y = i * 64 - phase;
          s32 height = 108 - y;

          if (height > 64) {
            height = 64;
          }
          if (height > 0 && y >= top - 64) {
            if (y < top) {
              height -= top - y;
              y = top;
            }
            blitters[1](canvas, work->sheet + picture * 3 * 0x800,
                        x - halfWidth, y, width, height);
          }
        }
      }

      // The spray at the column's foot.
      if (frame == 0) {
        for (i = 0; i != 32; i++) {
          work->particles[i].variant = -i - 4;
        }
      }
      for (i = 0, spray = work->particles; i != 24; i++, spray++) {
        if ((u32)spray->variant <= 17) {
          blitters[0](canvas, (u8 *)(((spray->variant / 3) << 11) + (s32)sheet),
                      HI(spray->x) - 16, HI(spray->y) - 32, 32, 64);
          BattleEffectKernels_IntegrateVector2(spray, 62, -0x2000);
        }
        if (++spray->variant == 0) {
          spray->x = ((Random16() & 31) + target.x / 2 - 16) << 16;
          spray->y = ((Random16() & 7) + 88) << 16;
          spray->velocityX = ((Random16() & 255) - 128) << 11;
          spray->velocityY = 0;
        }
      }

      // The drops: a fountain at frame 28, sixteen more a frame until 31.
      if (frame == 28) {
        struct EffectStep *fountain;

        for (i = 0, fountain = drops; i != 256; i++, fountain++) {
          if (fountain->variant == -1) {
            s32 radius = Random16() & 63;
            s32 angle = Random16() & 0xffff;

            fountain->x = ((Trig_Sin(angle) * radius) >> 3) + 0x200000;
            fountain->y = ((Trig_Cos(angle) * radius) >> 2) + 0x600000;
            fountain->velocityX = ((Random16() & 63) - 32) << 14;
            fountain->velocityY = (-(Random16() & 63) - 8) << 13;
            fountain->variant = 0;
          }
        }
      }
      if (frame >= 0 && frame <= 31) {
        s32 spawned = 0;

        for (i = 0, drop = drops; i != OCEAN_DROPS; i++, drop++) {
          if (drop->variant == -1) {
            drop->x = ((Random16() & 31) + target.x / 2 - 16) << 16;
            drop->y = 0x6c0000;
            drop->velocityX = ((Random16() & 63) - 32) << 14;
            drop->velocityY = (-(Random16() & 63) - 8) << 13;
            drop->variant = 0;
            if (++spawned == 16) {
              break;
            }
          }
        }
      }
      for (i = 0, drop = drops; i != OCEAN_DROPS; i++, drop++) {
        if (drop->variant >= 0) {
          s32 size = i % 3 + 3;

          blitters[i & 1](canvas, sprites + gData_08197410[size - 1],
                          HI(drop->x) - size / 2, HI(drop->y) - size, size,
                          size * 2);
          if (frame > 31) {
            BattleEffectKernels_IntegrateVector2(drop, 62, 0x4000);
          } else {
            BattleEffectKernels_IntegrateVector2(drop, 62,
                                                 Data_081994a8[i & 3]);
          }
          drop->variant++;
          if (drop->velocityY > 0 && (drop->y >> 16) > 104) {
            drop->variant = -1;
          }
          if (drop->y < -size) {
            drop->variant = -1;
          }
        }
      }
      if (frame <= 37) {
        work->shakeFrames = 16;
      }
      if (frame <= 7) {
        Func_08158ce0(8, 8);
      } else if (frame <= 37) {
        Func_08158ce0(4, 4);
      } else {
        Func_08158ce0(8, 8);
      }
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
