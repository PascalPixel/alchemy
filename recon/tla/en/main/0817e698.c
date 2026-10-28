// 2026-10-25, with the Makefile's options (-mthumb-slow-loads): differ 20 of
// 386; a greedy device search found nothing lower.
// Draft (tla-en): listing 386 instructions, draft 386, common 374, differ 24.
// The body of effect kinds 348, 353, 389 and 431 (BattleEffect_RunSpasm to
// BattleEffect_RunSack in variant_k.c pass modes 0 to 3). Written from the
// listing; ☀️'s BattleEffect_RunFortyEightFrameEffect has the same frame count
// and shape (0.55) but is not its twin.
// The shape matches, including the three mirrored copies as indexed loops
// (written with walking pointers loop.c makes outer givs of them: differ
// 206). Left: in each copy's inner loop the native steps the left pointer
// before its second load (ldrb, add r4, strb, add r0, ldrb, add r2, strb,
// sub r1); the draft loads first. After the calls the native loads the
// pool word (IwramClearWords, IwramCopyWords, BG2PA's 0x100) before the
// neighbouring constant, and the first "mov r3, #192" comes after the
// argument's spill. u32 or u8 counters and a s8 canvas change nothing.
// The copy loops' order is what sched2 gives when a load after a store to
// the canvas waits two cycles (here one): the second ldrb is ready at
// once and goes before the left pointer's add. Measured against it: an
// empty asm or a volatile memory barrier between the copies (loop.c
// then rebuilds the row pointers: differ 166), a one-pass block around
// the second copy (differ 278), a volatile load or store (the address
// givs no longer combine: 166 to 267), an asm with an unused output
// (deleted), and walking pointers per row (206).
// 2026-10-15 (lane N2): rewritten with the work struct, one mirroring macro
// for the three copies and named constants; the same 386 instructions and
// differ 24.
// Resources 0x13c (BattleEffect_EarthWallSheet), 0x148 (BattleEffect_OrangePaletteB),
// 0x129 (BattleEffect_FireballSheet) and 0x150 (BattleEffect_MarsDjinnSheet) need
// their names in system/resource/directory.s when this is adopted.
#include "battle_effect_work.h"
#include "effect_step.h"
#include "io_reg.h"
#include "iwram_call.h"
#include "ram_buffer.h"
#include "types.h"

#define HEAP_SLOT_BLITTER 104 // Ram_HeapSlots->unk64[1]

// The Lost Age's blitter modes run the other way from The Broken Seal's:
// additive 0, max 1, transparent 2, copy 3.
#define BLITTER_MAX 1
#define BLITTER(mode, flags) ((mode) << 4 | (flags))
#define CLIP_XY 3

#define BURST_FRAMES 48
#define BURST_DRAW_FRAMES 28

// The three 64x64 cels of the burst, built in the sheet's first 0x3000
// bytes; the picture loads behind them.
#define CEL_SIZE 0x1000
#define PICTURE_OFFSET 0x4000

// Copies a half-picture of width by height pixels into the bottom middle of
// a 64x64 cel, once as it is and once mirrored, so the cel is symmetrical.
#define MIRROR_HALF(cel, half, width, height)                                  \
  for (y = 0; y != (height); y++) {                                            \
    for (x = 0; x != (width); x++) {                                           \
      work->sheet[(cel) * CEL_SIZE + (y + 64 - (height)) * 64 + 32 - (width) + \
                  x] = work->sheet[(half) + y * (width) + x];                  \
      work->sheet[(cel) * CEL_SIZE + (y + 64 - (height)) * 64 + 31 + (width) - \
                  x] = work->sheet[(half) + y * (width) + x];                  \
    }                                                                          \
  }

// A burst cel's texture: log2 of its width and height in the first word
// (64x64 here), then its pixels.
struct BurstTexture {
  u32 size;
  void *pixels;
};

// One entry of the list Render_DrawModelList draws; kind 6 is a textured quad.
struct RenderPrimitive {
  s32 kind;
  s32 unk04;
  const void *faces;
  void *vertices;
  struct BurstTexture *texture;
  s32 unk14;
};

extern char ResourceId_EarthWallSheet;
extern char ResourceId_OrangePaletteB;
extern char ResourceId_FireballSheet;
extern char ResourceId_MarsDjinnSheet;

// The burst quad: its face list and its four corners, 64x64 pixels standing
// on its base.
extern const u8 gData_08199340[];
extern const u8 gData_081991f0[];

// Per mode: the quad's scale (16.16, doubled when used) and its unk14.
extern const s32 gData_081994d0[];
extern const s32 gData_081994e0[];

void BattleEffect_StartCanvasLayer(s32 bgControl);
void Resource_LoadAndDecompress(s32 resourceId, void *target, s32 unpack,
                                s32 wait);
void *Resource_GetTableEntry(s32 resourceId);
s32 Task_Add(s32 callback, s32 priority);
s32 Task_Remove(s32 callback);
void EffectPosition_ProjectBattler(
    s32 id, struct EffectPosition *position);
