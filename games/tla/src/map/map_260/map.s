	.include "games/common/include/game/ed_asm.h"
	.include "games/tla/src/map/map.inc"
	.section .rodata
	.global Field_Map260
Field_Map260:
	.ifeq EDITION_INTERNATIONAL
	map_header 0, 0, 128, 128, 3, 2, 2, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map260, .LField_Map260_METATILES
	map_block Field_Map260, .LField_Map260_SHAPES
	map_block Field_Map260, .LField_Map260_GRID
	map_block Field_Map260, .LField_Map260_SHAPE_GRID
	map_block Field_Map260, .LField_Map260_ANIMATION
	map_block Field_Map260, .LField_Map260_BLEND
	map_block Field_Map260, .LField_Map260_SCENE
.LField_Map260_METATILES:
	.incbin "games/tla/src/map/map_260/metatiles.delta1.lz"
	.balign 4, 0
.LField_Map260_SHAPES:
	.incbin "games/tla/src/map/map_260/shapes.table.lz"
	.balign 4, 0
.LField_Map260_GRID:
	.incbin "games/tla/src/map/map_260/grid_ja.parts.lz"
	.balign 4, 0
.LField_Map260_SHAPE_GRID:
	.incbin "games/tla/src/map/map_260/shape_grid_ja.plane.lz"
	.balign 4, 0
.LField_Map260_ANIMATION:
	.incbin "games/tla/src/map/map_259/animation.script.lz"
	.balign 4, 0
.LField_Map260_BLEND:
	.incbin "games/tla/src/map/map_260/blend.script.lz"
	.balign 4, 0
.LField_Map260_SCENE:
	.incbin "games/tla/src/map/map_260/scene_ja.parts.lz"
	.else
	map_header 0, 0, 128, 128, 3, 2, 2, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map260, .LField_Map260_METATILES
	map_block Field_Map260, .LField_Map260_SHAPES
	map_block Field_Map260, .LField_Map260_GRID
	map_block Field_Map260, .LField_Map260_SHAPE_GRID
	map_block Field_Map260, .LField_Map260_ANIMATION
	map_block Field_Map260, .LField_Map260_BLEND
	map_block Field_Map260, .LField_Map260_SCENE
.LField_Map260_METATILES:
	.incbin "games/tla/src/map/map_260/metatiles.delta1.lz"
	.balign 4, 0
.LField_Map260_SHAPES:
	.incbin "games/tla/src/map/map_260/shapes.table.lz"
	.balign 4, 0
.LField_Map260_GRID:
	.incbin "games/tla/src/map/map_260/grid.parts.lz"
	.balign 4, 0
.LField_Map260_SHAPE_GRID:
	.incbin "games/tla/src/map/map_260/shape_grid.plane.lz"
	.balign 4, 0
.LField_Map260_ANIMATION:
	.incbin "games/tla/src/map/map_259/animation.script.lz"
	.balign 4, 0
.LField_Map260_BLEND:
	.incbin "games/tla/src/map/map_260/blend.script.lz"
	.balign 4, 0
.LField_Map260_SCENE:
	.incbin "games/tla/src/map/map_260/scene.parts.lz"
	.endif
