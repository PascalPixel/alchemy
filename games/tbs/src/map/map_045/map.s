	.include "games/tbs/src/map/map.inc"
	.section .rodata
	.global Field_Map045
Field_Map045:
	map_header 0, 0, 64, 64, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 64, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map045, .LField_Map045_METATILES
	map_block Field_Map045, .LField_Map045_SHAPES
	map_block Field_Map045, .LField_Map045_GRID
	map_block Field_Map045
	map_block Field_Map045
	map_block Field_Map045, .LField_Map045_MARKERS
.LField_Map045_METATILES:
	.incbin "map/map_045/metatiles.delta0.lz"
	.balign 4, 0
.LField_Map045_SHAPES:
	.incbin "map/map_045/shapes.table.lz"
	.balign 4, 0
.LField_Map045_GRID:
	.incbin "map/map_045/grid.parts.lz"
	.balign 4, 0
.LField_Map045_MARKERS:
	.incbin "map/map_000/markers.table"
	.balign 4, 0
	.global Tileset_Set35Palette
Tileset_Set35Palette:
	.incbin "graphics/tileset/set_35/palette.gbapal.lz"
	.byte 0x05	@ PACKER: left in the buffer by Field_Map045+0x17b
	.global Tileset_Set35TilesA
Tileset_Set35TilesA:
	.incbin "graphics/tileset/set_35/tiles_a.4bpp.mtf"
	.byte 0x07, 0xf0	@ PACKER: left in the buffer by Tileset_Set34TilesA+0x2bae
	.global Tileset_Set35TilesB
Tileset_Set35TilesB:
	.incbin "graphics/tileset/set_35/tiles_b.4bpp.mtf"
	.byte 0x8f, 0x63	@ PACKER: left in the buffer by Tileset_Set35TilesA+0x2272
	.global Tileset_Set35TilesC
Tileset_Set35TilesC:
	.incbin "graphics/tileset/set_35/tiles_c.4bpp.mtf"
	.byte 0x6b	@ PACKER: left in the buffer by Tileset_Set35TilesA+0x286f
	.global Tileset_Set35TilesD
Tileset_Set35TilesD:
	.incbin "graphics/tileset/set_35/tiles_d.4bpp.mtf"
