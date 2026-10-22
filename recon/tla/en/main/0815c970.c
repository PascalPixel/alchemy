// Draft of Func_0815c970, the two-resource family's body (two kinds
// through two wrappers): a magenta swirl strikes the first target.

// On screen: a swirl uncoils over the first target in six cels, four
// frames each, the first four from the swirl sheet and the last two from
// the tail sheet, on the target's side of the canvas. At frame 8 the hit
// lands with an eight-frame shake: in mode 0 the battle resumes with hit
// cue 0x85 and the target recoils; in mode 1 cue 0x86 plays, the target
// flashes, at frame 13 it is thrown up into the air, and at frame 65 it
// lands with a short shake and the battle resumes with hit cue 0x86. Mode
// 0 takes the lime palette and lasts 48 frames; mode 1 lasts 74.

// Setup: the battle effect canvas at a scale of one, a transparent blitter for
// the caster's side, the swirl sheet in the effect work and the tail sheet
// in the map cell buffer, the transfer callback, and BG2 placed on the
// target.
// Frame loop, 48 or 74 frames, as above.
// Teardown: the callback and the blitter are released and the canvas
// layer is closed.

// Port of The Broken Seal's BattleEffect_RunTwoResource, with The Lost Age's
// heap slots, blitter mode (3 minus ☀️'s) and callback order, and no
// work->effect store.

// Listing 260 instructions, draft 252, differ 202. The logic and constants
// agree; the remaining rows are register allocation. The native keeps
// effect in r8, work in r10, mode in r11, the blitter in r6 and the heap
// base in r5 across the first two calls, spills canvas and frames to the
// stack (28-byte frame) and leaves r7 unused (pushed only to save the high
// registers: reload took it back as a spill register); here effect, work
// and canvas take r6, r7 and fp.

// Tried:
// the heap base through a local (the native keeps it in r5)  matched rows
// the blitter load in each branch, which cross-jumps the
// two loader calls into one, as the native has it          matched rows
// a conditional argument for the loader (trial 0)            213
// BattleEffect_LoadBlitter returning s32                                202
// declaration order, a greedy sweep of 9 locals              202

#include "types.h"
#include "battle_effect_work.h"
#include "battle_effect.h"
#include "battle_session.h"
#include "effect_step.h"
#include "io_reg.h"
#include "iwram_call.h"
#include "motobj.h"
#include "ram_buffer.h"

// Byte offset of the blitter slot in gHeapSlots (unk64[1]).
#define HEAP_BLITTER 104

// The Lost Age numbers the blitter modes the other way round from The
// Broken Seal's COMPOSITE_ modes; BattleEffect_LoadBlitter takes mode << 4 | flags.
enum { BLIT_ADDITIVE, BLIT_MAX, BLIT_TRANSPARENT, BLIT_COPY };
#define BLITTER(mode, flags) ((mode) << 4 | (flags))

extern char ResourceId_MagentaSwirlSheet;
extern char ResourceId_MagentaTailSheet;
extern char ResourceId_LimePalette;
// The swirl's six cels: ☀️'s gSpiderWeb_Cells, _CellX (six
// for each side), _CellWidths, _CellBiasY and _CellHeights.
extern const u16 gData_081986d4[];
extern const u8 gData_081986e0[];
extern const u8 gData_081986c8[];
extern const s8 gData_081986ec[];
extern const u8 gData_081986ce[];

void WaitFrames(s32 frames);
void Audio_PlayCue(s32 cue);
void BattleEffect_StartCanvasLayer(s32 bgControl);
// Closes the canvas layer.
void BattleEffect_EndCanvasLayer(void);
// Copies the canvas to VRAM when a transfer is pending.
void BattleEffect_ProcessPendingGraphicsTransfer(void);
// Counts down the actors' flash timers.
void ObjectGroup_TickMemberTimers(void);
// Builds a blitter in a heap slot: BattleEffect_BuildBlitter(slot, 7, 7,
// flags, mode), whose result it returns.
s32 BattleEffect_LoadBlitter(s32 slot, s32 modeFlags);
// BattleEventRuntime_StartPhase through its far call.
void Func_081180e8(s32 hitCue);
// An actor's recoil motion, through a far call.
void Func_08118088(s32 actor, s32 motion);
// Flashes an actor in a palette for some frames.
void Func_0814cd48(s32 actor, s32 palette, s32 group, s32 timerSlot,
                   s32 frames);
