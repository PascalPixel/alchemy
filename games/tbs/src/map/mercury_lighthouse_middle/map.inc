	.global Field_MercuryLighthouseMiddle
Field_MercuryLighthouseMiddle:
	map_header 0, 0, 128, 128, 3, 1, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	map_offset Field_MercuryLighthouseMiddle, .LField_MercuryLighthouseMiddle_CELL_PICTURES
	map_offset Field_MercuryLighthouseMiddle, .LField_MercuryLighthouseMiddle_SHAPES
	map_offset Field_MercuryLighthouseMiddle, .LField_MercuryLighthouseMiddle_GRID
	map_offset Field_MercuryLighthouseMiddle, .LField_MercuryLighthouseMiddle_ANIMATION
	map_offset Field_MercuryLighthouseMiddle, .LField_MercuryLighthouseMiddle_BLEND
	map_offset Field_MercuryLighthouseMiddle, .LField_MercuryLighthouseMiddle_MARKERS
.LField_MercuryLighthouseMiddle_CELL_PICTURES:
	.incbin "games/tbs/src/map/mercury_lighthouse_middle/cell_pictures.delta1.lz"
	.balign 4, 0
.LField_MercuryLighthouseMiddle_SHAPES:
	.incbin "games/tbs/src/map/mercury_lighthouse_middle/shapes.table.lz"
	.balign 4, 0
.LField_MercuryLighthouseMiddle_GRID:
	.incbin "games/tbs/src/map/mercury_lighthouse_middle/map.grid.lz"
	.balign 4, 0
.LField_MercuryLighthouseMiddle_ANIMATION:
	.incbin "games/tbs/src/map/mercury_lighthouse_entrance/tileset_anims.script.lz"
	.balign 4, 0
.LField_MercuryLighthouseMiddle_BLEND:
	.incbin "games/tbs/src/map/mercury_lighthouse_entrance/blend.script.lz"
	.balign 4, 0
.LField_MercuryLighthouseMiddle_MARKERS:
	treasure_spot 55, 24, 100
	treasure_spot 24, 34, 101
	treasure_spot 39, 39, 102
	treasure_spots_end
	.balign 4, 0
