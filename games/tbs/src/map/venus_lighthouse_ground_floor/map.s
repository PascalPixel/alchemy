	.include "games/tbs/include/ed_asm.h"
	.include "games/tbs/src/map/map.inc"
	.section .rodata
	.global Field_VenusLighthouseGroundFloor
Field_VenusLighthouseGroundFloor:
	.ifeq EDITION_INTERNATIONAL
	map_header 0, 0, 128, 128, 3, 1, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	map_offset Field_VenusLighthouseGroundFloor, .LField_VenusLighthouseGroundFloor_CELL_PICTURES
	map_offset Field_VenusLighthouseGroundFloor, .LField_VenusLighthouseGroundFloor_SHAPES
	map_offset Field_VenusLighthouseGroundFloor, .LField_VenusLighthouseGroundFloor_GRID
	map_offset Field_VenusLighthouseGroundFloor, .LField_VenusLighthouseGroundFloor_ANIMATION
	map_offset Field_VenusLighthouseGroundFloor, .LField_VenusLighthouseGroundFloor_BLEND
	map_offset Field_VenusLighthouseGroundFloor, .LField_VenusLighthouseGroundFloor_MARKERS
.LField_VenusLighthouseGroundFloor_CELL_PICTURES:
	.incbin "games/tbs/src/map/venus_lighthouse_ground_floor/cell_pictures.delta1.lz"
	.balign 4, 0
.LField_VenusLighthouseGroundFloor_SHAPES:
	.incbin "games/tbs/src/map/venus_lighthouse_ground_floor/shapes.table.lz"
	.balign 4, 0
.LField_VenusLighthouseGroundFloor_GRID:
	.incbin "games/tbs/src/map/venus_lighthouse_ground_floor/map_ja.grid.lz"
	.balign 4, 0
.LField_VenusLighthouseGroundFloor_ANIMATION:
	.incbin "games/tbs/src/map/venus_lighthouse_ground_floor/tileset_anims.script.lz"
	.balign 4, 0
.LField_VenusLighthouseGroundFloor_BLEND:
	.incbin "games/tbs/src/map/mercury_lighthouse_entrance/blend.script.lz"
	.balign 4, 0
.LField_VenusLighthouseGroundFloor_MARKERS:
	treasure_spot 29, 12, 100
	treasure_spot 5, 55, 101
	treasure_spots_end
	.else
	map_header 0, 0, 128, 128, 3, 1, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	map_offset Field_VenusLighthouseGroundFloor, .LField_VenusLighthouseGroundFloor_CELL_PICTURES
	map_offset Field_VenusLighthouseGroundFloor, .LField_VenusLighthouseGroundFloor_SHAPES
	map_offset Field_VenusLighthouseGroundFloor, .LField_VenusLighthouseGroundFloor_GRID
	map_offset Field_VenusLighthouseGroundFloor, .LField_VenusLighthouseGroundFloor_ANIMATION
	map_offset Field_VenusLighthouseGroundFloor, .LField_VenusLighthouseGroundFloor_BLEND
	map_offset Field_VenusLighthouseGroundFloor, .LField_VenusLighthouseGroundFloor_MARKERS
.LField_VenusLighthouseGroundFloor_CELL_PICTURES:
	.incbin "games/tbs/src/map/venus_lighthouse_ground_floor/cell_pictures.delta1.lz"
	.balign 4, 0
.LField_VenusLighthouseGroundFloor_SHAPES:
	.incbin "games/tbs/src/map/venus_lighthouse_ground_floor/shapes.table.lz"
	.balign 4, 0
.LField_VenusLighthouseGroundFloor_GRID:
	.incbin "games/tbs/src/map/venus_lighthouse_ground_floor/map.grid.lz"
	.balign 4, 0
.LField_VenusLighthouseGroundFloor_ANIMATION:
	.incbin "games/tbs/src/map/venus_lighthouse_ground_floor/tileset_anims.script.lz"
	.balign 4, 0
.LField_VenusLighthouseGroundFloor_BLEND:
	.incbin "games/tbs/src/map/mercury_lighthouse_entrance/blend.script.lz"
	.balign 4, 0
.LField_VenusLighthouseGroundFloor_MARKERS:
	treasure_spot 29, 12, 100
	treasure_spot 5, 55, 101
	treasure_spots_end
	.endif
	.balign 4, 0
	.global Tileset_PaletteVenusLighthouseGroundFloor
Tileset_PaletteVenusLighthouseGroundFloor:
	.incbin "games/tbs/src/graphics/tileset/palettes/venus_lighthouse_ground_floor.gbapal.lz"
