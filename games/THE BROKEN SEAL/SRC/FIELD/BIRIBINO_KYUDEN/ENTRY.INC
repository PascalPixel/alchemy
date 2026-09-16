	.irp EntryTarget, 0x020086e1, 0x02008089, 0x02008095, 0x0200809d, 0x02008101, 0x02008091
	overlay_veneer \EntryTarget
	.endr
