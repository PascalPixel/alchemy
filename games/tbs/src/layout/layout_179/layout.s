	.include "games/tbs/src/layout/layout.inc"
	.section .rodata
	.global Field_Layout179
Field_Layout179:
	layout_header 0, 0, 64, 64, 3, 0, 0, 0, 2, 2, 0, 0
	layout_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	layout_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	layout_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	layout_offset Field_Layout179, .LField_Layout179_METATILES
	layout_offset Field_Layout179, .LField_Layout179_SHAPES
	layout_offset Field_Layout179, .LField_Layout179_GRID
	layout_offset Field_Layout179
	layout_offset Field_Layout179
	layout_offset Field_Layout179, .LField_Layout179_MARKERS
.LField_Layout179_METATILES:
	.incbin "games/tbs/src/layout/layout_001/metatiles.delta0.lz"
	.balign 4, 0
.LField_Layout179_SHAPES:
	.incbin "games/tbs/src/layout/layout_001/shapes.table.lz"
	.balign 4, 0
.LField_Layout179_GRID:
	.incbin "games/tbs/src/layout/layout_001/map.grid.lz"
	.balign 4, 0
.LField_Layout179_MARKERS:
	treasure_spots_end
	.balign 4, 0
	.global Tileset_Set75Palette
Tileset_Set75Palette:
	.incbin "games/tbs/src/graphics/tileset/set_68/palette.gbapal.lz"
	.byte 0x00	@ PACKER: left in the buffer by Field_Layout179+0xb
	.global Tileset_Set75TilesA
Tileset_Set75TilesA:
	.incbin "games/tbs/src/graphics/tileset/set_68/tiles_a.4bpp.mtf"
	.byte 0xff, 0x00	@ PACKER: left in the buffer by Field_Layout179+0x14e
	.global Tileset_Set75TilesB
Tileset_Set75TilesB:
	.incbin "games/tbs/src/graphics/tileset/set_68/tiles_b.4bpp.mtf"
	.byte 0x07	@ PACKER: left in the buffer by Tileset_Set75TilesA+0x13f
	.global Tileset_Set75TilesC
Tileset_Set75TilesC:
	.incbin "games/tbs/src/graphics/tileset/set_68/tiles_b.4bpp.mtf"
	.byte 0x07	@ PACKER: left in the buffer by Tileset_Set75TilesA+0x13f
	.global Tileset_Set75TilesD
Tileset_Set75TilesD:
	.incbin "games/tbs/src/graphics/tileset/set_68/tiles_b.4bpp.mtf"
	.byte 0x07	@ PACKER: left in the buffer by Tileset_Set75TilesA+0x13f
