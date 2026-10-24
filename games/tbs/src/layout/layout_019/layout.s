	.include "games/tbs/src/layout/layout.inc"
	.section .rodata
	.global Field_Layout019
Field_Layout019:
	layout_header 0, 0, 120, 120, 3, 1, 2, 0, 2, 2, 0, 0
	layout_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	layout_layer 120, 0, 16, 16, 0, 0, 0xff, 0xff
	layout_layer 0, 120, 16, 16, 0, 0, 0xff, 0xff
	layout_offset Field_Layout019, .LField_Layout019_METATILES
	layout_offset Field_Layout019, .LField_Layout019_SHAPES
	layout_offset Field_Layout019, .LField_Layout019_GRID
	layout_offset Field_Layout019, .LField_Layout019_ANIMATION
	layout_offset Field_Layout019, .LField_Layout019_BLEND
	layout_offset Field_Layout019, .LField_Layout019_MARKERS
.LField_Layout019_METATILES:
	.incbin "games/tbs/src/layout/layout_019/metatiles.delta1.lz"
	.balign 4, 0
.LField_Layout019_SHAPES:
	.incbin "games/tbs/src/layout/layout_001/shapes.table.lz"
	.balign 4, 0
.LField_Layout019_GRID:
	.incbin "games/tbs/src/layout/layout_019/map.grid.lz"
	.balign 4, 0
.LField_Layout019_ANIMATION:
	.incbin "games/tbs/src/layout/layout_019/tileset_anims.script.lz"
	.balign 4, 0
.LField_Layout019_BLEND:
	.incbin "games/tbs/src/layout/layout_010/blend.script.lz"
	.balign 4, 0
.LField_Layout019_MARKERS:
	treasure_spot 32, 7, 100
	treasure_spot 33, 7, 101
	treasure_spot 35, 7, 102
	treasure_spot 36, 7, 103
	treasure_spot 33, 8, 104
	treasure_spot 35, 8, 105
	treasure_spots_end
	.balign 4, 0
	.global Tileset_Set16Palette
Tileset_Set16Palette:
	.incbin "games/tbs/src/graphics/tileset/set_16/palette.gbapal.lz"
	.byte 0x3f	@ PACKER: left in the buffer by Field_Layout019+0xf3
	.global Tileset_Set16TilesA
Tileset_Set16TilesA:
	.incbin "games/tbs/src/graphics/tileset/set_16/tiles_a.4bpp.mtf"
	.global Tileset_Set16TilesB
Tileset_Set16TilesB:
	.incbin "games/tbs/src/graphics/tileset/set_16/tiles_b.4bpp.mtf"
	.global Tileset_Set16TilesC
Tileset_Set16TilesC:
	.incbin "games/tbs/src/graphics/tileset/set_16/tiles_c.4bpp.mtf"
	.byte 0x1d, 0x65	@ PACKER: left in the buffer by Tileset_Set16TilesB+0x186
	.global Tileset_Set16TilesD
Tileset_Set16TilesD:
	.incbin "games/tbs/src/graphics/tileset/set_16/tiles_d.4bpp.mtf"
	.byte 0x82, 0xb0	@ PACKER: left in the buffer by Tileset_Set16TilesA+0xda2
