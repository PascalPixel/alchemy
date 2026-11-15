	.global Field_ShrineOfTheSeaGod10
Field_ShrineOfTheSeaGod10:
	map_header 0, 0, 128, 128, 3, 1, 2, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_ShrineOfTheSeaGod10, .LField_ShrineOfTheSeaGod10_CELL_PICTURES
	map_block Field_ShrineOfTheSeaGod10, .LField_ShrineOfTheSeaGod10_SHAPES
	map_block Field_ShrineOfTheSeaGod10, .LField_ShrineOfTheSeaGod10_GRID
	map_block Field_ShrineOfTheSeaGod10, .LField_ShrineOfTheSeaGod10_SHAPE_GRID
	map_block Field_ShrineOfTheSeaGod10, .LField_ShrineOfTheSeaGod10_ANIMATION
	map_block Field_ShrineOfTheSeaGod10, .LField_ShrineOfTheSeaGod10_BLEND
	map_block Field_ShrineOfTheSeaGod10, .LField_ShrineOfTheSeaGod10_SCENE
.LField_ShrineOfTheSeaGod10_CELL_PICTURES:
	.incbin "games/tla/src/map/shrine_of_the_sea_god_10/cell_pictures.delta1.lz"
	.balign 4, 0
.LField_ShrineOfTheSeaGod10_SHAPES:
	.incbin "games/tla/src/map/shrine_of_the_sea_god_10/shapes.table.lz"
	.balign 4, 0
.LField_ShrineOfTheSeaGod10_GRID:
	.incbin "games/tla/src/map/shrine_of_the_sea_god_10/map.grid.lz"
	.balign 4, 0
.LField_ShrineOfTheSeaGod10_SHAPE_GRID:
	.incbin "games/tla/src/map/shrine_of_the_sea_god_10/shape_grid.plane.lz"
	.balign 4, 0
.LField_ShrineOfTheSeaGod10_ANIMATION:
	.incbin "games/tla/src/map/shrine_of_the_sea_god_10/animation.script.lz"
	.balign 4, 0
.LField_ShrineOfTheSeaGod10_BLEND:
	.incbin "games/tla/src/map/shrine_of_the_sea_god_10/blend.script.lz"
	.balign 4, 0
.LField_ShrineOfTheSeaGod10_SCENE:
	.incbin "games/tla/src/map/shrine_of_the_sea_god_10/scene.scene.lz"
	.balign 4, 0
	.global Tileset_ShrineOfTheSeaGod10Palette
Tileset_ShrineOfTheSeaGod10Palette:
	.incbin "games/tla/src/graphics/tileset/shrine_of_the_sea_god_10/palette.gbapal.lz"
	.byte 0x85	@ PACKER: left in the buffer by Field_ShrineOfTheSeaGod10+0x11f
	.global Tileset_ShrineOfTheSeaGod10TilesA
Tileset_ShrineOfTheSeaGod10TilesA:
	.incbin "games/tla/src/graphics/tileset/shrine_of_the_sea_god_10/tiles_a.4bpp.mtf"
	.byte 0x86	@ PACKER: left in the buffer by Tileset_ShrineOfTheSeaGodSeaGodsTearTilesA+0x239f
	.global Tileset_ShrineOfTheSeaGod10TilesB
Tileset_ShrineOfTheSeaGod10TilesB:
	.incbin "games/tla/src/graphics/tileset/shrine_of_the_sea_god_10/tiles_b.4bpp.mtf"
	.byte 0x7f	@ PACKER: left in the buffer by Tileset_ShrineOfTheSeaGod10TilesA+0x124b
	.global Tileset_ShrineOfTheSeaGod10TilesC
Tileset_ShrineOfTheSeaGod10TilesC:
	.incbin "games/tla/src/graphics/tileset/shrine_of_the_sea_god_10/tiles_c.4bpp.mtf"
	.byte 0xd1	@ PACKER: left in the buffer by Tileset_ShrineOfTheSeaGod10TilesA+0x238f
	.global Tileset_ShrineOfTheSeaGod10TilesD
Tileset_ShrineOfTheSeaGod10TilesD:
	.incbin "games/tla/src/graphics/tileset/shrine_of_the_sea_god_10/tiles_d.4bpp.mtf"
	.byte 0xc1, 0x01, 0xb0	@ PACKER: left in the buffer by Tileset_ShrineOfTheSeaGod10TilesC+0x18fd
