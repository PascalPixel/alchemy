	.include "games/tbs/src/layout/layout.inc"
	.section .rodata
	.global Field_Layout040
Field_Layout040:
	layout_header 0, 0, 72, 94, 3, 2, 1, 0, 2, 2, 0, 0
	layout_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	layout_layer 0, 96, 16, 16, 0, 0, 0xff, 0xff
	layout_layer 80, 0, 16, 16, 0, 0, 0xff, 0xff
	layout_offset Field_Layout040, .LField_Layout040_METATILES
	layout_offset Field_Layout040, .LField_Layout040_SHAPES
	layout_offset Field_Layout040, .LField_Layout040_GRID
	layout_offset Field_Layout040, .LField_Layout040_ANIMATION
	layout_offset Field_Layout040, .LField_Layout040_BLEND
	layout_offset Field_Layout040, .LField_Layout040_MARKERS
.LField_Layout040_METATILES:
	.incbin "games/tbs/src/layout/layout_040/metatiles.delta2.lz"
	.balign 4, 0
.LField_Layout040_SHAPES:
	.incbin "games/tbs/src/layout/layout_040/shapes.table.lz"
	.balign 4, 0
.LField_Layout040_GRID:
	.incbin "games/tbs/src/layout/layout_040/grid.parts.lz"
	.balign 4, 0
.LField_Layout040_ANIMATION:
	.incbin "games/tbs/src/layout/layout_033/tileset_anims.script.lz"
	.balign 4, 0
.LField_Layout040_BLEND:
	.incbin "games/tbs/src/layout/layout_040/blend.script.lz"
	.balign 4, 0
.LField_Layout040_MARKERS:
	.incbin "games/tbs/src/layout/layout_000/treasure_spots.table"
	.balign 4, 0
	.global Tileset_Palette01
Tileset_Palette01:
	.incbin "games/tbs/src/graphics/tileset/palettes/pal_01.gbapal.lz"
	.byte 0x94, 0x01, 0x06	@ PACKER: left in the buffer by Field_Layout040+0x159
