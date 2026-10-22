// Draft, differ 188 of 468 (no device): the cloud swirl that
// Func_0813e7e0 runs after setting up the effect blocks, written from the
// listing (twins: BattleEffect_BeginCanvasLayer 0.45 by shape,
// BattleEffect_RunOrbitingSkulls 0.60 by call order). The rotation goes
// through six IO_WRITE_QUEUE_PUSH writes a frame, in the queue's order
// (value, address, control), as the native's inline pushes are.
// Left: allocation. The native holds canvas in r9 and work in r10, keeps
// the blitter it loads first at sp+20 and IwramCopyWords' address in r6,
// and forms &BG_PLTT[96] for each copy; here work takes r9, the blitter
// stays in r10 and the palette address is formed once. The 16.16 rotation
// pushes then differ in their temporaries.
// Tried: savedScrollX as u16 (194: loaded signed) or s32 (188, the
// native's ldrh into a word slot), canvas and work declared in either
// order, draw before or after palette, draw declared first (194).
#include "types.h"
#include "ram_buffer.h"
#include "io_reg.h"
#include "iwram_call.h"
#include "io_write_queue.h"
#include "input.h"
#include "bg_scroll.h"
#include "battle_display.h"
#include "battle_effect.h"
#include "battle_effect_work.h"

// The swirl runs until A is pressed, at most this many frames.
#define SWIRL_MAX_FRAMES 312000

extern char ResourceId_CloudSwirlSheet;
extern char ResourceId_EmberStreakSheet;
extern char ResourceId_BlueFlameSheet;
extern u8 gMapCellBuffer[];

void BattleEffect_BeginCanvasLayer(s32 layer);
void BattleEffect_EndCanvasLayer(void);
void BattleEffect_LoadBlitter(s32 slot, s32 value);
s32 Func_08196404(s32 slot, s32 width, s32 height, s32 flags, s32 mode);
void BattleEffect_ProcessPendingGraphicsTransfer(void);
void BattleEffect_FlushPendingGraphicsTransfer(void);
void Func_08164a4c(s32 blue, s32 green, s32 red);
void Func_0815f0a0(s32 resource);
void Resource_LoadAndDecompress(s32 id, void *target, s32 unpack, s32 wait);
void *Resource_GetTableEntry(s32 id);
s32 Resource_DecodeType01(const void *source, void *destination);
s32 Scheduler_AddOrUpdateCallback(void (*callback)(void), s32 order);
s32 Scheduler_RemoveCallback(void (*callback)(void));
void Runtime_ReleaseHeapBlock(s32 slot);
void WaitFrames(s32 frames);
s32 Trig_Sin(s32 angle);
s32 Trig_Cos(s32 angle);

