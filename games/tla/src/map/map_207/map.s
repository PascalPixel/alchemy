	.include "games/tla/src/map/map.inc"
	.section .rodata
	.global Field_Map207
Field_Map207:
	map_header 0, 0, 128, 128, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map207, .LField_Map207_METATILES
	map_block Field_Map207, .LField_Map207_SHAPES
	map_block Field_Map207, .LField_Map207_GRID
	map_block Field_Map207, .LField_Map207_SHAPE_GRID
	map_block Field_Map207, .LField_Map207_ANIMATION
	map_block Field_Map207, .LField_Map207_BLEND
	map_block Field_Map207, .LField_Map207_SCENE
.LField_Map207_METATILES:
	.incbin "map/map_207/metatiles.delta2.lz"
	.balign 4, 0
.LField_Map207_SHAPES:
	.incbin "map/map_207/shapes.table.lz"
	.balign 4, 0
.LField_Map207_GRID:
	.incbin "map/map_207/grid.parts.lz"
	.balign 4, 0
.LField_Map207_SHAPE_GRID:
	.incbin "map/map_207/shape_grid.plane.lz"
	.balign 4, 0
.LField_Map207_ANIMATION:
	.incbin "map/map_012/animation.script.lz"
	.balign 4, 0
.LField_Map207_BLEND:
	.incbin "map/map_001/blend.script.lz"
	.balign 4, 0
.LField_Map207_SCENE:
	.incbin "map/map_207/scene.parts.lz"
