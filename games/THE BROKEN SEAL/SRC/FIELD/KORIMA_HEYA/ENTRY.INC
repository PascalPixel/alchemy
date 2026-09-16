	.irp EntryTarget, 0x020081c5, 0x0200805d, 0x02008069, 0x02008071, 0x020081bd, 0x02008065
	overlay_veneer \EntryTarget
	.endr
