@ Resource_Data012: the field sprite bank. Each sprite's record gives its
@ size, scale, drawing and frame codec and points at its frame list and
@ animation list; SPRITES.TSV holds the records, each SPR_NNN.PNG a
@ sprite's frames drawn with the system palette and each SPR_NNN.TSV its
@ animation scripts. The Japanese edition's table SPRITEJA.TSV differs only
@ in its emotion icons, SPR_488J.
	.section .rodata
	.global Resource_Data012
Resource_Data012:
	.ifdef TLA_EDITION_JA
	.include "GRAPHICS/SPRITE/SPRITEJA.sprites"
	.else
	.include "GRAPHICS/SPRITE/SPRITES.sprites"
	.endif
