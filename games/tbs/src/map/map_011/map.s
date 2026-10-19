	.include "games/tbs/src/map/map.inc"
	.include "games/common/include/game/ed_asm.h"
	.section .rodata
	.global Field_Map011
Field_Map011:
	map_header 0, 0, 64, 32, 3, 2, 2, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 32, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 64, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map011, .LField_Map011_METATILES
	map_block Field_Map011, .LField_Map011_SHAPES
	map_block Field_Map011, .LField_Map011_GRID
	map_block Field_Map011, .LField_Map011_ANIMATION
	map_block Field_Map011, .LField_Map011_BLEND
	map_block Field_Map011, .LField_Map011_MARKERS
.LField_Map011_METATILES:
	.incbin "games/tbs/src/map/map_011/metatiles.delta1.lz"
	.balign 4, 0
.LField_Map011_SHAPES:
	.incbin "games/tbs/src/map/map_001/shapes.table.lz"
	.balign 4, 0
.LField_Map011_GRID:
	.incbin "games/tbs/src/map/map_011/grid.parts.lz"
	.balign 4, 0
.LField_Map011_ANIMATION:
	.incbin "games/tbs/src/map/map_011/animation.script.lz"
	.balign 4, 0
.LField_Map011_BLEND:
	.incbin "games/tbs/src/map/map_011/blend.script.lz"
	.balign 4, 0
.LField_Map011_MARKERS:
	.incbin "games/tbs/src/map/map_011/markers.table"
	.balign 4, 0
	.global Tileset_Set10Palette
Tileset_Set10Palette:
	.incbin "games/tbs/src/graphics/tileset/set_10/palette.gbapal.lz"
	.global Tileset_Set10TilesA
Tileset_Set10TilesA:
	.incbin "games/tbs/src/graphics/tileset/set_10/tiles_a.4bpp.mtf"
	.ifdef TBS_EDITION_JA
	.byte 0xf0, 0x4b	@ PACKER: left in the buffer by Tileset_Set09TilesA+0x1f7e
	.else
	.ifdef TBS_EDITION_EN
	.byte 0xf0, 0x4b	@ PACKER: left in the buffer by Tileset_Set09TilesA+0x1f7e
	.else
	.ifdef TBS_EDITION_DE
	.byte 0xf0, 0x4b	@ PACKER: left in the buffer by Tileset_Set09TilesA+0x1f7e
	.else
	.byte 0xc9, 0x36	@ PACKER: left in the buffer by Tileset_Set09TilesA+0x1f7e
	.endif
	.endif
	.endif
	.global Tileset_Set10TilesB
Tileset_Set10TilesB:
	.incbin "games/tbs/src/graphics/tileset/set_10/tiles_b.4bpp.mtf"
	.byte 0x7d, 0x04, 0xc1	@ PACKER: left in the buffer by Tileset_Set10TilesA+0x945
	.global Tileset_Set10TilesC
Tileset_Set10TilesC:
	.incbin "games/tbs/src/graphics/tileset/set_10/tiles_c.4bpp.mtf"
	.byte 0x12, 0xa2	@ PACKER: left in the buffer by Tileset_Set10TilesA+0xc4a
	.global Tileset_Set10TilesD
Tileset_Set10TilesD:
	.incbin "games/tbs/src/graphics/tileset/set_10/tiles_d.4bpp.mtf"
	.byte 0xe3, 0xb0, 0xd3	@ PACKER: left in the buffer by Tileset_Set10TilesC+0x225
