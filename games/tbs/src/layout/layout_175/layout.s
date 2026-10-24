	.include "games/tbs/src/layout/layout.inc"
	.section .rodata
	.global Field_Layout175
Field_Layout175:
	layout_header 0, 0, 64, 64, 3, 0, 0, 0, 2, 2, 0, 0
	layout_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	layout_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	layout_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	layout_offset Field_Layout175, .LField_Layout175_METATILES
	layout_offset Field_Layout175, .LField_Layout175_SHAPES
	layout_offset Field_Layout175, .LField_Layout175_GRID
	layout_offset Field_Layout175
	layout_offset Field_Layout175
	layout_offset Field_Layout175, .LField_Layout175_MARKERS
.LField_Layout175_METATILES:
	.incbin "games/tbs/src/layout/layout_001/metatiles.delta0.lz"
	.balign 4, 0
.LField_Layout175_SHAPES:
	.incbin "games/tbs/src/layout/layout_001/shapes.table.lz"
	.balign 4, 0
.LField_Layout175_GRID:
	.incbin "games/tbs/src/layout/layout_001/map.grid.lz"
	.balign 4, 0
.LField_Layout175_MARKERS:
	treasure_spots_end
	.balign 4, 0
	.global Tileset_Set71Palette
Tileset_Set71Palette:
	.incbin "games/tbs/src/graphics/tileset/set_68/palette.gbapal.lz"
	.byte 0x00	@ PACKER: left in the buffer by Field_Layout175+0xb
	.global Tileset_Set71TilesA
Tileset_Set71TilesA:
	.incbin "games/tbs/src/graphics/tileset/set_68/tiles_a.4bpp.mtf"
	.byte 0xff, 0x00	@ PACKER: left in the buffer by Field_Layout175+0x14e
	.global Tileset_Set71TilesB
Tileset_Set71TilesB:
	.incbin "games/tbs/src/graphics/tileset/set_68/tiles_b.4bpp.mtf"
	.byte 0x07	@ PACKER: left in the buffer by Tileset_Set71TilesA+0x13f
	.global Tileset_Set71TilesC
Tileset_Set71TilesC:
	.incbin "games/tbs/src/graphics/tileset/set_68/tiles_b.4bpp.mtf"
	.byte 0x07	@ PACKER: left in the buffer by Tileset_Set71TilesA+0x13f
	.global Tileset_Set71TilesD
Tileset_Set71TilesD:
	.incbin "games/tbs/src/graphics/tileset/set_68/tiles_b.4bpp.mtf"
	.byte 0x07	@ PACKER: left in the buffer by Tileset_Set71TilesA+0x13f
