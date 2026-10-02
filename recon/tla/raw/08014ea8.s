.syntax unified
	.thumb
	.global Func_08014ea8
	.thumb_func
Func_08014ea8:
	push {lr}
	ldr r2, .L_08014ed4
	ldr r3, [r2]
	cmp r3, #0
	ble .L_08014ed0
	subs r3, #1
	str r3, [r2]
	ldr r3, .L_08014ed8
	movs r2, #132
	ldr r0, [r3]
	lsls r2, r2, #24
	subs r0, #48
	str r0, [r3]
	movs r3, #128
	lsls r3, r3, #19
	adds r3, #212
	ldr r1, .L_08014edc
	adds r2, #12
	stmia r3!, {r0, r1, r2}
	subs r3, #12
.L_08014ed0:
	pop {pc}
	.2byte 0x0000
.L_08014ed4:
	.4byte gTransformStackDepth
.L_08014ed8:
	.4byte gTransformStackTop
.L_08014edc:
	.4byte gTransform
