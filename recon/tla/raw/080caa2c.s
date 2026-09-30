.syntax unified
	.thumb
	.global Func_080caa2c
	.thumb_func
Func_080caa2c:
	push {r5, r6, lr}
	movs r3, #192
	lsls r3, r3, #18
	adds r5, r0, #0
	ldr r6, [r3, #108]
	bl ObjectTable_Get
	cmp r0, #0
	beq .L_080caa4a
	bl Func_080200c8
	lsls r3, r5, #2
	adds r3, #20
	movs r2, #0
	str r2, [r6, r3]
.L_080caa4a:
	pop {r5, r6, pc}
