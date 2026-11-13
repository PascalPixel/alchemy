	.include "games/tla/src/map/map.inc"
	.section .rodata
	.global Field_Map087
Field_Map087:
	map_header 0, 0, 64, 64, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 64, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 64, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map087, .LField_Map087_CELL_PICTURES
	map_block Field_Map087, .LField_Map087_SHAPES
	map_block Field_Map087, .LField_Map087_GRID
	map_block Field_Map087, .LField_Map087_SHAPE_GRID
	map_block Field_Map087, .LField_Map087_ANIMATION
	map_block Field_Map087, .LField_Map087_BLEND
	map_block Field_Map087, .LField_Map087_SCENE
.LField_Map087_CELL_PICTURES:
	.incbin "games/tla/src/map/map_087/cell_pictures.delta2.lz"
	.balign 4, 0
.LField_Map087_SHAPES:
	.incbin "games/tla/src/map/map_087/shapes.table.lz"
	.balign 4, 0
.LField_Map087_GRID:
	.incbin "games/tla/src/map/map_087/map.grid.lz"
	.balign 4, 0
.LField_Map087_SHAPE_GRID:
	.incbin "games/tla/src/map/map_087/shape_grid.plane.lz"
	.balign 4, 0
.LField_Map087_ANIMATION:
	.incbin "games/tla/src/map/map_012/animation.script.lz"
	.balign 4, 0
.LField_Map087_BLEND:
	.incbin "games/tla/src/map/map_001/blend.script.lz"
	.balign 4, 0
.LField_Map087_SCENE:
	.incbin "games/tla/src/map/map_087/scene.scene.lz"
	.balign 4, 0
	.global Tileset_Set41Palette
Tileset_Set41Palette:
	.incbin "games/tla/src/graphics/tileset/set_41/palette.gbapal.lz"
	.byte 0xe1	@ PACKER: left in the buffer by Field_Map087+0xbb
	.global Tileset_Set41TilesA
Tileset_Set41TilesA:
	.incbin "games/tla/src/graphics/tileset/set_41/tiles_a.4bpp.mtf"
	.byte 0x80, 0x00	@ PACKER: left in the buffer by Field_Map086+0xd3e
	.global Tileset_Set41TilesB
Tileset_Set41TilesB:
	.incbin "games/tla/src/graphics/tileset/set_41/tiles_b.4bpp.mtf"
	.global Tileset_Set41TilesC
Tileset_Set41TilesC:
	.incbin "games/tla/src/graphics/tileset/set_41/tiles_c.4bpp.mtf"
	.byte 0xca	@ PACKER: left in the buffer by Tileset_Set41TilesB+0x13f
	.global Tileset_Set41TilesD
Tileset_Set41TilesD:
	.incbin "games/tla/src/graphics/tileset/set_41/tiles_d.4bpp.mtf"
	.byte 0x13	@ PACKER: left in the buffer by Tileset_Set41TilesA+0xc17
