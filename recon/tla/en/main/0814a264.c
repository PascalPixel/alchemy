// Near miss, differ 102 of 604 (effect kind 13, the fire pillars).

// ☀️ twin: BattleFx_RunVolcano (games/tbs/src/effect/effect.c), shape
// 0.847, structure 1.000. ⚓️'s changes: the effect argument read directly,
// the red and orange palettes from directory rows 0x147 and 0x148, two max
// blitters from BattleFx_LoadBlitter(104, 0x13) and (188, 0x17) (⚓️ max is mode
// 1), the camera from Ram_HeapSlots->cameraWork, the pillar's x before its
// y, the transfer callback at 0xc80, and the tables under their ⚓️ labels
// (pillar counts Data_08197a20, depths Data_08197a23, start frames
// Data_08197a2f).

// Same frame and instruction count. What is left is order, most of it in
// the pillar loop's set-up (18 rows: the native keeps the start-frame
// pointer, the frame's low bit, the sheet address and the screen record's
// address in four spill slots set in another order) and in the base cel's
// draws; elsewhere single rows and pool offsets.

// Tried (differ):
// ☀️'s body with ⚓️'s names and tables                              120
// the camera's yaw stepped as ☀️ writes it: yaw + 192, and
// (s16)yaw - 192 (plain -= 192 builds 0xff40 and adds)             102
// greedy declaration-order scan, every local through every place    102

#include "types.h"
#include "system.h"
#include "io_reg.h"
#include "ram_buffer.h"
#include "iwram_call.h"
#include "callback_scheduler.h"
#include "mem.h"
#include "effect_step.h"
#include "motobj.h"
#include "battle_effect.h"
#include "battle_effect_work.h"
#include "battle_display.h"
#include "graphics.h"
#include "transform.h"
#include "fixed_math.h"
#include "resource.h"

#define HEAP_BLITTER 104           // ☀️ HEAP_SLOT_BLITTER
#define HEAP_BLITTER_ALTERNATE 188 // ☀️ HEAP_SLOT_BLITTER_ALTERNATE

// The Lost Age numbers its blitter modes in the reverse of The Broken Seal's
// order: Func_08196404 builds additive for 0, max for 1, transparent for 2.
enum { BLITTER_ADDITIVE, BLITTER_MAX, BLITTER_TRANSPARENT, BLITTER_COPY };

#define CALLBACK_ORDER 0xc80
#define PILLAR_FRAMES 96
#define SPARK_MAX 1024

// The sparks live in the map cell buffer.
#define PARTICLES ((struct EffectStep *)Ram_MapCellBuffer)

extern char ResourceId_FirePillarSheetA; // directory row 0x146
extern char ResourceId_ParticleSpritesA; // directory row 0x134
extern char ResourceId_RedPaletteB;      // directory row 0x147
extern char ResourceId_OrangePaletteB;   // directory row 0x148

extern u8 Data_08197a20[];    // ☀️ gVolcano_Counts, for each variant
extern s8 Data_08197a23[][4]; // ☀️ gVolcano_Depths
extern u8 Data_08197a2f[];    // ☀️ gVolcano_StartFrames
extern u16 gData_08197410[];  // ☀️ gBattleFx_MoteCells

void BattleFx_BeginCanvasLayer(s32 bgControl);
void BattleFx_EndCanvasLayer(void);                  // ☀️ BattleFx_EndCanvasLayer
void BattleFx_ProcessPendingGraphicsTransfer(void);                  // ☀️ ProcessPendingGraphicsTransfer
void ObjectGroup_TickMemberTimers(void);                  // ☀️ ObjectGroup_TickMemberTimers
void Func_08158ce0(s32 x, s32 y);          // ☀️ Camera_ApplyShake
void Func_081180e8(s32 phase);             // ☀️ BattleEventRuntime_BeginPhaseFar
void Func_08118088(s32 actor, s32 motion); // ☀️ ApplyVariantMotionFar
s32 BattleFx_LoadBlitter(s32 slot, s32 flags);    // ☀️ BattleFx_BuildBlitter
void Func_0814cd48(s32 actor, s32 palette, s32 frames, s32 timerSlot,
                   s32 timerFrames); // ☀️ ObjectGroup_UpdateMembers
