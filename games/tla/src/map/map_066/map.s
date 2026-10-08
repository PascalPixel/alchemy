	.include "games/tla/src/map/map.inc"
	.section .rodata
	.global Field_Map066
Field_Map066:
	map_header 0, 0, 128, 128, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, 2, 2, 0xf, 0xf
	map_block Field_Map066, .LField_Map066_METATILES
	map_block Field_Map066, .LField_Map066_SHAPES
	map_block Field_Map066, .LField_Map066_GRID
	map_block Field_Map066, .LField_Map066_SHAPE_GRID
	map_block Field_Map066, .LField_Map066_ANIMATION
	map_block Field_Map066, .LField_Map066_BLEND
	map_block Field_Map066, .LField_Map066_SCENE
.LField_Map066_METATILES:
	.incbin "map/map_066/metatiles.delta2.lz"
	.balign 4, 0
.LField_Map066_SHAPES:
	.incbin "common/map/map_130/shapes.table.lz"
	.balign 4, 0
.LField_Map066_GRID:
	.incbin "map/map_066/grid.parts.lz"
	.balign 4, 0
.LField_Map066_SHAPE_GRID:
	.incbin "map/map_066/shape_grid.plane.lz"
	.balign 4, 0
.LField_Map066_ANIMATION:
	.incbin "map/map_062/animation.script.lz"
	.balign 4, 0
.LField_Map066_BLEND:
	.incbin "map/map_060/blend.script.lz"
	.balign 4, 0
.LField_Map066_SCENE:
	.incbin "map/map_066/scene.parts.lz"
