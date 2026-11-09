#include "types.h"
#include "actor.h"
#include "field_sprite.h"
#include "event_inline.h"

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
void Func_02002898(void);
void Func_020028a0(void);
void Func_02002800(struct Actor *object, const void *script);
void Animation_ApplyChildArgument(void *sprite, s32 argument);

void Func_02002144(void) {
  struct SceneState *state = &gSceneState;
  struct Actor *actor = Actor_GetById(Party_GetLeaderActor());
  struct SpriteWork *sprite = actor->animation;
  struct Actor *spark;
  struct FieldSprite *sparkSprite;
  s32 charge, step, i;

  if (sprite->child != 0)
    sprite->child[6] = 1;
  sprite->mode |= 8;
  sprite->mode &= 0xfe;
  /* FAKEMATCH(one-pass): the once-only block orders this statement; as a plain statement, 4 of 159 instructions
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
  Event_ResolvePendingActions(0);
  actor->animationKind = 0;
  for (i = 1; i >= 0; i--) {
    spark = ((struct Actor *)Import_ActorCreate(269, actor->x, actor->y, actor->z));
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
  Event_Wait(30);
  actor->animationKind = 1;
  sprite->level = 0;
  sprite->mode = 0;
  Func_020028a0();
  actor->x = state->x;
  actor->z = state->z;
  state->mode = 0;
  state->timer = 0;
}
