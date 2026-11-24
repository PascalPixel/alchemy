	.global Field_KandoreanTempleMimic
Field_KandoreanTempleMimic:
	map_header 0, 0, 90, 100, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 100, 16, 16, 0, 0, 0xff, 0xff
	map_layer 100, 0, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_KandoreanTempleMimic, .LField_KandoreanTempleMimic_CELL_TILES
	map_block Field_KandoreanTempleMimic, .LField_KandoreanTempleMimic_SHAPES
	map_block Field_KandoreanTempleMimic, .LField_KandoreanTempleMimic_GRID
	map_block Field_KandoreanTempleMimic, .LField_KandoreanTempleMimic_SHAPE_GRID
	map_block Field_KandoreanTempleMimic, .LField_KandoreanTempleMimic_ANIMATION
	map_block Field_KandoreanTempleMimic, .LField_KandoreanTempleMimic_BLEND
	map_block Field_KandoreanTempleMimic, .LField_KandoreanTempleMimic_SCENE
.LField_KandoreanTempleMimic_CELL_TILES:
	.incbin "games/tla/src/map/kandorean_temple_mimic/cell_tiles.delta2.lz"
	.balign 4, 0
.LField_KandoreanTempleMimic_SHAPES:
	.incbin "games/tla/src/map/kandorean_temple_mimic/shapes.table.lz"
	.balign 4, 0
.LField_KandoreanTempleMimic_GRID:
	.incbin "games/tla/src/map/kandorean_temple_mimic/map.grid.lz"
	.balign 4, 0
.LField_KandoreanTempleMimic_SHAPE_GRID:
	.incbin "games/tla/src/map/kandorean_temple_mimic/shape_grid.plane.lz"
	.balign 4, 0
.LField_KandoreanTempleMimic_ANIMATION:
	.incbin "games/tla/src/map/kandorean_temple_mimic/animation.script.lz"
	.balign 4, 0
.LField_KandoreanTempleMimic_BLEND:
	.incbin "games/tla/src/map/lobby/blend.script.lz"
	.balign 4, 0
.LField_KandoreanTempleMimic_SCENE:
	.incbin "games/tla/src/map/kandorean_temple_mimic/scene.scene.lz"
	.balign 4, 0
	.global Tileset_KandoreanTempleMimicPalette
Tileset_KandoreanTempleMimicPalette:
	.incbin "games/tla/src/graphics/tileset/kandorean_temple_mimic/palette.gbapal.lz"
	.byte 0xce, 0xcf	@ PACKER: left in the buffer by Field_KandoreanTemple2+0x1a6
	.global Tileset_KandoreanTempleMimicTilesA
Tileset_KandoreanTempleMimicTilesA:
	.incbin "games/tla/src/graphics/tileset/kandorean_temple_mimic/tiles_a.4bpp.mtf"
	.global Tileset_KandoreanTempleMimicTilesB
Tileset_KandoreanTempleMimicTilesB:
	.incbin "games/tla/src/graphics/tileset/kandorean_temple_mimic/tiles_b.4bpp.mtf"
	.byte 0xdc	@ PACKER: left in the buffer by Tileset_KandoreanTempleMimicTilesA+0x13f
	.global Tileset_KandoreanTempleMimicTilesC
Tileset_KandoreanTempleMimicTilesC:
	.incbin "games/tla/src/graphics/tileset/kandorean_temple_mimic/tiles_c.4bpp.mtf"
	.byte 0xd0, 0x88, 0xf9	@ PACKER: left in the buffer by Tileset_KandoreanTempleMimicTilesA+0xca5
	.global Tileset_KandoreanTempleMimicTilesD
Tileset_KandoreanTempleMimicTilesD:
	.incbin "games/tla/src/graphics/tileset/kandorean_temple_mimic/tiles_d.4bpp.mtf"
	.byte 0x72, 0x70, 0xf9	@ PACKER: left in the buffer by Tileset_KandoreanTempleMimicTilesA+0x15cd
