// Differ 31 of 119 (tla-en, 2026-11-08, -mtune=gba).
// 2026-11-08, no gain: owner = owners after the call, before or in the for init
// (62, 71), set inside the call's argument (31), owners[i] or &owners[i] in the
// loop (69, 71), owner++ after the call, items += 108 apart, the inner for as a
// while, declaration order (all 31). An 8-minute permute compiled almost none of
// its candidates (SHAMAN_VILLAGE_EMOTE) and found nothing.
// Was differ 56 under -Os (2026-11-05) and 75 (2026-11-01); the register notes below
// predate -Os. Now the countdown sits in r10 as native; left: native rebuilds sp
// (mov r0, sp for the call, mov r7, sp for the owner pointer) and loads the id into
// r5, while the draft keeps sp in r6 and puts items r5, slot r7: allocation only.
// Measured with no gain (2026-11-06): Unit_Get(*owner++), id split from owner++,
// s32 id; owner = owners after the call (62) or in the for init (71); 4 minutes of
// permuter (best still 31); 10 more minutes on 6 November (6,000 candidates),
// still 31. The id held in callee-saved r5 before the copy to r0 suggests the
// native's id pseudo crosses a call, which takes the callee-saved register
// the frame address keeps here.
// Near miss of Func_0200290c of overlay 696. Control flow, loads and scheduling
// agree; what is left is one allocation. The native gives items r6, slot r5, the
// owner pointer r7, i r8, count r9 and the countdown r10, with a 20-byte frame.
// The draft gives the countdown r7 and the owner pointer r9, and moves items and
// slot to r5 and r6, so the gap is copies between low and high registers.
// Why: the countdown has 13 weighted refs across 74 insns (priority 0.53), the
// owner pointer 7 across 58 (0.24), and with 2 calls crossed 13 > 4 * 2 makes
// caller-save profitable, so r10 needs at most 8 refs or at least 4 calls, or a
// preferred class other than LO_REGS. Init, final test, after-inner test and
// bottom test plus the decrement cannot come to 8 at these loop depths, and no
// source form tried lowers it.
// A second symptom: the native rebuilds sp for the call and for the owner
// pointer. Inferred, not shown: its frame-address pseudo (reg 40, hoisted by PRE)
// gets no register and is rematerialized; the draft keeps it in r5. It also holds the owner id in
// r5 before copying to r0, where the draft loads r0 directly (a named id local,
// s32 or s16, is merged into the same load).
// Found this round: the emote arm comes first, reached by goto from the failed
// owner test (127 to 78); owner = &owners[i] is the giv loop.c reduces, where
// owners[i] alone is "not worth while" and costs a spill; items[slot] through an
// s32 item gives ldrh r0 (78 to 75).
// Measured with no gain: u8, s32 or u32 countdown, remaining--, declaration
// orders, slot scoped to the loop, a unit local, *(owners + i), while for the
// outer for, the countdown in either loop condition. Func_020049cc called plainly
// costs 2 (77), so SHAMAN_VILLAGE_EMOTE stays. Goto loops were not measured: the
// one attempt did not compile, and refs do not change with them.
#include "../../../../games/tla/src/scene/shaman_village/shaman_village.h"

#define EVENT_WORK ((struct ShamanVillageEventWork *)Ram_HeapSlots->eventWork)

s32 PartyInventory_FindOwner(s32 item);
u8 *Unit_Get(s32 owner);
u8 *Func_02004894(s32 item);
void Actor_WatchAndSetScript(s32 actor, s32 target);

void Func_0200290c(void) {
  s16 owners[10];
  s32 count;
  s32 i;
  s32 slot;
  u32 remaining;
  u16 *items;
  s16 *owner;

  Func_0200498c((s32)MsgShamanMissingShamanRod);
  if (PartyInventory_FindOwner(65) == -1)
    goto refuse;
  owner = owners;
  count = Party_ListActiveOwners(owners);
  remaining = 8;
  for (i = 0; i < count; i++) {
    s16 id = *owner++;
    items = (u16 *)(Unit_Get(id) + 216);
    for (slot = 0; slot < 15; slot++) {
      s32 item = items[slot];
      if (item == 0)
        break;
      if ((u8)(Func_02004894(item)[2] - 1) <= 3)
        remaining = (u8)(remaining - 1);
      if (remaining == 0)
        break;
    }
    if (remaining == 0)
      break;
  }
  EVENT_WORK->field1c4++;
  if (remaining != 0) {
  refuse:
    /* FAKEMATCH: the emote and the frames are loaded before the actor; see
     * SHAMAN_VILLAGE_EMOTE. */
    SHAMAN_VILLAGE_EMOTE(15, 257, 20)
    ShamanVillage_MessagePut(0x200f, 0);
    Func_0200289c();
  } else {
    EVENT_WORK->field1c4++;
    ShamanVillage_MessagePut(0x200f, 0);
    Actor_WatchAndSetScript(16, gMainRoutineWork.currentOwner);
    Actor_WatchAndSetScript(17, gMainRoutineWork.currentOwner);
    Actor_WatchAndSetScript(15, gMainRoutineWork.currentOwner);
    GameFlag_SetBit(0x9e0);
    GameFlag_SetBit(0x201);
  }
}
