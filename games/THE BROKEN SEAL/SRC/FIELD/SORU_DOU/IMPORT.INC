	.irp EntryTarget, 0x080091f1, 0x080770c1, 0x080770c9, 0x0808a2f9, 0x0808a5e1
	overlay_veneer \EntryTarget
	.endr
