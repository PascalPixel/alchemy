.syntax unified
	.thumb
	.global Func_080fa368
	.thumb_func
Func_080fa368:
	push {r5, r6, r7, lr}
	movs r3, #192
	lsls r3, r3, #18
	adds r3, #220
	ldr r7, [r3]
	sub sp, #4
	adds r0, r7, #0
	bl Func_080f88d0
	movs r1, #2
	movs r2, #2
	bl Func_08104fe0
	movs r0, #151
	ldr r1, .L_080fa390
	lsls r0, r0, #1
	movs r2, #3
	adds r3, r7, r0
	b .L_080fa394
	.2byte 0x0000
.L_080fa390:
	.4byte 0x0000001a
.L_080fa394:
	subs r2, #1
	strh r1, [r3]
	subs r3, #2
	cmp r2, #0
	bge .L_080fa394
	movs r5, #0
	str r5, [r7, #44]
	str r5, [r7, #40]
	movs r6, #2
	movs r1, #17
	movs r2, #30
	movs r3, #3
	movs r0, #0
	str r6, [sp, #0]
	bl UiWindow_CreateFar
	adds r3, r7, #0
	adds r3, #244
	str r0, [r7, #48]
	str r5, [r7, #36]
	adds r2, r7, #0
	strb r5, [r3]
	adds r3, #1
	strb r5, [r3]
	adds r2, #246
	movs r3, #8
	strb r3, [r2]
	adds r3, r7, #0
	adds r3, #247
	strb r6, [r3]
	add sp, #4
	pop {r5, r6, r7, pc}
