@ Field_Map140: a field map.
	.include "games/THE LOST AGE/SRC/MAP/MAP.INC"
	.section .rodata
	.global Field_Map140
Field_Map140:
	map_header 0, 0, 90, 100, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 100, 16, 16, 0, 0, 0xff, 0xff
	map_layer 100, 0, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map140, .LField_Map140_METATILES
	map_block Field_Map140, .LField_Map140_SHAPES
	map_block Field_Map140, .LField_Map140_GRID
	map_block Field_Map140, .LField_Map140_SHAPE_GRID
	map_block Field_Map140, .LField_Map140_ANIMATION
	map_block Field_Map140, .LField_Map140_BLEND
	map_block Field_Map140, .LField_Map140_SCENE
.LField_Map140_METATILES:
	.incbin "MAP/MAP_140/METATILES.delta2.lz"
	.balign 4, 0
.LField_Map140_SHAPES:
	.incbin "MAP/MAP_140/SHAPES.table.lz"
	.balign 4, 0
.LField_Map140_GRID:
	.incbin "MAP/MAP_140/GRID.parts.lz"
	.balign 4, 0
.LField_Map140_SHAPE_GRID:
	.incbin "MAP/MAP_140/SHAPE_GRID.table.lz"
	.balign 4, 0
.LField_Map140_ANIMATION:
	.incbin "MAP/MAP_140/ANIMATION.script.lz"
	.balign 4, 0
.LField_Map140_BLEND:
	.incbin "MAP/MAP_140/BLEND.script.lz"
	.balign 4, 0
.LField_Map140_SCENE:
	.incbin "MAP/MAP_140/SCENE.parts.lz"
