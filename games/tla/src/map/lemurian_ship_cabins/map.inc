	.global Field_LemurianShipCabins
Field_LemurianShipCabins:
	map_header 0, 0, 120, 120, 3, 1, 2, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 120, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 120, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_LemurianShipCabins, .LField_LemurianShipCabins_CELL_TILES
	map_block Field_LemurianShipCabins, .LField_LemurianShipCabins_SHAPES
	map_block Field_LemurianShipCabins, .LField_LemurianShipCabins_GRID
	map_block Field_LemurianShipCabins, .LField_LemurianShipCabins_SHAPE_GRID
	map_block Field_LemurianShipCabins, .LField_LemurianShipCabins_ANIMATION
	map_block Field_LemurianShipCabins, .LField_LemurianShipCabins_BLEND
	map_block Field_LemurianShipCabins, .LField_LemurianShipCabins_SCENE
.LField_LemurianShipCabins_CELL_TILES:
	.incbin "games/tla/src/map/lemurian_ship_cabins/cell_tiles.delta1.lz"
	.balign 4, 0
.LField_LemurianShipCabins_SHAPES:
	.incbin "games/tla/src/map/lemurian_ship_cabins/shapes.table.lz"
	.balign 4, 0
.LField_LemurianShipCabins_GRID:
	.incbin "games/tla/src/map/lemurian_ship_cabins/map.grid.lz"
	.balign 4, 0
.LField_LemurianShipCabins_SHAPE_GRID:
	.incbin "games/tla/src/map/lemurian_ship_cabins/shape_grid.plane.lz"
	.balign 4, 0
.LField_LemurianShipCabins_ANIMATION:
	.incbin "games/tla/src/map/lemurian_ship_cabins/animation.script.lz"
	.balign 4, 0
.LField_LemurianShipCabins_BLEND:
	.incbin "games/tla/src/map/daila_rooms_houses/blend.script.lz"
	.balign 4, 0
.LField_LemurianShipCabins_SCENE:
	.incbin "games/tla/src/map/lemurian_ship_cabins/scene.scene.lz"
	.global Tileset_PaletteLemurianShip1
Tileset_PaletteLemurianShip1:
	.incbin "games/tla/src/graphics/tileset/palettes/lemurian_ship_cabins.gbapal.lz"
