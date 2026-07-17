@ Reconstructed thumb disassembly of a discovered function. The ROM's
@ compiler is not reproducible by free tools, so no asm-free C match
@ exists yet; build_asm.py verifies these bytes against the ROM.
.syntax unified
	.thumb
	.global Func_0808bc44
	.thumb_func
Func_0808bc44:
	ldr	r3, [pc, #80]
	movs	r0, #182
	ldr	r1, [r3, #0]
	lsls	r0, r0, #1
	movs	r2, #0
	adds	r3, r1, r0
	adds	r0, #2
	strh	r2, [r3, #0]
	adds	r3, r1, r0
	adds	r0, #2
	strh	r2, [r3, #0]
	adds	r3, r1, r0
	adds	r0, #2
	strh	r2, [r3, #0]
	adds	r3, r1, r0
	adds	r0, #2
	strh	r2, [r3, #0]
	adds	r3, r1, r0
	adds	r0, #2
	strh	r2, [r3, #0]
	adds	r3, r1, r0
	adds	r0, #2
	strh	r2, [r3, #0]
	adds	r3, r1, r0
	adds	r0, #2
	strh	r2, [r3, #0]
	adds	r3, r1, r0
	adds	r0, #2
	strh	r2, [r3, #0]
	adds	r3, r1, r0
	adds	r0, #2
	strh	r2, [r3, #0]
	adds	r3, r1, r0
	adds	r0, #2
	strh	r2, [r3, #0]
	adds	r3, r1, r0
	adds	r0, #2
	strh	r2, [r3, #0]
	adds	r3, r1, r0
	strh	r2, [r3, #0]
	bx	lr
	movs	r0, r0
	.4byte 0x03001ebc
