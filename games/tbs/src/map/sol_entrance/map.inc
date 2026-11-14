	.global Field_SolEntrance
Field_SolEntrance:
	map_header 0, 0, 30, 45, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 30, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 50, 16, 16, 0, 0, 0xff, 0xff
	map_offset Field_SolEntrance, .LField_SolEntrance_CELL_PICTURES
	map_offset Field_SolEntrance, .LField_SolEntrance_SHAPES
	map_offset Field_SolEntrance, .LField_SolEntrance_GRID
	map_offset Field_SolEntrance
	map_offset Field_SolEntrance
	map_offset Field_SolEntrance, .LField_SolEntrance_MARKERS
.LField_SolEntrance_CELL_PICTURES:
	.incbin "games/tbs/src/map/sol_entrance/cell_pictures.delta1.lz"
	.balign 4, 0
.LField_SolEntrance_SHAPES:
	.incbin "games/tbs/src/map/sol_entrance/shapes.table.lz"
	.balign 4, 0
.LField_SolEntrance_GRID:
	.incbin "games/tbs/src/map/sol_entrance/map.grid.lz"
	.balign 4, 0
.LField_SolEntrance_MARKERS:
	treasure_spots_end
	.balign 4, 0
	.global Tileset_SolEntrancePalette
Tileset_SolEntrancePalette:
	.incbin "games/tbs/src/graphics/tileset/sol_entrance/palette.gbapal.lz"
	.byte 0x13, 0x3e	@ PACKER: left in the buffer by Field_SolEntrance+0xe2
	.global Tileset_SolEntranceTilesA
Tileset_SolEntranceTilesA:
	.incbin "games/tbs/src/graphics/tileset/sol_entrance/tiles_a.4bpp.mtf"
	.byte 0xf0, 0xa4, 0x22	@ PACKER: left in the buffer by Field_SolEntrance+0x11e9
	.global Tileset_SolEntranceTilesB
Tileset_SolEntranceTilesB:
	.incbin "games/tbs/src/graphics/tileset/sol_entrance/tiles_b.4bpp.mtf"
	.byte 0xce, 0x43, 0x48	@ PACKER: left in the buffer by Field_SolEntrance+0x1c85
	.global Tileset_SolEntranceTilesC
Tileset_SolEntranceTilesC:
	.incbin "games/tbs/src/graphics/tileset/sol_entrance/tiles_c.4bpp.mtf"
	.byte 0x3e	@ PACKER: left in the buffer by Tileset_SolEntranceTilesB+0x50b
	.global Tileset_SolEntranceTilesD
Tileset_SolEntranceTilesD:
	.incbin "games/tbs/src/graphics/tileset/sol_entrance/tiles_d.4bpp.mtf"
	.byte 0x4e	@ PACKER: left in the buffer by Tileset_SolEntranceTilesC+0x13f
	.balign 4, 0
