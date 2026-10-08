	.include "games/tla/src/map/map.inc"
	.section .rodata
	.global Field_Map230
Field_Map230:
	map_header 0, 0, 128, 128, 3, 1, 2, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map230, .LField_Map230_METATILES
	map_block Field_Map230, .LField_Map230_SHAPES
	map_block Field_Map230, .LField_Map230_GRID
	map_block Field_Map230, .LField_Map230_SHAPE_GRID
	map_block Field_Map230, .LField_Map230_ANIMATION
	map_block Field_Map230
	map_block Field_Map230, .LField_Map230_SCENE
.LField_Map230_METATILES:
	.incbin "map/map_230/metatiles.delta1.lz"
	.balign 4, 0
.LField_Map230_SHAPES:
	.incbin "map/map_230/shapes.table.lz"
	.balign 4, 0
.LField_Map230_GRID:
	.incbin "map/map_230/grid.parts.lz"
	.balign 4, 0
.LField_Map230_SHAPE_GRID:
	.incbin "map/map_230/shape_grid.plane.lz"
	.balign 4, 0
.LField_Map230_ANIMATION:
	.incbin "map/map_012/animation.script.lz"
	.balign 4, 0
.LField_Map230_SCENE:
	.incbin "map/map_230/scene.parts.lz"
