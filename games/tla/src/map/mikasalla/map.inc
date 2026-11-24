	.global Field_Mikasalla
Field_Mikasalla:
	map_header 0, 0, 128, 128, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Mikasalla, .LField_Mikasalla_CELL_TILES
	map_block Field_Mikasalla, .LField_Mikasalla_SHAPES
	map_block Field_Mikasalla, .LField_Mikasalla_GRID
	map_block Field_Mikasalla, .LField_Mikasalla_SHAPE_GRID
	map_block Field_Mikasalla, .LField_Mikasalla_ANIMATION
	map_block Field_Mikasalla, .LField_Mikasalla_BLEND
	map_block Field_Mikasalla, .LField_Mikasalla_SCENE
.LField_Mikasalla_CELL_TILES:
	.incbin "games/tla/src/map/mikasalla/cell_tiles.delta1.lz"
	.balign 4, 0
.LField_Mikasalla_SHAPES:
	.incbin "games/tla/src/map/mikasalla/shapes.table.lz"
	.balign 4, 0
.LField_Mikasalla_GRID:
	.incbin "games/tla/src/map/mikasalla/map.grid.lz"
	.balign 4, 0
.LField_Mikasalla_SHAPE_GRID:
	.incbin "games/tla/src/map/mikasalla/shape_grid.plane.lz"
	.balign 4, 0
.LField_Mikasalla_ANIMATION:
	.incbin "games/tla/src/map/mikasalla/animation.script.lz"
	.balign 4, 0
.LField_Mikasalla_BLEND:
	.incbin "games/tla/src/map/lobby/blend.script.lz"
	.balign 4, 0
.LField_Mikasalla_SCENE:
	.incbin "games/tla/src/map/mikasalla/scene.scene.lz"
	.global Tileset_MikasallaPalette
Tileset_MikasallaPalette:
	.incbin "games/tla/src/graphics/tileset/mikasalla/palette.gbapal.lz"
	.byte 0x2f, 0x43, 0x0e	@ PACKER: left in the buffer by Field_Mikasalla+0x159
	.global Tileset_MikasallaTilesA
Tileset_MikasallaTilesA:
	.incbin "games/tla/src/graphics/tileset/mikasalla/tiles_a.4bpp.mtf"
	.byte 0x08, 0x80	@ PACKER: left in the buffer by Tileset_OseniaCliffsTilesB+0x21de
	.global Tileset_MikasallaTilesB
Tileset_MikasallaTilesB:
	.incbin "games/tla/src/graphics/tileset/mikasalla/tiles_b.4bpp.mtf"
	.ifdef TLA_EDITION_EN
	.balign 4, 0	@ PACKER: zero fill, not an earlier stream's bytes
	.endif
	.balign 4, 0
