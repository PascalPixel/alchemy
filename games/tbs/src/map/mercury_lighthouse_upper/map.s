	.include "games/tbs/src/map/map.inc"
	.section .rodata
	.global Field_MercuryLighthouseUpper
Field_MercuryLighthouseUpper:
	map_header 0, 0, 128, 128, 3, 1, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	map_offset Field_MercuryLighthouseUpper, .LField_MercuryLighthouseUpper_CELL_PICTURES
	map_offset Field_MercuryLighthouseUpper, .LField_MercuryLighthouseUpper_SHAPES
	map_offset Field_MercuryLighthouseUpper, .LField_MercuryLighthouseUpper_GRID
	map_offset Field_MercuryLighthouseUpper, .LField_MercuryLighthouseUpper_ANIMATION
	map_offset Field_MercuryLighthouseUpper, .LField_MercuryLighthouseUpper_BLEND
	map_offset Field_MercuryLighthouseUpper, .LField_MercuryLighthouseUpper_MARKERS
.LField_MercuryLighthouseUpper_CELL_PICTURES:
	.incbin "games/tbs/src/map/mercury_lighthouse_upper/cell_pictures.delta1.lz"
	.balign 4, 0
.LField_MercuryLighthouseUpper_SHAPES:
	.incbin "games/tbs/src/map/mercury_lighthouse_upper/shapes.table.lz"
	.balign 4, 0
.LField_MercuryLighthouseUpper_GRID:
	.incbin "games/tbs/src/map/mercury_lighthouse_upper/map.grid.lz"
	.balign 4, 0
.LField_MercuryLighthouseUpper_ANIMATION:
	.incbin "games/tbs/src/map/mercury_lighthouse_entrance/tileset_anims.script.lz"
	.balign 4, 0
.LField_MercuryLighthouseUpper_BLEND:
	.incbin "games/tbs/src/map/mercury_lighthouse_entrance/blend.script.lz"
	.balign 4, 0
.LField_MercuryLighthouseUpper_MARKERS:
	treasure_spots_end
	.balign 4, 0