// A turning cloud swirl on BG2, shown from Func_0813e7e0. The cloud sheet
// is drawn once into the canvas (its colours moved to palette entries 96
// on) and copied twice across the map buffer; then BG2 turns about the
// screen's centre while BG1 scrolls, the palette flashes white at the start,
// steps toward the swirl's own colours every fourth frame and from frame
// 120 dims. B starts it over and A ends it.
void Func_08191530(struct BattleEffectArgument *effect) {
  s32 savedScrollX;
  u8 *canvas;
  struct BattleEffectWork *work;
  struct BattlePresentationTransition *transition;
  DrawRectangle draw;
  const u8 *palette;
  s32 frame;
  s32 sine;
  s32 cosine;
  s32 i;

  savedScrollX = gBgScroll[1].x;
  canvas = Ram_HeapSlots->battleFxWork[1];
  work = (struct BattleEffectWork *)Ram_HeapSlots->battleFxWork[0];
  transition = Ram_HeapSlots->transitionWork;
  BattleEffect_BeginCanvasLayer(3);
  REG_DISPCNT = 0x7741;
  REG_BLDALPHA = 0x1010;
  REG_BLDCNT = 0x3f42;
  REG_BG2CNT = 0x787;
  REG_BG1CNT &= ~3;
  REG_BG1CNT |= 0;
  work->transferMode = 2;
  work->transferValue = 50;

  // The cloud sheet, recoloured to palette entries 96 on, drawn into the
  // canvas through the copy blitter.
  BattleEffect_LoadBlitter(104, 1 << 4 | 3);
  draw = (DrawRectangle)Ram_HeapSlots->unk64[1];
  palette = Resource_GetTableEntry((s32)&ResourceId_CloudSwirlSheet);
  Iwram_CopyWords((void *)&BG_PLTT[96], palette, 128);
  Resource_DecodeType01(palette + 128, work->sheet);
  palette = Resource_GetTableEntry((s32)&ResourceId_EmberStreakSheet);
  Iwram_CopyWords((void *)&BG_PLTT[96], palette, 128);
  for (i = 0; i != 0x4000; i++)
    work->sheet[i] += 96;
  Scheduler_AddOrUpdateCallback(BattleEffect_ProcessPendingGraphicsTransfer, 0xc80);
  draw(canvas, work->sheet, 0, 0, 128, 128);
  work->transferPending = 1;
  WaitFrames(1);
  Scheduler_RemoveCallback(BattleEffect_ProcessPendingGraphicsTransfer);
  Runtime_ReleaseHeapBlock(104);

  // The swirl itself in the canvas (colour 0 made 1, so none is clear),
  // copied twice side by side into the map buffer.
  Resource_LoadAndDecompress((s32)&ResourceId_CloudSwirlSheet, canvas, 1, 1);
  for (i = 0; i != 0x4000; i++) {
    if (canvas[i] == 0)
      canvas[i] = 1;
  }
  Func_08196404(104, 8, 7, 3, 3);
  draw = (DrawRectangle)Ram_HeapSlots->unk64[1];
  Scheduler_AddOrUpdateCallback(BattleEffect_FlushPendingGraphicsTransfer, 0xc80);
  transition->active = 1;
  draw(gMapCellBuffer, canvas, 0, 0, 128, 128);
  draw(gMapCellBuffer, canvas, 128, 0, 128, 128);
  work->transferPending = 1;
  WaitFrames(1);

  for (frame = 0; frame != SWIRL_MAX_FRAMES; frame++) {
    if (gInput.repeat & 2)
      frame = 0;
    if (gInput.repeat & 1)
      break;

    if (frame == 0)
      Iwram_FillWords((void *)BG_PLTT, 128, 0x7fff7fff);
    if (frame > 119)
      Func_08164a4c(-1, -1, -1);
    else if ((frame & 3) == 1)
      Func_0815f0a0((s32)&ResourceId_BlueFlameSheet);
    gBgScroll[1].x = frame / 2;

    // BG2 turns a 1024th of a circle a frame, at 1/532 of the sine's scale
    // (about half size), about the screen's centre (120, 72).
    sine = Trig_Sin(frame << 6) / 532;
    cosine = Trig_Cos(frame << 6) / 532;
    IO_WRITE_QUEUE_PUSH((u32)&REG_BG2PA, cosine, 0x20000);
    IO_WRITE_QUEUE_PUSH((u32)&REG_BG2PB, sine, 0x20000);
    IO_WRITE_QUEUE_PUSH((u32)&REG_BG2PC, -sine, 0x20000);
    IO_WRITE_QUEUE_PUSH((u32)&REG_BG2PD, cosine, 0x20000);
    IO_WRITE_QUEUE_PUSH((u32)&REG_BG2X,
                        (Iwram_MulQ16(-120 << 24, cosine) +
                         Iwram_MulQ16(-72 << 24, sine) + 0x400000) >>
                            8,
                        0x30000);
    IO_WRITE_QUEUE_PUSH((u32)&REG_BG2Y,
                        (Iwram_MulQ16(-120 << 24, -sine) +
                         Iwram_MulQ16(-72 << 24, cosine) + 0x400000) >>
                            8,
                        0x30000);
    WaitFrames(1);
  }

  gBgScroll[1].x = savedScrollX;
  transition->active = 0;
  Scheduler_RemoveCallback(BattleEffect_FlushPendingGraphicsTransfer);
  Runtime_ReleaseHeapBlock(104);
  BattleEffect_EndCanvasLayer();
}
