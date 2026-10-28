#include "trial_road.h"
#include "macros.h"

#include "scene_call.h"
#include "text/msg_ids.h"

struct EventWork15a {
  u8 unk000[0x15a];
  u16 value;
};

void Func_02005880(void);
void Func_02005888(void);
void Func_020059e8(s32 a);

void Func_020005b4(s32 id) {
  s32 flag;

  if (id == 9 || id == 11)
    flag = 0x285;
  else
    flag = 0x286;
  if (!GameFlag_Test(flag)) {
    Func_02005880();
    Func_020059e8(0);
    UiText_ShowPositionedMessageAndWait((s32)MsgTrialRoadChestBlocked, 1);
    Func_02005888();
  } else {
    UiText_ShowPositionedMessageAndWait((s32)MsgTrialRoadPlaceItemPrompt, 13);
    if (Event_AskYesNoWithActorAnimation(gAdventure.currentOwner, 1) == 0)
      ((struct EventWork15a *)Ram_HeapSlots->eventWork)->value = 1;
  }
}

void Func_02000628(void) {}

void Func_0200062c(s32 id) {
  struct TrialRoadMapWork *map =
      (struct TrialRoadMapWork *)Ram_HeapSlots->mapWork;
  struct ObjectRuntime *object = Object_GetById(id);

  if (object != NULL) {
    u8 *cells = map->layers[0].cells;
    u8 *cell = cells + (((object->x >> 20) + ((object->z >> 20) << 7)) << 2);
    s32 bit =
        128; /* FAKEMATCH: ⚓️ builds the bit in the register it ORs into */

    cell[3] = bit | cell[3];
  }
}

void Func_02000660(s32 id) {
  struct TrialRoadMapWork *map =
      (struct TrialRoadMapWork *)Ram_HeapSlots->mapWork;
  struct ObjectRuntime *object = Object_GetById(id);

  if (object != NULL) {
    u8 *cells = map->layers[0].cells;
    u8 *cell = cells + (((object->x >> 20) + ((object->z >> 20) << 7)) << 2);

    cell[3] &= 0x7f;
  }
}

s32 Func_02000694(struct ObjectRuntime *object) {
  struct ObjectRuntime *owner = object->watchedObject;

  SET_POSITION(object, owner->x, owner->y + (OBJECT_BYTE(object, 99) << 16),
               owner->z);
  object->terrainHeight = owner->terrainHeight;
  return 1;
}

extern const u8 gData_02005a7c[];

void Func_020006b4(s32 id) {
  struct ObjectRuntime *owner = Object_GetById(id);
  struct ObjectRuntime *object;

  if (owner != NULL) {
    object = Object_Create(489, owner->x, owner->y, owner->z);
    if (object != NULL) {
      ObjectDispatch_SetSingleChildFlags(object, 0);
      Func_020057d0(object, gData_02005a7c);
      object->watchedObject = owner;
      /* FAKEMATCH: stores the owner link before it copies the object for the
       * priority flags (differ 2 without it; an explicit flag pointer is differ
       * 27). */
      do {
      } while (0);
      object->priorityFlags |= 0x80;
      object->motionFlags = 0;
      object->collisionFlags = 0;
      OBJECT_BYTE(object, 99) = 16;
      owner->watchedObject = object;
    }
  }
}
