/// @file
/// The field object, the field bank's C as The Broken Seal's field.c is:
/// resource metadata and LZ decompression, composing sprite pictures and
/// placing projected sprites, the resource slots (sprite animations, actors and
/// their motion and scripts, the player's control, the map grid and layers and
/// the World Map), the tile animation, the view (screen shake and the ground
/// plane) and the debug sprite viewer. field_internal.h holds what the parts
/// share; the parts under field/ are included here in the order their code has
/// in the ROM. Camelot's own files are not recoverable: the ROM shows one
/// object, and these parts are ours, cut where The Broken Seal's are.

#include "field_internal.h"

// The parts, in ROM order (The Broken Seal's src/README.md, "Objects and
// parts").
#include "field/decompress.c"
#include "field/sprite_draw.c"

#undef Render_ApplyProjectedPlacement

#include "field/sprite_animation.c"
#include "field/actors.c"
#include "field/actor_motion.c"
#include "field/script_commands.c"
#include "field/player_control.c"
#include "field/pad_movement.c"
#include "field/move_by_keys.c"
#include "field/map_grid.c"
#include "field/map_layers.c"
#include "field/world_map.c"

#undef InitializeAnimationObjects
#undef ResourceMetadata_Register
#undef Animation_ApplyChildArgument
#undef Animation_ApplyChildValue
#undef SpriteAnimation_Alloc
#undef Render_ApplyProjectedPlacementAtDepth

#include "field/tile_animation.c"

#undef gWorldMapWork

#include "field/view.c"

#include "field/sprite_viewer.c"
