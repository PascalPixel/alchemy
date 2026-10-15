	.include "games/tla/src/map/map.inc"
	.section .rodata
	.global Field_Map244
Field_Map244:
	map_header 0, 0, 128, 128, 3, 1, 2, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map244, .LField_Map244_METATILES
	map_block Field_Map244, .LField_Map244_SHAPES
	map_block Field_Map244, .LField_Map244_GRID
	map_block Field_Map244, .LField_Map244_SHAPE_GRID
	map_block Field_Map244, .LField_Map244_ANIMATION
	map_block Field_Map244, .LField_Map244_BLEND
	map_block Field_Map244, .LField_Map244_SCENE
.LField_Map244_METATILES:
	.incbin "games/tla/src/map/map_244/metatiles.delta1.lz"
	.balign 4, 0
.LField_Map244_SHAPES:
	.incbin "games/tla/src/map/map_244/shapes.table.lz"
	.balign 4, 0
.LField_Map244_GRID:
	.incbin "games/tla/src/map/map_244/grid.parts.lz"
	.balign 4, 0
.LField_Map244_SHAPE_GRID:
	.incbin "games/tla/src/map/map_244/shape_grid.plane.lz"
	.balign 4, 0
.LField_Map244_ANIMATION:
	.incbin "games/tla/src/map/map_244/animation.script.lz"
	.balign 4, 0
.LField_Map244_BLEND:
	.incbin "games/tla/src/map/map_244/blend.script.lz"
	.balign 4, 0
.LField_Map244_SCENE:
	.incbin "games/tla/src/map/map_244/scene.parts.lz"
	.balign 4, 0
