	.global Field_LemurianShipDeck
Field_LemurianShipDeck:
	map_header 0, 0, 96, 72, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 96, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 72, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_LemurianShipDeck, .LField_LemurianShipDeck_CELL_PICTURES
	map_block Field_LemurianShipDeck, .LField_LemurianShipDeck_SHAPES
	map_block Field_LemurianShipDeck, .LField_LemurianShipDeck_GRID
	map_block Field_LemurianShipDeck, .LField_LemurianShipDeck_SHAPE_GRID
	map_block Field_LemurianShipDeck, .LField_LemurianShipDeck_ANIMATION
	map_block Field_LemurianShipDeck, .LField_LemurianShipDeck_BLEND
	map_block Field_LemurianShipDeck, .LField_LemurianShipDeck_SCENE
.LField_LemurianShipDeck_CELL_PICTURES:
	.incbin "games/tla/src/map/lemurian_ship_deck/cell_pictures.delta1.lz"
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
	.global Tileset_Set60Palette
Tileset_Set60Palette:
	.incbin "games/tla/src/graphics/tileset/set_60/palette.gbapal.lz"
	.global Tileset_Set60TilesA
Tileset_Set60TilesA:
	.incbin "games/tla/src/graphics/tileset/set_60/tiles_a.4bpp.mtf"
	.global Tileset_Set60TilesB
Tileset_Set60TilesB:
	.incbin "games/tla/src/graphics/tileset/set_60/tiles_b.4bpp.mtf"
	.global Tileset_Set60TilesC
Tileset_Set60TilesC:
	.incbin "games/tla/src/graphics/tileset/set_60/tiles_c.4bpp.mtf"
	.global Tileset_Set60TilesD
Tileset_Set60TilesD:
	.incbin "games/tla/src/graphics/tileset/set_60/tiles_d.4bpp.mtf"
	.byte 0xa4	@ PACKER: left in the buffer by Tileset_Set60TilesC+0x18db
