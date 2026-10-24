	.include "games/tbs/src/layout/layout.inc"
	.section .rodata
	.global Field_Layout076
Field_Layout076:
	layout_header 0, 0, 120, 120, 3, 2, 1, 0, 2, 2, 0, 0
	layout_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	layout_layer 0, 120, 16, 16, 0, 0, 0xff, 0xff
	layout_layer 120, 0, 16, 16, 0, 0, 0xff, 0xff
	layout_offset Field_Layout076, .LField_Layout076_METATILES
	layout_offset Field_Layout076, .LField_Layout076_SHAPES
	layout_offset Field_Layout076, .LField_Layout076_GRID
	layout_offset Field_Layout076, .LField_Layout076_ANIMATION
	layout_offset Field_Layout076, .LField_Layout076_BLEND
	layout_offset Field_Layout076, .LField_Layout076_MARKERS
.LField_Layout076_METATILES:
	.incbin "games/tbs/src/layout/layout_076/metatiles.delta1.lz"
	.balign 4, 0
.LField_Layout076_SHAPES:
	.incbin "games/tbs/src/layout/layout_076/shapes.table.lz"
	.balign 4, 0
.LField_Layout076_GRID:
	.incbin "games/tbs/src/layout/layout_076/map.grid.lz"
	.balign 4, 0
.LField_Layout076_ANIMATION:
	.incbin "games/tbs/src/layout/layout_032/tileset_anims.script.lz"
	.balign 4, 0
.LField_Layout076_BLEND:
	.incbin "games/tbs/src/layout/layout_005/blend.script.lz"
	.balign 4, 0
.LField_Layout076_MARKERS:
	treasure_spot 24, 7, 200
	treasure_spot 25, 7, 200
	treasure_spot 10, 13, 100
	treasure_spot 10, 30, 101
	treasure_spot 3, 43, 203
	treasure_spot 4, 43, 203
	treasure_spot 21, 45, 202
	treasure_spot 22, 45, 202
	treasure_spot 23, 45, 201
	treasure_spot 24, 45, 201
	treasure_spot 52, 45, 102
	treasure_spots_end
	.global Tileset_Set49Palette
Tileset_Set49Palette:
	.incbin "games/tbs/src/graphics/tileset/set_49/palette.gbapal.lz"
	.byte 0xb3, 0xec, 0xe0	@ PACKER: left in the buffer by Field_Layout076+0x1c9
	.global Tileset_Set49TilesA
Tileset_Set49TilesA:
	.incbin "games/tbs/src/graphics/tileset/set_49/tiles_a.4bpp.mtf"
	.byte 0x00, 0xc1, 0x01	@ PACKER: left in the buffer by Field_Layout076+0x2345
