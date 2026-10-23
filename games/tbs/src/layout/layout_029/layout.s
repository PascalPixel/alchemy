	.include "games/tbs/src/layout/layout.inc"
	.section .rodata
	.global Field_Layout029
Field_Layout029:
	layout_header 0, 0, 128, 128, 3, 2, 1, 0, 2, 2, 0, 0
	layout_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	layout_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	layout_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	layout_offset Field_Layout029, .LField_Layout029_METATILES
	layout_offset Field_Layout029, .LField_Layout029_SHAPES
	layout_offset Field_Layout029, .LField_Layout029_GRID
	layout_offset Field_Layout029, .LField_Layout029_ANIMATION
	layout_offset Field_Layout029, .LField_Layout029_BLEND
	layout_offset Field_Layout029, .LField_Layout029_MARKERS
.LField_Layout029_METATILES:
	.incbin "games/tbs/src/layout/layout_029/metatiles.delta2.lz"
	.balign 4, 0
.LField_Layout029_SHAPES:
	.incbin "games/tbs/src/layout/layout_029/shapes.table.lz"
	.balign 4, 0
.LField_Layout029_GRID:
	.incbin "games/tbs/src/layout/layout_029/grid.parts.lz"
	.balign 4, 0
.LField_Layout029_ANIMATION:
	.incbin "games/tbs/src/layout/layout_029/tileset_anims.script.lz"
	.balign 4, 0
.LField_Layout029_BLEND:
	.incbin "games/tbs/src/layout/layout_029/blend.script.lz"
	.balign 4, 0
.LField_Layout029_MARKERS:
	.incbin "games/tbs/src/layout/layout_029/treasure_spots.table"
	.balign 4, 0
	.global Tileset_Set22Palette
Tileset_Set22Palette:
	.incbin "games/tbs/src/graphics/tileset/set_22/palette.gbapal.lz"
	.global Tileset_Set22TilesA
Tileset_Set22TilesA:
	.incbin "games/tbs/src/graphics/tileset/set_22/tiles_a.4bpp.mtf"
	.byte 0x04, 0x1c, 0xf2	@ PACKER: left in the buffer by Tileset_Set21TilesA+0x2b3d
	.global Tileset_Set22TilesB
Tileset_Set22TilesB:
	.incbin "games/tbs/src/graphics/tileset/set_22/tiles_b.4bpp.mtf"
	.byte 0xf9	@ PACKER: left in the buffer by Tileset_Set22TilesA+0x1dcf
	.global Tileset_Set22TilesC
Tileset_Set22TilesC:
	.incbin "games/tbs/src/graphics/tileset/set_22/tiles_c.4bpp.mtf"
	.byte 0xf0	@ PACKER: left in the buffer by Tileset_Set22TilesB+0x13f
