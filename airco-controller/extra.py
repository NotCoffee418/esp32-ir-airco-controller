
# Hook into the normal upload process
Import("env")

def upload_fs(source, target, env):
    env.Execute("pio run -t uploadfs")

# Post action because pre-action seems to break upload.
# When we do need the changed data right away, upload manually first.
env.AddPostAction("upload", upload_fs)
