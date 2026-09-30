.syntax unified
	.thumb
	.global Func_0801480c
	.thumb_func
Func_0801480c:
	push {lr}
	ldr r3, .L_08014838
	movs r4, #0
	ldrb r3, [r3]
	ldr r1, .L_0801483c
	asrs r0, r0, #8
	cmp r3, #1
	bne .L_08014836
	movs r2, #25
	subs r1, #8
.L_08014820:
	subs r2, #1
	cmp r2, #0
	bne .L_0801482a
	adds r0, r4, #0
	b .L_08014836
.L_0801482a:
	adds r1, #8
	ldrb r3, [r1, #5]
	cmp r3, r0
	bne .L_08014820
	adds r4, #1
	b .L_08014820
.L_08014836:
	pop {pc}
.L_08014838:
	.4byte gSchedulerTaskCount
.L_0801483c:
	.4byte gSchedulerTaskTable
