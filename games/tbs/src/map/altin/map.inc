	.global Field_Altin
Field_Altin:
	map_header 0, 0, 64, 64, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 64, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 64, 16, 16, 0, 0, 0xff, 0xff
	map_offset Field_Altin, .LField_Altin_CELL_PICTURES
	map_offset Field_Altin, .LField_Altin_SHAPES
	map_offset Field_Altin, .LField_Altin_GRID
	map_offset Field_Altin, .LField_Altin_ANIMATION
	map_offset Field_Altin, .LField_Altin_BLEND
	map_offset Field_Altin, .LField_Altin_MARKERS
.LField_Altin_CELL_PICTURES:
	.incbin "games/tbs/src/map/altin/cell_pictures.delta1.lz"
	.balign 4, 0
.LField_Altin_SHAPES:
	.incbin "games/tbs/src/map/altin/shapes.table.lz"
	.balign 4, 0
.LField_Altin_GRID:
	.incbin "games/tbs/src/map/altin/map.grid.lz"
	.balign 4, 0
.LField_Altin_ANIMATION:
	.incbin "games/tbs/src/map/altin/tileset_anims.script.lz"
	.balign 4, 0
.LField_Altin_BLEND:
	.incbin "games/tbs/src/map/altin/blend.script.lz"
	.balign 4, 0
.LField_Altin_MARKERS:
	treasure_spots_end
	.balign 4, 0
	.global Tileset_AltinPalette
Tileset_AltinPalette:
	.incbin "games/tbs/src/graphics/tileset/altin/palette.gbapal.lz"
	.global Tileset_AltinTilesA
Tileset_AltinTilesA:
	.incbin "games/tbs/src/graphics/tileset/altin/tiles_a.4bpp.mtf"
	.global Tileset_AltinTilesB
Tileset_AltinTilesB:
	.incbin "games/tbs/src/graphics/tileset/altin/tiles_b.4bpp.mtf"
	.byte 0xd6, 0x54	@ PACKER: left in the buffer by Tileset_AltinTilesA+0x2062
	.global Tileset_AltinTilesC
Tileset_AltinTilesC:
	.incbin "games/tbs/src/graphics/tileset/altin/tiles_c.4bpp.mtf"
	.global Tileset_AltinTilesD
Tileset_AltinTilesD:
	.incbin "games/tbs/src/graphics/tileset/altin/tiles_d.4bpp.mtf"
	.byte 0xd0	@ PACKER: left in the buffer by Tileset_AltinTilesC+0x130b
	.balign 4, 0
