// Near miss, differ 12 of 312.
//
// Measured 2026-10-25 under -mthumb-slow-loads: differ 12. Most rows left are
// loads from memory or the stack that alchemy-gcc issues one slot earlier than
// the native, which issues only its pool loads early. The first is the
// argument's spill, which the native stores before it starts the 0x03000000
// base; an empty asm before the first statement closes that row (differ 10).

// Twin: The Broken Seal's BattleEffect_RunBladeRain. ⚓️ keeps the effect but no
// handler or caller reaches it. It rains crossed daggers instead of swords:
// every picture is a 32x32 cell, so the twin's three cell tables are gone,
// and a landed dagger is drawn whole. The rest is the twin with ⚓️'s
// calling conventions.

// Left: the native moves the blitter pair's address into r11 after the
// first literal pool, where this moves it before the branch around the
// pool; one row's place flips the pool's alignment pad, which shifts every
// pc offset and branch until the next pool (about 30 of the rows). The rest
// are single rows where the native builds a constant or issues a load one
// slot earlier (the landed dagger's sheet address and its flip index).

// Tried:
// the twin, cell tables replaced by 32x32 cells              38

#include "types.h"
#include "ram_buffer.h"
#include "io_reg.h"
#include "battle_effect.h"
#include "battle_effect_work.h"
#include "effect_step.h"

typedef DrawRectangle BlitterPair[2];

// The blitters' heap slots, as byte offsets into Ram_HeapSlots.
#define SLOT_BLITTER 104           // unk64[1]
#define SLOT_BLITTER_ALTERNATE 188 // unkb8[1]

// ⚓️'s blitter modes run the other way from The Broken Seal's.
enum { BLIT_ADDITIVE, BLIT_MAX, BLIT_TRANSPARENT, BLIT_COPY };
#define BLITTER(mode, flags) ((mode) << 4 | (flags))
#define CLIP (COMPOSITE_CLIP_X | COMPOSITE_CLIP_Y)

#define CALLBACK_ORDER 0xc80

// The pixel half of a 16.16 value.
#define HI(value) (((s16 *)&(value))[1])

extern char ResourceId_CrossedDaggersSheet;

void BattleEffect_StartCanvasLayer(s32 control);
void BattleEffect_EndCanvasLayer(void); // BattleEffect_EndCanvasLayer
s32 BattleEffect_LoadBlitter(s32 slot, s32 flags);
s32 BattleEffect_ProcessPendingGraphicsTransfer(void); // the canvas transfer callback
s32 Scheduler_AddOrUpdateCallback(void *callback, s32 order);
s32 Scheduler_RemoveCallback(void *callback);
void Runtime_ReleaseHeapBlock(s32 slot);
void Audio_PlayCue(s32 cue);
u32 Random16(void);
void Func_0814cd48(s32 actor, s32 palette, s32 frames, s32 timerSlot,
                   s32 timerFrames); // ObjectGroup_UpdateMembers
void Func_08158ce0(s32 x, s32 y);    // Camera_ApplyShake
void ObjectGroup_TickMemberTimers(void);            // ObjectGroup_TickMemberTimers
void WaitTicks(s32 ticks);

// A rain of daggers on the first target (unused).

// Sixteen daggers fall 8 pixels a frame, one every 4 frames, from columns
// 16 to 111, drifting towards the middle (x 64) the further out they start,
// and stick in the ground where they stop. The canvas fades in over 16
// frames and out from frame 104; from frame 24 to 84 the target flashes and
// the screen shakes every 4 frames, with a sound every 8.

