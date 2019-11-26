#ifndef H_3D1ABD8B97CA639754430FD9BE1181EA_Time_SPLTYPES_H
#define H_3D1ABD8B97CA639754430FD9BE1181EA_Time_SPLTYPES_H

#include <c_base.h>
#include <c_misc.h>
#include <c_sync.h>
#include <c_collection.h>
#include <c_field.h>
#include <v_copyIn.h>

#include "ccpp_Time_.h"


extern c_metaObject __Time__builtin_interfaces__load (c_base base);

extern c_metaObject __Time__builtin_interfaces_msg__load (c_base base);

extern c_metaObject __Time__builtin_interfaces_msg_dds___load (c_base base);

extern const char *builtin_interfaces_msg_dds__Time__metaDescriptor[];
extern const int builtin_interfaces_msg_dds__Time__metaDescriptorArrLength;
extern const int builtin_interfaces_msg_dds__Time__metaDescriptorLength;
extern c_metaObject __builtin_interfaces_msg_dds__Time___load (c_base base);
struct _builtin_interfaces_msg_dds__Time_ ;
extern  v_copyin_result __builtin_interfaces_msg_dds__Time___copyIn(c_base base, const struct builtin_interfaces::msg::dds_::Time_ *from, struct _builtin_interfaces_msg_dds__Time_ *to);
extern  void __builtin_interfaces_msg_dds__Time___copyOut(const void *_from, void *_to);
struct _builtin_interfaces_msg_dds__Time_ {
    c_long sec_;
    c_ulong nanosec_;
};

#undef OS_API
#endif
