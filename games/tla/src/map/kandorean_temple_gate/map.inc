	.global Field_KandoreanTempleGate
Field_KandoreanTempleGate:
	map_header 0, 0, 120, 120, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 120, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 120, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_KandoreanTempleGate, .LField_KandoreanTempleGate_CELL_TILES
	map_block Field_KandoreanTempleGate, .LField_KandoreanTempleGate_SHAPES
	map_block Field_KandoreanTempleGate, .LField_KandoreanTempleGate_GRID
	map_block Field_KandoreanTempleGate, .LField_KandoreanTempleGate_SHAPE_GRID
	map_block Field_KandoreanTempleGate, .LField_KandoreanTempleGate_ANIMATION
	map_block Field_KandoreanTempleGate
	map_block Field_KandoreanTempleGate, .LField_KandoreanTempleGate_SCENE
.LField_KandoreanTempleGate_CELL_TILES:
	.incbin "games/tla/src/map/kandorean_temple_gate/cell_tiles.delta1.lz"
	.balign 4, 0
.LField_KandoreanTempleGate_SHAPES:
	.incbin "games/tla/src/map/kandorean_temple_gate/shapes.table.lz"
	.balign 4, 0
.LField_KandoreanTempleGate_GRID:
	.incbin "games/tla/src/map/kandorean_temple_gate/map.grid.lz"
	.balign 4, 0
.LField_KandoreanTempleGate_SHAPE_GRID:
	.incbin "games/tla/src/map/kandorean_temple_gate/shape_grid.plane.lz"
	.balign 4, 0
.LField_KandoreanTempleGate_ANIMATION:
	.incbin "games/tla/src/map/kandorean_temple_mimic/animation.script.lz"
	.balign 4, 0
.LField_KandoreanTempleGate_SCENE:
	.incbin "games/tla/src/map/kandorean_temple_gate/scene.scene.lz"
	.global Tileset_KandoreanTempleGatePalette
Tileset_KandoreanTempleGatePalette:
	.incbin "games/tla/src/graphics/tileset/kandorean_temple_gate/palette.gbapal.lz"
	.byte 0x19, 0xc6, 0x8f	@ PACKER: left in the buffer by Field_KandoreanTempleGate+0x179
	.global Tileset_KandoreanTempleGateTilesA
Tileset_KandoreanTempleGateTilesA:
	.incbin "games/tla/src/graphics/tileset/kandorean_temple_gate/tiles_a.4bpp.mtf"
	.byte 0x14	@ PACKER: left in the buffer by Field_KandoreanTempleGate+0x1ae3
