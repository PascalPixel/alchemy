	.global Field_SuhallaGatePsynergyStone
Field_SuhallaGatePsynergyStone:
	map_header 0, 0, 94, 94, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 96, 16, 16, 0, 0, 0xff, 0xff
	map_layer 100, 0, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_SuhallaGatePsynergyStone, .LField_SuhallaGatePsynergyStone_CELL_TILES
	map_block Field_SuhallaGatePsynergyStone, .LField_SuhallaGatePsynergyStone_SHAPES
	map_block Field_SuhallaGatePsynergyStone, .LField_SuhallaGatePsynergyStone_GRID
	map_block Field_SuhallaGatePsynergyStone, .LField_SuhallaGatePsynergyStone_SHAPE_GRID
	map_block Field_SuhallaGatePsynergyStone, .LField_SuhallaGatePsynergyStone_ANIMATION
	map_block Field_SuhallaGatePsynergyStone, .LField_SuhallaGatePsynergyStone_BLEND
	map_block Field_SuhallaGatePsynergyStone, .LField_SuhallaGatePsynergyStone_SCENE
.LField_SuhallaGatePsynergyStone_CELL_TILES:
	.incbin "games/tla/src/map/suhalla_gate_psynergy_stone/cell_tiles.delta1.lz"
	.balign 4, 0
.LField_SuhallaGatePsynergyStone_SHAPES:
	.incbin "games/tla/src/map/suhalla_gate_psynergy_stone/shapes.table.lz"
	.balign 4, 0
.LField_SuhallaGatePsynergyStone_GRID:
	.incbin "games/tla/src/map/suhalla_gate_psynergy_stone/map.grid.lz"
	.balign 4, 0
.LField_SuhallaGatePsynergyStone_SHAPE_GRID:
	.incbin "games/tla/src/map/suhalla_gate_psynergy_stone/shape_grid.plane.lz"
	.balign 4, 0
.LField_SuhallaGatePsynergyStone_ANIMATION:
	.incbin "games/tla/src/map/suhalla_gate_psynergy_stone/animation.script.lz"
	.balign 4, 0
.LField_SuhallaGatePsynergyStone_BLEND:
	.incbin "games/tla/src/map/suhalla_gate_psynergy_stone/blend.script.lz"
	.balign 4, 0
.LField_SuhallaGatePsynergyStone_SCENE:
	.incbin "games/tla/src/map/suhalla_gate_psynergy_stone/scene.scene.lz"
	.global Tileset_SuhallaGatePsynergyStonePalette
Tileset_SuhallaGatePsynergyStonePalette:
	.incbin "games/tla/src/graphics/tileset/suhalla_gate_psynergy_stone/palette.gbapal.lz"
	.byte 0x1f, 0x44	@ PACKER: left in the buffer by Field_SuhallaGate3+0x1b6
	.global Tileset_SuhallaGatePsynergyStoneTilesA
Tileset_SuhallaGatePsynergyStoneTilesA:
	.incbin "games/tla/src/graphics/tileset/suhalla_gate_psynergy_stone/tiles_a.4bpp.mtf"
	.byte 0xd1	@ PACKER: left in the buffer by Tileset_SuhallaGateRoadTilesA+0x2afb
	.global Tileset_SuhallaGatePsynergyStoneTilesB
Tileset_SuhallaGatePsynergyStoneTilesB:
	.incbin "games/tla/src/graphics/tileset/suhalla_gate_psynergy_stone/tiles_b.4bpp.mtf"
	.byte 0x00, 0xa0, 0xbb	@ PACKER: left in the buffer by Tileset_SuhallaGatePsynergyStoneTilesA+0x20dd
	.global Tileset_SuhallaGatePsynergyStoneTilesC
Tileset_SuhallaGatePsynergyStoneTilesC:
	.incbin "games/tla/src/graphics/tileset/suhalla_gate_psynergy_stone/tiles_c.4bpp.mtf"
	.byte 0x27, 0xe9, 0x0b	@ PACKER: left in the buffer by Tileset_SuhallaGatePsynergyStoneTilesA+0x2249
	.global Tileset_SuhallaGatePsynergyStoneTilesD
Tileset_SuhallaGatePsynergyStoneTilesD:
	.incbin "games/tla/src/graphics/tileset/suhalla_gate_psynergy_stone/tiles_d.4bpp.mtf"
	.byte 0xf0	@ PACKER: left in the buffer by Tileset_SuhallaGatePsynergyStoneTilesC+0x13f
