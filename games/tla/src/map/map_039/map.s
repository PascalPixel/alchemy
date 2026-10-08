	.include "games/tla/src/map/map.inc"
	.section .rodata
	.global Field_Map039
Field_Map039:
	map_header 0, 0, 100, 68, 3, 1, 1, 1, 1, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, 1, -1, 0x3f, 0x3f
	map_block Field_Map039, .LField_Map039_METATILES
	map_block Field_Map039, .LField_Map039_SHAPES
	map_block Field_Map039, .LField_Map039_GRID
	map_block Field_Map039, .LField_Map039_SHAPE_GRID
	map_block Field_Map039, .LField_Map039_ANIMATION
	map_block Field_Map039, .LField_Map039_BLEND
	map_block Field_Map039, .LField_Map039_SCENE
.LField_Map039_METATILES:
	.incbin "map/map_039/metatiles.delta2.lz"
	.balign 4, 0
.LField_Map039_SHAPES:
	.incbin "map/map_039/shapes.table.lz"
	.balign 4, 0
.LField_Map039_GRID:
	.incbin "map/map_039/grid.parts.lz"
	.balign 4, 0
.LField_Map039_SHAPE_GRID:
	.incbin "map/map_039/shape_grid.plane.lz"
	.balign 4, 0
.LField_Map039_ANIMATION:
	.incbin "map/map_039/animation.script.lz"
	.balign 4, 0
.LField_Map039_BLEND:
	.incbin "map/map_039/blend.script.lz"
	.balign 4, 0
.LField_Map039_SCENE:
	.incbin "map/map_039/scene.parts.lz"
