	.include "games/tla/src/map/map.inc"
	.section .rodata
	.global Field_Map061
Field_Map061:
	map_header 0, 0, 128, 128, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, 2, 2, 0xf, 0xf
	map_block Field_Map061, .LField_Map061_METATILES
	map_block Field_Map061, .LField_Map061_SHAPES
	map_block Field_Map061, .LField_Map061_GRID
	map_block Field_Map061, .LField_Map061_SHAPE_GRID
	map_block Field_Map061, .LField_Map061_ANIMATION
	map_block Field_Map061, .LField_Map061_BLEND
	map_block Field_Map061, .LField_Map061_SCENE
.LField_Map061_METATILES:
	.incbin "games/tla/src/map/map_061/metatiles.delta2.lz"
	.balign 4, 0
.LField_Map061_SHAPES:
	.incbin "games/tla/src/map/map_061/shapes.table.lz"
	.balign 4, 0
.LField_Map061_GRID:
	.incbin "games/tla/src/map/map_061/grid.parts.lz"
	.balign 4, 0
.LField_Map061_SHAPE_GRID:
	.incbin "games/tla/src/map/map_061/shape_grid.plane.lz"
	.balign 4, 0
.LField_Map061_ANIMATION:
	.incbin "games/tla/src/map/map_061/animation.script.lz"
	.balign 4, 0
.LField_Map061_BLEND:
	.incbin "games/tla/src/map/map_060/blend.script.lz"
	.balign 4, 0
.LField_Map061_SCENE:
	.incbin "games/tla/src/map/map_061/scene.parts.lz"
	.balign 4, 0
