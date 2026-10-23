	.include "games/tbs/src/layout/layout.inc"
	.section .rodata
	.global Field_Layout176
Field_Layout176:
	layout_header 0, 0, 64, 64, 3, 0, 0, 0, 2, 2, 0, 0
	layout_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	layout_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	layout_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	layout_offset Field_Layout176, .LField_Layout176_METATILES
	layout_offset Field_Layout176, .LField_Layout176_SHAPES
	layout_offset Field_Layout176, .LField_Layout176_GRID
	layout_offset Field_Layout176
	layout_offset Field_Layout176
	layout_offset Field_Layout176, .LField_Layout176_MARKERS
.LField_Layout176_METATILES:
	.incbin "games/tbs/src/layout/layout_001/metatiles.delta0.lz"
	.balign 4, 0
.LField_Layout176_SHAPES:
	.incbin "games/tbs/src/layout/layout_001/shapes.table.lz"
	.balign 4, 0
.LField_Layout176_GRID:
	.incbin "games/tbs/src/layout/layout_001/map.grid.lz"
	.balign 4, 0
.LField_Layout176_MARKERS:
	.incbin "games/tbs/src/layout/layout_000/treasure_spots.table"
	.balign 4, 0
	.global Tileset_Set72Palette
Tileset_Set72Palette:
	.incbin "games/tbs/src/graphics/tileset/set_68/palette.gbapal.lz"
	.byte 0x00	@ PACKER: left in the buffer by Field_Layout176+0xb
	.global Tileset_Set72TilesA
Tileset_Set72TilesA:
	.incbin "games/tbs/src/graphics/tileset/set_68/tiles_a.4bpp.mtf"
	.byte 0xff, 0x00	@ PACKER: left in the buffer by Field_Layout176+0x14e
	.global Tileset_Set72TilesB
Tileset_Set72TilesB:
	.incbin "games/tbs/src/graphics/tileset/set_68/tiles_b.4bpp.mtf"
	.byte 0x07	@ PACKER: left in the buffer by Tileset_Set72TilesA+0x13f
	.global Tileset_Set72TilesC
Tileset_Set72TilesC:
	.incbin "games/tbs/src/graphics/tileset/set_68/tiles_b.4bpp.mtf"
	.byte 0x07	@ PACKER: left in the buffer by Tileset_Set72TilesA+0x13f
	.global Tileset_Set72TilesD
Tileset_Set72TilesD:
	.incbin "games/tbs/src/graphics/tileset/set_68/tiles_b.4bpp.mtf"
	.byte 0x07	@ PACKER: left in the buffer by Tileset_Set72TilesA+0x13f
