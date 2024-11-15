import sys
import git
import re


class git_repo_manager:
  def __init__(self, repo_dir):
    self.repo_ = git.Repo(repo_dir)

  def repo_safe_to_commit(self):
    ### confirm no other un-committed edits will be swept up into release
    safe = False
    if  self.repo_.is_dirty(untracked_files=True):
      print("ERROR: repo cannot be dirty for a build.")
      print("  see 'git status' for details")
    else:
      safe = True

    ### TODO: confirm submodules are checked out and to the appropriate tag
    return safe

  def update_tags_in_source(self, tag_name):
    ### update tag instances in source code
    success = False

    version_maj = 0
    version_min = 0
    version_bugfix= 0
    parts = tag_name.split('.')

    # TODO: fail if space detected

    if ' 'not in tag_name and len(parts) == 3:
      version_maj = int(re.sub('[^0-9]', '', parts[0]))
      version_min = int(parts[1])
      version_bugfix = int(parts[2])
    else:
      print("invalid tag ; must follow pattern 'dev1.2.3'")

    return success

  def commit(self, commit_message):
    pass

  def tag(self, tag_name):
    pass

  def push(self):
    # may not want to do this for dev
    pass

class docker_manager:
  def create_tag():
    pass
  def push_tag():
    pass

def main():
  tag_name = None
  if len(sys.argv) >= 2:
    tag_name = sys.argv[1]

    git_ = git_repo_manager(f"{sys.path[0]}/..")
    if git_.repo_safe_to_commit():
      if git_.update_tags_in_source(tag_name):
        git_.commit(f"tag instances updated to {tag_name}")
        git_.tag(tag_name)
        git_.push()
    docker_manager.create_tag()
  else:
    print("tag-name required")

if __name__=="__main__":
  main()