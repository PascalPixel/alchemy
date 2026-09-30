.syntax unified
	.thumb
	.global Func_080168cc
	.thumb_func
Func_080168cc:
	push {r5, r6, lr}
	ldr r2, .L_080168f0
	movs r5, #0
	ldr r3, [r2]
	cmp r3, #0
	beq .L_080168ee
	adds r6, r2, #0
.L_080168da:
	movs r0, #1
	bl WaitFrames
	ldr r3, .L_080168f4
	adds r5, #1
	cmp r5, r3
	bhi .L_080168ee
	ldr r3, [r6]
	cmp r3, #0
	bne .L_080168da
.L_080168ee:
	pop {r5, r6, pc}
.L_080168f0:
	.4byte Data_020055d0
.L_080168f4:
	.4byte 0x000927bf
