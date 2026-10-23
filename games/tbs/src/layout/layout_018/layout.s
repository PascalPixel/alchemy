	.include "games/tbs/src/layout/layout.inc"
	.section .rodata
	.global Field_Layout018
Field_Layout018:
	layout_header 0, 0, 120, 120, 3, 1, 2, 0, 2, 2, 0, 0
	layout_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	layout_layer 120, 0, 16, 16, 0, 0, 0xff, 0xff
	layout_layer 0, 120, 16, 16, 0, 0, 0xff, 0xff
	layout_offset Field_Layout018, .LField_Layout018_METATILES
	layout_offset Field_Layout018, .LField_Layout018_SHAPES
	layout_offset Field_Layout018, .LField_Layout018_GRID
	layout_offset Field_Layout018, .LField_Layout018_ANIMATION
	layout_offset Field_Layout018, .LField_Layout018_BLEND
	layout_offset Field_Layout018, .LField_Layout018_MARKERS
.LField_Layout018_METATILES:
	.incbin "games/tbs/src/layout/layout_018/metatiles.delta1.lz"
	.balign 4, 0
.LField_Layout018_SHAPES:
	.incbin "games/tbs/src/layout/layout_018/shapes.table.lz"
	.balign 4, 0
.LField_Layout018_GRID:
	.incbin "games/tbs/src/layout/layout_018/map.grid.lz"
	.balign 4, 0
.LField_Layout018_ANIMATION:
	.incbin "games/tbs/src/layout/layout_018/tileset_anims.script.lz"
	.balign 4, 0
.LField_Layout018_BLEND:
	.incbin "games/tbs/src/layout/layout_018/blend.script.lz"
	.balign 4, 0
.LField_Layout018_MARKERS:
	.incbin "games/tbs/src/layout/layout_018/treasure_spots.table"
	.balign 4, 0
	.global Tileset_Set15Palette
Tileset_Set15Palette:
	.incbin "games/tbs/src/graphics/tileset/set_15/palette.gbapal.lz"
	.global Tileset_Set15TilesA
Tileset_Set15TilesA:
	.incbin "games/tbs/src/graphics/tileset/set_15/tiles_a.4bpp.mtf"
	.byte 0x80, 0x36, 0x05	@ PACKER: left in the buffer by Field_Layout018+0x1c29
	.global Tileset_Set15TilesB
Tileset_Set15TilesB:
	.incbin "games/tbs/src/graphics/tileset/set_15/tiles_b.4bpp.mtf"
	.byte 0x3b	@ PACKER: left in the buffer by Field_Layout018+0x2547
	.global Tileset_Set15TilesC
Tileset_Set15TilesC:
	.incbin "games/tbs/src/graphics/tileset/set_15/tiles_c.4bpp.mtf"
	.byte 0x00, 0xa5	@ PACKER: left in the buffer by Tileset_Set15TilesB+0x12ca
	.global Tileset_Set15TilesD
Tileset_Set15TilesD:
	.incbin "games/tbs/src/graphics/tileset/set_15/tiles_d.4bpp.mtf"
	.byte 0x6f	@ PACKER: left in the buffer by Tileset_Set15TilesC+0xcaf
