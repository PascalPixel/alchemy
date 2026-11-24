	.global Field_MadraCatacombsLadders
Field_MadraCatacombsLadders:
	map_header 0, 0, 128, 128, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_MadraCatacombsLadders, .LField_MadraCatacombsLadders_CELL_TILES
	map_block Field_MadraCatacombsLadders, .LField_MadraCatacombsLadders_SHAPES
	map_block Field_MadraCatacombsLadders, .LField_MadraCatacombsLadders_GRID
	map_block Field_MadraCatacombsLadders, .LField_MadraCatacombsLadders_SHAPE_GRID
	map_block Field_MadraCatacombsLadders, .LField_MadraCatacombsLadders_ANIMATION
	map_block Field_MadraCatacombsLadders, .LField_MadraCatacombsLadders_BLEND
	map_block Field_MadraCatacombsLadders, .LField_MadraCatacombsLadders_SCENE
.LField_MadraCatacombsLadders_CELL_TILES:
	.incbin "games/tla/src/map/madra_catacombs_ladders/cell_tiles.delta2.lz"
	.balign 4, 0
.LField_MadraCatacombsLadders_SHAPES:
	.incbin "games/tla/src/map/madra_catacombs_ladders/shapes.table.lz"
	.balign 4, 0
.LField_MadraCatacombsLadders_GRID:
	.incbin "games/tla/src/map/madra_catacombs_ladders/map.grid.lz"
	.balign 4, 0
.LField_MadraCatacombsLadders_SHAPE_GRID:
	.incbin "games/tla/src/map/madra_catacombs_ladders/shape_grid.plane.lz"
	.balign 4, 0
.LField_MadraCatacombsLadders_ANIMATION:
	.incbin "games/tla/src/map/kandorean_temple_mimic/animation.script.lz"
	.balign 4, 0
.LField_MadraCatacombsLadders_BLEND:
	.incbin "games/tla/src/map/madra_catacombs_ladders/blend.script.lz"
	.balign 4, 0
.LField_MadraCatacombsLadders_SCENE:
	.incbin "games/tla/src/map/madra_catacombs_ladders/scene.scene.lz"
	.global Tileset_MadraCatacombsLaddersPalette
Tileset_MadraCatacombsLaddersPalette:
	.incbin "games/tla/src/graphics/tileset/madra_catacombs_ladders/palette.gbapal.lz"
	.global Tileset_MadraCatacombsLaddersTilesA
Tileset_MadraCatacombsLaddersTilesA:
	.incbin "games/tla/src/graphics/tileset/madra_catacombs_ladders/tiles_a.4bpp.mtf"
	.byte 0xdf	@ PACKER: left in the buffer by Tileset_MadraCatacombsEntranceTilesA+0x1f5b
	.global Tileset_MadraCatacombsLaddersTilesB
Tileset_MadraCatacombsLaddersTilesB:
	.incbin "games/tla/src/graphics/tileset/madra_catacombs_ladders/tiles_b.4bpp.mtf"
	.byte 0xfa, 0x5c	@ PACKER: left in the buffer by Tileset_MadraCatacombsLaddersTilesA+0x185e
	.global Tileset_MadraCatacombsLaddersTilesC
Tileset_MadraCatacombsLaddersTilesC:
	.incbin "games/tla/src/graphics/tileset/madra_catacombs_ladders/tiles_c.4bpp.mtf"
	.byte 0xa4	@ PACKER: left in the buffer by Tileset_MadraCatacombsLaddersTilesB+0x60f
	.global Tileset_MadraCatacombsLaddersTilesD
Tileset_MadraCatacombsLaddersTilesD:
	.incbin "games/tla/src/graphics/tileset/madra_catacombs_ladders/tiles_d.4bpp.mtf"
	.byte 0x05	@ PACKER: left in the buffer by Tileset_MadraCatacombsLaddersTilesC+0x13f
