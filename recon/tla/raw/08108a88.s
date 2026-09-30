.syntax unified
	.thumb
	.global Func_08108a88
	.thumb_func
Func_08108a88:
	push {r5, r6, lr}
	ldr r5, [r0]
	ldr r6, .L_08108aa0
	ldrh r4, [r5, #6]
	strh r1, [r0, #8]
	strh r4, [r0, #4]
	ldrh r4, [r5, #8]
	strh r2, [r0, #10]
	strh r4, [r0, #6]
	strb r3, [r0, #13]
	strb r6, [r0, #12]
	b .L_08108aa4
.L_08108aa0:
	.4byte 0x00000000
.L_08108aa4:
	pop {r5, r6, pc}
	.2byte 0x0000
