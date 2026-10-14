	.include "games/tla/src/map/map.inc"
	.section .rodata
	.global Field_Map097
Field_Map097:
	map_header 0, 0, 0, 64, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 192, 64, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 128, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map097, .LField_Map097_METATILES
	map_block Field_Map097, .LField_Map097_SHAPES
	map_block Field_Map097, .LField_Map097_GRID
	map_block Field_Map097, .LField_Map097_SHAPE_GRID
	map_block Field_Map097, .LField_Map097_ANIMATION
	map_block Field_Map097, .LField_Map097_BLEND
	map_block Field_Map097, .LField_Map097_SCENE
.LField_Map097_METATILES:
	.incbin "games/tla/src/map/map_097/metatiles.delta0.lz"
	.balign 4, 0
.LField_Map097_SHAPES:
	.incbin "games/tla/src/map/map_097/shapes.table.lz"
	.balign 4, 0
.LField_Map097_GRID:
	.incbin "games/tla/src/map/map_097/grid.parts.lz"
	.balign 4, 0
.LField_Map097_SHAPE_GRID:
	.incbin "games/tla/src/map/map_097/shape_grid.plane.lz"
	.balign 4, 0
.LField_Map097_ANIMATION:
	.incbin "games/tla/src/map/map_097/animation.script.lz"
	.balign 4, 0
.LField_Map097_BLEND:
	.incbin "games/tla/src/map/map_001/blend.script.lz"
	.balign 4, 0
.LField_Map097_SCENE:
	.incbin "games/tla/src/map/map_097/scene.parts.lz"
