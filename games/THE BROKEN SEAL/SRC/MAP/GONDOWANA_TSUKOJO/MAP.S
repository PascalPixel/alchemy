	.include "games/THE BROKEN SEAL/SRC/MAP/MAP.INC"
	.section .rodata
	.global Field_GondowanaTsukojo
Field_GondowanaTsukojo:
	map_header 0, 0, 64, 64, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 64, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 64, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_GondowanaTsukojo, .LField_GondowanaTsukojo_METATILES
	map_block Field_GondowanaTsukojo, .LField_GondowanaTsukojo_SHAPES
	map_block Field_GondowanaTsukojo, .LField_GondowanaTsukojo_GRID
	map_block Field_GondowanaTsukojo, .LField_GondowanaTsukojo_ANIMATION
	map_block Field_GondowanaTsukojo, .LField_GondowanaTsukojo_BLEND
	map_block Field_GondowanaTsukojo, .LField_GondowanaTsukojo_MARKERS
.LField_GondowanaTsukojo_METATILES:
	.incbin "MAP/GONDOWANA_TSUKOJO/METATILES.delta1.lz"
	.balign 4, 0
.LField_GondowanaTsukojo_SHAPES:
	.incbin "MAP/GONDOWANA_TSUKOJO/SHAPES.table.lz"
	.balign 4, 0
.LField_GondowanaTsukojo_GRID:
	.incbin "MAP/GONDOWANA_TSUKOJO/GRID.parts.lz"
	.balign 4, 0
.LField_GondowanaTsukojo_ANIMATION:
	.incbin "MAP/GONDOWANA_TSUKOJO/ANIMATION.script.lz"
	.balign 4, 0
.LField_GondowanaTsukojo_BLEND:
	.incbin "MAP/MAP_020/BLEND.script.lz"
	.balign 4, 0
.LField_GondowanaTsukojo_MARKERS:
	.incbin "MAP/MAP_000/MARKERS.table"
	.balign 4, 0
