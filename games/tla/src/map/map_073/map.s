	.include "games/tla/src/map/map.inc"
	.section .rodata
	.global Field_Map073
Field_Map073:
	map_header 0, 0, 128, 128, 3, 2, 2, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map073, .LField_Map073_METATILES
	map_block Field_Map073, .LField_Map073_SHAPES
	map_block Field_Map073, .LField_Map073_GRID
	map_block Field_Map073, .LField_Map073_SHAPE_GRID
	map_block Field_Map073, .LField_Map073_ANIMATION
	map_block Field_Map073
	map_block Field_Map073, .LField_Map073_SCENE
.LField_Map073_METATILES:
	.incbin "map/map_073/metatiles.delta2.lz"
	.balign 4, 0
.LField_Map073_SHAPES:
	.incbin "map/map_073/shapes.table.lz"
	.balign 4, 0
.LField_Map073_GRID:
	.incbin "map/map_073/grid.parts.lz"
	.balign 4, 0
.LField_Map073_SHAPE_GRID:
	.incbin "map/map_073/shape_grid.plane.lz"
	.balign 4, 0
.LField_Map073_ANIMATION:
	.incbin "map/map_012/animation.script.lz"
	.balign 4, 0
.LField_Map073_SCENE:
	.incbin "map/map_073/scene.parts.lz"
