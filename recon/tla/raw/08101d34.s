.syntax unified
	.thumb
	.global Func_08101d34
	.thumb_func
Func_08101d34:
	push {r5, r6, r7, lr}
	adds r4, r0, #0
	movs r7, #12
	ldrsh r0, [r4, r7]
	sub sp, #4
	adds r0, r0, r1
	movs r7, #14
	ldrsh r1, [r4, r7]
	ldr r5, [sp, #24]
	adds r6, r3, #0
	adds r1, r1, r2
	ldr r3, [sp, #20]
	adds r0, #1
	adds r1, #1
	adds r2, r6, #0
	str r5, [sp, #0]
	bl Func_08101c7c
	add sp, #4
	pop {r5, r6, r7, pc}
