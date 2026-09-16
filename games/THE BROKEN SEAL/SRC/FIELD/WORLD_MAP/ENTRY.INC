	.irp EntryTarget, 0x020086ed, 0x02008291, 0x0200829d, 0x0200837d, 0x02008591, 0x02008299
	overlay_veneer \EntryTarget
	.endr
