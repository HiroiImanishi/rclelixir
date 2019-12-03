#ifdef __cplusplus
extern "C"
{
#endif
#include "../include/total_nif.h"

//リソースタイプを作る．load()から呼び出される
int open_resource(ErlNifEnv* env){
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
int load(ErlNifEnv* env, void** priv,ERL_NIF_TERM load_info){
    if(open_resource(env) == -1) return -1;

    atom_ok = enif_make_atom(env,"ok");
    atom_true = enif_make_atom(env,"true");
    atom_false = enif_make_atom(env,"false");
    return 0;
}

int reload(ErlNifEnv* env,void** priv,ERL_NIF_TERM load_info){
    if(open_resource(env) == -1) return -1;
    return 0;
}

ErlNifFunc nif_funcs[] = {
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