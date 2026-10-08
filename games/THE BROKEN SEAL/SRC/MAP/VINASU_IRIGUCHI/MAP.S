	.include "games/COMMON/INCLUDE/GAME/ED_ASM.H"
	.include "games/THE BROKEN SEAL/SRC/MAP/MAP.INC"
	.section .rodata
	.global Field_VinasuIriguchi
Field_VinasuIriguchi:
	map_header 0, 0, 64, 68, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 64, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 68, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_VinasuIriguchi, .LField_VinasuIriguchi_METATILES
	map_block Field_VinasuIriguchi, .LField_VinasuIriguchi_SHAPES
	map_block Field_VinasuIriguchi, .LField_VinasuIriguchi_GRID
	map_block Field_VinasuIriguchi
	map_block Field_VinasuIriguchi
	map_block Field_VinasuIriguchi, .LField_VinasuIriguchi_MARKERS
.LField_VinasuIriguchi_METATILES:
	.incbin "MAP/VINASU_IRIGUCHI/METATILES.delta1.lz"
	.balign 4, 0
.LField_VinasuIriguchi_SHAPES:
	.incbin "MAP/VINASU_IRIGUCHI/SHAPES.table.lz"
	.balign 4, 0
.LField_VinasuIriguchi_GRID:
	.ifdef TBS_EDITION_FR
	.incbin "MAP/VINASU_IRIGUCHI/GRID_FR.parts.lz"
	.else
	.ifdef TBS_EDITION_ES
	.incbin "MAP/VINASU_IRIGUCHI/GRID_ES.parts.lz"
	.else
	.ifdef TBS_EDITION_IT
	.incbin "MAP/VINASU_IRIGUCHI/GRID_ES.parts.lz"
	.else
	.incbin "MAP/VINASU_IRIGUCHI/GRID.parts.lz"
	.endif
	.endif
	.endif
	.balign 4, 0
.LField_VinasuIriguchi_MARKERS:
	.incbin "MAP/MAP_000/MARKERS.table"
	.balign 4, 0
