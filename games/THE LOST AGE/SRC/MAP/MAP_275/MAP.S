@ Field_Map275: a field map.
	.include "games/THE LOST AGE/SRC/MAP/MAP.INC"
	.section .rodata
	.global Field_Map275
Field_Map275:
	map_header 0, 0, 204, 64, 3, 3, 2, 0, 2, 2, 0, 0
	map_layer 0, 0, 12, 12, -2, -2, 0xf, 0xf
	map_layer 0, 64, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map275, .LField_Map275_METATILES
	map_block Field_Map275, .LField_Map275_SHAPES
	map_block Field_Map275, .LField_Map275_GRID
	map_block Field_Map275, .LField_Map275_SHAPE_GRID
	map_block Field_Map275, .LField_Map275_ANIMATION
	map_block Field_Map275
	map_block Field_Map275, .LField_Map275_SCENE
.LField_Map275_METATILES:
	.incbin "MAP/MAP_275/METATILES.delta1.lz"
	.balign 4, 0
.LField_Map275_SHAPES:
	.incbin "MAP/MAP_275/SHAPES.table.lz"
	.balign 4, 0
.LField_Map275_GRID:
	.incbin "MAP/MAP_275/GRID.parts.lz"
	.balign 4, 0
.LField_Map275_SHAPE_GRID:
	.incbin "MAP/MAP_275/SHAPE_GRID.table.lz"
	.balign 4, 0
.LField_Map275_ANIMATION:
	.incbin "MAP/MAP_275/ANIMATION.script.lz"
	.balign 4, 0
.LField_Map275_SCENE:
	.incbin "MAP/MAP_275/SCENE.parts.lz"
