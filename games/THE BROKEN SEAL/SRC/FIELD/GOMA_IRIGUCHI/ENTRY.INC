	.irp EntryTarget, 0x020084f5, 0x0200832d, 0x02008339, 0x02008341, 0x020084ed, 0x02008335
	overlay_veneer \EntryTarget
	.endr
