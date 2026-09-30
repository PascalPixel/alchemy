.syntax unified
	.thumb
	.global Func_080d1e60
	.thumb_func
Func_080d1e60:
	push {lr}
	adds r2, r0, #0
	ldr r0, .L_080d1e80
	movs r1, #0
	ldrh r3, [r0]
	cmp r3, r2
	beq .L_080d1e7c
.L_080d1e6e:
	adds r1, #1
	adds r0, #4
	cmp r1, #242
	bhi .L_080d1e7c
	ldrh r3, [r0]
	cmp r3, r2
	bne .L_080d1e6e
.L_080d1e7c:
	pop {pc}
	.2byte 0x0000
.L_080d1e80:
	.4byte Data_080f0308
