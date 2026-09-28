	.global IwramSignedDivideEntry
	.thumb_func
IwramSignedDivideEntry:
	overlay_veneer 0x03000380
	overlay_veneer 0x080000c1
	overlay_veneer 0x080000d1
	overlay_veneer 0x080000f9
	overlay_veneer 0x08000119
	overlay_veneer 0x08000121
	overlay_veneer 0x08000129
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
	overlay_veneer 0x080090d1
	overlay_veneer 0x08009129
	overlay_veneer 0x08009149
	overlay_veneer 0x08009151
	overlay_veneer 0x08009159
	overlay_veneer 0x08009179
	overlay_veneer 0x08009181
	overlay_veneer 0x080091a1
	overlay_veneer 0x080091b9
	overlay_veneer 0x080091c1
	overlay_veneer 0x080091d9
	overlay_veneer 0x080091e1
	overlay_veneer 0x080091e9
	overlay_veneer 0x080091f1
	overlay_veneer 0x080091f9
	overlay_veneer 0x08009279
	overlay_veneer 0x08015041
	overlay_veneer 0x080770c1
	overlay_veneer 0x080770c9
	overlay_veneer 0x080770d1
	overlay_veneer 0x0808a011
	overlay_veneer 0x0808a019
	overlay_veneer 0x0808a021
	overlay_veneer 0x0808a039
	.global Engine_ActorGet
	.thumb_func
Engine_ActorGet:
	overlay_veneer 0x0808a081
	overlay_veneer 0x0808a091
	overlay_veneer 0x0808a099
	overlay_veneer 0x0808a0a1
	overlay_veneer 0x0808a0b9
	overlay_veneer 0x0808a0c1
	overlay_veneer 0x0808a0d1
	overlay_veneer 0x0808a0e1
	overlay_veneer 0x0808a0e9
	overlay_veneer 0x0808a0f1
	overlay_veneer 0x0808a101
	overlay_veneer 0x0808a109
	overlay_veneer 0x0808a111
	overlay_veneer 0x0808a139
	overlay_veneer 0x0808a141
	overlay_veneer 0x0808a149
	overlay_veneer 0x0808a151
	overlay_veneer 0x0808a159
	.global Engine_ObjectSetPalette
	.thumb_func
Engine_ObjectSetPalette:
	overlay_veneer 0x0808a161
	overlay_veneer 0x0808a171
	overlay_veneer 0x0808a181
	overlay_veneer 0x0808a189
	overlay_veneer 0x0808a1b9
	overlay_veneer 0x0808a1e1
	overlay_veneer 0x0808a1e9
	overlay_veneer 0x0808a1f1
	overlay_veneer 0x0808a209
	overlay_veneer 0x0808a211
	overlay_veneer 0x0808a219
	overlay_veneer 0x0808a221
	overlay_veneer 0x0808a229
	overlay_veneer 0x0808a249
	overlay_veneer 0x0808a331
	overlay_veneer 0x0808a339
	overlay_veneer 0x0808a349
	overlay_veneer 0x0808a361
	overlay_veneer 0x0808a369
	overlay_veneer 0x0808a371
	overlay_veneer 0x0808a409
	overlay_veneer 0x0808a581
	overlay_veneer 0x0808a5e1
	overlay_veneer 0x0808a5e9
	overlay_veneer 0x080f9011
