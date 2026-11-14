	.global Field_BabiLighthouseStoneDoor
Field_BabiLighthouseStoneDoor:
	map_header 0, 0, 128, 128, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	map_offset Field_BabiLighthouseStoneDoor, .LField_BabiLighthouseStoneDoor_CELL_PICTURES
	map_offset Field_BabiLighthouseStoneDoor, .LField_BabiLighthouseStoneDoor_SHAPES
	map_offset Field_BabiLighthouseStoneDoor, .LField_BabiLighthouseStoneDoor_GRID
	map_offset Field_BabiLighthouseStoneDoor, .LField_BabiLighthouseStoneDoor_ANIMATION
	map_offset Field_BabiLighthouseStoneDoor
	map_offset Field_BabiLighthouseStoneDoor, .LField_BabiLighthouseStoneDoor_MARKERS
.LField_BabiLighthouseStoneDoor_CELL_PICTURES:
	.incbin "games/tbs/src/map/babi_lighthouse_stone_door/cell_pictures.delta2.lz"
	.balign 4, 0
.LField_BabiLighthouseStoneDoor_SHAPES:
	.incbin "games/tbs/src/map/babi_lighthouse_stone_door/shapes.table.lz"
	.balign 4, 0
.LField_BabiLighthouseStoneDoor_GRID:
	.incbin "games/tbs/src/map/babi_lighthouse_stone_door/map.grid.lz"
	.balign 4, 0
.LField_BabiLighthouseStoneDoor_ANIMATION:
	.incbin "games/tbs/src/map/bilibin_treasury/tileset_anims.script.lz"
	.balign 4, 0
.LField_BabiLighthouseStoneDoor_MARKERS:
	treasure_spots_end
	.balign 4, 0
	.global Tileset_PaletteBabiLighthouseStoneDoor
Tileset_PaletteBabiLighthouseStoneDoor:
	.incbin "games/tbs/src/graphics/tileset/palettes/babi_lighthouse_stone_door.gbapal.lz"
	.byte 0x90, 0x27, 0xd4	@ PACKER: left in the buffer by Field_BabiLighthouseStoneDoor+0x1a5
	.balign 4, 0
