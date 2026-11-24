	.global Field_ProxSouthGate
Field_ProxSouthGate:
	map_header 0, 0, 38, 50, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 38, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 50, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_ProxSouthGate, .LField_ProxSouthGate_CELL_TILES
	map_block Field_ProxSouthGate, .LField_ProxSouthGate_SHAPES
	map_block Field_ProxSouthGate, .LField_ProxSouthGate_GRID
	map_block Field_ProxSouthGate, .LField_ProxSouthGate_SHAPE_GRID
	map_block Field_ProxSouthGate, .LField_ProxSouthGate_ANIMATION
	map_block Field_ProxSouthGate, .LField_ProxSouthGate_BLEND
	map_block Field_ProxSouthGate, .LField_ProxSouthGate_SCENE
.LField_ProxSouthGate_CELL_TILES:
	.incbin "games/tla/src/map/prox_south_gate/cell_tiles.delta1.lz"
	.balign 4, 0
.LField_ProxSouthGate_SHAPES:
	.incbin "games/tla/src/map/prox_south_gate/shapes.table.lz"
	.balign 4, 0
.LField_ProxSouthGate_GRID:
	.incbin "games/tla/src/map/prox_south_gate/map.grid.lz"
	.balign 4, 0
.LField_ProxSouthGate_SHAPE_GRID:
	.incbin "games/tla/src/map/prox_south_gate/shape_grid.plane.lz"
	.balign 4, 0
.LField_ProxSouthGate_ANIMATION:
	.incbin "games/tla/src/map/prox_south_gate/animation.script.lz"
	.balign 4, 0
.LField_ProxSouthGate_BLEND:
	.incbin "games/tla/src/map/lobby/blend.script.lz"
	.balign 4, 0
.LField_ProxSouthGate_SCENE:
	.incbin "games/tla/src/map/prox_south_gate/scene.scene.lz"
	.balign 4, 0
	.global Tileset_ProxSouthGatePalette
Tileset_ProxSouthGatePalette:
	.incbin "games/tla/src/graphics/tileset/prox_south_gate/palette.gbapal.lz"
	.byte 0x18	@ PACKER: left in the buffer by Field_ProxSouthGate+0x15f
	.global Tileset_ProxSouthGateTilesA
Tileset_ProxSouthGateTilesA:
	.incbin "games/tla/src/graphics/tileset/prox_south_gate/tiles_a.4bpp.mtf"
	.global Tileset_ProxSouthGateTilesB
Tileset_ProxSouthGateTilesB:
	.incbin "games/tla/src/graphics/tileset/prox_south_gate/tiles_b.4bpp.mtf"
	.byte 0xf6	@ PACKER: left in the buffer by Tileset_ProxSouthGateTilesA+0x2173
	.global Tileset_ProxSouthGateTilesC
Tileset_ProxSouthGateTilesC:
	.incbin "games/tla/src/graphics/tileset/prox_south_gate/tiles_c.4bpp.mtf"
	.byte 0x65, 0xab, 0x31	@ PACKER: left in the buffer by Tileset_ProxSouthGateTilesA+0x26bd
	.global Tileset_ProxSouthGateTilesD
Tileset_ProxSouthGateTilesD:
	.incbin "games/tla/src/graphics/tileset/prox_south_gate/tiles_d.4bpp.mtf"
	.byte 0xe1	@ PACKER: left in the buffer by Tileset_ProxSouthGateTilesC+0x5db
