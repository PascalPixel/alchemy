	.global Field_JupiterLighthouseRoom1
Field_JupiterLighthouseRoom1:
	map_header 0, 0, 128, 128, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_JupiterLighthouseRoom1, .LField_JupiterLighthouseRoom1_CELL_PICTURES
	map_block Field_JupiterLighthouseRoom1, .LField_JupiterLighthouseRoom1_SHAPES
	map_block Field_JupiterLighthouseRoom1, .LField_JupiterLighthouseRoom1_GRID
	map_block Field_JupiterLighthouseRoom1, .LField_JupiterLighthouseRoom1_SHAPE_GRID
	map_block Field_JupiterLighthouseRoom1, .LField_JupiterLighthouseRoom1_ANIMATION
	map_block Field_JupiterLighthouseRoom1
	map_block Field_JupiterLighthouseRoom1, .LField_JupiterLighthouseRoom1_SCENE
.LField_JupiterLighthouseRoom1_CELL_PICTURES:
	.incbin "games/tla/src/map/jupiter_lighthouse_room_1/cell_pictures.delta0.lz"
	.balign 4, 0
.LField_JupiterLighthouseRoom1_SHAPES:
	.incbin "games/tla/src/map/jupiter_lighthouse_room_1/shapes.table.lz"
	.balign 4, 0
.LField_JupiterLighthouseRoom1_GRID:
	.incbin "games/tla/src/map/jupiter_lighthouse_room_1/map.grid.lz"
	.balign 4, 0
.LField_JupiterLighthouseRoom1_SHAPE_GRID:
	.incbin "games/tla/src/map/jupiter_lighthouse_room_1/shape_grid.plane.lz"
	.balign 4, 0
.LField_JupiterLighthouseRoom1_ANIMATION:
	.incbin "games/tla/src/map/map_012/animation.script.lz"
	.balign 4, 0
.LField_JupiterLighthouseRoom1_SCENE:
	.incbin "games/tla/src/map/jupiter_lighthouse_room_1/scene.scene.lz"
	.global Tileset_Set104Palette
Tileset_Set104Palette:
	.incbin "games/tla/src/graphics/tileset/set_104/palette.gbapal.lz"
	.byte 0x00, 0x1d, 0x0f	@ PACKER: left in the buffer by Field_JupiterLighthouseRoom1+0x179
	.global Tileset_Set104TilesA
Tileset_Set104TilesA:
	.incbin "games/tla/src/graphics/tileset/set_104/tiles_a.4bpp.mtf"
	.byte 0xf1, 0x6c, 0x88	@ PACKER: left in the buffer by Tileset_Set103TilesC+0x1f3d
	.global Tileset_Set104TilesB
Tileset_Set104TilesB:
	.incbin "games/tla/src/graphics/tileset/set_104/tiles_b.4bpp.mtf"
	.byte 0x80, 0x62, 0xc6	@ PACKER: left in the buffer by Tileset_Set104TilesA+0x1471
	.global Tileset_Set104TilesC
Tileset_Set104TilesC:
	.incbin "games/tla/src/graphics/tileset/set_104/tiles_c.4bpp.mtf"
	.byte 0xac	@ PACKER: left in the buffer by Tileset_Set104TilesA+0x1cb7
	.global Tileset_Set104TilesD
Tileset_Set104TilesD:
	.incbin "games/tla/src/graphics/tileset/set_104/tiles_d.4bpp.mtf"
