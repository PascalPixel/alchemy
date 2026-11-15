	.global Field_SuhallaGate3
Field_SuhallaGate3:
	map_header 0, 0, 94, 94, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 96, 16, 16, 0, 0, 0xff, 0xff
	map_layer 100, 0, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_SuhallaGate3, .LField_SuhallaGate3_CELL_PICTURES
	map_block Field_SuhallaGate3, .LField_SuhallaGate3_SHAPES
	map_block Field_SuhallaGate3, .LField_SuhallaGate3_GRID
	map_block Field_SuhallaGate3, .LField_SuhallaGate3_SHAPE_GRID
	map_block Field_SuhallaGate3, .LField_SuhallaGate3_ANIMATION
	map_block Field_SuhallaGate3, .LField_SuhallaGate3_BLEND
	map_block Field_SuhallaGate3, .LField_SuhallaGate3_SCENE
.LField_SuhallaGate3_CELL_PICTURES:
	.incbin "games/tla/src/map/suhalla_gate_3/cell_pictures.delta1.lz"
	.balign 4, 0
.LField_SuhallaGate3_SHAPES:
	.incbin "games/tla/src/map/suhalla_gate_3/shapes.table.lz"
	.balign 4, 0
.LField_SuhallaGate3_GRID:
	.incbin "games/tla/src/map/suhalla_gate_3/map.grid.lz"
	.balign 4, 0
.LField_SuhallaGate3_SHAPE_GRID:
	.incbin "games/tla/src/map/suhalla_gate_3/shape_grid.plane.lz"
	.balign 4, 0
.LField_SuhallaGate3_ANIMATION:
	.incbin "games/tla/src/map/suhalla_gate_3/animation.script.lz"
	.balign 4, 0
.LField_SuhallaGate3_BLEND:
	.incbin "games/tla/src/map/suhalla_gate_3/blend.script.lz"
	.balign 4, 0
.LField_SuhallaGate3_SCENE:
	.incbin "games/tla/src/map/suhalla_gate_3/scene.scene.lz"
	.global Tileset_SuhallaGate3Palette
Tileset_SuhallaGate3Palette:
	.incbin "games/tla/src/graphics/tileset/suhalla_gate_3/pal_42.gbapal.lz"
	.byte 0x1f, 0x44	@ PACKER: left in the buffer by Field_SuhallaGate3+0x1b6
	.global Tileset_SuhallaGate3TilesA
Tileset_SuhallaGate3TilesA:
	.incbin "games/tla/src/graphics/tileset/suhalla_gate_3/tiles_a.4bpp.mtf"
	.byte 0xd1	@ PACKER: left in the buffer by Tileset_SuhallaGate1TilesA+0x2afb
	.global Tileset_SuhallaGate3TilesB
Tileset_SuhallaGate3TilesB:
	.incbin "games/tla/src/graphics/tileset/suhalla_gate_3/tiles_b.4bpp.mtf"
	.byte 0x00, 0xa0, 0xbb	@ PACKER: left in the buffer by Tileset_SuhallaGate3TilesA+0x20dd
	.global Tileset_SuhallaGate3TilesC
Tileset_SuhallaGate3TilesC:
	.incbin "games/tla/src/graphics/tileset/suhalla_gate_3/tiles_c.4bpp.mtf"
	.byte 0x27, 0xe9, 0x0b	@ PACKER: left in the buffer by Tileset_SuhallaGate3TilesA+0x2249
	.global Tileset_SuhallaGate3TilesD
Tileset_SuhallaGate3TilesD:
	.incbin "games/tla/src/graphics/tileset/suhalla_gate_3/tiles_d.4bpp.mtf"
	.byte 0xf0	@ PACKER: left in the buffer by Tileset_SuhallaGate3TilesC+0x13f
