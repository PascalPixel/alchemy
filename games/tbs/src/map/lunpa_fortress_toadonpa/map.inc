	.global Field_LunpaFortressToadonpa
Field_LunpaFortressToadonpa:
	map_header 0, 0, 72, 94, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 96, 16, 16, 0, 0, 0xff, 0xff
	map_layer 80, 0, 16, 16, 0, 0, 0xff, 0xff
	map_offset Field_LunpaFortressToadonpa, .LField_LunpaFortressToadonpa_CELL_PICTURES
	map_offset Field_LunpaFortressToadonpa, .LField_LunpaFortressToadonpa_SHAPES
	map_offset Field_LunpaFortressToadonpa, .LField_LunpaFortressToadonpa_GRID
	map_offset Field_LunpaFortressToadonpa, .LField_LunpaFortressToadonpa_ANIMATION
	map_offset Field_LunpaFortressToadonpa, .LField_LunpaFortressToadonpa_BLEND
	map_offset Field_LunpaFortressToadonpa, .LField_LunpaFortressToadonpa_MARKERS
.LField_LunpaFortressToadonpa_CELL_PICTURES:
	.incbin "games/tbs/src/map/lunpa_fortress_toadonpa/cell_pictures.delta2.lz"
	.balign 4, 0
.LField_LunpaFortressToadonpa_SHAPES:
	.incbin "games/tbs/src/map/fuchin_falls_cave_entrance/shapes.table.lz"
	.balign 4, 0
.LField_LunpaFortressToadonpa_GRID:
	.incbin "games/tbs/src/map/lunpa_fortress_toadonpa/map.grid.lz"
	.balign 4, 0
.LField_LunpaFortressToadonpa_ANIMATION:
	.incbin "games/tbs/src/map/vale_cave_kite/tileset_anims.script.lz"
	.balign 4, 0
.LField_LunpaFortressToadonpa_BLEND:
	.incbin "games/tbs/src/map/lunpa_fortress_ground_floor/blend.script.lz"
	.balign 4, 0
.LField_LunpaFortressToadonpa_MARKERS:
	treasure_spot 8, 4, 101
	treasure_spots_end
	.balign 4, 0
