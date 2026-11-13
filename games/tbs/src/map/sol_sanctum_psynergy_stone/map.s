	.include "games/tbs/src/map/map.inc"
	.include "games/tbs/include/ed_asm.h"
	.section .rodata
	.global Field_SolSanctumPsynergyStone
Field_SolSanctumPsynergyStone:
	map_header 0, 0, 64, 32, 3, 2, 2, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 32, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 64, 16, 16, 0, 0, 0xff, 0xff
	map_offset Field_SolSanctumPsynergyStone, .LField_SolSanctumPsynergyStone_CELL_PICTURES
	map_offset Field_SolSanctumPsynergyStone, .LField_SolSanctumPsynergyStone_SHAPES
	map_offset Field_SolSanctumPsynergyStone, .LField_SolSanctumPsynergyStone_GRID
	map_offset Field_SolSanctumPsynergyStone, .LField_SolSanctumPsynergyStone_ANIMATION
	map_offset Field_SolSanctumPsynergyStone, .LField_SolSanctumPsynergyStone_BLEND
	map_offset Field_SolSanctumPsynergyStone, .LField_SolSanctumPsynergyStone_MARKERS
.LField_SolSanctumPsynergyStone_CELL_PICTURES:
	.incbin "games/tbs/src/map/sol_sanctum_psynergy_stone/cell_pictures.delta1.lz"
	.balign 4, 0
.LField_SolSanctumPsynergyStone_SHAPES:
	.incbin "games/tbs/src/map/map_001/shapes.table.lz"
	.balign 4, 0
.LField_SolSanctumPsynergyStone_GRID:
	.incbin "games/tbs/src/map/sol_sanctum_psynergy_stone/map.grid.lz"
	.balign 4, 0
.LField_SolSanctumPsynergyStone_ANIMATION:
	.incbin "games/tbs/src/map/sol_sanctum_psynergy_stone/tileset_anims.script.lz"
	.balign 4, 0
.LField_SolSanctumPsynergyStone_BLEND:
	.incbin "games/tbs/src/map/sol_sanctum_psynergy_stone/blend.script.lz"
	.balign 4, 0
.LField_SolSanctumPsynergyStone_MARKERS:
	treasure_spot 4, 5, 100
	treasure_spots_end
	.balign 4, 0
	.global Tileset_SolSanctumPsynergyStonePalette
Tileset_SolSanctumPsynergyStonePalette:
	.incbin "games/tbs/src/graphics/tileset/sol_sanctum_psynergy_stone/palette.gbapal.lz"
	.global Tileset_SolSanctumPsynergyStoneTilesA
Tileset_SolSanctumPsynergyStoneTilesA:
	.incbin "games/tbs/src/graphics/tileset/sol_sanctum_psynergy_stone/tiles_a.4bpp.mtf"
	.ifdef TBS_EDITION_JA
	.byte 0xf0, 0x4b	@ PACKER: left in the buffer by Tileset_SolSanctumStatueTilesA+0x1f7e
	.else
	.ifdef TBS_EDITION_EN
	.byte 0xf0, 0x4b	@ PACKER: left in the buffer by Tileset_SolSanctumStatueTilesA+0x1f7e
	.else
	.ifdef TBS_EDITION_DE
	.byte 0xf0, 0x4b	@ PACKER: left in the buffer by Tileset_SolSanctumStatueTilesA+0x1f7e
	.else
	.byte 0xc9, 0x36	@ PACKER: left in the buffer by Tileset_SolSanctumStatueTilesA+0x1f7e
	.endif
	.endif
	.endif
	.global Tileset_SolSanctumPsynergyStoneTilesB
Tileset_SolSanctumPsynergyStoneTilesB:
	.incbin "games/tbs/src/graphics/tileset/sol_sanctum_psynergy_stone/tiles_b.4bpp.mtf"
	.byte 0x7d, 0x04, 0xc1	@ PACKER: left in the buffer by Tileset_SolSanctumPsynergyStoneTilesA+0x945
	.global Tileset_SolSanctumPsynergyStoneTilesC
Tileset_SolSanctumPsynergyStoneTilesC:
	.incbin "games/tbs/src/graphics/tileset/sol_sanctum_psynergy_stone/tiles_c.4bpp.mtf"
	.byte 0x12, 0xa2	@ PACKER: left in the buffer by Tileset_SolSanctumPsynergyStoneTilesA+0xc4a
	.global Tileset_SolSanctumPsynergyStoneTilesD
Tileset_SolSanctumPsynergyStoneTilesD:
	.incbin "games/tbs/src/graphics/tileset/sol_sanctum_psynergy_stone/tiles_d.4bpp.mtf"
	.byte 0xe3, 0xb0, 0xd3	@ PACKER: left in the buffer by Tileset_SolSanctumPsynergyStoneTilesC+0x225
