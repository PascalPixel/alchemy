	.global Field_ColosseumEntrance
Field_ColosseumEntrance:
	map_header 0, 0, 64, 64, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 64, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 64, 16, 16, 0, 0, 0xff, 0xff
	map_offset Field_ColosseumEntrance, .LField_ColosseumEntrance_CELL_PICTURES
	map_offset Field_ColosseumEntrance, .LField_ColosseumEntrance_SHAPES
	map_offset Field_ColosseumEntrance, .LField_ColosseumEntrance_GRID
	map_offset Field_ColosseumEntrance, .LField_ColosseumEntrance_ANIMATION
	map_offset Field_ColosseumEntrance, .LField_ColosseumEntrance_BLEND
	map_offset Field_ColosseumEntrance, .LField_ColosseumEntrance_MARKERS
.LField_ColosseumEntrance_CELL_PICTURES:
	.incbin "games/tbs/src/map/colosseum_entrance/cell_pictures.delta1.lz"
	.balign 4, 0
.LField_ColosseumEntrance_SHAPES:
	.incbin "games/tbs/src/map/colosseum_entrance/shapes.table.lz"
	.balign 4, 0
.LField_ColosseumEntrance_GRID:
	.incbin "games/tbs/src/map/colosseum_entrance/map.grid.lz"
	.balign 4, 0
.LField_ColosseumEntrance_ANIMATION:
	.incbin "games/tbs/src/map/kalay/tileset_anims.script.lz"
	.balign 4, 0
.LField_ColosseumEntrance_BLEND:
	.incbin "games/tbs/src/map/goma_cave_entrance/blend.script.lz"
	.balign 4, 0
.LField_ColosseumEntrance_MARKERS:
	treasure_spots_end
	.balign 4, 0
	.global Tileset_PaletteColosseumEntrance
Tileset_PaletteColosseumEntrance:
	.incbin "games/tbs/src/graphics/tileset/palettes/colosseum_entrance.gbapal.lz"
	.byte 0x7f, 0x45, 0xd8	@ PACKER: left in the buffer by Field_ColosseumEntrance+0x18d
	.balign 4, 0
