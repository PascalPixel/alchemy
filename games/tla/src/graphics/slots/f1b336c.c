#include "types.h"
#include "io_reg.h"
#include "fixed_math.h"
#include "effect_step.h"
#include "ram_buffer.h"
#include "edition.h"

// The title's letters and the left edge of the first: the Spanish title has
// nine letters and starts further left.
#if (LANGUAGE == LANGUAGE_SPANISH)
#define REEL_TITLE_LETTERS 9
#define REEL_TITLE_LEFT 8
#else
#define REEL_TITLE_LETTERS 8
#define REEL_TITLE_LEFT 24
#endif

struct ReelWork {
  u8 unk000[0x8c];
  s32 state;
  s32 cursor;
  s32 spins;
  u8 unk098[0x434];
  s32 subWindow;
  u8 unk4d0[8];
  u16 scanlineOffsets[160];
};

struct ReelEffectWork {
  struct EffectStep particles[REEL_TITLE_LETTERS];
  u8 unkAfterParticles[0x7780 - REEL_TITLE_LETTERS * sizeof(struct EffectStep)];
  s32 transferMode;
  s32 transferValue;
};

extern char MsgSlotsBet[];
extern const u8 gReelGame_TitleLetterWidths[];

s32 Window_CreateFar(s32 x, s32 y, s32 width, s32 height, s32 flags);
void Text_DrawCharacterAtOffsetFar(s32 message, s32 window, s32 x, s32 y);

#define SPARKS ((struct EffectStep *)Ram_MapCellBuffer)

/// The Broken Seal's ReelGame_InitializeTitle on this game's heap slots (the
/// reel work at 0xb4, the effect work at 0x5c with its particles first).
void Func_081b336c(void) {
  struct ReelWork *state;
  struct ReelEffectWork *work;
  struct EffectStep *letter;
  const u8 *width;
  s32 left;
  s32 step;
  s32 y;
  s32 i;
  s32 angle;
  s32 window;
  s32 message;

  state = (struct ReelWork *)Ram_HeapSlots->canvasMap;
  work = (struct ReelEffectWork *)Ram_HeapSlots->battleFxWork[0];
  left = 0;
  for (i = 0; i != 0x800; i++)
    SPARKS[i].variant = 0;

  letter = work->particles;
  y = -0x200000;
  width = gReelGame_TitleLetterWidths;
  for (i = 0; i != REEL_TITLE_LETTERS; i++) {
    letter[i].x = (left + REEL_TITLE_LEFT) << 16;
    step = *width;
    width++;
    left += step;
    letter[i].y = y;
    letter[i].velocityY = 0;
    letter[i].variant = 0;
    y += -0x80000;
  }

  for (i = 0; i != 160; i++)
    state->scanlineOffsets[i] = 0;

  for (i = 0; i != 40; i++) {
    angle = i * 0x199;
    state->scanlineOffsets[23 + i] = (u32)(Trig_Cos(angle) * 3) >> 15;
    state->scanlineOffsets[110 - i] = (u32)(Trig_Cos(angle) * 3) >> 15;
  }

  state->spins = 0;
  state->state = 0;
  state->cursor = 0;
  work->transferMode = 1;
  work->transferValue = 0;
  REG_BLDCNT = 0;
  window = Window_CreateFar(18, 0, 12, 4, 6);
  state->subWindow = window;
  message = (s32)MsgSlotsBet;
  Text_DrawCharacterAtOffsetFar(message, window, 0, 8);
  Text_DrawCharacterAtOffsetFar(message - 1, state->subWindow, 0, 0);
}
