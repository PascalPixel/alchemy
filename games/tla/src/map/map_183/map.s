	.include "games/tla/src/map/map.inc"
	.section .rodata
	.global Field_Map183
Field_Map183:
	map_header 0, 0, 128, 128, 3, 1, 2, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map183, .LField_Map183_METATILES
	map_block Field_Map183, .LField_Map183_SHAPES
	map_block Field_Map183, .LField_Map183_GRID
	map_block Field_Map183, .LField_Map183_SHAPE_GRID
	map_block Field_Map183, .LField_Map183_ANIMATION
	map_block Field_Map183, .LField_Map183_BLEND
	map_block Field_Map183, .LField_Map183_SCENE
.LField_Map183_METATILES:
	.incbin "games/tla/src/map/map_183/metatiles.delta1.lz"
	.balign 4, 0
.LField_Map183_SHAPES:
	.incbin "games/tla/src/map/map_183/shapes.table.lz"
	.balign 4, 0
.LField_Map183_GRID:
	.incbin "games/tla/src/map/map_183/grid.parts.lz"
	.balign 4, 0
.LField_Map183_SHAPE_GRID:
	.incbin "games/tla/src/map/map_183/shape_grid.plane.lz"
	.balign 4, 0
.LField_Map183_ANIMATION:
	.incbin "games/tla/src/map/map_183/animation.script.lz"
	.balign 4, 0
.LField_Map183_BLEND:
	.incbin "games/tla/src/map/map_030/blend.script.lz"
	.balign 4, 0
.LField_Map183_SCENE:
	.incbin "games/tla/src/map/map_183/scene.parts.lz"
