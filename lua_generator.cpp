#include "lua_generator.h"
#include "lua_file.h"
#include "lua_helpers.h"

#include <google/protobuf/io/printer.h>
#include <google/protobuf/io/zero_copy_stream.h>
#include <google/protobuf/descriptor.pb.h>
#include "google/protobuf/stubs/strutil.h"

namespace google {
namespace protobuf {
namespace compiler {
namespace mwpb_lua {

CLuaGenerator::CLuaGenerator()
{

}

CLuaGenerator::~CLuaGenerator()
{

}

bool CLuaGenerator::Generate(const FileDescriptor* file, 
	const string& parameter, 
	GeneratorContext* generator_context, 
	string* error) const
{
	vector<pair<string, string> > options;
	ParseGeneratorParameter(parameter, &options);

	// -----------------------------------------------------------------
	// parse generator options

	// TODO(kenton):  If we ever have more options, we may want to create a
	//   class that encapsulates them which we can pass down to all the
	//   generator classes.  Currently we pass dllexport_decl down to all of
	//   them via the constructors, but we don't want to have to add another
	//   constructor parameter for every option.

	// If the dllexport_decl option is passed to the compiler, we need to write
	// it in front of every symbol that should be exported if this .proto is
	// compiled into a Windows DLL.  E.g., if the user invokes the protocol
	// compiler as:
	//   protoc --cpp_out=dllexport_decl=FOO_EXPORT:outdir foo.proto
	// then we'll define classes like this:
	//   class FOO_EXPORT Foo {
	//     ...
	//   }
	// FOO_EXPORT is a macro which should expand to __declspec(dllexport) or
	// __declspec(dllimport) depending on what is being compiled.
	Options file_options;

	for (int i = 0; i < options.size(); i++) {
		if (options[i].first == "dllexport_decl") {
			file_options.dllexport_decl = options[i].second;
		} else if (options[i].first == "safe_boundary_check") {
			file_options.safe_boundary_check = true;
		} else {
			*error = "Unknown generator option: " + options[i].first;
			return false;
		}
	}

	// -----------------------------------------------------------------


	string basename = StripProto(file->name());
	basename.append(".pb");

	FileGenerator file_generator(file, file_options);

// 	// Generate header.
// 	{
// 		scoped_ptr<io::ZeroCopyOutputStream> output(
// 			generator_context->Open(basename + ".h"));
// 		io::Printer printer(output.get(), '$');
// 		file_generator.GenerateHeader(&printer);
// 	}
// 
// 	// Generate cc file.
// 	{
// 		scoped_ptr<io::ZeroCopyOutputStream> output(
// 			generator_context->Open(basename + ".cc"));
// 		io::Printer printer(output.get(), '$');
// 		file_generator.GenerateSource(&printer);
// 	}

	// Generate cc file.
	{
		// 把basename里面的.pb换成_pb.
		std::string strBaseName = basename;
		std::replace(strBaseName.begin(), strBaseName.end(), '.', '_');
		basename = strBaseName;

	 	scoped_ptr<io::ZeroCopyOutputStream> output(
	 		generator_context->Open(basename + ".lua"));
	 	io::Printer printer(output.get(), '$');
	 	file_generator.GenerateLua(&printer);
	}

	return true;
}


}//mwpb_lua
}//compiler
}//protobuf
}//google