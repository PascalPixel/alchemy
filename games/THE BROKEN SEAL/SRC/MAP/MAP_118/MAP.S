	.include "games/COMMON/INCLUDE/GAME/ED_ASM.H"
@ Field_Map118: a field map.
	.include "games/THE BROKEN SEAL/SRC/MAP/MAP.INC"
	.section .rodata
	.global Field_Map118
Field_Map118:
	.ifeq EDITION_INTERNATIONAL
	map_header 0, 0, 120, 120, 3, 1, 2, 1, 1, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map118, .LField_Map118_METATILES
	map_block Field_Map118, .LField_Map118_SHAPES
	map_block Field_Map118, .LField_Map118_GRID
	map_block Field_Map118, .LField_Map118_ANIMATION
	map_block Field_Map118, .LField_Map118_BLEND
	map_block Field_Map118, .LField_Map118_MARKERS
.LField_Map118_METATILES:
	.incbin "MAP/MAP_118/METATILES_JA.delta1.lz"
	.balign 4, 0
.LField_Map118_SHAPES:
	.incbin "MAP/MAP_118/SHAPES.table.lz"
	.balign 4, 0
.LField_Map118_GRID:
	.incbin "MAP/MAP_118/GRID_JA.parts.lz"
	.balign 4, 0
.LField_Map118_ANIMATION:
	.incbin "MAP/MAP_077/ANIMATION.script.lz"
	.balign 4, 0
.LField_Map118_BLEND:
	.incbin "MAP/MAP_025/BLEND.script.lz"
	.balign 4, 0
.LField_Map118_MARKERS:
	.incbin "MAP/MAP_118/MARKERS_JA.table"
	.else
	map_header 0, 0, 120, 120, 3, 1, 2, 1, 1, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map118, .LField_Map118_METATILES
	map_block Field_Map118, .LField_Map118_SHAPES
	map_block Field_Map118, .LField_Map118_GRID
	map_block Field_Map118, .LField_Map118_ANIMATION
	map_block Field_Map118, .LField_Map118_BLEND
	map_block Field_Map118, .LField_Map118_MARKERS
.LField_Map118_METATILES:
	.incbin "MAP/MAP_118/METATILES.delta2.lz"
	.balign 4, 0
.LField_Map118_SHAPES:
	.incbin "MAP/MAP_118/SHAPES.table.lz"
	.balign 4, 0
.LField_Map118_GRID:
	.incbin "MAP/MAP_118/GRID.parts.lz"
	.balign 4, 0
.LField_Map118_ANIMATION:
	.incbin "MAP/MAP_077/ANIMATION.script.lz"
	.balign 4, 0
.LField_Map118_BLEND:
	.incbin "MAP/MAP_025/BLEND.script.lz"
	.balign 4, 0
.LField_Map118_MARKERS:
	.incbin "MAP/MAP_118/MARKERS.table"
	.endif
