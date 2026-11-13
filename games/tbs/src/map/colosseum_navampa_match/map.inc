	.global Field_ColosseumNavampaMatch
Field_ColosseumNavampaMatch:
	map_header 0, 0, 206, 52, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 52, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 104, 16, 16, 0, 0, 0xff, 0xff
	map_offset Field_ColosseumNavampaMatch, .LField_ColosseumNavampaMatch_CELL_PICTURES
	map_offset Field_ColosseumNavampaMatch, .LField_ColosseumNavampaMatch_SHAPES
	map_offset Field_ColosseumNavampaMatch, .LField_ColosseumNavampaMatch_GRID
	map_offset Field_ColosseumNavampaMatch, .LField_ColosseumNavampaMatch_ANIMATION
	map_offset Field_ColosseumNavampaMatch, .LField_ColosseumNavampaMatch_BLEND
	map_offset Field_ColosseumNavampaMatch, .LField_ColosseumNavampaMatch_MARKERS
.LField_ColosseumNavampaMatch_CELL_PICTURES:
	.incbin "games/tbs/src/map/colosseum_navampa_match/cell_pictures.delta1.lz"
	.balign 4, 0
.LField_ColosseumNavampaMatch_SHAPES:
	.incbin "games/tbs/src/map/colosseum_navampa_match/shapes.table.lz"
	.balign 4, 0
.LField_ColosseumNavampaMatch_GRID:
	.incbin "games/tbs/src/map/colosseum_navampa_match/map.grid.lz"
	.balign 4, 0
.LField_ColosseumNavampaMatch_ANIMATION:
	.incbin "games/tbs/src/map/colosseum_navampa_match/tileset_anims.script.lz"
	.balign 4, 0
.LField_ColosseumNavampaMatch_BLEND:
	.incbin "games/tbs/src/map/goma_cave_entrance/blend.script.lz"
	.balign 4, 0
.LField_ColosseumNavampaMatch_MARKERS:
	treasure_spot 44, 6, 102
	treasure_spot 12, 7, 101
	treasure_spot 75, 13, 103
	treasure_spots_end
	.global Tileset_ColosseumNavampaMatchPalette
Tileset_ColosseumNavampaMatchPalette:
	.incbin "games/tbs/src/graphics/tileset/colosseum_navampa_match/palette.gbapal.lz"
	.byte 0x22, 0xb6	@ PACKER: left in the buffer by Field_ColosseumNavampaMatch+0x13a
	.global Tileset_ColosseumNavampaMatchTilesA
Tileset_ColosseumNavampaMatchTilesA:
	.incbin "games/tbs/src/graphics/tileset/colosseum_navampa_match/tiles_a.4bpp.mtf"
	.byte 0x81, 0xdf	@ PACKER: left in the buffer by Tileset_ColosseumSatrageMatchTilesA+0x2732
	.global Tileset_ColosseumNavampaMatchTilesB
Tileset_ColosseumNavampaMatchTilesB:
	.incbin "games/tbs/src/graphics/tileset/colosseum_navampa_match/tiles_b.4bpp.mtf"
	.byte 0xba	@ PACKER: left in the buffer by Tileset_ColosseumNavampaMatchTilesA+0x1c43
	.global Tileset_ColosseumNavampaMatchTilesC
Tileset_ColosseumNavampaMatchTilesC:
	.incbin "games/tbs/src/graphics/tileset/colosseum_navampa_match/tiles_c.4bpp.mtf"
	.byte 0x3c, 0xe0, 0x13	@ PACKER: left in the buffer by Tileset_ColosseumNavampaMatchTilesB+0x1445
	.global Tileset_ColosseumNavampaMatchTilesD
Tileset_ColosseumNavampaMatchTilesD:
	.incbin "games/tbs/src/graphics/tileset/colosseum_navampa_match/tiles_d.4bpp.mtf"
	.byte 0xa3	@ PACKER: left in the buffer by Tileset_ColosseumNavampaMatchTilesB+0x1b7b
	.balign 4, 0
