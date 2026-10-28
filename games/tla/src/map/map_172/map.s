	.include "games/tla/include/ed_asm.h"
	.include "games/tla/src/map/map.inc"
	.section .rodata
	.global Field_Map172
Field_Map172:
	.ifeq EDITION_INTERNATIONAL
	map_header 0, 0, 128, 96, 3, 1, 1, 1, 1, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, 253, 3, 0x1f, 0x1f
	map_block Field_Map172, .LField_Map172_CELL_PICTURES
	map_block Field_Map172, .LField_Map172_SHAPES
	map_block Field_Map172, .LField_Map172_GRID
	map_block Field_Map172, .LField_Map172_SHAPE_GRID
	map_block Field_Map172, .LField_Map172_ANIMATION
	map_block Field_Map172, .LField_Map172_BLEND
	map_block Field_Map172, .LField_Map172_SCENE
.LField_Map172_CELL_PICTURES:
	.incbin "games/tla/src/map/map_172/cell_pictures.delta2.lz"
	.balign 4, 0
.LField_Map172_SHAPES:
	.incbin "games/tla/src/map/map_172/shapes.table.lz"
	.balign 4, 0
.LField_Map172_GRID:
	.incbin "games/tla/src/map/map_172/grid_ja.parts.lz"
	.balign 4, 0
.LField_Map172_SHAPE_GRID:
	.incbin "games/tla/src/map/map_172/shape_grid_ja.plane.lz"
	.balign 4, 0
.LField_Map172_ANIMATION:
	.incbin "games/tla/src/map/map_172/animation.script.lz"
	.balign 4, 0
.LField_Map172_BLEND:
	.incbin "games/tla/src/map/map_172/blend.script.lz"
	.balign 4, 0
.LField_Map172_SCENE:
	.incbin "games/tla/src/map/map_172/scene_ja.parts.lz"
	.else
	map_header 0, 0, 128, 96, 3, 1, 1, 1, 1, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, -3, 3, 0x1f, 0x1f
	map_block Field_Map172, .LField_Map172_CELL_PICTURES
	map_block Field_Map172, .LField_Map172_SHAPES
	map_block Field_Map172, .LField_Map172_GRID
	map_block Field_Map172, .LField_Map172_SHAPE_GRID
	map_block Field_Map172, .LField_Map172_ANIMATION
	map_block Field_Map172, .LField_Map172_BLEND
	map_block Field_Map172, .LField_Map172_SCENE
.LField_Map172_CELL_PICTURES:
	.incbin "games/tla/src/map/map_172/cell_pictures.delta2.lz"
	.balign 4, 0
.LField_Map172_SHAPES:
	.incbin "games/tla/src/map/map_172/shapes.table.lz"
	.balign 4, 0
.LField_Map172_GRID:
	.incbin "games/tla/src/map/map_172/grid.parts.lz"
	.balign 4, 0
.LField_Map172_SHAPE_GRID:
	.incbin "games/tla/src/map/map_172/shape_grid.plane.lz"
	.balign 4, 0
.LField_Map172_ANIMATION:
	.incbin "games/tla/src/map/map_172/animation.script.lz"
	.balign 4, 0
.LField_Map172_BLEND:
	.incbin "games/tla/src/map/map_172/blend.script.lz"
	.balign 4, 0
.LField_Map172_SCENE:
	.incbin "games/tla/src/map/map_172/scene.parts.lz"
	.endif
