	.include "games/tla/include/ed_asm.h"
	.include "games/tla/src/map/map.inc"
	.section .rodata
	.global Field_Map192
Field_Map192:
	.ifeq EDITION_INTERNATIONAL
	map_header 0, 0, 120, 120, 3, 1, 2, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 120, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 120, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map192, .LField_Map192_CELL_PICTURES
	map_block Field_Map192, .LField_Map192_SHAPES
	map_block Field_Map192, .LField_Map192_GRID
	map_block Field_Map192, .LField_Map192_SHAPE_GRID
	map_block Field_Map192, .LField_Map192_ANIMATION
	map_block Field_Map192
	map_block Field_Map192, .LField_Map192_SCENE
.LField_Map192_CELL_PICTURES:
	.incbin "games/tla/src/map/map_192/cell_pictures.delta1.lz"
	.balign 4, 0
.LField_Map192_SHAPES:
	.incbin "games/tla/src/map/map_192/shapes.table.lz"
	.balign 4, 0
.LField_Map192_GRID:
	.incbin "games/tla/src/map/map_192/grid.parts.lz"
	.balign 4, 0
.LField_Map192_SHAPE_GRID:
	.incbin "games/tla/src/map/map_192/shape_grid.plane.lz"
	.balign 4, 0
.LField_Map192_ANIMATION:
	.incbin "games/tla/src/map/map_192/animation.script.lz"
	.balign 4, 0
.LField_Map192_SCENE:
	.incbin "games/tla/src/map/map_192/scene_ja.parts.lz"
	.else
	map_header 0, 0, 120, 120, 3, 1, 2, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 120, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 120, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map192, .LField_Map192_CELL_PICTURES
	map_block Field_Map192, .LField_Map192_SHAPES
	map_block Field_Map192, .LField_Map192_GRID
	map_block Field_Map192, .LField_Map192_SHAPE_GRID
	map_block Field_Map192, .LField_Map192_ANIMATION
	map_block Field_Map192
	map_block Field_Map192, .LField_Map192_SCENE
.LField_Map192_CELL_PICTURES:
	.incbin "games/tla/src/map/map_192/cell_pictures.delta1.lz"
	.balign 4, 0
.LField_Map192_SHAPES:
	.incbin "games/tla/src/map/map_192/shapes.table.lz"
	.balign 4, 0
.LField_Map192_GRID:
	.incbin "games/tla/src/map/map_192/grid.parts.lz"
	.balign 4, 0
.LField_Map192_SHAPE_GRID:
	.incbin "games/tla/src/map/map_192/shape_grid.plane.lz"
	.balign 4, 0
.LField_Map192_ANIMATION:
	.incbin "games/tla/src/map/map_192/animation.script.lz"
	.balign 4, 0
.LField_Map192_SCENE:
	.incbin "games/tla/src/map/map_192/scene.parts.lz"
	.endif
	.balign 4, 0
