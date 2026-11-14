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
	.global Tileset_GomaCaveEntrancePalette
Tileset_GomaCaveEntrancePalette:
	.incbin "games/tbs/src/graphics/tileset/goma_cave_entrance/palette.gbapal.lz"
	.global Tileset_GomaCaveEntranceTilesA
Tileset_GomaCaveEntranceTilesA:
	.incbin "games/tbs/src/graphics/tileset/goma_cave_entrance/tiles_a.4bpp.mtf"
	.byte 0xe1, 0xe8, 0xf4	@ PACKER: left in the buffer by Field_CottageRoomsB+0x2d51
	.global Tileset_GomaCaveEntranceTilesB
Tileset_GomaCaveEntranceTilesB:
	.incbin "games/tbs/src/graphics/tileset/goma_cave_entrance/tiles_b.4bpp.mtf"
	.global Tileset_GomaCaveEntranceTilesC
Tileset_GomaCaveEntranceTilesC:
	.incbin "games/tbs/src/graphics/tileset/goma_cave_entrance/tiles_c.4bpp.mtf"
	.global Tileset_GomaCaveEntranceTilesD
Tileset_GomaCaveEntranceTilesD:
	.incbin "games/tbs/src/graphics/tileset/goma_cave_entrance/tiles_d.4bpp.mtf"
	.balign 4, 0
