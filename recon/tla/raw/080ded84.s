.syntax unified
	.thumb
	.global Func_080ded84
	.thumb_func
Func_080ded84:
	push {r5, r6, lr}
	adds r6, r0, #0
	ldr r2, [r6, #12]
	ldr r3, [r6, #20]
	cmp r2, r3
	bgt .L_080deda2
	adds r2, r6, #0
	adds r2, #94
	movs r3, #2
	movs r5, #0
	strh r3, [r2]
	ldr r1, .L_080deda4
	bl Object_SetCallback
	str r5, [r6, #108]
.L_080deda2:
	pop {r5, r6, pc}
.L_080deda4:
	.4byte Data_080f0e54
