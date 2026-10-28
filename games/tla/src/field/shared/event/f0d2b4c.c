#include "types.h"
#include "event_work.h"
#include "object_runtime.h"
#include "adventure.h"
#include "ram_buffer.h"

void Func_080d7524(s32 element);
s32 Encounter_GetDjinnEnemyGroup(s32 djinn);
void Encounter_ResolveWorldMapZoneAndBackdrop(s32 *position);
void BattleEffect_SelectBattleCue(s32 cue, s32 mode);

extern s8 gData_02001000[];

/// Queues a Djinni battle. djinn / 20 is the element and djinn % 20 the
/// Djinni, where 19 means the one recorded for the element in the save (none:
/// nothing happens). Unless flag 0x800 is set the element's encounter is
/// prepared first; the pending event is 300 + the Djinni id with that flag,
/// the enemy group is chosen, and on the world map the zone and backdrop come
/// from the leader's position.
void Func_080d2b4c(u32 flags, u32 djinn) {
  u8 *work = Ram_HeapSlots->eventWork;
  u32 noPrepare = flags & 0x800;
  u32 group;
  s32 index;

  flags &= 0xff;
  group = djinn / 20;
  index = djinn % 20;

  if (index == 19) {
    index = gData_02001000[group + 0x1950];
    if (index == 0)
      return;
    index--;
  }
  djinn = group * 20 + index;
  if (noPrepare == 0)
    Func_080d7524(flags);
  gAdventure.pendingDjinnEvent = (djinn + 300) | noPrepare;
  *(s16 *)(work + 356) = Encounter_GetDjinnEnemyGroup(djinn);
  if (*(s8 *)(work + 394) == 3)
    Encounter_ResolveWorldMapZoneAndBackdrop(
        &ObjectTable_Get(Party_GetLeaderActor())->x);
  BattleEffect_SelectBattleCue(0, 0);
}
