	.include "games/tla/src/map/map.inc"
	.section .rodata
	.global Field_Map091
Field_Map091:
	map_header 0, 0, 128, 128, 2, 1, 3, 1, 1, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map091, .LField_Map091_METATILES
	map_block Field_Map091, .LField_Map091_SHAPES
	map_block Field_Map091, .LField_Map091_GRID
	map_block Field_Map091, .LField_Map091_SHAPE_GRID
	map_block Field_Map091, .LField_Map091_ANIMATION
	map_block Field_Map091, .LField_Map091_BLEND
	map_block Field_Map091, .LField_Map091_SCENE
.LField_Map091_METATILES:
	.incbin "map/map_091/metatiles.delta2.lz"
	.balign 4, 0
.LField_Map091_SHAPES:
	.incbin "map/map_091/shapes.table.lz"
	.balign 4, 0
.LField_Map091_GRID:
	.incbin "map/map_091/grid.parts.lz"
	.balign 4, 0
.LField_Map091_SHAPE_GRID:
	.incbin "map/map_091/shape_grid.plane.lz"
	.balign 4, 0
.LField_Map091_ANIMATION:
	.incbin "map/map_091/animation.script.lz"
	.balign 4, 0
.LField_Map091_BLEND:
	.incbin "map/map_001/blend.script.lz"
	.balign 4, 0
.LField_Map091_SCENE:
	.incbin "map/map_091/scene.parts.lz"
