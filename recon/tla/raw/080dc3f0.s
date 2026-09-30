.syntax unified
	.thumb
	.global Func_080dc3f0
	.thumb_func
Func_080dc3f0:
	push {r5, r6, lr}
	movs r3, #192
	lsls r3, r3, #18
	adds r3, #224
	ldr r3, [r3]
	movs r6, #23
	adds r5, r3, #0
	adds r5, #80
.L_080dc400:
	adds r0, r5, #0
	subs r6, #1
	bl Func_080ebc30
	adds r5, #72
	cmp r6, #0
	bge .L_080dc400
	pop {r5, r6, pc}
