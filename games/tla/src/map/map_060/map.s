	.include "games/tla/src/map/map.inc"
	.section .rodata
	.global Field_Map060
Field_Map060:
	map_header 0, 0, 128, 128, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, 2, 2, 0xf, 0xf
	map_block Field_Map060, .LField_Map060_METATILES
	map_block Field_Map060, .LField_Map060_SHAPES
	map_block Field_Map060, .LField_Map060_GRID
	map_block Field_Map060, .LField_Map060_SHAPE_GRID
	map_block Field_Map060, .LField_Map060_ANIMATION
	map_block Field_Map060, .LField_Map060_BLEND
	map_block Field_Map060, .LField_Map060_SCENE
.LField_Map060_METATILES:
	.incbin "games/tla/src/map/map_060/metatiles.delta2.lz"
	.balign 4, 0
.LField_Map060_SHAPES:
	.incbin "games/tla/src/map/map_060/shapes.table.lz"
	.balign 4, 0
.LField_Map060_GRID:
	.incbin "games/tla/src/map/map_060/grid.parts.lz"
	.balign 4, 0
.LField_Map060_SHAPE_GRID:
	.incbin "games/tla/src/map/map_060/shape_grid.plane.lz"
	.balign 4, 0
.LField_Map060_ANIMATION:
	.incbin "games/tla/src/map/map_060/animation.script.lz"
	.balign 4, 0
.LField_Map060_BLEND:
	.incbin "games/tla/src/map/map_060/blend.script.lz"
	.balign 4, 0
.LField_Map060_SCENE:
	.incbin "games/tla/src/map/map_060/scene.parts.lz"
