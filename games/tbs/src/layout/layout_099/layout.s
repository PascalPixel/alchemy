	.include "games/tbs/src/layout/layout.inc"
	.section .rodata
	.global Field_Layout099
Field_Layout099:
	layout_header 0, 0, 120, 120, 3, 1, 2, 0, 2, 2, 0, 0
	layout_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	layout_layer 120, 0, 16, 16, 0, 0, 0xff, 0xff
	layout_layer 0, 120, 16, 16, 0, 0, 0xff, 0xff
	layout_offset Field_Layout099, .LField_Layout099_METATILES
	layout_offset Field_Layout099, .LField_Layout099_SHAPES
	layout_offset Field_Layout099, .LField_Layout099_GRID
	layout_offset Field_Layout099, .LField_Layout099_ANIMATION
	layout_offset Field_Layout099, .LField_Layout099_BLEND
	layout_offset Field_Layout099, .LField_Layout099_MARKERS
.LField_Layout099_METATILES:
	.incbin "games/tbs/src/layout/layout_099/metatiles.delta2.lz"
	.balign 4, 0
.LField_Layout099_SHAPES:
	.incbin "games/tbs/src/layout/layout_099/shapes.table.lz"
	.balign 4, 0
.LField_Layout099_GRID:
	.incbin "games/tbs/src/layout/layout_099/map.grid.lz"
	.balign 4, 0
.LField_Layout099_ANIMATION:
	.incbin "games/tbs/src/layout/layout_052/tileset_anims.script.lz"
	.balign 4, 0
.LField_Layout099_BLEND:
	.incbin "games/tbs/src/layout/layout_018/blend.script.lz"
	.balign 4, 0
.LField_Layout099_MARKERS:
	.incbin "games/tbs/src/layout/layout_099/treasure_spots.table"
	.balign 4, 0
	.global Tileset_Palette09
Tileset_Palette09:
	.incbin "games/tbs/src/graphics/tileset/palettes/pal_09.gbapal.lz"
	.byte 0xcc	@ PACKER: left in the buffer by Field_Layout099+0x1c3
