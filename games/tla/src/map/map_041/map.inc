	.global Field_Map041
Field_Map041:
	map_header 0, 0, 120, 120, 3, 1, 2, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 120, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 120, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map041, .LField_Map041_CELL_PICTURES
	map_block Field_Map041, .LField_Map041_SHAPES
	map_block Field_Map041, .LField_Map041_GRID
	map_block Field_Map041, .LField_Map041_SHAPE_GRID
	map_block Field_Map041, .LField_Map041_ANIMATION
	map_block Field_Map041, .LField_Map041_BLEND
	map_block Field_Map041, .LField_Map041_SCENE
.LField_Map041_CELL_PICTURES:
	.incbin "games/tla/src/map/map_041/cell_pictures.delta2.lz"
	.balign 4, 0
.LField_Map041_SHAPES:
	.incbin "games/tla/src/map/map_041/shapes.table.lz"
	.balign 4, 0
.LField_Map041_GRID:
	.incbin "games/tla/src/map/map_041/map.grid.lz"
	.balign 4, 0
.LField_Map041_SHAPE_GRID:
	.incbin "games/tla/src/map/map_041/shape_grid.plane.lz"
	.balign 4, 0
.LField_Map041_ANIMATION:
	.incbin "games/tla/src/map/map_041/animation.script.lz"
	.balign 4, 0
.LField_Map041_BLEND:
	.incbin "games/tla/src/map/map_010/blend.script.lz"
	.balign 4, 0
.LField_Map041_SCENE:
	.incbin "games/tla/src/map/map_041/scene.scene.lz"
	.balign 4, 0
	.global Tileset_Set24Palette
Tileset_Set24Palette:
	.incbin "games/tla/src/graphics/tileset/set_24/palette.gbapal.lz"
	.byte 0xc7	@ PACKER: left in the buffer by Field_Map041+0x177
	.global Tileset_Set24TilesA
Tileset_Set24TilesA:
	.incbin "games/tla/src/graphics/tileset/set_24/tiles_a.4bpp.mtf"
	.byte 0x08, 0x0a	@ PACKER: left in the buffer by Field_Map041+0x14be
	.global Tileset_Set24TilesB
Tileset_Set24TilesB:
	.incbin "games/tla/src/graphics/tileset/set_24/tiles_b.4bpp.mtf"
	.byte 0x3c, 0x00	@ PACKER: left in the buffer by Field_Map041+0x1802
	.global Tileset_Set24TilesC
Tileset_Set24TilesC:
	.incbin "games/tla/src/graphics/tileset/set_24/tiles_c.4bpp.mtf"
	.ifdef TLA_EDITION_JA
	.byte 0x1d, 0x1e	@ PACKER: left in the buffer by Field_Map041+0x1976
	.else
	.byte 0x01, 0x00	@ PACKER: unused post-stream residue; Resource_DecodeType2 uses decoded RAM and output length; writer unknown
	.endif
	.global Tileset_Set24TilesD
Tileset_Set24TilesD:
	.incbin "games/tla/src/graphics/tileset/set_24/tiles_d.4bpp.mtf"
	.byte 0x22, 0xb9	@ PACKER: left in the buffer by Tileset_Set24TilesC+0xf1e
