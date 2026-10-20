# Alchemy CI and publication

`build.yml` tests the C tools, builds the pinned compiler sources, compares all
six Broken Seal editions without an input ROM, and compiles them with current
GCC. Pull requests stop there. The Lost Age still needs private cartridge
dumps, so twelve-edition verification happens locally.

For a landing, stage every source change and run `make land`. The receipt
records the staged source tree, measurement time and hashes of the TSV and
decomp.dev report; measurement refuses unstaged or untracked source. The
pre-push hook uploads that receipt and both reports, named for the pushed
commit, to the `decomp-reports` release. A changed measurement can never
overwrite an existing commit's report. No ROM, executable or map is uploaded.

After the main build succeeds, CI verifies the matching receipt and accepts
it as `<commit>.accepted.tsv`. History is reconstructed from the immutable
`progress-history-v1.tsv` baseline and accepted ancestors of that commit.
The original baseline retains its recorded dates; new measurement dates use
UTC. A rerun keeps the original measurement time. The release also holds an
immutable `<commit>.site.tar.gz` containing only the two PNG figures,
`history.tsv` and `source.tsv`. Releases preserve these beyond Actions'
artifact retention. `combined_report`, containing `report.pb`, remains a CI
artifact for decomp.dev.

`docs.yml` runs after successful CI and dispatches `alchemy-docs` with the
exact source commit, CI run ID and generator revision from `docs.ref`.
`alchemy-docs` verifies them, builds Doxygen and the fonts, and owns the Pages
deployment at <https://pascalpixel.github.io/alchemy-docs/>. Changes to that
repository run build checks; publication waits for a successful Alchemy CI.
There is no schedule and no generated Git branch. Both repositories reject
superseded source revisions before publication; Pages keeps the last
successful deployment on failure.

## Setup and recovery

Alchemy's `ALCHEMY_DOCS_TOKEN` secret is a fine-grained GitHub token restricted
to `PascalPixel/alchemy-docs`, with **Actions: read and write** permission.
It dispatches a workflow; Pages uses alchemy-docs' own short-lived token.
Renew the secret before its chosen expiration. Configure Pages in
alchemy-docs to build with GitHub Actions.

To publish a generator change, verify and push it in alchemy-docs, update
`docs.ref` here to its full commit SHA, then make a normal verified landing.
To retry publication, rerun the failed job. The `Publish docs` workflow also
accepts the ID of a successful main CI run; it refuses a superseded revision.
If the build fails before reports are archived, fix the build first.

The old `progress` branch remains as a frozen, recoverable archive. Its
baseline is also preserved in the release, checked against a fixed digest
by alchemy-devtools. Do not remove the release or its report assets: they
are the durable publication history.
