	.include "games/tbs/src/layout/layout.inc"
	.section .rodata
	.global Field_Layout044
Field_Layout044:
	layout_header 0, 0, 64, 64, 3, 2, 1, 0, 2, 2, 0, 0
	layout_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	layout_layer 64, 0, 16, 16, 0, 0, 0xff, 0xff
	layout_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	layout_offset Field_Layout044, .LField_Layout044_METATILES
	layout_offset Field_Layout044, .LField_Layout044_SHAPES
	layout_offset Field_Layout044, .LField_Layout044_GRID
	layout_offset Field_Layout044, .LField_Layout044_ANIMATION
	layout_offset Field_Layout044
	layout_offset Field_Layout044, .LField_Layout044_MARKERS
.LField_Layout044_METATILES:
	.incbin "games/tbs/src/layout/layout_044/metatiles.delta0.lz"
	.balign 4, 0
.LField_Layout044_SHAPES:
	.incbin "games/tbs/src/layout/layout_044/shapes.table.lz"
	.balign 4, 0
.LField_Layout044_GRID:
	.incbin "games/tbs/src/layout/layout_044/grid.parts.lz"
	.balign 4, 0
.LField_Layout044_ANIMATION:
	.incbin "games/tbs/src/layout/layout_044/animation.script.lz"
	.balign 4, 0
.LField_Layout044_MARKERS:
	.incbin "games/tbs/src/layout/layout_044/markers.table"
	.balign 4, 0
	.global Tileset_Set34Palette
Tileset_Set34Palette:
	.incbin "games/tbs/src/graphics/tileset/set_34/palette.gbapal.lz"
	.byte 0x6a, 0x7b	@ PACKER: left in the buffer by Field_Layout044+0x132
	.balign 4, 0
	.global Tileset_Set34TilesA
Tileset_Set34TilesA:
	.incbin "games/tbs/src/graphics/tileset/set_34/tiles_a.4bpp.mtf"
	.balign 4, 0	@ PACKER: zero fill, not an earlier stream's bytes
	.global Tileset_Set34TilesB
Tileset_Set34TilesB:
	.incbin "games/tbs/src/graphics/tileset/set_34/tiles_b.4bpp.mtf"
	.global Tileset_Set34TilesC
Tileset_Set34TilesC:
	.incbin "games/tbs/src/graphics/tileset/set_34/tiles_c.4bpp.mtf"
	.byte 0xd6, 0xb6, 0x08	@ PACKER: left in the buffer by Tileset_Set34TilesB+0x17c1
	.global Tileset_Set34TilesD
Tileset_Set34TilesD:
	.incbin "games/tbs/src/graphics/tileset/set_34/tiles_d.4bpp.mtf"
