	.include "games/tbs/src/map/map.inc"
	.section .rodata
	.global Field_SolSanctumCorridor
Field_SolSanctumCorridor:
	map_header 0, 0, 98, 88, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 88, 16, 16, 0, 0, 0xff, 0xff
	map_layer 98, 0, 16, 16, 0, 0, 0xff, 0xff
	map_offset Field_SolSanctumCorridor, .LField_SolSanctumCorridor_CELL_PICTURES
	map_offset Field_SolSanctumCorridor, .LField_SolSanctumCorridor_SHAPES
	map_offset Field_SolSanctumCorridor, .LField_SolSanctumCorridor_GRID
	map_offset Field_SolSanctumCorridor, .LField_SolSanctumCorridor_ANIMATION
	map_offset Field_SolSanctumCorridor, .LField_SolSanctumCorridor_BLEND
	map_offset Field_SolSanctumCorridor, .LField_SolSanctumCorridor_MARKERS
.LField_SolSanctumCorridor_CELL_PICTURES:
	.incbin "games/tbs/src/map/sol_sanctum_corridor/cell_pictures.delta1.lz"
	.balign 4, 0
.LField_SolSanctumCorridor_SHAPES:
	.incbin "games/tbs/src/map/map_001/shapes.table.lz"
	.balign 4, 0
.LField_SolSanctumCorridor_GRID:
	.incbin "games/tbs/src/map/sol_sanctum_corridor/map.grid.lz"
	.balign 4, 0
.LField_SolSanctumCorridor_ANIMATION:
	.incbin "games/tbs/src/map/sol_sanctum_maze/tileset_anims.script.lz"
	.balign 4, 0
.LField_SolSanctumCorridor_BLEND:
	.incbin "games/tbs/src/map/sol_sanctum_maze/blend.script.lz"
	.balign 4, 0
.LField_SolSanctumCorridor_MARKERS:
	treasure_spots_end
	.balign 4, 0
	.global Tileset_SolSanctumCorridorPalette
Tileset_SolSanctumCorridorPalette:
	.incbin "games/tbs/src/graphics/tileset/sol_sanctum_corridor/palette.gbapal.lz"
	.byte 0x43	@ PACKER: left in the buffer by Field_SolSanctumCorridor+0x113
	.global Tileset_SolSanctumCorridorTilesA
Tileset_SolSanctumCorridorTilesA:
	.incbin "games/tbs/src/graphics/tileset/sol_sanctum_corridor/tiles_a.4bpp.mtf"
	.global Tileset_SolSanctumCorridorTilesB
Tileset_SolSanctumCorridorTilesB:
	.incbin "games/tbs/src/graphics/tileset/sol_sanctum_corridor/tiles_b.4bpp.mtf"
	.byte 0x50, 0x92, 0x3f	@ PACKER: left in the buffer by Tileset_SolSanctumCorridorTilesA+0x909
	.global Tileset_SolSanctumCorridorTilesC
Tileset_SolSanctumCorridorTilesC:
	.incbin "games/tbs/src/graphics/tileset/sol_sanctum_corridor/tiles_c.4bpp.mtf"
	.byte 0xe1	@ PACKER: left in the buffer by Tileset_SolSanctumCorridorTilesB+0x147
	.global Tileset_SolSanctumCorridorTilesD
Tileset_SolSanctumCorridorTilesD:
	.incbin "games/tbs/src/graphics/tileset/sol_sanctum_corridor/tiles_d.4bpp.mtf"
