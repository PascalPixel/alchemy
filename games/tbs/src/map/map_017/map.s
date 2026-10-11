	.include "games/tbs/src/map/map.inc"
	.section .rodata
	.global Field_Map017
Field_Map017:
	map_header 0, 0, 64, 64, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 64, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 64, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map017, .LField_Map017_METATILES
	map_block Field_Map017, .LField_Map017_SHAPES
	map_block Field_Map017, .LField_Map017_GRID
	map_block Field_Map017
	map_block Field_Map017
	map_block Field_Map017, .LField_Map017_MARKERS
.LField_Map017_METATILES:
	.incbin "map/map_017/metatiles.delta1.lz"
	.balign 4, 0
.LField_Map017_SHAPES:
	.incbin "map/map_017/shapes.table.lz"
	.balign 4, 0
.LField_Map017_GRID:
	.incbin "map/map_017/grid.parts.lz"
	.balign 4, 0
.LField_Map017_MARKERS:
	.incbin "map/map_017/markers.table"
	.balign 4, 0
	.global Tileset_Set14Palette
Tileset_Set14Palette:
	.incbin "graphics/tileset/set_14/palette.gbapal.lz"
	.byte 0xc8	@ PACKER: left in the buffer by Field_Map017+0x183
	.global Tileset_Set14TilesA
Tileset_Set14TilesA:
	.incbin "graphics/tileset/set_14/tiles_a.4bpp.mtf"
	.byte 0x8a, 0x33, 0x2a	@ PACKER: left in the buffer by Field_Map006+0x2b51
	.global Tileset_Set14TilesB
Tileset_Set14TilesB:
	.incbin "graphics/tileset/set_14/tiles_b.4bpp.mtf"
	.byte 0x13, 0x1d	@ PACKER: left in the buffer by Tileset_Set14TilesA+0x26c2
	.global Tileset_Set14TilesC
Tileset_Set14TilesC:
	.incbin "graphics/tileset/set_14/tiles_c.4bpp.mtf"
	.byte 0x0a, 0xcd	@ PACKER: left in the buffer by Tileset_Set14TilesA+0x2756
	.global Tileset_Set14TilesD
Tileset_Set14TilesD:
	.incbin "graphics/tileset/set_14/tiles_d.4bpp.mtf"
	.byte 0x5e, 0x11, 0x7d	@ PACKER: left in the buffer by Tileset_Set14TilesC+0x1851
