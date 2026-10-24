	.include "games/tbs/src/layout/layout.inc"
	.section .rodata
	.global Field_Layout149
Field_Layout149:
	layout_header 0, 0, 128, 128, 3, 1, 2, 1, 1, 2, 0, 0
	layout_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	layout_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	layout_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	layout_offset Field_Layout149, .LField_Layout149_METATILES
	layout_offset Field_Layout149, .LField_Layout149_SHAPES
	layout_offset Field_Layout149, .LField_Layout149_GRID
	layout_offset Field_Layout149, .LField_Layout149_ANIMATION
	layout_offset Field_Layout149, .LField_Layout149_BLEND
	layout_offset Field_Layout149, .LField_Layout149_MARKERS
.LField_Layout149_METATILES:
	.incbin "games/tbs/src/layout/layout_149/metatiles.delta1.lz"
	.balign 4, 0
.LField_Layout149_SHAPES:
	.incbin "games/tbs/src/layout/layout_130/shapes.table.lz"
	.balign 4, 0
.LField_Layout149_GRID:
	.incbin "games/tbs/src/layout/layout_149/map.grid.lz"
	.balign 4, 0
.LField_Layout149_ANIMATION:
	.incbin "games/tbs/src/layout/layout_080/tileset_anims.script.lz"
	.balign 4, 0
.LField_Layout149_BLEND:
	.incbin "games/tbs/src/layout/layout_025/blend.script.lz"
	.balign 4, 0
.LField_Layout149_MARKERS:
	treasure_spots_end
	.balign 4, 0
	.global Tileset_Palette40
Tileset_Palette40:
	.incbin "games/tbs/src/graphics/tileset/palettes/pal_40.gbapal.lz"
	.byte 0xb5, 0x03	@ PACKER: left in the buffer by Field_Layout149+0x172
