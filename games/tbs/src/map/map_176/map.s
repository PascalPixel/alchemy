	.include "games/tbs/src/map/map.inc"
	.section .rodata
	.global Field_Map176
Field_Map176:
	map_header 0, 0, 64, 64, 3, 0, 0, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map176, .LField_Map176_METATILES
	map_block Field_Map176, .LField_Map176_SHAPES
	map_block Field_Map176, .LField_Map176_GRID
	map_block Field_Map176
	map_block Field_Map176
	map_block Field_Map176, .LField_Map176_MARKERS
.LField_Map176_METATILES:
	.incbin "games/tbs/src/map/map_001/metatiles.delta0.lz"
	.balign 4, 0
.LField_Map176_SHAPES:
	.incbin "games/tbs/src/map/map_001/shapes.table.lz"
	.balign 4, 0
.LField_Map176_GRID:
	.incbin "games/tbs/src/map/map_001/grid.parts.lz"
	.balign 4, 0
.LField_Map176_MARKERS:
	.incbin "games/tbs/src/map/map_000/markers.table"
	.balign 4, 0
	.global Tileset_Set72Palette
Tileset_Set72Palette:
	.incbin "games/tbs/src/graphics/tileset/set_68/palette.gbapal.lz"
	.byte 0x00	@ PACKER: left in the buffer by Field_Map176+0xb
	.global Tileset_Set72TilesA
Tileset_Set72TilesA:
	.incbin "games/tbs/src/graphics/tileset/set_68/tiles_a.4bpp.mtf"
	.byte 0xff, 0x00	@ PACKER: left in the buffer by Field_Map176+0x14e
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
