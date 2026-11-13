	.include "games/tbs/src/map/map.inc"
	.section .rodata
	.global Field_VenusLighthouseSecondFloor
Field_VenusLighthouseSecondFloor:
	map_header 0, 0, 128, 128, 3, 1, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	map_offset Field_VenusLighthouseSecondFloor, .LField_VenusLighthouseSecondFloor_CELL_PICTURES
	map_offset Field_VenusLighthouseSecondFloor, .LField_VenusLighthouseSecondFloor_SHAPES
	map_offset Field_VenusLighthouseSecondFloor, .LField_VenusLighthouseSecondFloor_GRID
	map_offset Field_VenusLighthouseSecondFloor, .LField_VenusLighthouseSecondFloor_ANIMATION
	map_offset Field_VenusLighthouseSecondFloor, .LField_VenusLighthouseSecondFloor_BLEND
	map_offset Field_VenusLighthouseSecondFloor, .LField_VenusLighthouseSecondFloor_MARKERS
.LField_VenusLighthouseSecondFloor_CELL_PICTURES:
	.incbin "games/tbs/src/map/venus_lighthouse_second_floor/cell_pictures.delta2.lz"
	.balign 4, 0
.LField_VenusLighthouseSecondFloor_SHAPES:
	.incbin "games/tbs/src/map/venus_lighthouse_second_floor/shapes.table.lz"
	.balign 4, 0
.LField_VenusLighthouseSecondFloor_GRID:
	.incbin "games/tbs/src/map/venus_lighthouse_second_floor/map.grid.lz"
	.balign 4, 0
.LField_VenusLighthouseSecondFloor_ANIMATION:
	.incbin "games/tbs/src/map/venus_lighthouse_first_floor/tileset_anims.script.lz"
	.balign 4, 0
.LField_VenusLighthouseSecondFloor_BLEND:
	.incbin "games/tbs/src/map/venus_lighthouse_first_floor/blend.script.lz"
	.balign 4, 0
.LField_VenusLighthouseSecondFloor_MARKERS:
	treasure_spots_end
	.balign 4, 0
