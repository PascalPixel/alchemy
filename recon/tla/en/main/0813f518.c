// Near miss, differ 40 of 380.

// Twin: The Broken Seal's BattleFx_RunGatheringBurst, reached from the
// scenes through the far-call table (table_47.s). Like 0813e3d0 it sets up
// its own heap blocks, stores work->effect and marks the work active
// itself, and reads the actors from the argument. The corner sparks' four
// blitters are loaded through Func_08196404 directly, with all five
// arguments.

// Left: in the corner-spark loop the native hoists the loader's mode (1)
// into r6 and reads the alternate blitter's slot afresh each turn; here the
// slot's address is the hoisted value and the 1 is built each turn. The
// rest are single rows where the native builds a constant or issues a load
// one slot earlier, and the pool offsets they move.

// Tried:
// the twin, work block allocated by sizeof (the struct is short)  44
// the work block's real size, 0x7b7c                             40
// the slot read inside the loop: the address is no longer
// hoisted, but neither is the 1                                60

#include "types.h"
#include "ram_buffer.h"
#include "io_reg.h"
#include "mem.h"
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

#define CALLBACK_ORDER 0xc80

// The rising sparkles live in the map cell buffer.
#define gSparkles ((struct EffectStep *)Ram_MapCellBuffer)

// The four corner sparks: their flips and their place around the target
// (The Broken Seal's gCornerSparks_DrawFlags, _X and _Y).
extern u8 Data_081976da[];
extern s8 Data_081976d2[];
extern s8 Data_081976d6[];

// The mote's cell for each size from 1 to 6 (gBattleFx_FlareCells).
extern u16 Data_08197424[];

extern char ResourceId_CyanSparkSheet;
extern char ResourceId_ParticleSpritesD;
extern char ResourceId_YellowPaletteA;
extern char ResourceId_IceTileSheet;
extern char ResourceId_RedPaletteA;
extern char ResourceId_VioletPaletteA;

void BattleFx_BeginCanvasLayer(s32 control);
void BattleFx_EndCanvasLayer(void); // BattleFx_EndCanvasLayer
s32 BattleFx_LoadBlitter(s32 slot, s32 flags);
void Func_08196404(s32 slot, s32 widthShift, s32 heightShift, s32 flags,
                   s32 mode); // BattleFx_BuildBlitter
s32 BattleFx_ProcessPendingGraphicsTransfer(void);      // the canvas transfer callback
s32 Scheduler_AddOrUpdateCallback(void *callback, s32 order);
s32 Scheduler_RemoveCallback(void *callback);
void Runtime_ReleaseHeapBlock(s32 slot);
void Audio_PlayCue(s32 cue);
u32 Random16(void);
void Func_0814cd48(s32 actor, s32 palette, s32 frames, s32 timerSlot,
                   s32 timerFrames); // ObjectGroup_UpdateMembers
void ObjectGroup_TickMemberTimers(void);            // ObjectGroup_TickMemberTimers
void WaitFrames(s32 frames);

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

  work = Runtime_AllocateHeapBlock(SLOT_BATTLE_EFFECT, BATTLE_EFFECT_WORK_SIZE);
  canvas = Runtime_AllocateHeapBlock(SLOT_BATTLE_CANVAS, 0x4000);
  sheet = Runtime_AllocateHeapBlock(SLOT_BATTLE_SHEET, 0x60e);
  work->framesSinceTransfer = 1;
  work->effect = effect;
  BattleFx_BeginCanvasLayer(0);
  work->fadeFrames = 24;
  work->fadeStep = 0;
  REG_BLDALPHA = 0x100c;
  REG_BG2PA = 0x100;
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
  Iwram_CopyWords((void *)BG_PLTT, palette, 128);

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
  Scheduler_AddOrUpdateCallback(BattleFx_ProcessPendingGraphicsTransfer, CALLBACK_ORDER);
  BattleFx_LoadBlitter(SLOT_BLITTER, BLITTER(BLIT_ADDITIVE, CLIP | COMPOSITE_FLIP_X));
  draw[0] = (DrawRectangle)Ram_HeapSlots->unk64[1];
  Audio_PlayCue(140);

  for (frame = 0; frame != 56; frame++) {
    EffectPosition_ApplyStepAndYOffset(effect->actor, &ground);
    REG_BG2X = (64 - ground.x) << 8;
    if (frame > 49)
      REG_BLDALPHA = (0x70 - frame * 2) | 0x1000;
    if (frame == 26) {
      Audio_PlayCue(212);
      Func_0814cd48(effect->actors[0], 7, -1, 0, 20);
    }

    // The burst, a picture every three frames.
    if (frame >= 28 && frame <= 48)
      draw[0](canvas, work->sheet + (frame - 28) / 3 * 2304 + 0x1400, 40,
              ground.y - 24, 48, 48);

    // The corner sparks, each with its own flips.
    if (frame >= 0 && frame <= 14) {
      s32 picture = frame / 3 % 5;
      DrawRectangle *slot = (DrawRectangle *)&Ram_HeapSlots->unkb8[1];

      for (i = 0; i != 4; i++) {
        Func_08196404(SLOT_BLITTER_ALTERNATE, 7, 7, Data_081976da[i] | CLIP,
                      BLIT_MAX);
        draw[1] = *slot;
        draw[1](canvas, work->sheet + (picture << 10), Data_081976d2[i] + 32,
                ground.y + Data_081976d6[i] - 32, 32, 32);
        Runtime_ReleaseHeapBlock(SLOT_BLITTER_ALTERNATE);
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
    ObjectGroup_TickMemberTimers();
    work->transferPending = 1;
    WaitFrames(1);
  }

  Runtime_ReleaseHeapBlock(SLOT_BLITTER);
  Scheduler_RemoveCallback(BattleFx_ProcessPendingGraphicsTransfer);
  BattleFx_EndCanvasLayer();
  Runtime_ReleaseHeapBlock(SLOT_BATTLE_SHEET);
  Runtime_ReleaseHeapBlock(SLOT_BATTLE_CANVAS);
  Runtime_ReleaseHeapBlock(SLOT_BATTLE_EFFECT);
}
