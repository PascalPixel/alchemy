	overlay_veneer 0x03000380
	overlay_veneer 0x030003ac
	overlay_veneer 0x080000c1
	overlay_veneer 0x080000d1
	.global Engine_TaskRemoveCallback
	.thumb_func
Engine_TaskRemoveCallback:
	overlay_veneer 0x080000d9
	.global Runtime_BumpAllocateAlternatePool
	.thumb_func
Runtime_BumpAllocateAlternatePool:
	overlay_veneer 0x08000171
	.global Runtime_BumpFree
	.thumb_func
Runtime_BumpFree:
	overlay_veneer 0x08000179
	.global Resource_DecodeType01
	.thumb_func
Resource_DecodeType01:
	overlay_veneer 0x080001a9
	overlay_veneer 0x080001f1
	overlay_veneer 0x08000291
	overlay_veneer 0x080002a9
	overlay_veneer 0x08000301
	overlay_veneer 0x08000371
	overlay_veneer 0x08000381
	overlay_veneer 0x08015011
	overlay_veneer 0x08015019
	overlay_veneer 0x08015041
	overlay_veneer 0x08015061
	overlay_veneer 0x08015071
	overlay_veneer 0x08015079
	overlay_veneer 0x08015081
	overlay_veneer 0x08015089
	overlay_veneer 0x08015141
	overlay_veneer 0x080151f1
	overlay_veneer 0x080151f9
	overlay_veneer 0x08015211
	overlay_veneer 0x08015281
	overlay_veneer 0x080152e9
	overlay_veneer 0x080152f1
	overlay_veneer 0x080152f9
	overlay_veneer 0x08015301
	overlay_veneer 0x08015319
	overlay_veneer 0x08015321
	overlay_veneer 0x08015361
	overlay_veneer 0x08015391
	overlay_veneer 0x080153d9
	overlay_veneer 0x08015419
	overlay_veneer 0x08077009
	overlay_veneer 0x08077019
	overlay_veneer 0x08077099
	overlay_veneer 0x080770c1
	overlay_veneer 0x080770c9
	overlay_veneer 0x080770d1
	overlay_veneer 0x08077151
	overlay_veneer 0x08077169
	overlay_veneer 0x08077271
	overlay_veneer 0x08077301
	overlay_veneer 0x08077319
	overlay_veneer 0x0808a011
	overlay_veneer 0x0808a081
	overlay_veneer 0x0808a239
	overlay_veneer 0x0808a361
	overlay_veneer 0x0808a369
	overlay_veneer 0x0808a371
	overlay_veneer 0x0808a4d9
	overlay_veneer 0x080f9011
