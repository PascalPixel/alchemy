	.irp EntryTarget, 0x0200aa55, 0x020089dd, 0x020089e9, 0x020089f1, 0x02008aa5, 0x020089e5
	overlay_veneer \EntryTarget
	.endr
