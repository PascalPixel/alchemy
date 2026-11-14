	.global Field_AlpineCrossing
Field_AlpineCrossing:
	map_header 0, 0, 72, 66, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 72, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 144, 0, 16, 16, 0, 0, 0xff, 0xff
	map_offset Field_AlpineCrossing, .LField_AlpineCrossing_CELL_PICTURES
	map_offset Field_AlpineCrossing, .LField_AlpineCrossing_SHAPES
	map_offset Field_AlpineCrossing, .LField_AlpineCrossing_GRID
	map_offset Field_AlpineCrossing, .LField_AlpineCrossing_ANIMATION
	map_offset Field_AlpineCrossing, .LField_AlpineCrossing_BLEND
	map_offset Field_AlpineCrossing, .LField_AlpineCrossing_MARKERS
.LField_AlpineCrossing_CELL_PICTURES:
	.incbin "games/tbs/src/map/alpine_crossing/cell_pictures.delta0.lz"
	.balign 4, 0
.LField_AlpineCrossing_SHAPES:
	.incbin "games/tbs/src/map/alpine_crossing/shapes.table.lz"
	.balign 4, 0
.LField_AlpineCrossing_GRID:
	.incbin "games/tbs/src/map/alpine_crossing/map.grid.lz"
	.balign 4, 0
.LField_AlpineCrossing_ANIMATION:
	.incbin "games/tbs/src/map/alpine_crossing/tileset_anims.script.lz"
	.balign 4, 0
.LField_AlpineCrossing_BLEND:
	.incbin "games/tbs/src/map/goma_cave_entrance/blend.script.lz"
	.balign 4, 0
.LField_AlpineCrossing_MARKERS:
	treasure_spot 27, 24, 100
	treasure_spots_end
	.balign 4, 0
	.global Tileset_PaletteAlpineCrossing
Tileset_PaletteAlpineCrossing:
	.incbin "games/tbs/src/graphics/tileset/palettes/alpine_crossing.gbapal.lz"
	.byte 0xdb, 0x0e, 0xea	@ PACKER: left in the buffer by Field_AlpineCrossing+0x195
	.balign 4, 0
