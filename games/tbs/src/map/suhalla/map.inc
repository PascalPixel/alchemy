	.global Field_Suhalla
Field_Suhalla:
	map_header 0, 0, 48, 48, 2, 1, 3, 1, 1, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	map_offset Field_Suhalla, .LField_Suhalla_CELL_PICTURES
	map_offset Field_Suhalla, .LField_Suhalla_SHAPES
	map_offset Field_Suhalla, .LField_Suhalla_GRID
	map_offset Field_Suhalla, .LField_Suhalla_ANIMATION
	map_offset Field_Suhalla
	map_offset Field_Suhalla, .LField_Suhalla_MARKERS
.LField_Suhalla_CELL_PICTURES:
	.incbin "games/tbs/src/map/suhalla/cell_pictures.delta1.lz"
	.balign 4, 0
.LField_Suhalla_SHAPES:
	.incbin "games/tbs/src/map/suhalla/shapes.table.lz"
	.balign 4, 0
.LField_Suhalla_GRID:
	.incbin "games/tbs/src/map/suhalla/map.grid.lz"
	.balign 4, 0
.LField_Suhalla_ANIMATION:
	.incbin "games/tbs/src/map/suhalla/tileset_anims.script.lz"
	.balign 4, 0
.LField_Suhalla_MARKERS:
	treasure_spot 1, 1, 100
	treasure_spot 13, 4, 101
	treasure_spot 1, 14, 102
	treasure_spots_end
	.global Tileset_Set64Palette
Tileset_Set64Palette:
	.incbin "games/tbs/src/graphics/tileset/set_64/palette.gbapal.lz"
	.global Tileset_Set64TilesA
Tileset_Set64TilesA:
	.incbin "games/tbs/src/graphics/tileset/set_64/tiles_a.4bpp.mtf"
	.ifdef TBS_EDITION_JA
	.byte 0xd8	@ PACKER: left in the buffer by Field_SuhallaDesertSouth+0x2617
	.else
	.ifdef TBS_EDITION_DE
	.byte 0xd8	@ PACKER: left in the buffer by Field_SuhallaDesertSouth+0x2617
	.else
	.byte 0x80	@ PACKER: left in the buffer by Field_SuhallaDesertSouth+0x2617
	.endif
	.endif
	.global Tileset_Set64TilesB
Tileset_Set64TilesB:
	.incbin "games/tbs/src/graphics/tileset/set_64/tiles_b.4bpp.mtf"
	.global Tileset_Set64TilesC
Tileset_Set64TilesC:
	.incbin "games/tbs/src/graphics/tileset/set_64/tiles_c.4bpp.mtf"
	.byte 0x00, 0x14	@ PACKER: left in the buffer by Tileset_Set64TilesB+0x106a
	.global Tileset_Set64TilesD
Tileset_Set64TilesD:
	.incbin "games/tbs/src/graphics/tileset/set_64/tiles_d.4bpp.mtf"
	.byte 0x81, 0x79	@ PACKER: left in the buffer by Tileset_Set64TilesB+0x1a26
	.balign 4, 0
