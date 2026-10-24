	.include "games/tbs/src/layout/layout.inc"
	.section .rodata
	.global Field_Layout101
Field_Layout101:
	layout_header 5, 124, 51, 188, 3, 2, 1, 1, 2, 2, 0, 0
	layout_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	layout_layer 60, 0, 16, 16, 0, 0, 0xff, 0xff
	layout_layer 120, 0, 16, 16, 0, 0, 0xff, 0xff
	layout_offset Field_Layout101, .LField_Layout101_METATILES
	layout_offset Field_Layout101, .LField_Layout101_SHAPES
	layout_offset Field_Layout101, .LField_Layout101_GRID
	layout_offset Field_Layout101, .LField_Layout101_ANIMATION
	layout_offset Field_Layout101
	layout_offset Field_Layout101, .LField_Layout101_MARKERS
.LField_Layout101_METATILES:
	.incbin "games/tbs/src/layout/layout_101/metatiles.delta1.lz"
	.balign 4, 0
.LField_Layout101_SHAPES:
	.incbin "games/tbs/src/layout/layout_101/shapes.table.lz"
	.balign 4, 0
.LField_Layout101_GRID:
	.incbin "games/tbs/src/layout/layout_101/map.grid.lz"
	.balign 4, 0
.LField_Layout101_ANIMATION:
	.incbin "games/tbs/src/layout/layout_101/tileset_anims.script.lz"
	.balign 4, 0
.LField_Layout101_MARKERS:
	treasure_spot 53, 56, 101
	treasure_spots_end
	.balign 4, 0
	.global Tileset_Set54Palette
Tileset_Set54Palette:
	.incbin "games/tbs/src/graphics/tileset/set_54/palette.gbapal.lz"
	.byte 0x2a, 0x91, 0x1b	@ PACKER: left in the buffer by Field_Layout101+0x1a5
	.global Tileset_Set54TilesA
Tileset_Set54TilesA:
	.incbin "games/tbs/src/graphics/tileset/set_54/tiles_a.4bpp.mtf"
	.global Tileset_Set54TilesB
Tileset_Set54TilesB:
	.incbin "games/tbs/src/graphics/tileset/set_54/tiles_b.4bpp.mtf"
	.byte 0x83	@ PACKER: left in the buffer by Tileset_Set54TilesA+0x23e3
	.global Tileset_Set54TilesC
Tileset_Set54TilesC:
	.incbin "games/tbs/src/graphics/tileset/set_54/tiles_c.4bpp.mtf"
