	.include "games/tla/src/map/map.inc"
	.section .rodata
	.global Field_Map189
Field_Map189:
	map_header 0, 0, 128, 160, 1, 1, 3, 1, 2, 1, 0, 0
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 160, 16, 16, 3, 3, 0x1f, 0x1f
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map189, .LField_Map189_METATILES
	map_block Field_Map189, .LField_Map189_SHAPES
	map_block Field_Map189, .LField_Map189_GRID
	map_block Field_Map189, .LField_Map189_SHAPE_GRID
	map_block Field_Map189, .LField_Map189_ANIMATION
	map_block Field_Map189, .LField_Map189_BLEND
	map_block Field_Map189, .LField_Map189_SCENE
.LField_Map189_METATILES:
	.incbin "games/tla/src/map/map_189/metatiles.delta2.lz"
	.balign 4, 0
.LField_Map189_SHAPES:
	.incbin "games/tla/src/map/map_189/shapes.table.lz"
	.balign 4, 0
.LField_Map189_GRID:
	.incbin "games/tla/src/map/map_189/grid.parts.lz"
	.balign 4, 0
.LField_Map189_SHAPE_GRID:
	.incbin "games/tla/src/map/map_189/shape_grid.plane.lz"
	.balign 4, 0
.LField_Map189_ANIMATION:
	.incbin "games/tla/src/map/map_189/animation.script.lz"
	.balign 4, 0
.LField_Map189_BLEND:
	.incbin "games/tla/src/map/map_189/blend.script.lz"
	.balign 4, 0
.LField_Map189_SCENE:
	.incbin "games/tla/src/map/map_189/scene.parts.lz"
	.balign 4, 0
