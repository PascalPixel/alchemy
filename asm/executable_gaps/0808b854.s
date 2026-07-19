.syntax unified
.text
	.thumb
	.global Fragment_0808b854
	.thumb_func
Fragment_0808b854:
	ldr	r3, [pc, #12]
	ldr	r3, [r3, #0]
	lsls	r0, r0, #2
	adds	r3, r3, r0
	adds	r3, #20
	adds	r0, r3, #0
	bx	lr
	.inst.n 0x0000
	.4byte 0x03001ebc
