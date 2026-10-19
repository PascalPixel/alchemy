#include "islets.h"

void Func_020020ec(s32 x, s32 z, s32 layers, s32 value) {
  struct MapWork *map = Ram_HeapSlots->mapWork;
  struct MapCell *cells0 = map->layers[0].cells;
  struct MapCell *cells2 = map->layers[2].cells;
  s32 index = x / 0x100000 + (z / 0x100000) * 128;

  if (layers & 1) {
    cells0 += index;
    cells0->collisionCode = value;
  }
  if (layers & 2) {
    cells2 += index;
    cells2->collisionCode = value;
  }
}

// Records objects 12 and 13's positions; with them on cells (7, 29) and
// (7, 28), clears layer 2's collision under object 12, and with them on
// (12, 25) and (12, 26), under object 13.
void Func_02002144(void) {
  struct ObjectRuntime *object;
  s32 *x0;
  s32 *z0;
  s32 x1;
  s32 z1;

  object = Object_GetById(12);
  x0 = &gData_0200519c;
  z0 = &gData_02005194;
  *x0 = object->x;
  *z0 = object->z;
  object = Object_GetById(13);
  gData_02005198 = x1 = object->x;
  {
    s32 *p = &gData_02005190;
    /* FAKEMATCH(pin): object 13's z in r2, as the native keeps it for its
     * test; as a plain local, 31 of 58 instructions differ in tla-en. */
    register s32 z asm("r2");

    /* FAKEMATCH(barrier): clobbering r2 loads the address before z; as the
     * plain store, 2 of 58 instructions differ in tla-en. */
    __asm__("" : "+l"(p) : : "r2");
    z = object->z;
    *p = z;
    z1 = z;
  }
  if (*x0 >> 20 == 7 && *z0 >> 20 == 29 && x1 >> 20 == 7 && z1 >> 20 == 28)
    Func_020020ec(*x0, *z0, 2, 0);
  if (gData_0200519c >> 20 == 12 && gData_02005194 >> 20 == 25 &&
      gData_02005198 >> 20 == 12 && gData_02005190 >> 20 == 26)
    Func_020020ec(gData_02005198, gData_02005190, 2, 0);
}
