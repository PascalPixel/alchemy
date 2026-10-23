	.include "games/tbs/src/layout/layout.inc"
	.section .rodata
	.global Field_Layout022
Field_Layout022:
	layout_header 0, 0, 96, 64, 3, 2, 1, 0, 2, 2, 0, 0
	layout_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	layout_layer 0, 68, 16, 16, 0, 0, 0xff, 0xff
	layout_layer 100, 0, 16, 16, 0, 0, 0xff, 0xff
	layout_offset Field_Layout022, .LField_Layout022_METATILES
	layout_offset Field_Layout022, .LField_Layout022_SHAPES
	layout_offset Field_Layout022, .LField_Layout022_GRID
	layout_offset Field_Layout022, .LField_Layout022_ANIMATION
	layout_offset Field_Layout022, .LField_Layout022_BLEND
	layout_offset Field_Layout022, .LField_Layout022_MARKERS
.LField_Layout022_METATILES:
	.incbin "games/tbs/src/layout/layout_022/metatiles.delta2.lz"
	.balign 4, 0
.LField_Layout022_SHAPES:
	.incbin "games/tbs/src/layout/layout_022/shapes.table.lz"
	.balign 4, 0
.LField_Layout022_GRID:
	.incbin "games/tbs/src/layout/layout_022/grid.parts.lz"
	.balign 4, 0
.LField_Layout022_ANIMATION:
	.incbin "games/tbs/src/layout/layout_022/animation.script.lz"
	.balign 4, 0
.LField_Layout022_BLEND:
	.incbin "games/tbs/src/layout/layout_022/blend.script.lz"
	.balign 4, 0
.LField_Layout022_MARKERS:
	.incbin "games/tbs/src/layout/layout_022/markers.table"
	.balign 4, 0
	.global Tileset_Set19Palette
Tileset_Set19Palette:
	.incbin "games/tbs/src/graphics/tileset/set_19/palette.gbapal.lz"
	.global Tileset_Set19TilesA
Tileset_Set19TilesA:
	.incbin "games/tbs/src/graphics/tileset/set_19/tiles_a.4bpp.mtf"
	.byte 0x12, 0x00	@ PACKER: left in the buffer by Tileset_Set17TilesA+0x2c16
	.global Tileset_Set19TilesB
Tileset_Set19TilesB:
	.incbin "games/tbs/src/graphics/tileset/set_19/tiles_b.4bpp.mtf"
	.byte 0x27, 0x88	@ PACKER: left in the buffer by Tileset_Set19TilesA+0x20fa
	.global Tileset_Set19TilesC
Tileset_Set19TilesC:
	.incbin "games/tbs/src/graphics/tileset/set_19/tiles_c.4bpp.mtf"
	.global Tileset_Set19TilesD
Tileset_Set19TilesD:
	.incbin "games/tbs/src/graphics/tileset/set_19/tiles_d.4bpp.mtf"
