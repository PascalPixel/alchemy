@ Field_Map144: a field map.
	.include "games/THE LOST AGE/SRC/MAP/MAP.INC"
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
	.incbin "MAP/MAP_144/METATILES.delta1.lz"
	.balign 4, 0
.LField_Map144_SHAPES:
	.incbin "MAP/MAP_144/SHAPES.table.lz"
	.balign 4, 0
.LField_Map144_GRID:
	.incbin "MAP/MAP_144/GRID.parts.lz"
	.balign 4, 0
.LField_Map144_SHAPE_GRID:
	.incbin "MAP/MAP_144/SHAPE_GRID.table.lz"
	.balign 4, 0
.LField_Map144_ANIMATION:
	.incbin "MAP/MAP_144/ANIMATION.script.lz"
	.balign 4, 0
.LField_Map144_BLEND:
	.incbin "MAP/MAP_144/BLEND.script.lz"
	.balign 4, 0
.LField_Map144_SCENE:
	.incbin "MAP/MAP_144/SCENE.parts.lz"
