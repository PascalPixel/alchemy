	.global Field_Map168
Field_Map168:
	map_header 0, 0, 130, 112, 3, 1, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map168, .LField_Map168_CELL_PICTURES
	map_block Field_Map168, .LField_Map168_SHAPES
	map_block Field_Map168, .LField_Map168_GRID
	map_block Field_Map168, .LField_Map168_SHAPE_GRID
	map_block Field_Map168, .LField_Map168_ANIMATION
	map_block Field_Map168, .LField_Map168_BLEND
	map_block Field_Map168, .LField_Map168_SCENE
.LField_Map168_CELL_PICTURES:
	.incbin "games/tla/src/map/map_168/cell_pictures.delta2.lz"
	.balign 4, 0
.LField_Map168_SHAPES:
	.incbin "games/tla/src/map/map_168/shapes.table.lz"
	.balign 4, 0
.LField_Map168_GRID:
	.incbin "games/tla/src/map/map_168/map.grid.lz"
	.balign 4, 0
.LField_Map168_SHAPE_GRID:
	.incbin "games/tla/src/map/map_168/shape_grid.plane.lz"
	.balign 4, 0
.LField_Map168_ANIMATION:
	.incbin "games/tla/src/map/map_168/animation.script.lz"
	.balign 4, 0
.LField_Map168_BLEND:
	.incbin "games/tla/src/map/map_168/blend.script.lz"
	.balign 4, 0
.LField_Map168_SCENE:
	.incbin "games/tla/src/map/map_168/scene.scene.lz"
	.global Tileset_Set78Palette
Tileset_Set78Palette:
	.incbin "games/tla/src/graphics/tileset/set_78/palette.gbapal.lz"
	.byte 0x5c, 0x13	@ PACKER: left in the buffer by Field_Map168+0x11e
	.global Tileset_Set78TilesA
Tileset_Set78TilesA:
	.incbin "games/tla/src/graphics/tileset/set_78/tiles_a.4bpp.mtf"
	.byte 0x1f	@ PACKER: left in the buffer by Tileset_Set77TilesA+0x1af7
	.global Tileset_Set78TilesB
Tileset_Set78TilesB:
	.incbin "games/tla/src/graphics/tileset/set_78/tiles_b.4bpp.mtf"
	.byte 0xa2	@ PACKER: left in the buffer by Tileset_Set78TilesA+0x140b
	.global Tileset_Set78TilesC
Tileset_Set78TilesC:
	.incbin "games/tla/src/graphics/tileset/set_78/tiles_c.4bpp.mtf"
	.byte 0x02	@ PACKER: left in the buffer by Tileset_Set78TilesB+0x553
	.global Tileset_Set78TilesD
Tileset_Set78TilesD:
	.incbin "games/tla/src/graphics/tileset/set_78/tiles_d.4bpp.mtf"
	.byte 0xb9, 0x40, 0x00	@ PACKER: left in the buffer by Tileset_Set78TilesB+0x639
