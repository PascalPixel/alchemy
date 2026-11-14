/// @file
/// The field object: the code that draws and moves everything in towns,
/// dungeons and the world map. It animates sprites, keeps the 64 actors and
/// runs their scripts, moves the player by the keys, loads, draws and scrolls
/// the maps, and answers what the ground is like under a point.
/// field_internal.h holds what the parts share; the parts under field/ are
/// included here in link order, one family to a part.
///
/// README.md in this folder gives the words, prefixes and spellings the parts
/// use. Where to start:
///
///   Sprites         field_sprite_draw.c: Sprite_ComposePicture,
///                   Render_PlaceProjectedSprite; field_sprite_animation.c:
///                   Sprite_Create, Sprite_StartAnimation
///   Actors          field_actors.c: Actor_Create, ActorSystem_Initialize;
///                   field_actor_motion.c: Actor_UpdateAllThumb,
///                   Actor_SetMoveTarget
///   Actor scripts   field_script_commands.c (the SC_ commands of
///   actor_script.h);
///                   field_player_control.c: Actor_UpdatePlayerControl
///   Maps            field_map_grid.c: MapGrid_LoadMap,
///                   MapGrid_UpdateLayerScroll; field_world_map.c:
///                   WorldMap_InitializePerspective, WorldMap_UpdateView
///   Ground          field_terrain.c: MapGrid_GetTerrainHeight,
///                   MapGrid_CheckCollision,
///                   WorldMap_GetCollision

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
