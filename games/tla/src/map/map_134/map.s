	.include "games/tla/src/map/map.inc"
	.section .rodata
	.global Field_Map134
Field_Map134:
	map_header 0, 0, 128, 128, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map134, .LField_Map134_METATILES
	map_block Field_Map134, .LField_Map134_SHAPES
	map_block Field_Map134, .LField_Map134_GRID
	map_block Field_Map134, .LField_Map134_SHAPE_GRID
	map_block Field_Map134, .LField_Map134_ANIMATION
	map_block Field_Map134, .LField_Map134_BLEND
	map_block Field_Map134, .LField_Map134_SCENE
.LField_Map134_METATILES:
	.incbin "games/tla/src/map/map_134/metatiles.delta1.lz"
	.balign 4, 0
.LField_Map134_SHAPES:
	.incbin "games/tla/src/map/map_134/shapes.table.lz"
	.balign 4, 0
.LField_Map134_GRID:
	.incbin "games/tla/src/map/map_134/grid.parts.lz"
	.balign 4, 0
.LField_Map134_SHAPE_GRID:
	.incbin "games/tla/src/map/map_134/shape_grid.plane.lz"
	.balign 4, 0
.LField_Map134_ANIMATION:
	.incbin "games/tla/src/map/map_134/animation.script.lz"
	.balign 4, 0
.LField_Map134_BLEND:
	.incbin "games/tla/src/map/map_001/blend.script.lz"
	.balign 4, 0
.LField_Map134_SCENE:
	.incbin "games/tla/src/map/map_134/scene.parts.lz"
	.balign 4, 0
