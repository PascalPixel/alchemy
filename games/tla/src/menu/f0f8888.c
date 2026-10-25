#include "m7_interfaces.h"

/// Turns an icon's render output on and copies its screen position into its
/// OAM attributes, with no affine matrix.
void UiIcon_PrepareObject(struct RenderOutput *object) {
  if (object != NULL) {
    object->active = 1;
    object->oam.x = object->x;
    object->oam.y = object->y;
    object->oam.matrix = 0;
    object->oam.affineMode = 0;
  }
}
