	.include "games/tla/include/game/ed_asm.h"
	.include "games/tla/src/map/map.inc"
	.section .rodata
	.global Field_Map032
Field_Map032:
	.ifeq EDITION_INTERNATIONAL
	map_header 0, 0, 128, 80, 3, 3, 2, 0, 2, 2, 0, 0
	map_layer 0, 64, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 72, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map032, .LField_Map032_METATILES
	map_block Field_Map032, .LField_Map032_SHAPES
	map_block Field_Map032, .LField_Map032_GRID
	map_block Field_Map032, .LField_Map032_SHAPE_GRID
	map_block Field_Map032, .LField_Map032_ANIMATION
	map_block Field_Map032, .LField_Map032_BLEND
	map_block Field_Map032, .LField_Map032_SCENE
.LField_Map032_METATILES:
	.incbin "games/tla/src/map/map_032/metatiles.delta0.lz"
	.balign 4, 0
.LField_Map032_SHAPES:
	.incbin "games/tla/src/map/map_032/shapes.table.lz"
	.balign 4, 0
.LField_Map032_GRID:
	.incbin "games/tla/src/map/map_032/grid_ja.parts.lz"
	.balign 4, 0
.LField_Map032_SHAPE_GRID:
	.incbin "games/tla/src/map/map_032/shape_grid.plane.lz"
	.balign 4, 0
.LField_Map032_ANIMATION:
	.incbin "games/tla/src/map/map_012/animation.script.lz"
	.balign 4, 0
.LField_Map032_BLEND:
	.incbin "games/tla/src/map/map_001/blend.script.lz"
	.balign 4, 0
.LField_Map032_SCENE:
	.incbin "games/tla/src/map/map_032/scene_ja.parts.lz"
	.else
	map_header 0, 0, 128, 80, 3, 3, 2, 0, 2, 2, 0, 0
	map_layer 0, 64, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 72, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map032, .LField_Map032_METATILES
	map_block Field_Map032, .LField_Map032_SHAPES
	map_block Field_Map032, .LField_Map032_GRID
	map_block Field_Map032, .LField_Map032_SHAPE_GRID
	map_block Field_Map032, .LField_Map032_ANIMATION
	map_block Field_Map032, .LField_Map032_BLEND
	map_block Field_Map032, .LField_Map032_SCENE
.LField_Map032_METATILES:
	.incbin "games/tla/src/map/map_032/metatiles.delta0.lz"
	.balign 4, 0
.LField_Map032_SHAPES:
	.incbin "games/tla/src/map/map_032/shapes.table.lz"
	.balign 4, 0
.LField_Map032_GRID:
	.incbin "games/tla/src/map/map_032/grid.parts.lz"
	.balign 4, 0
.LField_Map032_SHAPE_GRID:
	.incbin "games/tla/src/map/map_032/shape_grid.plane.lz"
	.balign 4, 0
.LField_Map032_ANIMATION:
	.incbin "games/tla/src/map/map_012/animation.script.lz"
	.balign 4, 0
.LField_Map032_BLEND:
	.incbin "games/tla/src/map/map_001/blend.script.lz"
	.balign 4, 0
.LField_Map032_SCENE:
	.incbin "games/tla/src/map/map_032/scene.parts.lz"
	.endif
	.balign 4, 0
