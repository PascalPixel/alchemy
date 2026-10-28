	.include "games/tla/src/map/map.inc"
	.section .rodata
	.global Field_Map196
Field_Map196:
	map_header 0, 0, 128, 128, 3, 2, 2, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map196, .LField_Map196_CELL_PICTURES
	map_block Field_Map196, .LField_Map196_SHAPES
	map_block Field_Map196, .LField_Map196_GRID
	map_block Field_Map196, .LField_Map196_SHAPE_GRID
	map_block Field_Map196, .LField_Map196_ANIMATION
	map_block Field_Map196, .LField_Map196_BLEND
	map_block Field_Map196, .LField_Map196_SCENE
.LField_Map196_CELL_PICTURES:
	.incbin "games/tla/src/map/map_196/cell_pictures.delta2.lz"
	.balign 4, 0
.LField_Map196_SHAPES:
	.incbin "games/tla/src/map/map_195/shapes.table.lz"
	.balign 4, 0
.LField_Map196_GRID:
	.incbin "games/tla/src/map/map_196/grid.parts.lz"
	.balign 4, 0
.LField_Map196_SHAPE_GRID:
	.incbin "games/tla/src/map/map_196/shape_grid.plane.lz"
	.balign 4, 0
.LField_Map196_ANIMATION:
	.incbin "games/tla/src/map/map_195/animation.script.lz"
	.balign 4, 0
.LField_Map196_BLEND:
	.incbin "games/tla/src/map/map_196/blend.script.lz"
	.balign 4, 0
.LField_Map196_SCENE:
	.incbin "games/tla/src/map/map_196/scene.parts.lz"
	.balign 4, 0
