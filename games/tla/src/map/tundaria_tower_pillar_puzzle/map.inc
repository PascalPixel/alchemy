	.global Field_TundariaTowerPillarPuzzle
Field_TundariaTowerPillarPuzzle:
	map_header 0, 0, 128, 128, 3, 2, 2, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_TundariaTowerPillarPuzzle, .LField_TundariaTowerPillarPuzzle_CELL_TILES
	map_block Field_TundariaTowerPillarPuzzle, .LField_TundariaTowerPillarPuzzle_SHAPES
	map_block Field_TundariaTowerPillarPuzzle, .LField_TundariaTowerPillarPuzzle_GRID
	map_block Field_TundariaTowerPillarPuzzle, .LField_TundariaTowerPillarPuzzle_SHAPE_GRID
	map_block Field_TundariaTowerPillarPuzzle, .LField_TundariaTowerPillarPuzzle_ANIMATION
	map_block Field_TundariaTowerPillarPuzzle, .LField_TundariaTowerPillarPuzzle_BLEND
	map_block Field_TundariaTowerPillarPuzzle, .LField_TundariaTowerPillarPuzzle_SCENE
.LField_TundariaTowerPillarPuzzle_CELL_TILES:
	.incbin "games/tla/src/map/tundaria_tower_pillar_puzzle/cell_tiles.delta1.lz"
	.balign 4, 0
.LField_TundariaTowerPillarPuzzle_SHAPES:
	.incbin "games/tla/src/map/tundaria_tower_pillar_puzzle/shapes.table.lz"
	.balign 4, 0
.LField_TundariaTowerPillarPuzzle_GRID:
	.incbin "games/tla/src/map/tundaria_tower_pillar_puzzle/map.grid.lz"
	.balign 4, 0
.LField_TundariaTowerPillarPuzzle_SHAPE_GRID:
	.incbin "games/tla/src/map/tundaria_tower_pillar_puzzle/shape_grid.plane.lz"
	.balign 4, 0
.LField_TundariaTowerPillarPuzzle_ANIMATION:
	.incbin "games/tla/src/map/tundaria_tower_ice_slide/animation.script.lz"
	.balign 4, 0
.LField_TundariaTowerPillarPuzzle_BLEND:
	.incbin "games/tla/src/map/tundaria_tower_reflux/blend.script.lz"
	.balign 4, 0
.LField_TundariaTowerPillarPuzzle_SCENE:
	.incbin "games/tla/src/map/tundaria_tower_pillar_puzzle/scene.scene.lz"
	.balign 4, 0
	.global Tileset_PaletteTundariaTower6
Tileset_PaletteTundariaTower6:
	.incbin "games/tla/src/graphics/tileset/palettes/tundaria_tower_pillar_puzzle.gbapal.lz"
