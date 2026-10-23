	.include "games/tbs/src/layout/layout.inc"
	.section .rodata
	.global Field_Map003
Field_Map003:
	map_header 0, 0, 60, 114, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 60, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 120, 0, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map003, .LField_Map003_METATILES
	map_block Field_Map003, .LField_Map003_SHAPES
	map_block Field_Map003, .LField_Map003_GRID
	map_block Field_Map003, .LField_Map003_ANIMATION
	map_block Field_Map003
	map_block Field_Map003, .LField_Map003_MARKERS
.LField_Map003_METATILES:
	.incbin "games/tbs/src/layout/layout_003/metatiles.delta1.lz"
	.balign 4, 0
.LField_Map003_SHAPES:
	.incbin "games/tbs/src/layout/layout_003/shapes.table.lz"
	.balign 4, 0
.LField_Map003_GRID:
	.incbin "games/tbs/src/layout/layout_003/grid.parts.lz"
	.balign 4, 0
.LField_Map003_ANIMATION:
	.incbin "games/tbs/src/layout/layout_003/animation.script.lz"
	.balign 4, 0
.LField_Map003_MARKERS:
	.incbin "games/tbs/src/layout/layout_003/markers.table"
	.balign 4, 0
	.global Tileset_Set02Palette
Tileset_Set02Palette:
	.incbin "games/tbs/src/graphics/tileset/set_02/palette.gbapal.lz"
	.byte 0xc7	@ PACKER: left in the buffer by Field_Map003+0x17f
	.global Tileset_Set02TilesA
Tileset_Set02TilesA:
	.incbin "games/tbs/src/graphics/tileset/set_02/tiles_a.4bpp.mtf"
	.byte 0xfc, 0x89, 0x00	@ PACKER: left in the buffer by Field_Map003+0x27b9
	.global Tileset_Set02TilesB
Tileset_Set02TilesB:
	.incbin "games/tbs/src/graphics/tileset/set_02/tiles_b.4bpp.mtf"
	.byte 0xdc, 0x73	@ PACKER: left in the buffer by Tileset_Set02TilesA+0x2646
	.global Tileset_Set02TilesC
Tileset_Set02TilesC:
	.incbin "games/tbs/src/graphics/tileset/set_02/tiles_c.4bpp.mtf"
	.byte 0x5c, 0x40	@ PACKER: left in the buffer by Tileset_Set02TilesA+0x26e2
	.global Tileset_Set02TilesD
Tileset_Set02TilesD:
	.incbin "games/tbs/src/graphics/tileset/set_02/tiles_d.4bpp.mtf"
	.byte 0xf7	@ PACKER: left in the buffer by Tileset_Set02TilesC+0x1073
