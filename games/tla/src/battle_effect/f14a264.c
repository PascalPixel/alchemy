#include "types.h"
#include "system.h"
#include "io_reg.h"
#include "ram_buffer.h"
#include "iwram_call.h"
#include "task.h"
#include "heap_release.h"
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

#define PILLAR_FRAMES 96
#define SPARK_MAX 1024

// The sparks live in the map cell buffer.
#define PARTICLES ((struct EffectStep *)Ram_MapCellBuffer)

extern char ResourceId_FirePillarSheetA; // directory row 0x146
extern char ResourceId_Data134; // directory row 0x134
extern char ResourceId_RedPaletteB;      // directory row 0x147
extern char ResourceId_OrangePaletteB;   // directory row 0x148

extern u8 Data_08197a20[];    // ☀️ gVolcano_Counts, for each variant
extern s8 Data_08197a23[][4]; // ☀️ gVolcano_Depths
extern u8 Data_08197a2f[];    // ☀️ gVolcano_StartFrames
extern u16 gData_08197410[];  // ☀️ gBattleEffect_MoteCells

void BattleEffect_StartCanvasLayer(s32 bgControl);
void BattleEffect_EndCanvasLayer(void);
void BattleEffect_ProcessPendingGraphicsTransfer(
    void); // ☀️ ProcessPendingGraphicsTransfer
void BattleEffect_CountDownBattlerTints(void);
void Func_08158ce0(s32 x, s32 y);          // ☀️ Camera_ApplyShake
void Func_081180e8(s32 phase);             // ☀️ BattleEvent_StartPhaseFar
void Func_08118088(s32 actor, s32 motion); // ☀️ ApplyVariantMotionFar
s32 BattleEffect_LoadBlitter(s32 slot,
                             s32 flags); // ☀️ BattleEffect_BuildBlitter
void Func_0814cd48(s32 actor, s32 palette, s32 frames, s32 timerSlot,
                   s32 timerFrames); // ☀️ BattleEffect_TintBattler
void Sound_PlayFar(s32 cue);

// One, two or four pillars of fire rise in front of the targets, one after
// another; each burns for the rest of the first 64 frames, scrolling its
// flames upward, throws a burst of sparks when it starts and a few more
// every frame after its sixteenth, and strikes every target as it starts.
void BattleEffect_RunVolcano(struct BattleEffectArgument *effect) {
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
  BattleEffect_StartCanvasLayer(1);
  REG_BLDALPHA = 0x1010;
  Resource_LoadAndDecompress((s32)&ResourceId_FirePillarSheetA, work->sheet, 1,
                             1);
  Resource_LoadAndDecompress((s32)&ResourceId_Data134, sheet, 0, 0);
  if (effect->variant == 0) {
    const void *palette = Resource_GetTableEntry((s32)&ResourceId_RedPaletteB);
    s32 (*copy)(void *destination, const void *source, s32 size) =
        Iwram_CopyWords;

    // The entry held in a local loads ahead of the destination's shift; as a
    // plain Iwram_CopyWords call, 2 of 604 instructions differ in tla-en.
    copy((void *)BG_PLTT, palette, 128);
  } else if (effect->variant == 2) {
    const void *palette =
        Resource_GetTableEntry((s32)&ResourceId_OrangePaletteB);
    s32 (*copy)(void *destination, const void *source, s32 size) =
        Iwram_CopyWords;

    // The entry held in a local loads ahead of the destination's shift; as a
    // plain Iwram_CopyWords call, 2 of 604 instructions differ in tla-en.
    copy((void *)BG_PLTT, palette, 128);
  }
  BattleEffect_LoadBlitter(HEAP_BLITTER, BLITTER_MAX << 4 | COMPOSITE_CLIP_X |
                                             COMPOSITE_CLIP_Y);
  draw[0] = (DrawRectangle)Ram_HeapSlots->unk64[1];
  BattleEffect_LoadBlitter(HEAP_BLITTER_ALTERNATE,
                           BLITTER_MAX << 4 | COMPOSITE_CLIP_X |
                               COMPOSITE_CLIP_Y | COMPOSITE_FLIP_X);
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
  Task_Add(BattleEffect_ProcessPendingGraphicsTransfer, TASK_PASS_FRAME);

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
        EffectPosition_ProjectPoint((s32 *)&work->particles[pillar], &screen);
        screen.x /= 2;
        screen.y -= 8;
        if (frame == Data_08197a2f[pillar])
          Sound_PlayFar(145);

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
    BattleEffect_CountDownBattlerTints();
    work->transferPending = 1;
    wait(1);
  }

  // Tear down.
  HeapSlot_Release(HEAP_BLITTER_ALTERNATE);
  HeapSlot_Release(HEAP_BLITTER);
  Task_Remove(BattleEffect_ProcessPendingGraphicsTransfer);
  BattleEffect_EndCanvasLayer();
}
