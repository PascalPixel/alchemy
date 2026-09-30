.syntax unified
	.thumb
	.global Func_0803f900
	.thumb_func
Func_0803f900:
	push {lr}
	movs r3, #192
	lsls r3, r3, #18
	ldr r1, [r3, #108]
	movs r0, #136
	lsls r0, r0, #2
	adds r2, r1, r0
	ldrh r3, [r2]
	movs r0, #255
	lsls r0, r0, #8
	adds r0, #255
	adds r3, r3, r0
	strh r3, [r2]
	lsls r3, r3, #16
	cmp r3, #0
	bne .L_0803f934
	movs r2, #135
	lsls r2, r2, #2
	adds r3, r1, r2
	ldr r0, [r3]
	movs r1, #2
	bl UiWork_Finalize
	ldr r0, .L_0803f938
	bl Func_08014644
.L_0803f934:
	pop {pc}
	.2byte 0x0000
.L_0803f938:
	.4byte Func_0803f900
