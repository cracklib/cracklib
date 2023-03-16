Release Process
===============

Follow semantic versioning standard if at all possible: https://semver.org/ (MAJOR.MINOR.PATCH-PRE)

During development, pick next planned version - increment patch level, and add "-dev"

Before publication, add in NEWS for example "2.9.x" or "master" or "dev" and only when it is the commit
for announcement, the moment to change the new version.

Release process:
	1) Start from a clean repo (git reset --hard; git clean -fdx)
	2) Update version in configure.ac and ../words/Makefile. Update NEWS file to reflect that version.
	3) Run autogen.sh and configure, then 'make dist'
	4) Validate that tarball successfully can be built from source
	5) Check for changes, there should be various files updated in po directory - all version number related
	6) Commit that with new version
	7) Create tag of form 'vX.Y.Z' pointed to that commit, push tags
	8) Make in cracklib words directory
	9) Upload the relevant binaries to a release on github attached to the tag that was already created above

After the release, repeat steps 2 through 5 with incremented version: MAJOR.MINOR.(PATCH+1)-dev

