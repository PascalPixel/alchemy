	.include "games/tbs/src/layout/layout.inc"
	.section .rodata
	.global Field_Map177
Field_Map177:
	map_header 0, 0, 64, 64, 3, 0, 0, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map177, .LField_Map177_METATILES
	map_block Field_Map177, .LField_Map177_SHAPES
	map_block Field_Map177, .LField_Map177_GRID
	map_block Field_Map177
	map_block Field_Map177
	map_block Field_Map177, .LField_Map177_MARKERS
.LField_Map177_METATILES:
	.incbin "games/tbs/src/layout/layout_001/metatiles.delta0.lz"
	.balign 4, 0
.LField_Map177_SHAPES:
	.incbin "games/tbs/src/layout/layout_001/shapes.table.lz"
	.balign 4, 0
.LField_Map177_GRID:
	.incbin "games/tbs/src/layout/layout_001/grid.parts.lz"
	.balign 4, 0
.LField_Map177_MARKERS:
	.incbin "games/tbs/src/layout/layout_000/markers.table"
	.balign 4, 0
	.global Tileset_Set73Palette
Tileset_Set73Palette:
	.incbin "games/tbs/src/graphics/tileset/set_68/palette.gbapal.lz"
	.byte 0x00	@ PACKER: left in the buffer by Field_Map177+0xb
	.global Tileset_Set73TilesA
Tileset_Set73TilesA:
	.incbin "games/tbs/src/graphics/tileset/set_68/tiles_a.4bpp.mtf"
	.byte 0xff, 0x00	@ PACKER: left in the buffer by Field_Map177+0x14e
	.global Tileset_Set73TilesB
Tileset_Set73TilesB:
	.incbin "games/tbs/src/graphics/tileset/set_68/tiles_b.4bpp.mtf"
	.byte 0x07	@ PACKER: left in the buffer by Tileset_Set73TilesA+0x13f
	.global Tileset_Set73TilesC
Tileset_Set73TilesC:
	.incbin "games/tbs/src/graphics/tileset/set_68/tiles_b.4bpp.mtf"
	.byte 0x07	@ PACKER: left in the buffer by Tileset_Set73TilesA+0x13f
	.global Tileset_Set73TilesD
Tileset_Set73TilesD:
	.incbin "games/tbs/src/graphics/tileset/set_68/tiles_b.4bpp.mtf"
	.byte 0x07	@ PACKER: left in the buffer by Tileset_Set73TilesA+0x13f
