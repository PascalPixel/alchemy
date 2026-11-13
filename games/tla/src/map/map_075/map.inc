	.global Field_Map075
Field_Map075:
	map_header 0, 0, 128, 128, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map075, .LField_Map075_CELL_PICTURES
	map_block Field_Map075, .LField_Map075_SHAPES
	map_block Field_Map075, .LField_Map075_GRID
	map_block Field_Map075, .LField_Map075_SHAPE_GRID
	map_block Field_Map075, .LField_Map075_ANIMATION
	map_block Field_Map075, .LField_Map075_BLEND
	map_block Field_Map075, .LField_Map075_SCENE
.LField_Map075_CELL_PICTURES:
	.incbin "games/tla/src/map/map_075/cell_pictures.delta0.lz"
	.balign 4, 0
.LField_Map075_SHAPES:
	.incbin "games/tla/src/map/map_075/shapes.table.lz"
	.balign 4, 0
.LField_Map075_GRID:
	.incbin "games/tla/src/map/map_075/map.grid.lz"
	.balign 4, 0
.LField_Map075_SHAPE_GRID:
	.incbin "games/tla/src/map/map_075/shape_grid.plane.lz"
	.balign 4, 0
.LField_Map075_ANIMATION:
	.incbin "games/tla/src/map/map_012/animation.script.lz"
	.balign 4, 0
.LField_Map075_BLEND:
	.incbin "games/tla/src/map/map_001/blend.script.lz"
	.balign 4, 0
.LField_Map075_SCENE:
	.incbin "games/tla/src/map/map_075/scene.scene.lz"
	.balign 4, 0
	.global Tileset_Set39Palette
Tileset_Set39Palette:
	.incbin "games/tla/src/graphics/tileset/set_39/palette.gbapal.lz"
	.byte 0x98, 0xb7, 0xe6	@ PACKER: left in the buffer by Field_Map075+0x191
	.global Tileset_Set39TilesA
Tileset_Set39TilesA:
	.incbin "games/tla/src/graphics/tileset/set_39/tiles_a.4bpp.mtf"
	.global Tileset_Set39TilesB
Tileset_Set39TilesB:
	.incbin "games/tla/src/graphics/tileset/set_39/tiles_b.4bpp.mtf"
	.byte 0x48	@ PACKER: left in the buffer by Tileset_Set39TilesA+0x269f
	.global Tileset_Set39TilesC
Tileset_Set39TilesC:
	.incbin "games/tla/src/graphics/tileset/set_39/tiles_c.4bpp.mtf"
	.byte 0xe7, 0xe3, 0x0d	@ PACKER: left in the buffer by Tileset_Set39TilesB+0x2681
	.global Tileset_Set39TilesD
Tileset_Set39TilesD:
	.incbin "games/tla/src/graphics/tileset/set_39/tiles_d.4bpp.mtf"
	.byte 0x7b	@ PACKER: left in the buffer by Tileset_Set39TilesC+0x333
