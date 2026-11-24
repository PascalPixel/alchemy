	.global Field_GondowanSettlementCave
Field_GondowanSettlementCave:
	map_header 0, 0, 128, 128, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_GondowanSettlementCave, .LField_GondowanSettlementCave_CELL_TILES
	map_block Field_GondowanSettlementCave, .LField_GondowanSettlementCave_SHAPES
	map_block Field_GondowanSettlementCave, .LField_GondowanSettlementCave_GRID
	map_block Field_GondowanSettlementCave, .LField_GondowanSettlementCave_SHAPE_GRID
	map_block Field_GondowanSettlementCave, .LField_GondowanSettlementCave_ANIMATION
	map_block Field_GondowanSettlementCave, .LField_GondowanSettlementCave_BLEND
	map_block Field_GondowanSettlementCave, .LField_GondowanSettlementCave_SCENE
.LField_GondowanSettlementCave_CELL_TILES:
	.incbin "games/tla/src/map/gondowan_settlement_cave/cell_tiles.delta2.lz"
	.balign 4, 0
.LField_GondowanSettlementCave_SHAPES:
	.incbin "games/tla/src/map/yampi_desert_cave_water_of_life_chest/shapes.table.lz"
	.balign 4, 0
.LField_GondowanSettlementCave_GRID:
	.incbin "games/tla/src/map/gondowan_settlement_cave/map.grid.lz"
	.balign 4, 0
.LField_GondowanSettlementCave_SHAPE_GRID:
	.incbin "games/tla/src/map/gondowan_settlement_cave/shape_grid.plane.lz"
	.balign 4, 0
.LField_GondowanSettlementCave_ANIMATION:
	.incbin "games/tla/src/map/kandorean_temple_mimic/animation.script.lz"
	.balign 4, 0
.LField_GondowanSettlementCave_BLEND:
	.incbin "games/tla/src/map/lobby/blend.script.lz"
	.balign 4, 0
.LField_GondowanSettlementCave_SCENE:
	.incbin "games/tla/src/map/gondowan_settlement_cave/scene.scene.lz"
	.balign 4, 0
	.global Tileset_GondowanSettlementCavePalette
Tileset_GondowanSettlementCavePalette:
	.incbin "games/tla/src/graphics/tileset/gondowan_settlement_cave/palette.gbapal.lz"
	.byte 0x09, 0x12, 0xe0	@ PACKER: left in the buffer by Field_GondowanSettlementCave+0x14d
	.global Tileset_GondowanSettlementCaveTilesA
Tileset_GondowanSettlementCaveTilesA:
	.incbin "games/tla/src/graphics/tileset/gondowan_settlement_cave/tiles_a.4bpp.mtf"
	.global Tileset_GondowanSettlementCaveTilesB
Tileset_GondowanSettlementCaveTilesB:
	.incbin "games/tla/src/graphics/tileset/gondowan_settlement_cave/tiles_b.4bpp.mtf"
	.byte 0x2b	@ PACKER: left in the buffer by Tileset_GondowanSettlementCaveTilesA+0x1bcf
	.global Tileset_GondowanSettlementCaveTilesC
Tileset_GondowanSettlementCaveTilesC:
	.incbin "games/tla/src/graphics/tileset/gondowan_settlement_cave/tiles_c.4bpp.mtf"
	.global Tileset_GondowanSettlementCaveTilesD
Tileset_GondowanSettlementCaveTilesD:
	.incbin "games/tla/src/graphics/tileset/gondowan_settlement_cave/tiles_d.4bpp.mtf"
	.byte 0x38, 0x5a, 0x61	@ PACKER: left in the buffer by Tileset_GondowanSettlementCaveTilesC+0xb29
