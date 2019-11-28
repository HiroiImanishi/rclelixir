defmodule Int16 do
    @on_load :load_nifs
    def load_nifs do
        :erlang.load_nif('../lib/int16',0)
    end
    def init_std_msgs_msg_Int16 do
        raise "NIF init_std_msgs_msg_int16/0 not implemented"
    end

    def destroy_std_msgs_msg_Int16(_a) do
        raise "NIF destroy_std_msgs_msg_int16/1 not implemented"
    end
    
    def get_message_type_from_std_msgs_msg_Int16 do
        raise "NIF get_message_type_from_std_msgs_msg_Int16/0 not implemented"
    end

    def cre_int16 do
        get_message_type_from_std_msgs_msg_Int16()
    end
end