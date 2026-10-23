	.include "games/tbs/src/layout/layout.inc"
	.section .rodata
	.global Field_Map032
Field_Map032:
	map_header 0, 0, 120, 120, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 120, 16, 16, 0, 0, 0xff, 0xff
	map_layer 120, 0, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map032, .LField_Map032_METATILES
	map_block Field_Map032, .LField_Map032_SHAPES
	map_block Field_Map032, .LField_Map032_GRID
	map_block Field_Map032, .LField_Map032_ANIMATION
	map_block Field_Map032, .LField_Map032_BLEND
	map_block Field_Map032, .LField_Map032_MARKERS
.LField_Map032_METATILES:
	.incbin "games/tbs/src/layout/layout_032/metatiles.delta1.lz"
	.balign 4, 0
.LField_Map032_SHAPES:
	.incbin "games/tbs/src/layout/layout_032/shapes.table.lz"
	.balign 4, 0
.LField_Map032_GRID:
	.incbin "games/tbs/src/layout/layout_032/grid.parts.lz"
	.balign 4, 0
.LField_Map032_ANIMATION:
	.incbin "games/tbs/src/layout/layout_032/animation.script.lz"
	.balign 4, 0
.LField_Map032_BLEND:
	.incbin "games/tbs/src/layout/layout_005/blend.script.lz"
	.balign 4, 0
.LField_Map032_MARKERS:
	.incbin "games/tbs/src/layout/layout_032/markers.table"
	.balign 4, 0
	.global Tileset_Set24Palette
Tileset_Set24Palette:
	.incbin "games/tbs/src/graphics/tileset/set_24/palette.gbapal.lz"
	.global Tileset_Set24TilesA
Tileset_Set24TilesA:
	.incbin "games/tbs/src/graphics/tileset/set_24/tiles_a.4bpp.mtf"
	.byte 0x07	@ PACKER: left in the buffer by Field_Map032+0x16fb
	.global Tileset_Set24TilesB
Tileset_Set24TilesB:
	.incbin "games/tbs/src/graphics/tileset/set_24/tiles_b.4bpp.mtf"
	.byte 0x00, 0x3f	@ PACKER: left in the buffer by Tileset_Set24TilesA+0x8fe
