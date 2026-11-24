	.global Field_LemurianShipDeck
Field_LemurianShipDeck:
	map_header 0, 0, 96, 72, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 96, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 72, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_LemurianShipDeck, .LField_LemurianShipDeck_CELL_TILES
	map_block Field_LemurianShipDeck, .LField_LemurianShipDeck_SHAPES
	map_block Field_LemurianShipDeck, .LField_LemurianShipDeck_GRID
	map_block Field_LemurianShipDeck, .LField_LemurianShipDeck_SHAPE_GRID
	map_block Field_LemurianShipDeck, .LField_LemurianShipDeck_ANIMATION
	map_block Field_LemurianShipDeck, .LField_LemurianShipDeck_BLEND
	map_block Field_LemurianShipDeck, .LField_LemurianShipDeck_SCENE
.LField_LemurianShipDeck_CELL_TILES:
	.incbin "games/tla/src/map/lemurian_ship_deck/cell_tiles.delta1.lz"
	.balign 4, 0
.LField_LemurianShipDeck_SHAPES:
	.incbin "games/tla/src/map/lemurian_ship_deck/shapes.table.lz"
	.balign 4, 0
.LField_LemurianShipDeck_GRID:
	.incbin "games/tla/src/map/lemurian_ship_deck/map.grid.lz"
	.balign 4, 0
.LField_LemurianShipDeck_SHAPE_GRID:
	.incbin "games/tla/src/map/lemurian_ship_deck/shape_grid.plane.lz"
	.balign 4, 0
.LField_LemurianShipDeck_ANIMATION:
	.incbin "games/tla/src/map/lemurian_ship_deck/animation.script.lz"
	.balign 4, 0
.LField_LemurianShipDeck_BLEND:
	.incbin "games/tla/src/map/lobby/blend.script.lz"
	.balign 4, 0
.LField_LemurianShipDeck_SCENE:
	.incbin "games/tla/src/map/lemurian_ship_deck/scene.scene.lz"
	.balign 4, 0
	.global Tileset_LemurianShipDeckPalette
Tileset_LemurianShipDeckPalette:
	.incbin "games/tla/src/graphics/tileset/lemurian_ship_deck/palette.gbapal.lz"
	.global Tileset_LemurianShipDeckTilesA
Tileset_LemurianShipDeckTilesA:
	.incbin "games/tla/src/graphics/tileset/lemurian_ship_deck/tiles_a.4bpp.mtf"
	.global Tileset_LemurianShipDeckTilesB
Tileset_LemurianShipDeckTilesB:
	.incbin "games/tla/src/graphics/tileset/lemurian_ship_deck/tiles_b.4bpp.mtf"
	.global Tileset_LemurianShipDeckTilesC
Tileset_LemurianShipDeckTilesC:
	.incbin "games/tla/src/graphics/tileset/lemurian_ship_deck/tiles_c.4bpp.mtf"
	.global Tileset_LemurianShipDeckTilesD
Tileset_LemurianShipDeckTilesD:
	.incbin "games/tla/src/graphics/tileset/lemurian_ship_deck/tiles_d.4bpp.mtf"
	.byte 0xa4	@ PACKER: left in the buffer by Tileset_LemurianShipDeckTilesC+0x18db
