.syntax unified
	.thumb
	.global Func_08014e90
	.thumb_func
Func_08014e90:
	movs r3, #128
	movs r2, #132
	lsls r3, r3, #19
	lsls r2, r2, #24
	adds r3, #212
	ldr r1, .L_08014ea4
	adds r2, #12
	stmia r3!, {r0, r1, r2}
	subs r3, #12
	bx lr
.L_08014ea4:
	.4byte gTransform
