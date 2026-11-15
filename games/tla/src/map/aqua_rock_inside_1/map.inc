	.global Field_AquaRockInside1
Field_AquaRockInside1:
	map_header 0, 0, 130, 112, 3, 2, 2, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_AquaRockInside1, .LField_AquaRockInside1_CELL_PICTURES
	map_block Field_AquaRockInside1, .LField_AquaRockInside1_SHAPES
	map_block Field_AquaRockInside1, .LField_AquaRockInside1_GRID
	map_block Field_AquaRockInside1, .LField_AquaRockInside1_SHAPE_GRID
	map_block Field_AquaRockInside1, .LField_AquaRockInside1_ANIMATION
	map_block Field_AquaRockInside1, .LField_AquaRockInside1_BLEND
	map_block Field_AquaRockInside1, .LField_AquaRockInside1_SCENE
.LField_AquaRockInside1_CELL_PICTURES:
	.incbin "games/tla/src/map/aqua_rock_inside_1/cell_pictures.delta2.lz"
	.balign 4, 0
.LField_AquaRockInside1_SHAPES:
	.incbin "games/tla/src/map/aqua_rock_inside_1/shapes.table.lz"
	.balign 4, 0
.LField_AquaRockInside1_GRID:
	.incbin "games/tla/src/map/aqua_rock_inside_1/map.grid.lz"
	.balign 4, 0
.LField_AquaRockInside1_SHAPE_GRID:
	.incbin "games/tla/src/map/aqua_rock_inside_1/shape_grid.plane.lz"
	.balign 4, 0
.LField_AquaRockInside1_ANIMATION:
	.incbin "games/tla/src/map/aqua_rock_inside_1/animation.script.lz"
	.balign 4, 0
.LField_AquaRockInside1_BLEND:
	.incbin "games/tla/src/map/aqua_rock_inside_1/blend.script.lz"
	.balign 4, 0
.LField_AquaRockInside1_SCENE:
	.incbin "games/tla/src/map/aqua_rock_inside_1/scene.scene.lz"
	.balign 4, 0
	.global Tileset_AquaRockInside1Palette
Tileset_AquaRockInside1Palette:
	.incbin "games/tla/src/graphics/tileset/aqua_rock_inside_1/palette.gbapal.lz"
	.global Tileset_AquaRockInside1TilesA
Tileset_AquaRockInside1TilesA:
	.incbin "games/tla/src/graphics/tileset/aqua_rock_inside_1/tiles_a.4bpp.mtf"
	.byte 0xbd	@ PACKER: left in the buffer by Field_AquaRock2+0x217b
	.global Tileset_AquaRockInside1TilesB
Tileset_AquaRockInside1TilesB:
	.incbin "games/tla/src/graphics/tileset/aqua_rock_inside_1/tiles_b.4bpp.mtf"
	.byte 0x3a	@ PACKER: left in the buffer by Tileset_AquaRockInside1TilesA+0x1be3
	.global Tileset_AquaRockInside1TilesC
Tileset_AquaRockInside1TilesC:
	.incbin "games/tla/src/graphics/tileset/aqua_rock_inside_1/tiles_c.4bpp.mtf"
	.byte 0x05, 0xb0, 0x73	@ PACKER: left in the buffer by Tileset_AquaRockInside1TilesA+0x1cf5
	.global Tileset_AquaRockInside1TilesD
Tileset_AquaRockInside1TilesD:
	.incbin "games/tla/src/graphics/tileset/aqua_rock_inside_1/tiles_d.4bpp.mtf"
