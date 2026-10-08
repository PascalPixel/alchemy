.syntax unified
	.arm
	.global Sound_ResamplerRecords
Sound_ResamplerRecords:
	mov	r2, #0
	mov	r4, #0
	b	Sound_ResamplerRecord1
