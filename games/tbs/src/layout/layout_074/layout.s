	.include "games/tbs/src/layout/layout.inc"
	.section .rodata
	.global Field_Layout074
Field_Layout074:
	layout_header 0, 0, 72, 66, 3, 2, 1, 0, 2, 2, 0, 0
	layout_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	layout_layer 72, 0, 16, 16, 0, 0, 0xff, 0xff
	layout_layer 144, 0, 16, 16, 0, 0, 0xff, 0xff
	layout_offset Field_Layout074, .LField_Layout074_METATILES
	layout_offset Field_Layout074, .LField_Layout074_SHAPES
	layout_offset Field_Layout074, .LField_Layout074_GRID
	layout_offset Field_Layout074, .LField_Layout074_ANIMATION
	layout_offset Field_Layout074, .LField_Layout074_BLEND
	layout_offset Field_Layout074, .LField_Layout074_MARKERS
.LField_Layout074_METATILES:
	.incbin "games/tbs/src/layout/layout_074/metatiles.delta0.lz"
	.balign 4, 0
.LField_Layout074_SHAPES:
	.incbin "games/tbs/src/layout/layout_074/shapes.table.lz"
	.balign 4, 0
.LField_Layout074_GRID:
	.incbin "games/tbs/src/layout/layout_074/map.grid.lz"
	.balign 4, 0
.LField_Layout074_ANIMATION:
	.incbin "games/tbs/src/layout/layout_074/tileset_anims.script.lz"
	.balign 4, 0
.LField_Layout074_BLEND:
	.incbin "games/tbs/src/layout/layout_020/blend.script.lz"
	.balign 4, 0
.LField_Layout074_MARKERS:
	treasure_spot 27, 24, 100
	treasure_spots_end
	.balign 4, 0
	.global Tileset_Palette04
Tileset_Palette04:
	.incbin "games/tbs/src/graphics/tileset/palettes/pal_04.gbapal.lz"
	.byte 0xdb, 0x0e, 0xea	@ PACKER: left in the buffer by Field_Layout074+0x195
