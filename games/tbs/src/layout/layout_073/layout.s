	.include "games/tbs/src/layout/layout.inc"
	.section .rodata
	.global Field_Layout073
Field_Layout073:
	layout_header 0, 0, 120, 120, 3, 2, 1, 0, 2, 2, 0, 0
	layout_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	layout_layer 0, 120, 16, 16, 0, 0, 0xff, 0xff
	layout_layer 120, 0, 16, 16, 0, 0, 0xff, 0xff
	layout_offset Field_Layout073, .LField_Layout073_METATILES
	layout_offset Field_Layout073, .LField_Layout073_SHAPES
	layout_offset Field_Layout073, .LField_Layout073_GRID
	layout_offset Field_Layout073, .LField_Layout073_ANIMATION
	layout_offset Field_Layout073, .LField_Layout073_BLEND
	layout_offset Field_Layout073, .LField_Layout073_MARKERS
.LField_Layout073_METATILES:
	.incbin "games/tbs/src/layout/layout_073/metatiles.delta1.lz"
	.balign 4, 0
.LField_Layout073_SHAPES:
	.incbin "games/tbs/src/layout/layout_073/shapes.table.lz"
	.balign 4, 0
.LField_Layout073_GRID:
	.incbin "games/tbs/src/layout/layout_073/map.grid.lz"
	.balign 4, 0
.LField_Layout073_ANIMATION:
	.incbin "games/tbs/src/layout/layout_073/tileset_anims.script.lz"
	.balign 4, 0
.LField_Layout073_BLEND:
	.incbin "games/tbs/src/layout/layout_073/blend.script.lz"
	.balign 4, 0
.LField_Layout073_MARKERS:
	treasure_spot 5, 3, 100
	treasure_spot 7, 3, 200
	treasure_spot 8, 3, 200
	treasure_spot 26, 3, 201
	treasure_spot 27, 3, 201
	treasure_spot 42, 3, 202
	treasure_spot 43, 3, 202
	treasure_spot 26, 4, 201
	treasure_spot 27, 4, 201
	treasure_spot 45, 4, 206
	treasure_spot 23, 6, 205
	treasure_spot 24, 6, 205
	treasure_spot 24, 11, 101
	treasure_spot 18, 22, 203
	treasure_spot 18, 23, 203
	treasure_spot 18, 24, 204
	treasure_spot 18, 25, 204
	treasure_spot 27, 27, 207
	treasure_spots_end
	.balign 4, 0
	.global Tileset_Set47Palette
Tileset_Set47Palette:
	.incbin "games/tbs/src/graphics/tileset/set_47/palette.gbapal.lz"
	.byte 0x98, 0xc9	@ PACKER: left in the buffer by Field_Layout073+0x192
	.global Tileset_Set47TilesA
Tileset_Set47TilesA:
	.incbin "games/tbs/src/graphics/tileset/set_47/tiles_a.4bpp.mtf"
	.byte 0x01, 0x6d	@ PACKER: left in the buffer by Field_Layout073+0x150e
	.global Tileset_Set47TilesB
Tileset_Set47TilesB:
	.incbin "games/tbs/src/graphics/tileset/set_47/tiles_b.4bpp.mtf"
	.byte 0xec	@ PACKER: left in the buffer by Field_Layout073+0x1de3
	.global Tileset_Set47TilesC
Tileset_Set47TilesC:
	.incbin "games/tbs/src/graphics/tileset/set_47/tiles_c.4bpp.mtf"
	.byte 0x2e, 0x1f	@ PACKER: left in the buffer by Tileset_Set47TilesB+0x1092
	.global Tileset_Set47TilesD
Tileset_Set47TilesD:
	.incbin "games/tbs/src/graphics/tileset/set_47/tiles_d.4bpp.mtf"
