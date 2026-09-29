@ Field_Map143: a field map.
	.include "games/THE BROKEN SEAL/SRC/MAP/MAP.INC"
	.section .rodata
	.global Field_Map143
Field_Map143:
	map_header 0, 0, 72, 72, 3, 1, 2, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 120, 16, 16, 0, 0, 0xff, 0xff
	map_layer 120, 0, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map143, .LField_Map143_METATILES
	map_block Field_Map143, .LField_Map143_SHAPES
	map_block Field_Map143, .LField_Map143_GRID
	map_block Field_Map143, .LField_Map143_ANIMATION
	map_block Field_Map143, .LField_Map143_BLEND
	map_block Field_Map143, .LField_Map143_MARKERS
.LField_Map143_METATILES:
	.incbin "MAP/MAP_143/METATILES.delta0.lz"
	.balign 4, 0
.LField_Map143_SHAPES:
	.incbin "MAP/MAP_143/SHAPES.table.lz"
	.balign 4, 0
.LField_Map143_GRID:
	.incbin "MAP/MAP_143/GRID.parts.lz"
	.balign 4, 0
.LField_Map143_ANIMATION:
	.incbin "MAP/MAP_028/ANIMATION.script.lz"
	.balign 4, 0
.LField_Map143_BLEND:
	.incbin "MAP/MAP_020/BLEND.script.lz"
	.balign 4, 0
.LField_Map143_MARKERS:
	.incbin "MAP/MAP_000/MARKERS.table"
