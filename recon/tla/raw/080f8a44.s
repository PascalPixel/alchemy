.syntax unified
	.thumb
	.global Func_080f8a44
	.thumb_func
Func_080f8a44:
	push {r5, r6, lr}
	movs r3, #192
	lsls r3, r3, #18
	adds r3, #220
	ldr r6, [r3]
	ldr r3, .L_080f8aa0
	ldr r5, [r6, #20]
	ldr r4, [r3]
	ldr r6, [r6, #16]
	ldr r2, .L_080f8aa4
	movs r3, #7
	lsrs r4, r4, #1
	ands r4, r3
	ldrb r2, [r2, r4]
	ldrh r3, [r6, #12]
	mov r12, r6
	adds r2, r2, r0
	ldr r6, .L_080f8a98
	lsls r3, r3, #3
	adds r2, r2, r3
	ldr r3, .L_080f8a9c
	adds r2, #8
	strh r2, [r5, #6]
	ands r2, r6
	ldrh r0, [r5, #22]
	ands r2, r3
	ldr r3, .L_080f8aa8
	ands r3, r0
	orrs r3, r2
	strh r3, [r5, #22]
	ldr r3, .L_080f8aac
	mov r0, r12
	ldrb r3, [r3, r4]
	ldrh r2, [r0, #14]
	adds r3, r3, r1
	lsls r2, r2, #3
	adds r3, r3, r2
	adds r3, #8
	strh r3, [r5, #8]
	ands r3, r6
	strb r3, [r5, #20]
	b .L_080f8ab0
.L_080f8a98:
	.4byte 0x0000ffff
.L_080f8a9c:
	.4byte 0x000001ff
.L_080f8aa0:
	.4byte Data_0300122c
.L_080f8aa4:
	.4byte Data_081059dc
.L_080f8aa8:
	.4byte 0xfffffe00
.L_080f8aac:
	.4byte Data_081059e5
.L_080f8ab0:
	pop {r5, r6, pc}
	.2byte 0x0000
