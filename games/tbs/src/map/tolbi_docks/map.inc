	.global Field_TolbiDocks
Field_TolbiDocks:
	map_header 0, 0, 60, 188, 3, 2, 1, 1, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 60, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 120, 0, 16, 16, 0, 0, 0xff, 0xff
	map_offset Field_TolbiDocks, .LField_TolbiDocks_CELL_PICTURES
	map_offset Field_TolbiDocks, .LField_TolbiDocks_SHAPES
	map_offset Field_TolbiDocks, .LField_TolbiDocks_GRID
	map_offset Field_TolbiDocks, .LField_TolbiDocks_ANIMATION
	map_offset Field_TolbiDocks
	map_offset Field_TolbiDocks, .LField_TolbiDocks_MARKERS
.LField_TolbiDocks_CELL_PICTURES:
	.incbin "games/tbs/src/map/tolbi_docks/cell_pictures.delta1.lz"
	.balign 4, 0
.LField_TolbiDocks_SHAPES:
	.incbin "games/tbs/src/map/tolbi_docks/shapes.table.lz"
	.balign 4, 0
.LField_TolbiDocks_GRID:
	.incbin "games/tbs/src/map/tolbi_docks/map.grid.lz"
	.balign 4, 0
.LField_TolbiDocks_ANIMATION:
	.incbin "games/tbs/src/map/kalay_docks/tileset_anims.script.lz"
	.balign 4, 0
.LField_TolbiDocks_MARKERS:
	treasure_spot 3, 3, 100
	treasure_spot 25, 12, 101
	treasure_spot 53, 56, 101
	treasure_spots_end
	.global Tileset_PaletteTolbiDocks
Tileset_PaletteTolbiDocks:
	.incbin "games/tbs/src/graphics/tileset/palettes/tolbi_docks.gbapal.lz"
	.byte 0x19, 0xe9, 0x78	@ PACKER: left in the buffer by Field_TolbiDocks+0x1a5
	.balign 4, 0
