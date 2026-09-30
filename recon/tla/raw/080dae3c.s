.syntax unified
	.thumb
	.global Func_080dae3c
	.thumb_func
Func_080dae3c:
	push {lr}
	movs r3, #192
	lsls r3, r3, #18
	adds r3, #164
	ldr r4, [r3]
	movs r0, #0
	ldr r3, [r4, #28]
	adds r1, r0, #0
	cmp r3, #0
	bne .L_080dae54
	adds r0, r4, #4
	b .L_080dae6e
.L_080dae54:
	adds r1, #1
	cmp r1, #7
	bgt .L_080dae6e
	lsls r3, r1, #3
	subs r3, r3, r1
	lsls r2, r3, #2
	adds r3, r2, #0
	adds r3, #28
	ldr r3, [r4, r3]
	cmp r3, #0
	bne .L_080dae54
	adds r3, r4, r2
	adds r0, r3, #4
.L_080dae6e:
	pop {pc}
