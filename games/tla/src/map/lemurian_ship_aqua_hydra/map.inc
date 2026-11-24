	.global Field_LemurianShipAquaHydra
Field_LemurianShipAquaHydra:
	map_header 0, 0, 120, 120, 3, 1, 2, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 120, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 120, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_LemurianShipAquaHydra, .LField_LemurianShipAquaHydra_CELL_TILES
	map_block Field_LemurianShipAquaHydra, .LField_LemurianShipAquaHydra_SHAPES
	map_block Field_LemurianShipAquaHydra, .LField_LemurianShipAquaHydra_GRID
	map_block Field_LemurianShipAquaHydra, .LField_LemurianShipAquaHydra_SHAPE_GRID
	map_block Field_LemurianShipAquaHydra, .LField_LemurianShipAquaHydra_ANIMATION
	map_block Field_LemurianShipAquaHydra, .LField_LemurianShipAquaHydra_BLEND
	map_block Field_LemurianShipAquaHydra, .LField_LemurianShipAquaHydra_SCENE
.LField_LemurianShipAquaHydra_CELL_TILES:
	.incbin "games/tla/src/map/lemurian_ship_aqua_hydra/cell_tiles.delta1.lz"
	.balign 4, 0
.LField_LemurianShipAquaHydra_SHAPES:
	.incbin "games/tla/src/map/lemurian_ship_aqua_hydra/shapes.table.lz"
	.balign 4, 0
.LField_LemurianShipAquaHydra_GRID:
	.incbin "games/tla/src/map/lemurian_ship_aqua_hydra/map.grid.lz"
	.balign 4, 0
.LField_LemurianShipAquaHydra_SHAPE_GRID:
	.incbin "games/tla/src/map/lemurian_ship_aqua_hydra/shape_grid.plane.lz"
	.balign 4, 0
.LField_LemurianShipAquaHydra_ANIMATION:
	.incbin "games/tla/src/map/lemurian_ship_aqua_hydra/animation.script.lz"
	.balign 4, 0
.LField_LemurianShipAquaHydra_BLEND:
	.incbin "games/tla/src/map/daila_rooms_houses/blend.script.lz"
	.balign 4, 0
.LField_LemurianShipAquaHydra_SCENE:
	.incbin "games/tla/src/map/lemurian_ship_aqua_hydra/scene.scene.lz"
