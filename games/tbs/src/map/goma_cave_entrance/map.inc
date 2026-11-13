	.global Field_GomaCaveEntrance
Field_GomaCaveEntrance:
	map_header 0, 0, 62, 58, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 62, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 124, 0, 16, 16, 0, 0, 0xff, 0xff
	map_offset Field_GomaCaveEntrance, .LField_GomaCaveEntrance_CELL_PICTURES
	map_offset Field_GomaCaveEntrance, .LField_GomaCaveEntrance_SHAPES
	map_offset Field_GomaCaveEntrance, .LField_GomaCaveEntrance_GRID
	map_offset Field_GomaCaveEntrance, .LField_GomaCaveEntrance_ANIMATION
	map_offset Field_GomaCaveEntrance, .LField_GomaCaveEntrance_BLEND
	map_offset Field_GomaCaveEntrance, .LField_GomaCaveEntrance_MARKERS
.LField_GomaCaveEntrance_CELL_PICTURES:
	.incbin "games/tbs/src/map/goma_cave_entrance/cell_pictures.delta0.lz"
	.balign 4, 0
.LField_GomaCaveEntrance_SHAPES:
	.incbin "games/tbs/src/map/goma_cave_entrance/shapes.table.lz"
	.balign 4, 0
.LField_GomaCaveEntrance_GRID:
	.incbin "games/tbs/src/map/goma_cave_entrance/map.grid.lz"
	.balign 4, 0
.LField_GomaCaveEntrance_ANIMATION:
	.incbin "games/tbs/src/map/goma_cave_entrance/tileset_anims.script.lz"
	.balign 4, 0
.LField_GomaCaveEntrance_BLEND:
	.incbin "games/tbs/src/map/goma_cave_entrance/blend.script.lz"
	.balign 4, 0
.LField_GomaCaveEntrance_MARKERS:
	treasure_spots_end
	.balign 4, 0
	.global Tileset_Set17Palette
Tileset_Set17Palette:
	.incbin "games/tbs/src/graphics/tileset/set_17/palette.gbapal.lz"
	.global Tileset_Set17TilesA
Tileset_Set17TilesA:
	.incbin "games/tbs/src/graphics/tileset/set_17/tiles_a.4bpp.mtf"
	.byte 0xe1, 0xe8, 0xf4	@ PACKER: left in the buffer by Field_CottageRoomsB+0x2d51
	.global Tileset_Set17TilesB
Tileset_Set17TilesB:
	.incbin "games/tbs/src/graphics/tileset/set_17/tiles_b.4bpp.mtf"
	.global Tileset_Set17TilesC
Tileset_Set17TilesC:
	.incbin "games/tbs/src/graphics/tileset/set_17/tiles_c.4bpp.mtf"
	.global Tileset_Set17TilesD
Tileset_Set17TilesD:
	.incbin "games/tbs/src/graphics/tileset/set_17/tiles_d.4bpp.mtf"
	.balign 4, 0
