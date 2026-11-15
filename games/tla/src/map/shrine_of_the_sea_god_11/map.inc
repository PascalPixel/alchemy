	.global Field_ShrineOfTheSeaGod11
Field_ShrineOfTheSeaGod11:
	map_header 0, 0, 128, 128, 2, 2, 3, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 12, 12, 0, 0, 0xff, 0xff
	map_block Field_ShrineOfTheSeaGod11, .LField_ShrineOfTheSeaGod11_CELL_PICTURES
	map_block Field_ShrineOfTheSeaGod11, .LField_ShrineOfTheSeaGod11_SHAPES
	map_block Field_ShrineOfTheSeaGod11, .LField_ShrineOfTheSeaGod11_GRID
	map_block Field_ShrineOfTheSeaGod11, .LField_ShrineOfTheSeaGod11_SHAPE_GRID
	map_block Field_ShrineOfTheSeaGod11, .LField_ShrineOfTheSeaGod11_ANIMATION
	map_block Field_ShrineOfTheSeaGod11
	map_block Field_ShrineOfTheSeaGod11, .LField_ShrineOfTheSeaGod11_SCENE
.LField_ShrineOfTheSeaGod11_CELL_PICTURES:
	.incbin "games/tla/src/map/shrine_of_the_sea_god_11/cell_pictures.delta1.lz"
	.balign 4, 0
.LField_ShrineOfTheSeaGod11_SHAPES:
	.incbin "games/tla/src/map/shrine_of_the_sea_god_11/shapes.table.lz"
	.balign 4, 0
.LField_ShrineOfTheSeaGod11_GRID:
	.incbin "games/tla/src/map/shrine_of_the_sea_god_11/map.grid.lz"
	.balign 4, 0
.LField_ShrineOfTheSeaGod11_SHAPE_GRID:
	.incbin "games/tla/src/map/shrine_of_the_sea_god_11/shape_grid.plane.lz"
	.balign 4, 0
.LField_ShrineOfTheSeaGod11_ANIMATION:
	.incbin "games/tla/src/map/shrine_of_the_sea_god_11/animation.script.lz"
	.balign 4, 0
.LField_ShrineOfTheSeaGod11_SCENE:
	.incbin "games/tla/src/map/shrine_of_the_sea_god_11/scene.scene.lz"
	.global Tileset_ShrineOfTheSeaGod11Palette
Tileset_ShrineOfTheSeaGod11Palette:
	.incbin "games/tla/src/graphics/tileset/shrine_of_the_sea_god_11/palette.gbapal.lz"
	.byte 0x27, 0x1f	@ PACKER: left in the buffer by Field_ShrineOfTheSeaGod11+0x16e
	.global Tileset_ShrineOfTheSeaGod11TilesA
Tileset_ShrineOfTheSeaGod11TilesA:
	.incbin "games/tla/src/graphics/tileset/shrine_of_the_sea_god_11/tiles_a.4bpp.mtf"
	.byte 0x81, 0x45	@ PACKER: left in the buffer by Tileset_ShrineOfTheSeaGod8TilesA+0x259a
	.global Tileset_ShrineOfTheSeaGod11TilesB
Tileset_ShrineOfTheSeaGod11TilesB:
	.incbin "games/tla/src/graphics/tileset/shrine_of_the_sea_god_11/tiles_b.4bpp.mtf"
	.global Tileset_ShrineOfTheSeaGod11TilesC
Tileset_ShrineOfTheSeaGod11TilesC:
	.incbin "games/tla/src/graphics/tileset/shrine_of_the_sea_god_11/tiles_c.4bpp.mtf"
	.byte 0xd8, 0x18	@ PACKER: left in the buffer by Tileset_ShrineOfTheSeaGod11TilesB+0x1e86
	.global Tileset_ShrineOfTheSeaGod11TilesD
Tileset_ShrineOfTheSeaGod11TilesD:
	.incbin "games/tla/src/graphics/tileset/shrine_of_the_sea_god_11/tiles_d.4bpp.mtf"
	.byte 0x08	@ PACKER: left in the buffer by Tileset_ShrineOfTheSeaGod11TilesC+0x6c3
