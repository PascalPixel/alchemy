	.include "games/tbs/src/map/map.inc"
	.section .rodata
	.global Field_KolimaForestCliff
Field_KolimaForestCliff:
	map_header 0, 0, 64, 64, 2, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 64, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_offset Field_KolimaForestCliff, .LField_KolimaForestCliff_CELL_PICTURES
	map_offset Field_KolimaForestCliff, .LField_KolimaForestCliff_SHAPES
	map_offset Field_KolimaForestCliff, .LField_KolimaForestCliff_GRID
	map_offset Field_KolimaForestCliff, .LField_KolimaForestCliff_ANIMATION
	map_offset Field_KolimaForestCliff, .LField_KolimaForestCliff_BLEND
	map_offset Field_KolimaForestCliff, .LField_KolimaForestCliff_MARKERS
.LField_KolimaForestCliff_CELL_PICTURES:
	.incbin "games/tbs/src/map/kolima_forest_cliff/cell_pictures.delta0.lz"
	.balign 4, 0
.LField_KolimaForestCliff_SHAPES:
	.incbin "games/tbs/src/map/kolima_forest_cliff/shapes.table.lz"
	.balign 4, 0
.LField_KolimaForestCliff_GRID:
	.incbin "games/tbs/src/map/kolima_forest_cliff/map.grid.lz"
	.balign 4, 0
.LField_KolimaForestCliff_ANIMATION:
	.incbin "games/tbs/src/map/kolima_forest_cliff/tileset_anims.script.lz"
	.balign 4, 0
.LField_KolimaForestCliff_BLEND:
	.incbin "games/tbs/src/map/kolima_bridge/blend.script.lz"
	.balign 4, 0
.LField_KolimaForestCliff_MARKERS:
	treasure_spots_end
	.balign 4, 0
	.global Tileset_Palette02
Tileset_Palette02:
	.incbin "games/tbs/src/graphics/tileset/palettes/pal_02.gbapal.lz"
	.byte 0xe3, 0x65	@ PACKER: left in the buffer by Field_KolimaForestCliff+0x166
