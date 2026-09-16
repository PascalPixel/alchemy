	.irp EntryTarget, 0x02008f31, 0x02008499, 0x020084a5, 0x020084ad, 0x02008f29, 0x020084a1
	overlay_veneer \EntryTarget
	.endr
