	.include "games/tbs/src/map/map.inc"
	.section .rodata
	.global Field_VenusLighthouseThirdFloor
Field_VenusLighthouseThirdFloor:
	map_header 0, 0, 128, 128, 3, 1, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	map_offset Field_VenusLighthouseThirdFloor, .LField_VenusLighthouseThirdFloor_CELL_PICTURES
	map_offset Field_VenusLighthouseThirdFloor, .LField_VenusLighthouseThirdFloor_SHAPES
	map_offset Field_VenusLighthouseThirdFloor, .LField_VenusLighthouseThirdFloor_GRID
	map_offset Field_VenusLighthouseThirdFloor, .LField_VenusLighthouseThirdFloor_ANIMATION
	map_offset Field_VenusLighthouseThirdFloor, .LField_VenusLighthouseThirdFloor_BLEND
	map_offset Field_VenusLighthouseThirdFloor, .LField_VenusLighthouseThirdFloor_MARKERS
.LField_VenusLighthouseThirdFloor_CELL_PICTURES:
	.incbin "games/tbs/src/map/venus_lighthouse_third_floor/cell_pictures.delta2.lz"
	.balign 4, 0
.LField_VenusLighthouseThirdFloor_SHAPES:
	.incbin "games/tbs/src/map/venus_lighthouse_third_floor/shapes.table.lz"
	.balign 4, 0
.LField_VenusLighthouseThirdFloor_GRID:
	.incbin "games/tbs/src/map/venus_lighthouse_third_floor/map.grid.lz"
	.balign 4, 0
.LField_VenusLighthouseThirdFloor_ANIMATION:
	.incbin "games/tbs/src/map/venus_lighthouse_first_floor/tileset_anims.script.lz"
	.balign 4, 0
.LField_VenusLighthouseThirdFloor_BLEND:
	.incbin "games/tbs/src/map/venus_lighthouse_first_floor/blend.script.lz"
	.balign 4, 0
.LField_VenusLighthouseThirdFloor_MARKERS:
	treasure_spot 8, 22, 100
	treasure_spots_end
	.balign 4, 0
