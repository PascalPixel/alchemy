	.irp EntryTarget, 0x02008941, 0x02008089, 0x02008095, 0x020080b9, 0x02008159, 0x02008091
	overlay_veneer \EntryTarget
	.endr
