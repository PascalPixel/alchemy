	.include "games/tla/src/map/map.inc"
	.section .rodata
	.global Field_Map008
Field_Map008:
	map_header 0, 0, 120, 120, 3, 1, 2, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 120, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 120, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map008, .LField_Map008_CELL_PICTURES
	map_block Field_Map008, .LField_Map008_SHAPES
	map_block Field_Map008, .LField_Map008_GRID
	map_block Field_Map008, .LField_Map008_SHAPE_GRID
	map_block Field_Map008, .LField_Map008_ANIMATION
	map_block Field_Map008
	map_block Field_Map008, .LField_Map008_SCENE
.LField_Map008_CELL_PICTURES:
	.incbin "games/tla/src/map/map_008/cell_pictures.delta1.lz"
	.balign 4, 0
.LField_Map008_SHAPES:
	.incbin "games/tla/src/map/map_008/shapes.table.lz"
	.balign 4, 0
.LField_Map008_GRID:
	.incbin "games/tla/src/map/map_008/map.grid.lz"
	.balign 4, 0
.LField_Map008_SHAPE_GRID:
	.incbin "games/tla/src/map/map_008/shape_grid.plane.lz"
	.balign 4, 0
.LField_Map008_ANIMATION:
	.incbin "games/tla/src/map/map_008/animation.script.lz"
	.balign 4, 0
.LField_Map008_SCENE:
	.incbin "games/tla/src/map/map_008/scene.scene.lz"
	.global Tileset_Set06Palette
Tileset_Set06Palette:
	.incbin "games/tla/src/graphics/tileset/set_06/palette.gbapal.lz"
	.byte 0x81, 0xc2	@ PACKER: left in the buffer by Field_Map008+0x196
	.global Tileset_Set06TilesA
Tileset_Set06TilesA:
	.incbin "games/tla/src/graphics/tileset/set_06/tiles_a.4bpp.mtf"
	.global Tileset_Set06TilesB
Tileset_Set06TilesB:
	.incbin "games/tla/src/graphics/tileset/set_06/tiles_b.4bpp.mtf"
	.byte 0x00	@ PACKER: left in the buffer by Tileset_Set06TilesA+0x270b
	.global Tileset_Set06TilesC
Tileset_Set06TilesC:
	.incbin "games/tla/src/graphics/tileset/set_06/tiles_c.4bpp.mtf"
	.byte 0xc1	@ PACKER: left in the buffer by Tileset_Set06TilesA+0x2943
	.global Tileset_Set06TilesD
Tileset_Set06TilesD:
	.incbin "games/tla/src/graphics/tileset/set_06/tiles_d.4bpp.mtf"
	.byte 0x67, 0x14	@ PACKER: left in the buffer by Tileset_Set06TilesC+0x14d2
