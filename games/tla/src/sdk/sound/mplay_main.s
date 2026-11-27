@ Copies of four of the functions of games/common/sdk/sound/sound.c and the
@ head of a fifth, which the cartridge keeps 0x1a70 bytes before them, and an
@ ARM high multiply after them. Nothing refers to any of them.
@ Each copy's bytes are its twin's but for what a link fills in: its pool
@ words are 0 where the twin's hold addresses (the block copy's control word
@ is kept), and its call to Bios_CpuSet, outside the SDK's sound library, is
@ a call to itself. Its other calls keep the twin's offsets, so each lands
@ 0x1a70 bytes before the twin's callee. The fifth stops short after the
@ player slot's offset, its two loads reaching 8 bytes further than its
@ twin's.
@ Why assembly: copies of functions the ROM also links, their calls keeping
@ those functions' offsets, their pool words never filled in and the last cut
@ short; the multiply switches to ARM inside its Thumb entry.
@ Why assembly: bx pc switches to ARM for umlalne and umull, a mode switch
@ inside a function; the nop after bx pc is never run.
@ Why SoundChannel_Unlink is assembly:
@ bcc on the flags of subs r1, #20 with no compare.
@ Why SoundDriver_CalculateStereoVolume is assembly:
@ it takes its channel and track in r4 and r5, a linkage no procedure-call
@ standard produces, with its two clamps placed before its entry.
@ Why ply_note is assembly:
@ it picks its channel with adcs r2, r2 on the carry of lsrs r1, r1, #7.
@ MusicPlayer2000's note-on, pret's ChnVolSetAsm and ply_note: the stereo
@ volume of a channel from its track's volume and pan, and the song command
@ that starts a note on a free or the weakest channel. Func_081c08a4 is a
@ label inside it that the leftover copy in leftover_driver.s branches to.
@ Why assembly: push {r4, r5, r6, r7} saves without lr, after pop {r0} has
@ taken back the r0 pushed with lr; r11 is an ordinary register, the mask of
@ the tracks still playing (mov r11, r4), not a frame pointer; bcc consumes
@ the carry of lsrs r0, r3, #7; mov lr, r3 and a lone BL suffix
@ (.inst.n 0xf800) call through lr. The bx r3 after its return is the
@ driver's call through r3.
@ MusicPlayer2000's MPlayMain, run once a VBlank: runs a player's callback,
@ steps its fade and, as often as its tempo allows, every track's song
@ commands and notes, then updates each channel's volume and pitch.
	.section .text,"ax",%progbits
	.section .rodata,"a",%progbits
	.section .text.channel_unlink,"ax",%progbits
	.section .text,"ax",%progbits
	.balign 4

.syntax unified
	.thumb

@ Unused: the copy of m4aSoundInit.
	.global m4aSoundInitWorkCopy
	.thumb_func
m4aSoundInitWorkCopy:
	push	{r4, r5, r6, lr}
	ldr	r0, .L_081c0520
	movs	r1, #2
	negs	r1, r1
	ands	r0, r1
	ldr	r1, .L_081c0524
	ldr	r2, .L_081c0528
	bl	.	@ Bios_CpuSet in the twin
	ldr	r0, .L_081c052c
	bl	Func_081c08a4 + 0x28	@ SoundInit in the twin
	ldr	r0, .L_081c0530
	bl	SoundDriver_CalculateStereoVolume + 0xe	@ MPlayExtender in the twin
	ldr	r0, .L_081c0534
	bl	Func_081c0a68	@ m4aSoundMode in the twin
	ldr	r0, .L_081c0538
	lsls	r0, r0, #16
	lsrs	r0, r0, #16
	cmp	r0, #0
	beq	.L_081c051a
	ldr	r5, .L_081c053c
	adds	r6, r0, #0
