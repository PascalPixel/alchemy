	.include "games/THE BROKEN SEAL/SRC/MAP/MAP.INC"
	.section .rodata
	.global Field_KaragoruDou
Field_KaragoruDou:
	map_header 0, 0, 64, 64, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 64, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 64, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_KaragoruDou, .LField_KaragoruDou_METATILES
	map_block Field_KaragoruDou, .LField_KaragoruDou_SHAPES
	map_block Field_KaragoruDou, .LField_KaragoruDou_GRID
	map_block Field_KaragoruDou, .LField_KaragoruDou_ANIMATION
	map_block Field_KaragoruDou, .LField_KaragoruDou_BLEND
	map_block Field_KaragoruDou, .LField_KaragoruDou_MARKERS
.LField_KaragoruDou_METATILES:
	.incbin "MAP/KARAGORU_DOU/METATILES.delta1.lz"
	.balign 4, 0
.LField_KaragoruDou_SHAPES:
	.incbin "MAP/KARAGORU_DOU/SHAPES.table.lz"
	.balign 4, 0
.LField_KaragoruDou_GRID:
	.incbin "MAP/KARAGORU_DOU/GRID.parts.lz"
	.balign 4, 0
.LField_KaragoruDou_ANIMATION:
	.incbin "MAP/KARAGORU_DOU/ANIMATION.script.lz"
	.balign 4, 0
.LField_KaragoruDou_BLEND:
	.incbin "MAP/MAP_020/BLEND.script.lz"
	.balign 4, 0
.LField_KaragoruDou_MARKERS:
	.incbin "MAP/MAP_000/MARKERS.table"
	.balign 4, 0
