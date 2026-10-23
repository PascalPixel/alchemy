	.include "games/tbs/src/layout/layout.inc"
	.section .rodata
	.global Field_Map150
Field_Map150:
	map_header 0, 0, 48, 48, 2, 1, 3, 1, 1, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map150, .LField_Map150_METATILES
	map_block Field_Map150, .LField_Map150_SHAPES
	map_block Field_Map150, .LField_Map150_GRID
	map_block Field_Map150, .LField_Map150_ANIMATION
	map_block Field_Map150
	map_block Field_Map150, .LField_Map150_MARKERS
.LField_Map150_METATILES:
	.incbin "games/tbs/src/layout/layout_150/metatiles.delta1.lz"
	.balign 4, 0
.LField_Map150_SHAPES:
	.incbin "games/tbs/src/layout/layout_150/shapes.table.lz"
	.balign 4, 0
.LField_Map150_GRID:
	.incbin "games/tbs/src/layout/layout_150/grid.parts.lz"
	.balign 4, 0
.LField_Map150_ANIMATION:
	.incbin "games/tbs/src/layout/layout_150/animation.script.lz"
	.balign 4, 0
.LField_Map150_MARKERS:
	.incbin "games/tbs/src/layout/layout_150/markers.table"
	.global Tileset_Set64Palette
Tileset_Set64Palette:
	.incbin "games/tbs/src/graphics/tileset/set_64/palette.gbapal.lz"
	.global Tileset_Set64TilesA
Tileset_Set64TilesA:
	.incbin "games/tbs/src/graphics/tileset/set_64/tiles_a.4bpp.mtf"
	.ifdef TBS_EDITION_JA
	.byte 0xd8	@ PACKER: left in the buffer by Field_Map148+0x2617
	.else
	.ifdef TBS_EDITION_DE
	.byte 0xd8	@ PACKER: left in the buffer by Field_Map148+0x2617
	.else
	.byte 0x80	@ PACKER: left in the buffer by Field_Map148+0x2617
	.endif
	.endif
	.global Tileset_Set64TilesB
Tileset_Set64TilesB:
	.incbin "games/tbs/src/graphics/tileset/set_64/tiles_b.4bpp.mtf"
	.global Tileset_Set64TilesC
Tileset_Set64TilesC:
	.incbin "games/tbs/src/graphics/tileset/set_64/tiles_c.4bpp.mtf"
	.byte 0x00, 0x14	@ PACKER: left in the buffer by Tileset_Set64TilesB+0x106a
	.global Tileset_Set64TilesD
Tileset_Set64TilesD:
	.incbin "games/tbs/src/graphics/tileset/set_64/tiles_d.4bpp.mtf"
	.byte 0x81, 0x79	@ PACKER: left in the buffer by Tileset_Set64TilesB+0x1a26
