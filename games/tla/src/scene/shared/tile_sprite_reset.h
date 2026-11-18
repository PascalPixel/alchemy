#include "field/perspective.h"

/// An actor script's call: turns the actor's sprite to the yaw of the World
/// Map's view (the map grid's heap slot holds the WorldMapView there), clears
/// its sprite flags and goes on with the script.
s32 TileSprite_Reset(struct Actor *actor) {
  struct WorldMapView *view = (struct WorldMapView *)Ram_HeapSlots->mapGrid;
  struct FieldSprite *sprite = actor->sprite;

  sprite->rotation = view->yaw;
  sprite->flags = 0;
  return 1;
}
