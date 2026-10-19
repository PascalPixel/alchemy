#include "types.h"
#include "io_reg.h"
#include "input.h"

// The frame state's byte 2, set while the audio test runs.
struct SoundFrameState {
  u8 unk00[2];
  u8 unk02;
  u8 unk03;
};

extern volatile struct SoundFrameState gData_03001138;
extern s32 Data_03000ee4;
extern s32 gIwramHi_Data1;

void Sound_LoadPresetParameters(s32 preset);
void AudioCommand_Play(s32 id);
void WaitFrames(s32 frames);

struct AudioTestCues {
  s32 cue[3];
};

// The three cues the test starts with: the object's .rodata, which the
// object list places after Audio_EmptyCallback, as ☀️'s sound.c has its
// .rodata in the sound tail.
static const struct AudioTestCues sAudioTestCues = {{0, 80, 100}};

// Runs the audio test's three cue selectors and five mixer presets forever,
// as ☀️'s AudioTest_RunParameterController (games/tbs/src/sound/sound.c),
// here from preset 3, with the frame state's byte 2 set, a clip timer of 2
// and the commands 0, 78 and 390.
void Func_081c1014(void) {
  /* FAKEMATCH(temporary): the cues' address loaded before sp is copied for
   * the array, through an empty asm on it (barrier); as s32 cues[3] = {0,
   * 80, 100}, 2 of 145 instructions differ in tla-en. */
  const struct AudioTestCues *initial = &sAudioTestCues;
  struct AudioTestCues cues;
  s32 preset;
  s32 row;
  s32 timer;

  __asm__ __volatile__("" : "+r"(initial));
  cues = *initial;
  row = 0;
  preset = 3;
  /* FAKEMATCH(one-pass): keeps the store's address load after the preset;
   * as a plain store, 10 of 145 instructions differ in tla-en. */
  do {
    gIwramHi_Data1 = 0;
  } while (0);
  gData_03001138.unk02 = 1;
  timer = 20;

  for (;;) {
    if (timer != 0)
      timer--;

    if (Data_03000ee4 != 0) {
      /* FAKEMATCH(one-pass): keeps the flag's clear before the timer's
       * store; as a plain store, 2 of 145 instructions differ in tla-en. */
      do {
        Data_03000ee4 = 0;
      } while (0);
      timer = 2;
    }

    if (gInput.repeat & KEY_SELECT) {
      preset = (preset + 1) % 5;
      Sound_LoadPresetParameters(preset);
    }
    if (gInput.repeat & KEY_R)
      cues.cue[row] += 10;
    if (gInput.repeat & KEY_L)
      cues.cue[row] -= 10;
    if (gInput.repeat & KEY_RIGHT)
      cues.cue[row] += 1;
    if (gInput.repeat & KEY_LEFT)
      cues.cue[row] -= 1;
    if (gInput.repeat & KEY_UP) {
      if (row > 0)
        row--;
    }
    if (gInput.repeat & KEY_DOWN) {
      if (row <= 1)
        row++;
    }
    if (gInput.repeat & KEY_A)
      AudioCommand_Play(cues.cue[row]);
    if (gInput.repeat & KEY_B)
      AudioCommand_Play(0);
    if (gInput.repeat & KEY_START)
      AudioCommand_Play(78);
    if (gInput.repeat & KEY_SELECT)
      AudioCommand_Play(390);

    WaitFrames(1);
  }
}
