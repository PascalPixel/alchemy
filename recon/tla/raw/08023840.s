.syntax unified
	.thumb
	.global Func_08023840
	.thumb_func
Func_08023840:
	push {lr}
	ldr r0, .L_08023860
	bl Scheduler_DisableCallbacks
	ldr r0, .L_08023864
	bl Scheduler_DisableCallbacks
	movs r1, #128
	lsls r1, r1, #19
	ldrh r2, [r1]
	movs r3, #225
	lsls r3, r3, #8
	adds r3, #255
	ands r3, r2
	strh r3, [r1]
	pop {pc}
.L_08023860:
	.4byte Func_0802386c
.L_08023864:
	.4byte Func_08023e18
