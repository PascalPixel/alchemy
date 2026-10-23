#include "orbit.h"
#include "flags.h"

// Crossbone Isle, Kalay, Kolima Forest (MapId_KolimaForestGlade), Lunpa,
// McCoy's palace garden, Mogall Forest and Vale: a Nut floating where it lies,
// until it is taken.

/// The Nut's callback, each frame: it sways up to two pixels either side of its
/// spot and dips up to two pixels below it, at a random pace.
static s32 Map_UpdateFloatingNut(s32 *p) {
  s16 *q = (s16 *)p[20];
  s32 a, b;
  s32 d = Trig_Sin(p[12]) * 2;
  if (d > 0)
    d = -d;
  p[2] = p[14] + Trig_Cos(p[12]) * 2;
  p[3] = p[15] + d;
  q[15] = Trig_Cos(p[12] + 0x8000) / 8;
  a = Engine_RandomNext();
  b = Engine_RandomNext();
  p[12] = p[12] + ((((u32)a << 9) >> 16) + (((u32)b << 9) >> 16)) + 0x400;
  return 0;
}

/// Draws object event id as the Nut's icon and starts it floating, 32 pixels up
/// unless the party returns to where it was.
void Map_FloatNut(s32 id) {
  OrbitingMapObject *objectEvent;
  OrbitingMapObjectSprite *sprite;
  u8 *transfer;
  s32 zero;

  objectEvent = (OrbitingMapObject *)ObjectEvent_GetById(id);
  sprite = objectEvent->sprite;
  sprite->priority = 1;
  sprite->fullColor = 0;
  sprite->paletteNumber = 0;

  zero = 0;
  sprite->state = zero;
  ObjectEvent_SetSpriteFlags((struct ObjectEvent *)objectEvent, zero);
  objectEvent->active = zero;
  objectEvent->mode = zero;

  if (GameFlag_Test(FLAG_KEEP_PARTY_POSITION) == 0)
    objectEvent->y += 0x200000;

  objectEvent->priorityFlags &= 0xfe;
  objectEvent->visible = 1;

  transfer = AllocHeapSlot(17, 0x608);
  Icon_DrawItem(ITEM_NUT);
  transfer += 0x400;
  VramCache_Load(sprite->palette, 128, transfer);
  FreeHeapSlot(17);

  objectEvent->orbitCenterX = objectEvent->x;
  objectEvent->orbitAngle = zero;
  objectEvent->orbitCenterY = objectEvent->y;
  objectEvent->active = 1;
  objectEvent->callback = (u32)Map_UpdateFloatingNut;
  objectEvent->state = zero;
}
