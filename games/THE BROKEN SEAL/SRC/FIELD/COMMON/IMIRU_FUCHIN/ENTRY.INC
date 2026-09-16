	.irp EntryTarget, 0x02009751, 0x02008051, 0x020080f1, 0x020080f9, 0x02008f31, 0x020080ed
	overlay_veneer \EntryTarget
	.endr
