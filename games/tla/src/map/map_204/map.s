	.include "games/tla/src/map/map.inc"
	.section .rodata
	.global Field_Map204
Field_Map204:
	map_header 0, 0, 128, 64, 3, 1, 2, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map204, .LField_Map204_METATILES
	map_block Field_Map204, .LField_Map204_SHAPES
	map_block Field_Map204, .LField_Map204_GRID
	map_block Field_Map204, .LField_Map204_SHAPE_GRID
	map_block Field_Map204, .LField_Map204_ANIMATION
	map_block Field_Map204, .LField_Map204_BLEND
	map_block Field_Map204, .LField_Map204_SCENE
.LField_Map204_METATILES:
	.incbin "map/map_204/metatiles.delta0.lz"
	.balign 4, 0
.LField_Map204_SHAPES:
	.incbin "map/map_204/shapes.table.lz"
	.balign 4, 0
.LField_Map204_GRID:
	.incbin "map/map_204/grid.parts.lz"
	.balign 4, 0
.LField_Map204_SHAPE_GRID:
	.incbin "map/map_204/shape_grid.plane.lz"
	.balign 4, 0
.LField_Map204_ANIMATION:
	.incbin "map/map_204/animation.script.lz"
	.balign 4, 0
.LField_Map204_BLEND:
	.incbin "map/map_001/blend.script.lz"
	.balign 4, 0
.LField_Map204_SCENE:
	.incbin "map/map_204/scene.parts.lz"
