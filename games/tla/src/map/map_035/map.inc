	.global Field_Map035
Field_Map035:
	map_header 0, 0, 128, 128, 3, 1, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map035, .LField_Map035_CELL_PICTURES
	map_block Field_Map035, .LField_Map035_SHAPES
	map_block Field_Map035, .LField_Map035_GRID
	map_block Field_Map035, .LField_Map035_SHAPE_GRID
	map_block Field_Map035, .LField_Map035_ANIMATION
	map_block Field_Map035, .LField_Map035_BLEND
	map_block Field_Map035, .LField_Map035_SCENE
.LField_Map035_CELL_PICTURES:
	.incbin "games/tla/src/map/map_035/cell_pictures.delta0.lz"
	.balign 4, 0
.LField_Map035_SHAPES:
	.incbin "games/tla/src/map/map_035/shapes.table.lz"
	.balign 4, 0
.LField_Map035_GRID:
	.incbin "games/tla/src/map/map_035/map.grid.lz"
	.balign 4, 0
.LField_Map035_SHAPE_GRID:
	.incbin "games/tla/src/map/map_035/shape_grid.plane.lz"
	.balign 4, 0
.LField_Map035_ANIMATION:
	.incbin "games/tla/src/map/map_035/animation.script.lz"
	.balign 4, 0
.LField_Map035_BLEND:
	.incbin "games/tla/src/map/map_035/blend.script.lz"
	.balign 4, 0
.LField_Map035_SCENE:
	.incbin "games/tla/src/map/map_035/scene.scene.lz"
	.balign 4, 0
	.global Tileset_Set18Palette
Tileset_Set18Palette:
	.incbin "games/tla/src/graphics/tileset/set_18/palette.gbapal.lz"
	.byte 0x99, 0xac	@ PACKER: left in the buffer by Field_Map035+0x10a
	.global Tileset_Set18TilesA
Tileset_Set18TilesA:
	.incbin "games/tla/src/graphics/tileset/set_18/tiles_a.4bpp.mtf"
	.byte 0xf8, 0x3e	@ PACKER: left in the buffer by Tileset_Set17TilesC+0x1b56
	.global Tileset_Set18TilesB
Tileset_Set18TilesB:
	.incbin "games/tla/src/graphics/tileset/set_18/tiles_b.4bpp.mtf"
	.global Tileset_Set18TilesC
Tileset_Set18TilesC:
	.incbin "games/tla/src/graphics/tileset/set_18/tiles_c.4bpp.mtf"
	.byte 0x83, 0xb4, 0x9c	@ PACKER: left in the buffer by Tileset_Set18TilesB+0x1f5
	.global Tileset_Set18TilesD
Tileset_Set18TilesD:
	.incbin "games/tla/src/graphics/tileset/set_18/tiles_d.4bpp.mtf"
	.byte 0x1f, 0xf8	@ PACKER: left in the buffer by Tileset_Set18TilesC+0x1be
