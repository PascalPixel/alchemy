	.include "games/tla/src/map/map.inc"
	.section .rodata
	.global Field_Map264
Field_Map264:
	map_header 0, 0, 128, 128, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 12, 12, -2, -2, 0x1f, 0x1f
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map264, .LField_Map264_METATILES
	map_block Field_Map264, .LField_Map264_SHAPES
	map_block Field_Map264, .LField_Map264_GRID
	map_block Field_Map264, .LField_Map264_SHAPE_GRID
	map_block Field_Map264, .LField_Map264_ANIMATION
	map_block Field_Map264
	map_block Field_Map264, .LField_Map264_SCENE
.LField_Map264_METATILES:
	.incbin "map/map_264/metatiles.delta2.lz"
	.balign 4, 0
.LField_Map264_SHAPES:
	.incbin "map/map_264/shapes.table.lz"
	.balign 4, 0
.LField_Map264_GRID:
	.incbin "map/map_264/grid.parts.lz"
	.balign 4, 0
.LField_Map264_SHAPE_GRID:
	.incbin "map/map_264/shape_grid.plane.lz"
	.balign 4, 0
.LField_Map264_ANIMATION:
	.incbin "map/map_264/animation.script.lz"
	.balign 4, 0
.LField_Map264_SCENE:
	.incbin "map/map_264/scene.parts.lz"
