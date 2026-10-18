// Part of runtime.c, the runtime object, compiled only through it.
// Screen fades through the blend registers.

#include "../runtime_internal.h"

enum {
  FADE_GAME_LAYERS = 0x3e,
  FADE_LAYER_MASK = 0x3f,
  FADE_BRIGHTEN = 0x80,
  FADE_DARKEN = 0xc0,
  FADE_LEVEL_MAX = 16
};

// Steps the screen fade once a frame from VBlank: sets BLDCNT to brighten or
// darken the chosen layers and moves BLDY one step from the start level to
// the target level.
void Fade_Update(void) {
  if (gBlendDuration != 0) {
    {
      volatile u16 *blendControl;
      u32 control;

      if (gBlendBrighten != 0) {
        control = gBlendLayers | FADE_BRIGHTEN;
        blendControl = &REG_BLDCNT;
      } else {
        control = gBlendLayers | FADE_DARKEN;
        blendControl = &REG_BLDCNT;
      }
      *blendControl = control;
    }
    {
      u8 *remaining = &gBlendFramesLeft;
      s32 delta;
      s32 level;
      s32 step;

      (*remaining)--;
      level = gBlendTargetLevel;
      delta = gBlendStartLevel - gBlendTargetLevel;
      step = *remaining;
      /* FAKEMATCH: an explicit libgcc call, which clobbers memory, so the zero
       * test reloads *remaining after it as the native does. With the /
       * operator the division is a libcall block CSE sees through: the test
       * reuses the step's load and the registers shift (differ 37 in all six
       * editions). The operator matches only if every read of *remaining is
       * forced to memory by its qualifier, which trades this call for that
       * device. */
      level += __divsi3(delta * step, gBlendDuration);
      REG_BLDY = level;
      if (*remaining == 0)
        gBlendDuration = 0;
    }
  }
}

// Screen fades. Fade_Update moves the fade one step each VBlank.
// The brightness (BLDY) runs from 0, the picture as drawn, to 16, all white
// or all black.
//   gBlendBrighten   : 0 fades toward black, 1 toward white
//   gBlendLayers     : the BLDCNT layers the fade covers
//   gBlendFramesLeft : frames until the fade ends

// Fades BG1 to BG3, the sprites and the backdrop (0x3e) from the current
// target brightness to `level` over `frames` frames.
#define DEFINE_BLEND_FADE(name, brighten, level)                               \
  void name(s32 frames) {                                                      \
    gBlendBrighten = brighten;                                                 \
    gBlendLayers = FADE_GAME_LAYERS;                                           \
    gBlendStartLevel = gBlendTargetLevel;                                      \
    gBlendTargetLevel = level;                                                 \
    gBlendDuration = frames;                                                   \
    gBlendFramesLeft = gBlendDuration;                                         \
  }

// Fades out to black.
DEFINE_BLEND_FADE(Fade_ToBlack, 0, FADE_LEVEL_MAX)

// Fades in from black.
DEFINE_BLEND_FADE(Fade_FromBlack, 0, 0)

// Fades out to white.
DEFINE_BLEND_FADE(Fade_ToWhite, 1, FADE_LEVEL_MAX)

// Fades in from white.
DEFINE_BLEND_FADE(Fade_FromWhite, 1, 0)

// Starts a fade of any layers.
//   bright : 0 toward black, 1 toward white
//   layer  : the BLDCNT layers to fade (low six bits)
//   start  : the starting brightness; above 16, the current target
//   target : the brightness to end at
//   frames : how long the fade takes
void Fade_Start(s8 bright, s32 layer, u32 start, s32 target, s32 frames) {
  gBlendBrighten = bright;
  gBlendLayers = layer & FADE_LAYER_MASK;
  if (start > FADE_LEVEL_MAX) {
    gBlendStartLevel = gBlendTargetLevel;
  } else {
    gBlendStartLevel = start;
  }
  gBlendTargetLevel = target;
  gBlendFramesLeft = (gBlendDuration = frames);
}

// Waits until the running fade has finished.
void Fade_Wait(void) {
  if (gBlendDuration != 0) {
    do {
      WaitFrames(1);
    } while (gBlendDuration != 0);
  }
}
