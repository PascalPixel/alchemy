	.global Field_ShrineOfTheSeaGod8
Field_ShrineOfTheSeaGod8:
	map_header 0, 0, 90, 100, 3, 1, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 100, 16, 16, 0, 0, 0xff, 0xff
	map_layer 100, 0, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_ShrineOfTheSeaGod8, .LField_ShrineOfTheSeaGod8_CELL_PICTURES
	map_block Field_ShrineOfTheSeaGod8, .LField_ShrineOfTheSeaGod8_SHAPES
	map_block Field_ShrineOfTheSeaGod8, .LField_ShrineOfTheSeaGod8_GRID
	map_block Field_ShrineOfTheSeaGod8, .LField_ShrineOfTheSeaGod8_SHAPE_GRID
	map_block Field_ShrineOfTheSeaGod8, .LField_ShrineOfTheSeaGod8_ANIMATION
	map_block Field_ShrineOfTheSeaGod8, .LField_ShrineOfTheSeaGod8_BLEND
	map_block Field_ShrineOfTheSeaGod8, .LField_ShrineOfTheSeaGod8_SCENE
.LField_ShrineOfTheSeaGod8_CELL_PICTURES:
	.incbin "games/tla/src/map/shrine_of_the_sea_god_8/cell_pictures.delta2.lz"
	.balign 4, 0
.LField_ShrineOfTheSeaGod8_SHAPES:
	.incbin "games/tla/src/map/shrine_of_the_sea_god_8/shapes.table.lz"
	.balign 4, 0
.LField_ShrineOfTheSeaGod8_GRID:
	.incbin "games/tla/src/map/shrine_of_the_sea_god_8/map.grid.lz"
	.balign 4, 0
.LField_ShrineOfTheSeaGod8_SHAPE_GRID:
	.incbin "games/tla/src/map/shrine_of_the_sea_god_8/shape_grid.plane.lz"
	.balign 4, 0
.LField_ShrineOfTheSeaGod8_ANIMATION:
	.incbin "games/tla/src/map/shrine_of_the_sea_god_8/animation.script.lz"
	.balign 4, 0
.LField_ShrineOfTheSeaGod8_BLEND:
	.incbin "games/tla/src/map/shrine_of_the_sea_god_8/blend.script.lz"
	.balign 4, 0
.LField_ShrineOfTheSeaGod8_SCENE:
	.incbin "games/tla/src/map/shrine_of_the_sea_god_8/scene.scene.lz"
	.balign 4, 0
	.global Tileset_ShrineOfTheSeaGod8Palette
Tileset_ShrineOfTheSeaGod8Palette:
	.incbin "games/tla/src/graphics/tileset/shrine_of_the_sea_god_8/palette.gbapal.lz"
	.byte 0x8a, 0x10	@ PACKER: left in the buffer by Field_ShrineOfTheSeaGod8+0x1ae
	.global Tileset_ShrineOfTheSeaGod8TilesA
Tileset_ShrineOfTheSeaGod8TilesA:
	.incbin "games/tla/src/graphics/tileset/shrine_of_the_sea_god_8/tiles_a.4bpp.mtf"
