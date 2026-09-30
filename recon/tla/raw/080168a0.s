.syntax unified
	.thumb
	.global Func_080168a0
	.thumb_func
Func_080168a0:
	push {r5, r6, lr}
	ldr r2, .L_080168c4
	movs r5, #0
	ldr r3, [r2]
	cmp r3, #0
	beq .L_080168c2
	adds r6, r2, #0
.L_080168ae:
	movs r0, #1
	bl WaitFrames
	ldr r3, .L_080168c8
	adds r5, #1
	cmp r5, r3
	bhi .L_080168c2
	ldr r3, [r6]
	cmp r3, #0
	bne .L_080168ae
.L_080168c2:
	pop {r5, r6, pc}
.L_080168c4:
	.4byte Data_020038d0
.L_080168c8:
	.4byte 0x000927bf
