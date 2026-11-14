	.global Field_MercuryLighthouseLower
Field_MercuryLighthouseLower:
	map_header 0, 0, 128, 128, 3, 1, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	map_offset Field_MercuryLighthouseLower, .LField_MercuryLighthouseLower_CELL_PICTURES
	map_offset Field_MercuryLighthouseLower, .LField_MercuryLighthouseLower_SHAPES
	map_offset Field_MercuryLighthouseLower, .LField_MercuryLighthouseLower_GRID
	map_offset Field_MercuryLighthouseLower, .LField_MercuryLighthouseLower_ANIMATION
	map_offset Field_MercuryLighthouseLower, .LField_MercuryLighthouseLower_BLEND
	map_offset Field_MercuryLighthouseLower, .LField_MercuryLighthouseLower_MARKERS
.LField_MercuryLighthouseLower_CELL_PICTURES:
	.incbin "games/tbs/src/map/mercury_lighthouse_lower/cell_pictures.delta1.lz"
	.balign 4, 0
.LField_MercuryLighthouseLower_SHAPES:
	.incbin "games/tbs/src/map/mercury_lighthouse_lower/shapes.table.lz"
	.balign 4, 0
.LField_MercuryLighthouseLower_GRID:
	.incbin "games/tbs/src/map/mercury_lighthouse_lower/map.grid.lz"
	.balign 4, 0
.LField_MercuryLighthouseLower_ANIMATION:
	.incbin "games/tbs/src/map/mercury_lighthouse_entrance/tileset_anims.script.lz"
	.balign 4, 0
.LField_MercuryLighthouseLower_BLEND:
	.incbin "games/tbs/src/map/mercury_lighthouse_entrance/blend.script.lz"
	.balign 4, 0
.LField_MercuryLighthouseLower_MARKERS:
	treasure_spot 8, 14, 100
	treasure_spot 72, 14, 100
	treasure_spots_end
	.balign 4, 0
