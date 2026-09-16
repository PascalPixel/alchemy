	.irp EntryTarget, 0x0200a8a1, 0x02008f59, 0x02008fd5, 0x02008fdd, 0x0200a7a1, 0x02008fad
	overlay_veneer \EntryTarget
	.endr
