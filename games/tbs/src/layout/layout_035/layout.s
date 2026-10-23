	.include "games/tbs/src/layout/layout.inc"
	.section .rodata
	.global Field_Map035
Field_Map035:
	.ifdef TBS_EDITION_ES
	map_header 0, 0, 64, 48, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 64, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 48, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map035, .LField_Map035_METATILES
	map_block Field_Map035, .LField_Map035_SHAPES
	map_block Field_Map035, .LField_Map035_GRID
	map_block Field_Map035
	map_block Field_Map035
	map_block Field_Map035, .LField_Map035_MARKERS
.LField_Map035_METATILES:
	.incbin "games/tbs/src/layout/layout_035/metatiles_es.delta1.lz"
	.balign 4, 0
.LField_Map035_SHAPES:
	.incbin "games/tbs/src/layout/layout_035/shapes.table.lz"
	.balign 4, 0
.LField_Map035_GRID:
	.incbin "games/tbs/src/layout/layout_035/grid_es.parts.lz"
	.balign 4, 0
.LField_Map035_MARKERS:
	.incbin "games/tbs/src/layout/layout_000/markers.table"
	.else
	.ifdef TBS_EDITION_FR
	map_header 0, 0, 64, 48, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 64, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 48, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map035, .LField_Map035_METATILES
	map_block Field_Map035, .LField_Map035_SHAPES
	map_block Field_Map035, .LField_Map035_GRID
	map_block Field_Map035
	map_block Field_Map035
	map_block Field_Map035, .LField_Map035_MARKERS
.LField_Map035_METATILES:
	.incbin "games/tbs/src/layout/layout_035/metatiles_es.delta1.lz"
	.balign 4, 0
.LField_Map035_SHAPES:
	.incbin "games/tbs/src/layout/layout_035/shapes.table.lz"
	.balign 4, 0
.LField_Map035_GRID:
	.incbin "games/tbs/src/layout/layout_035/grid_es.parts.lz"
	.balign 4, 0
.LField_Map035_MARKERS:
	.incbin "games/tbs/src/layout/layout_000/markers.table"
	.else
	.ifdef TBS_EDITION_IT
	map_header 0, 0, 64, 48, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 64, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 48, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map035, .LField_Map035_METATILES
	map_block Field_Map035, .LField_Map035_SHAPES
	map_block Field_Map035, .LField_Map035_GRID
	map_block Field_Map035
	map_block Field_Map035
	map_block Field_Map035, .LField_Map035_MARKERS
.LField_Map035_METATILES:
	.incbin "games/tbs/src/layout/layout_035/metatiles_es.delta1.lz"
	.balign 4, 0
.LField_Map035_SHAPES:
	.incbin "games/tbs/src/layout/layout_035/shapes.table.lz"
	.balign 4, 0
.LField_Map035_GRID:
	.incbin "games/tbs/src/layout/layout_035/grid_es.parts.lz"
	.balign 4, 0
.LField_Map035_MARKERS:
	.incbin "games/tbs/src/layout/layout_000/markers.table"
	.else
	map_header 0, 0, 64, 48, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 64, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 48, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map035, .LField_Map035_METATILES
	map_block Field_Map035, .LField_Map035_SHAPES
	map_block Field_Map035, .LField_Map035_GRID
	map_block Field_Map035
	map_block Field_Map035
	map_block Field_Map035, .LField_Map035_MARKERS
.LField_Map035_METATILES:
	.incbin "games/tbs/src/layout/layout_035/metatiles.delta1.lz"
	.balign 4, 0
.LField_Map035_SHAPES:
	.incbin "games/tbs/src/layout/layout_035/shapes.table.lz"
	.balign 4, 0
.LField_Map035_GRID:
	.incbin "games/tbs/src/layout/layout_035/grid.parts.lz"
	.balign 4, 0
.LField_Map035_MARKERS:
	.incbin "games/tbs/src/layout/layout_000/markers.table"
	.endif
	.endif
	.endif
	.balign 4, 0
	.global Tileset_Set27Palette
Tileset_Set27Palette:
	.incbin "games/tbs/src/graphics/tileset/set_27/palette.gbapal.lz"
	.byte 0x8c, 0xda, 0xd6	@ PACKER: left in the buffer by Field_Map035+0x1a9
	.global Tileset_Set27TilesA
Tileset_Set27TilesA:
	.incbin "games/tbs/src/graphics/tileset/set_27/tiles_a.4bpp.mtf"
	.byte 0x60, 0xcf, 0x83	@ PACKER: left in the buffer by Tileset_Set23TilesA+0x2d91
	.global Tileset_Set27TilesB
Tileset_Set27TilesB:
	.incbin "games/tbs/src/graphics/tileset/set_27/tiles_b.4bpp.mtf"
	.global Tileset_Set27TilesC
Tileset_Set27TilesC:
	.incbin "games/tbs/src/graphics/tileset/set_27/tiles_c.4bpp.mtf"
	.byte 0xb0, 0x3e	@ PACKER: left in the buffer by Tileset_Set27TilesB+0x197a
	.global Tileset_Set27TilesD
Tileset_Set27TilesD:
	.incbin "games/tbs/src/graphics/tileset/set_27/tiles_d.4bpp.mtf"
	.byte 0xa9	@ PACKER: left in the buffer by Tileset_Set27TilesC+0x13f
