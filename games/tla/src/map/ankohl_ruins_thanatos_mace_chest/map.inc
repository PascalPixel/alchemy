	.global Field_AnkohlRuinsThanatosMaceChest
Field_AnkohlRuinsThanatosMaceChest:
	map_header 0, 0, 128, 128, 3, 1, 2, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_AnkohlRuinsThanatosMaceChest, .LField_AnkohlRuinsThanatosMaceChest_CELL_TILES
	map_block Field_AnkohlRuinsThanatosMaceChest, .LField_AnkohlRuinsThanatosMaceChest_SHAPES
	map_block Field_AnkohlRuinsThanatosMaceChest, .LField_AnkohlRuinsThanatosMaceChest_GRID
	map_block Field_AnkohlRuinsThanatosMaceChest, .LField_AnkohlRuinsThanatosMaceChest_SHAPE_GRID
	map_block Field_AnkohlRuinsThanatosMaceChest, .LField_AnkohlRuinsThanatosMaceChest_ANIMATION
	map_block Field_AnkohlRuinsThanatosMaceChest, .LField_AnkohlRuinsThanatosMaceChest_BLEND
	map_block Field_AnkohlRuinsThanatosMaceChest, .LField_AnkohlRuinsThanatosMaceChest_SCENE
.LField_AnkohlRuinsThanatosMaceChest_CELL_TILES:
	.incbin "games/tla/src/map/ankohl_ruins_thanatos_mace_chest/cell_tiles.delta1.lz"
	.balign 4, 0
.LField_AnkohlRuinsThanatosMaceChest_SHAPES:
	.incbin "games/tla/src/map/ankohl_ruins_thanatos_mace_chest/shapes.table.lz"
	.balign 4, 0
.LField_AnkohlRuinsThanatosMaceChest_GRID:
	.incbin "games/tla/src/map/ankohl_ruins_thanatos_mace_chest/map.grid.lz"
	.balign 4, 0
.LField_AnkohlRuinsThanatosMaceChest_SHAPE_GRID:
	.incbin "games/tla/src/map/ankohl_ruins_thanatos_mace_chest/shape_grid.plane.lz"
	.balign 4, 0
.LField_AnkohlRuinsThanatosMaceChest_ANIMATION:
	.incbin "games/tla/src/map/ankohl_ruins_thanatos_mace_chest/animation.script.lz"
	.balign 4, 0
.LField_AnkohlRuinsThanatosMaceChest_BLEND:
	.incbin "games/tla/src/map/shrine_of_the_sea_god_ladders/blend.script.lz"
	.balign 4, 0
.LField_AnkohlRuinsThanatosMaceChest_SCENE:
	.incbin "games/tla/src/map/ankohl_ruins_thanatos_mace_chest/scene.scene.lz"
	.balign 4, 0
