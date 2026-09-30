.syntax unified
	.thumb
	.global Func_080dc0b8
	.thumb_func
Func_080dc0b8:
	push {lr}
	ldr r1, .L_080dc0d4
	movs r2, #128
	movs r0, #0
	lsls r2, r2, #2
.L_080dc0c2:
	ldrb r3, [r1]
	adds r1, #1
	cmp r3, #255
	bne .L_080dc0cc
	adds r0, #1
.L_080dc0cc:
	subs r2, #1
	cmp r2, #0
	bne .L_080dc0c2
	pop {pc}
.L_080dc0d4:
	.4byte Data_02003410
