#include "types.h"
#include "ram_buffer.h"
#include "object_runtime.h"

struct EmitterWork {
  u8 unk000[0x14];
  struct ObjectRuntime *actors[(0xca4 - 0x14) / 4];
  s8 revealActive;
};

struct EffectVisual {
  u8 unk00[5];
  u8 flags05;
  u8 unk06[2];
  u16 tile : 10;
  u16 attributes : 6;
  u8 unk0a[6];
  u8 tileSlot;
  u8 unk11[9];
  u8 value1a;
  u8 value1b;
};

extern const u8 gData_080effd8[];
extern const u8 gBattleEffect_ParticleScript[];
extern const u8 Data_080f01b8[];
s32 EventRuntime_GetControlledOwner(void);
void Vector_AddPolarOffset(s32 length, s32 angle, s32 *position);
void Object_Destroy(struct ObjectRuntime *object);
void WaitTicks(s32 ticks);
struct ObjectRuntime *Func_080200c0(s32 kind, s32 x, s32 y, s32 z);
void ObjectDispatch_InitializeFar(struct ObjectRuntime *object,
                                  const u8 *script);
void *Runtime_AllocateHeapBlock(s32 slot, s32 size);
void Runtime_ReleaseHeapBlock(s32 slot);
void Func_08038248(s32 item);
s32 VramCache_Load(u32 slot, u32 size, const void *picture);
void BattleEffect_SpawnRandomParticleAtPosition(void);
void Marker_MoveLeaderAwayFromEmitter(void);

/// Shows the picture of item above actor (in front of the controlled owner,
/// on the centre of the cell), clearing Reveal's sparkles first.
struct ObjectRuntime *BattleEffect_StartRandomParticleEmitter(s32 actor,
                                                              s32 item) {
  struct EmitterWork *work = Ram_HeapSlots->eventWork;
  struct ObjectRuntime *picture = work->actors[actor];
  struct ObjectRuntime *object;
  struct EffectVisual *visual;
  s32 position[3];
  u8 *buffer;
  s32 tile;
  s32 i;
  s32 zero;
  s32 mask;

  if (picture == NULL)
    return NULL;
  position[0] = picture->x;
  position[1] = picture->y;
  position[2] = picture->z;
  if (actor == EventRuntime_GetControlledOwner()) {
    Vector_AddPolarOffset(0x100000, picture->angle, position);
    position[0] = (position[0] & 0xfff00000) + 0x80000;
    position[2] = (position[2] & 0xfff00000) + 0x80000;
  }
  if (work->revealActive != 0) {
    object = Ram_HeapSlots->scriptObjects;
    i = 63;
    do {
      if (object->script != NULL) {
        if (object->update == BattleEffect_SpawnRandomParticleAtPosition)
          Object_Destroy(object);
        if (object->script == (s32 *)gBattleEffect_ParticleScript)
          Object_Destroy(object);
      }
      i--;
      object++;
    } while (i >= 0);
  }
  WaitTicks(3);
  picture =
      Func_080200c0(489, position[0], position[1] + 0x100000, position[2]);
  if (picture == NULL)
    return NULL;
  ObjectDispatch_InitializeFar(picture, gData_080effd8);
  visual = picture->animation;
  zero = 0;
  visual->value1a = zero;
  visual->value1b = zero;
  visual->flags05 &= zero - 33;
  mask = ((u8 *)visual)[9] & 0x0f;
  {
    s32 clear = 13;

    clear = -clear;
    mask &= clear;
  }
  mask |= 4;
  ((u8 *)visual)[9] = mask;
  picture->motionFlags = 2;
  picture->terrainHeight = position[1];
  picture->velocityY = 0x20000;
  picture->gravity = 0x4000;
  buffer = Runtime_AllocateHeapBlock(68, 0x608);
  Func_08038248(item);
  tile = VramCache_Load(visual->tileSlot, 128, buffer + 0x400);
  visual->tile = tile;
  Runtime_ReleaseHeapBlock(68);
  picture->update = Marker_MoveLeaderAwayFromEmitter;
  if (work->revealActive != 0) {
    struct ObjectRuntime *marker = Ram_HeapSlots->scriptObjects;
    s32 j;

    for (j = 0; j <= 63; j++, marker++) {
      if (marker->script != NULL && marker->script == (s32 *)Data_080f01b8 &&
          marker->x >> 20 == picture->x >> 20 &&
          marker->z >> 20 == picture->z >> 20) {
        Object_Destroy(marker);
        break;
      }
    }
  }
  return picture;
}
