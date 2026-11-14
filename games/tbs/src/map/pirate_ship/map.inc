	.global Field_PirateShip
Field_PirateShip:
	map_header 0, 0, 45, 120, 3, 1, 2, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 120, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 120, 16, 16, 0, 0, 0xff, 0xff
	map_offset Field_PirateShip, .LField_PirateShip_CELL_PICTURES
	map_offset Field_PirateShip, .LField_PirateShip_SHAPES
	map_offset Field_PirateShip, .LField_PirateShip_GRID
	map_offset Field_PirateShip, .LField_PirateShip_ANIMATION
	map_offset Field_PirateShip
	map_offset Field_PirateShip, .LField_PirateShip_MARKERS
.LField_PirateShip_CELL_PICTURES:
	.incbin "games/tbs/src/map/pirate_ship/cell_pictures.delta1.lz"
	.balign 4, 0
.LField_PirateShip_SHAPES:
	.incbin "games/tbs/src/map/pirate_ship/shapes.table.lz"
	.balign 4, 0
.LField_PirateShip_GRID:
	.incbin "games/tbs/src/map/pirate_ship/map.grid.lz"
	.balign 4, 0
.LField_PirateShip_ANIMATION:
	.incbin "games/tbs/src/map/tolbi_ship_deck/tileset_anims.script.lz"
	.balign 4, 0
.LField_PirateShip_MARKERS:
	treasure_spot 0, 1, 100
	treasure_spot 0, 3, 101
	treasure_spots_end
	.balign 4, 0
	.global Tileset_PalettePirateShip
Tileset_PalettePirateShip:
	.incbin "games/tbs/src/graphics/tileset/palettes/pirate_ship.gbapal.lz"
	.balign 4, 0
