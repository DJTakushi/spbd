import sys
import git
import re
import os
import subprocess
class git_repo_manager:
  def __init__(self, repo_dir,tag_name):
    self.tag_name_ = tag_name
    self.repo_dir_ = repo_dir
    self.repo_ = git.Repo(repo_dir)

    # safety checks
    self.safe_ = self.tag_safe() and self.repo_safe_to_commit()
    ### TODO: confirm submodules are checked out and to the appropriate tag

  def tag_safe(self):
    safe_ = False
    existing_tags = self.repo_.git.tag("-l",self.tag_name_)
    if len(existing_tags) == 0:
      safe_ = True
    else:
      print("ERROR : tag already exists :")
      print(existing_tags)
      safe_ = False
    return safe_

  def repo_safe_to_commit(self):
    ### confirm no other un-committed edits will be swept up into release
    safe_ = False
    if not self.repo_.is_dirty(untracked_files=True):
      safe_ = True
    else:
      print("ERROR: repo cannot be dirty for a build "\
            "(see 'git status' for details)")
    return safe_

  def is_safe(self):
    return  self.safe_

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

  def push(self, tag_name):
    # may not want to do this for dev
    self.repo_.git.push("origin","tag",tag_name)

class docker_manager:
  def __init__(self, tag):
    tag = tag.split('/')[-1] # get last part right of marker
    self.full_tag_ = f"sparkplugbdemo.azurecr.io/een:{tag}"
  def build_tagged_image(self):
    os.chdir(sys.path[0])
    cmd = ["docker","build","-t",self.full_tag_,"."]
    result = subprocess.run(cmd)
    success = False
    if result.returncode==0:
      success = True
      print(f"built {self.full_tag_}...")
    else:
      print(f"failed to build tag {self.full_tag_}")
    return success

  def push_tag(self):
    cmd = ["docker","push",self.full_tag_]
    result = subprocess.run(cmd)
    success = False
    if result.returncode==0:
      success = True
      print(f"pushed {self.full_tag_}...")
    else:
      print(f"failed to push tag {self.full_tag_}")
    return success


def main():
  tag_name = None
  no_git_ = False
  git_success_ = False
  if len(sys.argv) >= 2:
    for arg in sys.argv[1:]:
      if "--nogit" in arg:
        no_git_ = True

    tag_name = sys.argv[1]

    git_ = git_repo_manager(f"{sys.path[0]}/..",tag_name)
    if not no_git_ and git_.is_safe():
      if git_.update_tags_in_source(tag_name):
        git_.commit(f"tag instances updated to {tag_name}")
        git_.tag(tag_name)
        git_.push(tag_name)
        git_success_ = True

    if no_git_ or git_success_:
      docker_ = docker_manager(tag_name)
      if docker_.build_tagged_image():
        docker_.push_tag()
  else:
    print("tag-name required (E.G.: 'dev0.1.19')")

if __name__=="__main__":
  main()