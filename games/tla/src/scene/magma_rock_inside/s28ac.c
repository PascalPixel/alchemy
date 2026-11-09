#include "../../../../games/tla/src/scene/magma_rock_inside/local.h"
#include "io_reg.h"

struct FieldActor;

void Func_02004d40(void);
void Func_02004d48(void);
void Func_02004e20(void);
struct FieldActor *OverlayObject_PrepareObject(s32 x, s32 y, s32 z, s32 type);
void Func_02004cb0(struct FieldActor *actor);
s32 Func_02000694(u32 layer, s32 x, s32 y, s32 value);

void Func_020028ac(void) {
  struct Actor *object;
  struct FieldActor *actor;
  struct Actor *other;
  s32 i;

  object = Actor_Get(((s8 *)Ram_HeapSlots->eventWork)[0xd30]);
  Actor_SetSpriteColor(object, 0);
  Event_PlaySound(136);
  i = 0;
  do {
    object->scaleY = object->scaleX += 0x1000;
    System_Wait(5);
    i++;
  } while (i != 16);
  Event_SetFlag(0x305);
  other = Actor_Get(9);
  Func_02004d40();
  FieldEvent_ResolvePendingActions(0);
  MapGrid_SetShake(0x20000, 0x10000, 0x10000);
  Event_PlaySound(141);
  FieldEvent_Wait(30);
  FieldCamera_MoveTo(0x780000, -1, 0x1580000, 1);
  Func_02004e20();
  FieldEvent_Wait(30);
  Event_PlaySound(0x22d);
  {
    register u16 alpha __asm__("r3") =
        0x1000; /* FAKEMATCH: the blend value pinned to r3, as the
                   native stores it (differ 16 as a plain store, 30 without the
                   pins). */

    /* FAKEMATCH: with the pin above. */
    REG_BLDALPHA = alpha;
  }
  REG_BLDCNT = 0x3f10;
  {
    struct FieldActor *prepared = OverlayObject_PrepareObject(
        other->x, other->terrainHeight, other->z + 0x300000, 312);

    other->motionFlags = 0;
    actor = prepared;
  }
  i = 0;
  do {
    other->y -= 0x10000;
    FieldEvent_Wait(5);
    i++;
  } while (i != 32);
  Event_PlaySound(0x22a);
  Func_02004c98(other, 4);
  Func_02004cb0(actor);
  FieldEvent_Wait(1);
  {
    register u16 off __asm__("r3") =
        0; /* FAKEMATCH: the zero pinned to r3 (differ 4 as a plain store). */

    /* FAKEMATCH: with the pin above. */
    REG_BLDCNT = off;
  }
  Func_02000694(0, 7, 21, 128);
  Func_02004d48();
}
