	.global Field_TundariaTowerBurstBrooch
Field_TundariaTowerBurstBrooch:
	map_header 0, 0, 128, 128, 2, 2, 3, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0x1f, 0x1f
	map_layer 0, 128, 12, 12, 0, 0, 0xff, 0xff
	map_block Field_TundariaTowerBurstBrooch, .LField_TundariaTowerBurstBrooch_CELL_TILES
	map_block Field_TundariaTowerBurstBrooch, .LField_TundariaTowerBurstBrooch_SHAPES
	map_block Field_TundariaTowerBurstBrooch, .LField_TundariaTowerBurstBrooch_GRID
	map_block Field_TundariaTowerBurstBrooch, .LField_TundariaTowerBurstBrooch_SHAPE_GRID
	map_block Field_TundariaTowerBurstBrooch, .LField_TundariaTowerBurstBrooch_ANIMATION
	map_block Field_TundariaTowerBurstBrooch
	map_block Field_TundariaTowerBurstBrooch, .LField_TundariaTowerBurstBrooch_SCENE
.LField_TundariaTowerBurstBrooch_CELL_TILES:
	.incbin "games/tla/src/map/tundaria_tower_burst_brooch/cell_tiles.delta1.lz"
	.balign 4, 0
.LField_TundariaTowerBurstBrooch_SHAPES:
	.incbin "games/tla/src/map/tundaria_tower_burst_brooch/shapes.table.lz"
	.balign 4, 0
.LField_TundariaTowerBurstBrooch_GRID:
	.incbin "games/tla/src/map/tundaria_tower_burst_brooch/map.grid.lz"
	.balign 4, 0
.LField_TundariaTowerBurstBrooch_SHAPE_GRID:
	.incbin "games/tla/src/map/tundaria_tower_burst_brooch/shape_grid.plane.lz"
	.balign 4, 0
.LField_TundariaTowerBurstBrooch_ANIMATION:
	.incbin "games/tla/src/map/kandorean_temple_mimic/animation.script.lz"
	.balign 4, 0
.LField_TundariaTowerBurstBrooch_SCENE:
	.incbin "games/tla/src/map/tundaria_tower_burst_brooch/scene.scene.lz"
	.balign 4, 0
	.global Tileset_PaletteTundariaTowerBurstBrooch
Tileset_PaletteTundariaTowerBurstBrooch:
	.incbin "games/tla/src/graphics/tileset/palettes/tundaria_tower_burst_brooch.gbapal.lz"
