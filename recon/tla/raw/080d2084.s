.syntax unified
	.thumb
	.global Func_080d2084
	.thumb_func
Func_080d2084:
	push {r5, r6, lr}
	ldr r6, .L_080d20a0
	movs r5, #0
	b .L_080d208e
.L_080d208c:
	adds r5, #1
.L_080d208e:
	cmp r5, #119
	bhi .L_080d209e
	movs r0, #1
	bl WaitFrames
	ldr r3, [r6, #4]
	cmp r3, #0
	beq .L_080d208c
.L_080d209e:
	pop {r5, r6, pc}
.L_080d20a0:
	.4byte gInput
