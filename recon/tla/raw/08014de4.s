.syntax unified
	.thumb
	.global Func_08014de4
	.thumb_func
Func_08014de4:
	push {r5, lr}
	movs r1, #48
	movs r0, #8
	ldr r5, .L_08014e10
	bl Runtime_AllocateBlock
	ldr r2, .L_08014e14
	movs r3, #0
	str r3, [r2]
	str r0, [r5]
	ldr r3, .L_08014e18
	adds r0, r3, #0
	movs r1, #128
	lsls r1, r1, #9
	movs r2, #0
	movs r3, #0
	movs r4, #0
	stmia r0!, {r1, r2, r3, r4}
	stmia r0!, {r1, r2, r3, r4}
	stmia r0!, {r1, r2, r3, r4}
	pop {r5, pc}
	.2byte 0x0000
.L_08014e10:
	.4byte gTransformStackTop
.L_08014e14:
	.4byte gTransformStackDepth
.L_08014e18:
	.4byte gTransform
