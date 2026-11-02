// Matches: differ 0 (tla-en, 2026-11-01)
// The Broken Seal twin is LineScroll_BuildPage, for the heap slot 0x88
// workspace: fills the hidden page with each scanline's BG1, BG2 and BG3
// offsets, the scroll plus a sine wave on each axis with a step (in mode 1
// only BG3 waves), then shows that page. Nothing while the event work's byte
// 0xca4 is set. No device. What closed it after the compiler change: the
// scroll locals declared x1, y1, x2, y2, x3, y3; one u16 waveX3 and one u16
// waveY3 for the whole function, each set in both modes, so gcse shares each
// axis's shifted value (a single u16 for both axes, or one per section, lets
// the pseudo outbid the work pointer or lose r10); and the first section's
// locals set in the order a, waveX3, b, which gives b its slot at sp+8.
#include "types.h"
#include "bg_scroll.h"
#include "iwram_call.h"
#include "ram_buffer.h"

struct DisplayScrollWork {
  struct BgScroll rows[2][160][3];
  u8 page;
  u8 mode;
  u16 frame;
  u8 unkf04[4];
  s32 freqX;
  s32 freqY;
  s32 stepX;
  s32 stepY;
  s32 ampX;
  s32 ampY;
};

extern const s16 Data_080f08b0[];

void Func_080d607c(void) {
  struct DisplayScrollWork *work = Ram_HeapSlots->unk84[1];
  u8 mode = work->mode;
  s32 x1 = (s16)gBgScroll[1].x;
  s32 y1 = (s16)gBgScroll[1].y;
  s32 x2 = (s16)gBgScroll[2].x;
  s32 y2 = (s16)gBgScroll[2].y;
  s32 x3 = (s16)gBgScroll[3].x;
  s32 y3 = (s16)gBgScroll[3].y;
  u8 *event = Ram_HeapSlots->eventWork;
  u16 *row;
  s32 step;
  s32 phase;
  s32 amplitude;
  s32 scanline;
  u16 wave;
  u16 waveX3;
  u16 waveY3;

  if (event != NULL && *(s8 *)(event + 0xca4) != 0)
    return;
  if (mode == 0) {
    row = &work->rows[work->page ^ 1][0][0].x;
    step = work->stepX;
    phase = (work->frame + (u16)y1) * work->freqX;
    if (step == 0) {
      for (scanline = 0; scanline != 160; scanline++) {
        row[scanline * 6] = x1;
        row[scanline * 6 + 2] = x2;
        row[scanline * 6 + 4] = x3;
      }
    } else {
      u16 a = x1;
      u16 b = x2;

      waveX3 = x3;
      amplitude = work->ampX;
      for (scanline = 0; scanline != 160; scanline++) {
        wave = Iwram_MulQ16(Data_080f08b0[(phase >> 16) & 255], amplitude) /
               256;
        row[0] = a + wave;
        row[2] = b + wave;
        row[4] = waveX3 + wave;
        row += 6;
        phase += step;
      }
    }
    row = &work->rows[work->page ^ 1][0][0].y;
    step = work->stepY;
    phase = (work->frame + (u16)y1) * work->freqY;
    if (step == 0) {
      for (scanline = 0; scanline != 160; scanline++) {
        row[scanline * 6] = y1;
        row[scanline * 6 + 2] = y2;
        row[scanline * 6 + 4] = y3;
      }
    } else {
      u16 a = y1;
      u16 b = y2;
      waveY3 = y3;
      amplitude = work->ampY;
      for (scanline = 0; scanline != 160; scanline++) {
        wave = Iwram_MulQ16(Data_080f08b0[(phase >> 16) & 255], amplitude) /
               256;
        *row = a + wave;
        row += 2;
        *row = b + wave;
        row += 2;
        *row = waveY3 + wave;
        row += 2;
        phase += step;
      }
    }
  } else {
    row = &work->rows[work->page ^ 1][0][0].x;
    step = work->stepX;
    phase = (work->frame + (u16)y1) * work->freqX;
    if (step == 0) {
      for (scanline = 0; scanline != 160; scanline++) {
        row[scanline * 6] = x1;
        row[scanline * 6 + 2] = x2;
        row[scanline * 6 + 4] = x3;
      }
    } else {
      waveX3 = x3;
      amplitude = work->ampX;
      for (scanline = 0; scanline != 160; scanline++) {
        wave = Iwram_MulQ16(Data_080f08b0[(phase >> 16) & 255], amplitude) /
               256;
        row[0] = x1;
        row[2] = x2;
        row[4] = waveX3 + wave;
        row += 6;
        phase += step;
      }
    }
    row = &work->rows[work->page ^ 1][0][0].y;
    step = work->stepY;
    phase = (work->frame + (u16)y1) * work->freqY;
    if (step == 0) {
      for (scanline = 0; scanline != 160; scanline++) {
        row[scanline * 6] = y1;
        row[scanline * 6 + 2] = y2;
        row[scanline * 6 + 4] = y3;
      }
    } else {
      waveY3 = y3;
      amplitude = work->ampY;
      for (scanline = 0; scanline != 160; scanline++) {
        wave = Iwram_MulQ16(Data_080f08b0[(phase >> 16) & 255], amplitude) /
               256;
        row[0] = y1;
        row[2] = y2;
        row[4] = waveY3 + wave;
        row += 6;
        phase += step;
      }
    }
  }
  work->frame++;
  work->page ^= 1;
}
