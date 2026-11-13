	.include "games/tla/include/ed_asm.h"
	.include "games/tla/src/map/map.inc"
	.section .rodata
	.global Field_Map262
Field_Map262:
	map_header 0, 0, 128, 128, 3, 2, 2, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map262, .LField_Map262_CELL_PICTURES
	map_block Field_Map262, .LField_Map262_SHAPES
	map_block Field_Map262, .LField_Map262_GRID
	map_block Field_Map262, .LField_Map262_SHAPE_GRID
	map_block Field_Map262, .LField_Map262_ANIMATION
	map_block Field_Map262, .LField_Map262_BLEND
	map_block Field_Map262, .LField_Map262_SCENE
.LField_Map262_CELL_PICTURES:
	.incbin "games/tla/src/map/map_262/cell_pictures.delta1.lz"
	.balign 4, 0
.LField_Map262_SHAPES:
	.ifeq EDITION_INTERNATIONAL
	.incbin "games/tla/src/map/map_262/shapes_ja.table.lz"
	.else
	.incbin "games/tla/src/map/map_262/shapes.table.lz"
	.endif
	.balign 4, 0
.LField_Map262_GRID:
	.ifeq EDITION_INTERNATIONAL
	.incbin "games/tla/src/map/map_262/map_ja.grid.lz"
	.else
	.incbin "games/tla/src/map/map_262/map.grid.lz"
	.endif
	.balign 4, 0
.LField_Map262_SHAPE_GRID:
	.ifeq EDITION_INTERNATIONAL
	.incbin "games/tla/src/map/map_262/shape_grid_ja.plane.lz"
	.else
	.incbin "games/tla/src/map/map_262/shape_grid.plane.lz"
	.endif
	.balign 4, 0
.LField_Map262_ANIMATION:
	.incbin "games/tla/src/map/map_259/animation.script.lz"
	.balign 4, 0
.LField_Map262_BLEND:
	.incbin "games/tla/src/map/map_260/blend.script.lz"
	.balign 4, 0
.LField_Map262_SCENE:
	.ifeq EDITION_INTERNATIONAL
	.incbin "games/tla/src/map/map_262/scene_ja.scene.lz"
	.else
	.incbin "games/tla/src/map/map_262/scene.scene.lz"
	.endif
	.balign 4, 0
