	.include "games/tbs/src/layout/layout.inc"
	.section .rodata
	.global Field_Layout105
Field_Layout105:
	layout_header 0, 0, 120, 120, 3, 1, 2, 0, 2, 2, 0, 0
	layout_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	layout_layer 120, 0, 16, 16, 0, 0, 0xff, 0xff
	layout_layer 0, 120, 16, 16, 0, 0, 0xff, 0xff
	layout_offset Field_Layout105, .LField_Layout105_METATILES
	layout_offset Field_Layout105, .LField_Layout105_SHAPES
	layout_offset Field_Layout105, .LField_Layout105_GRID
	layout_offset Field_Layout105, .LField_Layout105_ANIMATION
	layout_offset Field_Layout105, .LField_Layout105_BLEND
	layout_offset Field_Layout105, .LField_Layout105_MARKERS
.LField_Layout105_METATILES:
	.incbin "games/tbs/src/layout/layout_105/metatiles.delta2.lz"
	.balign 4, 0
.LField_Layout105_SHAPES:
	.incbin "games/tbs/src/layout/layout_105/shapes.table.lz"
	.balign 4, 0
.LField_Layout105_GRID:
	.incbin "games/tbs/src/layout/layout_105/grid.parts.lz"
	.balign 4, 0
.LField_Layout105_ANIMATION:
	.incbin "games/tbs/src/layout/layout_105/tileset_anims.script.lz"
	.balign 4, 0
.LField_Layout105_BLEND:
	.incbin "games/tbs/src/layout/layout_105/blend.script.lz"
	.balign 4, 0
.LField_Layout105_MARKERS:
	.incbin "games/tbs/src/layout/layout_105/treasure_spots.table"
	.global Tileset_Set57Palette
Tileset_Set57Palette:
	.incbin "games/tbs/src/graphics/tileset/set_57/palette.gbapal.lz"
	.global Tileset_Set57TilesA
Tileset_Set57TilesA:
	.incbin "games/tbs/src/graphics/tileset/set_57/tiles_a.4bpp.mtf"
	.byte 0xc0, 0xbe, 0xc0	@ PACKER: left in the buffer by Tileset_Set56TilesC+0x13f1
	.global Tileset_Set57TilesB
Tileset_Set57TilesB:
	.incbin "games/tbs/src/graphics/tileset/set_57/tiles_b.4bpp.mtf"
	.byte 0x21, 0xcf, 0x49	@ PACKER: left in the buffer by Tileset_Set56TilesC+0x1795
	.global Tileset_Set57TilesC
Tileset_Set57TilesC:
	.incbin "games/tbs/src/graphics/tileset/set_57/tiles_c.4bpp.mtf"
	.byte 0xd6, 0x83, 0x39	@ PACKER: left in the buffer by Tileset_Set57TilesB+0x1271
	.global Tileset_Set57TilesD
Tileset_Set57TilesD:
	.incbin "games/tbs/src/graphics/tileset/set_57/tiles_d.4bpp.mtf"
