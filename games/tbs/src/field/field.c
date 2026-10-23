/// @file
/// The field object: the engine of the field and the world map. It draws and
/// animates sprites, keeps the 64 field objects and runs their scripts, moves
/// the player by the keys, loads, draws and scrolls the maps, and answers what
/// the ground is like under a point. field_internal.h holds what the parts
/// share; the parts under field/ are included here in the order their code has
/// in the ROM. Camelot's own files are not recoverable: the ROM shows one
/// object, and these parts are ours, cut where one family ends and the next
/// begins.
///
/// README.md in this folder gives the words, prefixes and spellings the parts
/// use. Where to start:
///
///   Sprites         sprite_draw.c: Sprite_ComposeAnimationFrame,
///                   Render_PlaceProjectedSprite; animation.c:
///                   ResourceObject_Create, AnimationObject_SelectAnimation
///   Objects         objects.c: FieldObject_Create, ObjectSystem_Initialize;
///                   object_motion.c: Object_UpdateAllThumb,
///                   Object_SetMoveTarget
///   Object scripts  script_commands.c (the SC_ commands of objcmd.h);
///                   player_control.c: FieldObject_UpdatePlayerControl
///   Maps            map_layers.c: MapGrid_LoadLayout,
///                   MapGrid_UpdateLayerScroll; world_map.c:
///                   WorldMap_InitializePerspective, WorldMap_UpdateView
///   Ground          terrain.c: MapGrid_GetTerrainHeight,
///   MapGrid_CheckObjectStep,
///                   GetWorldMapCollision

#include "field_internal.h"

// The field object's code, in ROM order. Each part is compiled here, as part
// of this one object, never on its own: the order of these includes is the
// order of the code in the ROM.
#include "field/decompress.c"
#include "field/sprite_draw.c"
#include "field/animation.c"
#include "field/objects.c"
#include "field/object_camera.c"
#include "field/object_motion.c"
#include "field/script_commands.c"
#include "field/player_control.c"
#include "field/map_grid.c"
#include "field/world_map.c"
#include "field/tile_animation.c"
#include "field/terrain.c"
#include "field/view.c"
#include "field/sprite_viewer.c"
