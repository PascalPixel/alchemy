	.irp EntryTarget, 0x020084e5, 0x0200806d, 0x02008079, 0x02008081, 0x020084dd, 0x02008075
	overlay_veneer \EntryTarget
	.endr
