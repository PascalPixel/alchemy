	.global Field_KolimaForestGlade
Field_KolimaForestGlade:
	map_header 0, 0, 64, 64, 2, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 64, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_offset Field_KolimaForestGlade, .LField_KolimaForestGlade_CELL_PICTURES
	map_offset Field_KolimaForestGlade, .LField_KolimaForestGlade_SHAPES
	map_offset Field_KolimaForestGlade, .LField_KolimaForestGlade_GRID
	map_offset Field_KolimaForestGlade, .LField_KolimaForestGlade_ANIMATION
	map_offset Field_KolimaForestGlade, .LField_KolimaForestGlade_BLEND
	map_offset Field_KolimaForestGlade, .LField_KolimaForestGlade_MARKERS
.LField_KolimaForestGlade_CELL_PICTURES:
	.incbin "games/tbs/src/map/kolima_forest_glade/cell_pictures.delta0.lz"
	.balign 4, 0
.LField_KolimaForestGlade_SHAPES:
	.incbin "games/tbs/src/map/kolima_forest_glade/shapes.table.lz"
	.balign 4, 0
.LField_KolimaForestGlade_GRID:
	.incbin "games/tbs/src/map/kolima_forest_glade/map.grid.lz"
	.balign 4, 0
.LField_KolimaForestGlade_ANIMATION:
	.incbin "games/tbs/src/map/kolima_forest_glade/tileset_anims.script.lz"
	.balign 4, 0
.LField_KolimaForestGlade_BLEND:
	.incbin "games/tbs/src/map/kolima_bridge/blend.script.lz"
	.balign 4, 0
.LField_KolimaForestGlade_MARKERS:
	treasure_spots_end
	.balign 4, 0
	.global Tileset_KolimaForestGladePalette
Tileset_KolimaForestGladePalette:
	.incbin "games/tbs/src/graphics/tileset/kolima_forest_glade/palette.gbapal.lz"
	.byte 0xcb, 0x81	@ PACKER: left in the buffer by Field_KolimaForestGlade+0x166
	.global Tileset_KolimaForestGladeTilesA
Tileset_KolimaForestGladeTilesA:
	.incbin "games/tbs/src/graphics/tileset/kolima_forest_glade/tiles_a.4bpp.mtf"
	.byte 0x00, 0x80, 0x57	@ PACKER: left in the buffer by Field_KolimaForestGlade+0x1145
	.balign 4, 0
