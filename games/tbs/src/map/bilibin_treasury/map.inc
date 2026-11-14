	.global Field_BilibinTreasury
Field_BilibinTreasury:
	map_header 0, 0, 72, 94, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 96, 16, 16, 0, 0, 0xff, 0xff
	map_layer 80, 0, 16, 16, 0, 0, 0xff, 0xff
	map_offset Field_BilibinTreasury, .LField_BilibinTreasury_CELL_PICTURES
	map_offset Field_BilibinTreasury, .LField_BilibinTreasury_SHAPES
	map_offset Field_BilibinTreasury, .LField_BilibinTreasury_GRID
	map_offset Field_BilibinTreasury, .LField_BilibinTreasury_ANIMATION
	map_offset Field_BilibinTreasury, .LField_BilibinTreasury_BLEND
	map_offset Field_BilibinTreasury, .LField_BilibinTreasury_MARKERS
.LField_BilibinTreasury_CELL_PICTURES:
	.incbin "games/tbs/src/map/bilibin_treasury/cell_pictures.delta2.lz"
	.balign 4, 0
.LField_BilibinTreasury_SHAPES:
	.incbin "games/tbs/src/map/bilibin_treasury/shapes.table.lz"
	.balign 4, 0
.LField_BilibinTreasury_GRID:
	.incbin "games/tbs/src/map/bilibin_treasury/map.grid.lz"
	.balign 4, 0
.LField_BilibinTreasury_ANIMATION:
	.incbin "games/tbs/src/map/bilibin_treasury/tileset_anims.script.lz"
	.balign 4, 0
.LField_BilibinTreasury_BLEND:
	.incbin "games/tbs/src/map/bilibin_treasury/blend.script.lz"
	.balign 4, 0
.LField_BilibinTreasury_MARKERS:
	treasure_spots_end
	.balign 4, 0
	.global Tileset_BilibinTreasuryPalette
Tileset_BilibinTreasuryPalette:
	.incbin "games/tbs/src/graphics/tileset/bilibin_treasury/palette.gbapal.lz"
	.byte 0xf3	@ PACKER: left in the buffer by Field_BilibinTreasury+0x15f
	.global Tileset_BilibinTreasuryTilesA
Tileset_BilibinTreasuryTilesA:
	.incbin "games/tbs/src/graphics/tileset/bilibin_treasury/tiles_a.4bpp.mtf"
	.byte 0x06	@ PACKER: left in the buffer by Field_CottageRoomsC+0x1c57
	.balign 4, 0