// Shakes the camera while work->shakeFrames lasts.
void Func_08158ce0(s32 randomMask, u32 shakeRange);
void *Resource_GetTableEntry(s32 id);
s32 Scheduler_AddOrUpdateCallback(void (*callback)(void), s32 order);
s32 Scheduler_RemoveCallback(void (*callback)(void));
void Runtime_ReleaseHeapBlock(s32 slot);

void Func_0815c970(struct BattleEffectArgument *effect, s32 mode) {
  struct HeapSlots *slots;
  struct BattleEffectWork *work;
  void *canvas;
  struct MotionObject *object;
  struct EffectPosition position;
  DrawRectangle draw;
  s32 frames;
  s32 frame;
  s32 cel;

  // Setup: the canvas at a scale of one, a transparent blitter, mirrored
  // for side 0.
  slots = Ram_HeapSlots;
  work = (struct BattleEffectWork *)slots->battleFxWork[0];
  canvas = slots->battleFxWork[1];
  BattleEffect_StartCanvasLayer(0);
  REG_BG2PA = 0x100;
  if (effect->side == 1) {
    BattleEffect_LoadBlitter(HEAP_BLITTER, BLITTER(BLIT_TRANSPARENT,
                                        COMPOSITE_CLIP_X | COMPOSITE_CLIP_Y));
    draw = (DrawRectangle)slots->unk64[1];
  } else {
    BattleEffect_LoadBlitter(HEAP_BLITTER, BLITTER(BLIT_TRANSPARENT,
                                        COMPOSITE_CLIP_X | COMPOSITE_CLIP_Y |
                                            COMPOSITE_FLIP_X));
    draw = (DrawRectangle)slots->unk64[1];
  }
  Resource_LoadAndDecompress((s32)&ResourceId_MagentaSwirlSheet, work->sheet, 1,
                             1);
  Resource_LoadAndDecompress((s32)&ResourceId_MagentaTailSheet,
                             Ram_MapCellBuffer, 1, 0);
  if (mode == 0) {
    void *palette = Resource_GetTableEntry((s32)&ResourceId_LimePalette);
    Iwram_CopyWords((void *)0x05000000, palette, 128);
  }
  work->transferMode = 2;
  work->transferValue = 75;
  Scheduler_AddOrUpdateCallback(BattleEffect_ProcessPendingGraphicsTransfer, 0xc80);

  // BG2 is placed on the first target.
  object = GetBattleObjectSlotFar(effect->actors[0])->object;
  EffectPosition_ProjectScaledBattler(effect->actors[0], &position);
  if (effect->side == 0)
    REG_BG2X = (16 - position.x) << 8;
  else
    REG_BG2X = (112 - position.x) << 8;
  frames = 74;
  if (mode != 1)
    frames = 48;

  for (frame = 0; frame != frames; frame++) {
    // The swirl: six cels, four frames each, the last two from the tail.
    cel = frame / 4;
    if (cel <= 5) {
      if (cel <= 3)
        draw(canvas, work->sheet + gData_081986d4[cel],
             gData_081986e0[cel + effect->side * 6], gData_081986ec[cel] + 32,
             gData_081986c8[cel], gData_081986ce[cel]);
      else
        draw(canvas, Ram_MapCellBuffer + gData_081986d4[cel],
             gData_081986e0[cel + effect->side * 6], gData_081986ec[cel] + 32,
             gData_081986c8[cel], gData_081986ce[cel]);
    }
    // The hit.
    if (frame == 8) {
      if (mode == 0) {
        Func_081180e8(0x85);
        Func_08118088(effect->actors[0], 1);
      } else {
        Audio_PlayCue(0x86);
        Func_0814cd48(effect->actors[0], 7, 5, 0, 4);
      }
      work->shakeFrames = 8;
    }
    // Mode 1 throws the target up at frame 13; it lands at frame 65.
    if (mode == 1) {
      if (frame == 13) {
        object->velocityY = 0xc0000;
        object->verticalMotionStrength = 0x7851;
        object->verticalMotionPhase = 0x4000;
      }
      if (frame == 65) {
        work->shakeFrames = 4;
        Func_081180e8(0x86);
      }
    }
    Func_08158ce0(8, 8);
    ObjectGroup_TickMemberTimers();
    work->transferPending = 1;
    WaitFrames(1);
  }

  // Teardown.
  Scheduler_RemoveCallback(BattleEffect_ProcessPendingGraphicsTransfer);
  Runtime_ReleaseHeapBlock(HEAP_BLITTER);
  BattleEffect_EndCanvasLayer();
}
