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
	.global Tileset_SuhallaPalette
Tileset_SuhallaPalette:
	.incbin "games/tbs/src/graphics/tileset/suhalla/palette.gbapal.lz"
	.global Tileset_SuhallaTilesA
Tileset_SuhallaTilesA:
	.incbin "games/tbs/src/graphics/tileset/suhalla/tiles_a.4bpp.mtf"
	.ifdef TBS_EDITION_JA
	.byte 0xd8	@ PACKER: left in the buffer by Field_SuhallaDesertSouth+0x2617
	.else
	.ifdef TBS_EDITION_DE
	.byte 0xd8	@ PACKER: left in the buffer by Field_SuhallaDesertSouth+0x2617
	.else
	.byte 0x80	@ PACKER: left in the buffer by Field_SuhallaDesertSouth+0x2617
	.endif
	.endif
	.global Tileset_SuhallaTilesB
Tileset_SuhallaTilesB:
	.incbin "games/tbs/src/graphics/tileset/suhalla/tiles_b.4bpp.mtf"
	.global Tileset_SuhallaTilesC
Tileset_SuhallaTilesC:
	.incbin "games/tbs/src/graphics/tileset/suhalla/tiles_c.4bpp.mtf"
	.byte 0x00, 0x14	@ PACKER: left in the buffer by Tileset_SuhallaTilesB+0x106a
	.global Tileset_SuhallaTilesD
Tileset_SuhallaTilesD:
	.incbin "games/tbs/src/graphics/tileset/suhalla/tiles_d.4bpp.mtf"
	.byte 0x81, 0x79	@ PACKER: left in the buffer by Tileset_SuhallaTilesB+0x1a26
	.balign 4, 0
