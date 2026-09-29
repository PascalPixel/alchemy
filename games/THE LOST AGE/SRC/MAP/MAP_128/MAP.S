@ Field_Map128: a field map.
	.include "games/THE LOST AGE/SRC/MAP/MAP.INC"
	.section .rodata
	.global Field_Map128
Field_Map128:
	map_header 0, 0, 120, 120, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 120, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 120, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map128, .LField_Map128_METATILES
	map_block Field_Map128, .LField_Map128_SHAPES
	map_block Field_Map128, .LField_Map128_GRID
	map_block Field_Map128, .LField_Map128_SHAPE_GRID
	map_block Field_Map128, .LField_Map128_ANIMATION
	map_block Field_Map128
	map_block Field_Map128, .LField_Map128_SCENE
.LField_Map128_METATILES:
	.incbin "MAP/MAP_128/METATILES.delta1.lz"
	.balign 4, 0
.LField_Map128_SHAPES:
	.incbin "MAP/MAP_128/SHAPES.table.lz"
	.balign 4, 0
.LField_Map128_GRID:
	.incbin "MAP/MAP_128/GRID.parts.lz"
	.balign 4, 0
.LField_Map128_SHAPE_GRID:
	.incbin "MAP/MAP_128/SHAPE_GRID.table.lz"
	.balign 4, 0
.LField_Map128_ANIMATION:
	.incbin "MAP/MAP_128/ANIMATION.script.lz"
	.balign 4, 0
.LField_Map128_SCENE:
	.incbin "MAP/MAP_128/SCENE.parts.lz"
