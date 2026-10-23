	.include "games/tbs/src/layout/layout.inc"
	.section .rodata
	.global Field_Layout102
Field_Layout102:
	layout_header 0, 0, 120, 120, 3, 1, 2, 0, 2, 2, 0, 0
	layout_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	layout_layer 120, 0, 16, 16, 0, 0, 0xff, 0xff
	layout_layer 0, 120, 16, 16, 0, 0, 0xff, 0xff
	layout_offset Field_Layout102, .LField_Layout102_METATILES
	layout_offset Field_Layout102, .LField_Layout102_SHAPES
	layout_offset Field_Layout102, .LField_Layout102_GRID
	layout_offset Field_Layout102, .LField_Layout102_ANIMATION
	layout_offset Field_Layout102, .LField_Layout102_BLEND
	layout_offset Field_Layout102, .LField_Layout102_MARKERS
.LField_Layout102_METATILES:
	.incbin "games/tbs/src/layout/layout_102/metatiles.delta2.lz"
	.balign 4, 0
.LField_Layout102_SHAPES:
	.incbin "games/tbs/src/layout/layout_102/shapes.table.lz"
	.balign 4, 0
.LField_Layout102_GRID:
	.incbin "games/tbs/src/layout/layout_102/map.grid.lz"
	.balign 4, 0
.LField_Layout102_ANIMATION:
	.incbin "games/tbs/src/layout/layout_018/tileset_anims.script.lz"
	.balign 4, 0
.LField_Layout102_BLEND:
	.incbin "games/tbs/src/layout/layout_018/blend.script.lz"
	.balign 4, 0
.LField_Layout102_MARKERS:
	.incbin "games/tbs/src/layout/layout_000/treasure_spots.table"
	.balign 4, 0
	.global Tileset_Set55Palette
Tileset_Set55Palette:
	.incbin "games/tbs/src/graphics/tileset/set_55/palette.gbapal.lz"
	.byte 0x05	@ PACKER: left in the buffer by Field_Layout102+0x1bf
	.global Tileset_Set55TilesA
Tileset_Set55TilesA:
	.incbin "games/tbs/src/graphics/tileset/set_55/tiles_a.4bpp.mtf"
