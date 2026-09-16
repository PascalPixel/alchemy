	.irp EntryTarget, 0x02008435, 0x020080e9, 0x020080f5, 0x020080fd, 0x02008105, 0x020080f1
	overlay_veneer \EntryTarget
	.endr
