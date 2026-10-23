	.include "games/tbs/src/layout/layout.inc"
	.section .rodata
	.global Field_Layout020
Field_Layout020:
	layout_header 0, 0, 62, 58, 3, 2, 1, 0, 2, 2, 0, 0
	layout_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	layout_layer 62, 0, 16, 16, 0, 0, 0xff, 0xff
	layout_layer 124, 0, 16, 16, 0, 0, 0xff, 0xff
	layout_offset Field_Layout020, .LField_Layout020_METATILES
	layout_offset Field_Layout020, .LField_Layout020_SHAPES
	layout_offset Field_Layout020, .LField_Layout020_GRID
	layout_offset Field_Layout020, .LField_Layout020_ANIMATION
	layout_offset Field_Layout020, .LField_Layout020_BLEND
	layout_offset Field_Layout020, .LField_Layout020_MARKERS
.LField_Layout020_METATILES:
	.incbin "games/tbs/src/layout/layout_020/metatiles.delta0.lz"
	.balign 4, 0
.LField_Layout020_SHAPES:
	.incbin "games/tbs/src/layout/layout_020/shapes.table.lz"
	.balign 4, 0
.LField_Layout020_GRID:
	.incbin "games/tbs/src/layout/layout_020/map.grid.lz"
	.balign 4, 0
.LField_Layout020_ANIMATION:
	.incbin "games/tbs/src/layout/layout_020/tileset_anims.script.lz"
	.balign 4, 0
.LField_Layout020_BLEND:
	.incbin "games/tbs/src/layout/layout_020/blend.script.lz"
	.balign 4, 0
.LField_Layout020_MARKERS:
	.incbin "games/tbs/src/layout/layout_000/treasure_spots.table"
	.balign 4, 0
	.global Tileset_Set17Palette
Tileset_Set17Palette:
	.incbin "games/tbs/src/graphics/tileset/set_17/palette.gbapal.lz"
	.global Tileset_Set17TilesA
Tileset_Set17TilesA:
	.incbin "games/tbs/src/graphics/tileset/set_17/tiles_a.4bpp.mtf"
	.byte 0xe1, 0xe8, 0xf4	@ PACKER: left in the buffer by Field_Layout006+0x2d51
	.global Tileset_Set17TilesB
Tileset_Set17TilesB:
	.incbin "games/tbs/src/graphics/tileset/set_17/tiles_b.4bpp.mtf"
	.global Tileset_Set17TilesC
Tileset_Set17TilesC:
	.incbin "games/tbs/src/graphics/tileset/set_17/tiles_c.4bpp.mtf"
	.global Tileset_Set17TilesD
Tileset_Set17TilesD:
	.incbin "games/tbs/src/graphics/tileset/set_17/tiles_d.4bpp.mtf"
