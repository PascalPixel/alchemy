	.include "games/tbs/src/layout/layout.inc"
	.section .rodata
	.global Field_Layout094
Field_Layout094:
	layout_header 0, 0, 120, 120, 3, 1, 2, 0, 2, 2, 0, 0
	layout_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	layout_layer 120, 0, 16, 16, 0, 0, 0xff, 0xff
	layout_layer 0, 120, 16, 16, 0, 0, 0xff, 0xff
	layout_offset Field_Layout094, .LField_Layout094_METATILES
	layout_offset Field_Layout094, .LField_Layout094_SHAPES
	layout_offset Field_Layout094, .LField_Layout094_GRID
	layout_offset Field_Layout094, .LField_Layout094_ANIMATION
	layout_offset Field_Layout094, .LField_Layout094_BLEND
	layout_offset Field_Layout094, .LField_Layout094_MARKERS
.LField_Layout094_METATILES:
	.incbin "games/tbs/src/layout/layout_094/metatiles.delta2.lz"
	.balign 4, 0
.LField_Layout094_SHAPES:
	.incbin "games/tbs/src/layout/layout_094/shapes.table.lz"
	.balign 4, 0
.LField_Layout094_GRID:
	.incbin "games/tbs/src/layout/layout_094/grid.parts.lz"
	.balign 4, 0
.LField_Layout094_ANIMATION:
	.incbin "games/tbs/src/layout/layout_094/animation.script.lz"
	.balign 4, 0
.LField_Layout094_BLEND:
	.incbin "games/tbs/src/layout/layout_018/blend.script.lz"
	.balign 4, 0
.LField_Layout094_MARKERS:
	.incbin "games/tbs/src/layout/layout_094/markers.table"
	.balign 4, 0
	.global Tileset_Set52Palette
Tileset_Set52Palette:
	.incbin "games/tbs/src/graphics/tileset/set_52/palette.gbapal.lz"
	.byte 0xc0	@ PACKER: left in the buffer by Field_Layout094+0x1c7
	.global Tileset_Set52TilesA
Tileset_Set52TilesA:
	.incbin "games/tbs/src/graphics/tileset/set_52/tiles_a.4bpp.mtf"
	.global Tileset_Set52TilesB
Tileset_Set52TilesB:
	.incbin "games/tbs/src/graphics/tileset/set_52/tiles_b.4bpp.mtf"
	.global Tileset_Set52TilesC
Tileset_Set52TilesC:
	.incbin "games/tbs/src/graphics/tileset/set_52/tiles_c.4bpp.mtf"
	.global Tileset_Set52TilesD
Tileset_Set52TilesD:
	.incbin "games/tbs/src/graphics/tileset/set_52/tiles_d.4bpp.mtf"
