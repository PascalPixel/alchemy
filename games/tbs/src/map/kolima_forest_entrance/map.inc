	.global Field_KolimaForestEntrance
Field_KolimaForestEntrance:
	map_header 0, 0, 64, 84, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 64, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_offset Field_KolimaForestEntrance, .LField_KolimaForestEntrance_CELL_PICTURES
	map_offset Field_KolimaForestEntrance, .LField_KolimaForestEntrance_SHAPES
	map_offset Field_KolimaForestEntrance, .LField_KolimaForestEntrance_GRID
	map_offset Field_KolimaForestEntrance, .LField_KolimaForestEntrance_ANIMATION
	map_offset Field_KolimaForestEntrance
	map_offset Field_KolimaForestEntrance, .LField_KolimaForestEntrance_MARKERS
.LField_KolimaForestEntrance_CELL_PICTURES:
	.incbin "games/tbs/src/map/kolima_forest_entrance/cell_pictures.delta0.lz"
	.balign 4, 0
.LField_KolimaForestEntrance_SHAPES:
	.incbin "games/tbs/src/map/kolima_forest_entrance/shapes.table.lz"
	.balign 4, 0
.LField_KolimaForestEntrance_GRID:
	.incbin "games/tbs/src/map/kolima_forest_entrance/map.grid.lz"
	.balign 4, 0
.LField_KolimaForestEntrance_ANIMATION:
	.incbin "games/tbs/src/map/kolima_forest_entrance/tileset_anims.script.lz"
	.balign 4, 0
.LField_KolimaForestEntrance_MARKERS:
	treasure_spots_end
	.balign 4, 0
	.global Tileset_KolimaForestPalette
Tileset_KolimaForestPalette:
	.incbin "games/tbs/src/graphics/tileset/kolima_forest/palette.gbapal.lz"
	.byte 0xb1, 0xc1	@ PACKER: left in the buffer by Field_KolimaForestEntrance+0x176
	.global Tileset_KolimaForestTilesA
Tileset_KolimaForestTilesA:
	.incbin "games/tbs/src/graphics/tileset/kolima_forest/tiles_a.4bpp.mtf"
	.global Tileset_KolimaForestTilesB
Tileset_KolimaForestTilesB:
	.incbin "games/tbs/src/graphics/tileset/kolima_forest/tiles_b.4bpp.mtf"
	.byte 0x4d, 0x0c, 0x83	@ PACKER: left in the buffer by Tileset_KolimaForestTilesA+0x1d6d
	.global Tileset_KolimaForestTilesC
Tileset_KolimaForestTilesC:
	.incbin "games/tbs/src/graphics/tileset/kolima_forest/tiles_c.4bpp.mtf"
	.byte 0xb8, 0x92, 0x64	@ PACKER: left in the buffer by Tileset_KolimaForestTilesB+0xa65
	.global Tileset_KolimaForestTilesD
Tileset_KolimaForestTilesD:
	.incbin "games/tbs/src/graphics/tileset/kolima_forest/tiles_d.4bpp.mtf"
	.balign 4, 0
