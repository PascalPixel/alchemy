	.include "games/common/include/game/ed_asm.h"
	.include "games/tla/src/map/map.inc"
	.section .rodata
	.global Field_Map262
Field_Map262:
	.ifeq EDITION_INTERNATIONAL
	map_header 0, 0, 128, 128, 3, 2, 2, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map262, .LField_Map262_METATILES
	map_block Field_Map262, .LField_Map262_SHAPES
	map_block Field_Map262, .LField_Map262_GRID
	map_block Field_Map262, .LField_Map262_SHAPE_GRID
	map_block Field_Map262, .LField_Map262_ANIMATION
	map_block Field_Map262, .LField_Map262_BLEND
	map_block Field_Map262, .LField_Map262_SCENE
.LField_Map262_METATILES:
	.incbin "map/map_262/metatiles.delta1.lz"
	.balign 4, 0
.LField_Map262_SHAPES:
	.incbin "map/map_262/shapes_ja.table.lz"
	.balign 4, 0
.LField_Map262_GRID:
	.incbin "map/map_262/grid_ja.parts.lz"
	.balign 4, 0
.LField_Map262_SHAPE_GRID:
	.incbin "map/map_262/shape_grid_ja.plane.lz"
	.balign 4, 0
.LField_Map262_ANIMATION:
	.incbin "map/map_259/animation.script.lz"
	.balign 4, 0
.LField_Map262_BLEND:
	.incbin "map/map_260/blend.script.lz"
	.balign 4, 0
.LField_Map262_SCENE:
	.incbin "map/map_262/scene_ja.parts.lz"
	.else
	map_header 0, 0, 128, 128, 3, 2, 2, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map262, .LField_Map262_METATILES
	map_block Field_Map262, .LField_Map262_SHAPES
	map_block Field_Map262, .LField_Map262_GRID
	map_block Field_Map262, .LField_Map262_SHAPE_GRID
	map_block Field_Map262, .LField_Map262_ANIMATION
	map_block Field_Map262, .LField_Map262_BLEND
	map_block Field_Map262, .LField_Map262_SCENE
.LField_Map262_METATILES:
	.incbin "map/map_262/metatiles.delta1.lz"
	.balign 4, 0
.LField_Map262_SHAPES:
	.incbin "map/map_262/shapes.table.lz"
	.balign 4, 0
.LField_Map262_GRID:
	.incbin "map/map_262/grid.parts.lz"
	.balign 4, 0
.LField_Map262_SHAPE_GRID:
	.incbin "map/map_262/shape_grid.plane.lz"
	.balign 4, 0
.LField_Map262_ANIMATION:
	.incbin "map/map_259/animation.script.lz"
	.balign 4, 0
.LField_Map262_BLEND:
	.incbin "map/map_260/blend.script.lz"
	.balign 4, 0
.LField_Map262_SCENE:
	.incbin "map/map_262/scene.parts.lz"
	.endif
