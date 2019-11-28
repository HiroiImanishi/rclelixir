defmodule Publisher do
    @on_load :load_nifs
    def load_nifs do
        :erlang.load_nif('../lib/publisher',0)
    end

    def rcl_get_zero_initialized_publisher do
        raise "NIF get_zero_initialized_publisher/0 not implemented"
    end

    def rcl_publisher_get_default_options do
        raise "rcl_get_zero_initialized_publisher/0 not implemented"
    end

    def rcl_publisher_get_topic_name(_a) do
        raise "rcl_get_zero_initialized_publisher/0 not implemented"
    end

    def rcl_publisher_fini(_a,_b) do
        raise "rcl_get_zero_initialized_publisher/0 not implemented"
    end

    def rcl_publisher_init(_a,_b,_c,_d,_e) do
        raise "rcl_get_zero_initialized_publisher/0 not implemented"
    end

    def rcl_publisher_is_valid(_a) do
        raise "rcl_get_zero_initialized_publisher/0 not implemented"
    end
    def rcl_publish(_a,_b,_c) do
        raise "rcl_publish/3 not implemented"
    end

    def setting_pub do
        con = NodeInit.rcl_get_zero_initialized_context()
        op = NodeInit.rcl_get_zero_initialized_init_options()
        NodeInit.rcl_init(0,'hello world',op,con)
        node_op = NodeInit.rcl_node_get_default_options()
        node = NodeInit.rcl_get_zero_initialized_node()
        NodeInit.rcl_node_init(node,'test_node','test_namespace_',con,node_op)
        mypub = rcl_get_zero_initialized_publisher()
        mypubop = Publisher.rcl_publisher_get_default_options()
        type_support = Int16.get_message_type_from_std_msgs_msg_Int16
        ret = Publisher.rcl_publisher_init(mypub,node,type_support,"topicname",mypubop)
    end
    
end