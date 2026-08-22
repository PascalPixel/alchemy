#ifndef GUARD_GS1_EDITION_H
#define GUARD_GS1_EDITION_H

/* The Japanese render work carries 0x90 more bytes before these flags. */
#if defined(GS1_EDITION_JA)
#define RENDER_DIRTY_OFS 0xF33
#define RENDER_MODE_OFS  0xF34
#define RENDER_BUSY_OFS  0x1189
#define RENDER_VALUE_TBL_OFS 0x114C
#define RENDER_NAME_TBL_OFS  0x116C
#else
#define RENDER_DIRTY_OFS 0xEA3
#define RENDER_MODE_OFS  0xEA4
#define RENDER_BUSY_OFS  0x12F9
#define RENDER_VALUE_TBL_OFS 0x12BC
#define RENDER_NAME_TBL_OFS  0x12DC
#endif

#if defined(GS1_EDITION_JA)
#define RENDER_RESOURCE_BASE 0x9D4
#elif defined(GS1_EDITION_DE) || defined(GS1_EDITION_ES) || \
      defined(GS1_EDITION_FR) || defined(GS1_EDITION_IT)
#define RENDER_RESOURCE_BASE 0xBDF
#else
#define RENDER_RESOURCE_BASE 0x99B
#endif

#endif
