// Twin: The Broken Seal's BattleEffect_RunElementBurst, reached from the
// scenes through the far-call table (table_47.s). Like 0813e3d0 it sets up
// its own heap blocks, stores work->effect and marks the work active
// itself, and reads the actors from the argument. The corner sparks' four
// blitters are loaded through Func_08196404 directly, with all five
// arguments.

#include "types.h"
#include "ram_buffer.h"
#include "io_reg.h"
#include "heap_release.h"
#include "battle_effect.h"
#include "battle_effect_work.h"
#include "effect_step.h"
#include "iwram_call.h"
#include "fixed_math.h"
#include "resource.h"

typedef DrawRectangle BlitterPair[2];

// Heap slots, as byte offsets into Ram_HeapSlots.
#define SLOT_BATTLE_EFFECT 92      // battleFxWork[0]
#define SLOT_BATTLE_CANVAS 96      // battleFxWork[1]
#define SLOT_BATTLE_SHEET 100      // unk64[0]
#define SLOT_BLITTER 104           // unk64[1]
#define SLOT_BLITTER_ALTERNATE 188 // unkb8[1]

// The whole effect work block: struct BattleEffectWork names only its first
// 0x78f4 bytes.
#define BATTLE_EFFECT_WORK_SIZE 0x7b7c

// ⚓️'s blitter modes run the other way from The Broken Seal's.
enum { BLIT_ADDITIVE, BLIT_MAX, BLIT_TRANSPARENT, BLIT_COPY };
#define BLITTER(mode, flags) ((mode) << 4 | (flags))
#define CLIP (COMPOSITE_CLIP_X | COMPOSITE_CLIP_Y)

#define TASK_PASS_FRAME 0xc80

// The rising sparkles live in the map cell buffer.
#define gSparkles ((struct EffectStep *)Ram_MapCellBuffer)

// The four corner sparks: their flips and their place around the target
// (The Broken Seal's gDeathPlunge_DrawFlags, _X and _Y).
extern u8 Data_081976da[];
extern s8 Data_081976d2[];
extern s8 Data_081976d6[];

// The mote's cell for each size from 1 to 6 (gBattleEffect_FlareCells).
extern u16 Data_08197424[];

extern char ResourceId_CyanSparkSheet;
extern char ResourceId_ParticleSpritesD;
extern char ResourceId_YellowPaletteA;
extern char ResourceId_IceTileSheet;
extern char ResourceId_RedPaletteA;
extern char ResourceId_VioletPaletteA;

void BattleEffect_StartCanvasLayer(s32 control);
void BattleEffect_EndCanvasLayer(void);
s32 BattleEffect_LoadBlitter(s32 slot, s32 flags);
s32 Func_08196404(s32 slot, s32 widthShift, s32 heightShift, s32 flags,
                  u32 mode); // BattleEffect_BuildBlitter
s32 BattleEffect_ProcessPendingGraphicsTransfer(
    void); // the canvas transfer callback
s32 Task_Add(void *func, s32 order);
s32 Task_Remove(void *func);
void HeapSlot_Release(s32 slot);
void Sound_PlayFar(s32 cue);
u32 Random16(void);
void Func_0814cd48(s32 actor, s32 palette, s32 frames, s32 timerSlot,
                   s32 timerFrames); // BattleEffect_TintBattler
void BattleEffect_CountDownBattlerTints(void);
void wait(s32 frames);

// Light gathering on the user and bursting, for the scenes.

// For the first 15 frames four sparks flare at the user's corners, each
// mirrored to face the middle; 64 motes spiral in from a ring around the
// user, shrinking as they come. At frame 26 the first target flashes with a
// sound, and from frame 28 to 48 a 48x48 burst plays out over seven
// pictures. effect->kind picks the colours (yellow, ice, red or violet). The
// last 6 frames fade out. The 128 sparkles set up in the map cell buffer are
// never drawn.