.L_081c04fe:
	ldr	r4, [r5]
	ldr	r1, [r5, #4]
	ldrb	r2, [r5, #8]
	adds	r0, r4, #0
	bl	Func_081c0c0c	@ MPlayOpen in the twin
	ldrh	r0, [r5, #10]
	strb	r0, [r4, #11]
	ldr	r0, .L_081c0540
	str	r0, [r4, #24]
	adds	r5, #12
	subs	r6, #1
	cmp	r6, #0
	bne	.L_081c04fe
.L_081c051a:
	pop	{r4, r5, r6}
	pop	{r0}
	bx	r0
.L_081c0520:
	.4byte	0	@ sySndMainRAMRom
.L_081c0524:
	.4byte	0	@ Ram_SoundMixer
.L_081c0528:
	.4byte	0x04000100	@ the block copy's control word: 256 words
.L_081c052c:
	.4byte	0	@ SoundInfoWork
.L_081c0530:
	.4byte	0	@ CgbChans
.L_081c0534:
	.4byte	0	@ the initial sound mode
.L_081c0538:
	.4byte	0	@ the player count
.L_081c053c:
	.4byte	0	@ MPlayTable
.L_081c0540:
	.4byte	0	@ MPlayMemAccArea

@ Unused: the copy of m4aSoundMain.
	.global m4aSoundMainWorkCopy
	.thumb_func
m4aSoundMainWorkCopy:
	push	{lr}
	bl	Stubs_Sound - 0x7e4	@ 0x081bf81c: SoundMain in the twin
	pop	{r0}
	bx	r0
	.2byte	0

@ Unused: the copy of m4aSongNumStart.
	.global m4aSongNumStartWorkCopy
	.thumb_func
m4aSongNumStartWorkCopy:
	push	{lr}
	lsls	r0, r0, #16
	ldr	r2, .L_081c0574
	ldr	r1, .L_081c0578
	lsrs	r0, r0, #13
	adds	r0, r0, r1
	ldrh	r3, [r0, #4]
	lsls	r1, r3, #1
	adds	r1, r1, r3
	lsls	r1, r1, #2
	adds	r1, r1, r2
	ldr	r2, [r1]
	ldr	r1, [r0]
	adds	r0, r2, #0
	bl	sySndInit + 0x68	@ 0x081c0c84: MPlayStart in the twin
	pop	{r0}
	bx	r0
.L_081c0574:
	.4byte	0	@ MPlayTable
.L_081c0578:
	.4byte	0	@ SongTable

@ Unused: the copy of m4aSongNumStartOrChange.
	.global m4aSongNumStartOrChangeWorkCopy
	.thumb_func
m4aSongNumStartOrChangeWorkCopy:
	push	{lr}
	lsls	r0, r0, #16
	ldr	r2, .L_081c05a8
	ldr	r1, .L_081c05ac
	lsrs	r0, r0, #13
	adds	r0, r0, r1
	ldrh	r3, [r0, #4]
	lsls	r1, r3, #1
	adds	r1, r1, r3
	lsls	r1, r1, #2
	adds	r1, r1, r2
	ldr	r1, [r1]
	ldr	r3, [r1]
	ldr	r2, [r0]
	cmp	r3, r2
	beq	.L_081c05b0
	adds	r0, r1, #0
	adds	r1, r2, #0
	bl	sySndInit + 0x68	@ 0x081c0c84: MPlayStart in the twin
	b	.L_081c05c4
	.2byte	0
.L_081c05a8:
	.4byte	0	@ MPlayTable
.L_081c05ac:
	.4byte	0	@ SongTable
.L_081c05b0:
	ldr	r2, [r1, #4]
	ldrh	r0, [r1, #4]
	cmp	r0, #0
	beq	.L_081c05bc
	cmp	r2, #0
	bge	.L_081c05c4
.L_081c05bc:
	adds	r0, r1, #0
	adds	r1, r3, #0
	bl	sySndInit + 0x68	@ 0x081c0c84: MPlayStart in the twin
.L_081c05c4:
	pop	{r0}
	bx	r0

@ Unused: the head of a second copy of m4aSongNumStart; its two loads reach
@ words past its end, in the next function.
	.global m4aSongNumStartWorkCopyHead
	.thumb_func
m4aSongNumStartWorkCopyHead:
	push	{lr}
	lsls	r0, r0, #16
	ldr	r2, [pc, #36]
	ldr	r1, [pc, #40]
	lsrs	r0, r0, #13
	adds	r0, r0, r1
	ldrh	r3, [r0, #4]
	lsls	r1, r3, #1
	adds	r1, r1, r3
	lsls	r1, r1, #2

@ Unused: the high word of a 32 by 32 multiply, as umul3232H32
@ (games/common/sdk/sound/umul_high.s) gives it in another form: a Thumb
@ entry that switches to ARM. The head before it runs into it.
	.global umul3232H32WorkCopy
	.thumb_func
umul3232H32WorkCopy:
	bx	pc
	nop
	.arm
	umull	r3, r0, r1, r0
	bx	lr

	.section .text,"ax",%progbits
	.balign 4
.syntax unified
	.thumb
.L_081c05e8:
	movs	r1, #178
	movs	r2, #255
	b.n	.L_081c05f8
@ Never run: a zero halfword after b.n that word-aligns the next function.
AlchemyUncredited_081c05ee:
	.2byte	0x0000
AlchemyUncreditedEnd_081c05ee:
	.global SoundDriver_MidiKeyToFrequency
	.thumb_func
SoundDriver_MidiKeyToFrequency:
	ldr	r0, [r0, #4]
	mov	ip, r4
	cmp	r1, #179
	bge.n	.L_081c05e8
.L_081c05f8:
	add	r4, pc, #136
	ldrb	r4, [r1, r4]
	lsls	r1, r4, #28
	lsrs	r1, r1, #25
	add	r3, pc, #32
	adds	r1, r1, r3
	ldmia	r1, {r1, r3}
	lsrs	r4, r4, #4
	lsrs	r1, r4
	lsrs	r3, r4
	movs	r4, #0
	lsls	r2, r2, #24
	bx	pc
AlchemyUncredited_081c0612:
	nop
	.arm
AlchemyUncreditedEnd_081c0612:
	umlalne	r4, r1, r3, r2
	umull	r4, r0, r1, r0
	mov	r4, ip
	bx	lr

.syntax unified
	.section .rodata,"a",%progbits
	.arm
	.4byte	0x80000000, 0x079c7c97, 0x879c7c97, 0x08105987, 0x8facd61e, 0x088b1a34, 0x9837f052, 0x090d277a
	.4byte	0xa14517cc, 0x0996f07c, 0xaadc0848, 0x0a28eaec, 0xb504f334, 0x0ac39387, 0xbfc886bb, 0x0b676e6f
	.4byte	0xcb2ff52a, 0x0c1507a1, 0xd744fccb, 0x0cccf36f, 0xe411f03a, 0x0d8fceff, 0xf1a1bf39, 0x0e5e40c7
	.byte	0xe0, 0xe1, 0xe2, 0xe3, 0xe4, 0xe5, 0xe6, 0xe7, 0xe8, 0xe9, 0xea, 0xeb, 0xd0, 0xd1, 0xd2, 0xd3
	.byte	0xd4, 0xd5, 0xd6, 0xd7, 0xd8, 0xd9, 0xda, 0xdb, 0xc0, 0xc1, 0xc2, 0xc3, 0xc4, 0xc5, 0xc6, 0xc7
	.byte	0xc8, 0xc9, 0xca, 0xcb, 0xb0, 0xb1, 0xb2, 0xb3, 0xb4, 0xb5, 0xb6, 0xb7, 0xb8, 0xb9, 0xba, 0xbb
	.byte	0xa0, 0xa1, 0xa2, 0xa3, 0xa4, 0xa5, 0xa6, 0xa7, 0xa8, 0xa9, 0xaa, 0xab, 0x90, 0x91, 0x92, 0x93
	.byte	0x94, 0x95, 0x96, 0x97, 0x98, 0x99, 0x9a, 0x9b, 0x80, 0x81, 0x82, 0x83, 0x84, 0x85, 0x86, 0x87
	.byte	0x88, 0x89, 0x8a, 0x8b, 0x70, 0x71, 0x72, 0x73, 0x74, 0x75, 0x76, 0x77, 0x78, 0x79, 0x7a, 0x7b
	.byte	0x60, 0x61, 0x62, 0x63, 0x64, 0x65, 0x66, 0x67, 0x68, 0x69, 0x6a, 0x6b, 0x50, 0x51, 0x52, 0x53
	.byte	0x54, 0x55, 0x56, 0x57, 0x58, 0x59, 0x5a, 0x5b, 0x40, 0x41, 0x42, 0x43, 0x44, 0x45, 0x46, 0x47
	.byte	0x48, 0x49, 0x4a, 0x4b, 0x30, 0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38, 0x39, 0x3a, 0x3b
	.byte	0x20, 0x21, 0x22, 0x23, 0x24, 0x25, 0x26, 0x27, 0x28, 0x29, 0x2a, 0x2b, 0x10, 0x11, 0x12, 0x13
	.byte	0x14, 0x15, 0x16, 0x17, 0x18, 0x19, 0x1a, 0x1b, 0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07
	.byte	0x08, 0x09, 0x0a, 0x0b

	.section .text.channel_unlink,"ax",%progbits
	.balign 2
.syntax unified
	.thumb
	.global SoundChannel_Unlink
	.thumb_func
SoundChannel_Unlink:
	ldr	r1, [r0, #44]
	subs	r1, #20
	bcc.n	.L_081c0758
	adds	r0, #48
	ldmia	r0!, {r2, r3}
	cmp	r3, #0
	beq.n	.L_081c0748
	str	r2, [r3, #48]
.L_081c0748:
	cmp	r2, #0
	bne.n	.L_081c074e
	adds	r2, r1, #0
.L_081c074e:
	str	r3, [r2, #52]
	movs	r1, #0
	subs	r0, #56
	str	r1, [r0, #44]
	bx	lr
.L_081c0758:
	bx	lr
@ ply_note's own copy of clear_modM (games/common/sdk/sound/clear_buffer.s),
@ its compare placed first.
	.global clear_modM_note
	.thumb_func
clear_modM_note:
	movs	r2, #0
	strb	r2, [r1, #22]
	strb	r2, [r1, #26]
	ldrb	r3, [r1, #24]
	movs	r2, #3
	cmp	r3, #0
	bne.n	.Lclear_mod_note_flags
	movs	r2, #12
.Lclear_mod_note_flags:
	ldrb	r3, [r1, #0]
	orrs	r3, r2
	strb	r3, [r1, #0]
	bx	lr

	.section .text.channel_unlink,"ax",%progbits
	.balign 2

.syntax unified
	.thumb
.L_081c0772:
	movs	r0, #255
	b.n	.L_081c0790
.L_081c0776:
	movs	r0, #255
	b.n	.L_081c079e
	.global SoundDriver_CalculateStereoVolume
	.thumb_func
SoundDriver_CalculateStereoVolume:
	ldrb	r1, [r4, #18]
	movs	r0, #20
	ldrsb	r2, [r4, r0]
	muls	r2, r1
	lsls	r1, r1, #7
	ldrb	r0, [r5, #16]
	adds	r3, r1, r2
	muls	r0, r3
	asrs	r0, r0, #14
	cmp	r0, #255
	bgt.n	.L_081c0772
.L_081c0790:
	strb	r0, [r4, #2]
	ldrb	r0, [r5, #17]
	subs	r3, r1, r2
	muls	r0, r3
	asrs	r0, r0, #14
	cmp	r0, #255
	bgt.n	.L_081c0776
.L_081c079e:
	strb	r0, [r4, #3]
	bx	lr
	.global ply_note
	.thumb_func
ply_note:
	push	{r4, r5, r6, r7, lr}
	mov	r4, r8
	mov	r5, r9
	mov	r6, sl
	mov	r7, fp
	push	{r4, r5, r6, r7}
	sub	sp, #24
	str	r1, [sp, #0]
	adds	r5, r2, #0
	ldr	r1, [pc, #528]
	ldr	r1, [r1, #0]
	str	r1, [sp, #4]
	ldr	r1, [pc, #528]
	adds	r0, r0, r1
	ldrb	r0, [r0, #0]
	strb	r0, [r5, #4]
	ldr	r3, [r5, #64]
	ldrb	r0, [r3, #0]
	cmp	r0, #128
	bcs.n	.L_081c07e8
	strb	r0, [r5, #5]
	adds	r3, #1
	ldrb	r0, [r3, #0]
	cmp	r0, #128
	bcs.n	.L_081c07e6
	strb	r0, [r5, #6]
	adds	r3, #1
	ldrb	r0, [r3, #0]
	cmp	r0, #128
	bcs.n	.L_081c07e6
	ldrb	r1, [r5, #4]
	adds	r1, r1, r0
	strb	r1, [r5, #4]
	adds	r3, #1
.L_081c07e6:
	str	r3, [r5, #64]
.L_081c07e8:
	movs	r0, #0
	str	r0, [sp, #20]
	adds	r4, r5, #0
	adds	r4, #36
	ldrb	r2, [r4, #0]
	movs	r0, #192
	tst	r0, r2
	beq.n	.L_081c0838
	ldrb	r3, [r5, #5]
	movs	r0, #64
	tst	r0, r2
	beq.n	.L_081c0808
	ldr	r1, [r5, #44]
	adds	r1, r1, r3
	ldrb	r0, [r1, #0]
	b.n	.L_081c080a
.L_081c0808:
	adds	r0, r3, #0
.L_081c080a:
	movs	r1, #12
	muls	r1, r0
	ldr	r0, [r5, #40]
	adds	r1, r1, r0
	mov	r9, r1
	mov	r6, r9
	ldrb	r1, [r6, #0]
	movs	r0, #192
	tst	r0, r1
	beq.n	.L_081c0820
	b.n	.L_081c09b8
.L_081c0820:
	movs	r0, #128
	tst	r0, r2
	beq.n	.L_081c083c
	ldrb	r1, [r6, #3]
	movs	r0, #128
	tst	r0, r1
	beq.n	.L_081c0834
	subs	r1, #192
	lsls	r1, r1, #1
	str	r1, [sp, #20]
.L_081c0834:
	ldrb	r3, [r6, #1]
	b.n	.L_081c083c
.L_081c0838:
	mov	r9, r4
	ldrb	r3, [r5, #5]
.L_081c083c:
	str	r3, [sp, #8]
	ldr	r6, [sp, #0]
	ldrb	r1, [r6, #9]
	ldrb	r0, [r5, #29]
	adds	r0, r0, r1
	cmp	r0, #255
	bls.n	.L_081c084c
	movs	r0, #255
.L_081c084c:
	str	r0, [sp, #16]
	mov	r6, r9
	ldrb	r0, [r6, #0]
	movs	r6, #7
	ands	r6, r0
	str	r6, [sp, #12]
	beq.n	.L_081c088c
	ldr	r0, [sp, #4]
	ldr	r4, [r0, #28]
	cmp	r4, #0
	bne.n	.L_081c0864
	b.n	.L_081c09b8
.L_081c0864:
	subs	r6, #1
	lsls	r0, r6, #6
	adds	r4, r4, r0
	ldrb	r1, [r4, #0]
	movs	r0, #199
	tst	r0, r1
	beq.n	.L_081c0912
	movs	r0, #64
	tst	r0, r1
	bne.n	.L_081c0912
	ldrb	r1, [r4, #19]
	ldr	r0, [sp, #16]
	cmp	r1, r0
	bcc.n	.L_081c0912
	beq.n	.L_081c0884
	b.n	.L_081c09b8
.L_081c0884:
	ldr	r0, [r4, #44]
	cmp	r0, r5
	bcs.n	.L_081c0912
	b.n	.L_081c09b8
.L_081c088c:
	ldr	r4, [sp, #4]
	movs	r3, #10
	adds	r4, #80
	movs	r7, #199
	movs	r2, #0
	movs	r6, #0
	movs	r0, #3
.L_081c089a:
	ldrb	r1, [r4, #0]
	tst	r1, r7
	bne.n	.L_081c08a6
	subs	r0, #1
	beq.n	.L_081c0912
	.global Func_081c08a4
	.thumb_func
Func_081c08a4:
	adds	r6, r4, #0
.L_081c08a6:
	lsrs	r1, r1, #7
	adcs	r2, r2
	adds	r4, #64
	subs	r3, #1
	bgt.n	.L_081c089a
	cmp	r6, #0
	beq.n	.L_081c08d6
	cmp	r2, #0
	bne.n	.L_081c08bc
	adds	r4, r6, #0
	b.n	.L_081c0912
.L_081c08bc:
	mov	r8, r6
	ldr	r6, [sp, #16]
	adds	r7, r5, #0
	b.n	.L_081c08ca
.L_081c08c4:
	ldr	r0, [r4, #44]
	cmp	r0, r7
	beq.n	.L_081c0912
.L_081c08ca:
	subs	r4, #64
	lsrs	r2, r2, #1
	bcs.n	.L_081c08c4
	bne.n	.L_081c08ca
	mov	r4, r8
	b.n	.L_081c0912
.L_081c08d6:
	lsls	r6, r7, #4
	cmp	r2, #0
	bne.n	.L_081c0904
	mov	r8, r2
	ldr	r6, [sp, #16]
	adds	r7, r5, #0
	movs	r2, #1
	lsls	r2, r2, #10
	subs	r2, #1
	b.n	.L_081c0904
.L_081c08ea:
	ldrb	r0, [r4, #19]
	cmp	r0, r6
	bne.n	.L_081c08fc
	ldr	r0, [r4, #44]
	cmp	r0, r7
	bcc.n	.L_081c0904
	adds	r7, r0, #0
	mov	r8, r4
	b.n	.L_081c0904
.L_081c08fc:
	bhi.n	.L_081c0904
	ldr	r7, [r4, #44]
	adds	r6, r0, #0
	mov	r8, r4
.L_081c0904:
	subs	r4, #64
	lsrs	r2, r2, #1
	bcs.n	.L_081c08ea
	bne.n	.L_081c0904
	mov	r4, r8
	cmp	r4, #0
	beq.n	.L_081c09b8
.L_081c0912:
	adds	r0, r4, #0
	bl	SoundChannel_Unlink
	movs	r1, #0
	str	r1, [r4, #48]
	ldr	r3, [r5, #32]
	str	r3, [r4, #52]
	cmp	r3, #0
	beq.n	.L_081c0926
	str	r4, [r3, #48]
.L_081c0926:
	str	r4, [r5, #32]
	str	r5, [r4, #44]
	ldrb	r0, [r5, #27]
	strb	r0, [r5, #28]
	cmp	r0, r1
	beq.n	.L_081c0938
	adds	r1, r5, #0
	bl	clear_modM_note
.L_081c0938:
	ldr	r0, [sp, #0]
	adds	r1, r5, #0
	bl	TrkVolPitSet
	ldr	r0, [r5, #4]
	str	r0, [r4, #16]
	ldr	r0, [sp, #16]
	strb	r0, [r4, #19]
	ldr	r0, [sp, #8]
	strb	r0, [r4, #8]
	ldr	r0, [sp, #20]
	strb	r0, [r4, #20]
	mov	r6, r9
	ldrb	r0, [r6, #0]
	strb	r0, [r4, #1]
	ldr	r7, [r6, #4]
	str	r7, [r4, #36]
	ldr	r0, [r6, #8]
	str	r0, [r4, #4]
	ldrh	r0, [r5, #30]
	strh	r0, [r4, #12]
	bl	SoundDriver_CalculateStereoVolume
	ldrb	r1, [r4, #8]
	movs	r0, #8
	ldrsb	r0, [r5, r0]
	adds	r3, r1, r0
	bpl.n	.L_081c0972
	movs	r3, #0
.L_081c0972:
	ldr	r6, [sp, #12]
	cmp	r6, #0
	beq.n	.L_081c09a0
	mov	r6, r9
	ldrb	r0, [r6, #2]
	strb	r0, [r4, #30]
	ldrb	r1, [r6, #3]
	movs	r0, #128
	tst	r0, r1
	bne.n	.L_081c098c
	movs	r0, #112
	tst	r0, r1
	bne.n	.L_081c098e
.L_081c098c:
	movs	r1, #8
.L_081c098e:
	strb	r1, [r4, #31]
	ldrb	r2, [r5, #9]
	adds	r1, r3, #0
	ldr	r0, [sp, #12]
	ldr	r3, [sp, #4]
	ldr	r3, [r3, #48]
	mov	lr, r3
	.inst.n	0xf800
	b.n	.L_081c09aa
.L_081c09a0:
	ldrb	r2, [r5, #9]
	adds	r1, r3, #0
	adds	r0, r7, #0
	bl	SoundDriver_MidiKeyToFrequency
.L_081c09aa:
	str	r0, [r4, #32]
	movs	r0, #128
	strb	r0, [r4, #0]
	ldrb	r1, [r5, #0]
	movs	r0, #240
	ands	r0, r1
	strb	r0, [r5, #0]
.L_081c09b8:
	add	sp, #24
	pop	{r0, r1, r2, r3, r4, r5, r6, r7}
	mov	r8, r0
	mov	r9, r1
	mov	sl, r2
	mov	fp, r3
	pop	{pc}
AlchemyUncredited_081c09c6:
	.2byte	0x0000
AlchemyUncreditedEnd_081c09c6:
	.4byte	0x03007ff0
	.4byte	ClockTable

	.section .text.channel_unlink,"ax",%progbits
	.balign 2

.syntax unified
	.thumb
	.global MPlayMain
	.thumb_func
MPlayMain:
	ldr r2, .L_081c0c14
	ldr r3, [r0, #52]
	cmp r2, r3
	beq .L_081c09da
	bx lr
.L_081c09da:
	adds r3, #1
	str r3, [r0, #52]
	push {r0, lr}
	ldr r3, [r0, #56]
	cmp r3, #0
	beq .L_081c09ec
	ldr r0, [r0, #60]
	mov lr, r3
	.inst.n 0xf800
.L_081c09ec:
	pop {r0}
	push {r4, r5, r6, r7}
	mov r4, r8
	mov r5, r9
	mov r6, r10
	mov r7, r11
	push {r4, r5, r6, r7}
	adds r7, r0, #0
	ldr r0, [r7, #4]
	cmp r0, #0
	bge .L_081c0a04
	b .L_081c0bfe
.L_081c0a04:
	ldr r0, .L_081c0c10
	ldr r0, [r0]
	mov r8, r0
	adds r0, r7, #0
	bl FadeOutBody
	ldr r0, [r7, #4]
	cmp r0, #0
	bge .L_081c0a18
	b .L_081c0bfe
.L_081c0a18:
	ldrh r0, [r7, #34]
	ldrh r1, [r7, #32]
	adds r0, r0, r1
	b .L_081c0b56
.L_081c0a20:
	ldrb r6, [r7, #8]
	ldr r5, [r7, #44]
	movs r3, #1
	movs r4, #0
.L_081c0a28:
	ldrb r0, [r5]
	movs r1, #128
	tst r1, r0
	bne .L_081c0a32
	b .L_081c0b34
.L_081c0a32:
	mov r10, r3
	orrs r4, r3
	mov r11, r4
	ldr r4, [r5, #32]
	cmp r4, #0
	beq .L_081c0a64
.L_081c0a3e:
	ldrb r1, [r4]
	movs r0, #199
	tst r0, r1
	beq .L_081c0a58
	ldrb r0, [r4, #16]
	subs r0, #1
	bcc .L_081c0a5e
	strb r0, [r4, #16]
	bne .L_081c0a5e
	movs r0, #64
	orrs r1, r0
	strb r1, [r4]
	b .L_081c0a5e
.L_081c0a58:
	adds r0, r4, #0
	bl SoundChannel_Unlink
.L_081c0a5e:
	ldr r4, [r4, #52]
	cmp r4, #0
	bne .L_081c0a3e
.L_081c0a64:
	ldrb r3, [r5]
	lsrs r0, r3, #7
	.global Func_081c0a68
	.thumb_func
Func_081c0a68:
	bcc .L_081c0ad8
	adds r0, r5, #0
	bl SoundMainBTM
	movs r0, #128
	strb r0, [r5]
	movs r0, #2
	strb r0, [r5, #15]
	movs r0, #64
	strb r0, [r5, #19]
	movs r0, #22
	strb r0, [r5, #25]
	movs r0, #1
	adds r1, r5, #6
	strb r0, [r1, #30]
	b .L_081c0ad8
.L_081c0a88:
	ldrb r1, [r5, #7]
	b .L_081c0a9e
.L_081c0a8c:
	ldr r2, [r5, #64]
	ldrb r1, [r2]
	cmp r1, #128
	bcc .L_081c0a88
	adds r2, #1
	str r2, [r5, #64]
	cmp r1, #189
	bcc .L_081c0abc
	strb r1, [r5, #7]
.L_081c0a9e:
	cmp r1, #207
	bcc .L_081c0abc
	adds r0, r1, #0
	subs r0, #207
	adds r1, r7, #0
	adds r2, r5, #0
	bl ply_note
	b .L_081c0ad8
.L_081c0ab0:
	ldr r0, .L_081c0ab8
	ldrb r0, [r1, r0]
	strb r0, [r5, #1]
	b .L_081c0ada
.L_081c0ab8:
	.4byte ClockTable + 49
.L_081c0abc:
	subs r1, #177
	bcc .L_081c0ab0
	strb r1, [r7, #10]
	mov r3, r8
	ldr r3, [r3, #52]
	lsls r1, r1, #2
	ldr r3, [r3, r1]
	adds r0, r7, #0
	adds r1, r5, #0
	mov lr, r3
	.inst.n 0xf800
	ldrb r0, [r5]
	cmp r0, #0
	beq .L_081c0b30
.L_081c0ad8:
	ldrb r0, [r5, #1]
.L_081c0ada:
	subs r0, #1
	bcc .L_081c0a8c
	strb r0, [r5, #1]
	ldrb r0, [r5, #23]
	cmp r0, #0
	beq .L_081c0b30
	ldrb r0, [r5, #28]
	subs r0, #1
	bcc .L_081c0af0
	strb r0, [r5, #28]
	b .L_081c0b30
.L_081c0af0:
	ldrb r1, [r5, #25]
	cmp r1, #0
	beq .L_081c0b30
	ldrb r0, [r5, #26]
	adds r0, r0, r1
	strb r0, [r5, #26]
	adds r1, r0, #0
	subs r0, #64
	lsls r0, r0, #24
	bpl .L_081c0b0a
	lsls r2, r1, #24
	asrs r2, r2, #24
	b .L_081c0b0e
.L_081c0b0a:
	movs r0, #128
	subs r2, r0, r1
.L_081c0b0e:
	ldrb r0, [r5, #23]
	muls r0, r2
	asrs r2, r0, #6
	ldrb r0, [r5, #22]
	eors r0, r2
	lsls r0, r0, #24
	beq .L_081c0b30
	strb r2, [r5, #22]
	ldrb r0, [r5]
	ldrb r1, [r5, #24]
	cmp r1, #0
	bne .L_081c0b2a
	movs r1, #12
	b .L_081c0b2c
.L_081c0b2a:
	movs r1, #3
.L_081c0b2c:
	orrs r0, r1
	strb r0, [r5]
.L_081c0b30:
	mov r3, r10
	mov r4, r11
.L_081c0b34:
	subs r6, #1
	ble .L_081c0b3e
	adds r5, #80
	lsls r3, r3, #1
	b .L_081c0a28
.L_081c0b3e:
	ldr r0, [r7, #12]
	adds r0, #1
	str r0, [r7, #12]
	cmp r4, #0
	bne .L_081c0b50
	movs r0, #128
	lsls r0, r0, #24
	str r0, [r7, #4]
	b .L_081c0bfe
.L_081c0b50:
	str r4, [r7, #4]
	ldrh r0, [r7, #34]
	subs r0, #150
.L_081c0b56:
	strh r0, [r7, #34]
	cmp r0, #150
	bcc .L_081c0b5e
	b .L_081c0a20
.L_081c0b5e:
	ldrb r2, [r7, #8]
	ldr r5, [r7, #44]
.L_081c0b62:
	ldrb r0, [r5]
	lsls r1, r0, #24
	bpl .L_081c0bf8
	lsls r1, r0, #28
	beq .L_081c0bf8
	mov r9, r2
	adds r0, r7, #0
	adds r1, r5, #0
	bl TrkVolPitSet
	ldr r4, [r5, #32]
	cmp r4, #0
	beq .L_081c0bee
.L_081c0b7c:
	ldrb r1, [r4]
	movs r0, #199
	tst r0, r1
	bne .L_081c0b8c
	adds r0, r4, #0
	bl SoundChannel_Unlink
	b .L_081c0be8
.L_081c0b8c:
	ldrb r0, [r4, #1]
	movs r6, #7
	ands r6, r0
	ldrb r3, [r5]
	movs r0, #3
	tst r0, r3
	beq .L_081c0baa
	bl SoundDriver_CalculateStereoVolume
	cmp r6, #0
	beq .L_081c0baa
	ldrb r0, [r4, #29]
	movs r1, #1
	orrs r0, r1
	strb r0, [r4, #29]
.L_081c0baa:
	ldrb r3, [r5]
	movs r0, #12
	tst r0, r3
	beq .L_081c0be8
	ldrb r1, [r4, #8]
	movs r0, #8
	ldrsb r0, [r5, r0]
	adds r2, r1, r0
	bpl .L_081c0bbe
	movs r2, #0
.L_081c0bbe:
	cmp r6, #0
	beq .L_081c0bdc
	mov r0, r8
	ldr r3, [r0, #48]
	adds r1, r2, #0
	ldrb r2, [r5, #9]
	adds r0, r6, #0
	mov lr, r3
	.inst.n 0xf800
	str r0, [r4, #32]
	ldrb r0, [r4, #29]
	movs r1, #2
	orrs r0, r1
	strb r0, [r4, #29]
	b .L_081c0be8
.L_081c0bdc:
	adds r1, r2, #0
	ldrb r2, [r5, #9]
	ldr r0, [r4, #36]
	bl SoundDriver_MidiKeyToFrequency
	str r0, [r4, #32]
.L_081c0be8:
	ldr r4, [r4, #52]
	cmp r4, #0
	bne .L_081c0b7c
.L_081c0bee:
	ldrb r0, [r5]
	movs r1, #240
	ands r0, r1
	strb r0, [r5]
	mov r2, r9
.L_081c0bf8:
	adds r5, #80
	subs r2, #1
	bgt .L_081c0b62
.L_081c0bfe:
	ldr r0, .L_081c0c14
	str r0, [r7, #52]
	pop {r0, r1, r2, r3, r4, r5, r6, r7}
	mov r8, r0
	mov r9, r1
	mov r10, r2
	mov r11, r3
	.global Func_081c0c0c
	.thumb_func
Func_081c0c0c:
	pop {pc}
	bx r3
.L_081c0c10:
	.4byte SoundInfoPointer
.L_081c0c14:
	.4byte 0x68736d53
