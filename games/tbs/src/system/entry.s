.syntax unified
.text
	.arm
	.global System_Entry
System_Entry:
	mov	r0, #18
	msr	CPSR_fc, r0
	ldr	sp, [pc, #40]
	mov	r0, #31
	msr	CPSR_fc, r0
	ldr	sp, [pc, #24]
	ldr	r1, [pc, #28]
	add	r0, pc, #908
	str	r0, [r1]
	ldr	r1, [pc, #20]
	mov	lr, pc
	bx	r1
	b System_Entry
	.4byte 0x03007f00
	.4byte 0x03007fa0
	.4byte 0x03007ffc
	.4byte System_Initialize
