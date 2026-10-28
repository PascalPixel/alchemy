	.include "games/tla/include/ed_asm.h"
	.include "games/tla/src/map/map.inc"
	.section .rodata
	.global Field_Map261
Field_Map261:
	.ifeq EDITION_INTERNATIONAL
	map_header 0, 0, 128, 128, 3, 2, 2, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map261, .LField_Map261_CELL_PICTURES
	map_block Field_Map261, .LField_Map261_SHAPES
	map_block Field_Map261, .LField_Map261_GRID
	map_block Field_Map261, .LField_Map261_SHAPE_GRID
	map_block Field_Map261, .LField_Map261_ANIMATION
	map_block Field_Map261, .LField_Map261_BLEND
	map_block Field_Map261, .LField_Map261_SCENE
.LField_Map261_CELL_PICTURES:
	.incbin "games/tla/src/map/map_261/cell_pictures.delta1.lz"
	.balign 4, 0
.LField_Map261_SHAPES:
	.incbin "games/tla/src/map/map_261/shapes.table.lz"
	.balign 4, 0
.LField_Map261_GRID:
	.incbin "games/tla/src/map/map_261/grid_ja.parts.lz"
	.balign 4, 0
.LField_Map261_SHAPE_GRID:
	.incbin "games/tla/src/map/map_261/shape_grid_ja.plane.lz"
	.balign 4, 0
.LField_Map261_ANIMATION:
	.incbin "games/tla/src/map/map_259/animation.script.lz"
	.balign 4, 0
.LField_Map261_BLEND:
	.incbin "games/tla/src/map/map_260/blend.script.lz"
	.balign 4, 0
.LField_Map261_SCENE:
	.incbin "games/tla/src/map/map_261/scene_ja.parts.lz"
	.else
	map_header 0, 0, 128, 128, 3, 2, 2, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map261, .LField_Map261_CELL_PICTURES
	map_block Field_Map261, .LField_Map261_SHAPES
	map_block Field_Map261, .LField_Map261_GRID
	map_block Field_Map261, .LField_Map261_SHAPE_GRID
	map_block Field_Map261, .LField_Map261_ANIMATION
	map_block Field_Map261, .LField_Map261_BLEND
	map_block Field_Map261, .LField_Map261_SCENE
.LField_Map261_CELL_PICTURES:
	.incbin "games/tla/src/map/map_261/cell_pictures.delta1.lz"
	.balign 4, 0
.LField_Map261_SHAPES:
	.incbin "games/tla/src/map/map_261/shapes.table.lz"
	.balign 4, 0
.LField_Map261_GRID:
	.incbin "games/tla/src/map/map_261/grid.parts.lz"
	.balign 4, 0
.LField_Map261_SHAPE_GRID:
	.incbin "games/tla/src/map/map_261/shape_grid.plane.lz"
	.balign 4, 0
.LField_Map261_ANIMATION:
	.incbin "games/tla/src/map/map_259/animation.script.lz"
	.balign 4, 0
.LField_Map261_BLEND:
	.incbin "games/tla/src/map/map_260/blend.script.lz"
	.balign 4, 0
.LField_Map261_SCENE:
	.incbin "games/tla/src/map/map_261/scene.parts.lz"
	.endif
	.balign 4, 0
