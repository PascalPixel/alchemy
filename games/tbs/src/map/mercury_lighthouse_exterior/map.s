	.include "games/tbs/src/map/map.inc"
	.section .rodata
	.global Field_MercuryLighthouseExterior
Field_MercuryLighthouseExterior:
	map_header 0, 0, 128, 128, 3, 1, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	map_offset Field_MercuryLighthouseExterior, .LField_MercuryLighthouseExterior_CELL_PICTURES
	map_offset Field_MercuryLighthouseExterior, .LField_MercuryLighthouseExterior_SHAPES
	map_offset Field_MercuryLighthouseExterior, .LField_MercuryLighthouseExterior_GRID
	map_offset Field_MercuryLighthouseExterior, .LField_MercuryLighthouseExterior_ANIMATION
	map_offset Field_MercuryLighthouseExterior, .LField_MercuryLighthouseExterior_BLEND
	map_offset Field_MercuryLighthouseExterior, .LField_MercuryLighthouseExterior_MARKERS
.LField_MercuryLighthouseExterior_CELL_PICTURES:
	.incbin "games/tbs/src/map/mercury_lighthouse_exterior/cell_pictures.delta2.lz"
	.balign 4, 0
.LField_MercuryLighthouseExterior_SHAPES:
	.incbin "games/tbs/src/map/mercury_lighthouse_exterior/shapes.table.lz"
	.balign 4, 0
.LField_MercuryLighthouseExterior_GRID:
	.incbin "games/tbs/src/map/mercury_lighthouse_exterior/map.grid.lz"
	.balign 4, 0
.LField_MercuryLighthouseExterior_ANIMATION:
	.incbin "games/tbs/src/map/mercury_lighthouse_exterior/tileset_anims.script.lz"
	.balign 4, 0
.LField_MercuryLighthouseExterior_BLEND:
	.incbin "games/tbs/src/map/mercury_lighthouse_exterior/blend.script.lz"
	.balign 4, 0
.LField_MercuryLighthouseExterior_MARKERS:
	treasure_spots_end
	.balign 4, 0
