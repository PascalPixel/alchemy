	.include "games/tla/src/map/map.inc"
	.section .rodata
	.global Field_Map150
Field_Map150:
	map_header 0, 0, 130, 112, 3, 2, 2, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map150, .LField_Map150_METATILES
	map_block Field_Map150, .LField_Map150_SHAPES
	map_block Field_Map150, .LField_Map150_GRID
	map_block Field_Map150, .LField_Map150_SHAPE_GRID
	map_block Field_Map150, .LField_Map150_ANIMATION
	map_block Field_Map150, .LField_Map150_BLEND
	map_block Field_Map150, .LField_Map150_SCENE
.LField_Map150_METATILES:
	.incbin "map/map_150/metatiles.delta2.lz"
	.balign 4, 0
.LField_Map150_SHAPES:
	.incbin "map/map_150/shapes.table.lz"
	.balign 4, 0
.LField_Map150_GRID:
	.incbin "map/map_150/grid.parts.lz"
	.balign 4, 0
.LField_Map150_SHAPE_GRID:
	.incbin "map/map_150/shape_grid.plane.lz"
	.balign 4, 0
.LField_Map150_ANIMATION:
	.incbin "map/map_150/animation.script.lz"
	.balign 4, 0
.LField_Map150_BLEND:
	.incbin "map/map_150/blend.script.lz"
	.balign 4, 0
.LField_Map150_SCENE:
	.incbin "map/map_150/scene.parts.lz"
