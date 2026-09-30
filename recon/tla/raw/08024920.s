.syntax unified
	.thumb
	.global Func_08024920
	.thumb_func
Func_08024920:
	push {lr}
	movs r3, #224
	lsls r3, r3, #3
	adds r2, r0, r3
.L_08024928:
	ldrh r3, [r0, #2]
	cmp r3, #0
	beq .L_08024930
	stmia r1!, {r0}
.L_08024930:
	adds r0, #128
	cmp r0, r2
	bne .L_08024928
	movs r3, #0
	str r3, [r1]
	pop {pc}
