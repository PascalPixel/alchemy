	.global Field_Map004
Field_Map004:
	map_header 0, 0, 128, 128, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map004, .LField_Map004_CELL_PICTURES
	map_block Field_Map004, .LField_Map004_SHAPES
	map_block Field_Map004, .LField_Map004_GRID
	map_block Field_Map004, .LField_Map004_SHAPE_GRID
	map_block Field_Map004, .LField_Map004_ANIMATION
	map_block Field_Map004, .LField_Map004_BLEND
	map_block Field_Map004, .LField_Map004_SCENE
.LField_Map004_CELL_PICTURES:
	.incbin "games/tla/src/map/map_004/cell_pictures.delta0.lz"
	.balign 4, 0
.LField_Map004_SHAPES:
	.incbin "games/tla/src/map/map_004/shapes.table.lz"
	.balign 4, 0
.LField_Map004_GRID:
	.incbin "games/tla/src/map/map_004/map.grid.lz"
	.balign 4, 0
.LField_Map004_SHAPE_GRID:
	.incbin "games/tla/src/map/map_004/shape_grid.plane.lz"
	.balign 4, 0
.LField_Map004_ANIMATION:
	.incbin "games/tla/src/map/map_004/animation.script.lz"
	.balign 4, 0
.LField_Map004_BLEND:
	.incbin "games/tla/src/map/lobby/blend.script.lz"
	.balign 4, 0
.LField_Map004_SCENE:
	.incbin "games/tla/src/map/map_000/scene.scene.lz"
	.balign 4, 0
	.global Tileset_Set03Palette
Tileset_Set03Palette:
	.incbin "games/tla/src/graphics/tileset/set_03/pal_41.gbapal.lz"
	.byte 0x71, 0xc0, 0x05	@ PACKER: left in the buffer by Field_Map004+0x171
	.global Tileset_Set03TilesA
Tileset_Set03TilesA:
	.incbin "games/tla/src/graphics/tileset/set_03/tiles_a.4bpp.mtf"
	.byte 0x68, 0x3d, 0x1a	@ PACKER: left in the buffer by Field_VenusLighthouseEntrance+0x2de1
	.global Tileset_Set03TilesB
Tileset_Set03TilesB:
	.incbin "games/tla/src/graphics/tileset/set_03/tiles_b.4bpp.mtf"
	.byte 0x73	@ PACKER: left in the buffer by Tileset_Set03TilesA+0x28a7
	.global Tileset_Set03TilesC
Tileset_Set03TilesC:
	.incbin "games/tla/src/graphics/tileset/set_03/tiles_c.4bpp.mtf"
	.global Tileset_Set03TilesD
Tileset_Set03TilesD:
	.incbin "games/tla/src/graphics/tileset/set_03/tiles_d.4bpp.mtf"
	.byte 0x82, 0x65	@ PACKER: left in the buffer by Tileset_Set03TilesC+0x1b3a
