#include "gabomba_statue_chamber.h"
#include "event_inline.h"

extern struct GabombaStatueChamberObject *gData_02002058;
void *HeapSlot_AllocFar(s32 slot, s32 size);
void HeapSlot_Release(s32 slot);
void Func_02000ef0();

void Func_02000f00(void) {
  struct GabombaStatueChamberObject *leader =
      ((struct GabombaStatueChamberObject *)Actor_GetById(8));
  s32 x = leader->x;
  s32 z = leader->z;
  /* FAKEMATCH(temporary): a local holding the 0 for the three byte stores,
   * set before the null test, so it takes r7; as the constant 0 in each store,
   * 20 instructions differ. */
  s32 zero = 0;
  struct GabombaStatueChamberObject *object;

  if ((gData_02002058 = object =
           ((struct GabombaStatueChamberObject *)Import_ActorCreate(
               489, x, 0x120000, z))) != NULL) {
    struct FieldSprite *sprite = object->sprite;
    u8 *block;

    sprite->fullColor = 0;
    sprite->palette = 0;
    sprite->priority = 2;
    object->motionFlags = zero;
    object->field5c = 1;
    object->update = Func_02000ef0;
    sprite->nextFrame = zero;
    sprite->unk1b[0] = zero;
    block = HeapSlot_AllocFar(68, 0x608);
    Icon_DrawItem(230);
    VramCache_Load(sprite->vramBlock, 128, block + 0x400);
    HeapSlot_Release(68);
  }
}

void Func_02000f88(void) {
  struct GabombaStatueChamberObject *leader =
      ((struct GabombaStatueChamberObject *)Actor_GetById(8));
  s32 x = leader->x;
  s32 z = leader->z;
  /* FAKEMATCH(temporary): a local holding the 0 for the three byte stores,
   * set before the null test, so it takes r7; as the constant 0 in each store,
   * 20 instructions differ. */
  s32 zero = 0;
  struct GabombaStatueChamberObject *object;

  if ((gData_02002058 = object =
           ((struct GabombaStatueChamberObject *)Import_ActorCreate(
               489, x, 0x120000, z))) != NULL) {
    struct FieldSprite *sprite = object->sprite;
    u8 *block;

    sprite->fullColor = 0;
    sprite->palette = 0;
    sprite->priority = 2;
    object->motionFlags = zero;
    object->field5c = 1;
    object->update = Func_02000ef0;
    sprite->nextFrame = zero;
    sprite->unk1b[0] = zero;
    block = HeapSlot_AllocFar(68, 0x608);
    Icon_DrawItem(242);
    VramCache_Load(sprite->vramBlock, 128, block + 0x400);
    HeapSlot_Release(68);
  }
}
