.syntax unified
	.thumb
	.global Func_08045018
	.thumb_func
Func_08045018:
	push {r5, lr}
	ldr r4, .L_08045040
	ldr r5, .L_08045044
	ldr r3, [r4]
	movs r1, #7
	lsrs r3, r3, #2
	ands r3, r1
	ldrb r2, [r0, #8]
	ldrb r3, [r5, r3]
	adds r2, r2, r3
	ldr r3, [r4]
	strb r2, [r0, #20]
	ldr r0, [r0]
	lsrs r3, r3, #2
	ands r3, r1
	ldrb r2, [r0, #8]
	ldrb r3, [r5, r3]
	adds r2, r2, r3
	strb r2, [r0, #20]
	pop {r5, pc}
.L_08045040:
	.4byte gFrameTick
.L_08045044:
	.4byte Data_0805f6d6
