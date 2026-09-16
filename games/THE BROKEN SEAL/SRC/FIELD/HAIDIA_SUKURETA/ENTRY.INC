	.irp EntryTarget, 0x0200950d, 0x02008089, 0x02008095, 0x0200809d, 0x020080a5, 0x02008091
	overlay_veneer \EntryTarget
	.endr
