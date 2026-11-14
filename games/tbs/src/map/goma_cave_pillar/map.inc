	.global Field_GomaCavePillar
Field_GomaCavePillar:
	map_header 0, 0, 96, 64, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 68, 16, 16, 0, 0, 0xff, 0xff
	map_layer 100, 0, 16, 16, 0, 0, 0xff, 0xff
	map_offset Field_GomaCavePillar, .LField_GomaCavePillar_CELL_PICTURES
	map_offset Field_GomaCavePillar, .LField_GomaCavePillar_SHAPES
	map_offset Field_GomaCavePillar, .LField_GomaCavePillar_GRID
	map_offset Field_GomaCavePillar, .LField_GomaCavePillar_ANIMATION
	map_offset Field_GomaCavePillar, .LField_GomaCavePillar_BLEND
	map_offset Field_GomaCavePillar, .LField_GomaCavePillar_MARKERS
.LField_GomaCavePillar_CELL_PICTURES:
	.incbin "games/tbs/src/map/goma_cave_pillar/cell_pictures.delta2.lz"
	.balign 4, 0
.LField_GomaCavePillar_SHAPES:
	.incbin "games/tbs/src/map/goma_cave_pillar/shapes.table.lz"
	.balign 4, 0
.LField_GomaCavePillar_GRID:
	.incbin "games/tbs/src/map/goma_cave_pillar/map.grid.lz"
	.balign 4, 0
.LField_GomaCavePillar_ANIMATION:
	.incbin "games/tbs/src/map/goma_cave_pillar/tileset_anims.script.lz"
	.balign 4, 0
.LField_GomaCavePillar_BLEND:
	.incbin "games/tbs/src/map/goma_cave_pillar/blend.script.lz"
	.balign 4, 0
.LField_GomaCavePillar_MARKERS:
	treasure_spot 27, 8, 101
	treasure_spot 41, 13, 100
	treasure_spots_end
	.balign 4, 0
	.global Tileset_GomaCavePillarPalette
Tileset_GomaCavePillarPalette:
	.incbin "games/tbs/src/graphics/tileset/goma_cave_pillar/palette.gbapal.lz"
	.global Tileset_GomaCavePillarTilesA
Tileset_GomaCavePillarTilesA:
	.incbin "games/tbs/src/graphics/tileset/goma_cave_pillar/tiles_a.4bpp.mtf"
	.byte 0x12, 0x00	@ PACKER: left in the buffer by Tileset_GomaCaveEntranceTilesA+0x2c16
	.global Tileset_GomaCavePillarTilesB
Tileset_GomaCavePillarTilesB:
	.incbin "games/tbs/src/graphics/tileset/goma_cave_pillar/tiles_b.4bpp.mtf"
	.byte 0x27, 0x88	@ PACKER: left in the buffer by Tileset_GomaCavePillarTilesA+0x20fa
	.global Tileset_GomaCavePillarTilesC
Tileset_GomaCavePillarTilesC:
	.incbin "games/tbs/src/graphics/tileset/goma_cave_pillar/tiles_c.4bpp.mtf"
	.global Tileset_GomaCavePillarTilesD
Tileset_GomaCavePillarTilesD:
	.incbin "games/tbs/src/graphics/tileset/goma_cave_pillar/tiles_d.4bpp.mtf"
	.balign 4, 0
