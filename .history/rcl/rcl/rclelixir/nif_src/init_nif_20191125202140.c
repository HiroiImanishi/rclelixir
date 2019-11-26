#include <erl_nif.h>
#include <stdio.h>
#include "rcl/init.h"
#include "rcl/context.h"
#include "rcl/init_options.h"

ErlNifResourceType* rt_context;
ErlNifResourceType* rt_ret;
ErlNifResourceType* rt_options;
ERL_NIF_TERM atom_ok;
ERL_NIF_TERM atom_true;
ERL_NIF_TERM atom_false;

void
free_res(ErlNifEnv* env, void* obj){
    printf("enter free_res\n");
    //Tracker* tracker = (Tracker*) enif_priv_data(env);
    
}

//リソースタイプを作る．load()から呼び出される
static int open_resource(ErlNifEnv* env){
    const char* mod = "Elixir.NodeInit";
    const char* name1 = "rcl_context_t";
    const char* name2 = "rcl_ret_t";
    const char* name3 = "rcl_init_options_t";
    int flags = ERL_NIF_RT_CREATE | ERL_NIF_RT_TAKEOVER;

    rt_context = enif_open_resource_type(env,mod,name1,NULL,flags,NULL);
    rt_ret     = enif_open_resource_type(env,mod,name2,NULL,flags,NULL);
    rt_options = enif_open_resource_type(env,mod,name3,NULL,flags,NULL);

    if(rt_context == NULL || rt_ret == NULL || rt_options == NULL) return -1;
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

ERL_NIF_TERM nif_rcl_get_zero_initialized_init_options(ErlNifEnv* env, int argc, const ERL_NIF_TERM argv[]){
    printf("1\n");
    if(argc != 0){
        printf("2\n");
        return enif_make_badarg(env);
    }
    printf("3\n");
    rcl_init_options_t* res;
    ERL_NIF_TERM ret;
    res = enif_alloc_resource(rt_options,sizeof(rcl_init_options_t));
    if(res == NULL) return enif_make_badarg(env);
    ret = enif_make_resource(env,res);
    enif_release_resource(res);

    *res = rcl_get_zero_initialized_init_options();

    return enif_make_tuple2(env,atom_ok,ret);
}

ERL_NIF_TERM nif_rcl_get_zero_initialized_context(ErlNifEnv* env, int argc, const ERL_NIF_TERM argv[]){
    if(argv != 0){
        return enif_make_badarg(env);
    }
    rcl_context_t* res;
    ERL_NIF_TERM ret;
    res = enif_alloc_resource(rt_context,sizeof(rcl_context_t));
    if(res == NULL) return enif_make_badarg(env);
    ret = enif_make_resource(env,res);
    enif_release_resource(res);

    *res = rcl_get_zero_initialized_context();

    return enif_make_tuple2(env,atom_ok,ret);
}

ERL_NIF_TERM nif_rcl_init(ErlNifEnv* env, int argc, const ERL_NIF_TERM argv[]){
    if(argc != 4){
        return enif_make_badarg(env);
    }
    rcl_ret_t* res;
    ERL_NIF_TERM ret;
    int argcval;
    char argv_buf[1024];    //ポインタも，その指し示す先もconst(変更不可)であることを示す
    (void)memset(&argv_buf, '\0', sizeof(argv_buf));
    const rcl_init_options_t* res_arg_options;
    rcl_context_t* res_arg_context;
    if(!enif_get_int(env,argv[0],&argcval)){
        return enif_make_badarg(env);
    }
    if(!enif_get_string(env,argv[1],argv_buf,sizeof(argv_buf),ERL_NIF_LATIN1)){
        return enif_make_badarg(env);
    }
    if(!enif_get_resource(env, argv[2], rt_options, (void**) &res_arg_options)){
	    return enif_make_badarg(env);
    }
    if(!enif_get_resource(env, argv[3], rt_context, (void**) &res_arg_context)){
	    return enif_make_badarg(env);
    }
    res = enif_alloc_resource(rt_ret,sizeof(rcl_ret_t));
    if(res == NULL) return enif_make_badarg(env);
    ret = enif_make_resource(env,res);
    enif_release_resource(res);
    
    //*res = rcl_init(argc,argv_buf,res_arg_options,res_arg_context);
    //引数決め打ち(rclのtest_namespace.cppを参考)
    //rcl_init_options_t init_options = rcl_get_zero_initialized_init_options();
    
    //rcl_context_t init_context = rcl_get_zero_initialized_context();
    //*res = rcl_init(0,NULL,&init_options,&init_context);
    *res = rcl_init(argcval,(const char * const *)argv_buf,res_arg_options,res_arg_context);
    
    return enif_make_tuple2(env,atom_ok,ret);
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
    res_arg_context = enif_alloc_resource(rt_context,sizeof(rcl_context_t));
    if(res_arg_context == NULL) return enif_make_badarg(env);

    res = enif_alloc_resource(rt_ret,sizeof(rcl_ret_t));
    if(res == NULL) return enif_make_badarg(env);
    ret = enif_make_resource(env,res);
    enif_release_resource(res);

    *res = rcl_shutdown(res_arg_context);

    return enif_make_tuple2(env,atom_ok,ret);
}
ErlNifFunc nif_funcs[]={
    {"rcl_get_zero_initialized_init_options",0,nif_rcl_get_zero_initialized_init_options},
    {"rcl_get_zero_initialized_context",0,nif_rcl_get_zero_initialized_context},
    {"rcl_init",4,nif_rcl_init},
    {"rcl_shutdown",1,nif_rcl_shutdown},
};
ERL_NIF_INIT(Elixir.NodeInit,nif_funcs,&load,&reload,NULL,NULL);
#ifdef __cplusplus
}
#endif