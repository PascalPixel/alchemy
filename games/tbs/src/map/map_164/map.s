	.include "games/tbs/src/map/map.inc"
	.section .rodata
	.global Field_Map164
Field_Map164:
	.ifdef TBS_EDITION_ES
	map_header 0, 0, 128, 128, 3, 1, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	map_offset Field_Map164, .LField_Map164_METATILES
	map_offset Field_Map164, .LField_Map164_SHAPES
	map_offset Field_Map164, .LField_Map164_GRID
	map_offset Field_Map164, .LField_Map164_ANIMATION
	map_offset Field_Map164, .LField_Map164_BLEND
	map_offset Field_Map164, .LField_Map164_MARKERS
.LField_Map164_METATILES:
	.incbin "games/tbs/src/map/map_164/metatiles.delta2.lz"
	.balign 4, 0
.LField_Map164_SHAPES:
	.incbin "games/tbs/src/map/map_164/shapes.table.lz"
	.balign 4, 0
.LField_Map164_GRID:
	.incbin "games/tbs/src/map/map_164/map_es.grid.lz"
	.balign 4, 0
.LField_Map164_ANIMATION:
	.incbin "games/tbs/src/map/map_164/tileset_anims.script.lz"
	.balign 4, 0
.LField_Map164_BLEND:
	.incbin "games/tbs/src/map/map_164/blend.script.lz"
	.balign 4, 0
.LField_Map164_MARKERS:
	treasure_spot 26, 9, 100
	treasure_spot 11, 10, 110
	treasure_spots_end
	.else
	.ifdef TBS_EDITION_FR
	map_header 0, 0, 128, 128, 3, 1, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	map_offset Field_Map164, .LField_Map164_METATILES
	map_offset Field_Map164, .LField_Map164_SHAPES
	map_offset Field_Map164, .LField_Map164_GRID
	map_offset Field_Map164, .LField_Map164_ANIMATION
	map_offset Field_Map164, .LField_Map164_BLEND
	map_offset Field_Map164, .LField_Map164_MARKERS
.LField_Map164_METATILES:
	.incbin "games/tbs/src/map/map_164/metatiles.delta2.lz"
	.balign 4, 0
.LField_Map164_SHAPES:
	.incbin "games/tbs/src/map/map_164/shapes.table.lz"
	.balign 4, 0
.LField_Map164_GRID:
	.incbin "games/tbs/src/map/map_164/map_es.grid.lz"
	.balign 4, 0
.LField_Map164_ANIMATION:
	.incbin "games/tbs/src/map/map_164/tileset_anims.script.lz"
	.balign 4, 0
.LField_Map164_BLEND:
	.incbin "games/tbs/src/map/map_164/blend.script.lz"
	.balign 4, 0
.LField_Map164_MARKERS:
	treasure_spot 26, 9, 100
	treasure_spot 11, 10, 110
	treasure_spots_end
	.else
	.ifdef TBS_EDITION_IT
	map_header 0, 0, 128, 128, 3, 1, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	map_offset Field_Map164, .LField_Map164_METATILES
	map_offset Field_Map164, .LField_Map164_SHAPES
	map_offset Field_Map164, .LField_Map164_GRID
	map_offset Field_Map164, .LField_Map164_ANIMATION
	map_offset Field_Map164, .LField_Map164_BLEND
	map_offset Field_Map164, .LField_Map164_MARKERS
.LField_Map164_METATILES:
	.incbin "games/tbs/src/map/map_164/metatiles.delta2.lz"
	.balign 4, 0
.LField_Map164_SHAPES:
	.incbin "games/tbs/src/map/map_164/shapes.table.lz"
	.balign 4, 0
.LField_Map164_GRID:
	.incbin "games/tbs/src/map/map_164/map_es.grid.lz"
	.balign 4, 0
.LField_Map164_ANIMATION:
	.incbin "games/tbs/src/map/map_164/tileset_anims.script.lz"
	.balign 4, 0
.LField_Map164_BLEND:
	.incbin "games/tbs/src/map/map_164/blend.script.lz"
	.balign 4, 0
.LField_Map164_MARKERS:
	treasure_spot 26, 9, 100
	treasure_spot 11, 10, 110
	treasure_spots_end
	.else
	map_header 0, 0, 128, 128, 3, 1, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	map_offset Field_Map164, .LField_Map164_METATILES
	map_offset Field_Map164, .LField_Map164_SHAPES
	map_offset Field_Map164, .LField_Map164_GRID
	map_offset Field_Map164, .LField_Map164_ANIMATION
	map_offset Field_Map164, .LField_Map164_BLEND
	map_offset Field_Map164, .LField_Map164_MARKERS
.LField_Map164_METATILES:
	.incbin "games/tbs/src/map/map_164/metatiles.delta2.lz"
	.balign 4, 0
.LField_Map164_SHAPES:
	.incbin "games/tbs/src/map/map_164/shapes.table.lz"
	.balign 4, 0
.LField_Map164_GRID:
	.incbin "games/tbs/src/map/map_164/map.grid.lz"
	.balign 4, 0
.LField_Map164_ANIMATION:
	.incbin "games/tbs/src/map/map_164/tileset_anims.script.lz"
	.balign 4, 0
.LField_Map164_BLEND:
	.incbin "games/tbs/src/map/map_164/blend.script.lz"
	.balign 4, 0
.LField_Map164_MARKERS:
	treasure_spot 26, 9, 100
	treasure_spot 11, 10, 110
	treasure_spots_end
	.endif
	.endif
	.endif
	.balign 4, 0
	.global Tileset_Palette51
Tileset_Palette51:
	.incbin "games/tbs/src/graphics/tileset/palettes/pal_51.gbapal.lz"
	.byte 0xf0, 0x40, 0xef	@ PACKER: left in the buffer by Field_Map164+0x159
