	.include "games/tla/src/map/map.inc"
	.section .rodata
	.global Field_Map177
Field_Map177:
	map_header 0, 0, 128, 128, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map177, .LField_Map177_METATILES
	map_block Field_Map177, .LField_Map177_SHAPES
	map_block Field_Map177, .LField_Map177_GRID
	map_block Field_Map177, .LField_Map177_SHAPE_GRID
	map_block Field_Map177, .LField_Map177_ANIMATION
	map_block Field_Map177, .LField_Map177_BLEND
	map_block Field_Map177, .LField_Map177_SCENE
.LField_Map177_METATILES:
	.incbin "games/tla/src/map/map_177/metatiles.delta1.lz"
	.balign 4, 0
.LField_Map177_SHAPES:
	.incbin "games/tla/src/map/map_177/shapes.table.lz"
	.balign 4, 0
.LField_Map177_GRID:
	.incbin "games/tla/src/map/map_177/grid.parts.lz"
	.balign 4, 0
.LField_Map177_SHAPE_GRID:
	.incbin "games/tla/src/map/map_177/shape_grid.plane.lz"
	.balign 4, 0
.LField_Map177_ANIMATION:
	.incbin "games/tla/src/map/map_177/animation.script.lz"
	.balign 4, 0
.LField_Map177_BLEND:
	.incbin "games/tla/src/map/map_057/blend.script.lz"
	.balign 4, 0
.LField_Map177_SCENE:
	.incbin "games/tla/src/map/map_177/scene.parts.lz"
