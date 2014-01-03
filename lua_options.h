#ifndef __MWPB_LUA_OPTIONS_HEADER__
#define __MWPB_LUA_OPTIONS_HEADER__
#include <string>

#include <google/protobuf/stubs/common.h>


namespace google {
namespace protobuf {
namespace compiler {
namespace mwpb_lua {

	struct Options 
	{
		Options() : safe_boundary_check(false) {

		}

		string dllexport_decl;
		bool safe_boundary_check;
	};


}
}
}
}


#endif