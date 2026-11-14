	.global Field_McCoyPalace
Field_McCoyPalace:
	map_header 0, 0, 128, 128, 3, 1, 2, 1, 1, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	map_offset Field_McCoyPalace, .LField_McCoyPalace_CELL_PICTURES
	map_offset Field_McCoyPalace, .LField_McCoyPalace_SHAPES
	map_offset Field_McCoyPalace, .LField_McCoyPalace_GRID
	map_offset Field_McCoyPalace, .LField_McCoyPalace_ANIMATION
	map_offset Field_McCoyPalace, .LField_McCoyPalace_BLEND
	map_offset Field_McCoyPalace, .LField_McCoyPalace_MARKERS
.LField_McCoyPalace_CELL_PICTURES:
	.incbin "games/tbs/src/map/mccoy_palace/cell_pictures.delta2.lz"
	.balign 4, 0
.LField_McCoyPalace_SHAPES:
	.incbin "games/tbs/src/map/mccoy_palace/shapes.table.lz"
	.balign 4, 0
.LField_McCoyPalace_GRID:
	.incbin "games/tbs/src/map/mccoy_palace/map.grid.lz"
	.balign 4, 0
.LField_McCoyPalace_ANIMATION:
	.incbin "games/tbs/src/map/mccoy_palace/tileset_anims.script.lz"
	.balign 4, 0
.LField_McCoyPalace_BLEND:
	.incbin "games/tbs/src/map/goma_cave_entrance/blend.script.lz"
	.balign 4, 0
.LField_McCoyPalace_MARKERS:
	treasure_spot 9, 3, 201
	treasure_spot 10, 3, 201
	treasure_spot 17, 3, 200
	treasure_spot 18, 3, 200
	treasure_spot 9, 4, 201
	treasure_spot 10, 4, 201
	treasure_spot 17, 4, 200
	treasure_spot 18, 4, 200
	treasure_spot 45, 29, 121
	treasure_spot 38, 38, 111
	treasure_spot 39, 38, 112
	treasure_spot 40, 38, 113
	treasure_spot 41, 38, 114
	treasure_spot 23, 40, 120
	treasure_spot 38, 40, 108
	treasure_spot 40, 40, 109
	treasure_spot 41, 40, 110
	treasure_spot 38, 41, 105
	treasure_spot 40, 41, 106
	treasure_spot 41, 41, 107
	treasure_spot 38, 55, 100
	treasure_spot 39, 55, 101
	treasure_spot 40, 55, 102
	treasure_spot 41, 55, 103
	treasure_spots_end
	.balign 4, 0
	.global Tileset_McCoyPalacePalette
Tileset_McCoyPalacePalette:
	.incbin "games/tbs/src/graphics/tileset/mccoy_palace/palette.gbapal.lz"
	.byte 0x40, 0x15, 0x08	@ PACKER: left in the buffer by Field_McCoyPalace+0x1a1
	.global Tileset_McCoyPalaceTilesA
Tileset_McCoyPalaceTilesA:
	.incbin "games/tbs/src/graphics/tileset/mccoy_palace/tiles_a.4bpp.mtf"
	.byte 0xfe	@ PACKER: left in the buffer by Tileset_McCoyPalaceGroundsTilesB+0x209f
	.global Tileset_McCoyPalaceTilesB
Tileset_McCoyPalaceTilesB:
	.incbin "games/tbs/src/graphics/tileset/mccoy_palace/tiles_b.4bpp.mtf"
	.global Tileset_McCoyPalaceTilesC
Tileset_McCoyPalaceTilesC:
	.incbin "games/tbs/src/graphics/tileset/mccoy_palace/tiles_c.4bpp.mtf"
	.byte 0x10, 0x84, 0x55	@ PACKER: left in the buffer by Tileset_McCoyPalaceGroundsTilesB+0x22d5
	.global Tileset_McCoyPalaceTilesD
Tileset_McCoyPalaceTilesD:
	.incbin "games/tbs/src/graphics/tileset/mccoy_palace/tiles_d.4bpp.mtf"
	.balign 4, 0
