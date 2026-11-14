	.global Field_KolimaForestFloodgate
Field_KolimaForestFloodgate:
	map_header 0, 0, 64, 64, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 64, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_offset Field_KolimaForestFloodgate, .LField_KolimaForestFloodgate_CELL_PICTURES
	map_offset Field_KolimaForestFloodgate, .LField_KolimaForestFloodgate_SHAPES
	map_offset Field_KolimaForestFloodgate, .LField_KolimaForestFloodgate_GRID
	map_offset Field_KolimaForestFloodgate, .LField_KolimaForestFloodgate_ANIMATION
	map_offset Field_KolimaForestFloodgate
	map_offset Field_KolimaForestFloodgate, .LField_KolimaForestFloodgate_MARKERS
.LField_KolimaForestFloodgate_CELL_PICTURES:
	.incbin "games/tbs/src/map/kolima_forest_floodgate/cell_pictures.delta0.lz"
	.balign 4, 0
.LField_KolimaForestFloodgate_SHAPES:
	.incbin "games/tbs/src/map/kolima_forest_floodgate/shapes.table.lz"
	.balign 4, 0
.LField_KolimaForestFloodgate_GRID:
	.incbin "games/tbs/src/map/kolima_forest_floodgate/map.grid.lz"
	.balign 4, 0
.LField_KolimaForestFloodgate_ANIMATION:
	.incbin "games/tbs/src/map/kolima_forest_floodgate/tileset_anims.script.lz"
	.balign 4, 0
.LField_KolimaForestFloodgate_MARKERS:
	treasure_spot 25, 7, 100
	treasure_spot 57, 7, 100
	treasure_spots_end
	.balign 4, 0
	.global Tileset_KolimaForestFloodgatePalette
Tileset_KolimaForestFloodgatePalette:
	.incbin "games/tbs/src/graphics/tileset/kolima_forest_floodgate/palette.gbapal.lz"
	.byte 0x6a, 0x7b	@ PACKER: left in the buffer by Field_KolimaForestFloodgate+0x132
	.balign 4, 0
	.global Tileset_KolimaForestFloodgateTilesA
Tileset_KolimaForestFloodgateTilesA:
	.incbin "games/tbs/src/graphics/tileset/kolima_forest_floodgate/tiles_a.4bpp.mtf"
	.balign 4, 0	@ PACKER: zero fill, not an earlier stream's bytes
	.global Tileset_KolimaForestFloodgateTilesB
Tileset_KolimaForestFloodgateTilesB:
	.incbin "games/tbs/src/graphics/tileset/kolima_forest_floodgate/tiles_b.4bpp.mtf"
	.global Tileset_KolimaForestFloodgateTilesC
Tileset_KolimaForestFloodgateTilesC:
	.incbin "games/tbs/src/graphics/tileset/kolima_forest_floodgate/tiles_c.4bpp.mtf"
	.byte 0xd6, 0xb6, 0x08	@ PACKER: left in the buffer by Tileset_KolimaForestFloodgateTilesB+0x17c1
	.global Tileset_KolimaForestFloodgateTilesD
Tileset_KolimaForestFloodgateTilesD:
	.incbin "games/tbs/src/graphics/tileset/kolima_forest_floodgate/tiles_d.4bpp.mtf"
	.balign 4, 0
