	.include "games/tbs/src/layout/layout.inc"
	.section .rodata
	.global Field_Layout002
Field_Layout002:
	.ifdef TBS_EDITION_ES
	layout_header 0, 0, 60, 188, 3, 2, 1, 1, 2, 2, 0, 0
	layout_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	layout_layer 60, 0, 16, 16, 0, 0, 0xff, 0xff
	layout_layer 120, 0, 16, 16, 0, 0, 0xff, 0xff
	layout_offset Field_Layout002, .LField_Layout002_METATILES
	layout_offset Field_Layout002, .LField_Layout002_SHAPES
	layout_offset Field_Layout002, .LField_Layout002_GRID
	layout_offset Field_Layout002, .LField_Layout002_ANIMATION
	layout_offset Field_Layout002
	layout_offset Field_Layout002, .LField_Layout002_MARKERS
.LField_Layout002_METATILES:
	.incbin "games/tbs/src/layout/layout_002/metatiles.delta1.lz"
	.balign 4, 0
.LField_Layout002_SHAPES:
	.incbin "games/tbs/src/layout/layout_002/shapes.table.lz"
	.balign 4, 0
.LField_Layout002_GRID:
	.incbin "games/tbs/src/layout/layout_002/grid_es.parts.lz"
	.balign 4, 0
.LField_Layout002_ANIMATION:
	.incbin "games/tbs/src/layout/layout_002/tileset_anims.script.lz"
	.balign 4, 0
.LField_Layout002_MARKERS:
	.incbin "games/tbs/src/layout/layout_002/treasure_spots_es.table"
	.else
	.ifdef TBS_EDITION_FR
	layout_header 0, 0, 60, 188, 3, 2, 1, 1, 2, 2, 0, 0
	layout_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	layout_layer 60, 0, 16, 16, 0, 0, 0xff, 0xff
	layout_layer 120, 0, 16, 16, 0, 0, 0xff, 0xff
	layout_offset Field_Layout002, .LField_Layout002_METATILES
	layout_offset Field_Layout002, .LField_Layout002_SHAPES
	layout_offset Field_Layout002, .LField_Layout002_GRID
	layout_offset Field_Layout002, .LField_Layout002_ANIMATION
	layout_offset Field_Layout002
	layout_offset Field_Layout002, .LField_Layout002_MARKERS
.LField_Layout002_METATILES:
	.incbin "games/tbs/src/layout/layout_002/metatiles.delta1.lz"
	.balign 4, 0
.LField_Layout002_SHAPES:
	.incbin "games/tbs/src/layout/layout_002/shapes.table.lz"
	.balign 4, 0
.LField_Layout002_GRID:
	.incbin "games/tbs/src/layout/layout_002/grid_es.parts.lz"
	.balign 4, 0
.LField_Layout002_ANIMATION:
	.incbin "games/tbs/src/layout/layout_002/tileset_anims.script.lz"
	.balign 4, 0
.LField_Layout002_MARKERS:
	.incbin "games/tbs/src/layout/layout_002/treasure_spots_es.table"
	.else
	.ifdef TBS_EDITION_IT
	layout_header 0, 0, 60, 188, 3, 2, 1, 1, 2, 2, 0, 0
	layout_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	layout_layer 60, 0, 16, 16, 0, 0, 0xff, 0xff
	layout_layer 120, 0, 16, 16, 0, 0, 0xff, 0xff
	layout_offset Field_Layout002, .LField_Layout002_METATILES
	layout_offset Field_Layout002, .LField_Layout002_SHAPES
	layout_offset Field_Layout002, .LField_Layout002_GRID
	layout_offset Field_Layout002, .LField_Layout002_ANIMATION
	layout_offset Field_Layout002
	layout_offset Field_Layout002, .LField_Layout002_MARKERS
.LField_Layout002_METATILES:
	.incbin "games/tbs/src/layout/layout_002/metatiles.delta1.lz"
	.balign 4, 0
.LField_Layout002_SHAPES:
	.incbin "games/tbs/src/layout/layout_002/shapes.table.lz"
	.balign 4, 0
.LField_Layout002_GRID:
	.incbin "games/tbs/src/layout/layout_002/grid_it.parts.lz"
	.balign 4, 0
.LField_Layout002_ANIMATION:
	.incbin "games/tbs/src/layout/layout_002/tileset_anims.script.lz"
	.balign 4, 0
.LField_Layout002_MARKERS:
	.incbin "games/tbs/src/layout/layout_002/treasure_spots_es.table"
	.else
	layout_header 0, 0, 60, 188, 3, 2, 1, 1, 2, 2, 0, 0
	layout_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	layout_layer 60, 0, 16, 16, 0, 0, 0xff, 0xff
	layout_layer 120, 0, 16, 16, 0, 0, 0xff, 0xff
	layout_offset Field_Layout002, .LField_Layout002_METATILES
	layout_offset Field_Layout002, .LField_Layout002_SHAPES
	layout_offset Field_Layout002, .LField_Layout002_GRID
	layout_offset Field_Layout002, .LField_Layout002_ANIMATION
	layout_offset Field_Layout002
	layout_offset Field_Layout002, .LField_Layout002_MARKERS
.LField_Layout002_METATILES:
	.incbin "games/tbs/src/layout/layout_002/metatiles.delta1.lz"
	.balign 4, 0
.LField_Layout002_SHAPES:
	.incbin "games/tbs/src/layout/layout_002/shapes.table.lz"
	.balign 4, 0
.LField_Layout002_GRID:
	.incbin "games/tbs/src/layout/layout_002/grid.parts.lz"
	.balign 4, 0
.LField_Layout002_ANIMATION:
	.incbin "games/tbs/src/layout/layout_002/tileset_anims.script.lz"
	.balign 4, 0
.LField_Layout002_MARKERS:
	.incbin "games/tbs/src/layout/layout_002/treasure_spots.table"
	.endif
	.endif
	.endif
	.balign 4, 0
	.global Tileset_Set01Palette
Tileset_Set01Palette:
	.incbin "games/tbs/src/graphics/tileset/set_01/palette.gbapal.lz"
	.byte 0xb3	@ PACKER: left in the buffer by Field_Layout002+0x193
	.global Tileset_Set01TilesA
Tileset_Set01TilesA:
	.incbin "games/tbs/src/graphics/tileset/set_01/tiles_a.4bpp.mtf"
	.byte 0x97	@ PACKER: left in the buffer by Field_Layout002+0x2c53
	.global Tileset_Set01TilesB
Tileset_Set01TilesB:
	.incbin "games/tbs/src/graphics/tileset/set_01/tiles_b.4bpp.mtf"
	.global Tileset_Set01TilesC
Tileset_Set01TilesC:
	.incbin "games/tbs/src/graphics/tileset/set_01/tiles_c.4bpp.mtf"
	.global Tileset_Set01TilesD
Tileset_Set01TilesD:
	.incbin "games/tbs/src/graphics/tileset/set_01/tiles_d.4bpp.mtf"
	.byte 0x8f	@ PACKER: left in the buffer by Tileset_Set01TilesC+0x1ecb
