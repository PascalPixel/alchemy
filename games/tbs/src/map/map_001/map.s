	.include "games/tbs/src/map/map.inc"
	.section .rodata
	.global Field_Map001
Field_Map001:
	map_header 0, 0, 64, 64, 3, 0, 0, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map001, .LField_Map001_METATILES
	map_block Field_Map001, .LField_Map001_SHAPES
	map_block Field_Map001, .LField_Map001_GRID
	map_block Field_Map001
	map_block Field_Map001
	map_block Field_Map001, .LField_Map001_MARKERS
.LField_Map001_METATILES:
	.incbin "games/common/src/map/map_001/metatiles.delta0.lz"
	.balign 4, 0
.LField_Map001_SHAPES:
	.incbin "games/common/src/map/map_001/shapes.table.lz"
	.balign 4, 0
.LField_Map001_GRID:
	.incbin "games/tbs/src/map/map_001/grid.parts.lz"
	.balign 4, 0
.LField_Map001_MARKERS:
	.incbin "games/tbs/src/map/map_000/markers.table"
	.balign 4, 0
	.global Tileset_BlankPalette
Tileset_BlankPalette:
	.incbin "games/common/src/graphics/tileset/set_68/palette.gbapal.lz"
	.byte 0x00	@ PACKER: left in the buffer by Field_Map001+0xb
	.global Tileset_BlankTilesA
Tileset_BlankTilesA:
	.incbin "games/common/src/graphics/tileset/set_68/tiles_a.4bpp.mtf"
	.byte 0xff, 0x00	@ PACKER: left in the buffer by Field_Map001+0x14e
	.global Tileset_BlankTilesB
Tileset_BlankTilesB:
	.incbin "games/common/src/graphics/tileset/set_68/tiles_b.4bpp.mtf"
	.byte 0x07	@ PACKER: left in the buffer by Tileset_BlankTilesA+0x13f
	.global Tileset_BlankTilesC
Tileset_BlankTilesC:
	.incbin "games/common/src/graphics/tileset/set_68/tiles_b.4bpp.mtf"
	.byte 0x07	@ PACKER: left in the buffer by Tileset_BlankTilesA+0x13f
	.global Tileset_BlankTilesD
Tileset_BlankTilesD:
	.incbin "games/common/src/graphics/tileset/set_68/tiles_b.4bpp.mtf"
	.byte 0x07	@ PACKER: left in the buffer by Tileset_BlankTilesA+0x13f
