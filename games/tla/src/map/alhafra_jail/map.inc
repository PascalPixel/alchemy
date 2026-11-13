	.global Field_AlhafraJail
Field_AlhafraJail:
	map_header 0, 0, 128, 128, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_AlhafraJail, .LField_AlhafraJail_CELL_PICTURES
	map_block Field_AlhafraJail, .LField_AlhafraJail_SHAPES
	map_block Field_AlhafraJail, .LField_AlhafraJail_GRID
	map_block Field_AlhafraJail, .LField_AlhafraJail_SHAPE_GRID
	map_block Field_AlhafraJail, .LField_AlhafraJail_ANIMATION
	map_block Field_AlhafraJail
	map_block Field_AlhafraJail, .LField_AlhafraJail_SCENE
.LField_AlhafraJail_CELL_PICTURES:
	.incbin "games/tla/src/map/alhafra_jail/cell_pictures.delta2.lz"
	.balign 4, 0
.LField_AlhafraJail_SHAPES:
	.incbin "games/tla/src/map/alhafra_jail/shapes.table.lz"
	.balign 4, 0
.LField_AlhafraJail_GRID:
	.incbin "games/tla/src/map/alhafra_jail/map.grid.lz"
	.balign 4, 0
.LField_AlhafraJail_SHAPE_GRID:
	.incbin "games/tla/src/map/alhafra_jail/shape_grid.plane.lz"
	.balign 4, 0
.LField_AlhafraJail_ANIMATION:
	.incbin "games/tla/src/map/map_021/animation.script.lz"
	.balign 4, 0
.LField_AlhafraJail_SCENE:
	.incbin "games/tla/src/map/alhafra_jail/scene.scene.lz"
	.global Tileset_Palette11
Tileset_Palette11:
	.incbin "games/tla/src/graphics/tileset/palettes/pal_11.gbapal.lz"
	.byte 0x5d	@ PACKER: left in the buffer by Field_AlhafraJail+0x19b
