.syntax unified
	.thumb
	.global Func_081a0674
	.thumb_func
Func_081a0674:
	push {lr}
	ldr r1, .L_081a06b0
	movs r2, #128
	lsls r2, r2, #9
	movs r3, #31
.L_081a067e:
	subs r3, #1
	stmia r0!, {r1}
	cmp r3, #0
	bge .L_081a067e
	movs r4, #128
	lsls r4, r4, #10
	adds r4, #2
	movs r3, #239
.L_081a068e:
	subs r3, #1
	stmia r0!, {r2}
	adds r2, r2, r4
	cmp r3, #0
	bge .L_081a068e
	movs r3, #47
.L_081a069a:
	subs r3, #1
	stmia r0!, {r1}
	cmp r3, #0
	bge .L_081a069a
	movs r2, #0
	movs r3, #191
.L_081a06a6:
	subs r3, #1
	stmia r0!, {r2}
	cmp r3, #0
	bge .L_081a06a6
	pop {pc}
.L_081a06b0:
	.4byte 0x01ff01ff
