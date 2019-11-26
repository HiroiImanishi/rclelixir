#ifndef _H_3D1ABD8B97CA639754430FD9BE1181EA_Time__H_
#define _H_3D1ABD8B97CA639754430FD9BE1181EA_Time__H_

#include "sacpp_mapping.h"
#include "cpp_dcps_if.h"


namespace builtin_interfaces
{
    namespace msg
    {
        namespace dds_
        {
            struct  Time_
            {
                ::DDS::Long sec_;
                ::DDS::ULong nanosec_;
            };

            typedef DDS_DCPSStruct_var<Time_> Time__var;
            typedef Time_& Time__out;

        }

    }

}

#endif /* _H_3D1ABD8B97CA639754430FD9BE1181EA_Time__H_ */
