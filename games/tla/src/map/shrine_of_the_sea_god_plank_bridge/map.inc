	.global Field_ShrineOfTheSeaGodPlankBridge
Field_ShrineOfTheSeaGodPlankBridge:
	map_header 0, 0, 90, 100, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 100, 16, 16, 0, 0, 0xff, 0xff
	map_layer 100, 0, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_ShrineOfTheSeaGodPlankBridge, .LField_ShrineOfTheSeaGodPlankBridge_CELL_TILES
	map_block Field_ShrineOfTheSeaGodPlankBridge, .LField_ShrineOfTheSeaGodPlankBridge_SHAPES
	map_block Field_ShrineOfTheSeaGodPlankBridge, .LField_ShrineOfTheSeaGodPlankBridge_GRID
	map_block Field_ShrineOfTheSeaGodPlankBridge, .LField_ShrineOfTheSeaGodPlankBridge_SHAPE_GRID
	map_block Field_ShrineOfTheSeaGodPlankBridge, .LField_ShrineOfTheSeaGodPlankBridge_ANIMATION
	map_block Field_ShrineOfTheSeaGodPlankBridge, .LField_ShrineOfTheSeaGodPlankBridge_BLEND
	map_block Field_ShrineOfTheSeaGodPlankBridge, .LField_ShrineOfTheSeaGodPlankBridge_SCENE
.LField_ShrineOfTheSeaGodPlankBridge_CELL_TILES:
	.incbin "games/tla/src/map/shrine_of_the_sea_god_plank_bridge/cell_tiles.delta2.lz"
	.balign 4, 0
.LField_ShrineOfTheSeaGodPlankBridge_SHAPES:
	.incbin "games/tla/src/map/shrine_of_the_sea_god_plank_bridge/shapes.table.lz"
	.balign 4, 0
.LField_ShrineOfTheSeaGodPlankBridge_GRID:
	.incbin "games/tla/src/map/shrine_of_the_sea_god_plank_bridge/map.grid.lz"
	.balign 4, 0
.LField_ShrineOfTheSeaGodPlankBridge_SHAPE_GRID:
	.incbin "games/tla/src/map/shrine_of_the_sea_god_plank_bridge/shape_grid.plane.lz"
	.balign 4, 0
.LField_ShrineOfTheSeaGodPlankBridge_ANIMATION:
	.incbin "games/tla/src/map/shrine_of_the_sea_god_rope/animation.script.lz"
	.balign 4, 0
.LField_ShrineOfTheSeaGodPlankBridge_BLEND:
	.incbin "games/tla/src/map/lobby/blend.script.lz"
	.balign 4, 0
.LField_ShrineOfTheSeaGodPlankBridge_SCENE:
	.incbin "games/tla/src/map/shrine_of_the_sea_god_plank_bridge/scene.scene.lz"
	.balign 4, 0
