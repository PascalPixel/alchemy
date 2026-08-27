pub type ExecutableDigests = (&'static str, &'static [&'static str]);
pub type TargetExecutables = (&'static str, &'static [ExecutableDigests]);
pub type HostTargets = (&'static str, &'static [TargetExecutables]);
pub type HostDigests = (&'static str, &'static [&'static str]);

// Rebuilt 2026-08-27 from the unchanged alchemy-gcc submodule commit
// (bcfb458, the agscc-rename commit) after a worktree-isolated subagent's
// `build`/`stage agscc` populated a fresh build-agscc/ tree with no source
// change (build-agscc/ and dist/ share the identical Aug 27 11:18 timestamp
// across all four binaries -- one coherent rebuild event, matching the same
// non-source-metadata pattern already recorded below for old_agbcc on
// 2026-08-26). Appended, not replaced; proved with a green
// `make full-rom-check` before pinning.
const GS1: &[ExecutableDigests] = &[
    (
        "xgcc",
        &[
            "1c0b96c0193a3f547bf10c70078d87df10a62d34207da545653165b65c80d4c2",
            "bf6161b89ca5832dff2784d92d2a39c857123e26fa4654ecc8ee46d3937b0b3a",
        ],
    ),
    (
        "cpp",
        &[
            "c69d8c0423517be6a4962bd88e343c193a0842a164dd600550ceb36f8155d736",
            "05509d9a475f1cc8d7937ad245eaf00f4e00b4f8b659016a4c5e86bad71d5ef7",
        ],
    ),
    (
        "tradcpp",
        &[
            "f1e4340c3550029675b49e22976dd7d10588e365816ae0f2decd19729371ee00",
            "2bed3f31593d16ab3c762be9725090fb7fb4d090ec9581b022070ee66541b759",
        ],
    ),
    (
        "cc1",
        &[
            "f7d1aeb098815f9b98c5df25bb47bab89aa14b1ab184b63c1472986f515fa3dd",
            "4525e2477f4d903580155e8d79cbb965b5af89e0d9ac903287713736238d37eb",
        ],
    ),
];
const EMPTY_GS1: &[ExecutableDigests] =
    &[("xgcc", &[]), ("cpp", &[]), ("tradcpp", &[]), ("cc1", &[])];
const EMPTY: &[TargetExecutables] = &[("gs1", EMPTY_GS1), ("gs2", EMPTY_GS1)];

// Admitted 2026-08-27: built from the unchanged alchemy-gcc submodule commit
// (640614ea242376836cee60ab4b2aa5facefd707d) on a fresh linux-x64 container,
// then proved with a green `make full-rom-check` (identical=True,
// byte_identical=yes) on that same build before pinning.
const LINUX_X64_GS1: &[ExecutableDigests] = &[
    (
        "xgcc",
        &["cc35e03215be6ec15aea1463c79e9598b754430c0927c2062ea337c46b725abc"],
    ),
    (
        "cpp",
        &["c01747023ae601e84c7739317d96bfae5ceab88de7271a83f7073fddeb9893aa"],
    ),
    (
        "tradcpp",
        &["cbf6602147a0b6cec2884d47ee56d44d1278a1fee31d2c413968c488c278787f"],
    ),
    (
        "cc1",
        &["d0c3f7857e568272f6d2a0d25f2138daeac2ee0941c4828db3abd10d9204b096"],
    ),
];

pub static EXPECTED: &[HostTargets] = &[
    ("darwin-arm64", &[("gs1", GS1), ("gs2", GS1)]),
    ("darwin-x64", EMPTY),
    (
        "linux-x64",
        &[("gs1", LINUX_X64_GS1), ("gs2", LINUX_X64_GS1)],
    ),
    ("linux-arm64", EMPTY),
];
pub static AGBCC_EXPECTED: &[HostDigests] = &[
    (
        "darwin-arm64",
        &[
            "dfbbb5a8a0a9e14e7ed84a0a723eb6e68974836159a2c3e925b904058382e15a",
            // Rebuilt 2026-08-26 from the unchanged alchemy-gcc submodule commit
            // (640614ea242376836cee60ab4b2aa5facefd707d) after a local disk cleanup
            // deleted the previously-staged dist/. agscc's (then gs1cc's) xgcc/cpp/tradcpp/cc1
            // reproduced the digests above byte-for-byte from the same rebuild;
            // old_agbcc alone differs, evidently from non-source build metadata
            // (its own smoke-compile passes), not a behavior change.
            "633678adb3c1ceae5b69b4a604724593bf198c216289c85f55109e473737b088",
            // Rebuilt 2026-08-27 from the unchanged submodule commit bcfb458
            // (the agscc-rename commit) after a worktree-isolated subagent's
            // build populated a fresh build tree with dist/agbcc/ missing
            // entirely from the main checkout (no source change). Confirmed
            // reproducible: two consecutive `stage agbcc` runs from the same
            // built tree produced byte-identical output. Proved with a green
            // `make full-rom-check` before pinning.
            "0c4cd3bbbb475245ec9ab5c9bada68f99ff4f2f6dec751c5c287731e9dbeaea8",
        ],
    ),
    ("darwin-x64", &[]),
    (
        "linux-x64",
        &["9200c74552a980be35fd58c8afdbd07bb76c9b785b57bad78d8303e00d738af3"],
    ),
    ("linux-arm64", &[]),
];
