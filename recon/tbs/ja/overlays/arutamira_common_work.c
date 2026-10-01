/* Draft: Arutamira's actor-wheel work buffer.
 * 2026-10-01: The common definition receives eight-byte common alignment,
 * even with an aligned(4) annotation. Its scene pointer is four bytes past
 * the natural loaded-image end in all six editions. The adopted definition
 * allocates its own BSS storage, word-aligned for the three-word DMA clear.
 * This data-layout attempt carries no credit.
 */
#include "TYPES.H"

u16 gArutamiraActorWheelWork[6] __attribute__((aligned(4)));
