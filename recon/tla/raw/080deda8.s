.syntax unified
	.thumb
	.global Func_080deda8
	.thumb_func
Func_080deda8:
	push {r5, r6, lr}
	adds r6, r0, #0
	ldr r2, .L_080dedd8
	ldr r3, [r6, #12]
	adds r3, r3, r2
	str r3, [r6, #12]
	bl Random16
	adds r5, r0, #0
	bl Random16
	ldr r3, [r6, #8]
	subs r5, r5, r0
	adds r3, r3, r5
	str r3, [r6, #8]
	ldr r2, [r6, #12]
	ldr r3, [r6, #20]
	cmp r2, r3
	bgt .L_080dedd6
	ldr r1, .L_080deddc
	adds r0, r6, #0
	bl Object_SetCallback
.L_080dedd6:
	pop {r5, r6, pc}
.L_080dedd8:
	.4byte 0xffffb334
.L_080deddc:
	.4byte Data_080f0e54
