	.include "games/tla/src/map/map.inc"
	.section .rodata
	.global Field_Map276
Field_Map276:
	map_header 0, 0, 204, 80, 3, 3, 2, 0, 2, 2, 0, 0
	map_layer 0, 0, 12, 12, -2, -2, 0xf, 0xf
	map_layer 0, 80, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 160, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map276, .LField_Map276_CELL_PICTURES
	map_block Field_Map276, .LField_Map276_SHAPES
	map_block Field_Map276, .LField_Map276_GRID
	map_block Field_Map276, .LField_Map276_SHAPE_GRID
	map_block Field_Map276, .LField_Map276_ANIMATION
	map_block Field_Map276
	map_block Field_Map276, .LField_Map276_SCENE
.LField_Map276_CELL_PICTURES:
	.incbin "games/tla/src/map/map_276/cell_pictures.delta1.lz"
	.balign 4, 0
.LField_Map276_SHAPES:
	.incbin "games/tla/src/map/map_276/shapes.table.lz"
	.balign 4, 0
.LField_Map276_GRID:
	.incbin "games/tla/src/map/map_276/map.grid.lz"
	.balign 4, 0
.LField_Map276_SHAPE_GRID:
	.incbin "games/tla/src/map/map_276/shape_grid.plane.lz"
	.balign 4, 0
.LField_Map276_ANIMATION:
	.incbin "games/tla/src/map/map_263/animation.script.lz"
	.balign 4, 0
.LField_Map276_SCENE:
	.incbin "games/tla/src/map/map_276/scene.scene.lz"
	.global Tileset_Palette62
Tileset_Palette62:
	.incbin "games/tla/src/graphics/tileset/palettes/pal_62.gbapal.lz"
	.byte 0x95, 0xe2, 0xb5	@ PACKER: left in the buffer by Field_Map276+0x129
	.global Tileset_Palette63
Tileset_Palette63:
	.incbin "games/tla/src/graphics/tileset/palettes/pal_63.gbapal.lz"
	.byte 0x81	@ PACKER: left in the buffer by Field_Map276+0x177
	.global Tileset_Palette64
Tileset_Palette64:
	.incbin "games/tla/src/graphics/tileset/palettes/pal_64.gbapal.lz"
	.byte 0x00, 0x81	@ PACKER: left in the buffer by Tileset_Palette63+0x176
	.global Tileset_Palette65
Tileset_Palette65:
	.incbin "games/tla/src/graphics/tileset/palettes/pal_65.gbapal.lz"
	.balign 4, 0	@ PACKER: zero fill, not an earlier stream's bytes
	.global Tileset_Palette66
Tileset_Palette66:
	.incbin "games/tla/src/graphics/tileset/palettes/pal_66.gbapal.lz"
	.byte 0x17	@ PACKER: left in the buffer by Tileset_Palette65+0x18f
