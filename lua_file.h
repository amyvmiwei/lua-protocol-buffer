#ifndef __MWPB_LUA_FILE_HEADER__
#define __MWPB_LUA_FILE_HEADER__

#include <string>
#include <vector>
#include <google/protobuf/stubs/common.h>

#include "lua_field.h"
#include "lua_options.h"


namespace google {
	namespace protobuf {
		class FileDescriptor;        // descriptor.h
		namespace io {
			class Printer;             // printer.h
		}
	}

namespace protobuf {
namespace compiler {
namespace mwpb_lua {



	class EnumGenerator;           // enum.h
	class MessageGenerator;        // message.h
	class ServiceGenerator;        // service.h
	class ExtensionGenerator;      // extension.h

	class FileGenerator {
	public:
		// See generator.cc for the meaning of dllexport_decl.
		explicit FileGenerator(const FileDescriptor* file,
			const Options& options);
		~FileGenerator();

		void GenerateHeader(io::Printer* printer);
		void GenerateSource(io::Printer* printer);

		void GenerateLua(io::Printer * printer);

	private:
		// Generate the BuildDescriptors() procedure, which builds all descriptors
		// for types defined in the file.
		void GenerateBuildDescriptors(io::Printer* printer);

		void GenerateNamespaceOpeners(io::Printer* printer);
		void GenerateNamespaceClosers(io::Printer* printer);

		const FileDescriptor* file_;

		scoped_array<scoped_ptr<MessageGenerator> > message_generators_;
		scoped_array<scoped_ptr<EnumGenerator> > enum_generators_;
		scoped_array<scoped_ptr<ServiceGenerator> > service_generators_;
		scoped_array<scoped_ptr<ExtensionGenerator> > extension_generators_;

		// E.g. if the package is foo.bar, package_parts_ is {"foo", "bar"}.
		vector<string> package_parts_;

		const Options options_;

		GOOGLE_DISALLOW_EVIL_CONSTRUCTORS(FileGenerator);
	};


}
}
}
}


#endif