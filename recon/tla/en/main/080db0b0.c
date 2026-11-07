// Matches: differ 0 of 378 (tla-en, 2026-11-07).
// History: differ 14 (2026-11-06) closed by reading the slot through a union
// of its views and by a once-only block around the first sine.
// The leader swings on the chain hanging from actor `id` (its WORK_A4 slot,
// Func_080dae70). The chain first swings to the leader's side and over to the
// other, then holds there while the leader hangs on it (mode 32), the swing
// pulsing every 16 frames and the sprite tilting with it. Down, B or the
// direction back toward the anchor lets go; any other key jumps: cue 152, the
// leader is thrown up and out on the chain (its radius growing for 8 frames)
// until the swing comes back round, then falls with its motion flags restored.
// The side's bits are written out at each use (side << 15) rather than kept in
// a local; the local gives a different stack frame.
#include "types.h"
#include "event_work.h"
#include "fixed_math.h"
#include "frame.h"
#include "input.h"
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

// The slot is read through a union of its views: the original's reads and
// stores of it alias every other access, as only a union access does here.
union ChainSlotView {
  struct WorkA4Slot slot;
  struct ChainSlot chain;
};

struct Unk_080db0b0Sprite {
  u8 unk00[18];
  u16 unk12;
};

void Func_080db0b0(s32 id) {
  s32 rootX, rootY;
  s32 motionFlags;
  struct Unk_080db0b0Sprite *heldSprite;
  s32 side = 0;
  struct Actor *leader = Actor_GetById(Party_GetLeaderActor());
  struct Actor *anchor = Actor_GetById(id);
  union ChainSlotView *slot;
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
  slot = (union ChainSlotView *)Func_080dae70(id);
  if (slot == NULL)
    return;

  start = slot->chain.swing;
  if ((slot->chain.phase ^ (side << 15)) & 0x8000) {
    for (swing = start; swing >= 0; swing -= 64) {
      slot->chain.swing = swing;
      wait(1);
    }
    start = 0;
  }
  for (swing = start; swing <= 1536; swing += 64) {
    slot->chain.swing = swing;
    slot->chain.phase = (side << 15) + 0x4000;
    wait(1);
  }
  while ((slot->chain.phase & 0xff00) != ((side ^ 1) << 15) + 0x4000) {
    slot->chain.swing = 1536;
    wait(1);
  }

  sprite = leader->animation;
  dir = 1;
  if (side) {
    letGo = 0xa2;
    dir -= 2;
  } else
    letGo = 0x92;
  slot->chain.active = 1;
  Actor_SetMode(leader, 32);
  while (1) {
    slot->chain.phase = ((side ^ 1) << 15) + 0x4000;
    switch (gFrameCount & 63) {
    case 0:
    case 16:
      slot->chain.swing = 1568;
      slot->chain.spacing = Q16_ONE * 29 / 2;
      sprite->unk12 = dir << 10;
      break;
    case 1:
    case 2:
    case 17:
    case 18:
      slot->chain.swing += 16;
      slot->chain.spacing += Q16_ONE / 4;
      sprite->unk12 += dir << 9;
      break;
    default:
      if (slot->chain.swing > 1536)
        slot->chain.swing -= 32;
      else
        slot->chain.swing = 1536;
      if (slot->chain.spacing > Q16_ONE * 14)
        slot->chain.spacing -= Q16_ONE / 32;
      if (sprite->unk12 != 0)
        sprite->unk12 -= dir << 7;
      break;
    }
    if (gInput.pressed & letGo) {
      sprite->unk12 = 0;
      slot->chain.active = 1;
      return;
    }
    if (gInput.pressed != 0)
      break;
    wait(1);
  }

  sprite->unk12 = 0;
  slot->chain.active = 0;
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
  rootX = slot->chain.root->next->x;
  rootY = slot->chain.root->next->y;
  radius = Q16_ONE * 65;
  steps = 8;
  while ((slot->chain.phase & 0xff00) != (side << 15) + 0x3800) {
    if (steps != 0) {
      radius += Q16_ONE * 5 / 16;
      slot->chain.spacing += Q16_ONE / 16;
      steps--;
    }
    slot->chain.swing = 2048;
    slot->chain.active = 1;
    // A once-only block: the sine is taken before Iwram_MulQ16 is loaded.
    do {
      sine = Trig_Sin(slot->chain.phase - 4000);
    } while (0);
    angle = Iwram_MulQ16(sine, slot->chain.swing + slot->chain.swing * 5 / 2);
    leader->x = rootX - Iwram_MulQ16(Trig_Sin(angle), radius);
    leader->y = rootY - Iwram_MulQ16(Trig_Cos(angle), radius);
    leader->targetX = leader->x;
    leader->targetY = leader->y;
    heldSprite->unk12 = angle;
    wait(1);
  }
  slot->chain.active = 1;
  Actor_SetMode(leader, 7);
  leader->motionFlags = motionFlags & ~1;
  leader->velocityY = Q16_ONE * 4;
  for (i = 7; i >= 0; i--) {
    slot->chain.spacing -= Q16_ONE / 16;
    wait(1);
  }
  slot->chain.spacing = Q16_ONE * 14;
  heldSprite->unk12 = 0;
  Actor_SetMode(leader, 6);
  Actor_SetSingleChildFlagsFar(leader, 1);
  leader->motionFlags = motionFlags;
  wait(6);
  slot->chain.active = 1;
}
