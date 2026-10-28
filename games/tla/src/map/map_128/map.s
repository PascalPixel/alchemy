	.include "games/tla/include/game/ed_asm.h"
	.include "games/tla/src/map/map.inc"
	.section .rodata
	.global Field_Map128
Field_Map128:
	.ifeq EDITION_INTERNATIONAL
	map_header 0, 0, 120, 120, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 120, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 120, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map128, .LField_Map128_CELL_PICTURES
	map_block Field_Map128, .LField_Map128_SHAPES
	map_block Field_Map128, .LField_Map128_GRID
	map_block Field_Map128, .LField_Map128_SHAPE_GRID
	map_block Field_Map128, .LField_Map128_ANIMATION
	map_block Field_Map128
	map_block Field_Map128, .LField_Map128_SCENE
.LField_Map128_CELL_PICTURES:
	.incbin "games/tla/src/map/map_128/cell_pictures.delta1.lz"
	.balign 4, 0
.LField_Map128_SHAPES:
	.incbin "games/tla/src/map/map_128/shapes.table.lz"
	.balign 4, 0
.LField_Map128_GRID:
	.incbin "games/tla/src/map/map_128/grid_ja.parts.lz"
	.balign 4, 0
.LField_Map128_SHAPE_GRID:
	.incbin "games/tla/src/map/map_128/shape_grid.plane.lz"
	.balign 4, 0
.LField_Map128_ANIMATION:
	.incbin "games/tla/src/map/map_008/animation.script.lz"
	.balign 4, 0
.LField_Map128_SCENE:
	.incbin "games/tla/src/map/map_128/scene_ja.parts.lz"
	.else
	map_header 0, 0, 120, 120, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 120, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 120, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map128, .LField_Map128_CELL_PICTURES
	map_block Field_Map128, .LField_Map128_SHAPES
	map_block Field_Map128, .LField_Map128_GRID
	map_block Field_Map128, .LField_Map128_SHAPE_GRID
	map_block Field_Map128, .LField_Map128_ANIMATION
	map_block Field_Map128
	map_block Field_Map128, .LField_Map128_SCENE
.LField_Map128_CELL_PICTURES:
	.incbin "games/tla/src/map/map_128/cell_pictures.delta1.lz"
	.balign 4, 0
.LField_Map128_SHAPES:
	.incbin "games/tla/src/map/map_128/shapes.table.lz"
	.balign 4, 0
.LField_Map128_GRID:
	.incbin "games/tla/src/map/map_128/grid.parts.lz"
	.balign 4, 0
.LField_Map128_SHAPE_GRID:
	.incbin "games/tla/src/map/map_128/shape_grid.plane.lz"
	.balign 4, 0
.LField_Map128_ANIMATION:
	.incbin "games/tla/src/map/map_008/animation.script.lz"
	.balign 4, 0
.LField_Map128_SCENE:
	.incbin "games/tla/src/map/map_128/scene.parts.lz"
	.endif
