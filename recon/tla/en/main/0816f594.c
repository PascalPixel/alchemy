// Draft of Func_0816f594, effect kind 110 (Radiant Fire), written from the
// listing on The Broken Seal's canvas skeleton. No close twin (shape
// BattleFx_RunUndeadSword 0.525, call order BattleFx_RunBreak
// 0.551); the textured square goes through the ARM polygon kernels as in
// the summons (Func_081969f8, Func_08196958, Func_08196a7c).

// Listing 389 instructions, draft 392, differ 257.
// Left: allocation. In the disc loop the native keeps the offset in r7,
// the size in r6 and 2i in r5, reads the table base back from the stack
// and multiplies by 2i + 3 (r5 + 3) for size + 1; here the table gets a
// walking pointer and size + 1 is size's own. The flame loops take r6
// where the native has r7 and the reverse, and the size test of the
// disc draw is laid out differently (size * 2 - 2 against 61 in the
// native).
// Tried (differ):
// first draft                                                       267
// locals in the native's stack-slot order (canvas, frame, camera,
// savedYaw, target, discs)                                         257
// size derived from i ((i + 1) * 2, i * 2 + 2, 2 + i * 2): the
// multiply then reads 2i + 3 as the native does, but the offset
// spills                                                            292
// offset += (i * 2 + 3) * size / 2                                    256
// the table read through work->sheet with no discs local             264
// alchemy permute 8 minutes, 2 jobs (from the 292 form): 266

#include "types.h"
#include "io_reg.h"
#include "ram_buffer.h"
#include "iwram_call.h"
#include "system.h"
#include "fixed_math.h"
#include "callback_scheduler.h"
#include "resource.h"
#include "mem.h"
#include "motobj.h"
#include "effect_step.h"
#include "battle_display.h"
#include "battle_effect.h"
#include "battle_effect_work.h"

// The Lost Age numbers the blitter modes from the other end of The Broken
// Seal's COMPOSITE_* list; the packed loader argument is mode << 4 | flags.
enum { BLIT_ADDITIVE, BLIT_MAX, BLIT_TRANSPARENT, BLIT_COPY };
#define BLITTER(mode, flags) ((mode) << 4 | (flags))

#define FLAMES 32

// A texture for the ARM polygon kernels: 1 << logWidth by 1 << logHeight
// pixels.
struct Texture {
  u32 logWidth : 8;
  u32 logHeight : 8;
  u32 unk02 : 16;
  void *pixels;
};

// One model job for Func_08196a7c, the polygon kernel runner.
struct RenderModel {
  s32 kind;
  s32 unk04;
  const void *mesh;
  void *vertices;
  struct Texture *texture;
  s32 brightness;
};

extern char ResourceId_Data0C2;
extern char ResourceId_OrangePaletteB;

extern const u16 gData_08199364[]; // a textured square
extern const u16 Data_081991e0[];  // its four corners

// The canvas layer and its helpers, with The Broken Seal's names.
void BattleFx_BeginCanvasLayer(s32 bgControl);
void Func_08143bb8(void); // BattleFx_EndCanvasLayer
void Func_08143000(void); // BattleFx_ProcessPendingGraphicsTransfer
void Func_081963ec(s32 slot, s32 blitter); // BattleFx_LoadWork, packed
void Func_0814cd48(s32 actor, s32 palette, s32 group, s32 timerSlot,
                   s32 frames);            // ObjectGroup_UpdateMembers
void Func_081434f8(void);                  // ObjectGroup_TickMemberTimers
void Func_08158ce0(s32 mask, u32 range);   // Camera_ApplyShake
void Func_08118088(s32 actor, s32 motion); // BattleMotion_ApplyVariantFar
void Func_081180e8(s32 phase);             // BattleEventRuntime_BeginPhaseFar
void Func_08157530(s32 actor, s32 target, s32 frames, s32 percent);
void Func_0815b434(void *map, s32 size, s32 scale); // a shaded disc
void Func_0818caa8(void *picture, s32 x, s32 y, s32 size);
void Render_ResetTransformState(void);
void Graphics_PrepareTransferInIwramWork(s32 *eye, s32 *position);
void Func_08015160(s32 x, s32 y, s32 z);
void Func_080150e4(s32 angle);
void SceneTransform_ApplyPitch(s32 angle);
void Func_0801521c(s32 angle);
void *Runtime_BumpAllocateAlternatePool(s32 size);
struct RenderModel *Func_081969f8(s32 count);
void Func_08196958(const void *corners, void *vertices, s32 count);
void Func_08196a7c(struct RenderModel *model);
void MemFree(void *block);
void Audio_PlayCue(s32 cue);

