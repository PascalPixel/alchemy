	.include "games/tbs/src/map/map.inc"
	.section .rodata
	.global Field_Map173
Field_Map173:
	map_header 0, 0, 64, 64, 3, 0, 0, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_offset Field_Map173, .LField_Map173_METATILES
	map_offset Field_Map173, .LField_Map173_SHAPES
	map_offset Field_Map173, .LField_Map173_GRID
	map_offset Field_Map173
	map_offset Field_Map173
	map_offset Field_Map173, .LField_Map173_MARKERS
.LField_Map173_METATILES:
	.incbin "games/tbs/src/map/map_001/metatiles.delta0.lz"
	.balign 4, 0
.LField_Map173_SHAPES:
	.incbin "games/tbs/src/map/map_001/shapes.table.lz"
	.balign 4, 0
.LField_Map173_GRID:
	.incbin "games/tbs/src/map/map_001/map.grid.lz"
	.balign 4, 0
.LField_Map173_MARKERS:
	treasure_spots_end
	.balign 4, 0
	.global Tileset_Set69Palette
Tileset_Set69Palette:
	.incbin "games/tbs/src/graphics/tileset/set_68/palette.gbapal.lz"
	.byte 0x00	@ PACKER: left in the buffer by Field_Map173+0xb
	.global Tileset_Set69TilesA
Tileset_Set69TilesA:
	.incbin "games/tbs/src/graphics/tileset/set_68/tiles_a.4bpp.mtf"
	.byte 0xff, 0x00	@ PACKER: left in the buffer by Field_Map173+0x14e
	.global Tileset_Set69TilesB
Tileset_Set69TilesB:
	.incbin "games/tbs/src/graphics/tileset/set_68/tiles_b.4bpp.mtf"
	.byte 0x07	@ PACKER: left in the buffer by Tileset_Set69TilesA+0x13f
	.global Tileset_Set69TilesC
Tileset_Set69TilesC:
	.incbin "games/tbs/src/graphics/tileset/set_68/tiles_b.4bpp.mtf"
	.byte 0x07	@ PACKER: left in the buffer by Tileset_Set69TilesA+0x13f
	.global Tileset_Set69TilesD
Tileset_Set69TilesD:
	.incbin "games/tbs/src/graphics/tileset/set_68/tiles_b.4bpp.mtf"
	.byte 0x07	@ PACKER: left in the buffer by Tileset_Set69TilesA+0x13f
