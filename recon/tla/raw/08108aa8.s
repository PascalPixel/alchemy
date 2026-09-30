.syntax unified
	.thumb
	.global Func_08108aa8
	.thumb_func
Func_08108aa8:
	push {r5, r6, lr}
	ldr r5, [r0]
	ldr r6, .L_08108adc
	movs r3, #1
	ldr r4, .L_08108ae0
	strb r3, [r0, #13]
	ldr r3, .L_08108ae4
	strh r1, [r5, #6]
	strh r1, [r0, #8]
	strh r1, [r0, #4]
	ands r1, r6
	ands r1, r3
	strb r4, [r0, #12]
	ldr r3, .L_08108ae8
	ldrh r4, [r5, #22]
	strh r2, [r0, #10]
	ands r3, r4
	orrs r3, r1
	strh r3, [r5, #22]
	strh r2, [r0, #6]
	ldr r3, [r0]
	strh r2, [r3, #8]
	ands r2, r6
	strb r2, [r3, #20]
	b .L_08108aec
	.2byte 0x0000
.L_08108adc:
	.4byte 0x0000ffff
.L_08108ae0:
	.4byte 0x00000000
.L_08108ae4:
	.4byte 0x000001ff
.L_08108ae8:
	.4byte 0xfffffe00
.L_08108aec:
	pop {r5, r6, pc}
	.2byte 0x0000
