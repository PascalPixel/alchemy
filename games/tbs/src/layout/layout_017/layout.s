	.include "games/tbs/src/layout/layout.inc"
	.section .rodata
	.global Field_Layout017
Field_Layout017:
	layout_header 0, 0, 64, 64, 3, 2, 1, 0, 2, 2, 0, 0
	layout_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	layout_layer 64, 0, 16, 16, 0, 0, 0xff, 0xff
	layout_layer 0, 64, 16, 16, 0, 0, 0xff, 0xff
	layout_offset Field_Layout017, .LField_Layout017_METATILES
	layout_offset Field_Layout017, .LField_Layout017_SHAPES
	layout_offset Field_Layout017, .LField_Layout017_GRID
	layout_offset Field_Layout017
	layout_offset Field_Layout017
	layout_offset Field_Layout017, .LField_Layout017_MARKERS
.LField_Layout017_METATILES:
	.incbin "games/tbs/src/layout/layout_017/metatiles.delta1.lz"
	.balign 4, 0
.LField_Layout017_SHAPES:
	.incbin "games/tbs/src/layout/layout_017/shapes.table.lz"
	.balign 4, 0
.LField_Layout017_GRID:
	.incbin "games/tbs/src/layout/layout_017/grid.parts.lz"
	.balign 4, 0
.LField_Layout017_MARKERS:
	.incbin "games/tbs/src/layout/layout_017/markers.table"
	.balign 4, 0
	.global Tileset_Set14Palette
Tileset_Set14Palette:
	.incbin "games/tbs/src/graphics/tileset/set_14/palette.gbapal.lz"
	.byte 0xc8	@ PACKER: left in the buffer by Field_Layout017+0x183
	.global Tileset_Set14TilesA
Tileset_Set14TilesA:
	.incbin "games/tbs/src/graphics/tileset/set_14/tiles_a.4bpp.mtf"
	.byte 0x8a, 0x33, 0x2a	@ PACKER: left in the buffer by Field_Layout006+0x2b51
	.global Tileset_Set14TilesB
Tileset_Set14TilesB:
	.incbin "games/tbs/src/graphics/tileset/set_14/tiles_b.4bpp.mtf"
	.byte 0x13, 0x1d	@ PACKER: left in the buffer by Tileset_Set14TilesA+0x26c2
	.global Tileset_Set14TilesC
Tileset_Set14TilesC:
	.incbin "games/tbs/src/graphics/tileset/set_14/tiles_c.4bpp.mtf"
	.byte 0x0a, 0xcd	@ PACKER: left in the buffer by Tileset_Set14TilesA+0x2756
	.global Tileset_Set14TilesD
Tileset_Set14TilesD:
	.incbin "games/tbs/src/graphics/tileset/set_14/tiles_d.4bpp.mtf"
	.byte 0x5e, 0x11, 0x7d	@ PACKER: left in the buffer by Tileset_Set14TilesC+0x1851
