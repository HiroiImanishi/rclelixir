#ifndef _H_CDD83B030AB71EE6850F6461F7F61F2B_Duration__H_
#define _H_CDD83B030AB71EE6850F6461F7F61F2B_Duration__H_

#include "sacpp_mapping.h"
#include "cpp_dcps_if.h"


namespace builtin_interfaces
{
    namespace msg
    {
        namespace dds_
        {
            struct  Duration_
            {
                ::DDS::Long sec_;
                ::DDS::ULong nanosec_;
            };

            typedef DDS_DCPSStruct_var<Duration_> Duration__var;
            typedef Duration_& Duration__out;

        }

    }

}

#endif /* _H_CDD83B030AB71EE6850F6461F7F61F2B_Duration__H_ */
