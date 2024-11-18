import sys

class deployment_manager:
  def get_registry_username():
    username_ = None
    return username_

  def get_registry_password():
    password_ = None
    return password_

  def get_deployment_dict_from_file(filepath):
    pass

  def add_registry_credentials(deployment,username,password):
    pass

  def save_tmp_deployment(deployment, path):
    pass

  def deploy(id,content,product_tags):
    pass

def print_help():
  pass

def main():
  deployment_path_ = None
  tag_ = None


  TAG_TAG = "--tag="
  for arg in sys.argv[1:]:
    if TAG_TAG in arg:
      tag_=arg.replace(TAG_TAG,"")
    else:
      if deployment_path_ == None:
        deployment_path_ = arg
      else:
        print(f"extra argument unknown ({arg})")

  if deployment_path_ == None:
    print("deployment required (E.G.: 'manifests/blender_dev_67.json')")
  if tag_ == None:
    print("tag must be specified (E.G. : '--tag=blender_dev_danny')")

  dm = deployment_manager
  username_ = dm.get_registry_username()
  password_ = dm.get_registry_password()
  j = dm.get_deployment_dict_from_file(deployment_path_)
  dm.add_registry_credentials(j,username_,password_)


if __name__=="__main__":
  main()