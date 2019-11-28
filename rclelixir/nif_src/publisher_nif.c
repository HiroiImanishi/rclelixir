// Copyright 2015 Open Source Robotics Foundation, Inc.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#ifdef __cplusplus
extern "C"
{
#endif
#include <erl_nif.h>
#include "rcl/publisher.h"

#include <stdio.h>
#include <string.h>

#include "rcl/allocator.h"
#include "rcl/error_handling.h"
#include "rcl/expand_topic_name.h"
#include "rcl/remap.h"
#include "rcutils/logging.h"
#include "rmw/error_handling.h"
#include "rmw/validate_full_topic_name.h"
//#include "tracetools/tracetools.h"

//#include "./common.h"
//#include "./publisher_impl.h"


ErlNifResourceType* rt_pub;
ErlNifResourceType* rt_pub_options;
ErlNifResourceType* rt_ret;
ErlNifResourceType* rt_node;
ErlNifResourceType* rt_rosidl_msg_type_support;
ErlNifResourceType* rt_rmw_pub_allocation;
ERL_NIF_TERM atom_ok;
ERL_NIF_TERM atom_true;
ERL_NIF_TERM atom_false;

//リソースタイプを作る．load()から呼び出される
static int open_resource(ErlNifEnv* env){
    const char* mod = "Elixir.NifPublisher";
    const char* name1 = "rcl_publisher_t";
    const char* name2 = "rcl_publisher_options_t";
    const char* name3 = "rcl_ret_t";
    const char* name4 = "rcl_node_t";
    const char* name5 = "rosidl_message_type_support_t";
    const char* name6 = "rmw_publisher_allocation_t";
    
    int flags = ERL_NIF_RT_CREATE | ERL_NIF_RT_TAKEOVER;

    //RES_TYPE = enif_open_resource_type(env,mod,name,free_res,flags,NULL);
    rt_pub         = enif_open_resource_type(env,mod,name1,NULL,flags,NULL);
    rt_pub_options = enif_open_resource_type(env,mod,name2,NULL,flags,NULL);
    rt_ret               = enif_open_resource_type(env,mod,name3,NULL,flags,NULL);
    rt_node              = enif_open_resource_type(env,mod,name4,NULL,flags,NULL);
    rt_rosidl_msg_type_support = enif_open_resource_type(env,mod,name5,NULL,flags,NULL);
    rt_rmw_pub_allocation = enif_open_resource_type(env,mod,name6,NULL,flags,NULL);
    
    if(rt_pub == NULL || rt_pub_options == NULL || rt_ret == NULL || rt_node == NULL || rt_rosidl_msg_type_support == NULL || rt_rmw_pub_allocation == NULL) return -1;
    return 0;
}

//@on_loadで呼び出す
static int load(ErlNifEnv* env, void** priv,ERL_NIF_TERM load_info){
    if(open_resource(env) == -1) return -1;

    atom_ok = enif_make_atom(env,"ok");
    atom_true = enif_make_atom(env,"true");
    atom_false = enif_make_atom(env,"false");
    return 0;
}

static int reload(ErlNifEnv* env,void** priv,ERL_NIF_TERM load_info){
    if(open_resource(env) == -1) return -1;
    return 0;
}

//wrap関数その1
ERL_NIF_TERM nif_rcl_get_zero_initialized_publisher(ErlNifEnv* env,int argc,const ERL_NIF_TERM argv[]){
    rcl_publisher_t* res;
    ERL_NIF_TERM ret;
    res = enif_alloc_resource(rt_pub,sizeof(rcl_publisher_t));
    if(res == NULL) return enif_make_badarg(env);
    ret = enif_make_resource(env,res);
    enif_release_resource(res);
    *res = rcl_get_zero_initialized_publisher();

    return ret;
}

//wrap関数その2
ERL_NIF_TERM nif_rcl_publisher_get_default_options(ErlNifEnv* env,int argc,const ERL_NIF_TERM argv[]){
  rcl_publisher_options_t* res;
  ERL_NIF_TERM ret;
  res = enif_alloc_resource(rt_pub_options,sizeof(rcl_publisher_options_t));
  if(res == NULL) return enif_make_badarg(env);
  ret = enif_make_resource(env,res);
  enif_release_resource(res);
  *res = rcl_publisher_get_default_options();

  return ret;
}

//wrap関数その3
ERL_NIF_TERM nif_rcl_publisher_get_topic_name(ErlNifEnv* env,int argc,const ERL_NIF_TERM argv[]){
  rcl_publisher_t* res;
  if(argc != 1){
      return enif_make_badarg(env);
  }

  if(!enif_get_resource(env,argv[0],rt_pub,(void**) &res)){
      return enif_make_badarg(env);
  }
  const char* result;
  result = rcl_publisher_get_topic_name(res);
  return enif_make_string(env,result,ERL_NIF_LATIN1);
}

//wrap関数4
ERL_NIF_TERM nif_rcl_publisher_fini(ErlNifEnv* env, int argc, const ERL_NIF_TERM argv[]){
  rcl_ret_t* res;
  ERL_NIF_TERM ret;

  rcl_publisher_t* res_arg_pub;
  rcl_node_t* res_arg_node;

  if(argc != 2)
  {
      return enif_make_badarg(env);
  }
  if(!enif_get_resource(env, argv[0], rt_pub, (void**) &res_arg_pub))
  {
    return enif_make_badarg(env);
  }
  if(!enif_get_resource(env, argv[1], rt_node, (void**) &res_arg_node))
  {
    return enif_make_badarg(env);
  }

  res = enif_alloc_resource(rt_ret,sizeof(rcl_ret_t));
  if(res == NULL) return enif_make_badarg(env);
  /*
  res_arg_pub = enif_alloc_resource(rt_pub,sizeof(rcl_publisher_t));
  if(res_arg_pub == NULL) return enif_make_badarg(env);
  res_arg_node = enif_alloc_resource(rt_node,sizeof(rcl_node_t));
  if(res_arg_node == NULL) return enif_make_badarg(env);
  */
  ret = enif_make_resource(env,res);
  enif_release_resource(res);
  
  *res = rcl_publisher_fini(res_arg_pub,res_arg_node);
  
  return ret;
} 

//rcl_publisher_init(
//   rcl_publisher_t * publisher,
//   const rcl_node_t * node,
//   const rosidl_message_type_support_t * type_support,
//   const char * topic_name,
//   const rcl_publisher_options_t * options);

ERL_NIF_TERM nif_rcl_publisher_init(ErlNifEnv* env, int argc, const ERL_NIF_TERM argv[]){
  if(argc != 5){
        return enif_make_badarg(env);
  }
  rcl_ret_t* res_return;
  ERL_NIF_TERM ret;
  rcl_publisher_t*  res_pub;
  rcl_node_t* res_node;
  rosidl_message_type_support_t* res_idl;
  rcl_publisher_options_t* res_options;
  printf("0\n");
  if(!enif_get_resource(env, argv[0], rt_pub, (void**) &res_pub))
  {
    return enif_make_badarg(env);
  }
  printf("1\n");
  if(!enif_get_resource(env, argv[1], rt_node, (void**) &res_node))
  {
    return enif_make_badarg(env);
  }
  printf("2\n");
  if(!enif_get_resource(env, argv[2], rt_rosidl_msg_type_support, (void**) &res_idl))
  {
    return enif_make_badarg(env);
  }
  printf("3\n");
  char buf[128]; //トピック名を格納するためのバッファ
  (void)memset(&buf,'\0',sizeof(buf));
  if(!enif_get_string(env,argv[3],buf,sizeof(buf),ERL_NIF_LATIN1)){
    return enif_make_badarg(env);
  }
  printf("4\n");
  if(!enif_get_resource(env, argv[4], rt_pub_options, (void**) &res_options))
  {
    return enif_make_badarg(env);
  }
  printf("5\n");
  //各構造体ポインタそれぞれについて，alloc_resource
  res_return = enif_alloc_resource(rt_ret,sizeof(rcl_ret_t));
  if(res_return == NULL) return enif_make_badarg(env);
  /*
  res_pub = enif_alloc_resource(rt_pub,sizeof(rcl_publisher_t));
  if(res_pub == NULL) return enif_make_badarg(env);
  res_node = enif_alloc_resource(rt_node,sizeof(rcl_node_t));
  if(res_node == NULL) return enif_make_badarg(env);
  res_idl = enif_alloc_resource(rt_rosidl_msg_type_support,sizeof(rosidl_message_type_support_t));
  if(res_idl == NULL) return enif_make_badarg(env);
  res_options = enif_alloc_resource(rt_pub_options,sizeof(rcl_publisher_options_t));
  if(res_options == NULL) return enif_make_badarg(env);
  */
  ret = enif_make_resource(env,res_return);
  enif_release_resource(res_return);
  printf("6\n");
  *res_return = rcl_publisher_init(res_pub,res_node,res_idl,buf,res_options);
  return ret;
}

//wrap関数6
ERL_NIF_TERM nif_rcl_publisher_is_valid(ErlNifEnv* env,int argc,const ERL_NIF_TERM argv[]){
  rcl_publisher_t* res;
  if(argc != 1){
    return enif_make_badarg(env);
  }
  if(!enif_get_resource(env, argv[0], rt_pub, (void**) &res))
  {
    return enif_make_badarg(env);
  }

  bool result;
  result = rcl_publisher_is_valid(res);
  if(result == true){
    return atom_true;
  }
  else return atom_false;
}

//wrap関数その7
ERL_NIF_TERM nif_rcl_publish(ErlNifEnv* env, int argc, const ERL_NIF_TERM argv[]){
  rcl_ret_t* res;
  ERL_NIF_TERM ret;

  rcl_publisher_t* res_arg_pub;
  const void * ros_message; //void*にはどんな型でも入って，使う場合に任意の型にキャストする．
  rmw_publisher_allocation_t* res_arg_puballoc;

  if(argc != 3)
  {
      return enif_make_badarg(env);
  }

  if(!enif_get_resource(env, argv[0], rt_pub, (void**) &res_arg_pub))
  {
    return enif_make_badarg(env);
  }
  
  if(!enif_get_resource(env, argv[2], rt_rmw_pub_allocation, (void**) &res_arg_puballoc))
  {
    return enif_make_badarg(env);
  }

  res = enif_alloc_resource(rt_ret,sizeof(rcl_ret_t));
  if(res == NULL) return enif_make_badarg(env);
  /*お前らはいらんやろ
  res_arg_pub = enif_alloc_resource(rt_pub,sizeof(rcl_publisher_t));
  if(res_arg_pub == NULL) return enif_make_badarg(env);
  res_arg_puballoc = enif_alloc_resource(rt_rmw_pub_allocation,sizeof(rmw_publisher_allocation_t));
  if(res_arg_node == NULL) return enif_make_badarg(env);
  */
  ret = enif_make_resource(env,res);
  enif_release_resource(res);
  
  *res = rcl_publish(res_arg_pub,ros_message,res_arg_puballoc);
  
  return ret;
}

static ErlNifFunc nif_funcs[] = {
    {"rcl_get_zero_initialized_publisher",0,nif_rcl_get_zero_initialized_publisher},
    {"rcl_publisher_get_default_options",0,nif_rcl_publisher_get_default_options},
    {"rcl_publisher_get_topic_name",1,nif_rcl_publisher_get_topic_name},
    {"rcl_publisher_fini",2,nif_rcl_publisher_fini},
    {"rcl_publisher_init",5,nif_rcl_publisher_init},
    {"rcl_publisher_is_valid",1,nif_rcl_publisher_is_valid},
    {"rcl_publish",3,nif_rcl_publish},
}

ERL_NIF_INIT(Elixir.Publisher,nif_funcs,&load,&reload,NULL,NULL);
#ifdef __cplusplus
}
#endif