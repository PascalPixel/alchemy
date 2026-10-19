	.include "games/tla/src/map/map.inc"
	.section .rodata
	.global Field_Map221
Field_Map221:
	map_header 0, 0, 120, 120, 3, 1, 2, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 120, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 120, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map221, .LField_Map221_METATILES
	map_block Field_Map221, .LField_Map221_SHAPES
	map_block Field_Map221, .LField_Map221_GRID
	map_block Field_Map221, .LField_Map221_SHAPE_GRID
	map_block Field_Map221, .LField_Map221_ANIMATION
	map_block Field_Map221, .LField_Map221_BLEND
	map_block Field_Map221, .LField_Map221_SCENE
.LField_Map221_METATILES:
	.incbin "games/tla/src/map/map_221/metatiles.delta1.lz"
	.balign 4, 0
.LField_Map221_SHAPES:
	.incbin "games/tla/src/map/map_221/shapes.table.lz"
	.balign 4, 0
.LField_Map221_GRID:
	.incbin "games/tla/src/map/map_221/grid.parts.lz"
	.balign 4, 0
.LField_Map221_SHAPE_GRID:
	.incbin "games/tla/src/map/map_221/shape_grid.plane.lz"
	.balign 4, 0
.LField_Map221_ANIMATION:
	.incbin "games/tla/src/map/map_221/animation.script.lz"
	.balign 4, 0
.LField_Map221_BLEND:
	.incbin "games/tla/src/map/map_010/blend.script.lz"
	.balign 4, 0
.LField_Map221_SCENE:
	.incbin "games/tla/src/map/map_221/scene.parts.lz"
	.balign 4, 0
