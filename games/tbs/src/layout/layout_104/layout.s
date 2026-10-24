	.include "games/tbs/src/layout/layout.inc"
	.section .rodata
	.global Field_Layout104
Field_Layout104:
	layout_header 0, 0, 46, 46, 3, 1, 2, 0, 2, 2, 0, 0
	layout_layer 0, 92, 6, 6, 0, 0, 0xff, 0xff
	layout_layer 0, 46, 16, 16, 0, 0, 0xff, 0xff
	layout_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	layout_offset Field_Layout104, .LField_Layout104_METATILES
	layout_offset Field_Layout104, .LField_Layout104_SHAPES
	layout_offset Field_Layout104, .LField_Layout104_GRID
	layout_offset Field_Layout104, .LField_Layout104_ANIMATION
	layout_offset Field_Layout104
	layout_offset Field_Layout104, .LField_Layout104_MARKERS
.LField_Layout104_METATILES:
	.incbin "games/tbs/src/layout/layout_104/metatiles.delta1.lz"
	.balign 4, 0
.LField_Layout104_SHAPES:
	.incbin "games/tbs/src/layout/layout_104/shapes.table.lz"
	.balign 4, 0
.LField_Layout104_GRID:
	.incbin "games/tbs/src/layout/layout_104/map.grid.lz"
	.balign 4, 0
.LField_Layout104_ANIMATION:
	.incbin "games/tbs/src/layout/layout_104/tileset_anims.script.lz"
	.balign 4, 0
.LField_Layout104_MARKERS:
	treasure_spots_end
	.balign 4, 0
	.global Tileset_Palette11
Tileset_Palette11:
	.incbin "games/tbs/src/graphics/tileset/palettes/pal_11.gbapal.lz"
	.byte 0xe7, 0x99	@ PACKER: left in the buffer by Field_Layout104+0x1d2
