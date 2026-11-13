	.global Field_Map191
Field_Map191:
	map_header 0, 0, 128, 112, 3, 1, 2, 1, 1, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map191, .LField_Map191_CELL_PICTURES
	map_block Field_Map191, .LField_Map191_SHAPES
	map_block Field_Map191, .LField_Map191_GRID
	map_block Field_Map191, .LField_Map191_SHAPE_GRID
	map_block Field_Map191, .LField_Map191_ANIMATION
	map_block Field_Map191, .LField_Map191_BLEND
	map_block Field_Map191, .LField_Map191_SCENE
.LField_Map191_CELL_PICTURES:
	.incbin "games/tla/src/map/map_191/cell_pictures.delta2.lz"
	.balign 4, 0
.LField_Map191_SHAPES:
	.incbin "games/tla/src/map/map_191/shapes.table.lz"
	.balign 4, 0
.LField_Map191_GRID:
	.incbin "games/tla/src/map/map_191/map.grid.lz"
	.balign 4, 0
.LField_Map191_SHAPE_GRID:
	.incbin "games/tla/src/map/map_191/shape_grid.plane.lz"
	.balign 4, 0
.LField_Map191_ANIMATION:
	.incbin "games/tla/src/map/map_191/animation.script.lz"
	.balign 4, 0
.LField_Map191_BLEND:
	.incbin "games/tla/src/map/map_001/blend.script.lz"
	.balign 4, 0
.LField_Map191_SCENE:
	.incbin "games/tla/src/map/map_191/scene.scene.lz"
	.global Tileset_Set83Palette
Tileset_Set83Palette:
	.incbin "games/tla/src/graphics/tileset/set_83/palette.gbapal.lz"
	.byte 0xa4, 0x02, 0x52	@ PACKER: left in the buffer by Field_Map191+0x1c9
	.global Tileset_Set83TilesA
Tileset_Set83TilesA:
	.incbin "games/tla/src/graphics/tileset/set_83/tiles_a.4bpp.mtf"
	.global Tileset_Set83TilesB
Tileset_Set83TilesB:
	.incbin "games/tla/src/graphics/tileset/set_83/tiles_b.4bpp.mtf"
	.byte 0x75, 0xea	@ PACKER: left in the buffer by Tileset_Set83TilesA+0x18ce
	.global Tileset_Set83TilesC
Tileset_Set83TilesC:
	.incbin "games/tla/src/graphics/tileset/set_83/tiles_c.4bpp.mtf"
	.byte 0x95, 0xeb	@ PACKER: left in the buffer by Tileset_Set83TilesA+0x212e
	.global Tileset_Set83TilesD
Tileset_Set83TilesD:
	.incbin "games/tla/src/graphics/tileset/set_83/tiles_d.4bpp.mtf"
