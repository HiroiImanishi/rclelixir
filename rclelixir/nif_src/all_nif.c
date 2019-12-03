
#ifdef __cplusplus
extern "C"
{
#endif
#include <erl_nif.h>
#include "rcl/node.h"
#include "rcl/init.h"
#include "rcl/context.h"
#include "rcl/init_options.h"
#include "rcl/node.h"

#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "rcl/arguments.h"
#include "rcl/error_handling.h"
//#include "rcl/localhost.h"
#include "rcl/logging.h"
#include "rcl/logging_rosout.h"
#include "rcl/rcl.h"
#include "rcl/remap.h"
#include "rcl/security_directory.h"
#include "rcutils/filesystem.h"
#include "rcutils/find.h"
#include "rcutils/format_string.h"
#include "rcutils/get_env.h"
#include "rcutils/logging.h"
#include "rcutils/macros.h"
#include "rcutils/repl_str.h"
#include "rcutils/snprintf.h"
#include "rcutils/strdup.h"
#include "rmw/error_handling.h"
#include "rmw/node_security_options.h"
#include "rmw/rmw.h"
#include "rmw/validate_namespace.h"
#include "rmw/validate_node_name.h"
//#include "tracetools/tracetools.h"

//#include "../src/context_impl.h"
//ここまでコメントアウトしてた

//---------publisher_nif.c------------
#include "rcl/publisher.h"
#include "rcl/allocator.h"
#include "rcl/error_handling.h"
#include "rcl/expand_topic_name.h"
#include "rcl/remap.h"
#include "rcutils/logging.h"
#include "rmw/validate_full_topic_name.h"

//---------msg_int16_nif.c------------
#include "msg_types.h"
#include <rosidl_generator_c/message_type_support_struct.h>
#include "std_msgs/msg/int16__struct.h"

//--------------------allocate_nif.c-----------------------------

#define ROS_SECURITY_STRATEGY_VAR_NAME "ROS_SECURITY_STRATEGY"
#define ROS_SECURITY_ENABLE_VAR_NAME "ROS_SECURITY_ENABLE"
#define ROS_DOMAIN_ID_VAR_NAME "ROS_DOMAIN_ID"


//------------------------nif_node.cとnif_init.cをマージ-------------------

ErlNifResourceType* rt_node;
ErlNifResourceType* rt_ret;
ErlNifResourceType* rt_context;
ErlNifResourceType* rt_node_options;
ErlNifResourceType* rt_init_options;

//---------publisher_nif.c------------
ErlNifResourceType* rt_pub;
ErlNifResourceType* rt_pub_options;
ErlNifResourceType* rt_rosidl_msg_type_support;
ErlNifResourceType* rt_rmw_pub_allocation;

//---------msg_int16_nif.c------------
ErlNifResourceType* rt_Int16;

ERL_NIF_TERM atom_ok;
ERL_NIF_TERM atom_true;
ERL_NIF_TERM atom_false;

//リソースタイプを作る．load()から呼び出される
static int open_resource(ErlNifEnv* env){
    const char* mod = "Elixir.Node";
    const char* name1 = "rcl_node_t";
    const char* name2 = "rcl_ret_t";
    const char* name3 = "rcl_node_options_t";
    const char* name4 = "rcl_context_t";
    const char* name5 = "rcl_init_options_t";

    const char* name6 = "rcl_publisher_t";
    const char* name7 = "rcl_publisher_options_t";
    const char* name8 = "rosidl_message_type_support_t";
    const char* name9 = "rmw_publisher_allocation_t";

    const char* name10 = "std_msgs__msg__Int16_t";

    int flags = ERL_NIF_RT_CREATE | ERL_NIF_RT_TAKEOVER;

    //RES_TYPE = enif_open_resource_type(env,mod,name,free_res,flags,NULL);
    rt_node         = enif_open_resource_type(env,mod,name1,NULL,flags,NULL);
    rt_ret          = enif_open_resource_type(env,mod,name2,NULL,flags,NULL);
    rt_node_options = enif_open_resource_type(env,mod,name3,NULL,flags,NULL);
    rt_context      = enif_open_resource_type(env,mod,name4,NULL,flags,NULL);
    rt_init_options = enif_open_resource_type(env,mod,name5,NULL,flags,NULL);
    
    rt_pub          = enif_open_resource_type(env,mod,name6,NULL,flags,NULL);
    rt_pub_options  = enif_open_resource_type(env,mod,name7,NULL,flags,NULL);
    rt_rosidl_msg_type_support = enif_open_resource_type(env,mod,name8,NULL,flags,NULL);
    rt_rmw_pub_allocation = enif_open_resource_type(env,mod,name9,NULL,flags,NULL);
   
    rt_Int16 = enif_open_resource_type(env,mod,name10,NULL,flags,NULL);

    //1〜5まで
    if(rt_node == NULL || rt_ret == NULL || rt_node_options == NULL || rt_context == NULL || rt_init_options == NULL) return -1;
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

//---------------------------nif_init.c-------------------
ERL_NIF_TERM nif_rcl_get_zero_initialized_init_options(ErlNifEnv* env, int argc, const ERL_NIF_TERM argv[]){
    if(argc != 0){
        return enif_make_badarg(env);
    }
    rcl_init_options_t* res;
    ERL_NIF_TERM ret;
    res = enif_alloc_resource(rt_init_options,sizeof(rcl_init_options_t));
    if(res == NULL) return enif_make_badarg(env);
    ret = enif_make_resource(env,res);
    enif_release_resource(res);

    *res = rcl_get_zero_initialized_init_options();

    return ret;
}


ERL_NIF_TERM nif_rcl_get_zero_initialized_context(ErlNifEnv* env, int argc, const ERL_NIF_TERM argv[]){
    if(argc != 0){
        return enif_make_badarg(env);
    }
    rcl_context_t* res;
    ERL_NIF_TERM ret;
    res = enif_alloc_resource(rt_context,sizeof(rcl_context_t));
    if(res == NULL) return enif_make_badarg(env);
    ret = enif_make_resource(env,res);
    enif_release_resource(res);

    *res = rcl_get_zero_initialized_context();

    return ret;
}

ERL_NIF_TERM nif_rcl_init(ErlNifEnv* env, int argc, const ERL_NIF_TERM argv[]){
    
    if(argc != 4){
      
        return enif_make_badarg(env);
    }

    rcl_ret_t* res;
    ERL_NIF_TERM ret;
    int argcval;
    char argv_buf[1024];    
    (void)memset(&argv_buf, '\0', sizeof(argv_buf));
   
    const rcl_init_options_t* res_arg_options;
    rcl_context_t* res_arg_context;
    if(!enif_get_int(env,argv[0],&argcval)){
        return enif_make_badarg(env);
    }
    
    if(!enif_get_string(env,argv[1],argv_buf,sizeof(argv_buf),ERL_NIF_LATIN1)){
        return enif_make_badarg(env);
    }
   
    if(!enif_get_resource(env, argv[2], rt_init_options, (void**) &res_arg_options)){
	    return enif_make_badarg(env);
    }
    
    if(!enif_get_resource(env, argv[3], rt_context, (void**) &res_arg_context)){
	    return enif_make_badarg(env);
    }
    
    res = enif_alloc_resource(rt_ret,sizeof(rcl_ret_t));
    if(res == NULL) return enif_make_badarg(env);
    ret = enif_make_resource(env,res);
    enif_release_resource(res);
    
   
    //ポインタも，その指し示す先もconst(変更不可)であることを示す
    *res = rcl_init(argcval,(const char * const *)argv_buf,res_arg_options,res_arg_context);
    
    return ret;
}

ERL_NIF_TERM nif_rcl_shutdown(ErlNifEnv* env, int argc, const ERL_NIF_TERM argv[]){
    if(argc != 1){
        return enif_make_badarg(env);
    }
    rcl_ret_t* res;
    ERL_NIF_TERM ret;

    rcl_context_t* res_arg_context;
    if(!enif_get_resource(env,argv[0],rt_context,(void**) &res_arg_context)){
        return enif_make_badarg(env);
    }
    res = enif_alloc_resource(rt_ret,sizeof(rcl_ret_t));
    if(res == NULL) return enif_make_badarg(env);
    ret = enif_make_resource(env,res);
    enif_release_resource(res);

    *res = rcl_shutdown(res_arg_context);

    return ret;
}

//-----------------------------node_nif.c-----------------------------

ERL_NIF_TERM nif_rcl_get_zero_initialized_node(ErlNifEnv* env,int argc,const ERL_NIF_TERM argv[]){
    rcl_node_t* res;
    ERL_NIF_TERM ret;
    
    if(argc != 0){
        return enif_make_badarg(env);
    }
    res = enif_alloc_resource(rt_node,sizeof(rcl_node_t));
    if(res == NULL) return enif_make_badarg(env);

    ret = enif_make_resource(env,res);
    enif_release_resource(res);
    
    *res = rcl_get_zero_initialized_node();

    return ret;
}

ERL_NIF_TERM nif_rcl_node_init(ErlNifEnv* env, int argc, const ERL_NIF_TERM argv[]){
   
    if(argc != 5){
        
        return enif_make_badarg(env);
    }
   
    rcl_ret_t* res;
    ERL_NIF_TERM ret;

    rcl_node_t* res_arg_node;
    rcl_context_t* res_arg_context;
    const rcl_node_options_t* res_arg_options;

    char name_buf[128];
    char namespace_buf[128];    
    (void)memset(&name_buf,'\0',sizeof(name_buf));
    (void)memset(&namespace_buf,'\0',sizeof(namespace_buf));
   
    if(!enif_get_resource(env, argv[0], rt_node, (void**) &res_arg_node)){
        return enif_make_badarg(env);
    }
    
    if(!enif_get_string(env,argv[1],name_buf,sizeof(name_buf),ERL_NIF_LATIN1)){
        return enif_make_badarg(env);
    }
    
    if(!enif_get_string(env,argv[2],name_buf,sizeof(namespace_buf),ERL_NIF_LATIN1)){
        return enif_make_badarg(env);
    }
   
    if(!enif_get_resource(env, argv[3], rt_context, (void**) &res_arg_context)){
        return enif_make_badarg(env);
    }
   
    if(!enif_get_resource(env, argv[4], rt_node_options, (void**) &res_arg_options)){
        return enif_make_badarg(env);
    }
   
    res = enif_alloc_resource(rt_ret,sizeof(rcl_ret_t));
    if(res == NULL) return enif_make_badarg(env);
    /*
    res_arg_node = enif_alloc_resource(rt_node,sizeof(rcl_node_t));
    if(res_arg_node == NULL) return enif_make_badarg(env);
    res_arg_context = enif_alloc_resource(rt_context,sizeof(rcl_context_t));
    if(res_arg_context == NULL) return enif_make_badarg(env);
    res_arg_options = enif_alloc_resource(rt_node_options,sizeof(rcl_node_options_t));
    if(res_arg_options == NULL) return enif_make_badarg(env);
    */
    ret = enif_make_resource(env,res);
    enif_release_resource(res);
   
    *res = rcl_node_init(res_arg_node,name_buf,namespace_buf,res_arg_context,res_arg_options);
    
    return ret;
}

ERL_NIF_TERM nif_rcl_node_fini(ErlNifEnv* env,int argc,const ERL_NIF_TERM argv[]){
    if(argc != 1){
        return enif_make_badarg(env);
    }
    rcl_ret_t* res;
    ERL_NIF_TERM ret;

    rcl_node_t* res_arg_node;
    if(!enif_get_resource(env, argv[0], rt_node, (void**) &res_arg_node)){
        return enif_make_badarg(env);
    }
    res = enif_alloc_resource(rt_ret,sizeof(rcl_ret_t));
    if(res == NULL) return enif_make_badarg(env);
    res_arg_node = enif_alloc_resource(rt_node,sizeof(rcl_node_t));
    if(res_arg_node == NULL) return enif_make_badarg(env);

    ret = enif_make_resource(env,res);
    enif_release_resource(res);

    *res = rcl_node_fini(res_arg_node);
    return ret;
} 

ERL_NIF_TERM nif_rcl_node_get_default_options(ErlNifEnv* env, int argc, const ERL_NIF_TERM argv[]){
    if(argc != 0){
        return enif_make_badarg(env);
    }

    rcl_node_options_t* res;
    ERL_NIF_TERM ret;

    res = enif_alloc_resource(rt_node_options,sizeof(rcl_node_options_t));
    if(res == NULL) return enif_make_badarg(env);

    ret = enif_make_resource(env,res);
    enif_release_resource(res);
    
    *res = rcl_node_get_default_options();

    return ret;
    
}

//------------------------------publisher_nif.c----------------------------
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
  
  ret = enif_make_resource(env,res_return);
  enif_release_resource(res_return);
  printf("6\n");
  *res_return = rcl_publisher_init(res_pub,res_node,res_idl,buf,res_options);
  printf("7\n");
  return ret;
}

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
  ret = enif_make_resource(env,res);
  enif_release_resource(res);
  
  *res = rcl_publish(res_arg_pub,ros_message,res_arg_puballoc);
  
  return ret;
}

