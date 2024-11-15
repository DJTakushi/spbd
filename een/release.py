import sys
import git


class git_repo_manager:
  def __init__(self, repo_dir):
    self.repo_ = git.Repo(repo_dir)

  def repo_safe_to_commit(self):
    ### confirm no other un-committed edits will be swept up into release
    ### confirm submodules are checked out and to the appropriate tag
    safe = False
    t = self.repo_.head.commit.tree

    d = self.repo_.git.diff(t)
    if d:
      print(f"diff found : {d}")
    else:
      if  self.repo_.is_dirty(untracked_files=True):
        print("repo is dirty")
      else:
        safe = True
    return safe

  def update_tags_in_source(self, tag_name):
    ### update tag instances in source code
    pass

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
      git_.update_tags_in_source(tag_name)
      git_.commit(f"tag instances updated to {tag_name}")
      git_.tag(tag_name)
      git_.push()
    docker_manager.create_tag()
  else:
    print("tag-name required")

if __name__=="__main__":
  main()