	.global Field_IsletsDog
Field_IsletsDog:
	map_header 0, 0, 50, 128, 3, 1, 1, 1, 1, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, -1, -3, 0x3f, 0x3f
	map_block Field_IsletsDog, .LField_IsletsDog_CELL_TILES
	map_block Field_IsletsDog, .LField_IsletsDog_SHAPES
	map_block Field_IsletsDog, .LField_IsletsDog_GRID
	map_block Field_IsletsDog, .LField_IsletsDog_SHAPE_GRID
	map_block Field_IsletsDog, .LField_IsletsDog_ANIMATION
	map_block Field_IsletsDog, .LField_IsletsDog_BLEND
	map_block Field_IsletsDog, .LField_IsletsDog_SCENE
.LField_IsletsDog_CELL_TILES:
	.incbin "games/tla/src/map/islets_dog/cell_tiles.delta2.lz"
	.balign 4, 0
.LField_IsletsDog_SHAPES:
	.incbin "games/tla/src/map/islets_dog/shapes.table.lz"
	.balign 4, 0
.LField_IsletsDog_GRID:
	.incbin "games/tla/src/map/islets_dog/map.grid.lz"
	.balign 4, 0
.LField_IsletsDog_SHAPE_GRID:
	.incbin "games/tla/src/map/islets_dog/shape_grid.plane.lz"
	.balign 4, 0
.LField_IsletsDog_ANIMATION:
	.incbin "games/tla/src/map/islets_dog/animation.script.lz"
	.balign 4, 0
.LField_IsletsDog_BLEND:
	.incbin "games/tla/src/map/islets_dog/blend.script.lz"
	.balign 4, 0
.LField_IsletsDog_SCENE:
	.incbin "games/tla/src/map/islets_dog/scene.scene.lz"
	.global Tileset_IsletsDogPalette
Tileset_IsletsDogPalette:
	.incbin "games/tla/src/graphics/tileset/islets_dog/palette.gbapal.lz"
	.byte 0x11, 0x98, 0x70	@ PACKER: left in the buffer by Field_Islets1+0x171
	.global Tileset_IsletsDogTilesA
Tileset_IsletsDogTilesA:
	.incbin "games/tla/src/graphics/tileset/islets_dog/tiles_a.4bpp.mtf"
	.byte 0x4d, 0x25	@ PACKER: left in the buffer by Tileset_GabombaCatacombsTeleportersTilesA+0x2a36
	.global Tileset_IsletsDogTilesB
Tileset_IsletsDogTilesB:
	.incbin "games/tla/src/graphics/tileset/islets_dog/tiles_b.4bpp.mtf"
	.byte 0x02	@ PACKER: left in the buffer by Tileset_IsletsDogTilesA+0x28cf
	.global Tileset_IsletsDogTilesC
Tileset_IsletsDogTilesC:
	.incbin "games/tla/src/graphics/tileset/islets_dog/tiles_c.4bpp.mtf"
	.global Tileset_IsletsDogTilesD
Tileset_IsletsDogTilesD:
	.incbin "games/tla/src/graphics/tileset/islets_dog/tiles_d.4bpp.mtf"
	.byte 0xf3	@ PACKER: left in the buffer by Tileset_IsletsDogTilesB+0x27d3
