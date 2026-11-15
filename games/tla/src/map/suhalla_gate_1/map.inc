	.global Field_SuhallaGate1
Field_SuhallaGate1:
	map_header 0, 0, 128, 128, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_SuhallaGate1, .LField_SuhallaGate1_CELL_PICTURES
	map_block Field_SuhallaGate1, .LField_SuhallaGate1_SHAPES
	map_block Field_SuhallaGate1, .LField_SuhallaGate1_GRID
	map_block Field_SuhallaGate1, .LField_SuhallaGate1_SHAPE_GRID
	map_block Field_SuhallaGate1, .LField_SuhallaGate1_ANIMATION
	map_block Field_SuhallaGate1, .LField_SuhallaGate1_BLEND
	map_block Field_SuhallaGate1, .LField_SuhallaGate1_SCENE
.LField_SuhallaGate1_CELL_PICTURES:
	.incbin "games/tla/src/map/suhalla_gate_1/cell_pictures.delta0.lz"
	.balign 4, 0
.LField_SuhallaGate1_SHAPES:
	.incbin "games/tla/src/map/suhalla_gate_1/shapes.table.lz"
	.balign 4, 0
.LField_SuhallaGate1_GRID:
	.incbin "games/tla/src/map/suhalla_gate_1/map.grid.lz"
	.balign 4, 0
.LField_SuhallaGate1_SHAPE_GRID:
	.incbin "games/tla/src/map/suhalla_gate_1/shape_grid.plane.lz"
	.balign 4, 0
.LField_SuhallaGate1_ANIMATION:
	.incbin "games/tla/src/map/suhalla_gate_1/animation.script.lz"
	.balign 4, 0
.LField_SuhallaGate1_BLEND:
	.incbin "games/tla/src/map/lobby/blend.script.lz"
	.balign 4, 0
.LField_SuhallaGate1_SCENE:
	.incbin "games/tla/src/map/map_000/scene.scene.lz"
	.balign 4, 0
	.global Tileset_SuhallaGate1Palette
Tileset_SuhallaGate1Palette:
	.incbin "games/tla/src/graphics/tileset/suhalla_gate_1/pal_41.gbapal.lz"
	.byte 0x71, 0xc0, 0x05	@ PACKER: left in the buffer by Field_SuhallaGate1+0x171
	.global Tileset_SuhallaGate1TilesA
Tileset_SuhallaGate1TilesA:
	.incbin "games/tla/src/graphics/tileset/suhalla_gate_1/tiles_a.4bpp.mtf"
	.byte 0x68, 0x3d, 0x1a	@ PACKER: left in the buffer by Field_VenusLighthouseEntrance+0x2de1
	.global Tileset_SuhallaGate1TilesB
Tileset_SuhallaGate1TilesB:
	.incbin "games/tla/src/graphics/tileset/suhalla_gate_1/tiles_b.4bpp.mtf"
	.byte 0x73	@ PACKER: left in the buffer by Tileset_SuhallaGate1TilesA+0x28a7
	.global Tileset_SuhallaGate1TilesC
Tileset_SuhallaGate1TilesC:
	.incbin "games/tla/src/graphics/tileset/suhalla_gate_1/tiles_c.4bpp.mtf"
	.global Tileset_SuhallaGate1TilesD
Tileset_SuhallaGate1TilesD:
	.incbin "games/tla/src/graphics/tileset/suhalla_gate_1/tiles_d.4bpp.mtf"
	.byte 0x82, 0x65	@ PACKER: left in the buffer by Tileset_SuhallaGate1TilesC+0x1b3a
