	.include "games/tbs/src/map/map.inc"
	.section .rodata
	.global Field_Map017
Field_Map017:
	map_header 0, 0, 64, 64, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 64, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 64, 16, 16, 0, 0, 0xff, 0xff
	map_offset Field_Map017, .LField_Map017_CELL_PICTURES
	map_offset Field_Map017, .LField_Map017_SHAPES
	map_offset Field_Map017, .LField_Map017_GRID
	map_offset Field_Map017
	map_offset Field_Map017
	map_offset Field_Map017, .LField_Map017_MARKERS
.LField_Map017_CELL_PICTURES:
	.incbin "games/tbs/src/map/map_017/cell_pictures.delta1.lz"
	.balign 4, 0
.LField_Map017_SHAPES:
	.incbin "games/tbs/src/map/map_017/shapes.table.lz"
	.balign 4, 0
.LField_Map017_GRID:
	.incbin "games/tbs/src/map/map_017/map.grid.lz"
	.balign 4, 0
.LField_Map017_MARKERS:
	treasure_spot 4, 9, 101
	treasure_spots_end
	.balign 4, 0
	.global Tileset_Set14Palette
Tileset_Set14Palette:
	.incbin "games/tbs/src/graphics/tileset/set_14/palette.gbapal.lz"
	.byte 0xc8	@ PACKER: left in the buffer by Field_Map017+0x183
	.global Tileset_Set14TilesA
Tileset_Set14TilesA:
	.incbin "games/tbs/src/graphics/tileset/set_14/tiles_a.4bpp.mtf"
	.byte 0x8a, 0x33, 0x2a	@ PACKER: left in the buffer by Field_Map006+0x2b51
	.global Tileset_Set14TilesB
Tileset_Set14TilesB:
	.incbin "games/tbs/src/graphics/tileset/set_14/tiles_b.4bpp.mtf"
	.byte 0x13, 0x1d	@ PACKER: left in the buffer by Tileset_Set14TilesA+0x26c2
	.global Tileset_Set14TilesC
Tileset_Set14TilesC:
	.incbin "games/tbs/src/graphics/tileset/set_14/tiles_c.4bpp.mtf"
	.byte 0x0a, 0xcd	@ PACKER: left in the buffer by Tileset_Set14TilesA+0x2756
	.global Tileset_Set14TilesD
Tileset_Set14TilesD:
	.incbin "games/tbs/src/graphics/tileset/set_14/tiles_d.4bpp.mtf"
	.byte 0x5e, 0x11, 0x7d	@ PACKER: left in the buffer by Tileset_Set14TilesC+0x1851
