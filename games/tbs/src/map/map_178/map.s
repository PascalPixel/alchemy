	.include "games/tbs/src/map/map.inc"
	.section .rodata
	.global Field_Map178
Field_Map178:
	map_header 0, 0, 64, 64, 3, 0, 0, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map178, .LField_Map178_METATILES
	map_block Field_Map178, .LField_Map178_SHAPES
	map_block Field_Map178, .LField_Map178_GRID
	map_block Field_Map178
	map_block Field_Map178
	map_block Field_Map178, .LField_Map178_MARKERS
.LField_Map178_METATILES:
	.incbin "common/map/map_001/metatiles.delta0.lz"
	.balign 4, 0
.LField_Map178_SHAPES:
	.incbin "common/map/map_001/shapes.table.lz"
	.balign 4, 0
.LField_Map178_GRID:
	.incbin "map/map_001/grid.parts.lz"
	.balign 4, 0
.LField_Map178_MARKERS:
	.incbin "map/map_000/markers.table"
	.balign 4, 0
	.global Tileset_Set74Palette
Tileset_Set74Palette:
	.incbin "graphics/tileset/set_68/palette.gbapal.lz"
	.byte 0x00	@ PACKER: left in the buffer by Field_Map178+0xb
	.global Tileset_Set74TilesA
Tileset_Set74TilesA:
	.incbin "graphics/tileset/set_68/tiles_a.4bpp.mtf"
	.byte 0xff, 0x00	@ PACKER: left in the buffer by Field_Map178+0x14e
	.global Tileset_Set74TilesB
Tileset_Set74TilesB:
	.incbin "graphics/tileset/set_68/tiles_b.4bpp.mtf"
	.byte 0x07	@ PACKER: left in the buffer by Tileset_Set74TilesA+0x13f
	.global Tileset_Set74TilesC
Tileset_Set74TilesC:
	.incbin "graphics/tileset/set_68/tiles_b.4bpp.mtf"
	.byte 0x07	@ PACKER: left in the buffer by Tileset_Set74TilesA+0x13f
	.global Tileset_Set74TilesD
Tileset_Set74TilesD:
	.incbin "graphics/tileset/set_68/tiles_b.4bpp.mtf"
	.byte 0x07	@ PACKER: left in the buffer by Tileset_Set74TilesA+0x13f
