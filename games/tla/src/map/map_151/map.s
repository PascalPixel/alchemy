	.include "games/tla/src/map/map.inc"
	.section .rodata
	.global Field_Map151
Field_Map151:
	map_header 0, 0, 130, 112, 3, 2, 2, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map151, .LField_Map151_METATILES
	map_block Field_Map151, .LField_Map151_SHAPES
	map_block Field_Map151, .LField_Map151_GRID
	map_block Field_Map151, .LField_Map151_SHAPE_GRID
	map_block Field_Map151, .LField_Map151_ANIMATION
	map_block Field_Map151, .LField_Map151_BLEND
	map_block Field_Map151, .LField_Map151_SCENE
.LField_Map151_METATILES:
	.incbin "map/map_151/metatiles.delta2.lz"
	.balign 4, 0
.LField_Map151_SHAPES:
	.incbin "map/map_151/shapes.table.lz"
	.balign 4, 0
.LField_Map151_GRID:
	.incbin "map/map_151/grid.parts.lz"
	.balign 4, 0
.LField_Map151_SHAPE_GRID:
	.incbin "map/map_151/shape_grid.plane.lz"
	.balign 4, 0
.LField_Map151_ANIMATION:
	.incbin "map/map_151/animation.script.lz"
	.balign 4, 0
.LField_Map151_BLEND:
	.incbin "map/map_150/blend.script.lz"
	.balign 4, 0
.LField_Map151_SCENE:
	.incbin "map/map_151/scene.parts.lz"
