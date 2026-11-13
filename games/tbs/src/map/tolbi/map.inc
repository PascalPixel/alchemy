	.global Field_Tolbi
Field_Tolbi:
	map_header 0, 0, 76, 64, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 76, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 76, 16, 16, 0, 0, 0xff, 0xff
	map_offset Field_Tolbi, .LField_Tolbi_CELL_PICTURES
	map_offset Field_Tolbi, .LField_Tolbi_SHAPES
	map_offset Field_Tolbi, .LField_Tolbi_GRID
	map_offset Field_Tolbi, .LField_Tolbi_ANIMATION
	map_offset Field_Tolbi, .LField_Tolbi_BLEND
	map_offset Field_Tolbi, .LField_Tolbi_MARKERS
.LField_Tolbi_CELL_PICTURES:
	.incbin "games/tbs/src/map/tolbi/cell_pictures.delta1.lz"
	.balign 4, 0
.LField_Tolbi_SHAPES:
	.incbin "games/tbs/src/map/tolbi/shapes.table.lz"
	.balign 4, 0
.LField_Tolbi_GRID:
	.incbin "games/tbs/src/map/tolbi/map.grid.lz"
	.balign 4, 0
.LField_Tolbi_ANIMATION:
	.incbin "games/tbs/src/map/tolbi/tileset_anims.script.lz"
	.balign 4, 0
.LField_Tolbi_BLEND:
	.incbin "games/tbs/src/map/goma_cave_entrance/blend.script.lz"
	.balign 4, 0
.LField_Tolbi_MARKERS:
	treasure_spot 8, 9, 100
	treasure_spot 18, 18, 239
	treasure_spot 19, 18, 239
	treasure_spot 30, 18, 101
	treasure_spot 3, 32, 102
	treasure_spots_end
	.balign 4, 0
	.global Tileset_PaletteTolbi
Tileset_PaletteTolbi:
	.incbin "games/tbs/src/graphics/tileset/palettes/tolbi.gbapal.lz"
	.byte 0x5b, 0x24, 0xd1	@ PACKER: left in the buffer by Field_Tolbi+0x18d
	.balign 4, 0
