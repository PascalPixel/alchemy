	.include "games/tbs/src/layout/layout.inc"
	.section .rodata
	.global Field_Layout045
Field_Layout045:
	layout_header 0, 0, 64, 64, 3, 2, 1, 0, 2, 2, 0, 0
	layout_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	layout_layer 64, 0, 16, 16, 0, 0, 0xff, 0xff
	layout_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	layout_offset Field_Layout045, .LField_Layout045_METATILES
	layout_offset Field_Layout045, .LField_Layout045_SHAPES
	layout_offset Field_Layout045, .LField_Layout045_GRID
	layout_offset Field_Layout045
	layout_offset Field_Layout045
	layout_offset Field_Layout045, .LField_Layout045_MARKERS
.LField_Layout045_METATILES:
	.incbin "games/tbs/src/layout/layout_045/metatiles.delta0.lz"
	.balign 4, 0
.LField_Layout045_SHAPES:
	.incbin "games/tbs/src/layout/layout_045/shapes.table.lz"
	.balign 4, 0
.LField_Layout045_GRID:
	.incbin "games/tbs/src/layout/layout_045/grid.parts.lz"
	.balign 4, 0
.LField_Layout045_MARKERS:
	.incbin "games/tbs/src/layout/layout_000/markers.table"
	.balign 4, 0
	.global Tileset_Set35Palette
Tileset_Set35Palette:
	.incbin "games/tbs/src/graphics/tileset/set_35/palette.gbapal.lz"
	.byte 0x05	@ PACKER: left in the buffer by Field_Layout045+0x17b
	.global Tileset_Set35TilesA
Tileset_Set35TilesA:
	.incbin "games/tbs/src/graphics/tileset/set_35/tiles_a.4bpp.mtf"
	.byte 0x07, 0xf0	@ PACKER: left in the buffer by Tileset_Set34TilesA+0x2bae
	.global Tileset_Set35TilesB
Tileset_Set35TilesB:
	.incbin "games/tbs/src/graphics/tileset/set_35/tiles_b.4bpp.mtf"
	.byte 0x8f, 0x63	@ PACKER: left in the buffer by Tileset_Set35TilesA+0x2272
	.global Tileset_Set35TilesC
Tileset_Set35TilesC:
	.incbin "games/tbs/src/graphics/tileset/set_35/tiles_c.4bpp.mtf"
	.byte 0x6b	@ PACKER: left in the buffer by Tileset_Set35TilesA+0x286f
	.global Tileset_Set35TilesD
Tileset_Set35TilesD:
	.incbin "games/tbs/src/graphics/tileset/set_35/tiles_d.4bpp.mtf"
