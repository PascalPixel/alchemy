	.include "games/common/include/game/ed_asm.h"
	.include "games/tla/src/map/map.inc"
	.section .rodata
	.global Field_Map141
Field_Map141:
	.ifeq EDITION_INTERNATIONAL
	map_header 0, 0, 90, 100, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 100, 16, 16, 0, 0, 0xff, 0xff
	map_layer 100, 0, 16, 16, 2, 2, 0xf, 0xf
	map_block Field_Map141, .LField_Map141_METATILES
	map_block Field_Map141, .LField_Map141_SHAPES
	map_block Field_Map141, .LField_Map141_GRID
	map_block Field_Map141, .LField_Map141_SHAPE_GRID
	map_block Field_Map141, .LField_Map141_ANIMATION
	map_block Field_Map141, .LField_Map141_BLEND
	map_block Field_Map141, .LField_Map141_SCENE
.LField_Map141_METATILES:
	.incbin "games/tla/src/map/map_141/metatiles.delta1.lz"
	.balign 4, 0
.LField_Map141_SHAPES:
	.incbin "games/tla/src/map/map_141/shapes.table.lz"
	.balign 4, 0
.LField_Map141_GRID:
	.incbin "games/tla/src/map/map_141/grid_ja.parts.lz"
	.balign 4, 0
.LField_Map141_SHAPE_GRID:
	.incbin "games/tla/src/map/map_141/shape_grid_ja.plane.lz"
	.balign 4, 0
.LField_Map141_ANIMATION:
	.incbin "games/tla/src/map/map_141/animation.script.lz"
	.balign 4, 0
.LField_Map141_BLEND:
	.incbin "games/tla/src/map/map_060/blend.script.lz"
	.balign 4, 0
.LField_Map141_SCENE:
	.incbin "games/tla/src/map/map_141/scene_ja.parts.lz"
	.else
	map_header 0, 0, 90, 100, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 100, 16, 16, 0, 0, 0xff, 0xff
	map_layer 100, 0, 16, 16, 2, 2, 0xf, 0xf
	map_block Field_Map141, .LField_Map141_METATILES
	map_block Field_Map141, .LField_Map141_SHAPES
	map_block Field_Map141, .LField_Map141_GRID
	map_block Field_Map141, .LField_Map141_SHAPE_GRID
	map_block Field_Map141, .LField_Map141_ANIMATION
	map_block Field_Map141, .LField_Map141_BLEND
	map_block Field_Map141, .LField_Map141_SCENE
.LField_Map141_METATILES:
	.incbin "games/tla/src/map/map_141/metatiles.delta1.lz"
	.balign 4, 0
.LField_Map141_SHAPES:
	.incbin "games/tla/src/map/map_141/shapes.table.lz"
	.balign 4, 0
.LField_Map141_GRID:
	.incbin "games/tla/src/map/map_141/grid.parts.lz"
	.balign 4, 0
.LField_Map141_SHAPE_GRID:
	.incbin "games/tla/src/map/map_141/shape_grid.plane.lz"
	.balign 4, 0
.LField_Map141_ANIMATION:
	.incbin "games/tla/src/map/map_141/animation.script.lz"
	.balign 4, 0
.LField_Map141_BLEND:
	.incbin "games/tla/src/map/map_060/blend.script.lz"
	.balign 4, 0
.LField_Map141_SCENE:
	.incbin "games/tla/src/map/map_141/scene.parts.lz"
	.endif
