#include "lua_extension.h"
#include "lua_helpers.h"

#include <map>

#include <google/protobuf/stubs/strutil.h>
#include <google/protobuf/io/printer.h>
#include <google/protobuf/descriptor.pb.h>

namespace google {
namespace protobuf {
namespace compiler {
namespace mwpb_lua {

	namespace {

		// Returns the fully-qualified class name of the message that this field
		// extends. This function is used in the Google-internal code to handle some
		// legacy cases.
		string ExtendeeClassName(const FieldDescriptor* descriptor) {
			const Descriptor* extendee = descriptor->containing_type();
			return ClassName(extendee, true);
		}

	}  // anonymous namespace

	ExtensionGenerator::ExtensionGenerator(const FieldDescriptor* descriptor,
		const Options& options)
		: descriptor_(descriptor),
		options_(options) {
			// Construct type_traits_.
			if (descriptor_->is_repeated()) {
				type_traits_ = "Repeated";
			}

			switch (descriptor_->cpp_type()) {
			case FieldDescriptor::CPPTYPE_ENUM:
				type_traits_.append("EnumTypeTraits< ");
				type_traits_.append(ClassName(descriptor_->enum_type(), true));
				type_traits_.append(", ");
				type_traits_.append(ClassName(descriptor_->enum_type(), true));
				type_traits_.append("_IsValid>");
				break;
			case FieldDescriptor::CPPTYPE_STRING:
				type_traits_.append("StringTypeTraits");
				break;
			case FieldDescriptor::CPPTYPE_MESSAGE:
				type_traits_.append("MessageTypeTraits< ");
				type_traits_.append(ClassName(descriptor_->message_type(), true));
				type_traits_.append(" >");
				break;
			default:
				type_traits_.append("PrimitiveTypeTraits< ");
				type_traits_.append(PrimitiveTypeName(descriptor_->cpp_type()));
				type_traits_.append(" >");
				break;
			}
	}

	ExtensionGenerator::~ExtensionGenerator() {}

	void ExtensionGenerator::GenerateDeclaration(io::Printer* printer) {
		map<string, string> vars;
		vars["extendee"     ] = ExtendeeClassName(descriptor_);
		vars["number"       ] = SimpleItoa(descriptor_->number());
		vars["type_traits"  ] = type_traits_;
		vars["name"         ] = descriptor_->name();
		vars["field_type"   ] = SimpleItoa(static_cast<int>(descriptor_->type()));
		vars["packed"       ] = descriptor_->options().packed() ? "true" : "false";
		vars["constant_name"] = FieldConstantName(descriptor_);

		// If this is a class member, it needs to be declared "static".  Otherwise,
		// it needs to be "extern".  In the latter case, it also needs the DLL
		// export/import specifier.
		if (descriptor_->extension_scope() == NULL) {
			vars["qualifier"] = "extern";
			if (!options_.dllexport_decl.empty()) {
				vars["qualifier"] = options_.dllexport_decl + " " + vars["qualifier"];
			}
		} else {
			vars["qualifier"] = "static";
		}

		printer->Print(vars,
			"static const int $constant_name$ = $number$;\n"
			"$qualifier$ ::google::protobuf::internal::ExtensionIdentifier< $extendee$,\n"
			"    ::google::protobuf::internal::$type_traits$, $field_type$, $packed$ >\n"
			"  $name$;\n"
			);

	}

	void ExtensionGenerator::GenerateDefinition(io::Printer* printer) {
		// If this is a class member, it needs to be declared in its class scope.
		string scope = (descriptor_->extension_scope() == NULL) ? "" :
			ClassName(descriptor_->extension_scope(), false) + "::";
	string name = scope + descriptor_->name();

	map<string, string> vars;
	vars["extendee"     ] = ExtendeeClassName(descriptor_);
	vars["type_traits"  ] = type_traits_;
	vars["name"         ] = name;
	vars["constant_name"] = FieldConstantName(descriptor_);
	vars["default"      ] = DefaultValue(descriptor_);
	vars["field_type"   ] = SimpleItoa(static_cast<int>(descriptor_->type()));
	vars["packed"       ] = descriptor_->options().packed() ? "true" : "false";
	vars["scope"        ] = scope;

	if (descriptor_->cpp_type() == FieldDescriptor::CPPTYPE_STRING) {
		// We need to declare a global string which will contain the default value.
		// We cannot declare it at class scope because that would require exposing
		// it in the header which would be annoying for other reasons.  So we
		// replace :: with _ in the name and declare it as a global.
		string global_name = StringReplace(name, "::", "_", true);
		vars["global_name"] = global_name;
		printer->Print(vars,
			"const ::std::string $global_name$_default($default$);\n");

		// Update the default to refer to the string global.
		vars["default"] = global_name + "_default";
	}

	// Likewise, class members need to declare the field constant variable.
	if (descriptor_->extension_scope() != NULL) {
		printer->Print(vars,
			"#ifndef _MSC_VER\n"
			"const int $scope$$constant_name$;\n"
			"#endif\n");
	}

	printer->Print(vars,
		"::google::protobuf::internal::ExtensionIdentifier< $extendee$,\n"
		"    ::google::protobuf::internal::$type_traits$, $field_type$, $packed$ >\n"
		"  $name$($constant_name$, $default$);\n");
	}

	void ExtensionGenerator::GenerateRegistration(io::Printer* printer) {
		map<string, string> vars;
		vars["extendee"   ] = ExtendeeClassName(descriptor_);
		vars["number"     ] = SimpleItoa(descriptor_->number());
		vars["field_type" ] = SimpleItoa(static_cast<int>(descriptor_->type()));
		vars["is_repeated"] = descriptor_->is_repeated() ? "true" : "false";
		vars["is_packed"  ] = (descriptor_->is_repeated() &&
			descriptor_->options().packed())
			? "true" : "false";

		switch (descriptor_->cpp_type()) {
		case FieldDescriptor::CPPTYPE_ENUM:
			printer->Print(vars,
				"::google::protobuf::internal::ExtensionSet::RegisterEnumExtension(\n"
				"  &$extendee$::default_instance(),\n"
				"  $number$, $field_type$, $is_repeated$, $is_packed$,\n");
			printer->Print(
				"  &$type$_IsValid);\n",
				"type", ClassName(descriptor_->enum_type(), true));
			break;
		case FieldDescriptor::CPPTYPE_MESSAGE:
			printer->Print(vars,
				"::google::protobuf::internal::ExtensionSet::RegisterMessageExtension(\n"
				"  &$extendee$::default_instance(),\n"
				"  $number$, $field_type$, $is_repeated$, $is_packed$,\n");
			printer->Print(
				"  &$type$::default_instance());\n",
				"type", ClassName(descriptor_->message_type(), true));
			break;
		default:
			printer->Print(vars,
				"::google::protobuf::internal::ExtensionSet::RegisterExtension(\n"
				"  &$extendee$::default_instance(),\n"
				"  $number$, $field_type$, $is_repeated$, $is_packed$);\n");
			break;
		}
	}


}}}}