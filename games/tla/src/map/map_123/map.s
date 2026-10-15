	.include "games/tla/src/map/map.inc"
	.section .rodata
	.global Field_Map123
Field_Map123:
	map_header 0, 0, 96, 72, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 96, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 72, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map123, .LField_Map123_METATILES
	map_block Field_Map123, .LField_Map123_SHAPES
	map_block Field_Map123, .LField_Map123_GRID
	map_block Field_Map123, .LField_Map123_SHAPE_GRID
	map_block Field_Map123, .LField_Map123_ANIMATION
	map_block Field_Map123, .LField_Map123_BLEND
	map_block Field_Map123, .LField_Map123_SCENE
.LField_Map123_METATILES:
	.incbin "games/tla/src/map/map_123/metatiles.delta1.lz"
	.balign 4, 0
.LField_Map123_SHAPES:
	.incbin "games/tla/src/map/map_123/shapes.table.lz"
	.balign 4, 0
.LField_Map123_GRID:
	.incbin "games/tla/src/map/map_123/grid.parts.lz"
	.balign 4, 0
.LField_Map123_SHAPE_GRID:
	.incbin "games/tla/src/map/map_123/shape_grid.plane.lz"
	.balign 4, 0
.LField_Map123_ANIMATION:
	.incbin "games/tla/src/map/map_123/animation.script.lz"
	.balign 4, 0
.LField_Map123_BLEND:
	.incbin "games/tla/src/map/map_001/blend.script.lz"
	.balign 4, 0
.LField_Map123_SCENE:
	.incbin "games/tla/src/map/map_123/scene.parts.lz"
	.balign 4, 0
