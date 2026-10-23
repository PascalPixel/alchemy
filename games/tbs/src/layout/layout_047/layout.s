	.include "games/tbs/src/layout/layout.inc"
	.section .rodata
	.global Field_Layout047
Field_Layout047:
	layout_header 0, 0, 128, 64, 3, 2, 1, 0, 2, 2, 0, 0
	layout_layer 0, 0, 8, 8, 0, 0, 0xff, 0xff
	layout_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	layout_layer 0, 64, 16, 16, 0, 0, 0xff, 0xff
	layout_offset Field_Layout047, .LField_Layout047_METATILES
	layout_offset Field_Layout047, .LField_Layout047_SHAPES
	layout_offset Field_Layout047, .LField_Layout047_GRID
	layout_offset Field_Layout047, .LField_Layout047_ANIMATION
	layout_offset Field_Layout047, .LField_Layout047_BLEND
	layout_offset Field_Layout047, .LField_Layout047_MARKERS
.LField_Layout047_METATILES:
	.incbin "games/tbs/src/layout/layout_047/metatiles.delta1.lz"
	.balign 4, 0
.LField_Layout047_SHAPES:
	.incbin "games/tbs/src/layout/layout_047/shapes.table.lz"
	.balign 4, 0
.LField_Layout047_GRID:
	.incbin "games/tbs/src/layout/layout_047/grid.parts.lz"
	.balign 4, 0
.LField_Layout047_ANIMATION:
	.incbin "games/tbs/src/layout/layout_037/tileset_anims.script.lz"
	.balign 4, 0
.LField_Layout047_BLEND:
	.incbin "games/tbs/src/layout/layout_047/blend.script.lz"
	.balign 4, 0
.LField_Layout047_MARKERS:
	.incbin "games/tbs/src/layout/layout_000/treasure_spots.table"
	.balign 4, 0
	.global Tileset_Set37Palette
Tileset_Set37Palette:
	.incbin "games/tbs/src/graphics/tileset/set_37/palette.gbapal.lz"
	.byte 0x9b, 0x46	@ PACKER: left in the buffer by Field_Layout047+0x16a
	.global Tileset_Set37TilesA
Tileset_Set37TilesA:
	.incbin "games/tbs/src/graphics/tileset/set_37/tiles_a.4bpp.mtf"
	.byte 0xe0, 0x91, 0x0f	@ PACKER: left in the buffer by Tileset_Set36TilesB+0x1d59
	.global Tileset_Set37TilesB
Tileset_Set37TilesB:
	.incbin "games/tbs/src/graphics/tileset/set_37/tiles_b.4bpp.mtf"
	.byte 0x2f	@ PACKER: left in the buffer by Tileset_Set37TilesA+0x13f
	.global Tileset_Set37TilesC
Tileset_Set37TilesC:
	.incbin "games/tbs/src/graphics/tileset/set_37/tiles_c.4bpp.mtf"
	.byte 0x23, 0xe7	@ PACKER: left in the buffer by Tileset_Set36TilesB+0x27fe
	.global Tileset_Set37TilesD
Tileset_Set37TilesD:
	.incbin "games/tbs/src/graphics/tileset/set_37/tiles_d.4bpp.mtf"
	.byte 0x09	@ PACKER: left in the buffer by Tileset_Set37TilesC+0x13f
