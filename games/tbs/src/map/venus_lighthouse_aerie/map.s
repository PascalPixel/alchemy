	.include "games/tbs/src/map/map.inc"
	.section .rodata
	.global Field_VenusLighthouseAerie
Field_VenusLighthouseAerie:
	map_header 0, 0, 128, 128, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	map_offset Field_VenusLighthouseAerie, .LField_VenusLighthouseAerie_CELL_PICTURES
	map_offset Field_VenusLighthouseAerie, .LField_VenusLighthouseAerie_SHAPES
	map_offset Field_VenusLighthouseAerie, .LField_VenusLighthouseAerie_GRID
	map_offset Field_VenusLighthouseAerie, .LField_VenusLighthouseAerie_ANIMATION
	map_offset Field_VenusLighthouseAerie
	map_offset Field_VenusLighthouseAerie, .LField_VenusLighthouseAerie_MARKERS
.LField_VenusLighthouseAerie_CELL_PICTURES:
	.incbin "games/tbs/src/map/venus_lighthouse_aerie/cell_pictures.delta0.lz"
	.balign 4, 0
.LField_VenusLighthouseAerie_SHAPES:
	.incbin "games/tbs/src/map/venus_lighthouse_aerie/shapes.table.lz"
	.balign 4, 0
.LField_VenusLighthouseAerie_GRID:
	.incbin "games/tbs/src/map/venus_lighthouse_aerie/map.grid.lz"
	.balign 4, 0
.LField_VenusLighthouseAerie_ANIMATION:
	.incbin "games/tbs/src/map/mercury_lighthouse_entrance/tileset_anims.script.lz"
	.balign 4, 0
.LField_VenusLighthouseAerie_MARKERS:
	treasure_spots_end
	.balign 4, 0
	.global Tileset_PaletteVenusLighthouseAerie
Tileset_PaletteVenusLighthouseAerie:
	.incbin "games/tbs/src/graphics/tileset/palettes/venus_lighthouse_aerie.gbapal.lz"
	.byte 0x06	@ PACKER: left in the buffer by Field_VenusLighthouseAerie+0x103
