	.global Field_MarsLighthouseMercuryLedge
Field_MarsLighthouseMercuryLedge:
	map_header 0, 0, 204, 64, 3, 3, 2, 0, 2, 2, 0, 0
	map_layer 0, 0, 12, 12, -2, -2, 0xf, 0xf
	map_layer 0, 64, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_MarsLighthouseMercuryLedge, .LField_MarsLighthouseMercuryLedge_CELL_TILES
	map_block Field_MarsLighthouseMercuryLedge, .LField_MarsLighthouseMercuryLedge_SHAPES
	map_block Field_MarsLighthouseMercuryLedge, .LField_MarsLighthouseMercuryLedge_GRID
	map_block Field_MarsLighthouseMercuryLedge, .LField_MarsLighthouseMercuryLedge_SHAPE_GRID
	map_block Field_MarsLighthouseMercuryLedge, .LField_MarsLighthouseMercuryLedge_ANIMATION
	map_block Field_MarsLighthouseMercuryLedge
	map_block Field_MarsLighthouseMercuryLedge, .LField_MarsLighthouseMercuryLedge_SCENE
.LField_MarsLighthouseMercuryLedge_CELL_TILES:
	.incbin "games/tla/src/map/mars_lighthouse_mercury_ledge/cell_tiles.delta1.lz"
	.balign 4, 0
.LField_MarsLighthouseMercuryLedge_SHAPES:
	.incbin "games/tla/src/map/mars_lighthouse_mercury_ledge/shapes.table.lz"
	.balign 4, 0
.LField_MarsLighthouseMercuryLedge_GRID:
	.incbin "games/tla/src/map/mars_lighthouse_mercury_ledge/map.grid.lz"
	.balign 4, 0
.LField_MarsLighthouseMercuryLedge_SHAPE_GRID:
	.incbin "games/tla/src/map/mars_lighthouse_mercury_ledge/shape_grid.plane.lz"
	.balign 4, 0
.LField_MarsLighthouseMercuryLedge_ANIMATION:
	.incbin "games/tla/src/map/mars_lighthouse_aerie/animation.script.lz"
	.balign 4, 0
.LField_MarsLighthouseMercuryLedge_SCENE:
	.incbin "games/tla/src/map/mars_lighthouse_mercury_ledge/scene.scene.lz"
	.global Tileset_MarsLighthouseMercuryLedgeTilesD
Tileset_MarsLighthouseMercuryLedgeTilesD:
	.incbin "games/tla/src/graphics/tileset/mars_lighthouse_mercury_ledge/tiles_d.4bpp.mtf"
	.byte 0x02, 0x19, 0xdd	@ PACKER: left in the buffer by Field_MarsLighthouseMercuryLedge+0x7cd
