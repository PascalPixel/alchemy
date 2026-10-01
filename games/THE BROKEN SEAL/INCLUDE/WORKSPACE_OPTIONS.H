#ifndef ALCHEMY_WORKSPACE_OPTIONS_H
#define ALCHEMY_WORKSPACE_OPTIONS_H

#include "TYPES.H"
#include "RENDER_INPUT.H"

/* A menu cursor record (object pointer and cursor state). The union keeps
   pointer stores ordered against the window reads that follow them;
   the two affected callers record their measured steering reason. */
union MenuCursor {
    struct RenderOutput *output;
    u8 data[0x10];
};

/* The workspace menu's block: the selected row and its row icons, and the
   options page with five settings, each a value in 0..count-1, and the
   menu objects that show them. */
struct WorkspaceWork {
    u8 unknown_000[0x574];
    u16 page;
    u8 unknown_576[8];
    u16 preset;
    u8 unknown_580[0x14];
    s8 option[5];
    s8 option_count[5];
    u8 unknown_59e[6];
    union MenuCursor cursor;
    union MenuCursor marker[2];
    u8 unknown_5d4[0x18];
    struct RenderOutput *frame[3][3];
    struct RenderOutput *icon[6];
};

extern struct WorkspaceWork *gSelectionWork;

struct RenderInput *Menu_OpenWorkspaceOptions(void);

#endif
