	.include "games/tla/src/map/map.inc"
	.section .rodata
	.global Field_Map004
Field_Map004:
	map_header 0, 0, 128, 128, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map004, .LField_Map004_METATILES
	map_block Field_Map004, .LField_Map004_SHAPES
	map_block Field_Map004, .LField_Map004_GRID
	map_block Field_Map004, .LField_Map004_SHAPE_GRID
	map_block Field_Map004, .LField_Map004_ANIMATION
	map_block Field_Map004, .LField_Map004_BLEND
	map_block Field_Map004, .LField_Map004_SCENE
.LField_Map004_METATILES:
	.incbin "games/tla/src/map/map_004/metatiles.delta0.lz"
	.balign 4, 0
.LField_Map004_SHAPES:
	.incbin "games/tla/src/map/map_004/shapes.table.lz"
	.balign 4, 0
.LField_Map004_GRID:
	.incbin "games/tla/src/map/map_004/grid.parts.lz"
	.balign 4, 0
.LField_Map004_SHAPE_GRID:
	.incbin "games/tla/src/map/map_004/shape_grid.plane.lz"
	.balign 4, 0
.LField_Map004_ANIMATION:
	.incbin "games/tla/src/map/map_004/animation.script.lz"
	.balign 4, 0
.LField_Map004_BLEND:
	.incbin "games/tla/src/map/map_001/blend.script.lz"
	.balign 4, 0
.LField_Map004_SCENE:
	.incbin "games/tla/src/map/map_000/scene.parts.lz"
	.balign 4, 0
