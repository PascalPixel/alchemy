	.global Field_BabiLighthouseRooms
Field_BabiLighthouseRooms:
	map_header 0, 0, 128, 128, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	map_offset Field_BabiLighthouseRooms, .LField_BabiLighthouseRooms_CELL_TILES
	map_offset Field_BabiLighthouseRooms, .LField_BabiLighthouseRooms_SHAPES
	map_offset Field_BabiLighthouseRooms, .LField_BabiLighthouseRooms_GRID
	map_offset Field_BabiLighthouseRooms, .LField_BabiLighthouseRooms_ANIMATION
	map_offset Field_BabiLighthouseRooms, .LField_BabiLighthouseRooms_BLEND
	map_offset Field_BabiLighthouseRooms, .LField_BabiLighthouseRooms_MARKERS
.LField_BabiLighthouseRooms_CELL_TILES:
	.incbin "games/tbs/src/map/babi_lighthouse_rooms/cell_tiles.delta2.lz"
	.balign 4, 0
.LField_BabiLighthouseRooms_SHAPES:
	.incbin "games/tbs/src/map/babi_lighthouse_rooms/shapes.table.lz"
	.balign 4, 0
.LField_BabiLighthouseRooms_GRID:
	.incbin "games/tbs/src/map/babi_lighthouse_rooms/map.grid.lz"
	.balign 4, 0
.LField_BabiLighthouseRooms_ANIMATION:
	.incbin "games/tbs/src/map/kolima/tileset_anims.script.lz"
	.balign 4, 0
.LField_BabiLighthouseRooms_BLEND:
	.incbin "games/tbs/src/map/babi_lighthouse_rooms/blend.script.lz"
	.balign 4, 0
.LField_BabiLighthouseRooms_MARKERS:
	treasure_spot 47, 26, 100
	treasure_spot 41, 29, 103
	treasure_spot 42, 42, 102
	treasure_spot 55, 43, 101
	treasure_spots_end
	.balign 4, 0
	.global Tileset_BabiLighthousePalette
Tileset_BabiLighthousePalette:
	.incbin "games/tbs/src/graphics/tileset/babi_lighthouse/palette.gbapal.lz"
	.byte 0xbe, 0xee	@ PACKER: left in the buffer by Field_BabiLighthouseRooms+0x186
	.global Tileset_BabiLighthouseTilesA
Tileset_BabiLighthouseTilesA:
	.incbin "games/tbs/src/graphics/tileset/babi_lighthouse/tiles_a.4bpp.mtf"
	.global Tileset_BabiLighthouseTilesB
Tileset_BabiLighthouseTilesB:
	.incbin "games/tbs/src/graphics/tileset/babi_lighthouse/tiles_b.4bpp.mtf"
	.byte 0x08, 0xe8	@ PACKER: left in the buffer by Tileset_KalayDocksTilesA+0x2672
	.global Tileset_BabiLighthouseTilesC
Tileset_BabiLighthouseTilesC:
	.incbin "games/tbs/src/graphics/tileset/babi_lighthouse/tiles_c.4bpp.mtf"
	.byte 0xfe	@ PACKER: left in the buffer by Tileset_BabiLighthouseTilesB+0x153
	.global Tileset_BabiLighthouseTilesD
Tileset_BabiLighthouseTilesD:
	.incbin "games/tbs/src/graphics/tileset/babi_lighthouse/tiles_d.4bpp.mtf"
	.byte 0xff	@ PACKER: left in the buffer by Tileset_BabiLighthouseTilesC+0x13f
	.balign 4, 0
