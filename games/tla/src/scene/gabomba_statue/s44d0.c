#include "types.h"
#include "object_runtime.h"

struct SceneState {
  u8 unk000[0x164];
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

struct ObjectRuntime *Object_GetById(s32 id);
void Func_02004380(s32 x, s32 z, s32 a);
void Func_0200567c(s32 a, s32 b, s32 c, s32 d, s32 e, s32 f);
void Func_02005854(s32 sound);
void Func_020056b4(void);
void Func_020056bc(void);
void Func_0200580c(s32 value);
void Func_020044b4(s32 mode);
void WaitFrames(s32 frames);

static __inline__ void Map_Copy(s32 a, s32 b, s32 c, s32 d, s32 e, s32 f) {
  /* FAKEMATCH: forwarding through the inline keeps each call's own constants,
   * as ☀️'s scene layer does. */
  Func_0200567c(a, b, c, d, e, f);
}

void Func_020044d0(s32 value) {
  struct SceneState *state = &gSceneState;
  s32 id;

  for (id = state->firstId; id < state->endId; id++) {
    struct ObjectRuntime *object = Object_GetById(id);

    Func_02004380((object->x >> 20) - state->originX,
                  (object->z >> 20) - state->originZ, value);
  }
}

/// Redraws side A once; with the statues ready, moves their offset down by
/// 0x333 a frame for twenty frames, then clears it.
void Func_02004530(void) {
  struct SceneState *state = &gSceneState;
  /* FAKEMATCH(pin): the count in r6, which leaves r5 to the state; as a plain
   * local, 22 of 72 instructions differ in tla-en. */
  register s32 i asm("r6");

  if (state->redrawA == 0) {
    Map_Copy(1, 13, 1, 1, state->originX + 3, state->originZ - 2);
    if (state->statuesReady != 0) {
      Func_02005854(248);
      Func_020056b4();
      Func_0200580c(0);
      Func_020044b4(3);
      i = 19;
      do {
        /* FAKEMATCH(pin): a hard register is not moved out of the loop, so the
         * offset's address is built every pass, as the native does; as
         * state->statueOffset, 30 of 72 instructions differ in tla-en. */
        register s32 *offset asm("r2") = &state->statueOffset;

        *offset -= 0x333;
        WaitFrames(1);
        i--;
      } while (i >= 0);
      Func_020044d0(0);
      Func_020056bc();
      state->statueOffset = 0;
    }
    state->redrawA = 1;
  }
}
