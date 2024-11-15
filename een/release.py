import sys
import git
import re
import os
import subprocess
class git_repo_manager:
  def __init__(self, repo_dir):
    self.repo_dir_ = repo_dir
    self.repo_ = git.Repo(repo_dir)

  def repo_safe_to_commit(self, force):
    ### confirm no other un-committed edits will be swept up into release
    safe = False
    e = "WARNING" if force else "ERROR"
    if  self.repo_.is_dirty(untracked_files=True):
      print(f"{e}: repo cannot be dirty for a build "\
            "(see 'git status' for details)")
    else:
      safe = True

    ### TODO: confirm submodules are checked out and to the appropriate tag

    if force:
      return force
    else:
      return safe

  def update_tags_in_source(self, tag_name):
    ### update tag instances in source code
    success = False

    version_maj = 0
    version_min = 0
    version_patch= 0
    parts = tag_name.split('.')

    if ' 'not in tag_name and len(parts) == 3:
      version_maj = int(re.sub('[^0-9]', '', parts[0]))
      version_min = int(parts[1])
      version_patch = int(parts[2])

      # CMakeLists.txt update
      cmake_lines = []
      cmake_lists_fp = self.repo_dir_+"/een/CMakeLists.txt"
      with open(cmake_lists_fp, 'r') as file:
        cmake_lines = file.readlines()
      for i in range(len(cmake_lines)):
        if "set(EEN_VERSION_MAJOR" in cmake_lines[i]:
          cmake_lines[i] = f"set(EEN_VERSION_MAJOR       \"{version_maj}\")\n"
          print("updated version_maj...")
        if "set(EEN_VERSION_MINOR" in cmake_lines[i]:
          cmake_lines[i] = f"set(EEN_VERSION_MINOR       \"{version_min}\")\n"
          print("updated version_min...")
        if "set(EEN_VERSION_PATCH" in cmake_lines[i]:
          cmake_lines[i] = f"set(EEN_VERSION_PATCH       \"{version_patch}\")\n"
          print("updated version_patch...")
      with open(cmake_lists_fp, 'w') as file:
        file.writelines(cmake_lines)
        success = True
    else:
      print("invalid tag ; must follow pattern 'dev1.2.3'")

    if success:
      print(f"updated {cmake_lists_fp}...")
    return success

  def commit(self, commit_message):
    self.repo_.git.add("--all")
    self.repo_.git.commit('-m',commit_message)
    print(f"committed with msg : '{commit_message}'...")

  def tag(self, tag_name):
    self.repo_.git.tag(tag_name)
    print(f"tagged commit to '{tag_name}...")

  def push(self):
    # may not want to do this for dev
    pass


class docker_manager:
  def build_image(tag):
    os.chdir(sys.path[0])
    full_tag = "sparkplugbdemo.azurecr.io/een:{tag}"
    cmd = ["docker","build","-t",full_tag,"."]
    result = subprocess.run(cmd, stdout=subprocess.PIPE, stderr=subprocess.PIPE)
    success = False
    if result.returncode==0:
      success = True
      print(f"built {full_tag}...")
    else:
      print(f"failed to build tag {full_tag}")
    return success

  def create_tag():
    pass
  def push_tag():
    pass

def main():
  tag_name = None
  force_ = False
  if len(sys.argv) >= 2:
    for arg in sys.argv[1:]:
      if "--force" in arg:
        force_ = True

    tag_name = sys.argv[1]

    git_ = git_repo_manager(f"{sys.path[0]}/..")
    if git_.repo_safe_to_commit(force_) or force_:
      if git_.update_tags_in_source(tag_name):
        git_.commit(f"tag instances updated to {tag_name}")
        git_.tag(tag_name)
        git_.push()
    docker_manager.create_tag()
  else:
    print("tag-name required")

if __name__=="__main__":
  main()