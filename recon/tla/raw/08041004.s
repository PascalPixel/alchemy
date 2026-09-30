.syntax unified
	.thumb
	.global Func_08041004
	.thumb_func
Func_08041004:
	push {lr}
	movs r3, #192
	lsls r3, r3, #18
	adds r3, #208
	ldr r0, [r3]
	movs r3, #160
	lsls r3, r3, #3
	adds r3, #164
	adds r0, r0, r3
	bl Func_08108030
	pop {pc}