void Audio_PlayCue(s32 cue);

// One, two or four pillars of fire rise in front of the targets, one after
// another; each burns for the rest of the first 64 frames, scrolling its
// flames upward, throws a burst of sparks when it starts and a few more
// every frame after its sixteenth, and strikes every target as it starts.
void BattleFx_RunVolcano(struct BattleEffectArgument *effect) {
  struct BattleEffectWork *work;
  void *canvas;
  s32 frame;
  s32 pillar;
  u8 *sheet;
  struct EffectPosition screen;
  DrawRectangle draw[2];
  struct MotionObject *object;
  struct BattleCamera *camera;
  s32 i;

  // Set up: the canvas layer, the pillar sheet, the particle sprites, the
  // variant's palette and two max blitters, the second mirrored.
  work = (struct BattleEffectWork *)Ram_HeapSlots->battleFxWork[0];
  canvas = Ram_HeapSlots->battleFxWork[1];
  sheet = Ram_HeapSlots->unk64[0];
  BattleFx_BeginCanvasLayer(1);
  REG_BLDALPHA = 0x1010;
  Resource_LoadAndDecompress((s32)&ResourceId_FirePillarSheetA, work->sheet, 1,
                             1);
  Resource_LoadAndDecompress((s32)&ResourceId_ParticleSpritesA, sheet, 0, 0);
  if (effect->variant == 0)
    Iwram_CopyWords((void *)BG_PLTT,
                    Resource_GetTableEntry((s32)&ResourceId_RedPaletteB), 128);
  else if (effect->variant == 2)
    Iwram_CopyWords((void *)BG_PLTT,
                    Resource_GetTableEntry((s32)&ResourceId_OrangePaletteB),
                    128);
  BattleFx_LoadBlitter(HEAP_BLITTER,
                BLITTER_MAX << 4 | COMPOSITE_CLIP_X | COMPOSITE_CLIP_Y);
  draw[0] = (DrawRectangle)Ram_HeapSlots->unk64[1];
  BattleFx_LoadBlitter(HEAP_BLITTER_ALTERNATE, BLITTER_MAX << 4 | COMPOSITE_CLIP_X |
                                            COMPOSITE_CLIP_Y |
                                            COMPOSITE_FLIP_X);
  draw[1] = (DrawRectangle)Ram_HeapSlots->unkb8[1];

  for (i = 0; i != SPARK_MAX; i++)
    PARTICLES[i].variant = 0;

  // The pillars stand in front of the first target, on its side of the
  // field, at the variant's depths.
  object = GetBattleObjectSlotFar(effect->actors[0])->object;
  for (i = 0; i != 4; i++) {
    struct EffectStep *base = &work->particles[i];
    s32 x;

    x = ((Random16() & 15) + 72) << 16;
    base->x = x;
    base->y = 0;
    base->z = Data_08197a23[effect->variant][i] << 16;
    if (object->x < 0)
      base->x = -x;
  }

  work->transferMode = 2;
  work->transferValue = 50;
  Scheduler_AddOrUpdateCallback(BattleFx_ProcessPendingGraphicsTransfer, CALLBACK_ORDER);

  for (frame = 0; frame != PILLAR_FRAMES; frame++) {
    // The strongest variant turns the camera for 64 frames.
    camera = Ram_HeapSlots->cameraWork;
    if (effect->variant == 2 && frame < 64) {
      if (effect->side == 0)
        camera->yaw = camera->yaw + 192;
      else
        camera->yaw = (s16)camera->yaw - 192;
    }
    if (frame == 16)
      Func_081180e8(134);
    Render_ResetTransformState();
    Transform_SetLookAt(camera->eye, camera->pos);

    if (frame < 64) {
      for (pillar = 0; pillar != Data_08197a20[effect->variant]; pillar++) {
        EffectPosition_ProjectPoint((s32 *)&work->particles[pillar],
                                           &screen);
        screen.x /= 2;
        screen.y -= 8;
        if (frame == Data_08197a2f[pillar])
          Audio_PlayCue(145);

        // Four frames after it starts the pillar burns: its flames scroll
        // up through two copies, and its base flickers between two cels.
        if (frame >= Data_08197a2f[pillar] + 4) {
          s32 scroll = (frame * 16 + pillar * 25) % 104;

          draw[pillar & 1](canvas, work->sheet, screen.x - 17,
                           screen.y - scroll - 104, 34, 104);
          draw[pillar & 1](canvas, work->sheet, screen.x - 17,
                           screen.y - scroll, 34, scroll);
          if (frame & 1) {
            draw[0](canvas, work->sheet + 0xdd0, screen.x - 20, screen.y - 24,
                    20, 37);
            draw[1](canvas, work->sheet + 0xdd0, screen.x, screen.y - 24, 20,
                    37);
          } else {
            draw[0](canvas, work->sheet + 0x10b4, screen.x - 20, screen.y - 24,
                    20, 37);
            draw[1](canvas, work->sheet + 0x10b4, screen.x, screen.y - 24, 20,
                    37);
          }
        }

        // Sparks: 200 as the pillar starts, then four a frame from its
        // sixteenth frame on.
        if (frame == Data_08197a2f[pillar] ||
            frame >= Data_08197a2f[pillar] + 16) {
          s32 spawned = 0;

          for (i = 0; i != SPARK_MAX; i++) {
            struct EffectStep *spark = &PARTICLES[i];

            if (spark->variant == 0) {
              s32 speed;
              s32 angle;

              speed = (Random16() & 0x3ff) + 32;
              angle = (Random16() & 0x7fff) - 0x4000;
              spark->x = screen.x << 8;
              spark->y = (screen.y << 8) + 0x1000;
              spark->z = (Trig_Sin(angle) * speed) >> 15;
              spark->velocityY = -(Trig_Cos(angle) * speed * 2) >> 15;
              spawned++;
              if (frame == Data_08197a2f[pillar]) {
                spark->variant = (Random16() & 7) + 48;
                if (spawned == 200)
                  break;
              } else {
                spark->variant = (Random16() & 7) + 24;
                if (spawned == 4)
                  break;
              }
            }
          }
        }

        // The pillar strikes every target as it starts.
        if (frame == Data_08197a2f[pillar]) {
          work->shakeFrames = 2;
          for (i = 0; i != effect->count; i++) {
            Func_0814cd48(effect->actors[i], 10, 5, i, 8);
            Func_08118088(effect->actors[i], 1);
          }
        }
      }
    }

    // The sparks: 8.8 fixed point, z holding the x speed; they slow, fall,
    // bounce at half speed below line 120, and shrink as they burn out.
    for (i = 0; i != SPARK_MAX; i++) {
      struct EffectStep *spark = &PARTICLES[i];
      s32 life = spark->variant;

      if (life > 0) {
        s32 x;
        s32 y;
        s32 py;

        spark->variant = life - 1;
        x = spark->x += spark->z;
        y = spark->y += spark->velocityY;
        spark->z = spark->z * 60 / 64;
        spark->velocityY = spark->velocityY * 60 / 64 - 16;
        py = y / 256;
        if (py > 120) {
          spark->velocityY = -spark->velocityY / 2;
        } else if (x >= 0 && x >> 8 <= 126 && y >= 0) {
          s32 px = x >> 8;
          s32 size = (life - 17) / 8;

          if (size <= 0)
            size = 1;
          draw[i & 1](canvas, sheet + gData_08197410[size - 1], px - size / 2,
                      py - size, size, size * 2);
        }
      }
    }

    Func_08158ce0(16, 16);
    ObjectGroup_TickMemberTimers();
    work->transferPending = 1;
    WaitFrames(1);
  }

  // Tear down.
  Runtime_ReleaseHeapBlock(HEAP_BLITTER_ALTERNATE);
  Runtime_ReleaseHeapBlock(HEAP_BLITTER);
  Scheduler_RemoveCallback(BattleFx_ProcessPendingGraphicsTransfer);
  BattleFx_EndCanvasLayer();
}
