	.include "games/tla/include/game/ed_asm.h"
	.include "games/tla/src/map/map.inc"
	.section .rodata
	.global Field_Map095
Field_Map095:
	.ifeq EDITION_INTERNATIONAL
	map_header 0, 0, 60, 60, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 60, 16, 16, 0, 0, 0xff, 0xff
	map_layer 60, 0, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map095, .LField_Map095_METATILES
	map_block Field_Map095, .LField_Map095_SHAPES
	map_block Field_Map095, .LField_Map095_GRID
	map_block Field_Map095, .LField_Map095_SHAPE_GRID
	map_block Field_Map095, .LField_Map095_ANIMATION
	map_block Field_Map095, .LField_Map095_BLEND
	map_block Field_Map095, .LField_Map095_SCENE
.LField_Map095_METATILES:
	.incbin "games/tla/src/map/map_095/metatiles_ja.delta0.lz"
	.balign 4, 0
.LField_Map095_SHAPES:
	.incbin "games/tla/src/map/map_095/shapes.table.lz"
	.balign 4, 0
.LField_Map095_GRID:
	.incbin "games/tla/src/map/map_095/grid.parts.lz"
	.balign 4, 0
.LField_Map095_SHAPE_GRID:
	.incbin "games/tla/src/map/map_095/shape_grid.plane.lz"
	.balign 4, 0
.LField_Map095_ANIMATION:
	.incbin "games/tla/src/map/map_012/animation.script.lz"
	.balign 4, 0
.LField_Map095_BLEND:
	.incbin "games/tla/src/map/map_001/blend.script.lz"
	.balign 4, 0
.LField_Map095_SCENE:
	.incbin "games/tla/src/map/map_095/scene_ja.parts.lz"
	.else
	map_header 0, 0, 60, 60, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 60, 16, 16, 0, 0, 0xff, 0xff
	map_layer 60, 0, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map095, .LField_Map095_METATILES
	map_block Field_Map095, .LField_Map095_SHAPES
	map_block Field_Map095, .LField_Map095_GRID
	map_block Field_Map095, .LField_Map095_SHAPE_GRID
	map_block Field_Map095, .LField_Map095_ANIMATION
	map_block Field_Map095, .LField_Map095_BLEND
	map_block Field_Map095, .LField_Map095_SCENE
.LField_Map095_METATILES:
	.incbin "games/tla/src/map/map_095/metatiles.delta0.lz"
	.balign 4, 0
.LField_Map095_SHAPES:
	.incbin "games/tla/src/map/map_095/shapes.table.lz"
	.balign 4, 0
.LField_Map095_GRID:
	.incbin "games/tla/src/map/map_095/grid.parts.lz"
	.balign 4, 0
.LField_Map095_SHAPE_GRID:
	.incbin "games/tla/src/map/map_095/shape_grid.plane.lz"
	.balign 4, 0
.LField_Map095_ANIMATION:
	.incbin "games/tla/src/map/map_012/animation.script.lz"
	.balign 4, 0
.LField_Map095_BLEND:
	.incbin "games/tla/src/map/map_001/blend.script.lz"
	.balign 4, 0
.LField_Map095_SCENE:
	.incbin "games/tla/src/map/map_095/scene.parts.lz"
	.endif
	.balign 4, 0
