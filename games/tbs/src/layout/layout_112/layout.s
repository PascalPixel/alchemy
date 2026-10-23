	.include "games/tbs/src/layout/layout.inc"
	.section .rodata
	.global Field_Layout112
Field_Layout112:
	layout_header 0, 0, 64, 64, 3, 2, 1, 0, 2, 2, 0, 0
	layout_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	layout_layer 64, 0, 16, 16, 0, 0, 0xff, 0xff
	layout_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	layout_offset Field_Layout112, .LField_Layout112_METATILES
	layout_offset Field_Layout112, .LField_Layout112_SHAPES
	layout_offset Field_Layout112, .LField_Layout112_GRID
	layout_offset Field_Layout112, .LField_Layout112_ANIMATION
	layout_offset Field_Layout112
	layout_offset Field_Layout112, .LField_Layout112_MARKERS
.LField_Layout112_METATILES:
	.incbin "games/tbs/src/layout/layout_112/metatiles.delta0.lz"
	.balign 4, 0
.LField_Layout112_SHAPES:
	.incbin "games/tbs/src/layout/layout_112/shapes.table.lz"
	.balign 4, 0
.LField_Layout112_GRID:
	.incbin "games/tbs/src/layout/layout_112/map.grid.lz"
	.balign 4, 0
.LField_Layout112_ANIMATION:
	.incbin "games/tbs/src/layout/layout_112/tileset_anims.script.lz"
	.balign 4, 0
.LField_Layout112_MARKERS:
	.incbin "games/tbs/src/layout/layout_112/treasure_spots.table"
	.balign 4, 0
	.global Tileset_Palette17
Tileset_Palette17:
	.incbin "games/tbs/src/graphics/tileset/palettes/pal_17.gbapal.lz"
	.byte 0x53	@ PACKER: left in the buffer by Field_Layout112+0x12b
	.global Tileset_Palette18
Tileset_Palette18:
	.incbin "games/tbs/src/graphics/tileset/palettes/pal_18.gbapal.lz"
	.byte 0x00, 0x00, 0x38	@ PACKER: unused post-stream residue; MapGrid_LoadLayout copies decoded palette RAM; writer unknown
