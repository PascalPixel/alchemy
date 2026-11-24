	.global Field_AnkohlRuinsSandDoors
Field_AnkohlRuinsSandDoors:
	map_header 0, 0, 128, 128, 3, 3, 2, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_AnkohlRuinsSandDoors, .LField_AnkohlRuinsSandDoors_CELL_TILES
	map_block Field_AnkohlRuinsSandDoors, .LField_AnkohlRuinsSandDoors_SHAPES
	map_block Field_AnkohlRuinsSandDoors, .LField_AnkohlRuinsSandDoors_GRID
	map_block Field_AnkohlRuinsSandDoors, .LField_AnkohlRuinsSandDoors_SHAPE_GRID
	map_block Field_AnkohlRuinsSandDoors, .LField_AnkohlRuinsSandDoors_ANIMATION
	map_block Field_AnkohlRuinsSandDoors, .LField_AnkohlRuinsSandDoors_BLEND
	map_block Field_AnkohlRuinsSandDoors, .LField_AnkohlRuinsSandDoors_SCENE
.LField_AnkohlRuinsSandDoors_CELL_TILES:
	.incbin "games/tla/src/map/ankohl_ruins_sand_doors/cell_tiles.delta1.lz"
	.balign 4, 0
.LField_AnkohlRuinsSandDoors_SHAPES:
	.incbin "games/tla/src/map/ankohl_ruins_sand_doors/shapes.table.lz"
	.balign 4, 0
.LField_AnkohlRuinsSandDoors_GRID:
	.incbin "games/tla/src/map/ankohl_ruins_sand_doors/map.grid.lz"
	.balign 4, 0
.LField_AnkohlRuinsSandDoors_SHAPE_GRID:
	.incbin "games/tla/src/map/ankohl_ruins_sand_doors/shape_grid.plane.lz"
	.balign 4, 0
.LField_AnkohlRuinsSandDoors_ANIMATION:
	.incbin "games/tla/src/map/ankohl_ruins_thanatos_mace_chest/animation.script.lz"
	.balign 4, 0
.LField_AnkohlRuinsSandDoors_BLEND:
	.incbin "games/tla/src/map/lobby/blend.script.lz"
	.balign 4, 0
.LField_AnkohlRuinsSandDoors_SCENE:
	.incbin "games/tla/src/map/ankohl_ruins_sand_doors/scene.scene.lz"
	.balign 4, 0
