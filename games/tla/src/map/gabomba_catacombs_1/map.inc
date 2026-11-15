	.global Field_GabombaCatacombs1
Field_GabombaCatacombs1:
	map_header 0, 0, 90, 100, 3, 3, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 100, 16, 16, 0, 0, 0xff, 0xff
	map_layer 100, 0, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_GabombaCatacombs1, .LField_GabombaCatacombs1_CELL_PICTURES
	map_block Field_GabombaCatacombs1, .LField_GabombaCatacombs1_SHAPES
	map_block Field_GabombaCatacombs1, .LField_GabombaCatacombs1_GRID
	map_block Field_GabombaCatacombs1, .LField_GabombaCatacombs1_SHAPE_GRID
	map_block Field_GabombaCatacombs1, .LField_GabombaCatacombs1_ANIMATION
	map_block Field_GabombaCatacombs1, .LField_GabombaCatacombs1_BLEND
	map_block Field_GabombaCatacombs1, .LField_GabombaCatacombs1_SCENE
.LField_GabombaCatacombs1_CELL_PICTURES:
	.incbin "games/tla/src/map/gabomba_catacombs_1/cell_pictures.delta1.lz"
	.balign 4, 0
.LField_GabombaCatacombs1_SHAPES:
	.incbin "games/tla/src/map/gabomba_catacombs_1/shapes.table.lz"
	.balign 4, 0
.LField_GabombaCatacombs1_GRID:
	.incbin "games/tla/src/map/gabomba_catacombs_1/map.grid.lz"
	.balign 4, 0
.LField_GabombaCatacombs1_SHAPE_GRID:
	.incbin "games/tla/src/map/gabomba_catacombs_1/shape_grid.plane.lz"
	.balign 4, 0
.LField_GabombaCatacombs1_ANIMATION:
	.incbin "games/tla/src/map/shrine_of_the_sea_god_8/animation.script.lz"
	.balign 4, 0
.LField_GabombaCatacombs1_BLEND:
	.incbin "games/tla/src/map/lobby/blend.script.lz"
	.balign 4, 0
.LField_GabombaCatacombs1_SCENE:
	.incbin "games/tla/src/map/gabomba_catacombs_1/scene.scene.lz"
	.balign 4, 0
	.global Tileset_GabombaCatacombs1Palette
Tileset_GabombaCatacombs1Palette:
	.incbin "games/tla/src/graphics/tileset/gabomba_catacombs_1/palette.gbapal.lz"
	.byte 0xd5, 0x1b	@ PACKER: left in the buffer by Field_GabombaCatacombs1+0x1a2
	.global Tileset_GabombaCatacombs1TilesA
Tileset_GabombaCatacombs1TilesA:
	.incbin "games/tla/src/graphics/tileset/gabomba_catacombs_1/tiles_a.4bpp.mtf"
	.byte 0x77, 0x46	@ PACKER: left in the buffer by Field_KibomboMountainsWaft+0x2ec2
	.global Tileset_GabombaCatacombs1TilesB
Tileset_GabombaCatacombs1TilesB:
	.incbin "games/tla/src/graphics/tileset/gabomba_catacombs_1/tiles_b.4bpp.mtf"
	.byte 0x4d, 0xd0, 0x25	@ PACKER: left in the buffer by Tileset_GabombaCatacombs1TilesA+0x197d
	.global Tileset_GabombaCatacombs1TilesC
Tileset_GabombaCatacombs1TilesC:
	.incbin "games/tla/src/graphics/tileset/gabomba_catacombs_1/tiles_c.4bpp.mtf"
	.byte 0x38, 0xa4	@ PACKER: left in the buffer by Tileset_GabombaCatacombs1TilesB+0x104a
	.global Tileset_GabombaCatacombs1TilesD
Tileset_GabombaCatacombs1TilesD:
	.incbin "games/tla/src/graphics/tileset/gabomba_catacombs_1/tiles_d.4bpp.mtf"
