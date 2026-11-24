	.global Field_AirsRockInsidePsynergyStone
Field_AirsRockInsidePsynergyStone:
	map_header 0, 0, 64, 64, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 64, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 64, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_AirsRockInsidePsynergyStone, .LField_AirsRockInsidePsynergyStone_CELL_TILES
	map_block Field_AirsRockInsidePsynergyStone, .LField_AirsRockInsidePsynergyStone_SHAPES
	map_block Field_AirsRockInsidePsynergyStone, .LField_AirsRockInsidePsynergyStone_GRID
	map_block Field_AirsRockInsidePsynergyStone, .LField_AirsRockInsidePsynergyStone_SHAPE_GRID
	map_block Field_AirsRockInsidePsynergyStone, .LField_AirsRockInsidePsynergyStone_ANIMATION
	map_block Field_AirsRockInsidePsynergyStone, .LField_AirsRockInsidePsynergyStone_BLEND
	map_block Field_AirsRockInsidePsynergyStone, .LField_AirsRockInsidePsynergyStone_SCENE
.LField_AirsRockInsidePsynergyStone_CELL_TILES:
	.incbin "games/tla/src/map/airs_rock_inside_psynergy_stone/cell_tiles.delta2.lz"
	.balign 4, 0
.LField_AirsRockInsidePsynergyStone_SHAPES:
	.incbin "games/tla/src/map/airs_rock_inside_psynergy_stone/shapes.table.lz"
	.balign 4, 0
.LField_AirsRockInsidePsynergyStone_GRID:
	.incbin "games/tla/src/map/airs_rock_inside_psynergy_stone/map.grid.lz"
	.balign 4, 0
.LField_AirsRockInsidePsynergyStone_SHAPE_GRID:
	.incbin "games/tla/src/map/airs_rock_inside_psynergy_stone/shape_grid.plane.lz"
	.balign 4, 0
.LField_AirsRockInsidePsynergyStone_ANIMATION:
	.incbin "games/tla/src/map/kandorean_temple_mimic/animation.script.lz"
	.balign 4, 0
.LField_AirsRockInsidePsynergyStone_BLEND:
	.incbin "games/tla/src/map/lobby/blend.script.lz"
	.balign 4, 0
.LField_AirsRockInsidePsynergyStone_SCENE:
	.incbin "games/tla/src/map/airs_rock_inside_psynergy_stone/scene.scene.lz"
	.balign 4, 0
	.global Tileset_AirsRockInsidePsynergyStonePalette
Tileset_AirsRockInsidePsynergyStonePalette:
	.incbin "games/tla/src/graphics/tileset/airs_rock_inside_psynergy_stone/palette.gbapal.lz"
	.byte 0xe1	@ PACKER: left in the buffer by Field_AirsRockInsidePsynergyStone+0xbb
	.global Tileset_AirsRockInsidePsynergyStoneTilesA
Tileset_AirsRockInsidePsynergyStoneTilesA:
	.incbin "games/tla/src/graphics/tileset/airs_rock_inside_psynergy_stone/tiles_a.4bpp.mtf"
	.byte 0x80, 0x00	@ PACKER: left in the buffer by Field_AirsRockInside9+0xd3e
	.global Tileset_AirsRockInsidePsynergyStoneTilesB
Tileset_AirsRockInsidePsynergyStoneTilesB:
	.incbin "games/tla/src/graphics/tileset/airs_rock_inside_psynergy_stone/tiles_b.4bpp.mtf"
	.global Tileset_AirsRockInsidePsynergyStoneTilesC
Tileset_AirsRockInsidePsynergyStoneTilesC:
	.incbin "games/tla/src/graphics/tileset/airs_rock_inside_psynergy_stone/tiles_c.4bpp.mtf"
	.byte 0xca	@ PACKER: left in the buffer by Tileset_AirsRockInsidePsynergyStoneTilesB+0x13f
	.global Tileset_AirsRockInsidePsynergyStoneTilesD
Tileset_AirsRockInsidePsynergyStoneTilesD:
	.incbin "games/tla/src/graphics/tileset/airs_rock_inside_psynergy_stone/tiles_d.4bpp.mtf"
	.byte 0x13	@ PACKER: left in the buffer by Tileset_AirsRockInsidePsynergyStoneTilesA+0xc17
