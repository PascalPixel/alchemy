	.include "games/tbs/src/layout/layout.inc"
	.section .rodata
	.global Field_Layout126
Field_Layout126:
	layout_header 0, 0, 64, 64, 3, 2, 1, 0, 2, 2, 0, 0
	layout_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	layout_layer 64, 0, 16, 16, 0, 0, 0xff, 0xff
	layout_layer 0, 64, 16, 16, 0, 0, 0xff, 0xff
	layout_offset Field_Layout126, .LField_Layout126_METATILES
	layout_offset Field_Layout126, .LField_Layout126_SHAPES
	layout_offset Field_Layout126, .LField_Layout126_GRID
	layout_offset Field_Layout126, .LField_Layout126_ANIMATION
	layout_offset Field_Layout126, .LField_Layout126_BLEND
	layout_offset Field_Layout126, .LField_Layout126_MARKERS
.LField_Layout126_METATILES:
	.incbin "games/tbs/src/layout/layout_126/metatiles.delta1.lz"
	.balign 4, 0
.LField_Layout126_SHAPES:
	.incbin "games/tbs/src/layout/layout_126/shapes.table.lz"
	.balign 4, 0
.LField_Layout126_GRID:
	.incbin "games/tbs/src/layout/layout_126/map.grid.lz"
	.balign 4, 0
.LField_Layout126_ANIMATION:
	.incbin "games/tbs/src/layout/layout_093/tileset_anims.script.lz"
	.balign 4, 0
.LField_Layout126_BLEND:
	.incbin "games/tbs/src/layout/layout_020/blend.script.lz"
	.balign 4, 0
.LField_Layout126_MARKERS:
	treasure_spots_end
	.balign 4, 0
	.global Tileset_Palette30
Tileset_Palette30:
	.incbin "games/tbs/src/graphics/tileset/palettes/pal_30.gbapal.lz"
	.byte 0x7f, 0x45, 0xd8	@ PACKER: left in the buffer by Field_Layout126+0x18d
