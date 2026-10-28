#include "battle_effect.h"
#include "battle_effect_work.h"
#include "io_reg.h"
#include "iwram_call.h"
#include "ram_buffer.h"
#include "types.h"

// The blitter heap slots, by byte offset into Ram_HeapSlots.
#define HEAP_SLOT_BLITTER 104 // unk64[1]

// The Lost Age numbers its blitter modes from the other end of The Broken
// Seal's: additive 0, max 1, transparent 2, copy 3.
#define BLITTER_MAX 1
#define BLITTER(mode, flags) ((mode) << 4 | (flags))

// The pixel half of a 16.16 value.
#define HI(v) (((s16 *)&(v))[1])

#define BUBBLE_COUNT 256
#define BUBBLE_FRAMES 60
#define BUBBLE_HIT_CUE 126

extern char ResourceId_BlueBeamSheet;
extern char ResourceId_Data0FD;

// 256 bubbles past the map cells. Each is an EffectStep with its cel in z
// and its life in variant (-1 once it has left the top of the canvas).
extern struct EffectStep gData_02015000[];

// The fifteen cels of Resource_Data0FD, a glowing oval from 16x32 down to
// 3x4: their widths, heights and byte offsets into the sheet.
extern u8 gData_0819966f[];
extern u8 gData_0819967e[];
extern u16 gData_0819968e[];

void BattleEffect_ProcessPendingGraphicsTransfer(void);
s32 Task_Add(void (*callback)(void), s32 order);
s32 Task_Remove(void (*callback)(void));
void BattleEffect_LoadBlitter(s32 slot, s32 blitter);
void *Resource_GetTableEntry(s32 id);
u32 Random16(void);
void BattleEffectKernels_IntegrateVector2(struct EffectStep *step, s32 damping,
                                      s32 gravity);
void Func_0814cd48(s32 actor, s32 palette, s32 member, s32 timer, s32 frames);
void Func_081180e8(s32 phase);
void Sound_PlayFar(s32 cue);
void BattleEffect_CountDownBattlerTints(void);
void WaitFrames(s32 frames);
void HeapSlot_Release(s32 slot);
void BattleEffect_EndCanvasLayer(void);

// A column of bubbles. On the first frame 256 glowing ovals of random size
// appear over the bottom left of the canvas; for 60 frames they float up
// and drift sideways, drawn with the max blitter in the blue beam's palette,
// and each one is gone once it has risen past the top. From frame 18 the
// targets flash one by one, eight frames apart, each with the hit sound.
void Func_08182898(struct BattleEffectArgument *effect) {
  void *canvas = Ram_HeapSlots->battleFxWork[1];
  struct BattleEffectWork *work;
  /* FAKEMATCH(dead): the blitter pair, of which this effect uses the first,
   * spills as one DImode pseudo into the unused frame word the native has;
   * as a single DrawRectangle, 8 of 225 instructions differ in tla-en. */
  DrawRectangle draw[2];
  s32 frame;
  s32 i;
  struct EffectStep *bubble;

  // Each frame the transfer callback copies the canvas and clears it.
  {
    /* FAKEMATCH(pin): the callback pinned to r0 behind an empty asm that
     * holds r1, with work loaded after it, so the pool load comes before
     * work's; as Task_Add(BattleEffect_..., 0xc80) with work
     * set at its declaration, 2 of 225 instructions differ in tla-en. */
    register void (*callback)(void) __asm__("r0") =
        BattleEffect_ProcessPendingGraphicsTransfer;

    __asm__("" : "+l"(callback) : : "r1");
    work = (struct BattleEffectWork *)Ram_HeapSlots->battleFxWork[0];
    Task_Add(callback, 0xc80);
  }
  work->transferMode = 2;
  work->transferValue = 50;
  BattleEffect_LoadBlitter(HEAP_SLOT_BLITTER,
                BLITTER(BLITTER_MAX, COMPOSITE_CLIP_X | COMPOSITE_CLIP_Y));
  draw[0] = (DrawRectangle)Ram_HeapSlots->unk64[1];

  // The blue beam's palette colours the bubble cels.
  Iwram_CopyWords((void *)BG_PLTT,
                  Resource_GetTableEntry((s32)&ResourceId_BlueBeamSheet), 128);
  Resource_LoadAndDecompress((s32)&ResourceId_Data0FD, work->sheet, 0, 0);

  for (frame = 0; frame != BUBBLE_FRAMES; frame++) {
    // The first frame scatters every bubble below the bottom left.
    if (frame == 0) {
      struct EffectStep *spawn = gData_02015000;

      for (i = 0; i != BUBBLE_COUNT; i++) {
        spawn->x = (Random16() & 127) << 16;
        spawn->y = ((Random16() & 127) + 120) << 16;
        spawn->velocityX = ((Random16() & 31) - 16) << 13;
        spawn->velocityY = (8 - (Random16() & 31)) << 12;
        spawn->z = (Random16() & 7) + 5;
        spawn->variant = 32;
        spawn++;
      }
    }

    // Draw each bubble centred on its place, let it rise, and drop it once
    // it is wholly above the canvas.
    bubble = gData_02015000;
    for (i = 0; i != BUBBLE_COUNT; i++) {
      if (bubble->variant >= 0) {
        s32 cel = bubble->z;
        u32 width;
        u32 height;

        draw[0](canvas, work->sheet + gData_0819968e[cel],
                HI(bubble->x) - ((width = gData_0819966f[cel]) >> 1),
                HI(bubble->y) - ((height = gData_0819967e[cel]) >> 1), width,
                height);
        BattleEffectKernels_IntegrateVector2(bubble, 63, -0x4000);
        if (bubble->y < -(gData_0819967e[cel] << 16))
          bubble->variant = -1;
      }
      bubble++;
    }

    // The targets flash in turn from frame 18; the first hit also starts
    // battle phase 126.
    for (i = 0; i != effect->count; i++) {
      if (frame == i * 8 + 18) {
        if (i == 0) {
          Func_0814cd48(effect->actors[i], 7, -1, i, 16);
          Func_081180e8(BUBBLE_HIT_CUE);
          Sound_PlayFar(BUBBLE_HIT_CUE);
        } else {
          Sound_PlayFar(BUBBLE_HIT_CUE);
          Func_0814cd48(effect->actors[i], 7, -1, i, 16);
        }
      }
    }

    BattleEffect_CountDownBattlerTints();
    work->transferPending = 1;
    WaitFrames(1);
  }

  HeapSlot_Release(HEAP_SLOT_BLITTER);
  Task_Remove(BattleEffect_ProcessPendingGraphicsTransfer);
  BattleEffect_EndCanvasLayer();
}
