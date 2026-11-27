/* @file */
/* The field object, the field module's C as The Broken Seal's field.c is: */
/* resource metadata and LZ decompression, composing sprite pictures and */
/* placing projected sprites, the resource slots (sprite animations, actors and
 */
/* their motion and scripts, the player's control, the map grid and layers and
 */
/* the World Map), the tile animation, the view (screen shake and the ground */
/* plane) and the debug sprite viewer. field_internal.h holds what the parts */
/* share; the parts under field/ are included here in the order their code has
 */
/* in the ROM. Camelot's own files are not recoverable: the ROM shows one */
/* object, and these parts are ours, cut where The Broken Seal's are. */

/* The parts, in ROM order (The Broken Seal's src/README.md, "Objects and */
/* parts"). */
#include "field_decompress.c"
#include "field_sprite_draw.c"
#include "field_sprite_animation.c"
#include "field_actors.c"
#include "field_actor_motion.c"
#include "field_script_commands.c"
#include "field_player_control.c"
#include "field_pad_movement.c"
#include "field_move_by_keys.c"
#include "field_map_grid.c"
#include "field_world_map.c"
#include "field_tile_animation.c"
#include "field_view.c"

#include "field_sprite_viewer.c"
