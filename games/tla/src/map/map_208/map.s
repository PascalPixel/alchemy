	.include "games/tla/src/map/map.inc"
	.section .rodata
	.global Field_Map208
Field_Map208:
	map_header 0, 0, 128, 128, 2, 2, 3, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map208, .LField_Map208_METATILES
	map_block Field_Map208, .LField_Map208_SHAPES
	map_block Field_Map208, .LField_Map208_GRID
	map_block Field_Map208, .LField_Map208_SHAPE_GRID
	map_block Field_Map208, .LField_Map208_ANIMATION
	map_block Field_Map208, .LField_Map208_BLEND
	map_block Field_Map208, .LField_Map208_SCENE
.LField_Map208_METATILES:
	.incbin "games/tla/src/map/map_208/metatiles.delta1.lz"
	.balign 4, 0
.LField_Map208_SHAPES:
	.incbin "games/tla/src/map/map_208/shapes.table.lz"
	.balign 4, 0
.LField_Map208_GRID:
	.incbin "games/tla/src/map/map_208/grid.parts.lz"
	.balign 4, 0
.LField_Map208_SHAPE_GRID:
	.incbin "games/tla/src/map/map_208/shape_grid.plane.lz"
	.balign 4, 0
.LField_Map208_ANIMATION:
	.incbin "games/tla/src/map/map_012/animation.script.lz"
	.balign 4, 0
.LField_Map208_BLEND:
	.incbin "games/tla/src/map/map_001/blend.script.lz"
	.balign 4, 0
.LField_Map208_SCENE:
	.incbin "games/tla/src/map/map_208/scene.parts.lz"
	.balign 4, 0
