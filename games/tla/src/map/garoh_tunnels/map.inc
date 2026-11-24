	.global Field_GarohTunnels
Field_GarohTunnels:
	map_header 0, 0, 128, 128, 3, 1, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_GarohTunnels, .LField_GarohTunnels_CELL_TILES
	map_block Field_GarohTunnels, .LField_GarohTunnels_SHAPES
	map_block Field_GarohTunnels, .LField_GarohTunnels_GRID
	map_block Field_GarohTunnels, .LField_GarohTunnels_SHAPE_GRID
	map_block Field_GarohTunnels, .LField_GarohTunnels_ANIMATION
	map_block Field_GarohTunnels, .LField_GarohTunnels_BLEND
	map_block Field_GarohTunnels, .LField_GarohTunnels_SCENE
.LField_GarohTunnels_CELL_TILES:
	.incbin "games/tla/src/map/garoh_tunnels/cell_tiles.delta1.lz"
	.balign 4, 0
.LField_GarohTunnels_SHAPES:
	.incbin "games/tla/src/map/garoh_tunnels/shapes.table.lz"
	.balign 4, 0
.LField_GarohTunnels_GRID:
	.incbin "games/tla/src/map/garoh_tunnels/map.grid.lz"
	.balign 4, 0
.LField_GarohTunnels_SHAPE_GRID:
	.incbin "games/tla/src/map/garoh_tunnels/shape_grid.plane.lz"
	.balign 4, 0
.LField_GarohTunnels_ANIMATION:
	.incbin "games/tla/src/map/kandorean_temple_mimic/animation.script.lz"
	.balign 4, 0
.LField_GarohTunnels_BLEND:
	.incbin "games/tla/src/map/garoh_tunnels/blend.script.lz"
	.balign 4, 0
.LField_GarohTunnels_SCENE:
	.incbin "games/tla/src/map/garoh_tunnels/scene.scene.lz"
	.balign 4, 0
	.global Tileset_PaletteGaroh4
Tileset_PaletteGaroh4:
	.incbin "games/tla/src/graphics/tileset/palettes/garoh_tunnels.gbapal.lz"
	.byte 0xf0	@ PACKER: left in the buffer by Field_GarohTunnels+0xf3
