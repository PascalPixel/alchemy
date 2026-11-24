	.global Field_ProxHouses
Field_ProxHouses:
	map_header 0, 0, 120, 120, 3, 1, 2, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 120, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 120, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_ProxHouses, .LField_ProxHouses_CELL_TILES
	map_block Field_ProxHouses, .LField_ProxHouses_SHAPES
	map_block Field_ProxHouses, .LField_ProxHouses_GRID
	map_block Field_ProxHouses, .LField_ProxHouses_SHAPE_GRID
	map_block Field_ProxHouses, .LField_ProxHouses_ANIMATION
	map_block Field_ProxHouses, .LField_ProxHouses_BLEND
	map_block Field_ProxHouses, .LField_ProxHouses_SCENE
.LField_ProxHouses_CELL_TILES:
	.incbin "games/tla/src/map/prox_houses/cell_tiles.delta1.lz"
	.balign 4, 0
.LField_ProxHouses_SHAPES:
	.incbin "games/tla/src/map/prox_houses/shapes.table.lz"
	.balign 4, 0
.LField_ProxHouses_GRID:
	.incbin "games/tla/src/map/prox_houses/map.grid.lz"
	.balign 4, 0
.LField_ProxHouses_SHAPE_GRID:
	.incbin "games/tla/src/map/prox_houses/shape_grid.plane.lz"
	.balign 4, 0
.LField_ProxHouses_ANIMATION:
	.incbin "games/tla/src/map/kalt_island_houses/animation.script.lz"
	.balign 4, 0
.LField_ProxHouses_BLEND:
	.incbin "games/tla/src/map/daila_rooms_houses/blend.script.lz"
	.balign 4, 0
.LField_ProxHouses_SCENE:
	.incbin "games/tla/src/map/prox_houses/scene.scene.lz"
	.balign 4, 0
	.global Tileset_PaletteMadraRoomsCavernHomes
Tileset_PaletteMadraRoomsCavernHomes:
	.incbin "games/tla/src/graphics/tileset/palettes/madra_rooms_cavern_homes.gbapal.lz"
	.byte 0x0f	@ PACKER: left in the buffer by Field_ProxHouses+0x1b7
	.global Tileset_PaletteMikasallaRoomsSanctum
Tileset_PaletteMikasallaRoomsSanctum:
	.incbin "games/tla/src/graphics/tileset/palettes/mikasalla_rooms_sanctum.gbapal.lz"
	.byte 0xb4	@ PACKER: left in the buffer by Tileset_PaletteMadraRoomsCavernHomes+0x187
