@ Field_Map282: a field map.
	.include "games/THE LOST AGE/SRC/MAP/MAP.INC"
	.section .rodata
	.global Field_Map282
Field_Map282:
	map_header 0, 0, 128, 128, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map282, .LField_Map282_METATILES
	map_block Field_Map282, .LField_Map282_SHAPES
	map_block Field_Map282, .LField_Map282_GRID
	map_block Field_Map282, .LField_Map282_SHAPE_GRID
	map_block Field_Map282, .LField_Map282_ANIMATION
	map_block Field_Map282, .LField_Map282_BLEND
	map_block Field_Map282, .LField_Map282_SCENE
.LField_Map282_METATILES:
	.incbin "MAP/MAP_282/METATILES.delta2.lz"
	.balign 4, 0
.LField_Map282_SHAPES:
	.incbin "MAP/MAP_282/SHAPES.table.lz"
	.balign 4, 0
.LField_Map282_GRID:
	.incbin "MAP/MAP_282/GRID.parts.lz"
	.balign 4, 0
.LField_Map282_SHAPE_GRID:
	.incbin "MAP/MAP_282/SHAPE_GRID.plane.lz"
	.balign 4, 0
.LField_Map282_ANIMATION:
	.incbin "MAP/MAP_012/ANIMATION.script.lz"
	.balign 4, 0
.LField_Map282_BLEND:
	.incbin "MAP/MAP_001/BLEND.script.lz"
	.balign 4, 0
.LField_Map282_SCENE:
	.incbin "MAP/MAP_282/SCENE.parts.lz"
