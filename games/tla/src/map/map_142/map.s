	.include "games/tla/include/ed_asm.h"
	.include "games/tla/src/map/map.inc"
	.section .rodata
	.global Field_Map142
Field_Map142:
	.ifeq EDITION_INTERNATIONAL
	map_header 0, 0, 90, 100, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 100, 16, 16, 0, 0, 0xff, 0xff
	map_layer 100, 0, 16, 16, 2, 2, 0xf, 0xf
	map_block Field_Map142, .LField_Map142_CELL_PICTURES
	map_block Field_Map142, .LField_Map142_SHAPES
	map_block Field_Map142, .LField_Map142_GRID
	map_block Field_Map142, .LField_Map142_SHAPE_GRID
	map_block Field_Map142, .LField_Map142_ANIMATION
	map_block Field_Map142, .LField_Map142_BLEND
	map_block Field_Map142, .LField_Map142_SCENE
.LField_Map142_CELL_PICTURES:
	.incbin "games/tla/src/map/map_142/cell_pictures.delta2.lz"
	.balign 4, 0
.LField_Map142_SHAPES:
	.incbin "games/tla/src/map/map_142/shapes.table.lz"
	.balign 4, 0
.LField_Map142_GRID:
	.incbin "games/tla/src/map/map_142/grid_ja.parts.lz"
	.balign 4, 0
.LField_Map142_SHAPE_GRID:
	.incbin "games/tla/src/map/map_142/shape_grid_ja.plane.lz"
	.balign 4, 0
.LField_Map142_ANIMATION:
	.incbin "games/tla/src/map/map_142/animation.script.lz"
	.balign 4, 0
.LField_Map142_BLEND:
	.incbin "games/tla/src/map/map_060/blend.script.lz"
	.balign 4, 0
.LField_Map142_SCENE:
	.incbin "games/tla/src/map/map_142/scene_ja.parts.lz"
	.else
	map_header 0, 0, 90, 100, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 100, 16, 16, 0, 0, 0xff, 0xff
	map_layer 100, 0, 16, 16, 2, 2, 0xf, 0xf
	map_block Field_Map142, .LField_Map142_CELL_PICTURES
	map_block Field_Map142, .LField_Map142_SHAPES
	map_block Field_Map142, .LField_Map142_GRID
	map_block Field_Map142, .LField_Map142_SHAPE_GRID
	map_block Field_Map142, .LField_Map142_ANIMATION
	map_block Field_Map142, .LField_Map142_BLEND
	map_block Field_Map142, .LField_Map142_SCENE
.LField_Map142_CELL_PICTURES:
	.incbin "games/tla/src/map/map_142/cell_pictures.delta2.lz"
	.balign 4, 0
.LField_Map142_SHAPES:
	.incbin "games/tla/src/map/map_142/shapes.table.lz"
	.balign 4, 0
.LField_Map142_GRID:
	.incbin "games/tla/src/map/map_142/grid.parts.lz"
	.balign 4, 0
.LField_Map142_SHAPE_GRID:
	.incbin "games/tla/src/map/map_142/shape_grid.plane.lz"
	.balign 4, 0
.LField_Map142_ANIMATION:
	.incbin "games/tla/src/map/map_142/animation.script.lz"
	.balign 4, 0
.LField_Map142_BLEND:
	.incbin "games/tla/src/map/map_060/blend.script.lz"
	.balign 4, 0
.LField_Map142_SCENE:
	.incbin "games/tla/src/map/map_142/scene.parts.lz"
	.endif
