	.global Field_Map102
Field_Map102:
	map_header 0, 0, 64, 66, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map102, .LField_Map102_CELL_PICTURES
	map_block Field_Map102, .LField_Map102_SHAPES
	map_block Field_Map102, .LField_Map102_GRID
	map_block Field_Map102, .LField_Map102_SHAPE_GRID
	map_block Field_Map102, .LField_Map102_ANIMATION
	map_block Field_Map102, .LField_Map102_BLEND
	map_block Field_Map102, .LField_Map102_SCENE
.LField_Map102_CELL_PICTURES:
	.incbin "games/tla/src/map/map_102/cell_pictures.delta0.lz"
	.balign 4, 0
.LField_Map102_SHAPES:
	.incbin "games/tla/src/map/map_102/shapes.table.lz"
	.balign 4, 0
.LField_Map102_GRID:
	.incbin "games/tla/src/map/map_102/map.grid.lz"
	.balign 4, 0
.LField_Map102_SHAPE_GRID:
	.incbin "games/tla/src/map/map_102/shape_grid.plane.lz"
	.balign 4, 0
.LField_Map102_ANIMATION:
	.incbin "games/tla/src/map/map_102/animation.script.lz"
	.balign 4, 0
.LField_Map102_BLEND:
	.incbin "games/tla/src/map/lobby/blend.script.lz"
	.balign 4, 0
.LField_Map102_SCENE:
	.incbin "games/tla/src/map/map_102/scene.scene.lz"
	.balign 4, 0
	.global Tileset_Set51Palette
Tileset_Set51Palette:
	.incbin "games/tla/src/graphics/tileset/set_51/palette.gbapal.lz"
	.byte 0x5f, 0x52	@ PACKER: left in the buffer by Field_Map102+0x156
	.global Tileset_Set51TilesA
Tileset_Set51TilesA:
	.incbin "games/tla/src/graphics/tileset/set_51/tiles_a.4bpp.mtf"
	.ifdef TLA_EDITION_JA
	.byte 0xd4, 0x0e, 0x40	@ PACKER: unused post-stream residue; Resource_DecodeType2 uses decoded RAM and output length; writer unknown
	.else
	.byte 0xef, 0x12, 0x40	@ PACKER: unused post-stream residue; Resource_DecodeType2 uses decoded RAM and output length; writer unknown
	.endif
	.global Tileset_Set51TilesB
Tileset_Set51TilesB:
	.incbin "games/tla/src/graphics/tileset/set_51/tiles_b.4bpp.mtf"
	.byte 0x8e, 0x16, 0x18	@ PACKER: left in the buffer by Tileset_Set51TilesA+0x2221
	.global Tileset_Set51TilesC
Tileset_Set51TilesC:
	.incbin "games/tla/src/graphics/tileset/set_51/tiles_c.4bpp.mtf"
	.global Tileset_Set51TilesD
Tileset_Set51TilesD:
	.incbin "games/tla/src/graphics/tileset/set_51/tiles_d.4bpp.mtf"
	.byte 0xe0, 0x75	@ PACKER: left in the buffer by Tileset_Set51TilesC+0x30a
