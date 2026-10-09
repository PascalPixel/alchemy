	.include "games/common/include/game/ed_asm.h"
	.section .rodata
	.global Ui_CommandIcons
Ui_CommandIcons:
	.incbin "graphics/ui/command_icons.icons"
	.ifdef TBS_EDITION_JA
	.byte 0x52, 0xdc	@ PACKER: left in the buffer by Ui_Icons+0x3842
	.else
	.byte 0x7b, 0xbc	@ PACKER: left in the buffer by Ui_Icons+0x3842
	.endif
