// Draft of Func_08127308, The Broken Seal's BattleFx_PlayUnitElementEffect
// (games/tbs/src/battle/battle.c): differ 36 of 280 (tla-en, 2026-10-15).
// The Lost Age reads the battle work once (slot 36) and keeps it on the
// stack, scales the palette with ColorBuffer_Scale, and in mode 1 sends the
// canvas origin from a local pair rather than the effect work. The twin's
// unread ten-word tail and its slot-12 pointer are not needed: the frame
// is the four positions and launches, the origin pair, and two spills.

// Left: order only, about a dozen places, each a pooled or stack load the
// native issues one slot earlier than sched2 does here (the session
// reload inside 0x670 and 0x56c, the brightness pointer's spill, the IO
// queue entry's first store before the count's, the queue and IME
// addresses before mode 1's loop). The residue the Djinn drafts record.

#include "types.h"
#include "effect_step.h"
#include "io_reg.h"
#include "io_write_queue.h"
#include "iowrite.h"
#include "iwram_call.h"
#include "ram_buffer.h"
#include "battle_display.h"

struct SparkWork {
  u8 unk0000[0x13bc];
  s32 frames;
  s32 ready;
  s32 originX;
  s32 originY;
  s32 originPending;
};

// The parts of the battle work this effect uses.
struct ElementSession {
  u8 unk000[0x56c];
  u16 palette[128];
  s32 brightness;
  u16 background;
};

void WaitFrames(s32 frames);
void Func_0812628c(s32 mode, s32 value, s32 fade);
void ColorBuffer_Scale(u16 *source, u16 *destination, s32 scale, s32 count);
s32 Func_08127068(s32 mode);
s32 Func_0811c2b4(s32 slot, s32 *screen);
void Func_081272b0(s32 mode);
void Func_08138020(struct BattlePresentationWork *launch);
void Func_08138030(struct BattlePresentationWork *launch);
void Func_08138038(s32 mode);
void Func_08138040(void);

// Plays a unit's element effect in one of four ways: 0, the star field
// around the unit while the backdrop darkens over 25 frames (45 frames);
// 1, the prepared canvas scene of the kind, centred on the unit (40
// frames); 2, the ice fleck burst; 3, the gathering burst.
void Func_08127308(s32 unit, s32 kind, s32 mode, s32 variant) {
  struct EffectPosition position;
  struct EffectPosition position2;
  struct BattlePresentationWork launch;
  struct BattlePresentationWork launch2;
  s32 origin[2];
  struct ElementSession *session =
      (struct ElementSession *)Ram_HeapSlots->battleWork;
  volatile u16 *ime;
  struct SparkWork *work;
  s32 frame;
  s32 *brightness;
  struct EffectPosition *pos;

  WaitFrames(1);
  Func_0812628c(1, session->background, 0);
  Iwram_ClearWords((void *)0x06004000, 0x4000);
  QueueIoWriteDelay2(0x04000000, 0x3741);
  QueueIoWriteDelay2(0x0400000c, 0x784);
  QueueIoWriteDelay2(0x04000050, 0x3f44);
  WaitFrames(1);
  Io_Write16(240, &REG_WIN0H);
  Io_Write16(0x1088, &REG_WIN0V);
  Io_Write16(63, &REG_WININ);
  Io_Write16(17, &REG_WINOUT);
  if (mode == 0) {
    QueueIoWriteDelay2(0x04000052, 0x100e);
    Func_08127068(kind);
    for (frame = 0, pos = &position; frame <= 44; frame++) {
      brightness = &session->brightness;
      ime = &REG_IME;
      work = (struct SparkWork *)Ram_HeapSlots->battleFxWork[0];
      if (frame <= 24)
        ColorBuffer_Scale(session->palette, (u16 *)0x050000c0,
                          *brightness = 0x10000 - frame * 1092, 128);
      Func_0811c2b4(unit, (s32 *)pos);
      work->originX = (64 - pos->x) << 8;
      work->originY = (64 - pos->y) << 8;
      IO_WRITE_QUEUE_PUSH_IME(ime, 0x04000028, (u32)&work->originX, 0x84000002);
      work->originPending = 1;
      WaitFrames(1);
    }
    Func_081272b0(kind);
  } else if (mode == 1) {
    Func_08138038(kind);
    for (frame = 0; frame < 40; frame++) {
      Func_0811c2b4(unit, (s32 *)&position2);
      origin[0] = (64 - position2.x) << 8;
      origin[1] = (64 - position2.y) << 8;
      IO_WRITE_QUEUE_PUSH(0x04000028, (u32)origin, 0x84000002);
      WaitFrames(1);
    }
    Func_08138040();
  } else if (mode == 2) {
    launch.flags = 0;
    launch.kind = kind;
    launch.variant = variant;
    launch.actor = unit;
    launch.actors[0] = unit;
    launch.firstTarget = unit;
    launch.count = 1;
    launch.initialValue = 1;
    Func_08138020(&launch);
  } else {
    launch2.kind = kind;
    launch2.flags = 0;
    launch2.variant = 0;
    launch2.actor = unit;
    launch2.actors[0] = unit;
    launch2.firstTarget = unit;
    launch2.count = 1;
    launch2.initialValue = 1;
    Func_08138030(&launch2);
  }
}
