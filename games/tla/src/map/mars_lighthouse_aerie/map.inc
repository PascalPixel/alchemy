	.global Field_MarsLighthouseAerie
Field_MarsLighthouseAerie:
	map_header 0, 0, 128, 128, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 12, 12, -2, -2, 0x1f, 0x1f
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_MarsLighthouseAerie, .LField_MarsLighthouseAerie_CELL_PICTURES
	map_block Field_MarsLighthouseAerie, .LField_MarsLighthouseAerie_SHAPES
	map_block Field_MarsLighthouseAerie, .LField_MarsLighthouseAerie_GRID
	map_block Field_MarsLighthouseAerie, .LField_MarsLighthouseAerie_SHAPE_GRID
	map_block Field_MarsLighthouseAerie, .LField_MarsLighthouseAerie_ANIMATION
	map_block Field_MarsLighthouseAerie
	map_block Field_MarsLighthouseAerie, .LField_MarsLighthouseAerie_SCENE
.LField_MarsLighthouseAerie_CELL_PICTURES:
	.incbin "games/tla/src/map/mars_lighthouse_aerie/cell_pictures.delta2.lz"
	.balign 4, 0
.LField_MarsLighthouseAerie_SHAPES:
	.incbin "games/tla/src/map/mars_lighthouse_aerie/shapes.table.lz"
	.balign 4, 0
.LField_MarsLighthouseAerie_GRID:
	.incbin "games/tla/src/map/mars_lighthouse_aerie/map.grid.lz"
	.balign 4, 0
.LField_MarsLighthouseAerie_SHAPE_GRID:
	.incbin "games/tla/src/map/mars_lighthouse_aerie/shape_grid.plane.lz"
	.balign 4, 0
.LField_MarsLighthouseAerie_ANIMATION:
	.incbin "games/tla/src/map/mars_lighthouse_aerie/animation.script.lz"
	.balign 4, 0
.LField_MarsLighthouseAerie_SCENE:
	.incbin "games/tla/src/map/mars_lighthouse_aerie/scene.scene.lz"
	.global Tileset_Set110Palette
Tileset_Set110Palette:
	.incbin "games/tla/src/graphics/tileset/set_110/palette.gbapal.lz"
	.byte 0xc7, 0xba	@ PACKER: left in the buffer by Field_MarsLighthouseAerie+0x16e
	.global Tileset_Set110TilesA
Tileset_Set110TilesA:
	.incbin "games/tla/src/graphics/tileset/set_110/tiles_a.4bpp.mtf"
	.byte 0x04, 0x07, 0x1a	@ PACKER: left in the buffer by Field_Map257+0x23a5
	.global Tileset_Set110TilesB
Tileset_Set110TilesB:
	.incbin "games/tla/src/graphics/tileset/set_110/tiles_b.4bpp.mtf"
	.byte 0xc1, 0xc3	@ PACKER: left in the buffer by Tileset_Set110TilesA+0x6e6
