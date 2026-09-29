@ Character_DescriptorTable: the field sprite bank. Each sprite's record
@ gives its size, scale, drawing and frame codec and points at its frame
@ list and animation list; SPRITES.TSV holds the records, each SPR_NNN.PNG
@ a sprite's frames drawn with the system palette, each SPR_NNN.TSV its
@ animation scripts, and LAYOUT.TSV the order the sprites are laid out in.
@ The Japanese and Italian tables, SPRITEJA.TSV and SPRITEIT.TSV, draw the
@ emotion icons of SPR_021 their own way. The bank continues in .rodata.a
@ and .rodata.b around the unused self-pointer slots of ANIMREF0, ANIMREF1
@ and ANIMREF2.
	.section .rodata
	.global Character_DescriptorTable
Character_DescriptorTable:
	.ifdef TBS_EDITION_JA
	.include "GRAPHICS/SPRITE/SPRITEJA.spriteblocks"
	.else
	.ifdef TBS_EDITION_IT
	.include "GRAPHICS/SPRITE/SPRITEIT.spriteblocks"
	.else
	.include "GRAPHICS/SPRITE/SPRITES.spriteblocks"
	.endif
	.endif
