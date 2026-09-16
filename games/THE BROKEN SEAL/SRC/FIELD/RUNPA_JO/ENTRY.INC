	.irp EntryTarget, 0x0200cda1, 0x020089dd, 0x02008a35, 0x02008a81, 0x02008af9, 0x02008a31
	overlay_veneer \EntryTarget
	.endr
