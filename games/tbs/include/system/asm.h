#ifndef ALCHEMY_ASM_H
#define ALCHEMY_ASM_H

// Every piece of assembly the C holds, one macro to each instruction sequence and one for a local
// kept in a register, each a fake match. The helpers that use them are ordinary inlines in their
// own headers. No other file has an asm statement with anything in it.
//
// They are a whole set, not a scattering: the system library is built to the shape of SGL, Sega's
// engine for the Saturn, on the hardware GBATEK describes, and these are the few operations of it
// small enough to write in place. The system library's README, src/system/README.md, says what that
// rests on.

// The BIOS
//
// The two BIOS calls the game makes in place, by GBATEK's numbers: SWI 02h, Halt, and SWI 03h,
// Stop. The BIOS's arithmetic and decompression it never calls: the system library brings its own.

// Halts the CPU until an interrupt (the BIOS Halt call).
#define ASM_BIOS_HALT()                                                                            \
	/* FAKEMATCH(asm): Halt, which no C compiles to. */                                            \
	__asm__ volatile("swi 2")

// Stops the CPU and the screen until an enabled interrupt wakes them (the BIOS Stop call).
#define ASM_BIOS_STOP()                                                                            \
	/* FAKEMATCH(asm): Stop, which no C compiles to. */                                            \
	__asm__ volatile("swi 3")

// DMA
//
// A channel's three registers, source, destination and control (GBATEK's DMAxSAD, DMAxDAD and
// DMAxCNT), written with one instruction. syDMAXCopy takes its arguments as SGL's slDMAXCopy does,
// in the registers a call would pass them.

// Stores source, destination and control into a DMA channel's three registers with one stmia, and
// steps the channel pointer back.
#define ASM_DMA_STORE(source, control, channel, destination)                                       \
	/* FAKEMATCH(asm): stmia of three registers with no                                            \
	 * paired ldmia, which no agscc pattern emits. */                                              \
	__asm__ volatile("stmia %2!, {%0, %3, %1}\n\tsub %2, #12"                                      \
	                 : "+r"(source), "+r"(control)                                                 \
	                 : "r"(channel), "r"(destination)                                              \
	                 : "memory", "cc")

// Fixed point
//
// SGL's slMulFX. The multiply itself is a function in IWRAM; these are the ways the C reaches it
// without paying for a call, or writes it out in ARM code.

// Calls an IWRAM function that returns through ip instead of lr, so the caller keeps lr and r3.
#define ASM_CALL_RETURN_IP(result, factor, routine)                                                \
	/* FAKEMATCH(asm): a call that returns through ip,                                             \
	 * which no procedure-call standard produces. */                                               \
	__asm__ volatile(".align 2\n\t"                                                                \
	                 "mov ip, pc\n\t"                                                              \
	                 "bx %2\n\t"                                                                   \
	                 : "+r"(result)                                                                \
	                 : "r"(factor), "r"(routine)                                                   \
	                 : "r1", "r2", "ip", "cc")

// ASM_CALL_RETURN_IP for the modern build, which pins no register.
#define ASM_CALL_RETURN_IP_MODERN(result, left, right, routine)                                    \
	/* FAKEMATCH(asm): the same call written without pinned registers. */                          \
	__asm__ volatile("mov r0, %1\n\t"                                                              \
	                 "mov r1, %2\n\t"                                                              \
	                 ".align 2\n\t"                                                                \
	                 "mov ip, pc\n\t"                                                              \
	                 "bx %3\n\t"                                                                   \
	                 "mov %0, r0"                                                                  \
	                 : "=r"(result)                                                                \
	                 : "r"(left), "r"(right), "r"(routine)                                         \
	                 : "r0", "r1", "r2", "ip", "cc")

// The 64-bit product of two words, its halves in any two registers.
#define ASM_SMULL(high, low, left, right)                                                          \
	/* FAKEMATCH(asm): smull with RdLo and RdHi in any two                                         \
	 * registers, even over its inputs, where mulsidi3 pairs them (RdHi = RdLo                     \
	 * + 1) apart from the inputs and smulsi3_highpart discards RdLo; its orr                      \
	 * takes a fresh output reload register beside two dying inputs and                            \
	 * nothing is scheduled across it, as only asm statements do. */                               \
	__asm__ volatile("smull %1, %0, %3, %2" : "=r"(high), "=r"(low) : "r"(left), "r"(right))

