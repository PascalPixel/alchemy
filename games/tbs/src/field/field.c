// The field object: the code that draws and moves everything in towns, dungeons and the world map.
// It animates sprites, keeps the 64 actors and runs their scripts, moves the player by the keys,
// loads, draws and scrolls the maps, and answers what the ground is like under a point.
// field_internal.h holds what the parts share; the parts under field/ are included here in link
// order, one group to a part.
//
// README.md in this folder gives the words, prefixes and spellings the parts use. Where to start:
//
// - sprites: fdSpriteComposePicture and fdRenderPlaceProjectedSprite (field_sprite_draw.c),
//   fdSpriteCreate and fdSpriteStartAnimation (field_sprite_animation.c);
// - actors: fdActorCreate and fdActorSystemInit (field_actors.c), fdActorUpdateAllOnField and
//   fdActorSetMoveTarget (field_actor_motion.c);
// - actor scripts: field_script_commands.c, and the SC_ commands of actor_script.h;
// - the player: fdActorUpdatePlayerControl (field_player_control.c);
// - maps: fdMapGridLoadMap and fdMapGridUpdateLayerScroll (field_map_grid.c),
//   fdWorldMapInitPerspective and fdWorldMapUpdateView (field_world_map.c);
// - the ground: fdMapGridGetTerrainHeight, fdMapGridCheckCollision and fdWorldMapGetCollision
//   (field_terrain.c).

#include "field_internal.h"

// The parts, in ROM order (src/README.md, "Objects and parts").
#include "field_decompress.c"
#include "field_sprite_draw.c"
#include "field_sprite_animation.c"
#include "field_actors.c"
#include "field_actor_camera.c"
#include "field_actor_motion.c"
#include "field_script_commands.c"
#include "field_player_control.c"
#include "field_map_grid.c"
#include "field_world_map.c"
#include "field_tile_animation.c"
#include "field_terrain.c"
#include "field_view.c"
#include "field_sprite_viewer.c"
