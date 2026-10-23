	.include "games/tbs/src/layout/layout.inc"
	.section .rodata
	.global Field_Layout119
Field_Layout119:
	layout_header 0, 0, 76, 64, 3, 2, 1, 0, 2, 2, 0, 0
	layout_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	layout_layer 76, 0, 16, 16, 0, 0, 0xff, 0xff
	layout_layer 0, 76, 16, 16, 0, 0, 0xff, 0xff
	layout_offset Field_Layout119, .LField_Layout119_METATILES
	layout_offset Field_Layout119, .LField_Layout119_SHAPES
	layout_offset Field_Layout119, .LField_Layout119_GRID
	layout_offset Field_Layout119, .LField_Layout119_ANIMATION
	layout_offset Field_Layout119, .LField_Layout119_BLEND
	layout_offset Field_Layout119, .LField_Layout119_MARKERS
.LField_Layout119_METATILES:
	.incbin "games/tbs/src/layout/layout_119/metatiles.delta1.lz"
	.balign 4, 0
.LField_Layout119_SHAPES:
	.incbin "games/tbs/src/layout/layout_119/shapes.table.lz"
	.balign 4, 0
.LField_Layout119_GRID:
	.incbin "games/tbs/src/layout/layout_119/grid.parts.lz"
	.balign 4, 0
.LField_Layout119_ANIMATION:
	.incbin "games/tbs/src/layout/layout_119/tileset_anims.script.lz"
	.balign 4, 0
.LField_Layout119_BLEND:
	.incbin "games/tbs/src/layout/layout_020/blend.script.lz"
	.balign 4, 0
.LField_Layout119_MARKERS:
	.incbin "games/tbs/src/layout/layout_119/treasure_spots.table"
	.balign 4, 0
	.global Tileset_Palette25
Tileset_Palette25:
	.incbin "games/tbs/src/graphics/tileset/palettes/pal_25.gbapal.lz"
	.byte 0x5b, 0x24, 0xd1	@ PACKER: left in the buffer by Field_Layout119+0x18d
