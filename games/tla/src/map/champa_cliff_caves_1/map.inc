	.global Field_ChampaCliffCaves1
Field_ChampaCliffCaves1:
	map_header 0, 0, 128, 128, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_ChampaCliffCaves1, .LField_ChampaCliffCaves1_CELL_PICTURES
	map_block Field_ChampaCliffCaves1, .LField_ChampaCliffCaves1_SHAPES
	map_block Field_ChampaCliffCaves1, .LField_ChampaCliffCaves1_GRID
	map_block Field_ChampaCliffCaves1, .LField_ChampaCliffCaves1_SHAPE_GRID
	map_block Field_ChampaCliffCaves1, .LField_ChampaCliffCaves1_ANIMATION
	map_block Field_ChampaCliffCaves1, .LField_ChampaCliffCaves1_BLEND
	map_block Field_ChampaCliffCaves1, .LField_ChampaCliffCaves1_SCENE
.LField_ChampaCliffCaves1_CELL_PICTURES:
	.incbin "games/tla/src/map/champa_cliff_caves_1/cell_pictures.delta1.lz"
	.balign 4, 0
.LField_ChampaCliffCaves1_SHAPES:
	.incbin "games/tla/src/map/champa_cliff_caves_1/shapes.table.lz"
	.balign 4, 0
.LField_ChampaCliffCaves1_GRID:
	.incbin "games/tla/src/map/champa_cliff_caves_1/map.grid.lz"
	.balign 4, 0
.LField_ChampaCliffCaves1_SHAPE_GRID:
	.incbin "games/tla/src/map/champa_cliff_caves_1/shape_grid.plane.lz"
	.balign 4, 0
.LField_ChampaCliffCaves1_ANIMATION:
	.incbin "games/tla/src/map/champa_cliff_caves_1/animation.script.lz"
	.balign 4, 0
.LField_ChampaCliffCaves1_BLEND:
	.incbin "games/tla/src/map/garoh_4/blend.script.lz"
	.balign 4, 0
.LField_ChampaCliffCaves1_SCENE:
	.incbin "games/tla/src/map/champa_cliff_caves_1/scene.scene.lz"
	.balign 4, 0
	.global Tileset_ChampaCliffCaves1Palette
Tileset_ChampaCliffCaves1Palette:
	.incbin "games/tla/src/graphics/tileset/champa_cliff_caves_1/palette.gbapal.lz"
	.byte 0x78	@ PACKER: left in the buffer by Field_ChampaCliffCaves1+0x15f
	.global Tileset_ChampaCliffCaves1TilesA
Tileset_ChampaCliffCaves1TilesA:
	.incbin "games/tla/src/graphics/tileset/champa_cliff_caves_1/tiles_a.4bpp.mtf"
	.byte 0xe6	@ PACKER: left in the buffer by Field_AquaRock1+0x2aa3
	.global Tileset_ChampaCliffCaves1TilesB
Tileset_ChampaCliffCaves1TilesB:
	.incbin "games/tla/src/graphics/tileset/champa_cliff_caves_1/tiles_b.4bpp.mtf"
	.byte 0xc7, 0x69, 0x1f	@ PACKER: left in the buffer by Tileset_ChampaCliffCaves1TilesA+0x21ad
	.global Tileset_ChampaCliffCaves1TilesC
Tileset_ChampaCliffCaves1TilesC:
	.incbin "games/tla/src/graphics/tileset/champa_cliff_caves_1/tiles_c.4bpp.mtf"
	.byte 0x92, 0xe5	@ PACKER: left in the buffer by Tileset_ChampaCliffCaves1TilesB+0x1e76
	.global Tileset_ChampaCliffCaves1TilesD
Tileset_ChampaCliffCaves1TilesD:
	.incbin "games/tla/src/graphics/tileset/champa_cliff_caves_1/tiles_d.4bpp.mtf"
	.byte 0x11	@ PACKER: left in the buffer by Tileset_ChampaCliffCaves1TilesC+0x13f
