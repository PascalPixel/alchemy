	.include "games/tla/src/map/map.inc"
	.section .rodata
	.global Field_Map224
Field_Map224:
	map_header 0, 0, 64, 64, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map224, .LField_Map224_CELL_PICTURES
	map_block Field_Map224, .LField_Map224_SHAPES
	map_block Field_Map224, .LField_Map224_GRID
	map_block Field_Map224, .LField_Map224_SHAPE_GRID
	map_block Field_Map224, .LField_Map224_ANIMATION
	map_block Field_Map224, .LField_Map224_BLEND
	map_block Field_Map224, .LField_Map224_SCENE
.LField_Map224_CELL_PICTURES:
	.incbin "games/tla/src/map/map_224/cell_pictures.delta0.lz"
	.balign 4, 0
.LField_Map224_SHAPES:
	.incbin "games/tla/src/map/map_224/shapes.table.lz"
	.balign 4, 0
.LField_Map224_GRID:
	.incbin "games/tla/src/map/map_224/map.grid.lz"
	.balign 4, 0
.LField_Map224_SHAPE_GRID:
	.incbin "games/tla/src/map/map_224/shape_grid.plane.lz"
	.balign 4, 0
.LField_Map224_ANIMATION:
	.incbin "games/tla/src/map/map_224/animation.script.lz"
	.balign 4, 0
.LField_Map224_BLEND:
	.incbin "games/tla/src/map/map_001/blend.script.lz"
	.balign 4, 0
.LField_Map224_SCENE:
	.incbin "games/tla/src/map/map_224/scene.scene.lz"
	.balign 4, 0
	.global Tileset_Set97Palette
Tileset_Set97Palette:
	.incbin "games/tla/src/graphics/tileset/set_97/palette.gbapal.lz"
	.byte 0xd1, 0x0c	@ PACKER: left in the buffer by Field_Map224+0x172
	.global Tileset_Set97TilesA
Tileset_Set97TilesA:
	.incbin "games/tla/src/graphics/tileset/set_97/tiles_a.4bpp.mtf"
	.byte 0xf6, 0xf1	@ PACKER: left in the buffer by Field_Map221+0x2466
	.global Tileset_Set97TilesB
Tileset_Set97TilesB:
	.incbin "games/tla/src/graphics/tileset/set_97/tiles_b.4bpp.mtf"
	.byte 0xf3, 0x7d	@ PACKER: left in the buffer by Tileset_Set97TilesA+0x199a
	.global Tileset_Set97TilesC
Tileset_Set97TilesC:
	.incbin "games/tla/src/graphics/tileset/set_97/tiles_c.4bpp.mtf"
	.byte 0x28	@ PACKER: left in the buffer by Tileset_Set97TilesA+0x1e33
	.global Tileset_Set97TilesD
Tileset_Set97TilesD:
	.incbin "games/tla/src/graphics/tileset/set_97/tiles_d.4bpp.mtf"
	.byte 0x05, 0xe0	@ PACKER: left in the buffer by Tileset_Set97TilesC+0xab6
