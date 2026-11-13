#include "orbit.h"
#include "flags.h"

// Crossbone Isle, Kalay, Kolima Forest (SceneId_KolimaForestGlade), Lunpa,
// McCoy's palace garden, Mogall Forest and Vale: a Nut floating where it lies,
// until it is taken.

/// The Nut's callback, each frame: it sways up to two pixels either side of its
/// spot and dips up to two pixels below it, at a random pace.
static s32 Scene_UpdateFloatingNut(OrbitingMapActor *nut) {
  struct Sprite *sprite = (struct Sprite *)nut->sprite;
  s32 first, second;
  s32 dip = Trig_Sin(nut->orbitAngle) * 2;

  if (dip > 0)
    dip = -dip;
  nut->x = nut->orbitCenterX + Trig_Cos(nut->orbitAngle) * 2;
  nut->y = nut->orbitCenterY + dip;
  sprite->rotation = Trig_Cos(nut->orbitAngle + TRIG_TURN / 2) / 8;
  // The pace: a sixty-fourth of a turn and two random steps under 1/128 each.
  first = Random_Next();
  second = Random_Next();
  nut->orbitAngle = nut->orbitAngle +
                    ((((u32)first << 9) >> 16) + (((u32)second << 9) >> 16)) +
                    TRIG_TURN / 64;
  return 0;
}

/// Draws actor id as the Nut's icon and starts it floating, 32 pixels up
/// unless the party returns to where it was.
void Scene_FloatNut(s32 id) {
  OrbitingMapActor *actor;
  OrbitingMapActorSprite *sprite;
  u8 *transfer;

  actor = (OrbitingMapActor *)Actor_GetById(id);
  sprite = actor->sprite;
  sprite->priority = 1;
  sprite->fullColor = 0;
  sprite->paletteNumber = 0;

  sprite->state = 0;
  Actor_SetSpriteFlags((struct Actor *)actor, 0);
  actor->active = 0;
  actor->mode = 0;

  if (GameFlag_Get(FLAG_KEEP_PARTY_POSITION) == 0)
    actor->y += px(32);

  actor->priorityFlags &= ~ACTOR_PRIORITY_AUTOMATIC;
  actor->visible = 1;

  transfer = HeapSlot_Alloc(HEAP_SLOT_GLYPH_TRANSFER, sizeof(GlyphTransfer));
  Icon_DrawItem(ITEM_NUT);
  transfer += 1024;
  VramCache_Load(sprite->palette, 128, transfer);
  HeapSlot_Release(HEAP_SLOT_GLYPH_TRANSFER);

  actor->orbitCenterX = actor->x;
  actor->orbitAngle = 0;
  actor->orbitCenterY = actor->y;
  actor->active = 1;
  actor->callback = (u32)Scene_UpdateFloatingNut;
  actor->state = 0;
}
