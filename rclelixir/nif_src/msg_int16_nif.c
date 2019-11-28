#include <erl_nif.h>
#include "msg_types.h"
#include <rosidl_generator_c/message_type_support_struct.h>
#include "std_msgs/msg/int16__struct.h"
//#include "/opt/ros/dashing/include/std_msgs/msg/int16__struct.h"

ErlNifResourceType* rt_Int16;
ErlNifResourceType* rt_msgtypesupport;
//リソースタイプを作る．load()から呼び出される
static int open_resource(ErlNifEnv* env){
    const char* mod = "Elixir.Int16";
    const char* name1 = "std_msgs__msg__Int16_t";
    const char* name2 = "rosidl_message_type_support_t";
    int flags = ERL_NIF_RT_CREATE | ERL_NIF_RT_TAKEOVER;

    rt_Int16 = enif_open_resource_type(env,mod,name1,NULL,flags,NULL);
    rt_msgtypesupport = enif_open_resource_type(env,mod,name2,NULL,flags,NULL);
    if(rt_Int16 == NULL || rt_msgtypesupport == NULL) return -1;
    return 0;
}

//@on_loadで呼び出す
static int load(ErlNifEnv* env, void** priv,ERL_NIF_TERM load_info){
    if(open_resource(env) == -1) return -1;

    //atom_ok = enif_make_atom(env,"ok");
    //atom_true = enif_make_atom(env,"true");
    //atom_false = enif_make_atom(env,"false");
    return 0;
}

static int reload(ErlNifEnv* env,void** priv,ERL_NIF_TERM load_info){
    if(open_resource(env) == -1) return -1;
    return 0;
}

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
    
    res = enif_alloc_resource(rt_msgtypesupport,sizeof(rosidl_message_type_support_t));
    if(res == NULL) return enif_make_badarg(env);
    ret = enif_make_resource(env,res);
    enif_release_resource(res);
    res = get_message_type_from_std_msgs_msg_Int16();

    return ret;
}

static ErlNifFunc nif_funcs[]={
    {"init_std_msgs_msg_Int16",0,nif_init_Int16},
    {"destroy_std_msgs_msg_Int16",1,nif_destroy_Int16},
    {"get_message_type_from_std_msgs_msg_Int16",0,nif_getmsgtype_Int16},
}

ERL_NIF_INIT(Elixir.Int16,nif_funcs,&load,&reload,NULL,NULL);