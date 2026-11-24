	.global Field_IzumoRuinsSecondRoom
Field_IzumoRuinsSecondRoom:
	map_header 0, 0, 128, 128, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_IzumoRuinsSecondRoom, .LField_IzumoRuinsSecondRoom_CELL_TILES
	map_block Field_IzumoRuinsSecondRoom, .LField_IzumoRuinsSecondRoom_SHAPES
	map_block Field_IzumoRuinsSecondRoom, .LField_IzumoRuinsSecondRoom_GRID
	map_block Field_IzumoRuinsSecondRoom, .LField_IzumoRuinsSecondRoom_SHAPE_GRID
	map_block Field_IzumoRuinsSecondRoom, .LField_IzumoRuinsSecondRoom_ANIMATION
	map_block Field_IzumoRuinsSecondRoom
	map_block Field_IzumoRuinsSecondRoom, .LField_IzumoRuinsSecondRoom_SCENE
.LField_IzumoRuinsSecondRoom_CELL_TILES:
	.incbin "games/tla/src/map/izumo_ruins_second_room/cell_tiles.delta1.lz"
	.balign 4, 0
.LField_IzumoRuinsSecondRoom_SHAPES:
	.incbin "games/tla/src/map/izumo_ruins_second_room/shapes.table.lz"
	.balign 4, 0
.LField_IzumoRuinsSecondRoom_GRID:
	.incbin "games/tla/src/map/izumo_ruins_second_room/map.grid.lz"
	.balign 4, 0
.LField_IzumoRuinsSecondRoom_SHAPE_GRID:
	.incbin "games/tla/src/map/izumo_ruins_second_room/shape_grid.plane.lz"
	.balign 4, 0
.LField_IzumoRuinsSecondRoom_ANIMATION:
	.incbin "games/tla/src/map/izumo_ruins_second_room/animation.script.lz"
	.balign 4, 0
.LField_IzumoRuinsSecondRoom_SCENE:
	.incbin "games/tla/src/map/izumo_ruins_second_room/scene.scene.lz"
	.global Tileset_IzumoRuinsSecondRoomPalette
Tileset_IzumoRuinsSecondRoomPalette:
	.incbin "games/tla/src/graphics/tileset/izumo_ruins_second_room/palette.gbapal.lz"
	.byte 0xfd, 0xa6, 0x32	@ PACKER: left in the buffer by Field_IzumoRuinsSecondRoom+0x14d
	.global Tileset_IzumoRuinsSecondRoomTilesA
Tileset_IzumoRuinsSecondRoomTilesA:
	.incbin "games/tla/src/graphics/tileset/izumo_ruins_second_room/tiles_a.4bpp.mtf"
	.byte 0x04, 0xc3, 0x4b	@ PACKER: left in the buffer by Field_IzumoRuinsSecondRoom+0x4d1
