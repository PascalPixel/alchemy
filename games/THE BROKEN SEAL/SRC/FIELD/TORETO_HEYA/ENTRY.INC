	.irp EntryTarget, 0x02009005, 0x02008315, 0x02008325, 0x0200832d, 0x02008695, 0x0200831d
	overlay_veneer \EntryTarget
	.endr
