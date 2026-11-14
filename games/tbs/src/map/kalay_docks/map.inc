	.global Field_KalayDocks
Field_KalayDocks:
	map_header 5, 124, 51, 188, 3, 2, 1, 1, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 60, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 120, 0, 16, 16, 0, 0, 0xff, 0xff
	map_offset Field_KalayDocks, .LField_KalayDocks_CELL_PICTURES
	map_offset Field_KalayDocks, .LField_KalayDocks_SHAPES
	map_offset Field_KalayDocks, .LField_KalayDocks_GRID
	map_offset Field_KalayDocks, .LField_KalayDocks_ANIMATION
	map_offset Field_KalayDocks
	map_offset Field_KalayDocks, .LField_KalayDocks_MARKERS
.LField_KalayDocks_CELL_PICTURES:
	.incbin "games/tbs/src/map/kalay_docks/cell_pictures.delta1.lz"
	.balign 4, 0
.LField_KalayDocks_SHAPES:
	.incbin "games/tbs/src/map/kalay_docks/shapes.table.lz"
	.balign 4, 0
.LField_KalayDocks_GRID:
	.incbin "games/tbs/src/map/kalay_docks/map.grid.lz"
	.balign 4, 0
.LField_KalayDocks_ANIMATION:
	.incbin "games/tbs/src/map/kalay_docks/tileset_anims.script.lz"
	.balign 4, 0
.LField_KalayDocks_MARKERS:
	treasure_spot 53, 56, 101
	treasure_spots_end
	.balign 4, 0
	.global Tileset_KalayDocksPalette
Tileset_KalayDocksPalette:
	.incbin "games/tbs/src/graphics/tileset/kalay_docks/palette.gbapal.lz"
	.byte 0x2a, 0x91, 0x1b	@ PACKER: left in the buffer by Field_KalayDocks+0x1a5
	.global Tileset_KalayDocksTilesA
Tileset_KalayDocksTilesA:
	.incbin "games/tbs/src/graphics/tileset/kalay_docks/tiles_a.4bpp.mtf"
	.global Tileset_KalayDocksTilesB
Tileset_KalayDocksTilesB:
	.incbin "games/tbs/src/graphics/tileset/kalay_docks/tiles_b.4bpp.mtf"
	.byte 0x83	@ PACKER: left in the buffer by Tileset_KalayDocksTilesA+0x23e3
	.global Tileset_KalayDocksTilesC
Tileset_KalayDocksTilesC:
	.incbin "games/tbs/src/graphics/tileset/kalay_docks/tiles_c.4bpp.mtf"
	.balign 4, 0
