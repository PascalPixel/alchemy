.syntax unified
	.thumb
	.global Func_0813d064
	.thumb_func
Func_0813d064:
	push {r5, lr}
	adds r5, r0, #0
	movs r3, #36
	ldrsh r1, [r5, r3]
	ldr r0, [r5, #8]
	movs r3, #20
	movs r2, #4
	bl Func_08157530
	movs r0, #2
	bl WaitFrames
	ldr r3, [r5, #24]
	cmp r3, #0
	bne .L_0813d08c
	adds r0, r5, #0
	movs r1, #12
	bl Func_0813d098
	b .L_0813d094
.L_0813d08c:
	adds r0, r5, #0
	movs r1, #13
	bl Func_0813d098
.L_0813d094:
	pop {r5, pc}
	.2byte 0x0000
