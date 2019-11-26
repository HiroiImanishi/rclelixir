defmodule NodeInit do
    @on_load :load_nifs
    def load_nifs do
        IO.puts "load_nifs"
        :erlang.load_nif('../lib/init',0)
    end
    def rcl_get_zero_initialized_init_options do
        raise "NIF rcl_get_zero_initialized_init_options/0 not implemented"
    end
    def rcl_get_zero_initialized_context do
        raise "NIF rcl_get_zero_initialized_context/0 not implemented"
    end
    def rcl_init(_a,_b,_c,_d) do
        raise "NIF rcl_init/4 not implemented"
    end

    def rcl_shutdown(_a) do
        raise "NIF rcl_shutdown/1 not implemented" 
    end

    def all_init do
        a = rcl_get_zero_initialized_init_options()
        b = rcl_get_zero_initialized_context()
        rcl_init(0,'hello',elem(a,1),elem(b,1))
    end
end