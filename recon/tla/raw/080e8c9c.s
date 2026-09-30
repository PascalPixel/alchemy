.syntax unified
	.thumb
	.global Func_080e8c9c
	.thumb_func
Func_080e8c9c:
	push {r5, lr}
	movs r3, #192
	lsls r3, r3, #18
	ldr r1, [r3, #32]
	adds r2, r3, #0
	adds r2, #224
	ldr r0, [r3, #92]
	ldr r4, [r2]
	ldr r2, [r3, #108]
	adds r3, r1, #0
	adds r3, #252
	ldr r3, [r3]
	cmp r3, #0
	bne .L_080e8cc8
	movs r5, #197
	lsls r5, r5, #1
	adds r3, r2, r5
	ldrb r3, [r3]
	lsls r3, r3, #24
	asrs r3, r3, #24
	cmp r3, #3
	bne .L_080e8cd4
.L_080e8cc8:
	movs r0, #0
	movs r1, #0
	movs r2, #0
	bl Func_08020228
	b .L_080e8cf8
.L_080e8cd4:
	ldr r3, [r0, #12]
	adds r2, r1, #0
	adds r2, #236
	str r3, [r2]
	adds r2, #8
	ldr r3, [r0, #16]
	movs r1, #1
	str r3, [r2]
	subs r2, #4
	ldr r3, [r0, #20]
	str r3, [r2]
	adds r2, #8
	ldr r3, [r0, #24]
	str r3, [r2]
	movs r3, #24
	ldrsh r0, [r4, r3]
	bl Object_AttachWorkTargetToObject
.L_080e8cf8:
	pop {r5, pc}
	.2byte 0x0000
