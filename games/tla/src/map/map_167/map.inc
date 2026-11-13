	.global Field_Map167
Field_Map167:
	map_header 0, 0, 130, 112, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, 2, 2, 0xf, 0xf
	map_block Field_Map167, .LField_Map167_CELL_PICTURES
	map_block Field_Map167, .LField_Map167_SHAPES
	map_block Field_Map167, .LField_Map167_GRID
	map_block Field_Map167, .LField_Map167_SHAPE_GRID
	map_block Field_Map167, .LField_Map167_ANIMATION
	map_block Field_Map167, .LField_Map167_BLEND
	map_block Field_Map167, .LField_Map167_SCENE
.LField_Map167_CELL_PICTURES:
	.incbin "games/tla/src/map/map_167/cell_pictures.delta2.lz"
	.balign 4, 0
.LField_Map167_SHAPES:
	.incbin "games/tla/src/map/map_167/shapes.table.lz"
	.balign 4, 0
.LField_Map167_GRID:
	.incbin "games/tla/src/map/map_167/map.grid.lz"
	.balign 4, 0
.LField_Map167_SHAPE_GRID:
	.incbin "games/tla/src/map/map_167/shape_grid.plane.lz"
	.balign 4, 0
.LField_Map167_ANIMATION:
	.incbin "games/tla/src/map/map_167/animation.script.lz"
	.balign 4, 0
.LField_Map167_BLEND:
	.incbin "games/tla/src/map/map_167/blend.script.lz"
	.balign 4, 0
.LField_Map167_SCENE:
	.incbin "games/tla/src/map/map_167/scene.scene.lz"
	.balign 4, 0
	.global Tileset_Set77Palette
Tileset_Set77Palette:
	.incbin "games/tla/src/graphics/tileset/set_77/palette.gbapal.lz"
	.byte 0x18	@ PACKER: left in the buffer by Field_Map167+0x133
	.global Tileset_Set77TilesA
Tileset_Set77TilesA:
	.incbin "games/tla/src/graphics/tileset/set_77/tiles_a.4bpp.mtf"
	.byte 0x20	@ PACKER: left in the buffer by Field_Map167+0x1c6b
	.global Tileset_Set77TilesB
Tileset_Set77TilesB:
	.incbin "games/tla/src/graphics/tileset/set_77/tiles_b.4bpp.mtf"
	.byte 0x28	@ PACKER: left in the buffer by Tileset_Set77TilesA+0x11b7
	.global Tileset_Set77TilesC
Tileset_Set77TilesC:
	.incbin "games/tla/src/graphics/tileset/set_77/tiles_c.4bpp.mtf"
	.global Tileset_Set77TilesD
Tileset_Set77TilesD:
	.incbin "games/tla/src/graphics/tileset/set_77/tiles_d.4bpp.mtf"
	.byte 0x23, 0x70, 0x22	@ PACKER: left in the buffer by Tileset_Set77TilesC+0x639
