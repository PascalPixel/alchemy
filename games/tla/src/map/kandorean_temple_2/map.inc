	.global Field_KandoreanTemple2
Field_KandoreanTemple2:
	map_header 0, 0, 90, 100, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 100, 16, 16, 0, 0, 0xff, 0xff
	map_layer 100, 0, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_KandoreanTemple2, .LField_KandoreanTemple2_CELL_PICTURES
	map_block Field_KandoreanTemple2, .LField_KandoreanTemple2_SHAPES
	map_block Field_KandoreanTemple2, .LField_KandoreanTemple2_GRID
	map_block Field_KandoreanTemple2, .LField_KandoreanTemple2_SHAPE_GRID
	map_block Field_KandoreanTemple2, .LField_KandoreanTemple2_ANIMATION
	map_block Field_KandoreanTemple2, .LField_KandoreanTemple2_BLEND
	map_block Field_KandoreanTemple2, .LField_KandoreanTemple2_SCENE
.LField_KandoreanTemple2_CELL_PICTURES:
	.incbin "games/tla/src/map/kandorean_temple_2/cell_pictures.delta2.lz"
	.balign 4, 0
.LField_KandoreanTemple2_SHAPES:
	.incbin "games/tla/src/map/kandorean_temple_2/shapes.table.lz"
	.balign 4, 0
.LField_KandoreanTemple2_GRID:
	.incbin "games/tla/src/map/kandorean_temple_2/map.grid.lz"
	.balign 4, 0
.LField_KandoreanTemple2_SHAPE_GRID:
	.incbin "games/tla/src/map/kandorean_temple_2/shape_grid.plane.lz"
	.balign 4, 0
.LField_KandoreanTemple2_ANIMATION:
	.incbin "games/tla/src/map/kandorean_temple_2/animation.script.lz"
	.balign 4, 0
.LField_KandoreanTemple2_BLEND:
	.incbin "games/tla/src/map/lobby/blend.script.lz"
	.balign 4, 0
.LField_KandoreanTemple2_SCENE:
	.incbin "games/tla/src/map/kandorean_temple_2/scene.scene.lz"
	.balign 4, 0
	.global Tileset_KandoreanTemple2Palette
Tileset_KandoreanTemple2Palette:
	.incbin "games/tla/src/graphics/tileset/kandorean_temple_2/palette.gbapal.lz"
	.byte 0xce, 0xcf	@ PACKER: left in the buffer by Field_KandoreanTemple2+0x1a6
	.global Tileset_KandoreanTemple2TilesA
Tileset_KandoreanTemple2TilesA:
	.incbin "games/tla/src/graphics/tileset/kandorean_temple_2/tiles_a.4bpp.mtf"
	.global Tileset_KandoreanTemple2TilesB
Tileset_KandoreanTemple2TilesB:
	.incbin "games/tla/src/graphics/tileset/kandorean_temple_2/tiles_b.4bpp.mtf"
	.byte 0xdc	@ PACKER: left in the buffer by Tileset_KandoreanTemple2TilesA+0x13f
	.global Tileset_KandoreanTemple2TilesC
Tileset_KandoreanTemple2TilesC:
	.incbin "games/tla/src/graphics/tileset/kandorean_temple_2/tiles_c.4bpp.mtf"
	.byte 0xd0, 0x88, 0xf9	@ PACKER: left in the buffer by Tileset_KandoreanTemple2TilesA+0xca5
	.global Tileset_KandoreanTemple2TilesD
Tileset_KandoreanTemple2TilesD:
	.incbin "games/tla/src/graphics/tileset/kandorean_temple_2/tiles_d.4bpp.mtf"
	.byte 0x72, 0x70, 0xf9	@ PACKER: left in the buffer by Tileset_KandoreanTemple2TilesA+0x15cd
