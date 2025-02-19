import subprocess


def post_action(source, target, env):
    with open("lib/version/version_build.h") as f:
        line1 = f.readline().strip()
    fw_ver = line1.split(" = ")[1].strip('";')
    subprocess.check_output(["scripts/release.sh", fw_ver])


# Try to import `env` - PlatformIO's black magic
try:
    Import("env")
    env.AddPostAction("buildprog", post_action)
except NameError:
    pass


if __name__ == "__main__":
    print("Running in standalone mode ...")
    # placeholder for future case if such mode is needed
