.syntax unified
	.thumb
	.global Func_080ca1a4
	.thumb_func
Func_080ca1a4:
	push {lr}
	ldr r3, .L_080ca1b8
	lsls r0, r0, #2
	adds r0, r0, r3
	ldrh r3, [r0]
	ldrh r1, [r0, #2]
	adds r0, r3, #0
	bl Func_080ca18c
	pop {pc}
.L_080ca1b8:
	.4byte Data_080eef54
