	.global Field_McCoyPalaceGrounds
Field_McCoyPalaceGrounds:
	.ifdef TBS_EDITION_ES
	map_header 0, 0, 64, 48, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 64, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 48, 16, 16, 0, 0, 0xff, 0xff
	map_offset Field_McCoyPalaceGrounds, .LField_McCoyPalaceGrounds_CELL_PICTURES
	map_offset Field_McCoyPalaceGrounds, .LField_McCoyPalaceGrounds_SHAPES
	map_offset Field_McCoyPalaceGrounds, .LField_McCoyPalaceGrounds_GRID
	map_offset Field_McCoyPalaceGrounds
	map_offset Field_McCoyPalaceGrounds
	map_offset Field_McCoyPalaceGrounds, .LField_McCoyPalaceGrounds_MARKERS
.LField_McCoyPalaceGrounds_CELL_PICTURES:
	.incbin "games/tbs/src/map/mccoy_palace_grounds/cell_pictures_es.delta1.lz"
	.balign 4, 0
.LField_McCoyPalaceGrounds_SHAPES:
	.incbin "games/tbs/src/map/mccoy_palace_grounds/shapes.table.lz"
	.balign 4, 0
.LField_McCoyPalaceGrounds_GRID:
	.incbin "games/tbs/src/map/mccoy_palace_grounds/map_es.grid.lz"
	.balign 4, 0
.LField_McCoyPalaceGrounds_MARKERS:
	treasure_spots_end
	.else
	.ifdef TBS_EDITION_FR
	map_header 0, 0, 64, 48, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 64, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 48, 16, 16, 0, 0, 0xff, 0xff
	map_offset Field_McCoyPalaceGrounds, .LField_McCoyPalaceGrounds_CELL_PICTURES
	map_offset Field_McCoyPalaceGrounds, .LField_McCoyPalaceGrounds_SHAPES
	map_offset Field_McCoyPalaceGrounds, .LField_McCoyPalaceGrounds_GRID
	map_offset Field_McCoyPalaceGrounds
	map_offset Field_McCoyPalaceGrounds
	map_offset Field_McCoyPalaceGrounds, .LField_McCoyPalaceGrounds_MARKERS
.LField_McCoyPalaceGrounds_CELL_PICTURES:
	.incbin "games/tbs/src/map/mccoy_palace_grounds/cell_pictures_es.delta1.lz"
	.balign 4, 0
.LField_McCoyPalaceGrounds_SHAPES:
	.incbin "games/tbs/src/map/mccoy_palace_grounds/shapes.table.lz"
	.balign 4, 0
.LField_McCoyPalaceGrounds_GRID:
	.incbin "games/tbs/src/map/mccoy_palace_grounds/map_es.grid.lz"
	.balign 4, 0
.LField_McCoyPalaceGrounds_MARKERS:
	treasure_spots_end
	.else
	.ifdef TBS_EDITION_IT
	map_header 0, 0, 64, 48, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 64, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 48, 16, 16, 0, 0, 0xff, 0xff
	map_offset Field_McCoyPalaceGrounds, .LField_McCoyPalaceGrounds_CELL_PICTURES
	map_offset Field_McCoyPalaceGrounds, .LField_McCoyPalaceGrounds_SHAPES
	map_offset Field_McCoyPalaceGrounds, .LField_McCoyPalaceGrounds_GRID
	map_offset Field_McCoyPalaceGrounds
	map_offset Field_McCoyPalaceGrounds
	map_offset Field_McCoyPalaceGrounds, .LField_McCoyPalaceGrounds_MARKERS
.LField_McCoyPalaceGrounds_CELL_PICTURES:
	.incbin "games/tbs/src/map/mccoy_palace_grounds/cell_pictures_es.delta1.lz"
	.balign 4, 0
.LField_McCoyPalaceGrounds_SHAPES:
	.incbin "games/tbs/src/map/mccoy_palace_grounds/shapes.table.lz"
	.balign 4, 0
.LField_McCoyPalaceGrounds_GRID:
	.incbin "games/tbs/src/map/mccoy_palace_grounds/map_es.grid.lz"
	.balign 4, 0
.LField_McCoyPalaceGrounds_MARKERS:
	treasure_spots_end
	.else
	map_header 0, 0, 64, 48, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 64, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 48, 16, 16, 0, 0, 0xff, 0xff
	map_offset Field_McCoyPalaceGrounds, .LField_McCoyPalaceGrounds_CELL_PICTURES
	map_offset Field_McCoyPalaceGrounds, .LField_McCoyPalaceGrounds_SHAPES
	map_offset Field_McCoyPalaceGrounds, .LField_McCoyPalaceGrounds_GRID
	map_offset Field_McCoyPalaceGrounds
	map_offset Field_McCoyPalaceGrounds
	map_offset Field_McCoyPalaceGrounds, .LField_McCoyPalaceGrounds_MARKERS
.LField_McCoyPalaceGrounds_CELL_PICTURES:
	.incbin "games/tbs/src/map/mccoy_palace_grounds/cell_pictures.delta1.lz"
	.balign 4, 0
.LField_McCoyPalaceGrounds_SHAPES:
	.incbin "games/tbs/src/map/mccoy_palace_grounds/shapes.table.lz"
	.balign 4, 0
.LField_McCoyPalaceGrounds_GRID:
	.incbin "games/tbs/src/map/mccoy_palace_grounds/map.grid.lz"
	.balign 4, 0
.LField_McCoyPalaceGrounds_MARKERS:
	treasure_spots_end
	.endif
	.endif
	.endif
	.balign 4, 0
	.global Tileset_McCoyPalaceGroundsPalette
Tileset_McCoyPalaceGroundsPalette:
	.incbin "games/tbs/src/graphics/tileset/mccoy_palace_grounds/palette.gbapal.lz"
	.byte 0x8c, 0xda, 0xd6	@ PACKER: left in the buffer by Field_McCoyPalaceGrounds+0x1a9
	.global Tileset_McCoyPalaceGroundsTilesA
Tileset_McCoyPalaceGroundsTilesA:
	.incbin "games/tbs/src/graphics/tileset/mccoy_palace_grounds/tiles_a.4bpp.mtf"
	.byte 0x60, 0xcf, 0x83	@ PACKER: left in the buffer by Tileset_BilibinTilesA+0x2d91
	.global Tileset_McCoyPalaceGroundsTilesB
Tileset_McCoyPalaceGroundsTilesB:
	.incbin "games/tbs/src/graphics/tileset/mccoy_palace_grounds/tiles_b.4bpp.mtf"
	.global Tileset_McCoyPalaceGroundsTilesC
Tileset_McCoyPalaceGroundsTilesC:
	.incbin "games/tbs/src/graphics/tileset/mccoy_palace_grounds/tiles_c.4bpp.mtf"
	.byte 0xb0, 0x3e	@ PACKER: left in the buffer by Tileset_McCoyPalaceGroundsTilesB+0x197a
	.global Tileset_McCoyPalaceGroundsTilesD
Tileset_McCoyPalaceGroundsTilesD:
	.incbin "games/tbs/src/graphics/tileset/mccoy_palace_grounds/tiles_d.4bpp.mtf"
	.byte 0xa9	@ PACKER: left in the buffer by Tileset_McCoyPalaceGroundsTilesC+0x13f
	.balign 4, 0
