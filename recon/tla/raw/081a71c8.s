.syntax unified
	.thumb
	.global Func_081a71c8
	.thumb_func
Func_081a71c8:
	push {r5, r6, lr}
	ldr r5, .L_081a728c
	sub sp, #4
	ldrb r3, [r5]
	mov r6, sp
	adds r6, #3
	strb r3, [r6]
	bl Func_08014b70
	bl Func_08014368
	movs r0, #1
	bl WaitFrames
	bl Scheduler_ResetTaskTable
	ldr r2, .L_081a7290
	movs r3, #0
	strb r3, [r2]
	strb r3, [r5]
	bl Func_081a6030
	movs r0, #0
	bl Func_081a6154
	adds r5, r0, #0
	cmp r5, #0
	beq .L_081a722c
	bl Func_081a6638
	adds r5, r0, #0
	cmp r5, #0
	beq .L_081a722c
	movs r0, #1
	bl Func_081a6154
	adds r5, r0, #0
	cmp r5, #0
	beq .L_081a722c
	bl Func_081a6724
	adds r5, r0, #0
	cmp r5, #0
	beq .L_081a722c
	movs r0, #2
	bl Func_081a6154
	adds r5, r0, #0
	cmp r5, #0
	bne .L_081a7244
.L_081a722c:
	movs r0, #0
	movs r1, #0
	bl Func_081a81d4
	movs r0, #10
	bl Func_081a8228
	movs r0, #10
	bl WaitFrames
	cmp r5, #0
	beq .L_081a7248
.L_081a7244:
	bl Func_081a6ea0
.L_081a7248:
	ldr r2, .L_081a7290
	movs r3, #1
	strb r3, [r2]
	movs r0, #1
	bl WaitFrames
	ldr r3, .L_081a7294
	movs r2, #0
	movs r5, #3
.L_081a725a:
	subs r5, #1
	strh r2, [r3, #2]
	strh r2, [r3]
	adds r3, #4
	cmp r5, #0
	bge .L_081a725a
	ldr r3, .L_081a7288
	movs r2, #128
	lsls r2, r2, #19
	strh r3, [r2]
	bl Func_08014bac
	bl Func_08014b70
	bl Func_081a814c
	ldr r2, .L_081a728c
	ldrb r3, [r6]
	movs r0, #1
	strb r3, [r2]
	add sp, #4
	b .L_081a7298
	.2byte 0x0000
.L_081a7288:
	.4byte 0x00000000
.L_081a728c:
	.4byte gCpuLoadDisplayEnabled
.L_081a7290:
	.4byte gRenderOamEnabled
.L_081a7294:
	.4byte Data_03001120
.L_081a7298:
	pop {r5, r6, pc}
	.2byte 0x0000
