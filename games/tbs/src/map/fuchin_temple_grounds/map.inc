	.global Field_FuchinTempleGrounds
Field_FuchinTempleGrounds:
	map_header 0, 0, 55, 68, 3, 1, 2, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 120, 16, 16, 0, 0, 0xff, 0xff
	map_layer 120, 0, 16, 16, 0, 0, 0xff, 0xff
	map_offset Field_FuchinTempleGrounds, .LField_FuchinTempleGrounds_CELL_PICTURES
	map_offset Field_FuchinTempleGrounds, .LField_FuchinTempleGrounds_SHAPES
	map_offset Field_FuchinTempleGrounds, .LField_FuchinTempleGrounds_GRID
	map_offset Field_FuchinTempleGrounds, .LField_FuchinTempleGrounds_ANIMATION
	map_offset Field_FuchinTempleGrounds, .LField_FuchinTempleGrounds_BLEND
	map_offset Field_FuchinTempleGrounds, .LField_FuchinTempleGrounds_MARKERS
.LField_FuchinTempleGrounds_CELL_PICTURES:
	.incbin "games/tbs/src/map/fuchin_temple_grounds/cell_pictures.delta0.lz"
	.balign 4, 0
.LField_FuchinTempleGrounds_SHAPES:
	.incbin "games/tbs/src/map/fuchin_temple_grounds/shapes.table.lz"
	.balign 4, 0
.LField_FuchinTempleGrounds_GRID:
	.incbin "games/tbs/src/map/fuchin_temple_grounds/map.grid.lz"
	.balign 4, 0
.LField_FuchinTempleGrounds_ANIMATION:
	.incbin "games/tbs/src/map/vale_cave_mouth/tileset_anims.script.lz"
	.balign 4, 0
.LField_FuchinTempleGrounds_BLEND:
	.incbin "games/tbs/src/map/goma_cave_entrance/blend.script.lz"
	.balign 4, 0
.LField_FuchinTempleGrounds_MARKERS:
	treasure_spot 24, 17, 110
	treasure_spots_end
	.balign 4, 0
	.global Tileset_Set44Palette
Tileset_Set44Palette:
	.incbin "games/tbs/src/graphics/tileset/set_44/palette.gbapal.lz"
	.byte 0x32, 0xc8	@ PACKER: left in the buffer by Field_FuchinTempleGrounds+0x1be
	.global Tileset_Set44TilesA
Tileset_Set44TilesA:
	.incbin "games/tbs/src/graphics/tileset/set_44/tiles_a.4bpp.mtf"
	.balign 4, 0
