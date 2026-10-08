	.include "games/tla/src/map/map.inc"
	.section .rodata
	.global Field_Map144
Field_Map144:
	map_header 0, 0, 88, 64, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map144, .LField_Map144_METATILES
	map_block Field_Map144, .LField_Map144_SHAPES
	map_block Field_Map144, .LField_Map144_GRID
	map_block Field_Map144, .LField_Map144_SHAPE_GRID
	map_block Field_Map144, .LField_Map144_ANIMATION
	map_block Field_Map144, .LField_Map144_BLEND
	map_block Field_Map144, .LField_Map144_SCENE
.LField_Map144_METATILES:
	.incbin "map/map_144/metatiles.delta1.lz"
	.balign 4, 0
.LField_Map144_SHAPES:
	.incbin "map/map_144/shapes.table.lz"
	.balign 4, 0
.LField_Map144_GRID:
	.incbin "map/map_144/grid.parts.lz"
	.balign 4, 0
.LField_Map144_SHAPE_GRID:
	.incbin "map/map_144/shape_grid.plane.lz"
	.balign 4, 0
.LField_Map144_ANIMATION:
	.incbin "map/map_144/animation.script.lz"
	.balign 4, 0
.LField_Map144_BLEND:
	.incbin "map/map_001/blend.script.lz"
	.balign 4, 0
.LField_Map144_SCENE:
	.incbin "map/map_144/scene.parts.lz"
