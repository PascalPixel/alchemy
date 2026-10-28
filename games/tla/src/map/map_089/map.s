	.include "games/tla/include/game/ed_asm.h"
	.include "games/tla/src/map/map.inc"
	.section .rodata
	.global Field_Map089
Field_Map089:
	.ifeq EDITION_INTERNATIONAL
	map_header 0, 0, 120, 120, 3, 1, 2, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 120, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 120, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map089, .LField_Map089_CELL_PICTURES
	map_block Field_Map089, .LField_Map089_SHAPES
	map_block Field_Map089, .LField_Map089_GRID
	map_block Field_Map089, .LField_Map089_SHAPE_GRID
	map_block Field_Map089, .LField_Map089_ANIMATION
	map_block Field_Map089, .LField_Map089_BLEND
	map_block Field_Map089, .LField_Map089_SCENE
.LField_Map089_CELL_PICTURES:
	.incbin "games/tla/src/map/map_089/cell_pictures.delta1.lz"
	.balign 4, 0
.LField_Map089_SHAPES:
	.incbin "games/tla/src/map/map_089/shapes.table.lz"
	.balign 4, 0
.LField_Map089_GRID:
	.incbin "games/tla/src/map/map_089/grid_ja.parts.lz"
	.balign 4, 0
.LField_Map089_SHAPE_GRID:
	.incbin "games/tla/src/map/map_089/shape_grid.plane.lz"
	.balign 4, 0
.LField_Map089_ANIMATION:
	.incbin "games/tla/src/map/map_052/animation.script.lz"
	.balign 4, 0
.LField_Map089_BLEND:
	.incbin "games/tla/src/map/map_010/blend.script.lz"
	.balign 4, 0
.LField_Map089_SCENE:
	.incbin "games/tla/src/map/map_089/scene_ja.parts.lz"
	.else
	map_header 0, 0, 120, 120, 3, 1, 2, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 120, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 120, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map089, .LField_Map089_CELL_PICTURES
	map_block Field_Map089, .LField_Map089_SHAPES
	map_block Field_Map089, .LField_Map089_GRID
	map_block Field_Map089, .LField_Map089_SHAPE_GRID
	map_block Field_Map089, .LField_Map089_ANIMATION
	map_block Field_Map089, .LField_Map089_BLEND
	map_block Field_Map089, .LField_Map089_SCENE
.LField_Map089_CELL_PICTURES:
	.incbin "games/tla/src/map/map_089/cell_pictures.delta1.lz"
	.balign 4, 0
.LField_Map089_SHAPES:
	.incbin "games/tla/src/map/map_089/shapes.table.lz"
	.balign 4, 0
.LField_Map089_GRID:
	.incbin "games/tla/src/map/map_089/grid.parts.lz"
	.balign 4, 0
.LField_Map089_SHAPE_GRID:
	.incbin "games/tla/src/map/map_089/shape_grid.plane.lz"
	.balign 4, 0
.LField_Map089_ANIMATION:
	.incbin "games/tla/src/map/map_052/animation.script.lz"
	.balign 4, 0
.LField_Map089_BLEND:
	.incbin "games/tla/src/map/map_010/blend.script.lz"
	.balign 4, 0
.LField_Map089_SCENE:
	.incbin "games/tla/src/map/map_089/scene.parts.lz"
	.endif
	.balign 4, 0
