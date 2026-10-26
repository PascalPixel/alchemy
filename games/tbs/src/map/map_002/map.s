	.include "games/tbs/src/map/map.inc"
	.section .rodata
	.global Field_Map002
Field_Map002:
	.ifdef TBS_EDITION_ES
	map_header 0, 0, 60, 188, 3, 2, 1, 1, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 60, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 120, 0, 16, 16, 0, 0, 0xff, 0xff
	map_offset Field_Map002, .LField_Map002_METATILES
	map_offset Field_Map002, .LField_Map002_SHAPES
	map_offset Field_Map002, .LField_Map002_GRID
	map_offset Field_Map002, .LField_Map002_ANIMATION
	map_offset Field_Map002
	map_offset Field_Map002, .LField_Map002_MARKERS
.LField_Map002_METATILES:
	.incbin "games/tbs/src/map/map_002/metatiles.delta1.lz"
	.balign 4, 0
.LField_Map002_SHAPES:
	.incbin "games/tbs/src/map/map_002/shapes.table.lz"
	.balign 4, 0
.LField_Map002_GRID:
	.incbin "games/tbs/src/map/map_002/map_es.grid.lz"
	.balign 4, 0
.LField_Map002_ANIMATION:
	.incbin "games/tbs/src/map/map_002/tileset_anims.script.lz"
	.balign 4, 0
.LField_Map002_MARKERS:
	treasure_spot 24, 55, 100
	treasure_spot 53, 56, 101
	treasure_spots_end
	.else
	.ifdef TBS_EDITION_FR
	map_header 0, 0, 60, 188, 3, 2, 1, 1, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 60, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 120, 0, 16, 16, 0, 0, 0xff, 0xff
	map_offset Field_Map002, .LField_Map002_METATILES
	map_offset Field_Map002, .LField_Map002_SHAPES
	map_offset Field_Map002, .LField_Map002_GRID
	map_offset Field_Map002, .LField_Map002_ANIMATION
	map_offset Field_Map002
	map_offset Field_Map002, .LField_Map002_MARKERS
.LField_Map002_METATILES:
	.incbin "games/tbs/src/map/map_002/metatiles.delta1.lz"
	.balign 4, 0
.LField_Map002_SHAPES:
	.incbin "games/tbs/src/map/map_002/shapes.table.lz"
	.balign 4, 0
.LField_Map002_GRID:
	.incbin "games/tbs/src/map/map_002/map_es.grid.lz"
	.balign 4, 0
.LField_Map002_ANIMATION:
	.incbin "games/tbs/src/map/map_002/tileset_anims.script.lz"
	.balign 4, 0
.LField_Map002_MARKERS:
	treasure_spot 24, 55, 100
	treasure_spot 53, 56, 101
	treasure_spots_end
	.else
	.ifdef TBS_EDITION_IT
	map_header 0, 0, 60, 188, 3, 2, 1, 1, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 60, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 120, 0, 16, 16, 0, 0, 0xff, 0xff
	map_offset Field_Map002, .LField_Map002_METATILES
	map_offset Field_Map002, .LField_Map002_SHAPES
	map_offset Field_Map002, .LField_Map002_GRID
	map_offset Field_Map002, .LField_Map002_ANIMATION
	map_offset Field_Map002
	map_offset Field_Map002, .LField_Map002_MARKERS
.LField_Map002_METATILES:
	.incbin "games/tbs/src/map/map_002/metatiles.delta1.lz"
	.balign 4, 0
.LField_Map002_SHAPES:
	.incbin "games/tbs/src/map/map_002/shapes.table.lz"
	.balign 4, 0
.LField_Map002_GRID:
	.incbin "games/tbs/src/map/map_002/map_it.grid.lz"
	.balign 4, 0
.LField_Map002_ANIMATION:
	.incbin "games/tbs/src/map/map_002/tileset_anims.script.lz"
	.balign 4, 0
.LField_Map002_MARKERS:
	treasure_spot 24, 55, 100
	treasure_spot 53, 56, 101
	treasure_spots_end
	.else
	map_header 0, 0, 60, 188, 3, 2, 1, 1, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 60, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 120, 0, 16, 16, 0, 0, 0xff, 0xff
	map_offset Field_Map002, .LField_Map002_METATILES
	map_offset Field_Map002, .LField_Map002_SHAPES
	map_offset Field_Map002, .LField_Map002_GRID
	map_offset Field_Map002, .LField_Map002_ANIMATION
	map_offset Field_Map002
	map_offset Field_Map002, .LField_Map002_MARKERS
.LField_Map002_METATILES:
	.incbin "games/tbs/src/map/map_002/metatiles.delta1.lz"
	.balign 4, 0
.LField_Map002_SHAPES:
	.incbin "games/tbs/src/map/map_002/shapes.table.lz"
	.balign 4, 0
.LField_Map002_GRID:
	.incbin "games/tbs/src/map/map_002/map.grid.lz"
	.balign 4, 0
.LField_Map002_ANIMATION:
	.incbin "games/tbs/src/map/map_002/tileset_anims.script.lz"
	.balign 4, 0
.LField_Map002_MARKERS:
	treasure_spot 24, 55, 100
	treasure_spot 53, 56, 101
	treasure_spots_end
	.endif
	.endif
	.endif
	.balign 4, 0
	.global Tileset_Set01Palette
Tileset_Set01Palette:
	.incbin "games/tbs/src/graphics/tileset/set_01/palette.gbapal.lz"
	.byte 0xb3	@ PACKER: left in the buffer by Field_Map002+0x193
	.global Tileset_Set01TilesA
Tileset_Set01TilesA:
	.incbin "games/tbs/src/graphics/tileset/set_01/tiles_a.4bpp.mtf"
	.byte 0x97	@ PACKER: left in the buffer by Field_Map002+0x2c53
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
