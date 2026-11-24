	.global Field_EasternAlhafraShip
Field_EasternAlhafraShip:
	map_header 0, 0, 90, 90, 3, 1, 2, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 90, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 90, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_EasternAlhafraShip, .LField_EasternAlhafraShip_CELL_TILES
	map_block Field_EasternAlhafraShip, .LField_EasternAlhafraShip_SHAPES
	map_block Field_EasternAlhafraShip, .LField_EasternAlhafraShip_GRID
	map_block Field_EasternAlhafraShip, .LField_EasternAlhafraShip_SHAPE_GRID
	map_block Field_EasternAlhafraShip, .LField_EasternAlhafraShip_ANIMATION
	map_block Field_EasternAlhafraShip, .LField_EasternAlhafraShip_BLEND
	map_block Field_EasternAlhafraShip, .LField_EasternAlhafraShip_SCENE
.LField_EasternAlhafraShip_CELL_TILES:
	.incbin "games/tla/src/map/eastern_alhafra_ship/cell_tiles.delta1.lz"
	.balign 4, 0
.LField_EasternAlhafraShip_SHAPES:
	.incbin "games/tla/src/map/eastern_alhafra_ship/shapes.table.lz"
	.balign 4, 0
.LField_EasternAlhafraShip_GRID:
	.incbin "games/tla/src/map/eastern_alhafra_ship/map.grid.lz"
	.balign 4, 0
.LField_EasternAlhafraShip_SHAPE_GRID:
	.incbin "games/tla/src/map/eastern_alhafra_ship/shape_grid.plane.lz"
	.balign 4, 0
.LField_EasternAlhafraShip_ANIMATION:
	.incbin "games/tla/src/map/kandorean_temple_mimic/animation.script.lz"
	.balign 4, 0
.LField_EasternAlhafraShip_BLEND:
	.incbin "games/tla/src/map/daila_rooms_houses/blend.script.lz"
	.balign 4, 0
.LField_EasternAlhafraShip_SCENE:
	.incbin "games/tla/src/map/eastern_alhafra_ship/scene.scene.lz"
	.balign 4, 0
	.global Tileset_EasternAlhafraShipPalette
Tileset_EasternAlhafraShipPalette:
	.incbin "games/tla/src/graphics/tileset/eastern_alhafra_ship/palette.gbapal.lz"
	.global Tileset_EasternAlhafraShipTilesA
Tileset_EasternAlhafraShipTilesA:
	.incbin "games/tla/src/graphics/tileset/eastern_alhafra_ship/tiles_a.4bpp.mtf"
	.byte 0xa0, 0x32, 0x14	@ PACKER: left in the buffer by Tileset_EasternAlhafraTilesB+0x245d
	.global Tileset_EasternAlhafraShipTilesB
Tileset_EasternAlhafraShipTilesB:
	.incbin "games/tla/src/graphics/tileset/eastern_alhafra_ship/tiles_b.4bpp.mtf"
	.byte 0x9a, 0xa6, 0xe3	@ PACKER: left in the buffer by Tileset_EasternAlhafraShipTilesA+0x1ce9
	.global Tileset_EasternAlhafraShipTilesC
Tileset_EasternAlhafraShipTilesC:
	.incbin "games/tla/src/graphics/tileset/eastern_alhafra_ship/tiles_c.4bpp.mtf"
	.byte 0x88, 0xf0, 0x05	@ PACKER: left in the buffer by Tileset_EasternAlhafraShipTilesB+0x16fd
	.global Tileset_EasternAlhafraShipTilesD
Tileset_EasternAlhafraShipTilesD:
	.incbin "games/tla/src/graphics/tileset/eastern_alhafra_ship/tiles_d.4bpp.mtf"
	.byte 0xf6, 0x81, 0x3a	@ PACKER: left in the buffer by Tileset_EasternAlhafraShipTilesC+0x1579
