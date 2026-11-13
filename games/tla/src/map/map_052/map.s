	.include "games/tla/src/map/map.inc"
	.section .rodata
	.global Field_Map052
Field_Map052:
	map_header 0, 0, 120, 120, 3, 1, 2, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 120, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 120, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map052, .LField_Map052_CELL_PICTURES
	map_block Field_Map052, .LField_Map052_SHAPES
	map_block Field_Map052, .LField_Map052_GRID
	map_block Field_Map052, .LField_Map052_SHAPE_GRID
	map_block Field_Map052, .LField_Map052_ANIMATION
	map_block Field_Map052, .LField_Map052_BLEND
	map_block Field_Map052, .LField_Map052_SCENE
.LField_Map052_CELL_PICTURES:
	.incbin "games/tla/src/map/map_052/cell_pictures.delta1.lz"
	.balign 4, 0
.LField_Map052_SHAPES:
	.incbin "games/tla/src/map/map_052/shapes.table.lz"
	.balign 4, 0
.LField_Map052_GRID:
	.incbin "games/tla/src/map/map_052/map.grid.lz"
	.balign 4, 0
.LField_Map052_SHAPE_GRID:
	.incbin "games/tla/src/map/map_052/shape_grid.plane.lz"
	.balign 4, 0
.LField_Map052_ANIMATION:
	.incbin "games/tla/src/map/map_052/animation.script.lz"
	.balign 4, 0
.LField_Map052_BLEND:
	.incbin "games/tla/src/map/map_010/blend.script.lz"
	.balign 4, 0
.LField_Map052_SCENE:
	.incbin "games/tla/src/map/map_052/scene.scene.lz"
	.balign 4, 0
	.global Tileset_Set33Palette
Tileset_Set33Palette:
	.incbin "games/tla/src/graphics/tileset/set_33/palette.gbapal.lz"
	.global Tileset_Set33TilesA
Tileset_Set33TilesA:
	.incbin "games/tla/src/graphics/tileset/set_33/tiles_a.4bpp.mtf"
	.byte 0x40, 0x4e, 0x01	@ PACKER: left in the buffer by Field_Map052+0x20ed
	.global Tileset_Set33TilesB
Tileset_Set33TilesB:
	.incbin "games/tla/src/graphics/tileset/set_33/tiles_b.4bpp.mtf"
	.byte 0x43, 0x23	@ PACKER: left in the buffer by Tileset_Set33TilesA+0x1a1a
	.global Tileset_Set33TilesC
Tileset_Set33TilesC:
	.incbin "games/tla/src/graphics/tileset/set_33/tiles_c.4bpp.mtf"
	.byte 0x6e	@ PACKER: left in the buffer by Tileset_Set33TilesB+0x110b
	.global Tileset_Set33TilesD
Tileset_Set33TilesD:
	.incbin "games/tla/src/graphics/tileset/set_33/tiles_d.4bpp.mtf"
