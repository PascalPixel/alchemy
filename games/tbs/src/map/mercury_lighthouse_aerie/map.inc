	.global Field_MercuryLighthouseAerie
Field_MercuryLighthouseAerie:
	map_header 0, 0, 128, 128, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	map_offset Field_MercuryLighthouseAerie, .LField_MercuryLighthouseAerie_CELL_PICTURES
	map_offset Field_MercuryLighthouseAerie, .LField_MercuryLighthouseAerie_SHAPES
	map_offset Field_MercuryLighthouseAerie, .LField_MercuryLighthouseAerie_GRID
	map_offset Field_MercuryLighthouseAerie, .LField_MercuryLighthouseAerie_ANIMATION
	map_offset Field_MercuryLighthouseAerie
	map_offset Field_MercuryLighthouseAerie, .LField_MercuryLighthouseAerie_MARKERS
.LField_MercuryLighthouseAerie_CELL_PICTURES:
	.incbin "games/tbs/src/map/mercury_lighthouse_aerie/cell_pictures.delta0.lz"
	.balign 4, 0
.LField_MercuryLighthouseAerie_SHAPES:
	.incbin "games/tbs/src/map/mercury_lighthouse_aerie/shapes.table.lz"
	.balign 4, 0
.LField_MercuryLighthouseAerie_GRID:
	.incbin "games/tbs/src/map/mercury_lighthouse_aerie/map.grid.lz"
	.balign 4, 0
.LField_MercuryLighthouseAerie_ANIMATION:
	.incbin "games/tbs/src/map/mercury_lighthouse_entrance/tileset_anims.script.lz"
	.balign 4, 0
.LField_MercuryLighthouseAerie_MARKERS:
	treasure_spot 31, 8, 100
	treasure_spots_end
	.balign 4, 0
