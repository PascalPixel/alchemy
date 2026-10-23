	.include "games/tbs/src/layout/layout.inc"
	.section .rodata
	.global Field_Map005
Field_Map005:
	map_header 0, 0, 120, 120, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 120, 16, 16, 0, 0, 0xff, 0xff
	map_layer 120, 0, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map005, .LField_Map005_METATILES
	map_block Field_Map005, .LField_Map005_SHAPES
	map_block Field_Map005, .LField_Map005_GRID
	map_block Field_Map005, .LField_Map005_ANIMATION
	map_block Field_Map005, .LField_Map005_BLEND
	map_block Field_Map005, .LField_Map005_MARKERS
.LField_Map005_METATILES:
	.incbin "games/tbs/src/layout/layout_005/metatiles.delta1.lz"
	.balign 4, 0
.LField_Map005_SHAPES:
	.incbin "games/tbs/src/layout/layout_005/shapes.table.lz"
	.balign 4, 0
.LField_Map005_GRID:
	.incbin "games/tbs/src/layout/layout_005/grid.parts.lz"
	.balign 4, 0
.LField_Map005_ANIMATION:
	.incbin "games/tbs/src/layout/layout_005/animation.script.lz"
	.balign 4, 0
.LField_Map005_BLEND:
	.incbin "games/tbs/src/layout/layout_005/blend.script.lz"
	.balign 4, 0
.LField_Map005_MARKERS:
	.incbin "games/tbs/src/layout/layout_005/markers.table"
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
	.byte 0x02, 0xf6, 0x07	@ PACKER: left in the buffer by Field_Map005+0x253d
	.global Tileset_Set04TilesC
Tileset_Set04TilesC:
	.incbin "games/tbs/src/graphics/tileset/set_04/tiles_c.4bpp.mtf"
	.byte 0x86, 0xb4	@ PACKER: left in the buffer by Tileset_Set04TilesB+0x15fa
	.global Tileset_Set04TilesD
Tileset_Set04TilesD:
	.incbin "games/tbs/src/graphics/tileset/set_04/tiles_d.4bpp.mtf"
	.byte 0x3c	@ PACKER: left in the buffer by Tileset_Set04TilesC+0x12c7
