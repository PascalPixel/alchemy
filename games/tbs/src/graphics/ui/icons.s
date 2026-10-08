	.include "games/common/include/game/ed_asm.h"
	.section .rodata
	.global Ui_Icons
Ui_Icons:
	.ifeq EDITION_INTERNATIONAL
	.incbin "graphics/ui/icons/icons_ja.icons4"
	.else
	.incbin "graphics/ui/icons/icons.icons4"
	.endif
