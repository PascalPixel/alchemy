	.irp EntryTarget, 0x0200a501, 0x02008ee1, 0x02008f39, 0x02008f41, 0x0200a4ad, 0x02008f35
	overlay_veneer \EntryTarget
	.endr