void Func_0813f518(struct BattleEffectArgument *effect) {
  struct EffectPosition ground;
  struct BattleEffectWork *work;
  void *canvas;
  BlitterPair draw;
  u8 *sheet;
  void *palette;
  s32 frame;
  s32 i;

  work = HeapSlot_Alloc(SLOT_BATTLE_EFFECT, BATTLE_EFFECT_WORK_SIZE);
  canvas = HeapSlot_Alloc(SLOT_BATTLE_CANVAS, 0x4000);
  sheet = HeapSlot_Alloc(SLOT_BATTLE_SHEET, 0x60e);
  work->vblanksSinceTransfer = 1;
  work->effect = effect;
  BattleEffect_StartCanvasLayer(0);
  work->fadeFrames = 24;
  work->fadeStep = 0;
  REG_BLDALPHA = BLEND_ALPHA(12, 16);
  REG_BG2PA = Q_8_8(1);
  Resource_LoadAndDecompress((s32)&ResourceId_CyanSparkSheet, work->sheet, 1,
                             0);
  Resource_LoadAndDecompress((s32)&ResourceId_ParticleSpritesD, sheet, 0, 0);
  switch (effect->kind) {
  case 0:
    palette = Resource_GetTableEntry((s32)&ResourceId_YellowPaletteA);
    break;
  case 1:
    palette = Resource_GetTableEntry((s32)&ResourceId_IceTileSheet);
    break;
  case 2:
    palette = Resource_GetTableEntry((s32)&ResourceId_RedPaletteA);
    break;
  default:
    palette = Resource_GetTableEntry((s32)&ResourceId_VioletPaletteA);
    break;
  }
  {
    /* FAKEMATCH(temporary): called through a local pointer to its fixed entry,
     * which the native loads before the palette address; as a plain
     * Iwram_CopyWords call, 2 of 380 instructions differ in tla-en. */
    s32 (*copy)(void *, const void *, s32) = Iwram_CopyWordsEntry;

    copy((void *)BG_PLTT, palette, 128);
  }

  for (i = 0; i != 128; i++) {
    gSparkles[i].y = 0x800000;
    gSparkles[i].x = Random16() & 0xffff;
    gSparkles[i].z = (Random16() & 0x1ff) + 0x400;
    gSparkles[i].variant = -i;
  }

  // A mote's x is its angle and y its distance from the user; its variant
  // counts down its life.
  for (i = 0; i != 64; i++) {
    work->particles[i].x = Random16() & 0xffff;
    work->particles[i].y = (Random16() & 31) + 16;
    work->particles[i].variant = (i & 15) + 16;
  }
  work->transferMode = 2;
  work->transferValue = 75;
  Task_Add(BattleEffect_ProcessPendingGraphicsTransfer,
                                TASK_PASS_FRAME);
  BattleEffect_LoadBlitter(SLOT_BLITTER,
                           BLITTER(BLIT_ADDITIVE, CLIP | COMPOSITE_FLIP_X));
  draw[0] = (DrawRectangle)Ram_HeapSlots->unk64[1];
  Sound_PlayFar(140);

  for (frame = 0; frame != 56; frame++) {
    EffectPosition_ProjectScaledBattler(effect->actor, &ground);
    REG_BG2X = (64 - ground.x) << 8;
    if (frame > 49)
      REG_BLDALPHA = (0x70 - frame * 2) | 0x1000;
    if (frame == 26) {
      Sound_PlayFar(212);
      Func_0814cd48(effect->actors[0], 7, -1, 0, 20);
    }

    // The burst, a picture every three frames.
    if (frame >= 28 && frame <= 48)
      draw[0](canvas, work->sheet + (frame - 28) / 3 * 2304 + 0x1400, 40,
              ground.y - 24, 48, 48);

    // The corner sparks, each with its own flips.
    if (frame >= 0 && frame <= 14) {
      s32 picture = frame / 3 % 5;
      /* FAKEMATCH(temporary): the mode set once before the loop, which the
       * native keeps in r6; as BLIT_MAX in the call, 38 of 380 instructions
       * differ in tla-en. */
      s32 mode;

      for (i = 0, mode = BLIT_MAX; i != 4; i++) {
        Func_08196404(SLOT_BLITTER_ALTERNATE, 7, 7, Data_081976da[i] | CLIP,
                      mode);
        draw[1] = (DrawRectangle)Ram_HeapSlots->unkb8[1];
        draw[1](canvas, work->sheet + (picture << 10), Data_081976d2[i] + 32,
                ground.y + Data_081976d6[i] - 32, 32, 32);
        HeapSlot_Release(SLOT_BLITTER_ALTERNATE);
      }
    }

    // The motes close in 2 pixels a frame.
    if (frame >= 0) {
      for (i = 0; i != 64; i++) {
        struct EffectStep *mote = &work->particles[i];

        if (mote->variant >= 0 && mote->y > 0) {
          s32 size = (mote->variant >> 3) + 1;
          s32 x = (mote->y * Trig_Sin(mote->x) >> 16) + 64;
          s32 y = (mote->y * Trig_Cos(mote->x) >> 16) + ground.y;

          if (size <= 0)
            size = 1;
          draw[0](canvas, sheet + Data_08197424[size - 1], x - size, y - size,
                  size * 2, size * 2);
          mote->y -= 2;
          mote->variant--;
        }
      }
    }
    BattleEffect_CountDownBattlerTints();
    work->transferPending = 1;
    wait(1);
  }

  HeapSlot_Release(SLOT_BLITTER);
  Task_Remove(BattleEffect_ProcessPendingGraphicsTransfer);
  BattleEffect_EndCanvasLayer();
  HeapSlot_Release(SLOT_BATTLE_SHEET);
  HeapSlot_Release(SLOT_BATTLE_CANVAS);
  HeapSlot_Release(SLOT_BATTLE_EFFECT);
}
