	.include "games/tbs/src/layout/layout.inc"
	.section .rodata
	.global Field_Layout157
Field_Layout157:
	layout_header 0, 0, 128, 128, 3, 2, 1, 0, 2, 2, 0, 0
	layout_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	layout_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	layout_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	layout_offset Field_Layout157, .LField_Layout157_METATILES
	layout_offset Field_Layout157, .LField_Layout157_SHAPES
	layout_offset Field_Layout157, .LField_Layout157_GRID
	layout_offset Field_Layout157, .LField_Layout157_ANIMATION
	layout_offset Field_Layout157, .LField_Layout157_BLEND
	layout_offset Field_Layout157, .LField_Layout157_MARKERS
.LField_Layout157_METATILES:
	.incbin "games/tbs/src/layout/layout_157/metatiles.delta1.lz"
	.balign 4, 0
.LField_Layout157_SHAPES:
	.incbin "games/tbs/src/layout/layout_157/shapes.table.lz"
	.balign 4, 0
.LField_Layout157_GRID:
	.incbin "games/tbs/src/layout/layout_157/map.grid.lz"
	.balign 4, 0
.LField_Layout157_ANIMATION:
	.incbin "games/tbs/src/layout/layout_037/tileset_anims.script.lz"
	.balign 4, 0
.LField_Layout157_BLEND:
	.incbin "games/tbs/src/layout/layout_020/blend.script.lz"
	.balign 4, 0
.LField_Layout157_MARKERS:
	treasure_spot 38, 9, 102
	treasure_spot 50, 11, 101
	treasure_spot 40, 16, 103
	treasure_spot 43, 30, 100
	treasure_spots_end
	.balign 4, 0
	.global Tileset_Palette44
Tileset_Palette44:
	.incbin "games/tbs/src/graphics/tileset/palettes/pal_44.gbapal.lz"
	.byte 0x32	@ PACKER: left in the buffer by Field_Layout157+0x173
	.global Tileset_Palette45
Tileset_Palette45:
	.incbin "games/tbs/src/graphics/tileset/palettes/pal_45.gbapal.lz"
