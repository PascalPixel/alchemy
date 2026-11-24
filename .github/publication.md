# Alchemy CI and publication

`build.yml` proves in public what anyone can check: from a clean clone with no
ROM, it builds the pinned compilers and all twelve editions, the six of The
Broken Seal and the six of The Lost Age, and checks each against `rom.sha1`.
ccache hands back a compile only when its preprocessed source, compiler and
options are identical, so a push that changes comments recompiles almost
nothing; every ROM is still linked and checked. Pull requests stop there. The
format check, `make test` and `make modern` run locally before a change is
sent.

`site.yml` rebuilds the docs site once a day at most, at midnight UTC, and only
when `main` changed that day: it starts the site build through the Vercel
deploy hook in the `VERCEL_DEPLOY_HOOK` secret. Vercel builds `alchemy-docs` at
its main branch: it clones Alchemy's main, generates the pages from the source
and serves them at <https://alchemy.pascalpixel.com/>. A push to `alchemy-docs`
builds the same way. A push here does not: a site build takes several minutes
of a four-core machine, and one for every push made build minutes the largest
line of the month's bill. There is no generated Git
branch; Vercel keeps the last successful deployment on failure.

Both games are complete, so CI measures and publishes nothing. The record of
how the source was written stays as history: the `decomp-reports` release and
its numbered successors hold every landing's receipt, report and accepted
figures, and the old `progress` branch is a frozen archive. Do not remove
them.

## Setup and recovery

The `VERCEL_DEPLOY_HOOK` secret is the URL of a deploy hook on the `alchemy`
Vercel project for branch `main`; anyone holding it can start a build, nothing
more. To replace it, create a new hook with `vercel deploy-hooks create`,
store its URL in the secret and remove the old hook. To rebuild the site at
once, run the Site workflow by hand: `gh workflow run site.yml`.
