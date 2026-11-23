@ One 16-colour palette in three field overlays (685, 6a6, 6a7): Champa's cliff
@ caves load it into OBJ palette 15 before the explosion sprite sheet
@ (FieldEffect_Explosion), and Loho's sparks and the Northern Reaches' crash do
@ the same.
	.section .rodata
	.global Field_ExplosionPalette
Field_ExplosionPalette:
	.incbin "games/tla/src/graphics/field/explosion_palette.gbapal"
