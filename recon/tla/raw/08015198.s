.syntax unified
	.thumb
	.global Func_08015198
	.thumb_func
Func_08015198:
	ldr r3, [r0]
	ldr r2, .L_080151a8
	str r3, [r2, #36]
	ldr r3, [r0, #4]
	str r3, [r2, #40]
	ldr r3, [r0, #8]
	str r3, [r2, #44]
	bx lr
.L_080151a8:
	.4byte gTransform
