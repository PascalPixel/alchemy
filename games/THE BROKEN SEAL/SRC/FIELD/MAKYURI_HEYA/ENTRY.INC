	.irp EntryTarget, 0x0200bd21, 0x02008e21, 0x02008e79, 0x02008e81, 0x02008f31, 0x02008e75
	overlay_veneer \EntryTarget
	.endr
