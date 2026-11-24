	.global Field_YallamRoomsShops
Field_YallamRoomsShops:
	map_header 0, 0, 120, 120, 3, 1, 2, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 120, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 120, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_YallamRoomsShops, .LField_YallamRoomsShops_CELL_TILES
	map_block Field_YallamRoomsShops, .LField_YallamRoomsShops_SHAPES
	map_block Field_YallamRoomsShops, .LField_YallamRoomsShops_GRID
	map_block Field_YallamRoomsShops, .LField_YallamRoomsShops_SHAPE_GRID
	map_block Field_YallamRoomsShops, .LField_YallamRoomsShops_ANIMATION
	map_block Field_YallamRoomsShops, .LField_YallamRoomsShops_BLEND
	map_block Field_YallamRoomsShops, .LField_YallamRoomsShops_SCENE
.LField_YallamRoomsShops_CELL_TILES:
	.incbin "games/tla/src/map/yallam_rooms_shops/cell_tiles.delta2.lz"
	.balign 4, 0
.LField_YallamRoomsShops_SHAPES:
	.incbin "games/tla/src/map/yallam_rooms_shops/shapes.table.lz"
	.balign 4, 0
.LField_YallamRoomsShops_GRID:
	.incbin "games/tla/src/map/yallam_rooms_shops/map.grid.lz"
	.balign 4, 0
.LField_YallamRoomsShops_SHAPE_GRID:
	.incbin "games/tla/src/map/yallam_rooms_shops/shape_grid.plane.lz"
	.balign 4, 0
.LField_YallamRoomsShops_ANIMATION:
	.incbin "games/tla/src/map/madra_rooms_cavern_homes/animation.script.lz"
	.balign 4, 0
.LField_YallamRoomsShops_BLEND:
	.incbin "games/tla/src/map/daila_rooms_houses/blend.script.lz"
	.balign 4, 0
.LField_YallamRoomsShops_SCENE:
	.incbin "games/tla/src/map/yallam_rooms_shops/scene.scene.lz"
	.balign 4, 0
	.global Tileset_PaletteYallamRoomsShopsA
Tileset_PaletteYallamRoomsShopsA:
	.incbin "games/tla/src/graphics/tileset/palettes/yallam_rooms_shops_a.gbapal.lz"
	.byte 0xf6, 0x1c, 0xf7	@ PACKER: left in the buffer by Field_YallamRooms2+0x199
	.global Tileset_PaletteYallamRoomsShopsB
Tileset_PaletteYallamRoomsShopsB:
	.incbin "games/tla/src/graphics/tileset/palettes/yallam_rooms_shops_b.gbapal.lz"
	.byte 0x44	@ PACKER: left in the buffer by Tileset_PaletteYallamRoomsShopsA+0x15b
