	.global Field_KolimaForestTree
Field_KolimaForestTree:
	map_header 0, 0, 64, 64, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 64, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_offset Field_KolimaForestTree, .LField_KolimaForestTree_CELL_PICTURES
	map_offset Field_KolimaForestTree, .LField_KolimaForestTree_SHAPES
	map_offset Field_KolimaForestTree, .LField_KolimaForestTree_GRID
	map_offset Field_KolimaForestTree
	map_offset Field_KolimaForestTree
	map_offset Field_KolimaForestTree, .LField_KolimaForestTree_MARKERS
.LField_KolimaForestTree_CELL_PICTURES:
	.incbin "games/tbs/src/map/kolima_forest_tree/cell_pictures.delta0.lz"
	.balign 4, 0
.LField_KolimaForestTree_SHAPES:
	.incbin "games/tbs/src/map/kolima_forest_tree/shapes.table.lz"
	.balign 4, 0
.LField_KolimaForestTree_GRID:
	.incbin "games/tbs/src/map/kolima_forest_tree/map.grid.lz"
	.balign 4, 0
.LField_KolimaForestTree_MARKERS:
	treasure_spots_end
	.balign 4, 0
	.global Tileset_KolimaForestTreePalette
Tileset_KolimaForestTreePalette:
	.incbin "games/tbs/src/graphics/tileset/kolima_forest_tree/palette.gbapal.lz"
	.byte 0x05	@ PACKER: left in the buffer by Field_KolimaForestTree+0x17b
	.global Tileset_KolimaForestTreeTilesA
Tileset_KolimaForestTreeTilesA:
	.incbin "games/tbs/src/graphics/tileset/kolima_forest_tree/tiles_a.4bpp.mtf"
	.byte 0x07, 0xf0	@ PACKER: left in the buffer by Tileset_KolimaForestFloodgateTilesA+0x2bae
	.global Tileset_KolimaForestTreeTilesB
Tileset_KolimaForestTreeTilesB:
	.incbin "games/tbs/src/graphics/tileset/kolima_forest_tree/tiles_b.4bpp.mtf"
	.byte 0x8f, 0x63	@ PACKER: left in the buffer by Tileset_KolimaForestTreeTilesA+0x2272
	.global Tileset_KolimaForestTreeTilesC
Tileset_KolimaForestTreeTilesC:
	.incbin "games/tbs/src/graphics/tileset/kolima_forest_tree/tiles_c.4bpp.mtf"
	.byte 0x6b	@ PACKER: left in the buffer by Tileset_KolimaForestTreeTilesA+0x286f
	.global Tileset_KolimaForestTreeTilesD
Tileset_KolimaForestTreeTilesD:
	.incbin "games/tbs/src/graphics/tileset/kolima_forest_tree/tiles_d.4bpp.mtf"
	.balign 4, 0
