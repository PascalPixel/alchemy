	.include "games/tla/src/map/map.inc"
	.section .rodata
	.global Field_Map002
Field_Map002:
	map_header 0, 0, 64, 68, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 64, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 68, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map002, .LField_Map002_CELL_PICTURES
	map_block Field_Map002, .LField_Map002_SHAPES
	map_block Field_Map002, .LField_Map002_GRID
	map_block Field_Map002, .LField_Map002_SHAPE_GRID
	map_block Field_Map002, .LField_Map002_ANIMATION
	map_block Field_Map002, .LField_Map002_BLEND
	map_block Field_Map002, .LField_Map002_SCENE
.LField_Map002_CELL_PICTURES:
	.incbin "games/tla/src/map/map_002/cell_pictures.delta1.lz"
	.balign 4, 0
.LField_Map002_SHAPES:
	.incbin "games/tla/src/map/map_002/shapes.table.lz"
	.balign 4, 0
.LField_Map002_GRID:
	.incbin "games/tla/src/map/map_002/map.grid.lz"
	.balign 4, 0
.LField_Map002_SHAPE_GRID:
	.incbin "games/tla/src/map/map_002/shape_grid.plane.lz"
	.balign 4, 0
.LField_Map002_ANIMATION:
	.incbin "games/tla/src/map/map_012/animation.script.lz"
	.balign 4, 0
.LField_Map002_BLEND:
	.incbin "games/tla/src/map/map_001/blend.script.lz"
	.balign 4, 0
.LField_Map002_SCENE:
	.incbin "games/tla/src/map/map_002/scene.scene.lz"
	.balign 4, 0
	.global Tileset_Set01Palette
Tileset_Set01Palette:
	.incbin "games/tla/src/graphics/tileset/set_01/pal_49.gbapal.lz"
	.byte 0x24, 0x04	@ PACKER: left in the buffer by Field_Map002+0x106
	.global Tileset_Set01TilesA
Tileset_Set01TilesA:
	.incbin "games/tla/src/graphics/tileset/set_01/tiles_a.4bpp.mtf"
	.byte 0x49, 0x8e, 0xe3	@ PACKER: left in the buffer by Field_Map002+0x11e9
	.global Tileset_Set01TilesB
Tileset_Set01TilesB:
	.incbin "games/tla/src/graphics/tileset/set_01/tiles_b.4bpp.mtf"
	.byte 0x5b, 0xac, 0x10	@ PACKER: left in the buffer by Field_Map002+0x1c85
	.global Tileset_Set01TilesC
Tileset_Set01TilesC:
	.incbin "games/tla/src/graphics/tileset/set_01/tiles_c.4bpp.mtf"
	.byte 0x3e	@ PACKER: left in the buffer by Tileset_Set01TilesB+0x50b
	.global Tileset_Set01TilesD
Tileset_Set01TilesD:
	.incbin "games/tla/src/graphics/tileset/set_01/tiles_d.4bpp.mtf"
	.byte 0x4e	@ PACKER: left in the buffer by Tileset_Set01TilesC+0x13f
