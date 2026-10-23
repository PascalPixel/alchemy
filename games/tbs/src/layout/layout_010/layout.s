	.include "games/tbs/src/layout/layout.inc"
	.include "games/tbs/include/game/ed_asm.h"
	.section .rodata
	.global Field_Map010
Field_Map010:
	map_header 0, 0, 100, 64, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 64, 16, 16, 0, 0, 0xff, 0xff
	map_layer 100, 0, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map010, .LField_Map010_METATILES
	map_block Field_Map010, .LField_Map010_SHAPES
	map_block Field_Map010, .LField_Map010_GRID
	map_block Field_Map010, .LField_Map010_ANIMATION
	map_block Field_Map010, .LField_Map010_BLEND
	map_block Field_Map010, .LField_Map010_MARKERS
.LField_Map010_METATILES:
	.incbin "games/tbs/src/layout/layout_010/metatiles.delta1.lz"
	.balign 4, 0
.LField_Map010_SHAPES:
	.incbin "games/tbs/src/layout/layout_010/shapes.table.lz"
	.balign 4, 0
.LField_Map010_GRID:
	.incbin "games/tbs/src/layout/layout_010/grid.parts.lz"
	.balign 4, 0
.LField_Map010_ANIMATION:
	.incbin "games/tbs/src/layout/layout_010/animation.script.lz"
	.balign 4, 0
.LField_Map010_BLEND:
	.incbin "games/tbs/src/layout/layout_010/blend.script.lz"
	.balign 4, 0
.LField_Map010_MARKERS:
	.incbin "games/tbs/src/layout/layout_000/markers.table"
	.balign 4, 0
	.global Tileset_Set09Palette
Tileset_Set09Palette:
	.ifdef TBS_EDITION_ES
	.incbin "games/tbs/src/graphics/tileset/set_09/palette_es.gbapal.lz"
	.else
	.ifdef TBS_EDITION_FR
	.incbin "games/tbs/src/graphics/tileset/set_09/palette_es.gbapal.lz"
	.else
	.ifdef TBS_EDITION_IT
	.incbin "games/tbs/src/graphics/tileset/set_09/palette_es.gbapal.lz"
	.else
	.incbin "games/tbs/src/graphics/tileset/set_09/palette.gbapal.lz"
	.endif
	.endif
	.endif
	.byte 0xd4	@ PACKER: left in the buffer by Field_Map010+0x133
	.global Tileset_Set09TilesA
Tileset_Set09TilesA:
	.ifdef TBS_EDITION_ES
	.incbin "games/tbs/src/graphics/tileset/set_09/tiles_a_es.4bpp.mtf"
	.else
	.ifdef TBS_EDITION_FR
	.incbin "games/tbs/src/graphics/tileset/set_09/tiles_a_es.4bpp.mtf"
	.else
	.ifdef TBS_EDITION_IT
	.incbin "games/tbs/src/graphics/tileset/set_09/tiles_a_es.4bpp.mtf"
	.else
	.incbin "games/tbs/src/graphics/tileset/set_09/tiles_a.4bpp.mtf"
	.endif
	.endif
	.endif
	.ifdef TBS_EDITION_ES
	.byte 0x89, 0x99	@ PACKER: left in the buffer by Tileset_Set08TilesA+0x2482
	.else
	.ifdef TBS_EDITION_FR
	.byte 0x89, 0x99	@ PACKER: left in the buffer by Tileset_Set08TilesA+0x2482
	.else
	.ifdef TBS_EDITION_IT
	.byte 0x89, 0x99	@ PACKER: left in the buffer by Tileset_Set08TilesA+0x2482
	.else
	.byte 0x06	@ PACKER: left in the buffer by Tileset_Set08TilesA+0x247f
	.endif
	.endif
	.endif
	.global Tileset_Set09TilesB
Tileset_Set09TilesB:
	.incbin "games/tbs/src/graphics/tileset/set_09/tiles_b.4bpp.mtf"
	.ifdef TBS_EDITION_JA
	.byte 0xee	@ PACKER: left in the buffer by Tileset_Set09TilesA+0xd7b
	.else
	.ifdef TBS_EDITION_EN
	.byte 0xee	@ PACKER: left in the buffer by Tileset_Set09TilesA+0xd7b
	.else
	.ifdef TBS_EDITION_DE
	.byte 0xee	@ PACKER: left in the buffer by Tileset_Set09TilesA+0xd7b
	.else
	.byte 0x86	@ PACKER: left in the buffer by Tileset_Set09TilesA+0xd7b
	.endif
	.endif
	.endif
	.global Tileset_Set09TilesC
Tileset_Set09TilesC:
	.incbin "games/tbs/src/graphics/tileset/set_09/tiles_c.4bpp.mtf"
	.byte 0x1c, 0x40	@ PACKER: left in the buffer by Tileset_Set09TilesB+0xade
