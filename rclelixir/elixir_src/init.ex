defmodule RCLInit do
    @on_load :load_nifs
    def load_nifs do
        IO.puts "load_nifs"
        :erlang.load_nif('../lib/rclinit',0)
    end
    
    #return rcl_init_options_t
    def rcl_get_zero_initialized_init_options do
        raise "NIF rcl_get_zero_initialized_init_options/0 not implemented"
    end

    #return rcl_context_t
    def rcl_get_zero_initialized_context do
        raise "NIF rcl_get_zero_initialized_context/0 not implemented"
    end
    
    @doc """
        return {:ok,rcl_ret_t}
        arguments...(
            int argc,
            char const * const * argv,
            const rcl_init_options_t * options,
            rcl_context_t * context)
    """
    def rcl_init(_a,_b,_c,_d) do
        raise "NIF rcl_init/4 not implemented"
    end

    @doc """
        return {:ok,rcl_ret_t}
        arguments...(
            rcl_context_t)
    """        
    def rcl_shutdown(_a) do
        raise "NIF rcl_shutdown/1 not implemented" 
    end

    def all_init do
        a = rcl_get_zero_initialized_init_options()
        b = rcl_get_zero_initialized_context()
        ret = rcl_init(0,'hello',elem(a,1),elem(b,1))
    end

    def test(_a) do
        raise "NIF test/1 not implemented"
    end
    
end