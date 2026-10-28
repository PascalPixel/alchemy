	.include "games/tla/include/game/ed_asm.h"
	.include "games/tla/src/map/map.inc"
	.section .rodata
	.global Field_Map253
Field_Map253:
	.ifeq EDITION_INTERNATIONAL
	map_header 0, 0, 92, 128, 3, 1, 2, 1, 1, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map253, .LField_Map253_CELL_PICTURES
	map_block Field_Map253, .LField_Map253_SHAPES
	map_block Field_Map253, .LField_Map253_GRID
	map_block Field_Map253, .LField_Map253_SHAPE_GRID
	map_block Field_Map253, .LField_Map253_ANIMATION
	map_block Field_Map253, .LField_Map253_BLEND
	map_block Field_Map253, .LField_Map253_SCENE
.LField_Map253_CELL_PICTURES:
	.incbin "games/tla/src/map/map_253/cell_pictures.delta2.lz"
	.balign 4, 0
.LField_Map253_SHAPES:
	.incbin "games/tla/src/map/map_253/shapes_ja.table.lz"
	.balign 4, 0
.LField_Map253_GRID:
	.incbin "games/tla/src/map/map_253/grid_ja.parts.lz"
	.balign 4, 0
.LField_Map253_SHAPE_GRID:
	.incbin "games/tla/src/map/map_253/shape_grid_ja.plane.lz"
	.balign 4, 0
.LField_Map253_ANIMATION:
	.incbin "games/tla/src/map/map_253/animation.script.lz"
	.balign 4, 0
.LField_Map253_BLEND:
	.incbin "games/tla/src/map/map_253/blend.script.lz"
	.balign 4, 0
.LField_Map253_SCENE:
	.incbin "games/tla/src/map/map_253/scene_ja.parts.lz"
	.else
	map_header 0, 0, 92, 128, 3, 1, 2, 1, 1, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map253, .LField_Map253_CELL_PICTURES
	map_block Field_Map253, .LField_Map253_SHAPES
	map_block Field_Map253, .LField_Map253_GRID
	map_block Field_Map253, .LField_Map253_SHAPE_GRID
	map_block Field_Map253, .LField_Map253_ANIMATION
	map_block Field_Map253, .LField_Map253_BLEND
	map_block Field_Map253, .LField_Map253_SCENE
.LField_Map253_CELL_PICTURES:
	.incbin "games/tla/src/map/map_253/cell_pictures.delta2.lz"
	.balign 4, 0
.LField_Map253_SHAPES:
	.incbin "games/tla/src/map/map_253/shapes.table.lz"
	.balign 4, 0
.LField_Map253_GRID:
	.incbin "games/tla/src/map/map_253/grid.parts.lz"
	.balign 4, 0
.LField_Map253_SHAPE_GRID:
	.incbin "games/tla/src/map/map_253/shape_grid.plane.lz"
	.balign 4, 0
.LField_Map253_ANIMATION:
	.incbin "games/tla/src/map/map_253/animation.script.lz"
	.balign 4, 0
.LField_Map253_BLEND:
	.incbin "games/tla/src/map/map_253/blend.script.lz"
	.balign 4, 0
.LField_Map253_SCENE:
	.incbin "games/tla/src/map/map_253/scene.parts.lz"
	.endif
	.balign 4, 0
