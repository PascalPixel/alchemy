	.global Field_YampiDesertPsynergyStone
Field_YampiDesertPsynergyStone:
	map_header 0, 0, 128, 128, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, 2, 2, 0xf, 0xf
	map_block Field_YampiDesertPsynergyStone, .LField_YampiDesertPsynergyStone_CELL_TILES
	map_block Field_YampiDesertPsynergyStone, .LField_YampiDesertPsynergyStone_SHAPES
	map_block Field_YampiDesertPsynergyStone, .LField_YampiDesertPsynergyStone_GRID
	map_block Field_YampiDesertPsynergyStone, .LField_YampiDesertPsynergyStone_SHAPE_GRID
	map_block Field_YampiDesertPsynergyStone, .LField_YampiDesertPsynergyStone_ANIMATION
	map_block Field_YampiDesertPsynergyStone, .LField_YampiDesertPsynergyStone_BLEND
	map_block Field_YampiDesertPsynergyStone, .LField_YampiDesertPsynergyStone_SCENE
.LField_YampiDesertPsynergyStone_CELL_TILES:
	.incbin "games/tla/src/map/yampi_desert_psynergy_stone/cell_tiles.delta2.lz"
	.balign 4, 0
.LField_YampiDesertPsynergyStone_SHAPES:
	.incbin "games/tla/src/map/yampi_desert_psynergy_stone/shapes.table.lz"
	.balign 4, 0
.LField_YampiDesertPsynergyStone_GRID:
	.incbin "games/tla/src/map/yampi_desert_psynergy_stone/map.grid.lz"
	.balign 4, 0
.LField_YampiDesertPsynergyStone_SHAPE_GRID:
	.incbin "games/tla/src/map/yampi_desert_psynergy_stone/shape_grid.plane.lz"
	.balign 4, 0
.LField_YampiDesertPsynergyStone_ANIMATION:
	.incbin "games/tla/src/map/yampi_desert_blitz/animation.script.lz"
	.balign 4, 0
.LField_YampiDesertPsynergyStone_BLEND:
	.incbin "games/tla/src/map/yampi_desert_guardian_ring_chest/blend.script.lz"
	.balign 4, 0
.LField_YampiDesertPsynergyStone_SCENE:
	.incbin "games/tla/src/map/yampi_desert_psynergy_stone/scene.scene.lz"
	.balign 4, 0
