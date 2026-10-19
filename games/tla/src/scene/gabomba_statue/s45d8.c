#include "types.h"

struct PuzzleCell {
  u8 kind;
  u8 blocked;
  u8 state;
  u8 occupied;
};

struct SceneState {
  struct PuzzleCell cells[88];
  u8 unk160[4];
  s16 firstId;
  s16 endId;
  u16 tileEntry;
  s8 redrawA;
  s8 redrawB;
  s16 originX;
  s16 originZ;
  s16 statuesReady;
  u8 unk172[2];
  s32 statueOffset;
};

extern struct SceneState gSceneState;

void Func_0200567c(s32 a, s32 b, s32 c, s32 d, s32 e, s32 f);
void Func_02005854(s32 sound);
void Func_020056b4(void);
void Func_020056bc(void);
void Func_0200580c(s32 value);
void Func_020044b4(s32 mode);
void Func_020044d0(s32 value);
void WaitFrames(s32 frames);

static __inline__ void Map_Copy(s32 a, s32 b, s32 c, s32 d, s32 e, s32 f) {
  /* FAKEMATCH: forwarding through the inline keeps each call's own constants,
   * as ☀️'s scene layer does. */
  Func_0200567c(a, b, c, d, e, f);
}

void Func_020045d8(void) {
  if (gSceneState.redrawA != 0) {
    Map_Copy(1, 12, 1, 1, gSceneState.originX + 3, gSceneState.originZ - 2);
    gSceneState.redrawA = 0;
  }
}

/// Side B's twin of Func_02004530: the offset moves up instead.
void Func_02004620(void) {
  struct SceneState *state = &gSceneState;
  /* FAKEMATCH(pin): the count in r6, as in Func_02004530; as a plain local,
   * 22 of 74 instructions differ in tla-en. */
  register s32 i asm("r6");

  if (state->redrawB == 0) {
    Map_Copy(0, 13, 1, 1, state->originX + 7, state->originZ - 2);
    if (state->statuesReady != 0) {
      Func_02005854(248);
      Func_020056b4();
      Func_0200580c(0);
      Func_020044b4(2);
      i = 19;
      do {
        /* FAKEMATCH(pin): the offset's address built every pass, as in
         * Func_02004530; as state->statueOffset with the barrier, 30 of 74
         * instructions differ in tla-en. */
        register s32 *offset asm("r2") = &state->statueOffset;
        s32 value = *offset;

        /* FAKEMATCH(barrier): clobbering r1 loads the offset before 0x333 is
         * built in r1; as *offset += 0x333, 2 of 74 instructions differ in
         * tla-en. */
        __asm__("" : "+l"(value) : : "r1");
        *offset = value + 0x333;
        WaitFrames(1);
        i--;
      } while (i >= 0);
      Func_020044d0(1);
      Func_020056bc();
      state->statueOffset = 0;
    }
    state->redrawB = 1;
  }
}
