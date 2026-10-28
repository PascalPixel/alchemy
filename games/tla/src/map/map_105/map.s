	.include "games/tla/include/game/ed_asm.h"
	.include "games/tla/src/map/map.inc"
	.section .rodata
	.global Field_Map105
Field_Map105:
	.ifeq EDITION_INTERNATIONAL
	map_header 0, 0, 64, 66, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map105, .LField_Map105_CELL_PICTURES
	map_block Field_Map105, .LField_Map105_SHAPES
	map_block Field_Map105, .LField_Map105_GRID
	map_block Field_Map105, .LField_Map105_SHAPE_GRID
	map_block Field_Map105, .LField_Map105_ANIMATION
	map_block Field_Map105, .LField_Map105_BLEND
	map_block Field_Map105, .LField_Map105_SCENE
.LField_Map105_CELL_PICTURES:
	.incbin "games/tla/src/map/map_105/cell_pictures_ja.delta1.lz"
	.balign 4, 0
.LField_Map105_SHAPES:
	.incbin "games/tla/src/map/map_103/shapes.table.lz"
	.balign 4, 0
.LField_Map105_GRID:
	.incbin "games/tla/src/map/map_105/grid_ja.parts.lz"
	.balign 4, 0
.LField_Map105_SHAPE_GRID:
	.incbin "games/tla/src/map/map_103/shape_grid.plane.lz"
	.balign 4, 0
.LField_Map105_ANIMATION:
	.incbin "games/tla/src/map/map_012/animation.script.lz"
	.balign 4, 0
.LField_Map105_BLEND:
	.incbin "games/tla/src/map/map_001/blend.script.lz"
	.balign 4, 0
.LField_Map105_SCENE:
	.incbin "games/tla/src/map/map_105/scene_ja.parts.lz"
	.else
	map_header 0, 0, 64, 66, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map105, .LField_Map105_CELL_PICTURES
	map_block Field_Map105, .LField_Map105_SHAPES
	map_block Field_Map105, .LField_Map105_GRID
	map_block Field_Map105, .LField_Map105_SHAPE_GRID
	map_block Field_Map105, .LField_Map105_ANIMATION
	map_block Field_Map105, .LField_Map105_BLEND
	map_block Field_Map105, .LField_Map105_SCENE
.LField_Map105_CELL_PICTURES:
	.incbin "games/tla/src/map/map_105/cell_pictures.delta1.lz"
	.balign 4, 0
.LField_Map105_SHAPES:
	.incbin "games/tla/src/map/map_103/shapes.table.lz"
	.balign 4, 0
.LField_Map105_GRID:
	.incbin "games/tla/src/map/map_105/grid.parts.lz"
	.balign 4, 0
.LField_Map105_SHAPE_GRID:
	.incbin "games/tla/src/map/map_103/shape_grid.plane.lz"
	.balign 4, 0
.LField_Map105_ANIMATION:
	.incbin "games/tla/src/map/map_012/animation.script.lz"
	.balign 4, 0
.LField_Map105_BLEND:
	.incbin "games/tla/src/map/map_001/blend.script.lz"
	.balign 4, 0
.LField_Map105_SCENE:
	.incbin "games/tla/src/map/map_105/scene.parts.lz"
	.endif
	.balign 4, 0
