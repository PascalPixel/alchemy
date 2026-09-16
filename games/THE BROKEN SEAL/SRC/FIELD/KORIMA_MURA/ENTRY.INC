	.irp EntryTarget, 0x02008695, 0x0200816d, 0x020081d5, 0x020081dd, 0x02008285, 0x020081ad
	overlay_veneer \EntryTarget
	.endr
