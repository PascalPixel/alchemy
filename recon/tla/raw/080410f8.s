.syntax unified
	.thumb
	.global Func_080410f8
	.thumb_func
Func_080410f8:
	push {r5, r6, r7, lr}
	mov r7, r10
	mov r6, r8
	push {r6, r7}
	adds r5, r0, #0
	bl Func_0804101c
	movs r3, #192
	lsls r3, r3, #18
	adds r3, #208
	ldr r3, [r3]
	movs r0, #1
	mov r10, r3
	bl WaitFrames
	bl Func_08041068
	movs r7, #1
	negs r7, r7
	mov r8, r0
.L_08041120:
	cmp r7, #0
	beq .L_08041170
	adds r0, r5, #0
	movs r1, #9
	adds r0, #9
	bl __modsi3
	mov r1, r8
	adds r5, r0, #0
	movs r0, #12
	ldrsh r3, [r1, r0]
	mov r0, r8
	lsls r3, r3, #3
	subs r1, r3, #4
	movs r2, #14
	ldrsh r3, [r0, r2]
	lsls r2, r5, #4
	lsls r3, r3, #3
	adds r3, r3, r2
	adds r2, r3, #0
	movs r3, #1
	negs r3, r3
	adds r2, #12
	cmp r7, r3
	bne .L_08041160
	movs r0, #160
	lsls r0, r0, #3
	adds r0, #164
	add r0, r10
	bl Func_08108048
	b .L_0804116e
.L_08041160:
	movs r0, #160
	lsls r0, r0, #3
	adds r0, #164
	add r0, r10
	movs r3, #3
	bl Func_08108040
.L_0804116e:
	movs r7, #0
.L_08041170:
	movs r0, #1
	bl WaitFrames
	ldr r6, .L_080411fc
	movs r2, #2
	ldr r3, [r6, #4]
	ands r3, r2
	cmp r3, #0
	beq .L_0804118e
	movs r0, #113
	movs r5, #1
	bl Audio_PlayCue
	negs r5, r5
	b .L_080411ca
.L_0804118e:
	ldr r3, [r6, #4]
	movs r2, #1
	ands r3, r2
	cmp r3, #0
	beq .L_080411a0
	movs r0, #112
	bl Audio_PlayCue
	b .L_080411ca
.L_080411a0:
	ldr r3, [r6, #12]
	movs r2, #64
	ands r3, r2
	cmp r3, #0
	beq .L_080411b4
	movs r0, #111
	subs r5, #1
	movs r7, #1
	bl Audio_PlayCue
.L_080411b4:
	ldr r3, [r6, #12]
	movs r2, #128
	ands r3, r2
	cmp r3, #0
	beq .L_08041120
	movs r0, #111
	adds r5, #1
	movs r7, #1
	bl Audio_PlayCue
	b .L_08041120
.L_080411ca:
	mov r0, r8
	movs r1, #2
	bl UiWork_Finalize
	bl Func_08041054
	movs r0, #1
	bl WaitFrames
	movs r0, #1
	negs r0, r0
	cmp r5, r0
	beq .L_080411f0
	ldr r3, .L_08041200
	lsls r2, r5, #3
	adds r2, #4
	ldr r0, [r3, r2]
	mov lr, r0
	.2byte 0xf800
.L_080411f0:
	adds r0, r5, #0
	pop {r3, r5}
	mov r8, r3
	mov r10, r5
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_080411fc:
	.4byte gInput
.L_08041200:
	.4byte Data_080aa128
