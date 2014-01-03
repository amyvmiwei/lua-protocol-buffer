#ifndef __MWPB_LUA_SERVICE_HEADER__
#define __MWPB_LUA_SERVICE_HEADER__


#include <map>
#include <string>
#include <google/protobuf/stubs/common.h>
#include <google/protobuf/descriptor.h>
#include "lua_options.h"

namespace google {
namespace protobuf {
	namespace io {
		class Printer;             // printer.h
	}
}

namespace protobuf {
namespace compiler {
namespace mwpb_lua {

	class ServiceGenerator {
	public:
		// See generator.cc for the meaning of dllexport_decl.
		explicit ServiceGenerator(const ServiceDescriptor* descriptor,
			const Options& options);
		~ServiceGenerator();

		// Header stuff.

		// Generate the class definitions for the service's interface and the
		// stub implementation.
		void GenerateDeclarations(io::Printer* printer);

		// Source file stuff.

		// Generate code that initializes the global variable storing the service's
		// descriptor.
		void GenerateDescriptorInitializer(io::Printer* printer, int index);

		// Generate implementations of everything declared by GenerateDeclarations().
		void GenerateImplementation(io::Printer* printer);

	private:
		enum RequestOrResponse { REQUEST, RESPONSE };
		enum VirtualOrNon { VIRTUAL, NON_VIRTUAL };

		// Header stuff.

		// Generate the service abstract interface.
		void GenerateInterface(io::Printer* printer);

		// Generate the stub class definition.
		void GenerateStubDefinition(io::Printer* printer);

		// Prints signatures for all methods in the
		void GenerateMethodSignatures(VirtualOrNon virtual_or_non,
			io::Printer* printer);

		// Source file stuff.

		// Generate the default implementations of the service methods, which
		// produce a "not implemented" error.
		void GenerateNotImplementedMethods(io::Printer* printer);

		// Generate the CallMethod() method of the service.
		void GenerateCallMethod(io::Printer* printer);

		// Generate the Get{Request,Response}Prototype() methods.
		void GenerateGetPrototype(RequestOrResponse which, io::Printer* printer);

		// Generate the stub's implementations of the service methods.
		void GenerateStubMethods(io::Printer* printer);

		const ServiceDescriptor* descriptor_;
		map<string, string> vars_;

		GOOGLE_DISALLOW_EVIL_CONSTRUCTORS(ServiceGenerator);
	};



}}}}
#endif