#include "../../../../games/tla/src/scene/magma_rock_inside/local.h"
#include "io_reg.h"

struct FieldActor;

void Func_02004d40(void);
void Func_02004d48(void);
void Func_02004e20(void);
struct FieldActor *OverlayObject_PrepareObject(s32 x, s32 y, s32 z, s32 type);
void Func_02004cb0(struct FieldActor *actor);
s32 Func_02000694(u32 layer, s32 x, s32 y, s32 value);

void Func_020021c0(void) {
  u8 *work = Ram_HeapSlots->eventWork;
  struct Actor *object;
  struct FieldActor *actor;
  struct Actor *other;
  s32 i;

  object = Actor_GetById(((s8 *)work)[0xd30]);
  Actor_SetPartAttribute(object, 0);
  Sound_Play(136);
  i = 0;
  do {
    object->scaleY = object->scaleX += 0x1000;
    wait(5);
    i++;
  } while (i != 16);
  GameFlag_SetBit(((s8 *)work)[0xd30] + 0x2ee);
  if (GameFlag_Test(0x303) && GameFlag_Test(0x304)) {
    other = Actor_GetById(11);
    Func_02004d40();
    FieldEvent_ResolvePendingActions(0);
    MapGrid_SetShake(0x20000, 0x10000, 0x10000);
    Sound_Play(141);
    FieldEvent_Wait(30);
    FieldCamera_MoveTo(0x2680000, -1, 0x1280000, 1);
    Func_02004e20();
    FieldEvent_Wait(30);
    Sound_Play(0x22d);
    {
      /* FAKEMATCH(pin): the blend value pinned to r3, where the native stores
       * it from; as plain locals with neither pin, differ 37 (of M not
       * recorded). */
      register u16 alpha __asm__("r3") = 0x1000;

      /* FAKEMATCH(temporary): the store through the pinned local; as a plain
       * store of 0x1000, differ 20 (of M not recorded). */
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
    Sound_Play(0x22a);
    Func_02004c98(other, 4);
    Func_02004cb0(actor);
    FieldEvent_Wait(1);
    {
      /* FAKEMATCH(pin): the zero pinned to r3; with the local below, as a
       * plain store of 0, differ 4 (of M not recorded). */
      register u16 off __asm__("r3") = 0;

      /* FAKEMATCH(temporary): the store through the pinned local, there only
       * for the pin above; as a plain store of 0, measured with it. */
      REG_BLDCNT = off;
    }
    Func_02000694(0, 38, 18, 0);
    Func_02004d48();
  }
}
