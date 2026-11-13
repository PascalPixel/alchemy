	.include "games/tla/src/map/map.inc"
	.section .rodata
	.global Field_Map049
Field_Map049:
	map_header 0, 0, 74, 64, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 74, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 64, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map049, .LField_Map049_CELL_PICTURES
	map_block Field_Map049, .LField_Map049_SHAPES
	map_block Field_Map049, .LField_Map049_GRID
	map_block Field_Map049, .LField_Map049_SHAPE_GRID
	map_block Field_Map049, .LField_Map049_ANIMATION
	map_block Field_Map049, .LField_Map049_BLEND
	map_block Field_Map049, .LField_Map049_SCENE
.LField_Map049_CELL_PICTURES:
	.incbin "games/tla/src/map/map_049/cell_pictures.delta2.lz"
	.balign 4, 0
.LField_Map049_SHAPES:
	.incbin "games/tla/src/map/map_049/shapes.table.lz"
	.balign 4, 0
.LField_Map049_GRID:
	.incbin "games/tla/src/map/map_049/map.grid.lz"
	.balign 4, 0
.LField_Map049_SHAPE_GRID:
	.incbin "games/tla/src/map/map_049/shape_grid.plane.lz"
	.balign 4, 0
.LField_Map049_ANIMATION:
	.incbin "games/tla/src/map/map_049/animation.script.lz"
	.balign 4, 0
.LField_Map049_BLEND:
	.incbin "games/tla/src/map/map_001/blend.script.lz"
	.balign 4, 0
.LField_Map049_SCENE:
	.incbin "games/tla/src/map/map_049/scene.parts.lz"
	.balign 4, 0
	.global Tileset_Set30Palette
Tileset_Set30Palette:
	.incbin "games/tla/src/graphics/tileset/set_30/palette.gbapal.lz"
	.global Tileset_Set30TilesA
Tileset_Set30TilesA:
	.incbin "games/tla/src/graphics/tileset/set_30/tiles_a.4bpp.mtf"
	.byte 0x61	@ PACKER: left in the buffer by Field_Map049+0x1a2b
	.global Tileset_Set30TilesB
Tileset_Set30TilesB:
	.incbin "games/tla/src/graphics/tileset/set_30/tiles_b.4bpp.mtf"
	.byte 0x5a, 0x14	@ PACKER: left in the buffer by Tileset_Set29TilesC+0x2336
	.global Tileset_Set30TilesC
Tileset_Set30TilesC:
	.incbin "games/tla/src/graphics/tileset/set_30/tiles_c.4bpp.mtf"
	.byte 0x9d	@ PACKER: left in the buffer by Tileset_Set30TilesB+0x11af
	.global Tileset_Set30TilesD
Tileset_Set30TilesD:
	.incbin "games/tla/src/graphics/tileset/set_30/tiles_d.4bpp.mtf"
	.byte 0x53, 0xb6, 0x22	@ PACKER: left in the buffer by Tileset_Set30TilesB+0x1921
