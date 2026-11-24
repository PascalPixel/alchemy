/// @file
/// The field_psynergy object: the field Psynergy that spread effects around
/// their target, then in ROM order the tile copies, the field Psynergy The Lost
/// Age adds, from Cyclone to Hover (each part named for the Psynergy it runs,
/// its kind in FieldPsynergy_Run), the summon tablets, the actors' cells, the
/// step vectors and field particles, and the world and map screen and its
/// image. field_psynergy_internal.h holds the includes every part shares; the
/// parts beside it, field_psynergy_<part>.c, are included here in ROM order.
/// The ROM shows one object; the parts are ours.

#include "field_psynergy_internal.h"

// The parts, in ROM order.
#include "field_psynergy_radial_effects.c"

#undef Actor_CreateFar
#undef EffectSprite_DrawAtWorldPosition

#include "field_psynergy_tile_copies.c"

#include "field_psynergy_cyclone.c"
#include "field_psynergy_blaze.c"
#include "field_psynergy_sand.c"
#include "field_psynergy_burst.c"
#include "field_psynergy_pound.c"
#include "field_psynergy_growing_sprite.c"
#include "field_psynergy_scoop.c"
#include "field_psynergy_teleport.c"
#include "field_psynergy_parch.c"
#include "field_psynergy_ring.c"
#include "field_psynergy_tremor.c"
#include "field_psynergy_grind.c"
#include "field_psynergy_hover.c"
#include "field_psynergy_summon_tablet.c"
#include "field_psynergy_shape_entries.c"

#undef EffectSprite_DrawAtWorldPosition
#undef Resource_GetMetadataRecordFarFar

#include "field_psynergy_actor_cells.c"
#include "field_psynergy_step_vector.c"

#undef BG_VRAM

#include "field_psynergy_map_screen.c"
#include "field_psynergy_map_screen_image.c"
