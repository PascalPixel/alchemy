	.global Field_ShrineOfTheSeaGodSeaGodsTear
Field_ShrineOfTheSeaGodSeaGodsTear:
	map_header 0, 0, 90, 100, 3, 1, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 100, 16, 16, 0, 0, 0xff, 0xff
	map_layer 100, 0, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_ShrineOfTheSeaGodSeaGodsTear, .LField_ShrineOfTheSeaGodSeaGodsTear_CELL_TILES
	map_block Field_ShrineOfTheSeaGodSeaGodsTear, .LField_ShrineOfTheSeaGodSeaGodsTear_SHAPES
	map_block Field_ShrineOfTheSeaGodSeaGodsTear, .LField_ShrineOfTheSeaGodSeaGodsTear_GRID
	map_block Field_ShrineOfTheSeaGodSeaGodsTear, .LField_ShrineOfTheSeaGodSeaGodsTear_SHAPE_GRID
	map_block Field_ShrineOfTheSeaGodSeaGodsTear, .LField_ShrineOfTheSeaGodSeaGodsTear_ANIMATION
	map_block Field_ShrineOfTheSeaGodSeaGodsTear, .LField_ShrineOfTheSeaGodSeaGodsTear_BLEND
	map_block Field_ShrineOfTheSeaGodSeaGodsTear, .LField_ShrineOfTheSeaGodSeaGodsTear_SCENE
.LField_ShrineOfTheSeaGodSeaGodsTear_CELL_TILES:
	.incbin "games/tla/src/map/shrine_of_the_sea_god_sea_gods_tear/cell_tiles.delta2.lz"
	.balign 4, 0
.LField_ShrineOfTheSeaGodSeaGodsTear_SHAPES:
	.incbin "games/tla/src/map/shrine_of_the_sea_god_sea_gods_tear/shapes.table.lz"
	.balign 4, 0
.LField_ShrineOfTheSeaGodSeaGodsTear_GRID:
	.incbin "games/tla/src/map/shrine_of_the_sea_god_sea_gods_tear/map.grid.lz"
	.balign 4, 0
.LField_ShrineOfTheSeaGodSeaGodsTear_SHAPE_GRID:
	.incbin "games/tla/src/map/shrine_of_the_sea_god_sea_gods_tear/shape_grid.plane.lz"
	.balign 4, 0
.LField_ShrineOfTheSeaGodSeaGodsTear_ANIMATION:
	.incbin "games/tla/src/map/shrine_of_the_sea_god_sea_gods_tear/animation.script.lz"
	.balign 4, 0
.LField_ShrineOfTheSeaGodSeaGodsTear_BLEND:
	.incbin "games/tla/src/map/shrine_of_the_sea_god_sea_gods_tear/blend.script.lz"
	.balign 4, 0
.LField_ShrineOfTheSeaGodSeaGodsTear_SCENE:
	.incbin "games/tla/src/map/shrine_of_the_sea_god_sea_gods_tear/scene.scene.lz"
	.balign 4, 0
	.global Tileset_ShrineOfTheSeaGodSeaGodsTearPalette
Tileset_ShrineOfTheSeaGodSeaGodsTearPalette:
	.incbin "games/tla/src/graphics/tileset/shrine_of_the_sea_god_sea_gods_tear/palette.gbapal.lz"
	.byte 0x8a, 0x10	@ PACKER: left in the buffer by Field_ShrineOfTheSeaGod8+0x1ae
	.global Tileset_ShrineOfTheSeaGodSeaGodsTearTilesA
Tileset_ShrineOfTheSeaGodSeaGodsTearTilesA:
	.incbin "games/tla/src/graphics/tileset/shrine_of_the_sea_god_sea_gods_tear/tiles_a.4bpp.mtf"
