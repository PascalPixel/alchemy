	.irp EntryTarget, 0x02008241, 0x020081c9, 0x020081d9, 0x020081e1, 0x020081e9, 0x020081d1
	overlay_veneer \EntryTarget
	.endr
