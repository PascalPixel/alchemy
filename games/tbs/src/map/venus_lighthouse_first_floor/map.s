	.include "games/tbs/src/map/map.inc"
	.section .rodata
	.global Field_VenusLighthouseFirstFloor
Field_VenusLighthouseFirstFloor:
	.ifdef TBS_EDITION_ES
	map_header 0, 0, 128, 128, 3, 1, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	map_offset Field_VenusLighthouseFirstFloor, .LField_VenusLighthouseFirstFloor_CELL_PICTURES
	map_offset Field_VenusLighthouseFirstFloor, .LField_VenusLighthouseFirstFloor_SHAPES
	map_offset Field_VenusLighthouseFirstFloor, .LField_VenusLighthouseFirstFloor_GRID
	map_offset Field_VenusLighthouseFirstFloor, .LField_VenusLighthouseFirstFloor_ANIMATION
	map_offset Field_VenusLighthouseFirstFloor, .LField_VenusLighthouseFirstFloor_BLEND
	map_offset Field_VenusLighthouseFirstFloor, .LField_VenusLighthouseFirstFloor_MARKERS
.LField_VenusLighthouseFirstFloor_CELL_PICTURES:
	.incbin "games/tbs/src/map/venus_lighthouse_first_floor/cell_pictures.delta2.lz"
	.balign 4, 0
.LField_VenusLighthouseFirstFloor_SHAPES:
	.incbin "games/tbs/src/map/venus_lighthouse_first_floor/shapes.table.lz"
	.balign 4, 0
.LField_VenusLighthouseFirstFloor_GRID:
	.incbin "games/tbs/src/map/venus_lighthouse_first_floor/map_es.grid.lz"
	.balign 4, 0
.LField_VenusLighthouseFirstFloor_ANIMATION:
	.incbin "games/tbs/src/map/venus_lighthouse_first_floor/tileset_anims.script.lz"
	.balign 4, 0
.LField_VenusLighthouseFirstFloor_BLEND:
	.incbin "games/tbs/src/map/venus_lighthouse_first_floor/blend.script.lz"
	.balign 4, 0
.LField_VenusLighthouseFirstFloor_MARKERS:
	treasure_spot 26, 9, 100
	treasure_spot 11, 10, 110
	treasure_spots_end
	.else
	.ifdef TBS_EDITION_FR
	map_header 0, 0, 128, 128, 3, 1, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	map_offset Field_VenusLighthouseFirstFloor, .LField_VenusLighthouseFirstFloor_CELL_PICTURES
	map_offset Field_VenusLighthouseFirstFloor, .LField_VenusLighthouseFirstFloor_SHAPES
	map_offset Field_VenusLighthouseFirstFloor, .LField_VenusLighthouseFirstFloor_GRID
	map_offset Field_VenusLighthouseFirstFloor, .LField_VenusLighthouseFirstFloor_ANIMATION
	map_offset Field_VenusLighthouseFirstFloor, .LField_VenusLighthouseFirstFloor_BLEND
	map_offset Field_VenusLighthouseFirstFloor, .LField_VenusLighthouseFirstFloor_MARKERS
.LField_VenusLighthouseFirstFloor_CELL_PICTURES:
	.incbin "games/tbs/src/map/venus_lighthouse_first_floor/cell_pictures.delta2.lz"
	.balign 4, 0
.LField_VenusLighthouseFirstFloor_SHAPES:
	.incbin "games/tbs/src/map/venus_lighthouse_first_floor/shapes.table.lz"
	.balign 4, 0
.LField_VenusLighthouseFirstFloor_GRID:
	.incbin "games/tbs/src/map/venus_lighthouse_first_floor/map_es.grid.lz"
	.balign 4, 0
.LField_VenusLighthouseFirstFloor_ANIMATION:
	.incbin "games/tbs/src/map/venus_lighthouse_first_floor/tileset_anims.script.lz"
	.balign 4, 0
.LField_VenusLighthouseFirstFloor_BLEND:
	.incbin "games/tbs/src/map/venus_lighthouse_first_floor/blend.script.lz"
	.balign 4, 0
