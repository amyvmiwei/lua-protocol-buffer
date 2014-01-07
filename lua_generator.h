#ifndef __MWPB_LUA_GENERATOR_HEADER__
#define __MWPB_LUA_GENERATOR_HEADER__

#include <string>
#include <google/protobuf/compiler/code_generator.h>


namespace google {
namespace protobuf {
namespace compiler {
namespace mwpb_lua {

	class LIBPROTOC_EXPORT CLuaGenerator : public CodeGenerator
	{
	public:
		CLuaGenerator();
		virtual ~CLuaGenerator();

		virtual bool Generate(const FileDescriptor* file,
			const string& parameter,
			GeneratorContext* generator_context, 
			string* error) const;


	protected:
	private:
		GOOGLE_DISALLOW_EVIL_CONSTRUCTORS(CLuaGenerator);
	};

}//mwpb_lua
}//compiler
}//protobuf
}//google


#endif