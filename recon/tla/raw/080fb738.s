.syntax unified
	.thumb
	.global Func_080fb738
	.thumb_func
Func_080fb738:
	push {r5, r6, r7, lr}
	movs r3, #192
	lsls r3, r3, #18
	adds r3, #220
	ldr r6, [r3]
	sub sp, #8
	movs r3, #10
	adds r7, r6, #0
	str r3, [sp, #0]
	adds r7, #52
	movs r3, #2
	str r3, [sp, #4]
	adds r5, r0, #0
	movs r3, #13
	movs r1, #0
	movs r2, #0
	adds r0, r7, #0
	bl UiWindow_UpdateOrCreate
	bl Func_080f92ac
	movs r3, #182
	lsls r3, r3, #1
	lsls r5, r5, #1
	adds r5, r5, r3
	ldrh r3, [r6, r5]
	cmp r3, #0
	beq .L_080fb778
	ldr r0, [r7]
	adds r1, r3, #0
	bl Func_080fb8ac
.L_080fb778:
	movs r0, #1
	add sp, #8
	pop {r5, r6, r7, pc}
	.2byte 0x0000
