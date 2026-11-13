	.include "games/tla/src/map/map.inc"
	.section .rodata
	.global Field_Map180
Field_Map180:
	map_header 0, 0, 128, 128, 3, 3, 3, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 4, -4, 0x1f, 0x1f
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map180, .LField_Map180_CELL_PICTURES
	map_block Field_Map180, .LField_Map180_SHAPES
	map_block Field_Map180, .LField_Map180_GRID
	map_block Field_Map180, .LField_Map180_SHAPE_GRID
	map_block Field_Map180, .LField_Map180_ANIMATION
	map_block Field_Map180, .LField_Map180_BLEND
	map_block Field_Map180, .LField_Map180_SCENE
.LField_Map180_CELL_PICTURES:
	.incbin "games/tla/src/map/map_180/cell_pictures.delta2.lz"
	.balign 4, 0
.LField_Map180_SHAPES:
	.incbin "games/tla/src/map/map_180/shapes.table.lz"
	.balign 4, 0
.LField_Map180_GRID:
	.incbin "games/tla/src/map/map_180/map.grid.lz"
	.balign 4, 0
.LField_Map180_SHAPE_GRID:
	.incbin "games/tla/src/map/map_180/shape_grid.plane.lz"
	.balign 4, 0
.LField_Map180_ANIMATION:
	.incbin "games/tla/src/map/map_179/animation.script.lz"
	.balign 4, 0
.LField_Map180_BLEND:
	.incbin "games/tla/src/map/map_179/blend.script.lz"
	.balign 4, 0
.LField_Map180_SCENE:
	.incbin "games/tla/src/map/map_180/scene.scene.lz"
	.balign 4, 0
	.global Tileset_Set81Palette
Tileset_Set81Palette:
	.incbin "games/tla/src/graphics/tileset/set_81/palette.gbapal.lz"
	.byte 0x48, 0x1d, 0x3f	@ PACKER: left in the buffer by Field_Map180+0x149
	.global Tileset_Set81TilesA
Tileset_Set81TilesA:
	.incbin "games/tla/src/graphics/tileset/set_81/tiles_a.4bpp.mtf"
	.ifdef TLA_EDITION_JA
	.byte 0x30	@ PACKER: left in the buffer by Tileset_Set79TilesB+0x20bb
	.else
	.byte 0x40	@ PACKER: unused post-stream residue; Resource_DecodeType2 uses decoded RAM and output length; writer unknown
	.endif
	.global Tileset_Set81TilesB
Tileset_Set81TilesB:
	.incbin "games/tla/src/graphics/tileset/set_81/tiles_b.4bpp.mtf"
	.global Tileset_Set81TilesC
Tileset_Set81TilesC:
	.incbin "games/tla/src/graphics/tileset/set_81/tiles_c.4bpp.mtf"
	.byte 0xd7, 0x23	@ PACKER: left in the buffer by Tileset_Set81TilesB+0x15c2
	.global Tileset_Set81TilesD
Tileset_Set81TilesD:
	.incbin "games/tla/src/graphics/tileset/set_81/tiles_d.4bpp.mtf"
