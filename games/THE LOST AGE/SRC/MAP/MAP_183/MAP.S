@ Field_Map183: a field map.
	.include "games/THE LOST AGE/SRC/MAP/MAP.INC"
	.section .rodata
	.global Field_Map183
Field_Map183:
	map_header 0, 0, 128, 128, 3, 1, 2, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map183, .LField_Map183_METATILES
	map_block Field_Map183, .LField_Map183_SHAPES
	map_block Field_Map183, .LField_Map183_GRID
	map_block Field_Map183, .LField_Map183_SHAPE_GRID
	map_block Field_Map183, .LField_Map183_ANIMATION
	map_block Field_Map183, .LField_Map183_BLEND
	map_block Field_Map183, .LField_Map183_SCENE
.LField_Map183_METATILES:
	.incbin "MAP/MAP_183/METATILES.delta1.lz"
	.balign 4, 0
.LField_Map183_SHAPES:
	.incbin "MAP/MAP_183/SHAPES.table.lz"
	.balign 4, 0
.LField_Map183_GRID:
	.incbin "MAP/MAP_183/GRID.parts.lz"
	.balign 4, 0
.LField_Map183_SHAPE_GRID:
	.incbin "MAP/MAP_183/SHAPE_GRID.table.lz"
	.balign 4, 0
.LField_Map183_ANIMATION:
	.incbin "MAP/MAP_183/ANIMATION.script.lz"
	.balign 4, 0
.LField_Map183_BLEND:
	.incbin "MAP/MAP_183/BLEND.script.lz"
	.balign 4, 0
.LField_Map183_SCENE:
	.incbin "MAP/MAP_183/SCENE.parts.lz"
