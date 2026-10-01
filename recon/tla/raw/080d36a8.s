.syntax unified
	.thumb
	.global Func_080d36a8
	.thumb_func
Func_080d36a8:
	push {r5, r6, lr}
	adds r5, r0, #0
	bl ObjectTable_Get
	movs r3, #192
	lsls r3, r3, #18
	ldr r6, [r3, #108]
	cmp r0, #0
	beq .L_080d36c6
	bl Object_Destroy
	lsls r3, r5, #2
	adds r3, #20
	movs r2, #0
	str r2, [r6, r3]
.L_080d36c6:
	pop {r5, r6, pc}
