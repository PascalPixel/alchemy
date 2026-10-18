// Draft, differ 31 of 196: starts the random particle emitter at an event
// actor. The leader's emitter goes one step ahead of it, snapped to the cell
// centre; in particle mode the old spawners and particles go first. The new
// object takes its sprite tiles from a decoded heap block, and in particle mode
// replaces any marker on the same cell. No ☀️ twin found. Left: register
// choice in the last loop, where the native keeps the object and count in r0
// and r1 and this draft in r5 and r6 (a search loop with the destroy after it
// differs more), with a few moves one slot apart before it. The event actors
// are an array field (through a cast pointer the slot address is formed the
// other way round: differ 33); the sprite stores are FieldSprite bitfields
// (QImode masks -33 and -13).
#include "types.h"
#include "event_work.h"
#include "field_sprite.h"
#include "object_runtime.h"
#include "ram_buffer.h"

void WaitFrames(s32 count);
void Object_Destroy(struct ObjectRuntime *object);
void Vector_AddPolarOffset(s32 distance, s32 angle, s32 *position);
struct ObjectRuntime *Func_080200c0(s32 sprite, s32 x, s32 y, s32 z);
void ObjectDispatch_InitializeFar(struct ObjectRuntime *object,
                                  const void *script);
void *Runtime_AllocateHeapBlock(s32 slot, s32 size);
void Runtime_ReleaseHeapBlock(s32 slot);
void Func_08038248(s32 kind);
s32 VramCache_Load(u32 slot, u32 size, const void *source);
void BattleFx_SpawnRandomParticleAtPosition(void);
void Marker_MoveLeaderAwayFromEmitter(void);

struct EventActors {
  u8 unk00[20];
  struct ObjectRuntime *actors[64];
};

extern s32 gBattleFx_ParticleScript[];
extern s32 Data_080f01b8[];
extern const u8 gData_080effd8[];

struct ObjectRuntime *BattleFx_StartRandomParticleEmitter(s32 id, s32 kind) {
  u8 *work = Ram_HeapSlots->eventWork;
  struct ObjectRuntime *actor = ((struct EventActors *)work)->actors[id];
  struct ObjectRuntime *object;
  struct FieldSprite *sprite;
  u8 *tiles;
  s32 position[3];
  s32 i;

  if (actor == NULL)
    return NULL;
  position[0] = actor->x;
  position[1] = actor->y;
  position[2] = actor->z;
  if (id == EventRuntime_GetControlledOwner()) {
    Vector_AddPolarOffset(0x100000, actor->angle, position);
    position[0] = (position[0] & 0xfff00000) + 0x80000;
    position[2] = (position[2] & 0xfff00000) + 0x80000;
  }
  if (*(s8 *)(work + 0xca4) != 0) {
    object = Ram_HeapSlots->scriptObjects;
    for (i = 63; i >= 0; i--, object++) {
      if (object->script != NULL) {
        if (object->update == BattleFx_SpawnRandomParticleAtPosition)
          Object_Destroy(object);
        if (object->script == gBattleFx_ParticleScript)
          Object_Destroy(object);
      }
    }
  }
  WaitFrames(3);
  actor = Func_080200c0(489, position[0], position[1] + 0x100000, position[2]);
  if (actor == NULL)
    return NULL;
  ObjectDispatch_InitializeFar(actor, gData_080effd8);
  sprite = actor->animation;
  sprite->nextFrame = 0;
  sprite->unk1b[0] = 0;
  sprite->fullColor = 0;
  sprite->palette = 0;
  sprite->priority = 1;
  actor->motionFlags = 2;
  actor->terrainHeight = position[1];
  actor->velocityY = 0x20000;
  actor->gravity = 0x4000;
  tiles = Runtime_AllocateHeapBlock(68, 0x608);
  Func_08038248(kind);
  sprite->tile = VramCache_Load(sprite->vramBlock, 128, tiles + 0x400);
  Runtime_ReleaseHeapBlock(68);
  actor->update = Marker_MoveLeaderAwayFromEmitter;
  if (*(s8 *)(work + 0xca4) != 0) {
    object = Ram_HeapSlots->scriptObjects;
    for (i = 0; i <= 63; i++, object++) {
      if (object->script != NULL && object->script == Data_080f01b8 &&
          object->x >> 20 == actor->x >> 20 &&
          object->z >> 20 == actor->z >> 20) {
        Object_Destroy(object);
        break;
      }
    }
  }
  return actor;
}
