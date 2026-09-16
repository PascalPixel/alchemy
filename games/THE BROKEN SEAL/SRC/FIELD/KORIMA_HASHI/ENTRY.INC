	.irp EntryTarget, 0x02008c69, 0x02008a4d, 0x02008a59, 0x02008a61, 0x02008c61, 0x02008a55
	overlay_veneer \EntryTarget
	.endr