// The middle 32 bits of a 64-bit product, from its two halves.
#define ASM_ORR_SHIFTED(result, high, low)                                                         \
	/* FAKEMATCH(asm): syMulFXArm's next instruction. */                                           \
	__asm__ volatile("orr %0, %1, %2, lsr #16" : "=r"(result) : "r"(high), "r"(low))

// The matrix
//
// SGL's slUnitMatrix: twelve words, 1.0 on the diagonal. It destroys r0 to r4, the registers every
// function of the system library treats as scratch.

// Stores the unit matrix at a pointer: three runs of 1.0 and three zeros.
#define ASM_MATRIX_UNIT(matrix)                                                                    \
	/* FAKEMATCH(asm): three stmia no C store or copy                                              \
	 * compiles to. */                                                                             \
	__asm__ volatile("mov r0, %0\n\tmov r1, #128\n\tmov r2, #0\n\tmov r3, #0\n\t"                  \
	                 "mov r4, #0\n\tlsl r1, r1, #9\n\t"                                            \
	                 "stmia r0!, {r1, r2, r3, r4}\n\t"                                             \
	                 "stmia r0!, {r1, r2, r3, r4}\n\t"                                             \
	                 "stmia r0!, {r1, r2, r3, r4}"                                                 \
	                 :                                                                             \
	                 : "r"(matrix)                                                                 \
	                 : "r0", "r1", "r2", "r3", "r4", "memory")

// ASM_MATRIX_UNIT as the system library itself writes it for the current matrix: the same stores,
// with r3 left off the list of what it destroys.
#define ASM_MATRIX_UNIT_CURRENT(matrix)                                                            \
	/* FAKEMATCH(asm): syUnitMatrix's three stmia,                                                 \
	 * applied to the current matrix. */                                                           \
	__asm__ volatile("mov r0, %0\n\tmov r1, #128\n\tmov r2, #0\n\tmov r3, #0\n\t"                  \
	                 "mov r4, #0\n\tlsl r1, r1, #9\n\t"                                            \
	                 "stmia r0!, {r1, r2, r3, r4}\n\t"                                             \
	                 "stmia r0!, {r1, r2, r3, r4}\n\t"                                             \
	                 "stmia r0!, {r1, r2, r3, r4}"                                                 \
	                 :                                                                             \
	                 : "r"(matrix)                                                                 \
	                 : "r0", "r1", "r2", "r4", "memory")

// The stack and the restart
//
// What the tick wait and the soft reset do to the processor itself.

// Reads the hardware stack pointer into `variable`.
#define ASM_STACK_POINTER_GET(variable)                                                            \
	/* FAKEMATCH(asm): a read of sp. */                                                            \
	__asm__ volatile("mov %0, sp" : "=r"(variable))

// Moves the hardware stack pointer to `value`.
#define ASM_STACK_POINTER_SET(value)                                                               \
	/* FAKEMATCH(asm): a write to sp no C can express. */                                          \
	__asm__ volatile("mov sp, %0" : : "r"(value) : "memory")

// Registers and calls C cannot name

// Keeps a local in the register named.
#define ASM_REGISTER(declaration, name)                                                            \
	/* FAKEMATCH(asm): a local pinned to a register by an asm register name.                       \
	 */                                                                                            \
	register declaration __asm__(name)

// A statement that emits nothing

// The statement that ends a menu sprite's set-up: it emits nothing, and the compiler counts four
// bytes for it when it places the next literal pool.
#define ASM_NOTHING()                                                                              \
	/* FAKEMATCH(barrier): an empty asm; the literal pools after a menu                            \
	 * sprite's two stores sit where the ROM has them only with four bytes                         \
	 * counted that no instruction emits, which nothing else in this compiler                      \
	 * does; as no asm. */                                                                         \
	__asm__ volatile("")

// The modern build

// Declares `symbol` as the bytes at the linked name `name`, whatever type the name has elsewhere:
// how the modern build reaches a fixed address.
#define ASM_LINKED_NAME(symbol, name)                                                              \
	/* FAKEMATCH(asm): another name for a linked symbol, by an asm label. */                       \
	extern char symbol[] __asm__(name)

#endif
