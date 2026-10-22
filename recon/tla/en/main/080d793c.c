// Near miss, differ 6 of 131 (ported from The Broken Seal's
// Djinn_ResolvePendingEvent, with ⚓️'s actors up to 79, the plain flag
// calls, the event at the party state's 0x254 and its 0x1000 kind tested
// like the first, falling into the common clear): three scheduling swaps
// remain. The native builds 0xfff's first half (movs r6, #240) after the
// two reads of the pending event, and in both range checks loads -300
// (ldr r0) before the last add of 0x7ff; this draft does each the other
// way. In sched2 the two candidates tie on priority and the constant's add
// has more dependents. The flag as u32, the reads swapped, the mask in the
// test, blocks or an empty asm around the reads differ 6 to 166; permute
// (4 min) found nothing. It joins battle/effect/slots.c, ahead of
// BattleEffect_UpdateAllSlots, when it matches.
#include "types.h"
#include "party_state.h"
#include "system.h"

void GameFlag_SetBit(s32 flag);
s16 *Scene_FindActorPlacement(s32 action);
void Func_080d7788(s32 actor, s32 element, s32 index);

struct MapActor {
  s16 id;
  s16 event;
};

void Func_080d793c(s32 capture) {
  struct PartyState *st = &gPartyState;
  s16 *pending = &st->pendingDjinnEvent;
  s32 kind = *pending & 0xf000;
  s32 flag = *pending & 0xfff;

  if (capture == 0) {
    if (kind == 0) {
      flag &= 0x7ff;
      if (flag < 300 || flag > 380)
        return;
      if (st->unk256 > 0 && st->unk256 != 999)
        return;
      GameFlag_SetBit(flag - 172);
      *pending = kind;
      return;
    } else if (kind == 0x1000) {
      if (st->unk256 > 0 && st->unk256 != 999)
        GameFlag_SetBit(flag);
    } else
      return;
  } else if (kind == 0) {
    flag &= 0x7ff;
    if (flag >= 300 && flag <= 380) {
      flag &= 0x7ff;
      if (st->unk256 > 0) {
        s32 djinn = flag - 300;
        s32 element = djinn / 20;
        s32 index = djinn % 20;
        s32 actor;

        for (actor = 8; actor <= 79; actor++) {
          struct MapActor *found =
              (struct MapActor *)Scene_FindActorPlacement(actor);
          if (found != 0 && found->event - 48 == flag - 300) {
            WaitFrames(40);
            Func_080d7788(actor, element, index);
            break;
          }
        }
      }
    }
  }
  gPartyState.pendingDjinnEvent = 0;
}
