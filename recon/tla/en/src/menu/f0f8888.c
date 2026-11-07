#include "m7_interfaces.h"

/// Turns an icon's window sprite on and copies its screen position into its
/// OAM attributes, with no affine matrix.
void Icon_PrepareActor(struct WindowSprite *object) {
  if (object != NULL) {
    object->active = 1;
    object->sprite.oam.x = object->x;
    object->sprite.oam.y = object->y;
    object->sprite.oam.matrix = 0;
    object->sprite.oam.affineMode = 0;
  }
}
