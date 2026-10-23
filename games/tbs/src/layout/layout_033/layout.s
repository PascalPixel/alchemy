	.include "games/tbs/src/layout/layout.inc"
	.section .rodata
	.global Field_Layout033
Field_Layout033:
	layout_header 0, 0, 72, 94, 3, 2, 1, 0, 2, 2, 0, 0
	layout_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	layout_layer 0, 96, 16, 16, 0, 0, 0xff, 0xff
	layout_layer 80, 0, 16, 16, 0, 0, 0xff, 0xff
	layout_offset Field_Layout033, .LField_Layout033_METATILES
	layout_offset Field_Layout033, .LField_Layout033_SHAPES
	layout_offset Field_Layout033, .LField_Layout033_GRID
	layout_offset Field_Layout033, .LField_Layout033_ANIMATION
	layout_offset Field_Layout033, .LField_Layout033_BLEND
	layout_offset Field_Layout033, .LField_Layout033_MARKERS
.LField_Layout033_METATILES:
	.incbin "games/tbs/src/layout/layout_033/metatiles.delta2.lz"
	.balign 4, 0
.LField_Layout033_SHAPES:
	.incbin "games/tbs/src/layout/layout_033/shapes.table.lz"
	.balign 4, 0
.LField_Layout033_GRID:
	.incbin "games/tbs/src/layout/layout_033/grid.parts.lz"
	.balign 4, 0
.LField_Layout033_ANIMATION:
	.incbin "games/tbs/src/layout/layout_033/tileset_anims.script.lz"
	.balign 4, 0
.LField_Layout033_BLEND:
	.incbin "games/tbs/src/layout/layout_033/blend.script.lz"
	.balign 4, 0
.LField_Layout033_MARKERS:
	.incbin "games/tbs/src/layout/layout_000/treasure_spots.table"
	.balign 4, 0
	.global Tileset_Set25Palette
Tileset_Set25Palette:
	.incbin "games/tbs/src/graphics/tileset/set_25/palette.gbapal.lz"
	.byte 0xf3	@ PACKER: left in the buffer by Field_Layout033+0x15f
	.global Tileset_Set25TilesA
Tileset_Set25TilesA:
	.incbin "games/tbs/src/graphics/tileset/set_25/tiles_a.4bpp.mtf"
	.byte 0x06	@ PACKER: left in the buffer by Field_Layout032+0x1c57
