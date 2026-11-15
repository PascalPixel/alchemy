	.global Field_MadraCatacombs6
Field_MadraCatacombs6:
	map_header 0, 0, 128, 128, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_MadraCatacombs6, .LField_MadraCatacombs6_CELL_PICTURES
	map_block Field_MadraCatacombs6, .LField_MadraCatacombs6_SHAPES
	map_block Field_MadraCatacombs6, .LField_MadraCatacombs6_GRID
	map_block Field_MadraCatacombs6, .LField_MadraCatacombs6_SHAPE_GRID
	map_block Field_MadraCatacombs6, .LField_MadraCatacombs6_ANIMATION
	map_block Field_MadraCatacombs6, .LField_MadraCatacombs6_BLEND
	map_block Field_MadraCatacombs6, .LField_MadraCatacombs6_SCENE
.LField_MadraCatacombs6_CELL_PICTURES:
	.incbin "games/tla/src/map/madra_catacombs_6/cell_pictures.delta1.lz"
	.balign 4, 0
.LField_MadraCatacombs6_SHAPES:
	.incbin "games/tla/src/map/madra_catacombs_6/shapes.table.lz"
	.balign 4, 0
.LField_MadraCatacombs6_GRID:
	.incbin "games/tla/src/map/madra_catacombs_6/map.grid.lz"
	.balign 4, 0
.LField_MadraCatacombs6_SHAPE_GRID:
	.incbin "games/tla/src/map/madra_catacombs_6/shape_grid.plane.lz"
	.balign 4, 0
.LField_MadraCatacombs6_ANIMATION:
	.incbin "games/tla/src/map/kandorean_temple_2/animation.script.lz"
	.balign 4, 0
.LField_MadraCatacombs6_BLEND:
	.incbin "games/tla/src/map/lobby/blend.script.lz"
	.balign 4, 0
.LField_MadraCatacombs6_SCENE:
	.incbin "games/tla/src/map/madra_catacombs_6/scene.scene.lz"
	.global Tileset_MadraCatacombs6Palette
Tileset_MadraCatacombs6Palette:
	.incbin "games/tla/src/graphics/tileset/madra_catacombs_6/palette.gbapal.lz"
	.byte 0x2d, 0x40, 0xff	@ PACKER: left in the buffer by Field_MadraCatacombs6+0x169
	.global Tileset_MadraCatacombs6TilesA
Tileset_MadraCatacombs6TilesA:
	.incbin "games/tla/src/graphics/tileset/madra_catacombs_6/tiles_a.4bpp.mtf"
	.byte 0x04	@ PACKER: left in the buffer by Field_MadraInteriors2+0x25af
	.global Tileset_MadraCatacombs6TilesB
Tileset_MadraCatacombs6TilesB:
	.incbin "games/tla/src/graphics/tileset/madra_catacombs_6/tiles_b.4bpp.mtf"
	.byte 0xce, 0x80	@ PACKER: left in the buffer by Tileset_MadraCatacombs6TilesA+0x1cbe
	.global Tileset_MadraCatacombs6TilesC
Tileset_MadraCatacombs6TilesC:
	.incbin "games/tla/src/graphics/tileset/madra_catacombs_6/tiles_c.4bpp.mtf"
	.byte 0x20, 0xdb	@ PACKER: left in the buffer by Tileset_MadraCatacombs6TilesB+0x1aca
	.global Tileset_MadraCatacombs6TilesD
Tileset_MadraCatacombs6TilesD:
	.incbin "games/tla/src/graphics/tileset/madra_catacombs_6/tiles_d.4bpp.mtf"
