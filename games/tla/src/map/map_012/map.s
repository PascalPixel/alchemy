	.include "games/tla/src/map/map.inc"
	.section .rodata
	.global Field_Map012
Field_Map012:
	map_header 0, 0, 90, 100, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 100, 16, 16, 0, 0, 0xff, 0xff
	map_layer 100, 0, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map012, .LField_Map012_CELL_PICTURES
	map_block Field_Map012, .LField_Map012_SHAPES
	map_block Field_Map012, .LField_Map012_GRID
	map_block Field_Map012, .LField_Map012_SHAPE_GRID
	map_block Field_Map012, .LField_Map012_ANIMATION
	map_block Field_Map012, .LField_Map012_BLEND
	map_block Field_Map012, .LField_Map012_SCENE
.LField_Map012_CELL_PICTURES:
	.incbin "games/tla/src/map/map_012/cell_pictures.delta2.lz"
	.balign 4, 0
.LField_Map012_SHAPES:
	.incbin "games/tla/src/map/map_012/shapes.table.lz"
	.balign 4, 0
.LField_Map012_GRID:
	.incbin "games/tla/src/map/map_012/map.grid.lz"
	.balign 4, 0
.LField_Map012_SHAPE_GRID:
	.incbin "games/tla/src/map/map_012/shape_grid.plane.lz"
	.balign 4, 0
.LField_Map012_ANIMATION:
	.incbin "games/tla/src/map/map_012/animation.script.lz"
	.balign 4, 0
.LField_Map012_BLEND:
	.incbin "games/tla/src/map/map_001/blend.script.lz"
	.balign 4, 0
.LField_Map012_SCENE:
	.incbin "games/tla/src/map/map_012/scene.parts.lz"
	.balign 4, 0
	.global Tileset_Set09Palette
Tileset_Set09Palette:
	.incbin "games/tla/src/graphics/tileset/set_09/palette.gbapal.lz"
	.byte 0xce, 0xcf	@ PACKER: left in the buffer by Field_Map012+0x1a6
	.global Tileset_Set09TilesA
Tileset_Set09TilesA:
	.incbin "games/tla/src/graphics/tileset/set_09/tiles_a.4bpp.mtf"
	.global Tileset_Set09TilesB
Tileset_Set09TilesB:
	.incbin "games/tla/src/graphics/tileset/set_09/tiles_b.4bpp.mtf"
	.byte 0xdc	@ PACKER: left in the buffer by Tileset_Set09TilesA+0x13f
	.global Tileset_Set09TilesC
Tileset_Set09TilesC:
	.incbin "games/tla/src/graphics/tileset/set_09/tiles_c.4bpp.mtf"
	.byte 0xd0, 0x88, 0xf9	@ PACKER: left in the buffer by Tileset_Set09TilesA+0xca5
	.global Tileset_Set09TilesD
Tileset_Set09TilesD:
	.incbin "games/tla/src/graphics/tileset/set_09/tiles_d.4bpp.mtf"
	.byte 0x72, 0x70, 0xf9	@ PACKER: left in the buffer by Tileset_Set09TilesA+0x15cd
