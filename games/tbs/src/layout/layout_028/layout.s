	.include "games/tbs/src/layout/layout.inc"
	.section .rodata
	.global Field_Layout028
Field_Layout028:
	layout_header 0, 0, 42, 54, 3, 1, 2, 0, 2, 2, 0, 0
	layout_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	layout_layer 0, 120, 16, 16, 0, 0, 0xff, 0xff
	layout_layer 120, 0, 16, 16, 0, 0, 0xff, 0xff
	layout_offset Field_Layout028, .LField_Layout028_METATILES
	layout_offset Field_Layout028, .LField_Layout028_SHAPES
	layout_offset Field_Layout028, .LField_Layout028_GRID
	layout_offset Field_Layout028, .LField_Layout028_ANIMATION
	layout_offset Field_Layout028, .LField_Layout028_BLEND
	layout_offset Field_Layout028, .LField_Layout028_MARKERS
.LField_Layout028_METATILES:
	.incbin "games/tbs/src/layout/layout_028/metatiles.delta0.lz"
	.balign 4, 0
.LField_Layout028_SHAPES:
	.incbin "games/tbs/src/layout/layout_028/shapes.table.lz"
	.balign 4, 0
.LField_Layout028_GRID:
	.incbin "games/tbs/src/layout/layout_028/grid.parts.lz"
	.balign 4, 0
.LField_Layout028_ANIMATION:
	.incbin "games/tbs/src/layout/layout_028/animation.script.lz"
	.balign 4, 0
.LField_Layout028_BLEND:
	.incbin "games/tbs/src/layout/layout_020/blend.script.lz"
	.balign 4, 0
.LField_Layout028_MARKERS:
	.incbin "games/tbs/src/layout/layout_000/markers.table"
	.balign 4, 0
	.global Tileset_Set21Palette
Tileset_Set21Palette:
	.incbin "games/tbs/src/graphics/tileset/set_21/palette.gbapal.lz"
	.byte 0x98, 0x49	@ PACKER: left in the buffer by Field_Layout028+0x19a
	.global Tileset_Set21TilesA
Tileset_Set21TilesA:
	.incbin "games/tbs/src/graphics/tileset/set_21/tiles_a.4bpp.mtf"
	.byte 0xdc, 0x06, 0xaa	@ PACKER: left in the buffer by Field_Layout027+0x2bad
	.global Tileset_Set21TilesB
Tileset_Set21TilesB:
	.incbin "games/tbs/src/graphics/tileset/set_21/tiles_b.4bpp.mtf"
	.byte 0xf1, 0xe8, 0x05	@ PACKER: left in the buffer by Tileset_Set21TilesA+0x20d1
	.global Tileset_Set21TilesC
Tileset_Set21TilesC:
	.incbin "games/tbs/src/graphics/tileset/set_21/tiles_c.4bpp.mtf"
	.byte 0x83, 0x38	@ PACKER: left in the buffer by Tileset_Set21TilesB+0x2056
	.global Tileset_Set21TilesD
Tileset_Set21TilesD:
	.incbin "games/tbs/src/graphics/tileset/set_21/tiles_d.4bpp.mtf"
	.byte 0x7c, 0x2f	@ PACKER: left in the buffer by Tileset_Set21TilesC+0x1cea
