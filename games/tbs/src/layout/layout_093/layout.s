	.include "games/tbs/src/layout/layout.inc"
	.section .rodata
	.global Field_Layout093
Field_Layout093:
	layout_header 0, 0, 64, 64, 3, 2, 1, 0, 2, 2, 0, 0
	layout_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	layout_layer 64, 0, 16, 16, 0, 0, 0xff, 0xff
	layout_layer 0, 64, 16, 16, 0, 0, 0xff, 0xff
	layout_offset Field_Layout093, .LField_Layout093_METATILES
	layout_offset Field_Layout093, .LField_Layout093_SHAPES
	layout_offset Field_Layout093, .LField_Layout093_GRID
	layout_offset Field_Layout093, .LField_Layout093_ANIMATION
	layout_offset Field_Layout093, .LField_Layout093_BLEND
	layout_offset Field_Layout093, .LField_Layout093_MARKERS
.LField_Layout093_METATILES:
	.incbin "games/tbs/src/layout/layout_093/metatiles.delta1.lz"
	.balign 4, 0
.LField_Layout093_SHAPES:
	.incbin "games/tbs/src/layout/layout_093/shapes.table.lz"
	.balign 4, 0
.LField_Layout093_GRID:
	.incbin "games/tbs/src/layout/layout_093/map.grid.lz"
	.balign 4, 0
.LField_Layout093_ANIMATION:
	.incbin "games/tbs/src/layout/layout_093/tileset_anims.script.lz"
	.balign 4, 0
.LField_Layout093_BLEND:
	.incbin "games/tbs/src/layout/layout_020/blend.script.lz"
	.balign 4, 0
.LField_Layout093_MARKERS:
	treasure_spot 6, 8, 100
	treasure_spot 5, 25, 101
	treasure_spots_end
	.balign 4, 0
	.global Tileset_Set51Palette
Tileset_Set51Palette:
	.incbin "games/tbs/src/graphics/tileset/set_51/palette.gbapal.lz"
	.byte 0x94	@ PACKER: left in the buffer by Field_Layout093+0x17b
	.balign 4, 0
	.global Tileset_Set51TilesA
Tileset_Set51TilesA:
	.incbin "games/tbs/src/graphics/tileset/set_51/tiles_a.4bpp.mtf"
	.balign 4, 0	@ PACKER: zero fill, not an earlier stream's bytes
	.global Tileset_Set51TilesB
Tileset_Set51TilesB:
	.incbin "games/tbs/src/graphics/tileset/set_51/tiles_b.4bpp.mtf"
	.byte 0xd1, 0xbc	@ PACKER: left in the buffer by Field_Layout092+0x2776
	.global Tileset_Set51TilesC
Tileset_Set51TilesC:
	.incbin "games/tbs/src/graphics/tileset/set_51/tiles_c.4bpp.mtf"
	.byte 0x24, 0xda, 0xbb	@ PACKER: left in the buffer by Tileset_Set51TilesB+0x1865
	.global Tileset_Set51TilesD
Tileset_Set51TilesD:
	.incbin "games/tbs/src/graphics/tileset/set_51/tiles_d.4bpp.mtf"
