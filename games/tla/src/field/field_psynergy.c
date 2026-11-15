/// @file
/// The field_psynergy object: the field Psynergy that spread effects around
/// their target, then in ROM order the tile copies, the field Psynergy The Lost
/// Age adds, from Cyclone to Hover (each part named for the Psynergy it runs,
/// its kind in FieldPsynergy_Run), the actors' cells, the step vectors and
/// field particles, and the world and World Map pictures.
/// field_psynergy_internal.h holds the includes every part shares; the parts
/// under field_psynergy/ are included here in ROM order. The ROM shows one
/// object; the parts are ours.

#include "field_psynergy_internal.h"

// The parts, in ROM order.
#include "field_psynergy/radial_effects.c"

#undef Actor_CreateFar
#undef EffectSprite_DrawAtWorldPosition

#include "field_psynergy/tile_copies.c"

#include "field_psynergy/cyclone.c"
#include "field_psynergy/blaze.c"
#include "field_psynergy/sand.c"
#include "field_psynergy/burst.c"
#include "field_psynergy/pound.c"
#include "field_psynergy/growing_sprite.c"
#include "field_psynergy/scoop.c"
#include "field_psynergy/teleport.c"
#include "field_psynergy/parch.c"
#include "field_psynergy/unnamed_28.c"
#include "field_psynergy/tremor.c"
#include "field_psynergy/grind.c"
#include "field_psynergy/hover.c"
#include "field_psynergy/summon_tablet.c"
#include "field_psynergy/collision_tags.c"

#undef Camera_WorldToScreen
#undef EffectSprite_DrawAtWorldPosition
#undef Resource_GetMetadataRecordFarFar

#include "field_psynergy/actor_cells.c"
#include "field_psynergy/step_vector.c"

#undef BG_VRAM

#include "field_psynergy/world_picture.c"
#include "field_psynergy/world_map_picture.c"
