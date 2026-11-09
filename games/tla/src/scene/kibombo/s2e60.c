#include "kibombo.h"
#include "scene_tables.h"

void *HeapSlot_AllocFar(s32 slot, s32 size);
void HeapSlot_Release(s32 slot);
void Icon_DrawItem(s32 resource);
void VramCache_Load(s32 handle, s32 size, void *source);

void Func_02002de0(void) {
  struct KibomboObject **slot = &gData_02007950;
  /* FAKEMATCH: the native keeps one zero in r7 for the three byte stores below;
   * plain zeros are built in r2 where they are stored (differ 30). */
  s32 zero = 0;
  struct KibomboObject *object =
      Import_ActorCreate(0x1e9, 0x1090000, 0x240000, 0x1580000);

  *slot = object;
  if (object != 0) {
    struct FieldSprite *sprite = object->sprite;
    u8 *buffer;

    sprite->fullColor = 0;
    sprite->palette = 0;
    sprite->priority = 1;
    object->motionFlags = zero;
    object->field5c = 1;
    sprite->nextFrame = zero;
    sprite->unk1b[0] = zero;
    buffer = HeapSlot_AllocFar(68, 0x608);
    Icon_DrawItem(242);
    VramCache_Load(sprite->vramBlock, 128, buffer + 0x400);
    HeapSlot_Release(68);
  }
}

void Func_02002e60(void) {
  struct KibomboObject *leader = Actor_GetById(gMainRoutineWork.currentOwner);


  if (leader->y > 0x100000) {
    Actor_SetHorizontalPositionWithTerrain(29, 0, 0);
    Actor_SetSpritePriority(30, 2);
  } else {
    Kibombo_ObjSetPos(Actor_GetById(29), PIXELS(120), 0, PIXELS(168));
    Actor_SetSpritePriority(30, 3);
  }
}

void Func_02002eb0(void) {
  if (gData_02007950 != 0) {
    struct KibomboObject *object = Actor_GetById(27);

    Actor_SetPositionAndResetMotion(gData_02007950, object->x,
                                    object->y + PIXELS(12), object->z);
  }
}
