	.global Field_AltmillerCaveEntrance
Field_AltmillerCaveEntrance:
	map_header 0, 0, 94, 94, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 96, 16, 16, 0, 0, 0xff, 0xff
	map_layer 100, 0, 16, 16, 0, 0, 0xff, 0xff
	map_offset Field_AltmillerCaveEntrance, .LField_AltmillerCaveEntrance_CELL_PICTURES
	map_offset Field_AltmillerCaveEntrance, .LField_AltmillerCaveEntrance_SHAPES
	map_offset Field_AltmillerCaveEntrance, .LField_AltmillerCaveEntrance_GRID
	map_offset Field_AltmillerCaveEntrance, .LField_AltmillerCaveEntrance_ANIMATION
	map_offset Field_AltmillerCaveEntrance, .LField_AltmillerCaveEntrance_BLEND
	map_offset Field_AltmillerCaveEntrance, .LField_AltmillerCaveEntrance_MARKERS
.LField_AltmillerCaveEntrance_CELL_PICTURES:
	.incbin "games/tbs/src/map/altmiller_cave_entrance/cell_pictures.delta2.lz"
	.balign 4, 0
.LField_AltmillerCaveEntrance_SHAPES:
	.incbin "games/tbs/src/map/altmiller_cave_entrance/shapes.table.lz"
	.balign 4, 0
.LField_AltmillerCaveEntrance_GRID:
	.incbin "games/tbs/src/map/altmiller_cave_entrance/map.grid.lz"
	.balign 4, 0
.LField_AltmillerCaveEntrance_ANIMATION:
	.incbin "games/tbs/src/map/altmiller_cave_entrance/tileset_anims.script.lz"
	.balign 4, 0
.LField_AltmillerCaveEntrance_BLEND:
	.incbin "games/tbs/src/map/goma_cave_entrance/blend.script.lz"
	.balign 4, 0
.LField_AltmillerCaveEntrance_MARKERS:
	treasure_spots_end
	.balign 4, 0
	.global Tileset_Palette31
Tileset_Palette31:
	.incbin "games/tbs/src/graphics/tileset/palettes/pal_31.gbapal.lz"
	.byte 0xc8, 0x1f, 0x81	@ PACKER: left in the buffer by Field_AltmillerCaveEntrance+0x159
	.balign 4, 0
