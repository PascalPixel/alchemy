	.include "games/tbs/src/layout/layout.inc"
	.section .rodata
	.global Field_Layout158
Field_Layout158:
	layout_header 0, 0, 128, 128, 3, 2, 1, 0, 2, 2, 0, 0
	layout_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	layout_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	layout_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	layout_offset Field_Layout158, .LField_Layout158_METATILES
	layout_offset Field_Layout158, .LField_Layout158_SHAPES
	layout_offset Field_Layout158, .LField_Layout158_GRID
	layout_offset Field_Layout158, .LField_Layout158_ANIMATION
	layout_offset Field_Layout158
	layout_offset Field_Layout158, .LField_Layout158_MARKERS
.LField_Layout158_METATILES:
	.incbin "games/tbs/src/layout/layout_158/metatiles.delta2.lz"
	.balign 4, 0
.LField_Layout158_SHAPES:
	.incbin "games/tbs/src/layout/layout_158/shapes.table.lz"
	.balign 4, 0
.LField_Layout158_GRID:
	.incbin "games/tbs/src/layout/layout_158/grid.parts.lz"
	.balign 4, 0
.LField_Layout158_ANIMATION:
	.incbin "games/tbs/src/layout/layout_033/tileset_anims.script.lz"
	.balign 4, 0
.LField_Layout158_MARKERS:
	.incbin "games/tbs/src/layout/layout_000/treasure_spots.table"
	.balign 4, 0
	.global Tileset_Palette46
Tileset_Palette46:
	.incbin "games/tbs/src/graphics/tileset/palettes/pal_46.gbapal.lz"
	.byte 0x90, 0x27, 0xd4	@ PACKER: left in the buffer by Field_Layout158+0x1a5
