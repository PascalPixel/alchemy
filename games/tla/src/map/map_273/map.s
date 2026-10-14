	.include "games/tla/src/map/map.inc"
	.section .rodata
	.global Field_Map273
Field_Map273:
	map_header 0, 0, 204, 64, 3, 3, 2, 0, 2, 2, 0, 0
	map_layer 0, 0, 12, 12, -2, -2, 0xf, 0xf
	map_layer 0, 64, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map273, .LField_Map273_METATILES
	map_block Field_Map273, .LField_Map273_SHAPES
	map_block Field_Map273, .LField_Map273_GRID
	map_block Field_Map273, .LField_Map273_SHAPE_GRID
	map_block Field_Map273, .LField_Map273_ANIMATION
	map_block Field_Map273
	map_block Field_Map273, .LField_Map273_SCENE
.LField_Map273_METATILES:
	.incbin "games/tla/src/map/map_273/metatiles.delta1.lz"
	.balign 4, 0
.LField_Map273_SHAPES:
	.incbin "games/tla/src/map/map_273/shapes.table.lz"
	.balign 4, 0
.LField_Map273_GRID:
	.incbin "games/tla/src/map/map_273/grid.parts.lz"
	.balign 4, 0
.LField_Map273_SHAPE_GRID:
	.incbin "games/tla/src/map/map_273/shape_grid.plane.lz"
	.balign 4, 0
.LField_Map273_ANIMATION:
	.incbin "games/tla/src/map/map_263/animation.script.lz"
	.balign 4, 0
.LField_Map273_SCENE:
	.incbin "games/tla/src/map/map_273/scene.parts.lz"