void Render_ResetTransformState(void);
void Sound_PlayFar(s32 cue);
void Func_081180e8(s32 phase);
s32 BattleEffect_LoadBlitter(s32 slot, s32 blitter);
void Func_0814cd48(s32 actor, s32 palette, s32 member, s32 timer, s32 frames);
void *Heap_AllocEwram(s32 size);
void HeapSlot_Release(s32 slot);
struct RenderPrimitive *Render_AllocateModelList(s32 count);
void Func_08015160(s32 x, s32 y, s32 z);
void Func_0801521c(s32 scale);
void Render_TransformVertices(const void *corners, void *vertices, s32 count);
void Render_DrawModelList(struct RenderPrimitive *list);
void Heap_Release(void *buffer);
void Func_08158ce0(s32 x, s32 y);
void ObjectGroup_CountDownMemberTimers(void);
void WaitFrames(s32 frames);
void BattleEffect_ProcessPendingGraphicsTransfer(void);
void BattleEffect_EndCanvasLayer(void);

// A symmetrical burst over the target. Three half-pictures of the earth
// wall sheet are mirrored into three 64x64 cels; the burst grows through
// them in its first twelve frames, flickers between the last two until
// frame 28 and is gone, while the screen shakes for all 48 frames. The
// mode picks the palette (Mars Djinn, Mars Djinn, fireball, orange), the
// scale, how hard the screen shakes and the target's flash colour.
void Func_0817e698(struct BattleEffectArgument *effect, s32 mode) {
  struct BattleEffectWork *work =
      (struct BattleEffectWork *)Ram_HeapSlots->battleFxWork[0];
  struct BurstTexture texture;
  struct EffectPosition target;
  struct RenderPrimitive *quad;
  void *vertices;
  s32 frame;
  s32 cel;
  s32 y;
  s32 x;

  // Build the three cels from their halves.
  BattleEffect_StartCanvasLayer(0);
  Resource_LoadAndDecompress((s32)&ResourceId_EarthWallSheet,
                             work->sheet + PICTURE_OFFSET, 1, 1);
  Iwram_ClearWords(work->sheet, 0x4000);
  MIRROR_HALF(0, PICTURE_OFFSET + 0xa8e, 17, 49)
  MIRROR_HALF(1, PICTURE_OFFSET + 0xdcf, 19, 55)
  MIRROR_HALF(2, PICTURE_OFFSET + 0x11e4, 20, 64)
  if (mode == 3)
    Iwram_CopyWords((void *)BG_PLTT,
                    Resource_GetTableEntry((s32)&ResourceId_OrangePaletteB),
                    128);
  else if (mode == 2)
    Iwram_CopyWords((void *)BG_PLTT,
                    Resource_GetTableEntry((s32)&ResourceId_FireballSheet),
                    128);
  else
    Iwram_CopyWords((void *)BG_PLTT,
                    Resource_GetTableEntry((s32)&ResourceId_MarsDjinnSheet),
                    128);

  // Centre the canvas's x 64 on the target.
  EffectPosition_ProjectBattler(effect->actors[0], &target);
  REG_BG2PA = 0x100;
  REG_BG2X = (64 - target.x) << 8;
  BattleEffect_LoadBlitter(HEAP_SLOT_BLITTER, BLITTER(BLITTER_MAX, CLIP_XY));
  work->transferMode = 2;
  work->transferValue = 50;
  Task_Add((s32)BattleEffect_ProcessPendingGraphicsTransfer, 0xc80);
  if (mode == 2) {
    work->shakeFrames = 0;
    Sound_PlayFar(212);
  } else if (mode == 1 || mode == 3) {
    work->shakeFrames = 8;
    Sound_PlayFar(212);
  } else {
    work->shakeFrames = 32;
  }

  for (frame = 0; frame != BURST_FRAMES; frame++) {
    if (frame == 0) {
      if (mode == 2)
        Func_0814cd48(effect->actors[0], 7, -1, 0, 32);
      else
        Func_0814cd48(effect->actors[0], 10, -1, 0, 32);
    }
    if (frame == 24)
      Func_081180e8(0);
    if (frame == 8 && mode == 0)
      Sound_PlayFar(126);

    // Cels 0, 1 and 2 four frames each, then 1 and 2 by turns.
    if (frame < 32) {
      cel = frame / 4;
      if (cel > 2)
        cel = (cel & 1) + 1;
      if (frame < BURST_DRAW_FRAMES) {
        vertices = Heap_AllocEwram(32);
        quad = Render_AllocateModelList(1);
        texture.size = (texture.size & ~0xff) | 6;
        texture.size = (texture.size & ~0xff00) | 6 << 8;
        texture.pixels = work->sheet + cel * CEL_SIZE;
        quad->kind = 6;
        quad->faces = gData_08199340;
        quad->texture = &texture;
        quad->unk14 = gData_081994e0[mode];
        quad->vertices = vertices;
        Render_ResetTransformState();
        Func_08015160(0, (target.y - 58) << 16, 0);
        Func_0801521c(gData_081994d0[mode] << 1);
        Render_TransformVertices(gData_081991f0, vertices, 4);
        Render_DrawModelList(quad);
        Heap_Release(quad);
        Heap_Release(vertices);
      }
    }

    if (mode == 0)
      Func_08158ce0(4, 4);
    else
      Func_08158ce0(16, 16);
    ObjectGroup_CountDownMemberTimers();
    work->transferPending = 1;
    WaitFrames(1);
  }

  Task_Remove((s32)BattleEffect_ProcessPendingGraphicsTransfer);
  HeapSlot_Release(HEAP_SLOT_BLITTER);
  BattleEffect_EndCanvasLayer();
}