// Radiant Fire: eight bursts of fire bloom one after another around the
// target, each a shaded disc that swells and shrinks under a spinning
// square of flame, with a white flash of the canvas as the first lands.
//
// Setup: canvas layer 0 at half blend; one max blitter; the flame texture
// (resource 0x0c2) and the orange palette; 32 shaded discs of 2 to 64
// pixels drawn into the map cell buffer, their offsets kept in the work
// sheet.
// Frame loop (66 frames): at frame 0 the bursts are placed at random
// around the target and the user closes in; from frame 8 burst i lands at
// frame 16 + 4i and burns for 24 frames.
void Func_0816f594(struct BattleEffectArgument *effect) {
  struct BattleEffectWork *work;
  void *canvas;
  s32 frame;
  struct BattleCamera *camera;
  s16 savedYaw;
  struct MotionObject *target;
  s16 *discs;
  struct EffectPosition screen;
  struct Texture texture;
  struct EffectStep *flame;
  struct RenderModel *model;
  void *vertices;
  s32 offset;
  s32 size;
  s32 i;

  work = (struct BattleEffectWork *)Ram_HeapSlots->battleFxWork[0];
  canvas = Ram_HeapSlots->battleFxWork[1];
  BattleFx_BeginCanvasLayer(0);
  REG_BLDALPHA = 0x1010;
  Func_081963ec(104, BLITTER(BLIT_MAX, COMPOSITE_CLIP_X | COMPOSITE_CLIP_Y));
  Resource_LoadAndDecompress((s32)&ResourceId_Data0C2, work->sheet + 0x1000, 0,
                             0);
  Iwram_CopyWords((void *)BG_PLTT,
                  Resource_GetTableEntry((s32)&ResourceId_OrangePaletteB), 128);
  work->transferMode = 2;
  work->transferValue = 75;
  Scheduler_AddOrUpdateCallback(Func_08143000, 0xc80);
  camera = Ram_HeapSlots->cameraWork;
  savedYaw = camera->yaw;
  target = GetBattleObjectSlotFar(effect->actors[0])->object;

  // The discs, smallest first, one after another in the map cell buffer.
  discs = (s16 *)work->sheet;
  offset = 0;
  size = 2;
  for (i = 0; i != FLAMES; i++) {
    Func_0815b434(Ram_MapCellBuffer + offset, size, 0x14e20);
    discs[i] = offset;
    offset += size * (size + 1) / 2;
    size += 2;
  }

  for (frame = 0; frame != 66; frame++) {
    // Frame 0: each burst takes a random place on a ring about the target
    // that widens with its number, 48 pixels up, and is projected to the
    // screen; it waits 4 frames more than the one before it.
    if (frame == 0) {
      camera->yaw = savedYaw;
      Render_ResetTransformState();
      Graphics_PrepareTransferInIwramWork(camera->eye, camera->pos);
      flame = work->particles;
      for (i = 0; i != FLAMES; i++, flame++) {
        s32 angle;
        s32 radius;

        Random16();
        angle = Random16() & 0xffff;
        radius = (Random16() & 7) + i * 4;
        flame->x = target->x + Trig_Sin(angle) * radius * 2;
        flame->y = target->y + Trig_Cos(angle) * radius + (48 << 16);
        flame->z = target->z;
        EffectPosition_ApplyBaseAndYOffset(&flame->x, &screen);
        flame->x = screen.x / 2;
        flame->y = screen.y;
        Random16();
        flame->velocityX = 32 - i;
        flame->velocityY = Random16();
        flame->velocityZ = Random16();
        flame->variant = -i * 4;
      }
    }
    if (frame == 0)
      Func_08157530(effect->actor, effect->actors[0], 8, 80);

    // Frame 8: the canvas flashes white and the target is struck.
    if (frame == 8) {
      Iwram_FillWords(canvas, 0x3c00, 0x3f3f3f3f);
      work->shakeFrames = 16;
      Func_08118088(effect->actors[0], 0);
      Func_0814cd48(effect->actors[0], 7, 5, 0, 8);
      Func_081180e8(145);
    }

    if (frame >= 8) {
      // Each burst strikes the target as it lands, then its disc swells
      // and shrinks with a sine over 32 frames.
      flame = work->particles;
      for (i = 0; i != 8; i++, flame++) {
        flame->variant++;
        if (flame->variant == 8) {
          Audio_PlayCue(144);
          work->shakeFrames = 8;
          Func_08118088(effect->actors[0], 0);
          Func_0814cd48(effect->actors[0], 7, 5, 0, 8);
        }
        if (flame->variant >= 0 && flame->variant < 32) {
          size =
              ((Trig_Sin(flame->variant << 10) * flame->velocityX) >> 16) * 2;
          if (size >= 2 && size <= 63)
            Func_0818caa8(Ram_MapCellBuffer + discs[size / 2 - 1], flame->x,
                          flame->y, size);
        }
      }

      // The square of flame turns over each burst for 24 frames, fading
      // over the last eight.
      vertices = Runtime_BumpAllocateAlternatePool(32);
      model = Func_081969f8(1);
      texture.logWidth = 7;
      texture.logHeight = 7;
      texture.pixels = work->sheet + 0x1000;
      model->kind = 6;
      model->vertices = vertices;
      model->mesh = gData_08199364;
      model->texture = &texture;
      flame = work->particles;
      for (i = 0; i != 8; i++, flame++) {
        if (flame->variant >= 0 && flame->variant < 24) {
          s32 roll = flame->variant * 3 << 11;
          s32 brightness = 0;

          if (flame->variant > 15)
            brightness = (16 - flame->variant) * 8;
          model->brightness = brightness;
          Render_ResetTransformState();
          Func_08015160((flame->x - 64) << 16, (flame->y - 64) << 16, 0);
          Func_080150e4(flame->velocityZ);
          SceneTransform_ApplyPitch(flame->velocityY);
          Func_0801521c(roll);
          Func_08196958(Data_081991e0, vertices, 4);
          Func_08196a7c(model);
        }
      }
      MemFree(model);
      MemFree(vertices);
    }

    Func_08158ce0(4, 4);
    Func_081434f8();
    work->transferPending = 1;
    WaitFrames(1);
  }

  Scheduler_RemoveCallback(Func_08143000);
  Runtime_ReleaseHeapBlock(104);
  Func_08143bb8();
}
