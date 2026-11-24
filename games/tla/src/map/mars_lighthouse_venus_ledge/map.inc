	.global Field_MarsLighthouseVenusLedge
Field_MarsLighthouseVenusLedge:
	map_header 0, 0, 204, 80, 3, 3, 2, 0, 2, 2, 0, 0
	map_layer 0, 0, 12, 12, -2, -2, 0xf, 0xf
	map_layer 0, 80, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 160, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_MarsLighthouseVenusLedge, .LField_MarsLighthouseVenusLedge_CELL_TILES
	map_block Field_MarsLighthouseVenusLedge, .LField_MarsLighthouseVenusLedge_SHAPES
	map_block Field_MarsLighthouseVenusLedge, .LField_MarsLighthouseVenusLedge_GRID
	map_block Field_MarsLighthouseVenusLedge, .LField_MarsLighthouseVenusLedge_SHAPE_GRID
	map_block Field_MarsLighthouseVenusLedge, .LField_MarsLighthouseVenusLedge_ANIMATION
	map_block Field_MarsLighthouseVenusLedge
	map_block Field_MarsLighthouseVenusLedge, .LField_MarsLighthouseVenusLedge_SCENE
.LField_MarsLighthouseVenusLedge_CELL_TILES:
	.incbin "games/tla/src/map/mars_lighthouse_venus_ledge/cell_tiles.delta1.lz"
	.balign 4, 0
.LField_MarsLighthouseVenusLedge_SHAPES:
	.incbin "games/tla/src/map/mars_lighthouse_venus_ledge/shapes.table.lz"
	.balign 4, 0
.LField_MarsLighthouseVenusLedge_GRID:
	.incbin "games/tla/src/map/mars_lighthouse_venus_ledge/map.grid.lz"
	.balign 4, 0
.LField_MarsLighthouseVenusLedge_SHAPE_GRID:
	.incbin "games/tla/src/map/mars_lighthouse_venus_ledge/shape_grid.plane.lz"
	.balign 4, 0
.LField_MarsLighthouseVenusLedge_ANIMATION:
	.incbin "games/tla/src/map/mars_lighthouse_aerie/animation.script.lz"
	.balign 4, 0
.LField_MarsLighthouseVenusLedge_SCENE:
	.incbin "games/tla/src/map/mars_lighthouse_venus_ledge/scene.scene.lz"
	.global Tileset_PaletteMarsLighthouseOutsideThawed
Tileset_PaletteMarsLighthouseOutsideThawed:
	.incbin "games/tla/src/graphics/tileset/palettes/mars_lighthouse_outside_thawed.gbapal.lz"
	.byte 0x95, 0xe2, 0xb5	@ PACKER: left in the buffer by Field_MarsLighthouseVenusLedge+0x129
	.global Tileset_PaletteMarsLighthouseMarsTowerThawed
Tileset_PaletteMarsLighthouseMarsTowerThawed:
	.incbin "games/tla/src/graphics/tileset/palettes/mars_lighthouse_mars_tower_thawed.gbapal.lz"
	.byte 0x81	@ PACKER: left in the buffer by Field_MarsLighthouseVenusLedge+0x177
	.global Tileset_PaletteMarsLighthouseAerieThawed
Tileset_PaletteMarsLighthouseAerieThawed:
	.incbin "games/tla/src/graphics/tileset/palettes/mars_lighthouse_aerie_thawed.gbapal.lz"
	.byte 0x00, 0x81	@ PACKER: left in the buffer by Tileset_PaletteMarsLighthouseMarsTowerThawed+0x176
	.global Tileset_PaletteMarsLighthouseBeaconLedgeThawed
Tileset_PaletteMarsLighthouseBeaconLedgeThawed:
	.incbin "games/tla/src/graphics/tileset/palettes/mars_lighthouse_beacon_ledge_thawed.gbapal.lz"
	.balign 4, 0	@ PACKER: zero fill, not an earlier stream's bytes
	.global Tileset_PaletteMarsLighthouseHallThawed
Tileset_PaletteMarsLighthouseHallThawed:
	.incbin "games/tla/src/graphics/tileset/palettes/mars_lighthouse_hall_thawed.gbapal.lz"
	.byte 0x17	@ PACKER: left in the buffer by Tileset_PaletteMarsLighthouseBeaconLedgeThawed+0x18f
