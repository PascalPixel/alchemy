	.include "games/tla/src/map/map.inc"
	.section .rodata
	.global Field_Map068
Field_Map068:
	map_header 0, 0, 128, 128, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, 2, 2, 0xf, 0xf
	map_block Field_Map068, .LField_Map068_METATILES
	map_block Field_Map068, .LField_Map068_SHAPES
	map_block Field_Map068, .LField_Map068_GRID
	map_block Field_Map068, .LField_Map068_SHAPE_GRID
	map_block Field_Map068, .LField_Map068_ANIMATION
	map_block Field_Map068, .LField_Map068_BLEND
	map_block Field_Map068, .LField_Map068_SCENE
.LField_Map068_METATILES:
	.incbin "games/tla/src/map/map_068/metatiles.delta2.lz"
	.balign 4, 0
.LField_Map068_SHAPES:
	.incbin "games/common/src/map/map_062/shapes.table.lz"
	.balign 4, 0
.LField_Map068_GRID:
	.incbin "games/tla/src/map/map_068/grid.parts.lz"
	.balign 4, 0
.LField_Map068_SHAPE_GRID:
	.incbin "games/tla/src/map/map_068/shape_grid.plane.lz"
	.balign 4, 0
.LField_Map068_ANIMATION:
	.incbin "games/tla/src/map/map_062/animation.script.lz"
	.balign 4, 0
.LField_Map068_BLEND:
	.incbin "games/tla/src/map/map_060/blend.script.lz"
	.balign 4, 0
.LField_Map068_SCENE:
	.incbin "games/tla/src/map/map_068/scene.parts.lz"
	.balign 4, 0
