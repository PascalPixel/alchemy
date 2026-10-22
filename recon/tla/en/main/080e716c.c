// Near miss, differ 2 of 88 (2026-10-15): object 79 at the anchor, fetched
// or spawned (kind 489), takes a random item id from gData_080f0fc0's
// sixteen (0x8001 when PartyInventory_FindRoomFar finds no room) or, past
// 0xc000 of the roll, a 0x8000-flagged value scaled by the unit's level,
// and is handed to BattleEffect_DropObjectAtAnchor. One pair: the native loads
// gPartyState's address before it saves the event work in r8 (ldr r3,
// =gPartyState; mov r8, r2); this does it the other way. Measured: without the
// empty asm barrier before the anchor's load, differ 4; on 2026-10-19 the party
// state through a pointer behind an asm taking it with the event work as
// "+h" (2) or "+l" (26), an asm reading the event work after its load (66),
// and permute 4 min found nothing better. Belongs between
// anchor_drop.c's BattleEffect_DropObjectAtAnchor and Func_080e7238.
#include "types.h"
#include "ram_buffer.h"
#include "object_runtime.h"
#include "party_state.h"
#include "event_work.h"
#include "battle_unit.h"

struct AnchorWork {
  u8 unk0000[0x1930];
  struct ObjectRuntime *anchor;
};

struct DropEvent {
  u8 unk000[0xd34];
  u16 item;
};

struct ObjectRuntime *Object_GetById(s32 id);
struct ObjectRuntime *Func_080200c0(s32 kind, s32 x, s32 y, s32 z);
void *Unit_Get(s32 owner);
s32 PartyInventory_FindRoomFar(s32 item);
void Func_080d3b28(s32 id, s32 item);
void BattleEffect_DropObjectAtAnchor(s32 id);
u32 Random16(void);
extern const u16 gData_080f0fc0[];

void Func_080e716c(void) {
  struct AnchorWork *work = (struct AnchorWork *)Ram_HeapSlots->battleFxWork[0];
  struct ObjectRuntime *anchor;
  struct EventWork *event;
  struct BattleUnit *unit;
  struct ObjectRuntime *object;
  s32 item;
  s32 id = 79;

  asm("");
  anchor = work->anchor;
  event = Ram_HeapSlots->eventWork;
  unit = Unit_Get(gPartyState.currentOwner);

  object = Object_GetById(id);
  if (object == NULL) {
    object = Func_080200c0(489, anchor->x, anchor->y, anchor->z);
    if (object == NULL)
      return;
    event->objects[id] = object;
  }
  if (Random16() < 0xc000) {
    item = gData_080f0fc0[(Random16() * 16) >> 16];
    if (PartyInventory_FindRoomFar(item) == -1)
      item = 0x8001;
  } else {
    item = (((Random16() * unit->level) * 2) >> 16) + 1 | 0x8000;
  }
  object->x = anchor->x;
  object->y = anchor->y;
  object->z = anchor->z;
  Func_080d3b28(id, item);
  ((struct DropEvent *)event)->item = item;
  BattleEffect_DropObjectAtAnchor(id);
}
