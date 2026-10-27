// Under -mthumb-slow-loads (2026-10-26): differ 32 of 378 in tla-en,
// instruction order and register choice. Empty asms before `leader->motionFlags
// = 0;` and the root's x lower it to 22.
// Near miss, differ 40 of 378 (2026-10-25): the leader swings on the chain
// hanging from object `id` (its WORK_A4 slot, Func_080dae70). The chain first
// swings to the leader's side and over to the other, then holds there while the
// leader hangs on it (mode 32), the swing pulsing every 16 frames and the
// sprite tilting with it. Down, B or the direction back toward the anchor lets
// go; any other key jumps: cue 152, the leader is thrown up and out on the
// chain (its radius growing for 8 frames) until the swing comes back round,
// then falls with its motion flags restored. Left: instruction order only (the
// phase load before the side's constant, the root's next and the 0x410000
// radius, the growth step's constants, the frame count's case loads).
// The side's bits written out at each use (side << 15) rather than kept in
// a local give the native's frame (found by permute; differ 65 with the
// local).
#include "types.h"
#include "event_work.h"
#include "fixed_math.h"
#include "frame.h"
#include "input.h"
#include "iwram_call.h"
#include "object_runtime.h"
#include "worka0.h"

struct ObjectRuntime *Object_GetById(s32 id);
struct WorkA4Slot *Func_080dae70(s32 id);
void ObjectDispatch_SetSingleChildFlagsFar(struct ObjectRuntime *object,
                                           u32 flags);
void WaitTicks(s32 ticks);

struct ChainLink {
  struct ChainLink *next;
  s32 x;
  s32 y;
  s32 z;
};

struct ChainSlot {
  s16 id;
  u8 unk02[2];
  s32 swing;   ///< the arc's width
  s32 spacing; ///< between links
  u8 unk0c[4];
  s32 phase;
  s32 active;
  struct ChainLink *root;
};

struct SwingSprite {
  u8 unk00[18];
  u16 angle;
};

void Func_080db0b0(s32 id) {
  s32 rootX, rootY;
  s32 motionFlags;
  struct SwingSprite *heldSprite;
  s32 side = 0;
  struct ObjectRuntime *leader =
      Object_GetById(EventRuntime_GetControlledOwner());
  struct ObjectRuntime *anchor = Object_GetById(id);
  struct ChainSlot *slot;
  struct SwingSprite *sprite;
  s32 swing;
  s32 start;
  s32 dir;
  u16 letGo;
  s32 radius;
  s32 steps;
  s32 angle;
  s32 i;

  if (leader == NULL || anchor == NULL)
    return;
  motionFlags = leader->motionFlags;
  heldSprite = leader->animation;
  if (leader->x < anchor->x)
    side = 1;
  slot = (struct ChainSlot *)Func_080dae70(id);
  if (slot == NULL)
    return;

  start = slot->swing;
  if ((slot->phase ^ (side << 15)) & 0x8000) {
    for (swing = start; swing >= 0; swing -= 64) {
      slot->swing = swing;
      WaitTicks(1);
    }
    start = 0;
  }
  for (swing = start; swing <= 0x600; swing += 64) {
    slot->swing = swing;
    slot->phase = (side << 15) + 0x4000;
    WaitTicks(1);
  }
  while ((slot->phase & 0xff00) != ((side ^ 1) << 15) + 0x4000) {
    slot->swing = 0x600;
    WaitTicks(1);
  }

  sprite = leader->animation;
  dir = 1;
  if (side) {
    letGo = 0xa2;
    dir -= 2;
  } else
    letGo = 0x92;
  slot->active = 1;
  Object_SetMode(leader, 32);
  while (1) {
    slot->phase = ((side ^ 1) << 15) + 0x4000;
    switch (gFrameCount & 63) {
    case 0:
    case 16:
      slot->swing = 0x620;
      slot->spacing = 0xe8000;
      sprite->angle = dir << 10;
      break;
    case 1:
    case 2:
    case 17:
    case 18:
      slot->swing += 16;
      slot->spacing += 0x4000;
      sprite->angle += dir << 9;
      break;
    default:
      if (slot->swing > 0x600)
        slot->swing -= 32;
      else
        slot->swing = 0x600;
      if (slot->spacing > 0xe0000)
        slot->spacing -= 0x800;
      if (sprite->angle != 0)
        sprite->angle -= dir << 7;
      break;
    }
    if (gInput.pressed & letGo) {
      sprite->angle = 0;
      slot->active = 1;
      return;
    }
    if (gInput.pressed != 0)
      break;
    WaitTicks(1);
  }

  sprite->angle = 0;
  slot->active = 0;
  Object_SetMode(leader, 6);
  WaitTicks(6);
  Audio_PlayCue(152);
  Object_SetMode(leader, 7);
  leader->speedLimit = 0x30000;
  leader->acceleration = 0x20000;
  leader->velocityY = 0x40000;
  leader->motionFlags &= 0x7e;
  ObjectDispatch_SetSingleChildFlagsFar(leader, 0);
  WaitTicks(5);
  Object_SetMode(leader, 16);
  leader->motionFlags = 0;
  rootX = slot->root->next->x;
  rootY = slot->root->next->y;
  radius = 0x410000;
  steps = 8;
  while ((slot->phase & 0xff00) != (side << 15) + 0x3800) {
    if (steps != 0) {
      slot->spacing += 0x1000;
      radius += 0x5000;
      steps--;
    }
    slot->swing = 0x800;
    slot->active = 1;
    angle = Iwram_MulQ16(Trig_Sin(slot->phase - 0xfa0),
                         slot->swing + slot->swing * 5 / 2);
    leader->x = rootX - Iwram_MulQ16(Trig_Sin(angle), radius);
    leader->y = rootY - Iwram_MulQ16(Trig_Cos(angle), radius);
    leader->targetX = leader->x;
    leader->targetY = leader->y;
    heldSprite->angle = angle;
    WaitTicks(1);
  }
  slot->active = 1;
  Object_SetMode(leader, 7);
  leader->motionFlags = motionFlags & ~1;
  leader->velocityY = 0x40000;
  for (i = 7; i >= 0; i--) {
    slot->spacing -= 0x1000;
    WaitTicks(1);
  }
  slot->spacing = 0xe0000;
  heldSprite->angle = 0;
  Object_SetMode(leader, 6);
  ObjectDispatch_SetSingleChildFlagsFar(leader, 1);
  leader->motionFlags = motionFlags;
  WaitTicks(6);
  slot->active = 1;
}
