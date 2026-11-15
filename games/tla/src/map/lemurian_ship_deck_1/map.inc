	.global Field_LemurianShipDeck1
Field_LemurianShipDeck1:
	map_header 0, 0, 96, 72, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 96, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 72, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_LemurianShipDeck1, .LField_LemurianShipDeck1_CELL_PICTURES
	map_block Field_LemurianShipDeck1, .LField_LemurianShipDeck1_SHAPES
	map_block Field_LemurianShipDeck1, .LField_LemurianShipDeck1_GRID
	map_block Field_LemurianShipDeck1, .LField_LemurianShipDeck1_SHAPE_GRID
	map_block Field_LemurianShipDeck1, .LField_LemurianShipDeck1_ANIMATION
	map_block Field_LemurianShipDeck1, .LField_LemurianShipDeck1_BLEND
	map_block Field_LemurianShipDeck1, .LField_LemurianShipDeck1_SCENE
.LField_LemurianShipDeck1_CELL_PICTURES:
	.incbin "games/tla/src/map/lemurian_ship_deck_1/cell_pictures.delta1.lz"
	.balign 4, 0
.LField_LemurianShipDeck1_SHAPES:
	.incbin "games/tla/src/map/lemurian_ship_deck_1/shapes.table.lz"
	.balign 4, 0
.LField_LemurianShipDeck1_GRID:
	.incbin "games/tla/src/map/lemurian_ship_deck_1/map.grid.lz"
	.balign 4, 0
.LField_LemurianShipDeck1_SHAPE_GRID:
	.incbin "games/tla/src/map/lemurian_ship_deck_1/shape_grid.plane.lz"
	.balign 4, 0
.LField_LemurianShipDeck1_ANIMATION:
	.incbin "games/tla/src/map/lemurian_ship_deck_1/animation.script.lz"
	.balign 4, 0
.LField_LemurianShipDeck1_BLEND:
	.incbin "games/tla/src/map/lobby/blend.script.lz"
	.balign 4, 0
.LField_LemurianShipDeck1_SCENE:
	.incbin "games/tla/src/map/lemurian_ship_deck_1/scene.scene.lz"
	.global Tileset_LemurianShipDeck1TilesA
Tileset_LemurianShipDeck1TilesA:
	.incbin "games/tla/src/graphics/tileset/lemurian_ship_deck_1/tiles_a.4bpp.mtf"
	.byte 0xa7, 0xe6	@ PACKER: left in the buffer by Tileset_SeaOfTimeToLemuriaTilesD+0x1f0a
	.global Tileset_LemurianShipDeck1TilesB
Tileset_LemurianShipDeck1TilesB:
	.incbin "games/tla/src/graphics/tileset/lemurian_ship_deck_1/tiles_b.4bpp.mtf"
	.byte 0x12	@ PACKER: left in the buffer by Tileset_SeaOfTimeToLemuriaTilesD+0x2363
	.global Tileset_LemurianShipDeck1TilesC
Tileset_LemurianShipDeck1TilesC:
	.incbin "games/tla/src/graphics/tileset/lemurian_ship_deck_1/tiles_c.4bpp.mtf"
	.global Tileset_LemurianShipDeck1TilesD
Tileset_LemurianShipDeck1TilesD:
	.incbin "games/tla/src/graphics/tileset/lemurian_ship_deck_1/tiles_d.4bpp.mtf"
