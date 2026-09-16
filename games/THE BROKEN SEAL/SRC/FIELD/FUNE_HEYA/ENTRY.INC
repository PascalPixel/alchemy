	.irp EntryTarget, 0x0200aff1, 0x02008341, 0x02008375, 0x0200837d, 0x0200854d, 0x02008371
	overlay_veneer \EntryTarget
	.endr
