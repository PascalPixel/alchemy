#include "types.h"
#include "ram_buffer.h"
#include "actor.h"
#include "macros.h"

struct AnchorWork {
  u8 unk0000[0x1930];
  struct Actor *anchor;
};

struct ModeEvent {
  u8 unk000[352];
  u16 cue;
};

struct Actor *Actor_GetById(s32 id);
void WaitFrames(s32 frames);

/// Puts object id at the effect's anchor (if any) and lets it fall under
/// gravity, waits 50 frames, then cues the event with id | 0x1000.
void BattleEffect_DropObjectAtAnchor(s32 id) {
  struct ModeEvent *event = Ram_HeapSlots->eventWork;
  struct Actor *anchor;
  struct Actor *object;

  anchor = ((struct AnchorWork *)Ram_HeapSlots->battleFxWork[0])->anchor;
  object = Actor_GetById(id);
  if (anchor != NULL) {
    SET_POSITION(object, anchor->x, anchor->y, anchor->z);
  }
  if (*((s8 *)event + 394) == 3) {
    object->motionFlags = 2;
    object->terrainHeight = 0;
  } else {
    object->motionFlags = 3;
  }
  object->gravity = 0x170a;
  WaitFrames(50);
  event->cue = id | 0x1000;
}
