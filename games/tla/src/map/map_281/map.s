	.include "games/tla/src/map/map.inc"
	.section .rodata
	.global Field_Map281
Field_Map281:
	map_header 0, 0, 128, 128, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map281, .LField_Map281_METATILES
	map_block Field_Map281, .LField_Map281_SHAPES
	map_block Field_Map281, .LField_Map281_GRID
	map_block Field_Map281, .LField_Map281_SHAPE_GRID
	map_block Field_Map281, .LField_Map281_ANIMATION
	map_block Field_Map281, .LField_Map281_BLEND
	map_block Field_Map281, .LField_Map281_SCENE
.LField_Map281_METATILES:
	.incbin "games/tla/src/map/map_281/metatiles.delta2.lz"
	.balign 4, 0
.LField_Map281_SHAPES:
	.incbin "games/tla/src/map/map_281/shapes.table.lz"
	.balign 4, 0
.LField_Map281_GRID:
	.incbin "games/tla/src/map/map_281/grid.parts.lz"
	.balign 4, 0
.LField_Map281_SHAPE_GRID:
	.incbin "games/tla/src/map/map_281/shape_grid.plane.lz"
	.balign 4, 0
.LField_Map281_ANIMATION:
	.incbin "games/tla/src/map/map_012/animation.script.lz"
	.balign 4, 0
.LField_Map281_BLEND:
	.incbin "games/tla/src/map/map_001/blend.script.lz"
	.balign 4, 0
.LField_Map281_SCENE:
	.incbin "games/tla/src/map/map_281/scene.parts.lz"
