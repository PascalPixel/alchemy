	.global Field_LemuriaPalace
Field_LemuriaPalace:
	map_header 0, 0, 128, 112, 3, 1, 2, 1, 1, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_LemuriaPalace, .LField_LemuriaPalace_CELL_PICTURES
	map_block Field_LemuriaPalace, .LField_LemuriaPalace_SHAPES
	map_block Field_LemuriaPalace, .LField_LemuriaPalace_GRID
	map_block Field_LemuriaPalace, .LField_LemuriaPalace_SHAPE_GRID
	map_block Field_LemuriaPalace, .LField_LemuriaPalace_ANIMATION
	map_block Field_LemuriaPalace, .LField_LemuriaPalace_BLEND
	map_block Field_LemuriaPalace, .LField_LemuriaPalace_SCENE
.LField_LemuriaPalace_CELL_PICTURES:
	.incbin "games/tla/src/map/lemuria_palace/cell_pictures.delta2.lz"
	.balign 4, 0
.LField_LemuriaPalace_SHAPES:
	.incbin "games/tla/src/map/lemuria_palace/shapes.table.lz"
	.balign 4, 0
.LField_LemuriaPalace_GRID:
	.incbin "games/tla/src/map/lemuria_palace/map.grid.lz"
	.balign 4, 0
.LField_LemuriaPalace_SHAPE_GRID:
	.incbin "games/tla/src/map/lemuria_palace/shape_grid.plane.lz"
	.balign 4, 0
.LField_LemuriaPalace_ANIMATION:
	.incbin "games/tla/src/map/lemuria_palace/animation.script.lz"
	.balign 4, 0
.LField_LemuriaPalace_BLEND:
	.incbin "games/tla/src/map/lobby/blend.script.lz"
	.balign 4, 0
.LField_LemuriaPalace_SCENE:
	.incbin "games/tla/src/map/lemuria_palace/scene.scene.lz"
	.global Tileset_Set83Palette
Tileset_Set83Palette:
	.incbin "games/tla/src/graphics/tileset/set_83/palette.gbapal.lz"
	.byte 0xa4, 0x02, 0x52	@ PACKER: left in the buffer by Field_LemuriaPalace+0x1c9
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
