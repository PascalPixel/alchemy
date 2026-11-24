	.global Field_ShrineOfTheSeaGodLadders
Field_ShrineOfTheSeaGodLadders:
	map_header 0, 0, 128, 128, 3, 1, 2, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_ShrineOfTheSeaGodLadders, .LField_ShrineOfTheSeaGodLadders_CELL_TILES
	map_block Field_ShrineOfTheSeaGodLadders, .LField_ShrineOfTheSeaGodLadders_SHAPES
	map_block Field_ShrineOfTheSeaGodLadders, .LField_ShrineOfTheSeaGodLadders_GRID
	map_block Field_ShrineOfTheSeaGodLadders, .LField_ShrineOfTheSeaGodLadders_SHAPE_GRID
	map_block Field_ShrineOfTheSeaGodLadders, .LField_ShrineOfTheSeaGodLadders_ANIMATION
	map_block Field_ShrineOfTheSeaGodLadders, .LField_ShrineOfTheSeaGodLadders_BLEND
	map_block Field_ShrineOfTheSeaGodLadders, .LField_ShrineOfTheSeaGodLadders_SCENE
.LField_ShrineOfTheSeaGodLadders_CELL_TILES:
	.incbin "games/tla/src/map/shrine_of_the_sea_god_ladders/cell_tiles.delta1.lz"
	.balign 4, 0
.LField_ShrineOfTheSeaGodLadders_SHAPES:
	.incbin "games/tla/src/map/shrine_of_the_sea_god_ladders/shapes.table.lz"
	.balign 4, 0
.LField_ShrineOfTheSeaGodLadders_GRID:
	.incbin "games/tla/src/map/shrine_of_the_sea_god_ladders/map.grid.lz"
	.balign 4, 0
.LField_ShrineOfTheSeaGodLadders_SHAPE_GRID:
	.incbin "games/tla/src/map/shrine_of_the_sea_god_ladders/shape_grid.plane.lz"
	.balign 4, 0
.LField_ShrineOfTheSeaGodLadders_ANIMATION:
	.incbin "games/tla/src/map/shrine_of_the_sea_god_ladders/animation.script.lz"
	.balign 4, 0
.LField_ShrineOfTheSeaGodLadders_BLEND:
	.incbin "games/tla/src/map/shrine_of_the_sea_god_ladders/blend.script.lz"
	.balign 4, 0
.LField_ShrineOfTheSeaGodLadders_SCENE:
	.incbin "games/tla/src/map/shrine_of_the_sea_god_ladders/scene.scene.lz"
	.balign 4, 0
	.global Tileset_ShrineOfTheSeaGodLaddersPalette
Tileset_ShrineOfTheSeaGodLaddersPalette:
	.incbin "games/tla/src/graphics/tileset/shrine_of_the_sea_god_ladders/palette.gbapal.lz"
	.byte 0x85	@ PACKER: left in the buffer by Field_ShrineOfTheSeaGodLadders+0x11f
	.global Tileset_ShrineOfTheSeaGodLaddersTilesA
Tileset_ShrineOfTheSeaGodLaddersTilesA:
	.incbin "games/tla/src/graphics/tileset/shrine_of_the_sea_god_ladders/tiles_a.4bpp.mtf"
	.byte 0x86	@ PACKER: left in the buffer by Tileset_ShrineOfTheSeaGodSeaGodsTearTilesA+0x239f
	.global Tileset_ShrineOfTheSeaGodLaddersTilesB
Tileset_ShrineOfTheSeaGodLaddersTilesB:
	.incbin "games/tla/src/graphics/tileset/shrine_of_the_sea_god_ladders/tiles_b.4bpp.mtf"
	.byte 0x7f	@ PACKER: left in the buffer by Tileset_ShrineOfTheSeaGodLaddersTilesA+0x124b
	.global Tileset_ShrineOfTheSeaGodLaddersTilesC
Tileset_ShrineOfTheSeaGodLaddersTilesC:
	.incbin "games/tla/src/graphics/tileset/shrine_of_the_sea_god_ladders/tiles_c.4bpp.mtf"
	.byte 0xd1	@ PACKER: left in the buffer by Tileset_ShrineOfTheSeaGodLaddersTilesA+0x238f
	.global Tileset_ShrineOfTheSeaGodLaddersTilesD
Tileset_ShrineOfTheSeaGodLaddersTilesD:
	.incbin "games/tla/src/graphics/tileset/shrine_of_the_sea_god_ladders/tiles_d.4bpp.mtf"
	.byte 0xc1, 0x01, 0xb0	@ PACKER: left in the buffer by Tileset_ShrineOfTheSeaGodLaddersTilesC+0x18fd
