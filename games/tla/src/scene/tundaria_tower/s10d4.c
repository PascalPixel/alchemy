#include "types.h"
#include "io_reg.h"
#include "object_runtime.h"
#include "party_state.h"

struct ReflectionSprite {
  u8 unk00[2];
  u16 unk02;
  u8 unk04[12];
  s32 unk10;
  u8 unk14;
};

struct ReflectionAnimation {
  u8 unk00[0x28];
  struct ReflectionSprite *sprite;
};

struct ObjectRuntime *Object_GetById(s32 id);
s32 GameFlag_Test(s32 flag);

void Func_020010d4(struct ObjectRuntime *object) {
  struct ObjectRuntime *leader = Object_GetById(gPartyState.currentOwner);
  struct ReflectionAnimation *leaderAnimation = leader->animation;
  struct ReflectionAnimation *animation = object->animation;

  if (leader->z <= object->unk4c) {
    object->z = 0;
    object->x = 0;
    REG_BG2CNT &= 0xfffc;
    REG_BG2CNT |= 1;
  } else {
    REG_BG2CNT &= 0xfffc;
    REG_BG2CNT |= 2;
    if (GameFlag_Test(0x212)) {
      object->z = 0;
      object->x = 0;
    } else {
      struct ReflectionSprite *sprite = animation->sprite;
      struct ReflectionSprite *source;

      object->x = leader->x;
      object->y = leader->y;
      object->z = object->unk4c - (leader->z - object->unk4c);
      object->angle = ~leader->angle;
      source = leaderAnimation->sprite;
      sprite->unk10 = source->unk10;
      sprite->unk02 = source->unk02;
      sprite->unk14 = source->unk14;
    }
  }
}
