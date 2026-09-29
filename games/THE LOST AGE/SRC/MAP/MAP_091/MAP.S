@ Field_Map091: a field map.
	.include "games/THE LOST AGE/SRC/MAP/MAP.INC"
	.section .rodata
	.global Field_Map091
Field_Map091:
	map_header 0, 0, 128, 128, 2, 1, 3, 1, 1, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map091, .LField_Map091_METATILES
	map_block Field_Map091, .LField_Map091_SHAPES
	map_block Field_Map091, .LField_Map091_GRID
	map_block Field_Map091, .LField_Map091_SHAPE_GRID
	map_block Field_Map091, .LField_Map091_ANIMATION
	map_block Field_Map091, .LField_Map091_BLEND
	map_block Field_Map091, .LField_Map091_SCENE
.LField_Map091_METATILES:
	.incbin "MAP/MAP_091/METATILES.delta2.lz"
	.balign 4, 0
.LField_Map091_SHAPES:
	.incbin "MAP/MAP_091/SHAPES.table.lz"
	.balign 4, 0
.LField_Map091_GRID:
	.incbin "MAP/MAP_091/GRID.parts.lz"
	.balign 4, 0
.LField_Map091_SHAPE_GRID:
	.incbin "MAP/MAP_091/SHAPE_GRID.table.lz"
	.balign 4, 0
.LField_Map091_ANIMATION:
	.incbin "MAP/MAP_091/ANIMATION.script.lz"
	.balign 4, 0
.LField_Map091_BLEND:
	.incbin "MAP/MAP_091/BLEND.script.lz"
	.balign 4, 0
.LField_Map091_SCENE:
	.incbin "MAP/MAP_091/SCENE.parts.lz"
