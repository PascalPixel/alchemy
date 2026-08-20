#ifndef ALCHEMY_TRAMPOLINE_ALIGN_H
#define ALCHEMY_TRAMPOLINE_ALIGN_H

/**
 * Assembly alignment macros for ARM7TDMI Thumb-to-ARM mode interworking stubs.
 * Guarantees 4-byte instruction alignment for byte-exact ROM matching.
 */

#define ALCHEMY_ALIGN_ARM_STUB .align 2
#define ALCHEMY_ALIGN_THUMB_STUB .align 1

#endif /* ALCHEMY_TRAMPOLINE_ALIGN_H */