.LField_VenusLighthouseFirstFloor_MARKERS:
	treasure_spot 26, 9, 100
	treasure_spot 11, 10, 110
	treasure_spots_end
	.else
	.ifdef TBS_EDITION_IT
	map_header 0, 0, 128, 128, 3, 1, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	map_offset Field_VenusLighthouseFirstFloor, .LField_VenusLighthouseFirstFloor_CELL_PICTURES
	map_offset Field_VenusLighthouseFirstFloor, .LField_VenusLighthouseFirstFloor_SHAPES
	map_offset Field_VenusLighthouseFirstFloor, .LField_VenusLighthouseFirstFloor_GRID
	map_offset Field_VenusLighthouseFirstFloor, .LField_VenusLighthouseFirstFloor_ANIMATION
	map_offset Field_VenusLighthouseFirstFloor, .LField_VenusLighthouseFirstFloor_BLEND
	map_offset Field_VenusLighthouseFirstFloor, .LField_VenusLighthouseFirstFloor_MARKERS
.LField_VenusLighthouseFirstFloor_CELL_PICTURES:
	.incbin "games/tbs/src/map/venus_lighthouse_first_floor/cell_pictures.delta2.lz"
	.balign 4, 0
.LField_VenusLighthouseFirstFloor_SHAPES:
	.incbin "games/tbs/src/map/venus_lighthouse_first_floor/shapes.table.lz"
	.balign 4, 0
.LField_VenusLighthouseFirstFloor_GRID:
	.incbin "games/tbs/src/map/venus_lighthouse_first_floor/map_es.grid.lz"
	.balign 4, 0
.LField_VenusLighthouseFirstFloor_ANIMATION:
	.incbin "games/tbs/src/map/venus_lighthouse_first_floor/tileset_anims.script.lz"
	.balign 4, 0
.LField_VenusLighthouseFirstFloor_BLEND:
	.incbin "games/tbs/src/map/venus_lighthouse_first_floor/blend.script.lz"
	.balign 4, 0
.LField_VenusLighthouseFirstFloor_MARKERS:
	treasure_spot 26, 9, 100
	treasure_spot 11, 10, 110
	treasure_spots_end
	.else
	map_header 0, 0, 128, 128, 3, 1, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	map_offset Field_VenusLighthouseFirstFloor, .LField_VenusLighthouseFirstFloor_CELL_PICTURES
	map_offset Field_VenusLighthouseFirstFloor, .LField_VenusLighthouseFirstFloor_SHAPES
	map_offset Field_VenusLighthouseFirstFloor, .LField_VenusLighthouseFirstFloor_GRID
	map_offset Field_VenusLighthouseFirstFloor, .LField_VenusLighthouseFirstFloor_ANIMATION
	map_offset Field_VenusLighthouseFirstFloor, .LField_VenusLighthouseFirstFloor_BLEND
	map_offset Field_VenusLighthouseFirstFloor, .LField_VenusLighthouseFirstFloor_MARKERS
.LField_VenusLighthouseFirstFloor_CELL_PICTURES:
	.incbin "games/tbs/src/map/venus_lighthouse_first_floor/cell_pictures.delta2.lz"
	.balign 4, 0
.LField_VenusLighthouseFirstFloor_SHAPES:
	.incbin "games/tbs/src/map/venus_lighthouse_first_floor/shapes.table.lz"
	.balign 4, 0
.LField_VenusLighthouseFirstFloor_GRID:
	.incbin "games/tbs/src/map/venus_lighthouse_first_floor/map.grid.lz"
	.balign 4, 0
.LField_VenusLighthouseFirstFloor_ANIMATION:
	.incbin "games/tbs/src/map/venus_lighthouse_first_floor/tileset_anims.script.lz"
	.balign 4, 0
.LField_VenusLighthouseFirstFloor_BLEND:
	.incbin "games/tbs/src/map/venus_lighthouse_first_floor/blend.script.lz"
	.balign 4, 0
.LField_VenusLighthouseFirstFloor_MARKERS:
	treasure_spot 26, 9, 100
	treasure_spot 11, 10, 110
	treasure_spots_end
	.endif
	.endif
	.endif
	.balign 4, 0
	.global Tileset_PaletteVenusLighthouse
Tileset_PaletteVenusLighthouse:
	.incbin "games/tbs/src/graphics/tileset/palettes/venus_lighthouse.gbapal.lz"
	.byte 0xf0, 0x40, 0xef	@ PACKER: left in the buffer by Field_VenusLighthouseFirstFloor+0x159
