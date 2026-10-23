	.include "games/tbs/src/layout/layout.inc"
	.section .rodata
	.global Field_Layout001
Field_Layout001:
	layout_header 0, 0, 64, 64, 3, 0, 0, 0, 2, 2, 0, 0
	layout_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	layout_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	layout_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	layout_offset Field_Layout001, .LField_Layout001_METATILES
	layout_offset Field_Layout001, .LField_Layout001_SHAPES
	layout_offset Field_Layout001, .LField_Layout001_GRID
	layout_offset Field_Layout001
	layout_offset Field_Layout001
	layout_offset Field_Layout001, .LField_Layout001_MARKERS
.LField_Layout001_METATILES:
	.incbin "games/tbs/src/layout/layout_001/metatiles.delta0.lz"
	.balign 4, 0
.LField_Layout001_SHAPES:
	.incbin "games/tbs/src/layout/layout_001/shapes.table.lz"
	.balign 4, 0
.LField_Layout001_GRID:
	.incbin "games/tbs/src/layout/layout_001/grid.parts.lz"
	.balign 4, 0
.LField_Layout001_MARKERS:
	.incbin "games/tbs/src/layout/layout_000/treasure_spots.table"
	.balign 4, 0
	.global Tileset_BlankPalette
Tileset_BlankPalette:
	.incbin "games/tbs/src/graphics/tileset/set_68/palette.gbapal.lz"
	.byte 0x00	@ PACKER: left in the buffer by Field_Layout001+0xb
	.global Tileset_BlankTilesA
Tileset_BlankTilesA:
	.incbin "games/tbs/src/graphics/tileset/set_68/tiles_a.4bpp.mtf"
	.byte 0xff, 0x00	@ PACKER: left in the buffer by Field_Layout001+0x14e
	.global Tileset_BlankTilesB
Tileset_BlankTilesB:
	.incbin "games/tbs/src/graphics/tileset/set_68/tiles_b.4bpp.mtf"
	.byte 0x07	@ PACKER: left in the buffer by Tileset_BlankTilesA+0x13f
	.global Tileset_BlankTilesC
Tileset_BlankTilesC:
	.incbin "games/tbs/src/graphics/tileset/set_68/tiles_b.4bpp.mtf"
	.byte 0x07	@ PACKER: left in the buffer by Tileset_BlankTilesA+0x13f
	.global Tileset_BlankTilesD
Tileset_BlankTilesD:
	.incbin "games/tbs/src/graphics/tileset/set_68/tiles_b.4bpp.mtf"
	.byte 0x07	@ PACKER: left in the buffer by Tileset_BlankTilesA+0x13f
