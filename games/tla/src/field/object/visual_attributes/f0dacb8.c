/// @file
/// Finding a slot of a chain work, with routines not yet understood.
// Part of visual_attributes.c.

#include "../visual_attributes_internal.h"

/* VIEW(f0dacb8): tag struct ChainSlot as struct ChainSlot__f0dacb8; visual_attributes.c has another definition; tried with the definition the rest of the object has, it does not compile in tla-ja: games/tla/src/field/object/visual_attributes.c:6731: structure has no member named `root'. */
struct ChainSlot__f0dacb8 {
  s16 objectId;
  u8 unk02[2];
  s32 swing;   // the arc's width, eased each frame
  s32 spacing; // between links
  u8 unk08[4];
  s32 phase;
  s32 active;
  struct ChainLink *root;
};

/* VIEW(f0dacb8): tag struct ChainWork as struct ChainWork__f0dacb8; visual_attributes.c has another definition; tried with the definition the rest of the object has, Func_080dacb8 moves: 1 of 173 instructions in tla-ja. */
struct ChainWork__f0dacb8 {
  u8 unk00[4];
  struct ChainSlot__f0dacb8 slots[8];
};

struct Actor *Actor_GetById(u32 objectId);
s32 Party_GetLeaderActor(void);

#define CHAIN_WORK ((struct ChainWork__f0dacb8 *)Ram_HeapSlots->unk84[8])

void Func_080dacb8(void) {
  struct ChainWork__f0dacb8 *work = CHAIN_WORK;
  struct ChainSlot__f0dacb8 *slot;
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


/// Returns the first of WORK_A4's eight slots with no chain, or NULL.
struct WorkA4Slot *Func_080dae3c(void) {
  struct WorkA4 *work = WORK_A4;
  struct WorkA4Slot *found = NULL;
  s32 index;
  /* FAKEMATCH(dead): index set to 0 before the loop sets it again, so its
   * zero is copied from found's register (adds r1, r0, #0); without it, 2 of
   * 26 instructions differ in tla-en. */
  index = 0;

  for (index = 0; index <= 7; index++) {
    if (work->slots[index].root == NULL) {
      found = &work->slots[index];
      break;
    }
  }
  return found;
}

/// Returns the slot among WORK_A4's eight that holds id, or NULL.
struct WorkA4Slot *Func_080dae70(s32 id) {
  struct WorkA4 *work = WORK_A4;
  struct WorkA4Slot *found = NULL;
  s32 index;
  /* FAKEMATCH(dead): index set to 0 before the loop sets it again, so its
   * zero is copied from found's register (adds r1, r5, #0); without it, 2 of
   * 27 instructions differ in tla-en. */
  index = 0;

  for (index = 0; index <= 7; index++) {
    if (work->slots[index].id == id) {
      found = &work->slots[index];
      break;
    }
  }
  return found;
}

struct WorkA4Item *Func_080daea8(void) {
  struct WorkA4 *work = WORK_A4;
  struct WorkA4Item *item = work->items;
  s32 index;

  for (index = 0; index <= 127; index++) {
    if (item->used == 0)
      return item;
    item++;
  }
  return NULL;
}
#undef CHAIN_WORK
