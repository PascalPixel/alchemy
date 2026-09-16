	.irp EntryTarget, 0x02009395, 0x02008cc1, 0x02008d19, 0x02008d21, 0x02008d89, 0x02008d15
	overlay_veneer \EntryTarget
	.endr
