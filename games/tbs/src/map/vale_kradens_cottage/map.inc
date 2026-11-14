	.global Field_ValeKradensCottage
Field_ValeKradensCottage:
	map_header 0, 0, 60, 70, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 60, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 120, 0, 16, 16, 0, 0, 0xff, 0xff
	map_offset Field_ValeKradensCottage, .LField_ValeKradensCottage_CELL_PICTURES
	map_offset Field_ValeKradensCottage, .LField_ValeKradensCottage_SHAPES
	map_offset Field_ValeKradensCottage, .LField_ValeKradensCottage_GRID
	map_offset Field_ValeKradensCottage
	map_offset Field_ValeKradensCottage
	map_offset Field_ValeKradensCottage, .LField_ValeKradensCottage_MARKERS
.LField_ValeKradensCottage_CELL_PICTURES:
	.incbin "games/tbs/src/map/vale_kradens_cottage/cell_pictures.delta1.lz"
	.balign 4, 0
.LField_ValeKradensCottage_SHAPES:
	.incbin "games/tbs/src/map/vale_kradens_cottage/shapes.table.lz"
	.balign 4, 0
.LField_ValeKradensCottage_GRID:
	.incbin "games/tbs/src/map/vale_kradens_cottage/map.grid.lz"
	.balign 4, 0
.LField_ValeKradensCottage_MARKERS:
	treasure_spot 8, 12, 100
	treasure_spots_end
	.balign 4, 0
	.global Tileset_ValeKradensCottagePalette
Tileset_ValeKradensCottagePalette:
	.incbin "games/tbs/src/graphics/tileset/vale_kradens_cottage/palette.gbapal.lz"
	.byte 0xc1, 0xac, 0xce	@ PACKER: left in the buffer by Field_ValeKradensCottage+0x191
	.global Tileset_ValeKradensCottageTilesA
Tileset_ValeKradensCottageTilesA:
	.incbin "games/tbs/src/graphics/tileset/vale_kradens_cottage/tiles_a.4bpp.mtf"
	.byte 0x1e	@ PACKER: left in the buffer by Field_ValePlaza+0x283f
	.global Tileset_ValeKradensCottageTilesB
Tileset_ValeKradensCottageTilesB:
	.incbin "games/tbs/src/graphics/tileset/vale_kradens_cottage/tiles_b.4bpp.mtf"
	.byte 0xcb, 0x42, 0xda	@ PACKER: left in the buffer by Tileset_ValeKradensCottageTilesA+0x1e15
	.global Tileset_ValeKradensCottageTilesC
Tileset_ValeKradensCottageTilesC:
	.incbin "games/tbs/src/graphics/tileset/vale_kradens_cottage/tiles_c.4bpp.mtf"
	.byte 0xeb, 0x91	@ PACKER: left in the buffer by Tileset_ValeKradensCottageTilesA+0x26de
	.global Tileset_ValeKradensCottageTilesD
Tileset_ValeKradensCottageTilesD:
	.incbin "games/tbs/src/graphics/tileset/vale_kradens_cottage/tiles_d.4bpp.mtf"
	.byte 0xe1, 0xf0, 0xf8	@ PACKER: left in the buffer by Tileset_ValeKradensCottageTilesC+0x1145
	.balign 4, 0
