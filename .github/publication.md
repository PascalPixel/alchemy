# Alchemy CI and publication

`build.yml` proves in public what anyone can check: from a clean clone with no
ROM, it builds the pinned compilers and all twelve editions, the six of The
Broken Seal and the six of The Lost Age, and checks each against `rom.sha1`.
ccache hands back a compile only when its preprocessed source, compiler and
options are identical, so a push that changes comments recompiles almost
nothing; every ROM is still linked and checked. Pull requests stop there. The
format check, `make test` and `make modern` run locally before a change is
sent.

After a build of `main` succeeds, the `site` job refuses a superseded revision
and starts the site build through the Vercel deploy hook in the
`VERCEL_DEPLOY_HOOK` secret. Vercel builds `alchemy-docs` at its main branch:
it clones Alchemy's main, generates the pages from the source and serves them
at <https://alchemy.pascalpixel.com/>. A push to `alchemy-docs` builds the same
way. There is no schedule and no generated Git branch; Vercel keeps the last
successful deployment on failure.

Both games are at 100%, so nothing is measured or published from CI any more.
The measurements of the matching years stay as history: the `decomp-reports`
release and its numbered successors hold every landing's receipt, report and
accepted figures, and the old `progress` branch is a frozen archive. Do not
remove them. The final chart is in alchemy-devtools' README.

## Setup and recovery

The `VERCEL_DEPLOY_HOOK` secret is the URL of a deploy hook on the `alchemy`
Vercel project for branch `main`; anyone holding it can start a build, nothing
more. To replace it, create a new hook with `vercel deploy-hooks create`,
store its URL in the secret and remove the old hook. To retry a site build,
rerun the `site` job; it refuses a superseded revision.
