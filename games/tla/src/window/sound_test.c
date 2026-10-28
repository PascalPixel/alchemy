#include "types.h"
#include "ram_buffer.h"
#include "input.h"
#include "render_input.h"

struct SoundTestValues {
  s32 values[4];
};

extern const struct SoundTestValues Data_0805ea98;
extern const u8 SoundTest_MusicString[], SoundTest_JingleString[],
    SoundTest_SeString[], SoundTest_FxString[];

void Func_080409a4(void);
void Func_080409dc(void);
struct RenderInput *Func_080409f0(void);
void Func_08108040(void *cursor, s32 x, s32 y, s32 mode);
void wait(s32 frames);
void Sound_PlayFar(s32 cue);
s32 Sound_LoadPresetParameters(s32 preset);
void Window_Close(struct RenderInput *window, s32 release);
void WindowSprite_RedrawSavedRect(struct RenderInput *window);
void Window_DrawDividerLine(struct RenderInput *window, s32 x1, s32 y1,
                              s32 x2, s32 y2);
void Text_DrawStringInWindow(const u8 *text, struct RenderInput *window,
                               s32 x, s32 y);
void Text_DrawNumberInWindow(s32 value, s32 digits,
                               struct RenderInput *window, s32 x, s32 y);

/// The debug sound test. Four values (music, jingle, sound effect, sound
/// preset) start from Data_0805ea98; up and down pick one, left and right
/// step it by 1, L and R by 10; A plays it (the fourth loads its preset
/// parameters), START plays sound 390 and stops, and B leaves.
s32 Func_08040a48(void) {
  struct SoundTestValues test;
  s32 redraw = 1;
  s32 cursor = 0;
  u8 *work;
  struct RenderInput *window;
  s32 rows = 4;
  s32 i;
  s32 y;
  s32 *value;

  test = Data_0805ea98;
  Func_080409a4();
  work = Ram_HeapSlots->unkd0[0];
  window = Func_080409f0();
  wait(1);
  do {
    if (redraw != 0) {
      redraw = 0;
      cursor = (cursor + rows) % rows;
      test.values[3] = (test.values[3] + 5) % 5;
      WindowSprite_RedrawSavedRect(window);
      for (i = 0, y = 2; i < 3; i++, y += 2)
        Window_DrawDividerLine(window, 0, y, 14, y);
      for (i = 0, value = test.values; i < 4; i++)
        Text_DrawNumberInWindow(*value++, 0, window, 72, i * 16);
      Text_DrawStringInWindow(SoundTest_MusicString, window, 8, 0);
      Text_DrawStringInWindow(SoundTest_JingleString, window, 8, 16);
      Text_DrawStringInWindow(SoundTest_SeString, window, 8, 32);
      Text_DrawStringInWindow(SoundTest_FxString, window, 8, 48);
      Func_08108040(work + 0x5a4, ((s16)window->x << 3) - 4,
                    ((s16)window->y << 3) + (cursor << 4) + 12, 3);
    }
    wait(1);
    if (gInput.pressed & 2)
      break;
    if (gInput.pressed & 1) {
      if (cursor == 3)
        Sound_LoadPresetParameters(test.values[3]);
      else
        Sound_PlayFar(test.values[cursor]);
    }
    if (gInput.pressed & 8) {
      Sound_PlayFar(390);
      Sound_PlayFar(0);
    }
    if (gInput.repeat & 0x100) {
      test.values[cursor] += 10;
      redraw = 1;
    }
    if (gInput.repeat & 0x200) {
      test.values[cursor] -= 10;
      redraw = 1;
    }
    if (gInput.repeat & 0x10) {
      test.values[cursor]++;
      redraw = 1;
    }
    if (gInput.repeat & 0x20) {
      test.values[cursor]--;
      redraw = 1;
    }
    if (gInput.repeat & 0x40) {
      cursor--;
      redraw = 1;
    }
    if (gInput.repeat & 0x80) {
      cursor++;
      redraw = 1;
    }
  } while (1);
  Window_Close(window, 2);
  Func_080409dc();
  wait(1);
  return 0;
}
