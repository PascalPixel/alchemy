	.include "games/tbs/src/layout/layout.inc"
	.section .rodata
	.global Field_Layout005
Field_Layout005:
	layout_header 0, 0, 120, 120, 3, 2, 1, 0, 2, 2, 0, 0
	layout_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	layout_layer 0, 120, 16, 16, 0, 0, 0xff, 0xff
	layout_layer 120, 0, 16, 16, 0, 0, 0xff, 0xff
	layout_offset Field_Layout005, .LField_Layout005_METATILES
	layout_offset Field_Layout005, .LField_Layout005_SHAPES
	layout_offset Field_Layout005, .LField_Layout005_GRID
	layout_offset Field_Layout005, .LField_Layout005_ANIMATION
	layout_offset Field_Layout005, .LField_Layout005_BLEND
	layout_offset Field_Layout005, .LField_Layout005_MARKERS
.LField_Layout005_METATILES:
	.incbin "games/tbs/src/layout/layout_005/metatiles.delta1.lz"
	.balign 4, 0
.LField_Layout005_SHAPES:
	.incbin "games/tbs/src/layout/layout_005/shapes.table.lz"
	.balign 4, 0
.LField_Layout005_GRID:
	.incbin "games/tbs/src/layout/layout_005/map.grid.lz"
	.balign 4, 0
.LField_Layout005_ANIMATION:
	.incbin "games/tbs/src/layout/layout_005/tileset_anims.script.lz"
	.balign 4, 0
.LField_Layout005_BLEND:
	.incbin "games/tbs/src/layout/layout_005/blend.script.lz"
	.balign 4, 0
.LField_Layout005_MARKERS:
	treasure_spot 5, 3, 200
	treasure_spot 6, 3, 200
	treasure_spot 4, 4, 200
	treasure_spot 5, 4, 200
	treasure_spot 6, 4, 200
	treasure_spot 22, 5, 204
	treasure_spot 23, 5, 204
	treasure_spot 24, 5, 204
	treasure_spot 43, 5, 202
	treasure_spot 44, 5, 202
	treasure_spot 45, 5, 202
	treasure_spot 26, 11, 100
	treasure_spot 46, 25, 205
	treasure_spot 47, 25, 205
	treasure_spot 29, 29, 101
	treasure_spot 26, 43, 203
	treasure_spot 27, 43, 203
	treasure_spot 28, 43, 203
	treasure_spot 49, 45, 201
	treasure_spot 50, 45, 201
	treasure_spot 39, 50, 104
	treasure_spot 27, 52, 103
	treasure_spots_end
	.balign 4, 0
	.global Tileset_Set04Palette
Tileset_Set04Palette:
	.incbin "games/tbs/src/graphics/tileset/set_04/palette.gbapal.lz"
	.global Tileset_Set04TilesA
Tileset_Set04TilesA:
	.incbin "games/tbs/src/graphics/tileset/set_04/tiles_a.4bpp.mtf"
	.global Tileset_Set04TilesB
Tileset_Set04TilesB:
	.incbin "games/tbs/src/graphics/tileset/set_04/tiles_b.4bpp.mtf"
	.byte 0x02, 0xf6, 0x07	@ PACKER: left in the buffer by Field_Layout005+0x253d
	.global Tileset_Set04TilesC
Tileset_Set04TilesC:
	.incbin "games/tbs/src/graphics/tileset/set_04/tiles_c.4bpp.mtf"
	.byte 0x86, 0xb4	@ PACKER: left in the buffer by Tileset_Set04TilesB+0x15fa
	.global Tileset_Set04TilesD
Tileset_Set04TilesD:
	.incbin "games/tbs/src/graphics/tileset/set_04/tiles_d.4bpp.mtf"
	.byte 0x3c	@ PACKER: left in the buffer by Tileset_Set04TilesC+0x12c7
