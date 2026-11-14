	.global Field_LamakanDesertEast
Field_LamakanDesertEast:
	map_header 0, 0, 128, 128, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0x7f, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0x7f, 0xff
	map_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	map_offset Field_LamakanDesertEast, .LField_LamakanDesertEast_CELL_PICTURES
	map_offset Field_LamakanDesertEast, .LField_LamakanDesertEast_SHAPES
	map_offset Field_LamakanDesertEast, .LField_LamakanDesertEast_GRID
	map_offset Field_LamakanDesertEast, .LField_LamakanDesertEast_ANIMATION
	map_offset Field_LamakanDesertEast
	map_offset Field_LamakanDesertEast, .LField_LamakanDesertEast_MARKERS
.LField_LamakanDesertEast_CELL_PICTURES:
	.incbin "games/tbs/src/map/lamakan_desert_east/cell_pictures.delta1.lz"
	.balign 4, 0
.LField_LamakanDesertEast_SHAPES:
	.incbin "games/tbs/src/map/lamakan_desert_east/shapes.table.lz"
	.balign 4, 0
.LField_LamakanDesertEast_GRID:
	.incbin "games/tbs/src/map/lamakan_desert_east/map.grid.lz"
	.balign 4, 0
.LField_LamakanDesertEast_ANIMATION:
	.incbin "games/tbs/src/map/lamakan_desert_east/tileset_anims.script.lz"
	.balign 4, 0
.LField_LamakanDesertEast_MARKERS:
	treasure_spot 10, 10, 100
	treasure_spot 16, 55, 101
	treasure_spots_end
	.balign 4, 0
	.global Tileset_LamakanDesertEastPalette
Tileset_LamakanDesertEastPalette:
	.incbin "games/tbs/src/graphics/tileset/lamakan_desert_east/palette.gbapal.lz"
	.global Tileset_LamakanDesertEastTilesA
Tileset_LamakanDesertEastTilesA:
	.incbin "games/tbs/src/graphics/tileset/lamakan_desert_east/tiles_a.4bpp.mtf"
	.byte 0x54	@ PACKER: left in the buffer by Field_LamaTemple+0x28ef
	.global Tileset_LamakanDesertEastTilesB
Tileset_LamakanDesertEastTilesB:
	.incbin "games/tbs/src/graphics/tileset/lamakan_desert_east/tiles_b.4bpp.mtf"
	.byte 0x9c, 0x52	@ PACKER: left in the buffer by Tileset_LamakanDesertEastTilesA+0x220a
	.global Tileset_LamakanDesertEastTilesC
Tileset_LamakanDesertEastTilesC:
	.incbin "games/tbs/src/graphics/tileset/lamakan_desert_east/tiles_c.4bpp.mtf"
	.byte 0x96	@ PACKER: left in the buffer by Tileset_LamakanDesertEastTilesB+0x13f
	.global Tileset_LamakanDesertEastTilesD
Tileset_LamakanDesertEastTilesD:
	.incbin "games/tbs/src/graphics/tileset/lamakan_desert_east/tiles_d.4bpp.mtf"
	.byte 0xc0, 0x84	@ PACKER: left in the buffer by Tileset_LamakanDesertEastTilesB+0x1a16
	.balign 4, 0
