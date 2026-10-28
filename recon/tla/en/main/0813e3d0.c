// Near miss, differ 16 of 456.
//
// Measured 2026-10-25 under -mthumb-slow-loads: differ 16; an empty asm before
// the first statement does not help (differ 20).

// Twin: The Broken Seal's BattleEffect_RunElementPillar, reached from the
// scenes through the far-call table (table_47.s). It sets up its own heap
// blocks, so unlike a dispatched effect it stores work->effect and marks
// the work active (vblanksSinceTransfer = 1) itself. ⚓️ drops the twin's
// unused GetBattleObjectSlotFar call, works out the glow's picture again for
// the mirrored half, and reads the strength from the argument.

// Left: order only; the allocation matches. Single rows where the native
// builds a constant or issues a load one slot earlier (the heap base and the
// work fields after the allocations, BLDALPHA's value, the pillar's sheet
// offsets, the fleck's y and step constants), and the pool offsets they
// move.

// Tried:
// the twin as ported (its two FAKEMATCH devices dropped)      261
// the work block's real size, 0x7b7c                          257
// the twin's devices: work++; work--; and frame ^= 1 twice    261-263
// frame / 2 % 4 written out in both halves                   263
// alchemy permute, 10 minutes: a one-pass block after the
// left half's pillar (tagged)                              129
// the one-pass block around the whole left half              133
// the pillar's picture as frame / 2 % 4 in each half, with no
// shared index: i had held frame / 2, so the loops' counter
// shared the pillar's register (129 -> 42; a step local of
// its own gives 56)                                         42
// the same without the one-pass block                        259

#include "types.h"
#include "ram_buffer.h"
#include "io_reg.h"
#include "mem.h"
#include "battle_effect.h"
#include "battle_effect_work.h"
#include "effect_step.h"
#include "battle_display.h"
#include "iwram_call.h"
#include "fixed_math.h"
#include "graphics.h"
#include "transform.h"
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

// The flecks live in the map cell buffer.
#define gFlecks ((struct EffectStep *)Ram_MapCellBuffer)

// The fleck's cell for each size from 1 to 6 (gBattleEffect_FlareCells).
extern u16 Data_08197424[];

extern char ResourceId_IceTileSheet;
extern char ResourceId_ParticleSpritesD;
extern char ResourceId_YellowPaletteA;
extern char ResourceId_RedPaletteA;
extern char ResourceId_VioletPaletteA;

void BattleEffect_StartCanvasLayer(s32 control);
void BattleEffect_EndCanvasLayer(void); // BattleEffect_EndCanvasLayer
s32 BattleEffect_LoadBlitter(s32 slot, s32 flags);
s32 Task_Add(void *callback, s32 order);
s32 Task_Remove(void *callback);
void Runtime_ReleaseHeapBlock(s32 slot);
void Sound_PlayFar(s32 cue);
u32 Random16(void);
void EffectPosition_ProjectBattler(s32 actor,
                                                 struct EffectPosition *out);
void Func_0814cd48(s32 actor, s32 palette, s32 frames, s32 timerSlot,
                   s32 timerFrames); // ObjectGroup_UpdateMembers
void ObjectGroup_CountDownMemberTimers(void);            // ObjectGroup_CountDownMemberTimers
void WaitFrames(s32 frames);

// A pillar of ice rising at the user's feet with flecks spiralling up
// around it, for the scenes.

// The pillar is two mirrored halves of a 17x64 picture with a 24x43 glow,
// drawn at their maximum: four pillar pictures two frames apart and three
// glow pictures four frames apart. 32 flecks spiral up around it, one more
// each frame, turning 0x400 and rising 5 pixels a frame while the spiral
// widens. effect->kind picks the colours (yellow, ice, red or violet) and
// effect->variant + 1, from 1 to 4, the flecks' size. At frame 16 the first
// target flashes; the last 6 frames fade out.

