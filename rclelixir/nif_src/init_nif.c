#ifdef __cplusplus
extern "C"
{
#endif

#include <erl_nif.h>
#include <stdio.h>
#include "rcl/init.h"
#include "rcl/context.h"
#include "rcl/init_options.h"
#include "rcl/node.h"
ErlNifResourceType* rt_context;
ErlNifResourceType* rt_ret;
ErlNifResourceType* rt_options;
ErlNifResourceType* rt_node;

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
    const char* mod = "Elixir.RCLInit";
    const char* name1 = "rcl_context_t";
    const char* name2 = "rcl_ret_t";
    const char* name3 = "rcl_init_options_t";
    const char* name4 = "rcl_node_t";
    int flags = ERL_NIF_RT_CREATE | ERL_NIF_RT_TAKEOVER;

    rt_context = enif_open_resource_type(env,mod,name1,NULL,flags,NULL);
    rt_ret     = enif_open_resource_type(env,mod,name2,NULL,flags,NULL);
    rt_options = enif_open_resource_type(env,mod,name3,NULL,flags,NULL);
    rt_node = enif_open_resource_type(env,mod,name4,NULL,flags,NULL);
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
    printf("op1\n");
    if(argc != 0){
        printf("op2\n");
        return enif_make_badarg(env);
    }
    printf("op3\n");
    rcl_init_options_t* res;
    ERL_NIF_TERM ret;
    res = enif_alloc_resource(rt_options,sizeof(rcl_init_options_t));
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
    printf("ini1\n");
    if(argc != 4){
        printf("ini2\n");
        return enif_make_badarg(env);
    }

    rcl_ret_t* res;
    ERL_NIF_TERM ret;
    int argcval;
    char argv_buf[1024];    
    (void)memset(&argv_buf, '\0', sizeof(argv_buf));
    printf("ini3\n");
    const rcl_init_options_t* res_arg_options;
    rcl_context_t* res_arg_context;
    if(!enif_get_int(env,argv[0],&argcval)){
        return enif_make_badarg(env);
    }
    printf("ini4\n");
    if(!enif_get_string(env,argv[1],argv_buf,sizeof(argv_buf),ERL_NIF_LATIN1)){
        return enif_make_badarg(env);
    }
    printf("ini5\n");
    if(!enif_get_resource(env, argv[2], rt_options, (void**) &res_arg_options)){
	    return enif_make_badarg(env);
    }
    printf("ini6\n");
    if(!enif_get_resource(env, argv[3], rt_context, (void**) &res_arg_context)){
	    return enif_make_badarg(env);
    }
    printf("ini7\n");
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

    return enif_make_tuple2(env,atom_ok,ret);
}

void test(rcl_node_t* node){
    printf("test\n");
}

ERL_NIF_TERM nif_test(ErlNifEnv* env, int argc, const ERL_NIF_TERM argv[]){
    printf("1\n");
    if(argc != 1){
        return enif_make_badarg(env);
    }
    //リソース項からデータを取得する
    rcl_node_t* res_arg_node;
    if(!enif_get_resource(env,argv[0],rt_node,(void**) &res_arg_node)){
        return enif_make_badarg(env);
    }
    printf("2\n");
    test(res_arg_node);
    return enif_make_string(env,"test clear",ERL_NIF_LATIN1);
}

ErlNifFunc nif_funcs[]={
    {"test",1,nif_test},
    {"rcl_get_zero_initialized_init_options",0,nif_rcl_get_zero_initialized_init_options},
    {"rcl_get_zero_initialized_context",0,nif_rcl_get_zero_initialized_context},
    {"rcl_init",4,nif_rcl_init},
    {"rcl_shutdown",1,nif_rcl_shutdown},
};
ERL_NIF_INIT(Elixir.RCLInit,nif_funcs,&load,&reload,NULL,NULL);
#ifdef __cplusplus
}
#endif