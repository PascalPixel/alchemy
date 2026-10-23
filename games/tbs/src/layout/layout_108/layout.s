	.include "games/tbs/src/layout/layout.inc"
	.section .rodata
	.global Field_Layout108
Field_Layout108:
	layout_header 0, 0, 128, 128, 3, 2, 1, 0, 2, 2, 0, 0
	layout_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	layout_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	layout_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	layout_offset Field_Layout108, .LField_Layout108_METATILES
	layout_offset Field_Layout108, .LField_Layout108_SHAPES
	layout_offset Field_Layout108, .LField_Layout108_GRID
	layout_offset Field_Layout108, .LField_Layout108_ANIMATION
	layout_offset Field_Layout108, .LField_Layout108_BLEND
	layout_offset Field_Layout108, .LField_Layout108_MARKERS
.LField_Layout108_METATILES:
	.incbin "games/tbs/src/layout/layout_108/metatiles.delta2.lz"
	.balign 4, 0
.LField_Layout108_SHAPES:
	.incbin "games/tbs/src/layout/layout_108/shapes.table.lz"
	.balign 4, 0
.LField_Layout108_GRID:
	.incbin "games/tbs/src/layout/layout_108/map.grid.lz"
	.balign 4, 0
.LField_Layout108_ANIMATION:
	.incbin "games/tbs/src/layout/layout_037/tileset_anims.script.lz"
	.balign 4, 0
.LField_Layout108_BLEND:
	.incbin "games/tbs/src/layout/layout_108/blend.script.lz"
	.balign 4, 0
.LField_Layout108_MARKERS:
	.incbin "games/tbs/src/layout/layout_108/treasure_spots.table"
	.balign 4, 0
	.global Tileset_Set58Palette
Tileset_Set58Palette:
	.incbin "games/tbs/src/graphics/tileset/set_58/palette.gbapal.lz"
	.byte 0xbe, 0xee	@ PACKER: left in the buffer by Field_Layout108+0x186
	.global Tileset_Set58TilesA
Tileset_Set58TilesA:
	.incbin "games/tbs/src/graphics/tileset/set_58/tiles_a.4bpp.mtf"
	.global Tileset_Set58TilesB
Tileset_Set58TilesB:
	.incbin "games/tbs/src/graphics/tileset/set_58/tiles_b.4bpp.mtf"
	.byte 0x08, 0xe8	@ PACKER: left in the buffer by Tileset_Set54TilesA+0x2672
	.global Tileset_Set58TilesC
Tileset_Set58TilesC:
	.incbin "games/tbs/src/graphics/tileset/set_58/tiles_c.4bpp.mtf"
	.byte 0xfe	@ PACKER: left in the buffer by Tileset_Set58TilesB+0x153
	.global Tileset_Set58TilesD
Tileset_Set58TilesD:
	.incbin "games/tbs/src/graphics/tileset/set_58/tiles_d.4bpp.mtf"
	.byte 0xff	@ PACKER: left in the buffer by Tileset_Set58TilesC+0x13f
