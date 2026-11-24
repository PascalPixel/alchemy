	.global Field_HesperiaSettlement
Field_HesperiaSettlement:
	map_header 0, 0, 128, 64, 3, 1, 2, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_HesperiaSettlement, .LField_HesperiaSettlement_CELL_TILES
	map_block Field_HesperiaSettlement, .LField_HesperiaSettlement_SHAPES
	map_block Field_HesperiaSettlement, .LField_HesperiaSettlement_GRID
	map_block Field_HesperiaSettlement, .LField_HesperiaSettlement_SHAPE_GRID
	map_block Field_HesperiaSettlement, .LField_HesperiaSettlement_ANIMATION
	map_block Field_HesperiaSettlement, .LField_HesperiaSettlement_BLEND
	map_block Field_HesperiaSettlement, .LField_HesperiaSettlement_SCENE
.LField_HesperiaSettlement_CELL_TILES:
	.incbin "games/tla/src/map/hesperia_settlement/cell_tiles.delta0.lz"
	.balign 4, 0
.LField_HesperiaSettlement_SHAPES:
	.incbin "games/tla/src/map/hesperia_settlement/shapes.table.lz"
	.balign 4, 0
.LField_HesperiaSettlement_GRID:
	.incbin "games/tla/src/map/hesperia_settlement/map.grid.lz"
	.balign 4, 0
.LField_HesperiaSettlement_SHAPE_GRID:
	.incbin "games/tla/src/map/hesperia_settlement/shape_grid.plane.lz"
	.balign 4, 0
.LField_HesperiaSettlement_ANIMATION:
	.incbin "games/tla/src/map/hesperia_settlement/animation.script.lz"
	.balign 4, 0
.LField_HesperiaSettlement_BLEND:
	.incbin "games/tla/src/map/lobby/blend.script.lz"
	.balign 4, 0
.LField_HesperiaSettlement_SCENE:
	.incbin "games/tla/src/map/hesperia_settlement/scene.scene.lz"
	.global Tileset_HesperiaSettlementPalette
Tileset_HesperiaSettlementPalette:
	.incbin "games/tla/src/graphics/tileset/hesperia_settlement/palette.gbapal.lz"
	.global Tileset_HesperiaSettlementTilesA
Tileset_HesperiaSettlementTilesA:
	.incbin "games/tla/src/graphics/tileset/hesperia_settlement/tiles_a.4bpp.mtf"
	.global Tileset_HesperiaSettlementTilesB
Tileset_HesperiaSettlementTilesB:
	.incbin "games/tla/src/graphics/tileset/hesperia_settlement/tiles_b.4bpp.mtf"
	.byte 0x04, 0x7a	@ PACKER: left in the buffer by Tileset_HesperiaSettlementTilesA+0x199a
	.global Tileset_HesperiaSettlementTilesC
Tileset_HesperiaSettlementTilesC:
	.incbin "games/tla/src/graphics/tileset/hesperia_settlement/tiles_c.4bpp.mtf"
	.global Tileset_HesperiaSettlementTilesD
Tileset_HesperiaSettlementTilesD:
	.incbin "games/tla/src/graphics/tileset/hesperia_settlement/tiles_d.4bpp.mtf"
	.byte 0x0d	@ PACKER: left in the buffer by Tileset_HesperiaSettlementTilesC+0xb83
