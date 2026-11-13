	.global Field_BabiLighthouseEntry
Field_BabiLighthouseEntry:
	map_header 0, 0, 128, 128, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	map_offset Field_BabiLighthouseEntry, .LField_BabiLighthouseEntry_CELL_PICTURES
	map_offset Field_BabiLighthouseEntry, .LField_BabiLighthouseEntry_SHAPES
	map_offset Field_BabiLighthouseEntry, .LField_BabiLighthouseEntry_GRID
	map_offset Field_BabiLighthouseEntry, .LField_BabiLighthouseEntry_ANIMATION
	map_offset Field_BabiLighthouseEntry, .LField_BabiLighthouseEntry_BLEND
	map_offset Field_BabiLighthouseEntry, .LField_BabiLighthouseEntry_MARKERS
.LField_BabiLighthouseEntry_CELL_PICTURES:
	.incbin "games/tbs/src/map/babi_lighthouse_entry/cell_pictures.delta1.lz"
	.balign 4, 0
.LField_BabiLighthouseEntry_SHAPES:
	.incbin "games/tbs/src/map/babi_lighthouse_entry/shapes.table.lz"
	.balign 4, 0
.LField_BabiLighthouseEntry_GRID:
	.incbin "games/tbs/src/map/babi_lighthouse_entry/map.grid.lz"
	.balign 4, 0
.LField_BabiLighthouseEntry_ANIMATION:
	.incbin "games/tbs/src/map/kolima/tileset_anims.script.lz"
	.balign 4, 0
.LField_BabiLighthouseEntry_BLEND:
	.incbin "games/tbs/src/map/goma_cave_entrance/blend.script.lz"
	.balign 4, 0
.LField_BabiLighthouseEntry_MARKERS:
	treasure_spot 38, 9, 102
	treasure_spot 50, 11, 101
	treasure_spot 40, 16, 103
	treasure_spot 43, 30, 100
	treasure_spots_end
	.balign 4, 0
	.global Tileset_Palette44
Tileset_Palette44:
	.incbin "games/tbs/src/graphics/tileset/palettes/pal_44.gbapal.lz"
	.byte 0x32	@ PACKER: left in the buffer by Field_BabiLighthouseEntry+0x173
	.global Tileset_Palette45
Tileset_Palette45:
	.incbin "games/tbs/src/graphics/tileset/palettes/pal_45.gbapal.lz"
	.balign 4, 0
