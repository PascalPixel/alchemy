	.include "games/tla/src/map/map.inc"
	.section .rodata
	.global Field_Map254
Field_Map254:
	map_header 0, 0, 38, 50, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 38, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 50, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map254, .LField_Map254_METATILES
	map_block Field_Map254, .LField_Map254_SHAPES
	map_block Field_Map254, .LField_Map254_GRID
	map_block Field_Map254, .LField_Map254_SHAPE_GRID
	map_block Field_Map254, .LField_Map254_ANIMATION
	map_block Field_Map254, .LField_Map254_BLEND
	map_block Field_Map254, .LField_Map254_SCENE
.LField_Map254_METATILES:
	.incbin "games/tla/src/map/map_254/metatiles.delta1.lz"
	.balign 4, 0
.LField_Map254_SHAPES:
	.incbin "games/tla/src/map/map_254/shapes.table.lz"
	.balign 4, 0
.LField_Map254_GRID:
	.incbin "games/tla/src/map/map_254/grid.parts.lz"
	.balign 4, 0
.LField_Map254_SHAPE_GRID:
	.incbin "games/tla/src/map/map_254/shape_grid.plane.lz"
	.balign 4, 0
.LField_Map254_ANIMATION:
	.incbin "games/tla/src/map/map_254/animation.script.lz"
	.balign 4, 0
.LField_Map254_BLEND:
	.incbin "games/tla/src/map/map_001/blend.script.lz"
	.balign 4, 0
.LField_Map254_SCENE:
	.incbin "games/tla/src/map/map_254/scene.parts.lz"
	.balign 4, 0
