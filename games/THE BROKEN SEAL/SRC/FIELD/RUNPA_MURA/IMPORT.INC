	.global IwramSignedDivideEntry
	.thumb_func
IwramSignedDivideEntry:
	overlay_veneer 0x03000380
	overlay_veneer 0x080000c1
	overlay_veneer 0x080000d1
	overlay_veneer 0x080000d9
	overlay_veneer 0x080000f9
	overlay_veneer 0x08000119
	overlay_veneer 0x08000121
	overlay_veneer 0x08000141
	overlay_veneer 0x08000151
	overlay_veneer 0x080001c9
	.global Engine_ObjectSetAnimation
	.thumb_func
Engine_ObjectSetAnimation:
	overlay_veneer 0x08009081
	.global Engine_ObjectSetScript
	.thumb_func
Engine_ObjectSetScript:
	overlay_veneer 0x08009099
	.global Engine_ObjectCreate
	.thumb_func
Engine_ObjectCreate:
	overlay_veneer 0x080090c9
	overlay_veneer 0x08009179
	overlay_veneer 0x080091b9
	overlay_veneer 0x080091c1
	overlay_veneer 0x080091e1
	overlay_veneer 0x080091e9
	overlay_veneer 0x08009241
	overlay_veneer 0x08015041
	overlay_veneer 0x08015251
	overlay_veneer 0x080770c1
	overlay_veneer 0x080770c9
	overlay_veneer 0x080770d1
	overlay_veneer 0x0808a011
	overlay_veneer 0x0808a019
	overlay_veneer 0x0808a021
	overlay_veneer 0x0808a061
	overlay_veneer 0x0808a071
	.global Engine_ActorGet
	.thumb_func
Engine_ActorGet:
	overlay_veneer 0x0808a081
	overlay_veneer 0x0808a091
	overlay_veneer 0x0808a0a9
	overlay_veneer 0x0808a0b9
	overlay_veneer 0x0808a0c9
	overlay_veneer 0x0808a0d9
	overlay_veneer 0x0808a0e1
	overlay_veneer 0x0808a0e9
	overlay_veneer 0x0808a0f1
	overlay_veneer 0x0808a101
	overlay_veneer 0x0808a131
	overlay_veneer 0x0808a139
	overlay_veneer 0x0808a149
	.global Engine_ObjectSetPalette
	.thumb_func
Engine_ObjectSetPalette:
	overlay_veneer 0x0808a161
	overlay_veneer 0x0808a171
	overlay_veneer 0x0808a179
	overlay_veneer 0x0808a181
	overlay_veneer 0x0808a191
	overlay_veneer 0x0808a1b9
	overlay_veneer 0x0808a1e1
	overlay_veneer 0x0808a1e9
	overlay_veneer 0x0808a1f1
	overlay_veneer 0x0808a201
	overlay_veneer 0x0808a249
	overlay_veneer 0x0808a2a1
	overlay_veneer 0x0808a361
	overlay_veneer 0x0808a369
	overlay_veneer 0x0808a371
	overlay_veneer 0x0808a399
	overlay_veneer 0x0808a3c1
	overlay_veneer 0x0808a421
	overlay_veneer 0x0808a429
	overlay_veneer 0x0808a431
	overlay_veneer 0x0808a439
	overlay_veneer 0x0808a449
	overlay_veneer 0x0808a471
	overlay_veneer 0x0808a579
	overlay_veneer 0x0808a581
	overlay_veneer 0x080f9011
