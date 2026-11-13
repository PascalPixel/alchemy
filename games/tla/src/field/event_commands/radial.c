/// @file
/// The radial object: the field Psynergy that spread effects around their
/// target, then in ROM order the tile copies, the field Psynergy The Lost Age
/// adds, from Cyclone to Hover (each part named for the Psynergy it runs, its
/// kind in FieldPsynergy_Run), the actors' cells, the step vectors and field
/// particles, and the world and World Map pictures. radial_internal.h holds the
/// includes every part shares; the parts under radial/ are included here in
/// ROM order. The ROM shows one object; the parts are ours.

#include "radial_internal.h"

// The parts, in ROM order.
#include "radial/radial_effects.c"

#undef Actor_CreateFar
#undef EffectSprite_DrawAtWorldPosition

#include "radial/tile_copies.c"

#include "radial/cyclone.c"
#include "radial/blaze.c"
#include "radial/sand_sinking.c"
#include "radial/growing_particle.c"
#include "radial/sand.c"
#include "radial/burst.c"
#include "radial/sinking_object.c"
#include "radial/pound.c"
#include "radial/growing_sprite.c"
#include "radial/scoop.c"
#include "radial/unnamed_28.c"
#include "radial/tremor.c"
#include "radial/grind.c"
#include "radial/hover.c"
#include "radial/mote_trail.c"
#include "radial/collision_tags.c"

#undef BurstWork
#undef Camera_WorldToScreen
#undef EffectSprite_DrawAtWorldPosition
#undef ObjectAnimation
#undef Resource_GetMetadataRecordFarFar
#undef BattleEffect_DropObjectAtAnchor
#undef SparkParticle
#undef ObjectSprite
#undef Mote
#undef Actor_StartScriptFar

#include "radial/actor_cells.c"
#include "radial/step_vector.c"

#undef BG_VRAM

#include "radial/world_picture.c"
#include "radial/world_map_picture.c"
