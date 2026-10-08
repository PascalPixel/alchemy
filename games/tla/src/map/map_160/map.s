	.include "games/tla/src/map/map.inc"
	.section .rodata
	.global Field_Map160
Field_Map160:
	map_header 0, 0, 68, 88, 3, 1, 3, 1, 1, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map160, .LField_Map160_METATILES
	map_block Field_Map160, .LField_Map160_SHAPES
	map_block Field_Map160, .LField_Map160_GRID
	map_block Field_Map160, .LField_Map160_SHAPE_GRID
	map_block Field_Map160, .LField_Map160_ANIMATION
	map_block Field_Map160, .LField_Map160_BLEND
	map_block Field_Map160, .LField_Map160_SCENE
.LField_Map160_METATILES:
	.incbin "map/map_160/metatiles.delta2.lz"
	.balign 4, 0
.LField_Map160_SHAPES:
	.incbin "map/map_160/shapes.table.lz"
	.balign 4, 0
.LField_Map160_GRID:
	.incbin "map/map_160/grid.parts.lz"
	.balign 4, 0
.LField_Map160_SHAPE_GRID:
	.incbin "map/map_160/shape_grid.plane.lz"
	.balign 4, 0
.LField_Map160_ANIMATION:
	.incbin "map/map_126/animation.script.lz"
	.balign 4, 0
.LField_Map160_BLEND:
	.incbin "map/map_160/blend.script.lz"
	.balign 4, 0
.LField_Map160_SCENE:
	.incbin "map/map_160/scene.parts.lz"
