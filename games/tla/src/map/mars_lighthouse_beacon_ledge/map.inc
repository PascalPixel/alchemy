	.global Field_MarsLighthouseBeaconLedge
Field_MarsLighthouseBeaconLedge:
	map_header 0, 0, 128, 128, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 12, 12, -2, -2, 0x1f, 0x1f
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_MarsLighthouseBeaconLedge, .LField_MarsLighthouseBeaconLedge_CELL_TILES
	map_block Field_MarsLighthouseBeaconLedge, .LField_MarsLighthouseBeaconLedge_SHAPES
	map_block Field_MarsLighthouseBeaconLedge, .LField_MarsLighthouseBeaconLedge_GRID
	map_block Field_MarsLighthouseBeaconLedge, .LField_MarsLighthouseBeaconLedge_SHAPE_GRID
	map_block Field_MarsLighthouseBeaconLedge, .LField_MarsLighthouseBeaconLedge_ANIMATION
	map_block Field_MarsLighthouseBeaconLedge
	map_block Field_MarsLighthouseBeaconLedge, .LField_MarsLighthouseBeaconLedge_SCENE
.LField_MarsLighthouseBeaconLedge_CELL_TILES:
	.incbin "games/tla/src/map/mars_lighthouse_beacon_ledge/cell_tiles.delta2.lz"
	.balign 4, 0
.LField_MarsLighthouseBeaconLedge_SHAPES:
	.incbin "games/tla/src/map/mars_lighthouse_beacon_ledge/shapes.table.lz"
	.balign 4, 0
.LField_MarsLighthouseBeaconLedge_GRID:
	.incbin "games/tla/src/map/mars_lighthouse_beacon_ledge/map.grid.lz"
	.balign 4, 0
.LField_MarsLighthouseBeaconLedge_SHAPE_GRID:
	.incbin "games/tla/src/map/mars_lighthouse_beacon_ledge/shape_grid.plane.lz"
	.balign 4, 0
.LField_MarsLighthouseBeaconLedge_ANIMATION:
	.incbin "games/tla/src/map/mars_lighthouse_beacon_ledge/animation.script.lz"
	.balign 4, 0
.LField_MarsLighthouseBeaconLedge_SCENE:
	.incbin "games/tla/src/map/mars_lighthouse_beacon_ledge/scene.scene.lz"
	.balign 4, 0
	.global Tileset_MarsLighthouseBeaconLedgeTilesD
Tileset_MarsLighthouseBeaconLedgeTilesD:
	.incbin "games/tla/src/graphics/tileset/mars_lighthouse_beacon_ledge/tiles_d.4bpp.mtf"
	.byte 0xff, 0x00	@ PACKER: left in the buffer by Field_MarsLighthouseBeaconLedge+0xac2