/*
void test(rcl_context_t* context){
    printf("test\n");
}

ERL_NIF_TERM nif_test(ErlNifEnv* env, int argc, const ERL_NIF_TERM argv[]){
    printf("1\n");
    if(argc != 1){
        return enif_make_badarg(env);
    }
    //リソース項からデータを取得する
    rcl_context_t* res_arg_context;
    if(!enif_get_resource(env,argv[0],rt_context,(void**) &res_arg_context)){
        return enif_make_badarg(env);
    }
    printf("2\n");
    test(res_arg_context);
    return enif_make_string(env,"test clear",ERL_NIF_LATIN1);
}
*/
//-----------------------------msg_int16_nif.c-------------------------------
ERL_NIF_TERM nif_init_Int16(ErlNifEnv* env, int argc, const ERL_NIF_TERM argv[]){
    if(argc != 0){
        return enif_make_badarg(env);
    }
    std_msgs__msg__Int16* res;
    ERL_NIF_TERM ret;
    res = enif_alloc_resource(rt_Int16,sizeof(std_msgs__msg__Int16));
    if(res == NULL) return enif_make_badarg(env);
    ret = enif_make_resource(env,res);
    enif_release_resource(res);
    res = init_std_msgs_msg_Int16();
    return ret;
}

ERL_NIF_TERM nif_destroy_Int16(ErlNifEnv* env, int argc, const ERL_NIF_TERM argv[]){
    if(argc != 1){
        return enif_make_badarg(env);
    }
    std_msgs__msg__Int16* res_arg;
    ERL_NIF_TERM ret;
    if(!enif_get_resource(env,argv[0],rt_Int16,(void**)&res_arg)){
        return enif_make_badarg(env);
    }
    destroy_std_msgs_msg_Int16(res_arg);
    return enif_make_atom(env,"ok");
}

