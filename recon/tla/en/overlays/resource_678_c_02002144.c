// Draft, differ 4 of 159 (tla-en, 2026-11-05). Older notes below may predate this figure.
// Measured under -Os, 5 November 2026: differ 4 (tla-en); notes below may predate it.
// Draft, differ 104 of 159, skeleton gap 12 (tla-en, 2026-11-01): a charge-up
// step for the controlled character in Taopo Swamp: holding the direction pad
// raises gSceneState's timer (faster with B held), shown in the sprite's level;
// at 0x1400 it releases two objects of type 269 (Func_02002808) 15 frames apart,
// waits, puts the character back at the saved place and clears the state.
// The scene's own SceneState and SpriteWork (games/tla/src/scene/taopo_swamp/
// local.h) took this from 138 to 109; held as a volatile cell closed the reread.
// Left: allocation only, from which the skeleton gap's reload moves also follow.
// The native's order is sprite r7, actor r8, zero r10, state r9, i r11. Here
// zero takes r7, actor r8, sprite r10, i r9, state r11. From the lreg dump,
// floor_log2(refs) * refs / live: zero 6 over 22 (0.55), actor 11 over 106
// (0.31), sprite 10 over 100 (0.30), i 7 over 74 (0.19), state 9 over 226 (0.12).
// Counting the loop body's refs once instead of twice gives zero 3, actor 8, i 4:
// sprite 0.30, actor 0.23, zero 0.14, state 0.12, i 0.11, exactly the native's
// order, and 4 * 1 < 3 fails, which also keeps zero out of r4 (the question
// resource_678_c_0200013c.c leaves open). flow.c counts one per ref only outside
// natural loops or under optimize_size; the loop here is natural. Inference from
// the arithmetic, not tested: a compiler question for Pascal (K3).
// Tried: for counting up (reversed to the same code), a goto loop (135), a
// charge local (no change), sprite declared first (no change), zero as a local
// set after the call, before it, or in the sprite branch (no change), set before
// the loop or at its top (131, actor r7, sprite r8), the two byte stores swapped.
#include "types.h"
#include "call.h"
#include "actor.h"
#include "field_sprite.h"

struct SpriteWork {
  u8 unk00[17];
  u8 layer : 2;
  u8 level : 6;
  u8 unk12[7];
  u8 dirty;
  u8 mode;
  u8 unk1b[17];
  u8 *child;
};

struct SceneState {
  s16 mode;
  s16 timer;
  s32 x;
  s32 z;
};

// volatile: the VBlank handler sets the held keys, as gKeysHeld in The Broken Seal.
extern volatile u32 gInput;
extern struct SceneState gSceneState;
extern u8 gData_02002ae8[];
s32 Func_02002998(void);
struct Actor *Actor_GetById(s32 id);
void Func_02002898(void);
void Func_02002968(s32 mode);
void Func_020028a0(void);
struct Actor *Func_02002808(s32 type, s32 x, s32 y, s32 z);
void Func_02002800(struct Actor *object, const void *script);
void Animation_ApplyChildArgument(void *sprite, s32 argument);
void wait(s32 frames);
void FieldEvent_Wait(s32 frames);

void Func_02002144(void) {
  struct SceneState *state = &gSceneState;
  struct Actor *actor = Actor_GetById(Func_02002998());
  struct SpriteWork *sprite = actor->animation;
  struct Actor *spark;
  struct FieldSprite *sparkSprite;
  s32 charge, step, i;

  if (sprite->child != 0)
    sprite->child[6] = 1;
  sprite->mode = (sprite->mode | 8) & 0xfe;
  /* FAKEMATCH(one-pass): the once-only block orders this statement; as a plain statement, 8 of 159 instructions
   * differ in tla-en. */
  do {
    sprite->dirty = 1;
  } while (0);
  if (gInput & 0xf0) {
    step = 0x1400;
    if (gInput & 2)
      step = step * 3 >> 1;
    state->timer += step / 96 + 1;
  }
  sprite->level = state->timer / 256;
  state->mode = 1;
  if (state->timer < 0x1400)
    return;
  Func_02002898();
  Func_02002968(0);
  actor->animationKind = 0;
  for (i = 1; i >= 0; i--) {
    spark = Func_02002808(269, actor->x, actor->y, actor->z);
    if (spark != 0) {
      sparkSprite = spark->animation;
      Func_02002800(spark, gData_02002ae8);
      spark->motionFlags = 0;
      spark->terrainId = 1;
      if (sparkSprite != 0) {
        Animation_ApplyChildArgument(sparkSprite, 2);
        sparkSprite->nextFrame = 0;
        sparkSprite->priority = 2;
      }
    }
    wait(15);
  }
  FieldEvent_Wait(30);
  actor->animationKind = 1;
  sprite->level = 0;
  sprite->mode = 0;
  Func_020028a0();
  actor->x = state->x;
  actor->z = state->z;
  state->mode = 0;
  state->timer = 0;
}
