import json
import os
import sys
env_var_username = "SPARKPLUGBDEMO_REGSISTRY_DEPLOYMENT_USERNAME"
env_var_password = "SPARKPLUGBDEMO_REGSISTRY_DEPLOYMENT_PASSWORD"

class deployment_manager:
  def get_registry_username():
    username_ = os.environ.get(env_var_username,None)
    if username_ == None:
      print(f"{env_var_username} environment variable not set")
    return username_

  def get_registry_password():
    password_ = os.environ.get(env_var_password,None)
    if password_ == None:
      print(f"{env_var_password} environment variable not set")
    return password_

  def get_deployment_dict_from_file(filepath):
    j = None
    with open(filepath,mode="r") as file:
      j = json.load(file)
    if j == None:
      print(f"failed to load json from file {filepath}")
    return j

  def add_registry_credentials(deployment,username,password):
    deployment["content"]["modulesContent"]["$edgeAgent"]["properties.desired"]\
        ["runtime"]["settings"]["registryCredentials"]\
        ["sparkplugbdemo_registry"]["username"] = username
    deployment["content"]["modulesContent"]["$edgeAgent"]["properties.desired"]\
        ["runtime"]["settings"]["registryCredentials"]\
        ["sparkplugbdemo_registry"]["password"] = password
    
  def save_tmp_deployment(deployment, path):
    with open(path,'w') as f:
      json.dump(deployment,f,indent=2)

  def deploy(id,content,product_tags):
    pass

  def remove_file(filepath):
    os.remove(filepath)

def print_help():
  pass

def main():
  deployment_path_ = None
  tag_ = None
  id_ = None

  TAG_TAG = "--tag="
  for arg in sys.argv[1:]:
    if TAG_TAG in arg:
      tag_=arg.replace(TAG_TAG,"")
    else:
      if ".json" in arg:
        deployment_path_ = arg
        id_ = os.path.basename(deployment_path_).replace(".json","")
      else:
        print(f"argument unknown ({arg})")

  if deployment_path_ == None:
    print("deployment required (E.G.: 'manifests/blender_dev_67.json')")
    return 1
  if tag_ == None:
    print("tag must be specified (E.G. : '--tag=blender_dev_danny')")
    return 1

  dm = deployment_manager
  username_ = dm.get_registry_username()
  password_ = dm.get_registry_password()
  if username_ == None or password_ == None:
    print("container regsitry credentials must be set")
    return 1

  if not os.path.exists(deployment_path_):
    print(f"could not find deployment {deployment_path_}")
  deployment_name_ = os.path.basename(deployment_path_)
  deployment_name_tmp = "tmp"+deployment_name_

  j = dm.get_deployment_dict_from_file(deployment_path_)

  dm.add_registry_credentials(j,username_,password_)
  dm.save_tmp_deployment(j,deployment_name_tmp)
  dm.deploy(id_,deployment_name_tmp,tag_)

  dm.remove_file(deployment_name_tmp)


if __name__=="__main__":
  main()