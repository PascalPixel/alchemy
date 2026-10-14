	.include "games/tla/src/map/map.inc"
	.section .rodata
	.global Field_Map152
Field_Map152:
	map_header 0, 0, 130, 112, 3, 2, 2, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map152, .LField_Map152_METATILES
	map_block Field_Map152, .LField_Map152_SHAPES
	map_block Field_Map152, .LField_Map152_GRID
	map_block Field_Map152, .LField_Map152_SHAPE_GRID
	map_block Field_Map152, .LField_Map152_ANIMATION
	map_block Field_Map152, .LField_Map152_BLEND
	map_block Field_Map152, .LField_Map152_SCENE
.LField_Map152_METATILES:
	.incbin "games/tla/src/map/map_152/metatiles.delta2.lz"
	.balign 4, 0
.LField_Map152_SHAPES:
	.incbin "games/tla/src/map/map_152/shapes.table.lz"
	.balign 4, 0
.LField_Map152_GRID:
	.incbin "games/tla/src/map/map_152/grid.parts.lz"
	.balign 4, 0
.LField_Map152_SHAPE_GRID:
	.incbin "games/tla/src/map/map_152/shape_grid.plane.lz"
	.balign 4, 0
.LField_Map152_ANIMATION:
	.incbin "games/tla/src/map/map_152/animation.script.lz"
	.balign 4, 0
.LField_Map152_BLEND:
	.incbin "games/tla/src/map/map_150/blend.script.lz"
	.balign 4, 0
.LField_Map152_SCENE:
	.incbin "games/tla/src/map/map_152/scene.parts.lz"