void Func_0813e114(struct BattleEffectArgument *effect) {
  struct EffectPosition pos;
  BlitterPair draw;
  struct BattleEffectWork *work;
  void *canvas;
  DrawRectangle *blit;
  struct EffectStep *dagger;
  s32 frame;
  s32 i;

  work = (struct BattleEffectWork *)Ram_HeapSlots->battleFxWork[0];
  canvas = Ram_HeapSlots->battleFxWork[1];
  BattleEffect_StartCanvasLayer(1);
  REG_BG2PA = 0x100;
  REG_BLDALPHA = 0x1000;

  // [0] as drawn, [1] mirrored left to right; both skip clear pixels.
  BattleEffect_LoadBlitter(SLOT_BLITTER, BLITTER(BLIT_TRANSPARENT, CLIP));
  draw[0] = (DrawRectangle)Ram_HeapSlots->unk64[1];
  BattleEffect_LoadBlitter(SLOT_BLITTER_ALTERNATE,
                BLITTER(BLIT_TRANSPARENT, CLIP | COMPOSITE_FLIP_X));
  draw[1] = (DrawRectangle)Ram_HeapSlots->unkb8[1];
  blit = draw;
  Resource_LoadAndDecompress((s32)&ResourceId_CrossedDaggersSheet, work->sheet,
                             1, 1);
  work->transferMode = 1; // each transfer clears the canvas
  work->transferValue = 0;
  Scheduler_AddOrUpdateCallback(BattleEffect_ProcessPendingGraphicsTransfer, CALLBACK_ORDER);

  // Centre BG2 on the first target.
  EffectPosition_ProjectScaledBattler(effect->actors[0], &pos);
  REG_BG2X = (64 - pos.x) << 8;

  // Daggers start in columns 16 to 111; the further from the middle, the
  // faster they drift towards it (in steps of 2 pixels a frame).
  for (i = 0; i != 64; i++) {
    s32 column;

    dagger = &work->particles[i];
    column = Random16() % 96 + 16;
    dagger->x = column;
    dagger->y = (24 - i / 4) << 16;
    if (column <= 43)
      dagger->velocityX = 3;
    else if (column <= 51)
      dagger->velocityX = 2;
    else if (column <= 59)
      dagger->velocityX = 1;
    else if (column <= 67)
      dagger->velocityX = 0;
    else if (column <= 75)
      dagger->velocityX = -1;
    else if (column <= 83)
      dagger->velocityX = -2;
    else
      dagger->velocityX = -3;
    dagger->velocityX <<= 17;
    dagger->velocityY = 8 << 16;
    dagger->x <<= 16;
  }
  Audio_PlayCue(212);

  for (frame = 0; frame != 120; frame++) {
    if (frame <= 16) {
      REG_BLDALPHA = frame | 0x1000;
      if (frame == 16)
        REG_BLDCNT = 0;
    }
    if (frame > 103) {
      REG_BLDALPHA = (120 - frame) | 0x1000;
      if (frame == 104)
        REG_BLDCNT = 0x3f44;
    }

    // The drift picks the picture; a dagger going left is drawn mirrored.
    // Each one falls from 16 frames after its turn until it lands at 25.
    for (i = 15; i != -1; i--) {
      s32 drift;
      s32 speed;
      s32 cell;

      dagger = &work->particles[i];
      drift = dagger->velocityX;
      speed = drift;
      if (drift < 0)
        speed = -drift;
      cell = speed >> 17;
      if (frame < i * 4 + 25) {
        blit[(u32)drift >> 31](canvas, work->sheet + (cell << 10),
                               HI(dagger->x) - 16, HI(dagger->y) - 16, 32, 32);
        if (frame >= i * 4 + 16) {
          dagger->x += dagger->velocityX;
          dagger->y += dagger->velocityY;
        }
      } else {
        blit[(u32)drift >> 31](canvas, work->sheet + (cell << 10),
                               HI(dagger->x) - 16, HI(dagger->y) - 16, 32, 32);
      }
    }

    // From frame 24 to 84, every 4 frames: flash the target for 2 frames
    // and shake the screen; every 8, a sound.
    if (frame >= 23 && frame <= 87 && (frame & 3) == 0) {
      Func_0814cd48(effect->actors[0], 7, 5, 0, 2);
      work->shakeFrames = 1;
      if ((frame & 7) == 0)
        Audio_PlayCue(133);
    }
    Func_08158ce0(8, 8);
    ObjectGroup_TickMemberTimers();
    work->transferPending = 1;
    WaitTicks(1);
  }

  Scheduler_RemoveCallback(BattleEffect_ProcessPendingGraphicsTransfer);
  Runtime_ReleaseHeapBlock(SLOT_BLITTER_ALTERNATE);
  Runtime_ReleaseHeapBlock(SLOT_BLITTER);
  BattleEffect_EndCanvasLayer();
}
