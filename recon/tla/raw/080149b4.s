.syntax unified
	.thumb
	.global Func_080149b4
	.thumb_func
Func_080149b4:
	push {r5, r6, lr}
	movs r5, #0
	movs r4, #15
	movs r6, #1
.L_080149bc:
	adds r3, r4, #1
	adds r2, r5, #0
	lsls r2, r3
	lsls r1, r4, #1
	adds r3, r6, #0
	lsls r3, r1
	adds r2, r2, r3
	cmp r2, r0
	bgt .L_080149d6
	adds r3, r6, #0
	lsls r3, r4
	orrs r5, r3
	subs r0, r0, r2
.L_080149d6:
	subs r4, #1
	cmp r4, #0
	bge .L_080149bc
	adds r0, r5, #0
	pop {r5, r6, pc}
