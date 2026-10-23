	.include "games/tbs/src/layout/layout.inc"
	.section .rodata
	.global Field_Layout130
Field_Layout130:
	layout_header 0, 0, 94, 94, 3, 2, 1, 0, 2, 2, 0, 0
	layout_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	layout_layer 0, 96, 16, 16, 0, 0, 0xff, 0xff
	layout_layer 100, 0, 16, 16, 0, 0, 0xff, 0xff
	layout_offset Field_Layout130, .LField_Layout130_METATILES
	layout_offset Field_Layout130, .LField_Layout130_SHAPES
	layout_offset Field_Layout130, .LField_Layout130_GRID
	layout_offset Field_Layout130, .LField_Layout130_ANIMATION
	layout_offset Field_Layout130, .LField_Layout130_BLEND
	layout_offset Field_Layout130, .LField_Layout130_MARKERS
.LField_Layout130_METATILES:
	.incbin "games/tbs/src/layout/layout_130/metatiles.delta2.lz"
	.balign 4, 0
.LField_Layout130_SHAPES:
	.incbin "games/tbs/src/layout/layout_130/shapes.table.lz"
	.balign 4, 0
.LField_Layout130_GRID:
	.incbin "games/tbs/src/layout/layout_130/grid.parts.lz"
	.balign 4, 0
.LField_Layout130_ANIMATION:
	.incbin "games/tbs/src/layout/layout_130/animation.script.lz"
	.balign 4, 0
.LField_Layout130_BLEND:
	.incbin "games/tbs/src/layout/layout_020/blend.script.lz"
	.balign 4, 0
.LField_Layout130_MARKERS:
	.incbin "games/tbs/src/layout/layout_000/markers.table"
	.balign 4, 0
	.global Tileset_Palette31
Tileset_Palette31:
	.incbin "games/tbs/src/graphics/tileset/palettes/pal_31.gbapal.lz"
	.byte 0xc8, 0x1f, 0x81	@ PACKER: left in the buffer by Field_Layout130+0x159
