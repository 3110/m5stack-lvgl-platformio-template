Import("env", "projenv")

for e in [ env, projenv ]:
    # If compiler uses `-m32`, propagate it to linker.
    # Add via script, because `-Wl,-m32` does not work.
    if "-m32" in e['CCFLAGS']:
        e.Append(LINKFLAGS = ["-m32"])

exec_name = "${BUILD_DIR}/${PROGNAME}${PROGSUFFIX}"

env.AddTarget(
    name = "execute",
    dependencies = exec_name,
    actions = [exec_name],
    title = "Execute",
    description = "Build and execute",
    group="General"
)
