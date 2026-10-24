	.include "games/tbs/src/layout/layout.inc"
	.section .rodata
	.global Field_Layout016
Field_Layout016:
	layout_header 0, 0, 255, 81, 3, 2, 1, 0, 2, 2, 0, 0
	layout_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	layout_layer 0, 81, 16, 16, 0, 0, 0xff, 0xff
	layout_layer 0, 161, 16, 16, 0, 0, 0xff, 0xff
	layout_offset Field_Layout016, .LField_Layout016_METATILES
	layout_offset Field_Layout016, .LField_Layout016_SHAPES
	layout_offset Field_Layout016, .LField_Layout016_GRID
	layout_offset Field_Layout016, .LField_Layout016_ANIMATION
	layout_offset Field_Layout016, .LField_Layout016_BLEND
	layout_offset Field_Layout016, .LField_Layout016_MARKERS
.LField_Layout016_METATILES:
	.incbin "games/tbs/src/layout/layout_016/metatiles.delta1.lz"
	.balign 4, 0
.LField_Layout016_SHAPES:
	.incbin "games/tbs/src/layout/layout_015/shapes.table.lz"
	.balign 4, 0
.LField_Layout016_GRID:
	.incbin "games/tbs/src/layout/layout_016/map.grid.lz"
	.balign 4, 0
.LField_Layout016_ANIMATION:
	.incbin "games/tbs/src/layout/layout_012/tileset_anims.script.lz"
	.balign 4, 0
.LField_Layout016_BLEND:
	.incbin "games/tbs/src/layout/layout_012/blend.script.lz"
	.balign 4, 0
.LField_Layout016_MARKERS:
	treasure_spot 17, 3, 100
	treasure_spots_end
	.balign 4, 0
	.global Tileset_Palette00
Tileset_Palette00:
	.incbin "games/tbs/src/graphics/tileset/palettes/pal_00.gbapal.lz"
	.byte 0x41, 0x28, 0x40	@ PACKER: left in the buffer by Field_Layout016+0x159
