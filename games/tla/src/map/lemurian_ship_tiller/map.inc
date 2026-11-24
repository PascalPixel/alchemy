	.global Field_LemurianShipTiller
Field_LemurianShipTiller:
	map_header 0, 0, 96, 72, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 96, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 72, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_LemurianShipTiller, .LField_LemurianShipTiller_CELL_TILES
	map_block Field_LemurianShipTiller, .LField_LemurianShipTiller_SHAPES
	map_block Field_LemurianShipTiller, .LField_LemurianShipTiller_GRID
	map_block Field_LemurianShipTiller, .LField_LemurianShipTiller_SHAPE_GRID
	map_block Field_LemurianShipTiller, .LField_LemurianShipTiller_ANIMATION
	map_block Field_LemurianShipTiller, .LField_LemurianShipTiller_BLEND
	map_block Field_LemurianShipTiller, .LField_LemurianShipTiller_SCENE
.LField_LemurianShipTiller_CELL_TILES:
	.incbin "games/tla/src/map/lemurian_ship_tiller/cell_tiles.delta1.lz"
	.balign 4, 0
.LField_LemurianShipTiller_SHAPES:
	.incbin "games/tla/src/map/lemurian_ship_tiller/shapes.table.lz"
	.balign 4, 0
.LField_LemurianShipTiller_GRID:
	.incbin "games/tla/src/map/lemurian_ship_tiller/map.grid.lz"
	.balign 4, 0
.LField_LemurianShipTiller_SHAPE_GRID:
	.incbin "games/tla/src/map/lemurian_ship_tiller/shape_grid.plane.lz"
	.balign 4, 0
.LField_LemurianShipTiller_ANIMATION:
	.incbin "games/tla/src/map/lemurian_ship_tiller/animation.script.lz"
	.balign 4, 0
.LField_LemurianShipTiller_BLEND:
	.incbin "games/tla/src/map/lobby/blend.script.lz"
	.balign 4, 0
.LField_LemurianShipTiller_SCENE:
	.incbin "games/tla/src/map/lemurian_ship_tiller/scene.scene.lz"
	.global Tileset_LemurianShipDeck1TilesA
Tileset_LemurianShipDeck1TilesA:
	.incbin "games/tla/src/graphics/tileset/lemurian_ship_tiller/tiles_a.4bpp.mtf"
	.byte 0xa7, 0xe6	@ PACKER: left in the buffer by Tileset_SeaOfTimeToLemuriaTilesD+0x1f0a
	.global Tileset_LemurianShipDeck1TilesB
Tileset_LemurianShipDeck1TilesB:
	.incbin "games/tla/src/graphics/tileset/lemurian_ship_tiller/tiles_b.4bpp.mtf"
	.byte 0x12	@ PACKER: left in the buffer by Tileset_SeaOfTimeToLemuriaTilesD+0x2363
	.global Tileset_LemurianShipDeck1TilesC
Tileset_LemurianShipDeck1TilesC:
	.incbin "games/tla/src/graphics/tileset/lemurian_ship_tiller/tiles_c.4bpp.mtf"
	.global Tileset_LemurianShipDeck1TilesD
Tileset_LemurianShipDeck1TilesD:
	.incbin "games/tla/src/graphics/tileset/lemurian_ship_tiller/tiles_d.4bpp.mtf"
