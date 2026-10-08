	.include "games/tla/src/map/map.inc"
	.section .rodata
	.global Field_Map213
Field_Map213:
	map_header 0, 0, 130, 130, 3, 3, 2, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map213, .LField_Map213_METATILES
	map_block Field_Map213, .LField_Map213_SHAPES
	map_block Field_Map213, .LField_Map213_GRID
	map_block Field_Map213, .LField_Map213_SHAPE_GRID
	map_block Field_Map213, .LField_Map213_ANIMATION
	map_block Field_Map213, .LField_Map213_BLEND
	map_block Field_Map213, .LField_Map213_SCENE
.LField_Map213_METATILES:
	.incbin "map/map_213/metatiles.delta2.lz"
	.balign 4, 0
.LField_Map213_SHAPES:
	.incbin "map/map_213/shapes.table.lz"
	.balign 4, 0
.LField_Map213_GRID:
	.incbin "map/map_213/grid.parts.lz"
	.balign 4, 0
.LField_Map213_SHAPE_GRID:
	.incbin "map/map_213/shape_grid.plane.lz"
	.balign 4, 0
.LField_Map213_ANIMATION:
	.incbin "map/map_211/animation.script.lz"
	.balign 4, 0
.LField_Map213_BLEND:
	.incbin "map/map_001/blend.script.lz"
	.balign 4, 0
.LField_Map213_SCENE:
	.incbin "map/map_213/scene.parts.lz"
