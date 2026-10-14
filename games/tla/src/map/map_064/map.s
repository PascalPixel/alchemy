	.include "games/tla/src/map/map.inc"
	.section .rodata
	.global Field_Map064
Field_Map064:
	map_header 0, 0, 128, 128, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, 2, 2, 0xf, 0xf
	map_block Field_Map064, .LField_Map064_METATILES
	map_block Field_Map064, .LField_Map064_SHAPES
	map_block Field_Map064, .LField_Map064_GRID
	map_block Field_Map064, .LField_Map064_SHAPE_GRID
	map_block Field_Map064, .LField_Map064_ANIMATION
	map_block Field_Map064, .LField_Map064_BLEND
	map_block Field_Map064, .LField_Map064_SCENE
.LField_Map064_METATILES:
	.incbin "games/tla/src/map/map_064/metatiles.delta2.lz"
	.balign 4, 0
.LField_Map064_SHAPES:
	.incbin "games/tla/src/map/map_064/shapes.table.lz"
	.balign 4, 0
.LField_Map064_GRID:
	.incbin "games/tla/src/map/map_064/grid.parts.lz"
	.balign 4, 0
.LField_Map064_SHAPE_GRID:
	.incbin "games/tla/src/map/map_064/shape_grid.plane.lz"
	.balign 4, 0
.LField_Map064_ANIMATION:
	.incbin "games/tla/src/map/map_062/animation.script.lz"
	.balign 4, 0
.LField_Map064_BLEND:
	.incbin "games/tla/src/map/map_060/blend.script.lz"
	.balign 4, 0
.LField_Map064_SCENE:
	.incbin "games/tla/src/map/map_064/scene.parts.lz"
