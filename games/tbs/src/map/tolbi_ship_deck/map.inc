	.global Field_TolbiShipDeck
Field_TolbiShipDeck:
	map_header 0, 0, 45, 120, 3, 1, 2, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, -25, 0xff, 0xff
	map_layer 120, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 120, 16, 16, 0, 0, 0xff, 0xff
	map_offset Field_TolbiShipDeck, .LField_TolbiShipDeck_CELL_PICTURES
	map_offset Field_TolbiShipDeck, .LField_TolbiShipDeck_SHAPES
	map_offset Field_TolbiShipDeck, .LField_TolbiShipDeck_GRID
	map_offset Field_TolbiShipDeck, .LField_TolbiShipDeck_ANIMATION
	map_offset Field_TolbiShipDeck
	map_offset Field_TolbiShipDeck, .LField_TolbiShipDeck_MARKERS
.LField_TolbiShipDeck_CELL_PICTURES:
	.incbin "games/tbs/src/map/tolbi_ship_deck/cell_pictures.delta1.lz"
	.balign 4, 0
.LField_TolbiShipDeck_SHAPES:
	.incbin "games/tbs/src/map/tolbi_ship_deck/shapes.table.lz"
	.balign 4, 0
.LField_TolbiShipDeck_GRID:
	.incbin "games/tbs/src/map/tolbi_ship_deck/map.grid.lz"
	.balign 4, 0
.LField_TolbiShipDeck_ANIMATION:
	.incbin "games/tbs/src/map/tolbi_ship_deck/tileset_anims.script.lz"
	.balign 4, 0
.LField_TolbiShipDeck_MARKERS:
	treasure_spots_end
	.balign 4, 0
	.global Tileset_TolbiShipDeckPalette
Tileset_TolbiShipDeckPalette:
	.incbin "games/tbs/src/graphics/tileset/tolbi_ship_deck/palette.gbapal.lz"
	.byte 0xc5, 0x3e, 0x03	@ PACKER: left in the buffer by Field_TolbiShipDeck+0x1d1
	.global Tileset_TolbiShipDeckTilesA
Tileset_TolbiShipDeckTilesA:
	.incbin "games/tbs/src/graphics/tileset/tolbi_ship_deck/tiles_a.4bpp.mtf"
	.byte 0xd1, 0x43	@ PACKER: left in the buffer by Tileset_KalayDocksTilesB+0x208e
	.global Tileset_TolbiShipDeckTilesB
Tileset_TolbiShipDeckTilesB:
	.incbin "games/tbs/src/graphics/tileset/tolbi_ship_deck/tiles_b.4bpp.mtf"
	.byte 0x79, 0x3a	@ PACKER: left in the buffer by Tileset_KalayDocksTilesB+0x20fe
	.global Tileset_TolbiShipDeckTilesC
Tileset_TolbiShipDeckTilesC:
	.incbin "games/tbs/src/graphics/tileset/tolbi_ship_deck/tiles_c.4bpp.mtf"
	.global Tileset_TolbiShipDeckTilesD
Tileset_TolbiShipDeckTilesD:
	.incbin "games/tbs/src/graphics/tileset/tolbi_ship_deck/tiles_d.4bpp.mtf"
	.byte 0x08, 0x7e	@ PACKER: left in the buffer by Tileset_TolbiShipDeckTilesC+0x99a
	.balign 4, 0
