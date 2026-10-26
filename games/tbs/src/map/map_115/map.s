	.include "games/tbs/src/map/map.inc"
	.section .rodata
	.global Field_Map115
Field_Map115:
	.ifdef TBS_EDITION_ES
	map_header 0, 0, 80, 94, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 96, 16, 16, 0, 0, 0xff, 0xff
	map_layer 80, 0, 16, 16, 0, 0, 0xff, 0xff
	map_offset Field_Map115, .LField_Map115_METATILES
	map_offset Field_Map115, .LField_Map115_SHAPES
	map_offset Field_Map115, .LField_Map115_GRID
	map_offset Field_Map115, .LField_Map115_ANIMATION
	map_offset Field_Map115, .LField_Map115_BLEND
	map_offset Field_Map115, .LField_Map115_MARKERS
.LField_Map115_METATILES:
	.incbin "games/tbs/src/map/map_115/metatiles.delta2.lz"
	.balign 4, 0
.LField_Map115_SHAPES:
	.incbin "games/tbs/src/map/map_115/shapes.table.lz"
	.balign 4, 0
.LField_Map115_GRID:
	.incbin "games/tbs/src/map/map_115/map_es.grid.lz"
	.balign 4, 0
.LField_Map115_ANIMATION:
	.incbin "games/tbs/src/map/map_033/tileset_anims.script.lz"
	.balign 4, 0
.LField_Map115_BLEND:
	.incbin "games/tbs/src/map/map_033/blend.script.lz"
	.balign 4, 0
.LField_Map115_MARKERS:
	treasure_spot 12, 3, 102
	treasure_spot 26, 4, 103
	treasure_spot 18, 14, 101
	treasure_spot 28, 15, 100
	treasure_spots_end
	.else
	.ifdef TBS_EDITION_FR
	map_header 0, 0, 80, 94, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 96, 16, 16, 0, 0, 0xff, 0xff
	map_layer 80, 0, 16, 16, 0, 0, 0xff, 0xff
	map_offset Field_Map115, .LField_Map115_METATILES
	map_offset Field_Map115, .LField_Map115_SHAPES
	map_offset Field_Map115, .LField_Map115_GRID
	map_offset Field_Map115, .LField_Map115_ANIMATION
	map_offset Field_Map115, .LField_Map115_BLEND
	map_offset Field_Map115, .LField_Map115_MARKERS
.LField_Map115_METATILES:
	.incbin "games/tbs/src/map/map_115/metatiles.delta2.lz"
	.balign 4, 0
.LField_Map115_SHAPES:
	.incbin "games/tbs/src/map/map_115/shapes.table.lz"
	.balign 4, 0
.LField_Map115_GRID:
	.incbin "games/tbs/src/map/map_115/map_es.grid.lz"
	.balign 4, 0
.LField_Map115_ANIMATION:
	.incbin "games/tbs/src/map/map_033/tileset_anims.script.lz"
	.balign 4, 0
.LField_Map115_BLEND:
	.incbin "games/tbs/src/map/map_033/blend.script.lz"
	.balign 4, 0
.LField_Map115_MARKERS:
	treasure_spot 12, 3, 102
	treasure_spot 26, 4, 103
	treasure_spot 18, 14, 101
	treasure_spot 28, 15, 100
	treasure_spots_end
	.else
	.ifdef TBS_EDITION_IT
	map_header 0, 0, 80, 94, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 96, 16, 16, 0, 0, 0xff, 0xff
	map_layer 80, 0, 16, 16, 0, 0, 0xff, 0xff
	map_offset Field_Map115, .LField_Map115_METATILES
	map_offset Field_Map115, .LField_Map115_SHAPES
	map_offset Field_Map115, .LField_Map115_GRID
	map_offset Field_Map115, .LField_Map115_ANIMATION
	map_offset Field_Map115, .LField_Map115_BLEND
	map_offset Field_Map115, .LField_Map115_MARKERS
.LField_Map115_METATILES:
	.incbin "games/tbs/src/map/map_115/metatiles.delta2.lz"
	.balign 4, 0
.LField_Map115_SHAPES:
	.incbin "games/tbs/src/map/map_115/shapes.table.lz"
	.balign 4, 0
.LField_Map115_GRID:
	.incbin "games/tbs/src/map/map_115/map_es.grid.lz"
	.balign 4, 0
.LField_Map115_ANIMATION:
	.incbin "games/tbs/src/map/map_033/tileset_anims.script.lz"
	.balign 4, 0
.LField_Map115_BLEND:
	.incbin "games/tbs/src/map/map_033/blend.script.lz"
	.balign 4, 0
.LField_Map115_MARKERS:
	treasure_spot 12, 3, 102
	treasure_spot 26, 4, 103
	treasure_spot 18, 14, 101
	treasure_spot 28, 15, 100
	treasure_spots_end
	.else
	map_header 0, 0, 80, 94, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 96, 16, 16, 0, 0, 0xff, 0xff
	map_layer 80, 0, 16, 16, 0, 0, 0xff, 0xff
	map_offset Field_Map115, .LField_Map115_METATILES
	map_offset Field_Map115, .LField_Map115_SHAPES
	map_offset Field_Map115, .LField_Map115_GRID
	map_offset Field_Map115, .LField_Map115_ANIMATION
	map_offset Field_Map115, .LField_Map115_BLEND
	map_offset Field_Map115, .LField_Map115_MARKERS
.LField_Map115_METATILES:
	.incbin "games/tbs/src/map/map_115/metatiles.delta2.lz"
	.balign 4, 0
.LField_Map115_SHAPES:
	.incbin "games/tbs/src/map/map_115/shapes.table.lz"
	.balign 4, 0
.LField_Map115_GRID:
	.incbin "games/tbs/src/map/map_115/map.grid.lz"
	.balign 4, 0
.LField_Map115_ANIMATION:
	.incbin "games/tbs/src/map/map_033/tileset_anims.script.lz"
	.balign 4, 0
.LField_Map115_BLEND:
	.incbin "games/tbs/src/map/map_033/blend.script.lz"
	.balign 4, 0
.LField_Map115_MARKERS:
	treasure_spot 12, 3, 102
	treasure_spot 26, 4, 103
	treasure_spot 18, 14, 101
	treasure_spot 28, 15, 100
	treasure_spots_end
	.endif
	.endif
	.endif
	.balign 4, 0
	.global Tileset_Palette21
Tileset_Palette21:
	.incbin "games/tbs/src/graphics/tileset/palettes/pal_21.gbapal.lz"
	.byte 0xfe, 0xa6	@ PACKER: left in the buffer by Field_Map115+0x1ba
