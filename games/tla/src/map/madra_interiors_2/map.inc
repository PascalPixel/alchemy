	.global Field_MadraInteriors2
Field_MadraInteriors2:
	map_header 0, 0, 120, 120, 3, 1, 2, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 120, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 120, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_MadraInteriors2, .LField_MadraInteriors2_CELL_PICTURES
	map_block Field_MadraInteriors2, .LField_MadraInteriors2_SHAPES
	map_block Field_MadraInteriors2, .LField_MadraInteriors2_GRID
	map_block Field_MadraInteriors2, .LField_MadraInteriors2_SHAPE_GRID
	map_block Field_MadraInteriors2, .LField_MadraInteriors2_ANIMATION
	map_block Field_MadraInteriors2, .LField_MadraInteriors2_BLEND
	map_block Field_MadraInteriors2, .LField_MadraInteriors2_SCENE
.LField_MadraInteriors2_CELL_PICTURES:
	.incbin "games/tla/src/map/madra_interiors_2/cell_pictures.delta2.lz"
	.balign 4, 0
.LField_MadraInteriors2_SHAPES:
	.incbin "games/tla/src/map/madra_interiors_2/shapes.table.lz"
	.balign 4, 0
.LField_MadraInteriors2_GRID:
	.incbin "games/tla/src/map/madra_interiors_2/map.grid.lz"
	.balign 4, 0
.LField_MadraInteriors2_SHAPE_GRID:
	.incbin "games/tla/src/map/madra_interiors_2/shape_grid.plane.lz"
	.balign 4, 0
.LField_MadraInteriors2_ANIMATION:
	.incbin "games/tla/src/map/madra_interiors_2/animation.script.lz"
	.balign 4, 0
.LField_MadraInteriors2_BLEND:
	.incbin "games/tla/src/map/daila_interiors_houses/blend.script.lz"
	.balign 4, 0
.LField_MadraInteriors2_SCENE:
	.incbin "games/tla/src/map/madra_interiors_2/scene.scene.lz"
	.balign 4, 0
	.global Tileset_MadraInteriors2Palette
Tileset_MadraInteriors2Palette:
	.incbin "games/tla/src/graphics/tileset/madra_interiors_2/palette.gbapal.lz"
	.byte 0xc7	@ PACKER: left in the buffer by Field_MadraInteriors2+0x177
	.global Tileset_MadraInteriors2TilesA
Tileset_MadraInteriors2TilesA:
	.incbin "games/tla/src/graphics/tileset/madra_interiors_2/tiles_a.4bpp.mtf"
	.byte 0x08, 0x0a	@ PACKER: left in the buffer by Field_MadraInteriors2+0x14be
	.global Tileset_MadraInteriors2TilesB
Tileset_MadraInteriors2TilesB:
	.incbin "games/tla/src/graphics/tileset/madra_interiors_2/tiles_b.4bpp.mtf"
	.byte 0x3c, 0x00	@ PACKER: left in the buffer by Field_MadraInteriors2+0x1802
	.global Tileset_MadraInteriors2TilesC
Tileset_MadraInteriors2TilesC:
	.incbin "games/tla/src/graphics/tileset/madra_interiors_2/tiles_c.4bpp.mtf"
	.ifdef TLA_EDITION_JA
	.byte 0x1d, 0x1e	@ PACKER: left in the buffer by Field_MadraInteriors2+0x1976
	.else
	.byte 0x01, 0x00	@ PACKER: unused post-stream residue; Resource_DecodeType2 uses decoded RAM and output length; writer unknown
	.endif
	.global Tileset_MadraInteriors2TilesD
Tileset_MadraInteriors2TilesD:
	.incbin "games/tla/src/graphics/tileset/madra_interiors_2/tiles_d.4bpp.mtf"
	.byte 0x22, 0xb9	@ PACKER: left in the buffer by Tileset_MadraInteriors2TilesC+0xf1e
