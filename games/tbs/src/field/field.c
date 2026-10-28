/// @file
/// The field object: the code that draws and moves everything in towns,
/// dungeons and the world map. It animates sprites, keeps the 64 actors and
/// runs their scripts, moves the player by the keys, loads, draws and scrolls
/// the maps, and answers what the ground is like under a point.
/// field_internal.h holds what the parts share; the parts under field/ are
/// included here in the order their code has in the ROM. Camelot's own files
/// are not recoverable: the ROM shows one object, and these parts are ours, cut
/// where one family ends and the next begins.
///
/// README.md in this folder gives the words, prefixes and spellings the parts
/// use. Where to start:
///
///   Sprites         sprite_draw.c: Sprite_ComposePicture,
///                   Render_PlaceProjectedSprite; animation.c:
///                   Sprite_Create, Sprite_StartAnim
///   Actors          actors.c: Actor_Create, ActorSystem_Initialize;
///                   actor_motion.c: Actor_UpdateAllThumb,
///                   Actor_SetMoveTarget
///   Actor scripts   script_commands.c (the SC_ commands of actor_script.h);
///                   player_control.c: Actor_UpdatePlayerControl
///   Maps            map_layers.c: MapGrid_LoadMap,
///                   MapGrid_UpdateLayerScroll; world_map.c:
///                   WorldMap_InitializePerspective, WorldMap_UpdateView
///   Ground          terrain.c: MapGrid_GetTerrainHeight,
///                   MapGrid_CheckCollision,
///                   WorldMap_GetCollision

#include "field_internal.h"

// The parts, in ROM order (src/README.md, "Objects and parts").
#include "field/decompress.c"
#include "field/sprite_draw.c"
#include "field/animation.c"
#include "field/actors.c"
#include "field/actor_camera.c"
#include "field/actor_motion.c"
#include "field/script_commands.c"
#include "field/player_control.c"
#include "field/map_grid.c"
#include "field/world_map.c"
#include "field/tile_animation.c"
#include "field/terrain.c"
#include "field/view.c"
#include "field/sprite_viewer.c"
