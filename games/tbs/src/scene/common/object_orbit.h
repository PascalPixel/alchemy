#include "orbit.h"
#include "flags.h"

// Crossbone Isle, Kalay, Kolima Forest (SceneId_KolimaForestGlade), Lunpa,
// McCoy's palace garden, Mogall Forest and Vale: a Nut floating where it lies,
// until it is taken.

/// The Nut's callback, each frame: it sways up to two pixels either side of its
/// spot and dips up to two pixels below it, at a random pace.
static s32 Scene_UpdateFloatingNut(s32 *p) {
  s16 *q = (s16 *)p[20];
  s32 a, b;
  s32 d = Trig_Sin(p[12]) * 2;
  if (d > 0)
    d = -d;
  p[2] = p[14] + Trig_Cos(p[12]) * 2;
  p[3] = p[15] + d;
  q[15] = Trig_Cos(p[12] + 0x8000) / 8;
  a = Import_RandomNext();
  b = Import_RandomNext();
  p[12] = p[12] + ((((u32)a << 9) >> 16) + (((u32)b << 9) >> 16)) + 0x400;
  return 0;
}

/// Draws actor id as the Nut's icon and starts it floating, 32 pixels up
/// unless the party returns to where it was.
void Scene_FloatNut(s32 id) {
  OrbitingMapActor *actor;
  OrbitingMapActorSprite *sprite;
  u8 *transfer;
  s32 zero;

  actor = (OrbitingMapActor *)Actor_GetById(id);
  sprite = actor->sprite;
  sprite->priority = 1;
  sprite->fullColor = 0;
  sprite->paletteNumber = 0;

  zero = 0;
  sprite->state = zero;
  Actor_SetSpriteFlags((struct Actor *)actor, zero);
  actor->active = zero;
  actor->mode = zero;

  if (GameFlag_Get(FLAG_KEEP_PARTY_POSITION) == 0)
    actor->y += 0x200000;

  actor->priorityFlags &= ~ACTOR_PRIORITY_AUTOMATIC;
  actor->visible = 1;

  transfer = AllocHeapSlot(HEAP_SLOT_GLYPH_TRANSFER, 0x608);
  Icon_DrawItem(ITEM_NUT);
  transfer += 0x400;
  VramCache_Load(sprite->palette, 128, transfer);
  FreeHeapSlot(HEAP_SLOT_GLYPH_TRANSFER);

  actor->orbitCenterX = actor->x;
  actor->orbitAngle = zero;
  actor->orbitCenterY = actor->y;
  actor->active = 1;
  actor->callback = (u32)Scene_UpdateFloatingNut;
  actor->state = zero;
}
