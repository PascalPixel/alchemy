	.global Field_ChampaInteriors
Field_ChampaInteriors:
	map_header 0, 0, 120, 120, 3, 1, 2, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 120, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 120, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_ChampaInteriors, .LField_ChampaInteriors_CELL_PICTURES
	map_block Field_ChampaInteriors, .LField_ChampaInteriors_SHAPES
	map_block Field_ChampaInteriors, .LField_ChampaInteriors_GRID
	map_block Field_ChampaInteriors, .LField_ChampaInteriors_SHAPE_GRID
	map_block Field_ChampaInteriors, .LField_ChampaInteriors_ANIMATION
	map_block Field_ChampaInteriors, .LField_ChampaInteriors_BLEND
	map_block Field_ChampaInteriors, .LField_ChampaInteriors_SCENE
.LField_ChampaInteriors_CELL_PICTURES:
	.incbin "games/tla/src/map/champa_interiors/cell_pictures.delta1.lz"
	.balign 4, 0
.LField_ChampaInteriors_SHAPES:
	.incbin "games/tla/src/map/champa_interiors/shapes.table.lz"
	.balign 4, 0
.LField_ChampaInteriors_GRID:
	.incbin "games/tla/src/map/champa_interiors/map.grid.lz"
	.balign 4, 0
.LField_ChampaInteriors_SHAPE_GRID:
	.incbin "games/tla/src/map/champa_interiors/shape_grid.plane.lz"
	.balign 4, 0
.LField_ChampaInteriors_ANIMATION:
	.incbin "games/tla/src/map/izumo_interiors/animation.script.lz"
	.balign 4, 0
.LField_ChampaInteriors_BLEND:
	.incbin "games/tla/src/map/izumo_interiors/blend.script.lz"
	.balign 4, 0
.LField_ChampaInteriors_SCENE:
	.incbin "games/tla/src/map/champa_interiors/scene.scene.lz"
	.balign 4, 0
	.global Tileset_PaletteChampaInteriors
Tileset_PaletteChampaInteriors:
	.incbin "games/tla/src/graphics/tileset/palettes/champa_interiors.gbapal.lz"
	.byte 0x30	@ PACKER: left in the buffer by Field_ChampaInteriors+0x193
