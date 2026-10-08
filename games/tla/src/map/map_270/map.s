	.include "games/common/include/game/ed_asm.h"
	.include "games/tla/src/map/map.inc"
	.section .rodata
	.global Field_Map270
Field_Map270:
	.ifeq EDITION_INTERNATIONAL
	map_header 0, 0, 128, 128, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map270, .LField_Map270_METATILES
	map_block Field_Map270, .LField_Map270_SHAPES
	map_block Field_Map270, .LField_Map270_GRID
	map_block Field_Map270, .LField_Map270_SHAPE_GRID
	map_block Field_Map270, .LField_Map270_ANIMATION
	map_block Field_Map270, .LField_Map270_BLEND
	map_block Field_Map270, .LField_Map270_SCENE
.LField_Map270_METATILES:
	.incbin "map/map_270/metatiles.delta1.lz"
	.balign 4, 0
.LField_Map270_SHAPES:
	.incbin "map/map_270/shapes.table.lz"
	.balign 4, 0
.LField_Map270_GRID:
	.incbin "map/map_270/grid_ja.parts.lz"
	.balign 4, 0
.LField_Map270_SHAPE_GRID:
	.incbin "map/map_270/shape_grid_ja.plane.lz"
	.balign 4, 0
.LField_Map270_ANIMATION:
	.incbin "map/map_259/animation.script.lz"
	.balign 4, 0
.LField_Map270_BLEND:
	.incbin "map/map_260/blend.script.lz"
	.balign 4, 0
.LField_Map270_SCENE:
	.incbin "map/map_270/scene_ja.parts.lz"
	.else
	map_header 0, 0, 128, 128, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map270, .LField_Map270_METATILES
	map_block Field_Map270, .LField_Map270_SHAPES
	map_block Field_Map270, .LField_Map270_GRID
	map_block Field_Map270, .LField_Map270_SHAPE_GRID
	map_block Field_Map270, .LField_Map270_ANIMATION
	map_block Field_Map270, .LField_Map270_BLEND
	map_block Field_Map270, .LField_Map270_SCENE
.LField_Map270_METATILES:
	.incbin "map/map_270/metatiles.delta1.lz"
	.balign 4, 0
.LField_Map270_SHAPES:
	.incbin "map/map_270/shapes.table.lz"
	.balign 4, 0
.LField_Map270_GRID:
	.incbin "map/map_270/grid.parts.lz"
	.balign 4, 0
.LField_Map270_SHAPE_GRID:
	.incbin "map/map_270/shape_grid.plane.lz"
	.balign 4, 0
.LField_Map270_ANIMATION:
	.incbin "map/map_259/animation.script.lz"
	.balign 4, 0
.LField_Map270_BLEND:
	.incbin "map/map_260/blend.script.lz"
	.balign 4, 0
.LField_Map270_SCENE:
	.incbin "map/map_270/scene.parts.lz"
	.endif
