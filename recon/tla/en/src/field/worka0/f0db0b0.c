#include "types.h"
#include "event_work.h"
#include "fixed_math.h"
#include "frame.h"
#include "input.h"
#include "io_reg.h"
#include "iwram_call.h"
#include "actor.h"
#include "worka0.h"

struct Actor *Actor_GetById(s32 id);
struct WorkA4Slot *Func_080dae70(s32 id);
void Actor_SetSingleChildFlagsFar(struct Actor *actor, u32 flags);
void wait(s32 frames);

struct ChainLink {
  struct ChainLink *next;
  s32 x;
  s32 y;
  s32 z;
};

struct Unk_080db0b0Sprite {
  u8 unk00[18];
  u16 unk12;
};

// The half turn on the leader's side of the anchor: 0 on its right, 1 on its
// left.
#define SIDE_TURN(side) ((side) * (TRIG_TURN / 2))

void Func_080db0b0(s32 id) {
  s32 rootX, rootY;
  s32 motionFlags;
  struct Unk_080db0b0Sprite *heldSprite;
  s32 side = 0;
  struct Actor *leader = Actor_GetById(Party_GetLeaderActor());
  struct Actor *anchor = Actor_GetById(id);
  /* FAKEMATCH(signature): the slot read through a one-member union, whose
   * member accesses alias every store, as the original's do; as struct
   * WorkA4Slot *, 12 of 378 instructions differ in tla-en. */
  union {
    struct WorkA4Slot s;
  } *slot;
  struct Unk_080db0b0Sprite *sprite;
  s32 swing;
  s32 start;
  s32 dir;
  u16 letGo;
  s32 radius;
  s32 steps;
  s32 angle;
  s32 sine;
  s32 i;

  if (leader == NULL || anchor == NULL)
    return;
  motionFlags = leader->motionFlags;
  heldSprite = leader->animation;
  if (leader->x < anchor->x)
    side = 1;
  slot = (void *)Func_080dae70(id);
  if (slot == NULL)
    return;

  start = slot->s.swing;
  if ((slot->s.phase ^ SIDE_TURN(side)) & (TRIG_TURN / 2)) {
    for (swing = start; swing >= 0; swing -= 64) {
      slot->s.swing = swing;
      wait(1);
    }
    start = 0;
  }
  for (swing = start; swing <= 1536; swing += 64) {
    slot->s.swing = swing;
    slot->s.phase = SIDE_TURN(side) + TRIG_TURN / 4;
    wait(1);
  }
  while ((slot->s.phase & 0xff00) != SIDE_TURN(side ^ 1) + TRIG_TURN / 4) {
    slot->s.swing = 1536;
    wait(1);
  }

  sprite = leader->animation;
  dir = 1;
  if (side) {
    letGo = KEY_DOWN | KEY_LEFT | KEY_B;
    dir -= 2;
  } else
    letGo = KEY_DOWN | KEY_RIGHT | KEY_B;
  slot->s.active = 1;
  Actor_SetMode(leader, 32);
  while (1) {
    slot->s.phase = SIDE_TURN(side ^ 1) + TRIG_TURN / 4;
    switch (gFrameCount & 63) {
    case 0:
    case 16:
      slot->s.swing = 1568;
      slot->s.spacing = Q16_ONE * 29 / 2;
      sprite->unk12 = dir << 10;
      break;
    case 1:
    case 2:
    case 17:
    case 18:
      slot->s.swing += 16;
      slot->s.spacing += Q16_ONE / 4;
      sprite->unk12 += dir << 9;
      break;
    default:
      if (slot->s.swing > 1536)
        slot->s.swing -= 32;
      else
        slot->s.swing = 1536;
      if (slot->s.spacing > Q16_ONE * 14)
        slot->s.spacing -= Q16_ONE / 32;
      if (sprite->unk12 != 0)
        sprite->unk12 -= dir << 7;
      break;
    }
    if (gInput.pressed & letGo) {
      sprite->unk12 = 0;
      slot->s.active = 1;
      return;
    }
    if (gInput.pressed != 0)
      break;
    wait(1);
  }

  sprite->unk12 = 0;
  slot->s.active = 0;
  Actor_SetMode(leader, 6);
  wait(6);
  Sound_PlayFar(152);
  Actor_SetMode(leader, 7);
  leader->speedLimit = Q16_ONE * 3;
  leader->acceleration = Q16_ONE * 2;
  leader->velocityY = Q16_ONE * 4;
  leader->motionFlags &= 0x7e;
  Actor_SetSingleChildFlagsFar(leader, 0);
  wait(5);
  Actor_SetMode(leader, 16);
  leader->motionFlags = 0;
  rootX = slot->s.root->next->x;
  rootY = slot->s.root->next->y;
  radius = Q16_ONE * 65;
  steps = 8;
  while ((slot->s.phase & 0xff00) != SIDE_TURN(side) + TRIG_TURN * 7 / 32) {
    if (steps != 0) {
      radius += Q16_ONE * 5 / 16;
      slot->s.spacing += Q16_ONE / 16;
      steps--;
    }
    slot->s.swing = 2048;
    slot->s.active = 1;
    // A once-only block: the sine is taken before Iwram_MulQ16Entry is loaded.
    do {
      sine = Trig_Sin(slot->s.phase - 4000);
    } while (0);
    angle = Iwram_MulQ16(sine, slot->s.swing + slot->s.swing * 5 / 2);
    leader->x = rootX - Iwram_MulQ16(Trig_Sin(angle), radius);
    leader->y = rootY - Iwram_MulQ16(Trig_Cos(angle), radius);
    leader->targetX = leader->x;
    leader->targetY = leader->y;
    heldSprite->unk12 = angle;
    wait(1);
  }
  slot->s.active = 1;
  Actor_SetMode(leader, 7);
  leader->motionFlags = motionFlags & ~1;
  leader->velocityY = Q16_ONE * 4;
  for (i = 7; i >= 0; i--) {
    slot->s.spacing -= Q16_ONE / 16;
    wait(1);
  }
  slot->s.spacing = Q16_ONE * 14;
  heldSprite->unk12 = 0;
  Actor_SetMode(leader, 6);
  Actor_SetSingleChildFlagsFar(leader, 1);
  leader->motionFlags = motionFlags;
  wait(6);
  slot->s.active = 1;
}
