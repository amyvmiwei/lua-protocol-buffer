#ifndef __MWPB_LUA_EXTENSION_HEADER__
#define __MWPB_LUA_EXTENSION_HEADER__

#include <string>
#include <google/protobuf/stubs/common.h>
#include "lua_options.h"

namespace google {
namespace protobuf {
	class FieldDescriptor;       // descriptor.h
	namespace io {
		class Printer;             // printer.h
	}
}

namespace protobuf {
namespace compiler {
namespace mwpb_lua {


	// Generates code for an extension, which may be within the scope of some
	// message or may be at file scope.  This is much simpler than FieldGenerator
	// since extensions are just simple identifiers with interesting types.
	class ExtensionGenerator {
	public:
		// See generator.cc for the meaning of dllexport_decl.
		explicit ExtensionGenerator(const FieldDescriptor* desycriptor,
			const Options& options);
		~ExtensionGenerator();

		// Header stuff.
		void GenerateDeclaration(io::Printer* printer);

		// Source file stuff.
		void GenerateDefinition(io::Printer* printer);

		// Generate code to register the extension.
		void GenerateRegistration(io::Printer* printer);

	private:
		const FieldDescriptor* descriptor_;
		string type_traits_;
		Options options_;

		GOOGLE_DISALLOW_EVIL_CONSTRUCTORS(ExtensionGenerator);
	};



}}}}


#endif