ERL_NIF_TERM nif_getmsgtype_Int16(ErlNifEnv* env, int argc, const ERL_NIF_TERM argv[]){
    if(argc != 0){
        return enif_make_badarg(env);
    }
    const rosidl_message_type_support_t* res;
    ERL_NIF_TERM ret;
    
    res = enif_alloc_resource(rt_rosidl_msg_type_support,sizeof(rosidl_message_type_support_t));
    if(res == NULL) return enif_make_badarg(env);
    ret = enif_make_resource(env,res);
    enif_release_resource(res);
    res = get_message_type_from_std_msgs_msg_Int16();

    return ret;
}

static ErlNifFunc nif_funcs[] = {
    //-----------init_nif.c-----------
    {"rcl_get_zero_initialized_init_options",0,nif_rcl_get_zero_initialized_init_options},
    {"rcl_get_zero_initialized_context",0,nif_rcl_get_zero_initialized_context},
    {"rcl_init",4,nif_rcl_init},
    {"rcl_shutdown",1,nif_rcl_shutdown},
    //--------------node_nif.c--------------
    {"rcl_get_zero_initialized_node",0,nif_rcl_get_zero_initialized_node},
    {"rcl_node_get_default_options",0,nif_rcl_node_get_default_options},
    {"rcl_node_init",5,nif_rcl_node_init},
    {"rcl_node_fini",1,nif_rcl_node_fini},
    //--------------publisher_nif.c-------------
    {"rcl_get_zero_initialized_publisher",0,nif_rcl_get_zero_initialized_publisher},
    {"rcl_publisher_get_default_options",0,nif_rcl_publisher_get_default_options},
    {"rcl_publisher_get_topic_name",1,nif_rcl_publisher_get_topic_name},
    {"rcl_publisher_fini",2,nif_rcl_publisher_fini},
    {"rcl_publisher_init",5,nif_rcl_publisher_init},
    {"rcl_publisher_is_valid",1,nif_rcl_publisher_is_valid},
    {"rcl_publish",3,nif_rcl_publish},
    //---------------msg_int16_nif.c-----------
    {"init_std_msgs_msg_Int16",0,nif_init_Int16},
    {"destroy_std_msgs_msg_Int16",1,nif_destroy_Int16},
    {"get_message_type_from_std_msgs_msg_Int16",0,nif_getmsgtype_Int16},
}

ERL_NIF_INIT(Elixir.All,nif_funcs,&load,&reload,NULL,NULL);

#ifdef __cplusplus
}
#endif