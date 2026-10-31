// Near miss, differ 6 of 178 (tla-en, 2026-10-31; 80 before, 2026-10-26):
// two sched2 priority orders. The per-frame routine
// Func_080da9a8 schedules second: for each of the eight chains in the heap-
// offset-164 work (worka0.h's WorkA4 slots), advance its phase and hang its
// five links from the anchor in a swinging arc, then ease the swing towards
// 32 when the leader stands right under the anchor, or back to 256
// otherwise. The native reuses one counter for the chains and the links (the
// link loop leaves it at 6, so after one chain the outer loop takes one more
// and stops); this keeps that.
// Each slot starts with the object id (stride 28 from work + 4), and the
// links' phase is slot->phase - i * 800, which loop.c strength-reduces to
// the native's spilled step. Left, both sched2 priority (the --rtl dump):
// the native stores the leader and the target before it computes the first
// slot's address (adds r5, #4 has priority 2 over the stores' 1; a one-pass
// block around the leader and target lines ends the region there, differ 4,
// not kept), and stores the first step (800, loop.c's giv init, emitted
// before the link loop's LOOP_BEG, after i = 1 and sway = swing) before it
// sets i to 1 (movs r3, #1 feeding mov r9, r3 outranks it); an explicit step
// variable (20) and a do-while link loop (no change) did not move it.
#include "types.h"
#include "ram_buffer.h"
#include "actor.h"
#include "iwram_call.h"
#include "fixed_math.h"

struct ChainLink {
  struct ChainLink *next;
  s32 x;
  s32 y;
  s32 z;
};

struct ChainSlot {
  s16 objectId;
  u8 unk02[2];
  s32 swing;   // the arc's width, eased each frame
  s32 spacing; // between links
  u8 unk08[4];
  s32 phase;
  s32 active;
  struct ChainLink *root;
};

struct ChainWork {
  u8 unk00[4];
  struct ChainSlot slots[8];
};

struct Actor *Actor_GetById(s32 id);
s32 Party_GetLeaderActor(void);

#define CHAIN_WORK ((struct ChainWork *)Ram_HeapSlots->unk84[8])

void Func_080dacb8(void) {
  struct ChainWork *work = CHAIN_WORK;
  struct ChainSlot *slot;
  struct Actor *leader;
  s32 x, y, z;
  struct ChainLink *link;
  s32 i;
  s32 target;
  s32 phase;
  s32 swing;
  s32 sway;
  s32 radius;
  s32 angle;

  i = 0;
  leader = Actor_GetById(Party_GetLeaderActor());
  target = 0x100;
  for (slot = work->slots; i <= 7; i++, slot++) {
    if (slot->root == 0 || slot->active == 0)
      continue;

    slot->phase += 0x200;
    link = slot->root->next;
    x = link->x;
    y = link->y;
    z = link->z;
    swing = slot->swing;

    // Swing less while the leader stands within 8 pixels across, a pixel
    // deep, and between 8 above and 80 below the anchor.
    if ((x - leader->x >= 0 ? x - leader->x <= 0x7ffff
                            : leader->x - x <= 0x7ffff) &&
        (u32)(z - leader->z + 0xfffff) <= 0xffffe &&
        y + (8 << 16) > leader->y && y - (80 << 16) < leader->y)
      target = 32;

    // Hang the five links below the anchor.
    // BUG: the link loop reuses the chain counter, which it leaves at 6, so
    // after the first active chain only the chain after it is updated.
    link = link->next;
    for (i = 1, sway = swing; i <= 5; i++) {
      radius = i * slot->spacing;
      angle = Iwram_MulQ16(Trig_Sin(slot->phase - i * 800), swing + sway / 2);
      link->x = x - Iwram_MulQ16(Trig_Sin(angle), radius);
      sway += swing;
      link->y = y - Iwram_MulQ16(Trig_Cos(angle), radius);
      link->z = z;
      link = link->next;
    }

    // Ease the swing towards the target.
    if (slot->swing > target)
      slot->swing = Iwram_MulQ16(slot->swing, 0xfd70);
    else
      slot->swing = Iwram_MulQ16(slot->swing, 0x11c28);
  }
}
