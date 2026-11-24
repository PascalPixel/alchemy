	.global Field_MikasallaNorth
Field_MikasallaNorth:
	map_header 0, 0, 128, 128, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_MikasallaNorth, .LField_MikasallaNorth_CELL_TILES
	map_block Field_MikasallaNorth, .LField_MikasallaNorth_SHAPES
	map_block Field_MikasallaNorth, .LField_MikasallaNorth_GRID
	map_block Field_MikasallaNorth, .LField_MikasallaNorth_SHAPE_GRID
	map_block Field_MikasallaNorth, .LField_MikasallaNorth_ANIMATION
	map_block Field_MikasallaNorth, .LField_MikasallaNorth_BLEND
	map_block Field_MikasallaNorth, .LField_MikasallaNorth_SCENE
.LField_MikasallaNorth_CELL_TILES:
	.incbin "games/tla/src/map/mikasalla_north/cell_tiles.delta2.lz"
	.balign 4, 0
.LField_MikasallaNorth_SHAPES:
	.incbin "games/tla/src/map/mikasalla_north/shapes.table.lz"
	.balign 4, 0
.LField_MikasallaNorth_GRID:
	.incbin "games/tla/src/map/mikasalla_north/map.grid.lz"
	.balign 4, 0
.LField_MikasallaNorth_SHAPE_GRID:
	.incbin "games/tla/src/map/mikasalla_north/shape_grid.plane.lz"
	.balign 4, 0
.LField_MikasallaNorth_ANIMATION:
	.incbin "games/tla/src/map/mikasalla_north/animation.script.lz"
	.balign 4, 0
.LField_MikasallaNorth_BLEND:
	.incbin "games/tla/src/map/mikasalla_north/blend.script.lz"
	.balign 4, 0
.LField_MikasallaNorth_SCENE:
	.incbin "games/tla/src/map/mikasalla_north/scene.scene.lz"
	.balign 4, 0
	.global Tileset_Mikasalla2Palette
Tileset_Mikasalla2Palette:
	.incbin "games/tla/src/graphics/tileset/mikasalla_north/palette.gbapal.lz"
	.byte 0x74, 0x78, 0x1e	@ PACKER: left in the buffer by Field_MikasallaNorth+0xe9
	.global Tileset_Mikasalla2TilesA
Tileset_Mikasalla2TilesA:
	.incbin "games/tla/src/graphics/tileset/mikasalla_north/tiles_a.4bpp.mtf"
	.global Tileset_Mikasalla2TilesB
Tileset_Mikasalla2TilesB:
	.incbin "games/tla/src/graphics/tileset/mikasalla_north/tiles_b.4bpp.mtf"
	.byte 0x13, 0xf2	@ PACKER: left in the buffer by Tileset_Mikasalla2TilesA+0xaea
	.global Tileset_Mikasalla2TilesC
Tileset_Mikasalla2TilesC:
	.incbin "games/tla/src/graphics/tileset/mikasalla_north/tiles_c.4bpp.mtf"
	.byte 0xe4, 0x78, 0x1a	@ PACKER: left in the buffer by Tileset_Mikasalla2TilesB+0x675
	.global Tileset_Mikasalla2TilesD
Tileset_Mikasalla2TilesD:
	.incbin "games/tla/src/graphics/tileset/mikasalla_north/tiles_d.4bpp.mtf"
