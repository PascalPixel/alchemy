	.include "games/tbs/src/layout/layout.inc"
	.section .rodata
	.global Field_Layout160
Field_Layout160:
	layout_header 0, 0, 120, 120, 3, 1, 2, 0, 2, 2, 0, 0
	layout_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	layout_layer 120, 0, 16, 16, 0, 0, 0xff, 0xff
	layout_layer 0, 120, 16, 16, 0, 0, 0xff, 0xff
	layout_offset Field_Layout160, .LField_Layout160_METATILES
	layout_offset Field_Layout160, .LField_Layout160_SHAPES
	layout_offset Field_Layout160, .LField_Layout160_GRID
	layout_offset Field_Layout160, .LField_Layout160_ANIMATION
	layout_offset Field_Layout160, .LField_Layout160_BLEND
	layout_offset Field_Layout160, .LField_Layout160_MARKERS
.LField_Layout160_METATILES:
	.incbin "games/tbs/src/layout/layout_160/metatiles.delta2.lz"
	.balign 4, 0
.LField_Layout160_SHAPES:
	.incbin "games/tbs/src/layout/layout_160/shapes.table.lz"
	.balign 4, 0
.LField_Layout160_GRID:
	.incbin "games/tbs/src/layout/layout_160/grid.parts.lz"
	.balign 4, 0
.LField_Layout160_ANIMATION:
	.incbin "games/tbs/src/layout/layout_151/tileset_anims.script.lz"
	.balign 4, 0
.LField_Layout160_BLEND:
	.incbin "games/tbs/src/layout/layout_018/blend.script.lz"
	.balign 4, 0
.LField_Layout160_MARKERS:
	.incbin "games/tbs/src/layout/layout_160/treasure_spots.table"
	.balign 4, 0
	.global Tileset_Palette48
Tileset_Palette48:
	.incbin "games/tbs/src/graphics/tileset/palettes/pal_48.gbapal.lz"
	.byte 0x38, 0x16	@ PACKER: left in the buffer by Field_Layout160+0x1e6
