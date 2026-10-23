	.include "games/tbs/src/layout/layout.inc"
	.section .rodata
	.global Field_Layout048
Field_Layout048:
	layout_header 0, 0, 64, 64, 3, 2, 1, 0, 2, 2, 0, 0
	layout_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	layout_layer 64, 0, 16, 16, 0, 0, 0xff, 0xff
	layout_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	layout_offset Field_Layout048, .LField_Layout048_METATILES
	layout_offset Field_Layout048, .LField_Layout048_SHAPES
	layout_offset Field_Layout048, .LField_Layout048_GRID
	layout_offset Field_Layout048, .LField_Layout048_ANIMATION
	layout_offset Field_Layout048, .LField_Layout048_BLEND
	layout_offset Field_Layout048, .LField_Layout048_MARKERS
.LField_Layout048_METATILES:
	.incbin "games/tbs/src/layout/layout_048/metatiles.delta0.lz"
	.balign 4, 0
.LField_Layout048_SHAPES:
	.incbin "games/tbs/src/layout/layout_048/shapes.table.lz"
	.balign 4, 0
.LField_Layout048_GRID:
	.incbin "games/tbs/src/layout/layout_048/grid.parts.lz"
	.balign 4, 0
.LField_Layout048_ANIMATION:
	.incbin "games/tbs/src/layout/layout_048/tileset_anims.script.lz"
	.balign 4, 0
.LField_Layout048_BLEND:
	.incbin "games/tbs/src/layout/layout_020/blend.script.lz"
	.balign 4, 0
.LField_Layout048_MARKERS:
	.incbin "games/tbs/src/layout/layout_000/treasure_spots.table"
	.balign 4, 0
	.global Tileset_Set38Palette
Tileset_Set38Palette:
	.incbin "games/tbs/src/graphics/tileset/set_38/palette.gbapal.lz"
	.global Tileset_Set38TilesA
Tileset_Set38TilesA:
	.incbin "games/tbs/src/graphics/tileset/set_38/tiles_a.4bpp.mtf"
	.byte 0x80, 0x03	@ PACKER: left in the buffer by Field_Layout048+0x13da
