	.global Field_SuhallaGateRoad
Field_SuhallaGateRoad:
	map_header 0, 0, 128, 128, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_SuhallaGateRoad, .LField_SuhallaGateRoad_CELL_TILES
	map_block Field_SuhallaGateRoad, .LField_SuhallaGateRoad_SHAPES
	map_block Field_SuhallaGateRoad, .LField_SuhallaGateRoad_GRID
	map_block Field_SuhallaGateRoad, .LField_SuhallaGateRoad_SHAPE_GRID
	map_block Field_SuhallaGateRoad, .LField_SuhallaGateRoad_ANIMATION
	map_block Field_SuhallaGateRoad, .LField_SuhallaGateRoad_BLEND
	map_block Field_SuhallaGateRoad, .LField_SuhallaGateRoad_SCENE
.LField_SuhallaGateRoad_CELL_TILES:
	.incbin "games/tla/src/map/suhalla_gate_road/cell_tiles.delta0.lz"
	.balign 4, 0
.LField_SuhallaGateRoad_SHAPES:
	.incbin "games/tla/src/map/suhalla_gate_road/shapes.table.lz"
	.balign 4, 0
.LField_SuhallaGateRoad_GRID:
	.incbin "games/tla/src/map/suhalla_gate_road/map.grid.lz"
	.balign 4, 0
.LField_SuhallaGateRoad_SHAPE_GRID:
	.incbin "games/tla/src/map/suhalla_gate_road/shape_grid.plane.lz"
	.balign 4, 0
.LField_SuhallaGateRoad_ANIMATION:
	.incbin "games/tla/src/map/suhalla_gate_road/animation.script.lz"
	.balign 4, 0
.LField_SuhallaGateRoad_BLEND:
	.incbin "games/tla/src/map/lobby/blend.script.lz"
	.balign 4, 0
.LField_SuhallaGateRoad_SCENE:
	.incbin "games/tla/src/map/empty_map/scene.scene.lz"
	.balign 4, 0
	.global Tileset_SuhallaGateRoadPalette
Tileset_SuhallaGateRoadPalette:
	.incbin "games/tla/src/graphics/tileset/suhalla_gate_road/palette.gbapal.lz"
	.byte 0x71, 0xc0, 0x05	@ PACKER: left in the buffer by Field_SuhallaGate1+0x171
	.global Tileset_SuhallaGateRoadTilesA
Tileset_SuhallaGateRoadTilesA:
	.incbin "games/tla/src/graphics/tileset/suhalla_gate_road/tiles_a.4bpp.mtf"
	.byte 0x68, 0x3d, 0x1a	@ PACKER: left in the buffer by Field_VenusLighthouseEntrance+0x2de1
	.global Tileset_SuhallaGateRoadTilesB
Tileset_SuhallaGateRoadTilesB:
	.incbin "games/tla/src/graphics/tileset/suhalla_gate_road/tiles_b.4bpp.mtf"
	.byte 0x73	@ PACKER: left in the buffer by Tileset_SuhallaGateRoadTilesA+0x28a7
	.global Tileset_SuhallaGateRoadTilesC
Tileset_SuhallaGateRoadTilesC:
	.incbin "games/tla/src/graphics/tileset/suhalla_gate_road/tiles_c.4bpp.mtf"
	.global Tileset_SuhallaGateRoadTilesD
Tileset_SuhallaGateRoadTilesD:
	.incbin "games/tla/src/graphics/tileset/suhalla_gate_road/tiles_d.4bpp.mtf"
	.byte 0x82, 0x65	@ PACKER: left in the buffer by Tileset_SuhallaGateRoadTilesC+0x1b3a
