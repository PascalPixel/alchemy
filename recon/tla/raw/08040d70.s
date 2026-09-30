.syntax unified
	.thumb
	.global Func_08040d70
	.thumb_func
Func_08040d70:
	push {r5, r6, r7, lr}
	mov r7, r10
	mov r6, r8
	push {r6, r7}
	adds r5, r0, #0
	bl Func_08040c78
	movs r3, #192
	lsls r3, r3, #18
	adds r3, #208
	ldr r3, [r3]
	movs r0, #1
	mov r10, r3
	bl WaitFrames
	bl Func_08040cc4
	movs r7, #1
	negs r7, r7
	mov r8, r0
.L_08040d98:
	cmp r7, #0
	beq .L_08040de8
	adds r0, r5, #5
	movs r1, #5
	bl Math_Mod
	mov r1, r8
	adds r5, r0, #0
	movs r0, #12
	ldrsh r3, [r1, r0]
	mov r0, r8
	lsls r3, r3, #3
	subs r1, r3, #4
	movs r3, #14
	ldrsh r2, [r0, r3]
	lsls r3, r5, #1
	adds r3, r3, r5
	adds r3, r3, r2
	lsls r3, r3, #3
	adds r2, r3, #0
	movs r3, #1
	negs r3, r3
	adds r2, #16
	cmp r7, r3
	bne .L_08040dd8
	movs r0, #160
	lsls r0, r0, #3
	adds r0, #164
	add r0, r10
	bl Func_08108048
	b .L_08040de6
.L_08040dd8:
	movs r0, #160
	lsls r0, r0, #3
	adds r0, #164
	add r0, r10
	movs r3, #3
	bl Func_08108040
.L_08040de6:
	movs r7, #0
.L_08040de8:
	movs r0, #1
	bl WaitFrames
	ldr r6, .L_08040e60
	movs r2, #2
	ldr r3, [r6, #4]
	ands r3, r2
	cmp r3, #0
	beq .L_08040e06
	movs r0, #113
	movs r5, #1
	bl Audio_PlayCue
	negs r5, r5
	b .L_08040e42
.L_08040e06:
	ldr r3, [r6, #4]
	movs r2, #1
	ands r3, r2
	cmp r3, #0
	beq .L_08040e18
	movs r0, #112
	bl Audio_PlayCue
	b .L_08040e42
.L_08040e18:
	ldr r3, [r6, #12]
	movs r2, #64
	ands r3, r2
	cmp r3, #0
	beq .L_08040e2c
	movs r0, #111
	subs r5, #1
	movs r7, #1
	bl Audio_PlayCue
.L_08040e2c:
	ldr r3, [r6, #12]
	movs r2, #128
	ands r3, r2
	cmp r3, #0
	beq .L_08040d98
	movs r0, #111
	adds r5, #1
	movs r7, #1
	bl Audio_PlayCue
	b .L_08040d98
.L_08040e42:
	mov r0, r8
	movs r1, #2
	bl UiWork_Finalize
	bl Func_08040cb0
	movs r0, #1
	bl WaitFrames
	adds r0, r5, #0
	pop {r3, r5}
	mov r8, r3
	mov r10, r5
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_08040e60:
	.4byte gInput