void Func_0813e3d0(struct BattleEffectArgument *effect) {
  struct EffectPosition ground;
  struct EffectPosition pos;
  s32 point[3];
  struct BattleEffectWork *work;
  void *canvas;
  BlitterPair draw;
  u8 *sheet;
  struct BattleCamera *camera;
  s32 frame;
  void *palette;
  s32 i;

  work = Runtime_AllocateHeapBlock(SLOT_BATTLE_EFFECT, BATTLE_EFFECT_WORK_SIZE);
  canvas = Runtime_AllocateHeapBlock(SLOT_BATTLE_CANVAS, 0x4000);
  sheet = Runtime_AllocateHeapBlock(SLOT_BATTLE_SHEET, 0x60e);
  camera = Ram_HeapSlots->cameraWork;
  work->vblanksSinceTransfer = 1;
  work->effect = effect;
  BattleEffect_StartCanvasLayer(0);
  work->fadeFrames = 24;
  work->fadeStep = 0;
  REG_BLDALPHA = 0x100c;
  REG_BG2PA = 0x100;
  Resource_LoadAndDecompress((s32)&ResourceId_IceTileSheet, work->sheet, 1, 0);
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

  // A fleck's x is its angle and z its radius, in 1/16 pixels; fleck i
  // waits i frames before it rises.
  for (i = 0; i != 128; i++) {
    gFlecks[i].y = 0;
    gFlecks[i].x = Random16() & 0xffff;
    gFlecks[i].z = (Random16() & 0x1ff) + i * 2;
    gFlecks[i].variant = -i;
  }
  work->transferMode = 2;   // each transfer leaves 3/4 of the old picture,
  work->transferValue = 75; // a trail
  Task_Add(BattleEffect_ProcessPendingGraphicsTransfer, TASK_PASS_FRAME);
  BattleEffect_LoadBlitter(SLOT_BLITTER, BLITTER(BLIT_ADDITIVE, CLIP));
  draw[0] = (DrawRectangle)Ram_HeapSlots->unk64[1];
  effect->variant++;
  if (effect->variant <= 0)
    effect->variant = 1;
  if (effect->variant > 4)
    effect->variant = 4;
  Sound_PlayFar(212);

  for (frame = 0; frame != 56; frame++) {
    EffectPosition_ProjectBattler(effect->actor, &ground);
    REG_BG2X = (64 - ground.x) << 8;
    if (frame > 49)
      REG_BLDALPHA = (112 - frame * 2) | 0x1000;
    if (frame == 16)
      Func_0814cd48(effect->actors[0], 7, -1, 0, 20);

    // The pillar: the left half at x 47, then the right half mirrored at
    // x 64, each with its glow.
    if (frame <= 55) {
      s32 n;

      n = frame / 2 % 4; // a pillar picture every 2 frames
      BattleEffect_LoadBlitter(SLOT_BLITTER_ALTERNATE, BLITTER(BLIT_MAX, CLIP));
      /* FAKEMATCH: the native works out the pillar's and the glow's
       * pictures again for the mirrored half. The one-pass block ends CSE's
       * block, so they are not carried over (differ 259 without it, 42 with
       * it; found by alchemy permute). */
      do {
        draw[1] = (DrawRectangle)Ram_HeapSlots->unkb8[1];
        draw[1](canvas, work->sheet + n * 1088, 47, ground.y - 64, 17, 64);
        n = frame / 4 % 3; // a glow picture every 4 frames
      } while (0);
      draw[1](canvas, work->sheet + n * 1032 + 0x1100, 40, ground.y - 36, 24,
              43);
      Runtime_ReleaseHeapBlock(SLOT_BLITTER_ALTERNATE);
      BattleEffect_LoadBlitter(SLOT_BLITTER_ALTERNATE,
                    BLITTER(BLIT_MAX, CLIP | COMPOSITE_FLIP_X));
      draw[1] = (DrawRectangle)Ram_HeapSlots->unkb8[1];
      n = frame / 2 % 4;
      draw[1](canvas, work->sheet + n * 1088, 64, ground.y - 64, 17, 64);
      n = frame / 4 % 3;
      draw[1](canvas, work->sheet + n * 1032 + 0x1100, 64, ground.y - 36, 24,
              43);
      Runtime_ReleaseHeapBlock(SLOT_BLITTER_ALTERNATE);
    }
    Render_ResetTransformState();
    Transform_SetLookAt(camera->eye, camera->pos);

    // The first 32 flecks turn 0x400, rise 5 pixels and widen 4 pixels a
    // frame.
    for (i = 0; i != 32; i++) {
      struct EffectStep *fleck = &gFlecks[i];

      if (fleck->variant >= 0) {
        s32 size;

        point[0] = fleck->z * Trig_Sin(fleck->x) >> 4;
        point[2] = -(fleck->z * Trig_Cos(fleck->x) >> 4);
        point[1] = fleck->y;
        fleck->x += 0x400;
        fleck->y += 5 << 16;
        fleck->z += 64;
        EffectPosition_ProjectPoint(point, &pos);
        pos.x /= 2;
        size = (i & 1) + effect->variant;
        draw[0](canvas, sheet + Data_08197424[size - 1], pos.x - size,
                pos.y - size, size * 2, size * 2);
      }
      fleck->variant++;
    }
    ObjectGroup_CountDownMemberTimers();
    work->transferPending = 1;
    WaitFrames(1);
  }

  Runtime_ReleaseHeapBlock(SLOT_BLITTER);
  Task_Remove(BattleEffect_ProcessPendingGraphicsTransfer);
  BattleEffect_EndCanvasLayer();
  Runtime_ReleaseHeapBlock(SLOT_BATTLE_SHEET);
  Runtime_ReleaseHeapBlock(SLOT_BATTLE_CANVAS);
  Runtime_ReleaseHeapBlock(SLOT_BATTLE_EFFECT);
}
