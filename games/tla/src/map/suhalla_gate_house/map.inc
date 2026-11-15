	.global Field_SuhallaGateHouse
Field_SuhallaGateHouse:
	map_header 0, 0, 128, 128, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_SuhallaGateHouse, .LField_SuhallaGateHouse_CELL_PICTURES
	map_block Field_SuhallaGateHouse, .LField_SuhallaGateHouse_SHAPES
	map_block Field_SuhallaGateHouse, .LField_SuhallaGateHouse_GRID
	map_block Field_SuhallaGateHouse, .LField_SuhallaGateHouse_SHAPE_GRID
	map_block Field_SuhallaGateHouse, .LField_SuhallaGateHouse_ANIMATION
	map_block Field_SuhallaGateHouse, .LField_SuhallaGateHouse_BLEND
	map_block Field_SuhallaGateHouse, .LField_SuhallaGateHouse_SCENE
.LField_SuhallaGateHouse_CELL_PICTURES:
	.incbin "games/tla/src/map/suhalla_gate_house/cell_pictures.delta0.lz"
	.balign 4, 0
.LField_SuhallaGateHouse_SHAPES:
	.incbin "games/tla/src/map/suhalla_gate_house/shapes.table.lz"
	.balign 4, 0
.LField_SuhallaGateHouse_GRID:
	.incbin "games/tla/src/map/suhalla_gate_house/map.grid.lz"
	.balign 4, 0
.LField_SuhallaGateHouse_SHAPE_GRID:
	.incbin "games/tla/src/map/suhalla_gate_house/shape_grid.plane.lz"
	.balign 4, 0
.LField_SuhallaGateHouse_ANIMATION:
	.incbin "games/tla/src/map/suhalla_gate_house/animation.script.lz"
	.balign 4, 0
.LField_SuhallaGateHouse_BLEND:
	.incbin "games/tla/src/map/lobby/blend.script.lz"
	.balign 4, 0
.LField_SuhallaGateHouse_SCENE:
	.incbin "games/tla/src/map/map_000/scene.scene.lz"
	.balign 4, 0
	.global Tileset_SuhallaGateHousePalette
Tileset_SuhallaGateHousePalette:
	.incbin "games/tla/src/graphics/tileset/suhalla_gate_house/pal_41.gbapal.lz"
	.byte 0x71, 0xc0, 0x05	@ PACKER: left in the buffer by Field_SuhallaGate1+0x171
	.global Tileset_SuhallaGateHouseTilesA
Tileset_SuhallaGateHouseTilesA:
	.incbin "games/tla/src/graphics/tileset/suhalla_gate_house/tiles_a.4bpp.mtf"
	.byte 0x68, 0x3d, 0x1a	@ PACKER: left in the buffer by Field_VenusLighthouseEntrance+0x2de1
	.global Tileset_SuhallaGateHouseTilesB
Tileset_SuhallaGateHouseTilesB:
	.incbin "games/tla/src/graphics/tileset/suhalla_gate_house/tiles_b.4bpp.mtf"
	.byte 0x73	@ PACKER: left in the buffer by Tileset_SuhallaGateHouseTilesA+0x28a7
	.global Tileset_SuhallaGateHouseTilesC
Tileset_SuhallaGateHouseTilesC:
	.incbin "games/tla/src/graphics/tileset/suhalla_gate_house/tiles_c.4bpp.mtf"
	.global Tileset_SuhallaGateHouseTilesD
Tileset_SuhallaGateHouseTilesD:
	.incbin "games/tla/src/graphics/tileset/suhalla_gate_house/tiles_d.4bpp.mtf"
	.byte 0x82, 0x65	@ PACKER: left in the buffer by Tileset_SuhallaGateHouseTilesC+0x1b3a
