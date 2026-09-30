.syntax unified
	.thumb
	.global Func_081263c4
	.thumb_func
Func_081263c4:
	push {r5, r6, lr}
	movs r5, #168
	lsls r5, r5, #2
	adds r1, r5, #0
	movs r0, #40
	bl Runtime_AllocateBlock
	movs r3, #192
	lsls r3, r3, #18
	adds r3, #176
	ldr r6, [r3]
	ldr r3, .L_081263ec
	movs r2, #0
	adds r1, r5, #0
	mov lr, r3
	.2byte 0xf800
	movs r3, #0
	str r3, [r6, #8]
	pop {r5, r6, pc}
	.2byte 0x0000
.L_081263ec:
	.4byte IwramFillWords
