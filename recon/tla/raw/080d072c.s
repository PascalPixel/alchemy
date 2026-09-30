.syntax unified
	.thumb
	.global Func_080d072c
	.thumb_func
Func_080d072c:
	ldr r3, .L_080d0740
	movs r0, #1
	ldr r2, [r3, #4]
	ldr r3, .L_080d0744
	eors r2, r3
	negs r3, r2
	orrs r3, r2
	lsrs r3, r3, #31
	subs r0, r0, r3
	bx lr
.L_080d0740:
	.4byte Data_030001e4
.L_080d0744:
	.4byte Func_080d0954
