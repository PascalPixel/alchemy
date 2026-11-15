	.global Field_ShrineOfTheSeaGod6
Field_ShrineOfTheSeaGod6:
	map_header 0, 0, 90, 100, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 100, 16, 16, 0, 0, 0xff, 0xff
	map_layer 100, 0, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_ShrineOfTheSeaGod6, .LField_ShrineOfTheSeaGod6_CELL_PICTURES
	map_block Field_ShrineOfTheSeaGod6, .LField_ShrineOfTheSeaGod6_SHAPES
	map_block Field_ShrineOfTheSeaGod6, .LField_ShrineOfTheSeaGod6_GRID
	map_block Field_ShrineOfTheSeaGod6, .LField_ShrineOfTheSeaGod6_SHAPE_GRID
	map_block Field_ShrineOfTheSeaGod6, .LField_ShrineOfTheSeaGod6_ANIMATION
	map_block Field_ShrineOfTheSeaGod6, .LField_ShrineOfTheSeaGod6_BLEND
	map_block Field_ShrineOfTheSeaGod6, .LField_ShrineOfTheSeaGod6_SCENE
.LField_ShrineOfTheSeaGod6_CELL_PICTURES:
	.incbin "games/tla/src/map/shrine_of_the_sea_god_6/cell_pictures.delta2.lz"
	.balign 4, 0
.LField_ShrineOfTheSeaGod6_SHAPES:
	.incbin "games/tla/src/map/shrine_of_the_sea_god_6/shapes.table.lz"
	.balign 4, 0
.LField_ShrineOfTheSeaGod6_GRID:
	.incbin "games/tla/src/map/shrine_of_the_sea_god_6/map.grid.lz"
	.balign 4, 0
.LField_ShrineOfTheSeaGod6_SHAPE_GRID:
	.incbin "games/tla/src/map/shrine_of_the_sea_god_6/shape_grid.plane.lz"
	.balign 4, 0
.LField_ShrineOfTheSeaGod6_ANIMATION:
	.incbin "games/tla/src/map/shrine_of_the_sea_god_6/animation.script.lz"
	.balign 4, 0
.LField_ShrineOfTheSeaGod6_BLEND:
	.incbin "games/tla/src/map/lobby/blend.script.lz"
	.balign 4, 0
.LField_ShrineOfTheSeaGod6_SCENE:
	.incbin "games/tla/src/map/shrine_of_the_sea_god_6/scene.scene.lz"
	.global Tileset_PaletteShrineOfTheSeaGod6
Tileset_PaletteShrineOfTheSeaGod6:
	.incbin "games/tla/src/graphics/tileset/palettes/shrine_of_the_sea_god_6.gbapal.lz"
