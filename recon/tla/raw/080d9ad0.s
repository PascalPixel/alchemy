.syntax unified
	.thumb
	.global Func_080d9ad0
	.thumb_func
Func_080d9ad0:
	push {r5, r6, r7, lr}
	adds r7, r3, #0
	movs r3, #192
	lsls r3, r3, #18
	adds r3, #156
	ldr r3, [r3]
	adds r5, r0, #0
	adds r6, r2, #0
	lsls r2, r5, #5
	adds r3, r3, r2
	strh r1, [r3, #20]
	adds r0, r1, #0
	bl GameFlag_Test
	cmp r0, #0
	bne .L_080d9afc
	adds r0, r5, #0
	adds r1, r6, #0
	adds r2, r7, #0
	bl Func_080d9104
	b .L_080d9b06
.L_080d9afc:
	adds r0, r5, #0
	adds r1, r6, #0
	adds r2, r7, #0
	bl Func_080d8d68
.L_080d9b06:
	pop {r5, r6, r7, pc}
