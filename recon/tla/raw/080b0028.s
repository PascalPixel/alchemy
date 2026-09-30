.syntax unified
	.thumb
	.global Func_080b0028
	.thumb_func
Func_080b0028:
	push {r5, r6, lr}
	adds r6, r1, #0
	sub sp, #16
	bl Owner_GetState
	adds r1, r0, #0
	movs r0, #0
	cmp r6, #3
	bgt .L_080b0056
	movs r2, #165
	lsls r2, r2, #1
	adds r3, r1, r2
	mov r5, sp
	ldrh r0, [r3]
	adds r1, #248
	adds r2, r5, #0
	bl Func_080affac
	lsls r3, r6, #2
	ldr r0, [r5, r3]
	movs r1, #10
	bl Math_Div
.L_080b0056:
	add sp, #16
	pop {r5, r6, pc}
	.2byte 0x0000
