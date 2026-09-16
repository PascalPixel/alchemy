	.irp EntryTarget, 0x020083f9, 0x02008089, 0x02008095, 0x020080b9, 0x02008161, 0x02008091
	overlay_veneer \EntryTarget
	.endr
