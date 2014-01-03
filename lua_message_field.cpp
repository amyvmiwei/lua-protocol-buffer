#include "lua_message_field.h"
#include "lua_helpers.h"
#include <google/protobuf/io/printer.h>
#include <google/protobuf/stubs/strutil.h>

namespace google {
namespace protobuf {
namespace compiler {
namespace mwpb_lua {

namespace {

	void SetMessageVariables(const FieldDescriptor* descriptor,
		map<string, string>* variables,
		const Options& options) {
			SetCommonFieldVariables(descriptor, variables, options);
			(*variables)["type"] = FieldMessageTypeName(descriptor);
			(*variables)["stream_writer"] = (*variables)["declared_type"] +
				(HasFastArraySerialization(descriptor->message_type()->file()) ?
				"MaybeToArray" :
			"");
	}

}  // namespace

// ===================================================================


MessageFieldGenerator::
	MessageFieldGenerator(const FieldDescriptor* descriptor,
	const Options& options)
	: descriptor_(descriptor) {
		SetMessageVariables(descriptor, &variables_, options);
}

MessageFieldGenerator::~MessageFieldGenerator() {}

void MessageFieldGenerator::
	GeneratePrivateMembers(io::Printer* printer) const {
		//printer->Print(variables_, "$type$* $name$_;\n");
		printer->Print(variables_, "$name$_ = $type$,\n");
}

void MessageFieldGenerator::
	GenerateAccessorDeclarations(io::Printer* printer) const {
// 		printer->Print(variables_,
// 			"inline const $type$& $name$() const$deprecation$;\n"
// 			"inline $type$* mutable_$name$()$deprecation$;\n"
// 			"inline $type$* release_$name$()$deprecation$;\n"
// 			"inline void set_allocated_$name$($type$* $name$)$deprecation$;\n");

		printer->Print(variables_, "$classname$.get_$name$ = function()\n");
		printer->Indent();
		printer->Print(variables_, "return $classname$_Member.$name$_\n");
		printer->Outdent();
		printer->Print("end\n\n");

		printer->Print(variables_, "$classname$.set_$name$ = function(var)\n");
		printer->Indent();
		printer->Print(variables_, "$classname$_Member.$name$_ = var\n");
		printer->Print(variables_, "$classname$_Temp.$name$_ = true\n");
		printer->Outdent();
		printer->Print("end\n\n");
}

void MessageFieldGenerator::
	GenerateInlineAccessorDefinitions(io::Printer* printer) const {
		printer->Print(variables_,
			"inline const $type$& $classname$::$name$() const {\n");

		PrintHandlingOptionalStaticInitializers(
			variables_, descriptor_->file(), printer,
			// With static initializers.
			"  return $name$_ != NULL ? *$name$_ : *default_instance_->$name$_;\n",
			// Without.
			"  return $name$_ != NULL ? *$name$_ : *default_instance().$name$_;\n");

		printer->Print(variables_,
			"}\n"
			"inline $type$* $classname$::mutable_$name$() {\n"
			"  set_has_$name$();\n"
			"  if ($name$_ == NULL) $name$_ = new $type$;\n"
			"  return $name$_;\n"
			"}\n"
			"inline $type$* $classname$::release_$name$() {\n"
			"  clear_has_$name$();\n"
			"  $type$* temp = $name$_;\n"
			"  $name$_ = NULL;\n"
			"  return temp;\n"
			"}\n"
			"inline void $classname$::set_allocated_$name$($type$* $name$) {\n"
			"  delete $name$_;\n"
			"  $name$_ = $name$;\n"
			"  if ($name$) {\n"
			"    set_has_$name$();\n"
			"  } else {\n"
			"    clear_has_$name$();\n"
			"  }\n"
			"}\n");
}

void MessageFieldGenerator::
	GenerateClearingCode(io::Printer* printer) const {
		printer->Print(variables_,
			"if ($name$_ != NULL) $name$_->$type$::Clear();\n");
}

void MessageFieldGenerator::
	GenerateMergingCode(io::Printer* printer) const {
		printer->Print(variables_,
			"mutable_$name$()->$type$::MergeFrom(from.$name$());\n");
}

void MessageFieldGenerator::
	GenerateSwappingCode(io::Printer* printer) const {
		printer->Print(variables_, "std::swap($name$_, other->$name$_);\n");
}

void MessageFieldGenerator::
	GenerateConstructorCode(io::Printer* printer) const {
		printer->Print(variables_, "$name$_ = NULL;\n");
}

void MessageFieldGenerator::
	GenerateMergeFromCodedStream(io::Printer* printer) const {
		if (descriptor_->type() == FieldDescriptor::TYPE_MESSAGE) {
// 			printer->Print(variables_,
// 				"DO_(::google::protobuf::internal::WireFormatLite::ReadMessageNoVirtual(\n"
// 				"     input, mutable_$name$()));\n");
			printer->Print(variables_,
				"$classname$_Member.$name$_.MergePartialFromCodedStream();\n");
		} else {
// 			printer->Print(variables_,
// 				"DO_(::google::protobuf::internal::WireFormatLite::ReadGroupNoVirtual(\n"
// 				"      $number$, input, mutable_$name$()));\n");
			assert(0);
		}
}

void MessageFieldGenerator::
	GenerateSerializeWithCachedSizes(io::Printer* printer) const {
// 		printer->Print(variables_,
// 			"::google::protobuf::internal::WireFormatLite::Write$stream_writer$(\n"
// 			"  $number$, this->$name$(), output);\n");
// 		printer->Print(variables_,
// 			"pb_Write_$stream_writer$($number$, $classname$.get_$name$());\n"
// 			);

		printer->Print(variables_, 
			"$classname$_Member.$name$_.SerializeWithCachedSizes();\n");
}

void MessageFieldGenerator::
	GenerateSerializeWithCachedSizesToArray(io::Printer* printer) const {
		printer->Print(variables_,
			"target = ::google::protobuf::internal::WireFormatLite::\n"
			"  Write$declared_type$NoVirtualToArray(\n"
			"    $number$, this->$name$(), target);\n");
}

void MessageFieldGenerator::
	GenerateByteSize(io::Printer* printer) const {
		printer->Print(variables_,
			"total_size += $tag_size$ +\n"
			"  ::google::protobuf::internal::WireFormatLite::$declared_type$SizeNoVirtual(\n"
			"    this->$name$());\n");
}

// ===================================================================

RepeatedMessageFieldGenerator::
	RepeatedMessageFieldGenerator(const FieldDescriptor* descriptor,
	const Options& options)
	: descriptor_(descriptor) {
		SetMessageVariables(descriptor, &variables_, options);
}

RepeatedMessageFieldGenerator::~RepeatedMessageFieldGenerator() {}

void RepeatedMessageFieldGenerator::
	GeneratePrivateMembers(io::Printer* printer) const {
// 		printer->Print(variables_,
// 			"::google::protobuf::RepeatedPtrField< $type$ > $name$_;\n");
		printer->Print(variables_, "$name$_={},\n");
}

void RepeatedMessageFieldGenerator::
	GenerateAccessorDeclarations(io::Printer* printer) const {
// 		printer->Print(variables_,
// 			"inline const $type$& $name$(int index) const$deprecation$;\n"
// 			"inline $type$* mutable_$name$(int index)$deprecation$;\n"
// 			"inline $type$* add_$name$()$deprecation$;\n");
// 		printer->Print(variables_,
// 			"inline const ::google::protobuf::RepeatedPtrField< $type$ >&\n"
// 			"    $name$() const$deprecation$;\n"
// 			"inline ::google::protobuf::RepeatedPtrField< $type$ >*\n"
// 			"    mutable_$name$()$deprecation$;\n");
		printer->Print(variables_, "$classname$.add_$name$ = function(var)\n");
		printer->Indent();
		printer->Print(variables_, "table.insert($classname$_Member.$name$_, var)\n");
		printer->Print(variables_, "$classname$_Temp.$name$_ = true\n");
		printer->Outdent();
		printer->Print("end\n\n");

		printer->Print(variables_, "$classname$.get_$name$ = function(index)\n");
		printer->Indent();
		printer->Print(variables_, "return $classname$_Member.$name$_[index]\n");
		printer->Outdent();
		printer->Print("end\n\n");
}

void RepeatedMessageFieldGenerator::
	GenerateInlineAccessorDefinitions(io::Printer* printer) const {
		printer->Print(variables_,
			"inline const $type$& $classname$::$name$(int index) const {\n"
			"  return $name$_.$cppget$(index);\n"
			"}\n"
			"inline $type$* $classname$::mutable_$name$(int index) {\n"
			"  return $name$_.Mutable(index);\n"
			"}\n"
			"inline $type$* $classname$::add_$name$() {\n"
			"  return $name$_.Add();\n"
			"}\n");
		printer->Print(variables_,
			"inline const ::google::protobuf::RepeatedPtrField< $type$ >&\n"
			"$classname$::$name$() const {\n"
			"  return $name$_;\n"
			"}\n"
			"inline ::google::protobuf::RepeatedPtrField< $type$ >*\n"
			"$classname$::mutable_$name$() {\n"
			"  return &$name$_;\n"
			"}\n");
}

void RepeatedMessageFieldGenerator::
	GenerateClearingCode(io::Printer* printer) const {
		printer->Print(variables_, "$name$_.Clear();\n");
}

void RepeatedMessageFieldGenerator::
	GenerateMergingCode(io::Printer* printer) const {
		printer->Print(variables_, "$name$_.MergeFrom(from.$name$_);\n");
}

void RepeatedMessageFieldGenerator::
	GenerateSwappingCode(io::Printer* printer) const {
		printer->Print(variables_, "$name$_.Swap(&other->$name$_);\n");
}

void RepeatedMessageFieldGenerator::
	GenerateConstructorCode(io::Printer* printer) const {
		// Not needed for repeated fields.
}

void RepeatedMessageFieldGenerator::
	GenerateMergeFromCodedStream(io::Printer* printer) const {
		if (descriptor_->type() == FieldDescriptor::TYPE_MESSAGE) {
// 			printer->Print(variables_,
// 				"DO_(::google::protobuf::internal::WireFormatLite::ReadMessageNoVirtual(\n"
// 				"      input, add_$name$()));\n");
			printer->Print(variables_,
				"local msgtb = $type$;\n"
				"msgtb.MergePartialFromCodedStream();\n"
				"add_$name$(msgtb);\n"
				"");

		} else {
// 			printer->Print(variables_,
// 				"DO_(::google::protobuf::internal::WireFormatLite::ReadGroupNoVirtual(\n"
// 				"      $number$, input, add_$name$()));\n");
			assert(0);
		}
}

void RepeatedMessageFieldGenerator::
	GenerateSerializeWithCachedSizes(io::Printer* printer) const {
//  		printer->Print(variables_,
//  			"for (int i = 0; i < this->$name$_size(); i++) {\n"
//  			"  ::google::protobuf::internal::WireFormatLite::Write$stream_writer$(\n"
//  			"    $number$, this->$name$(i), output);\n"
//  			"}\n");


		//printer->Print(variables_,
		//	"for i=1, $classname$.$name$_size() do\n"
		//	"  pb_Write_$stream_writer$($number$, $classname$.get_$name$());\n"
		//	"end\n"
		//	);

// 		printer->Print(variables_, 
// 			"$classname$_Member.$name$_.SerializeWithCachedSizes();\n");

		printer->Print(variables_,
			"for i=1, $classname$.$name$_size() do\n"
			"  $classname$.get_$name$(i).SerializeWithCachedSizes();\n"
//			"  pb_Write_$stream_writer$($number$, $classname$.get_$name$());\n"
			"end\n"
			);
}

void RepeatedMessageFieldGenerator::
	GenerateSerializeWithCachedSizesToArray(io::Printer* printer) const {
		printer->Print(variables_,
			"for (int i = 0; i < this->$name$_size(); i++) {\n"
			"  target = ::google::protobuf::internal::WireFormatLite::\n"
			"    Write$declared_type$NoVirtualToArray(\n"
			"      $number$, this->$name$(i), target);\n"
			"}\n");
}

void RepeatedMessageFieldGenerator::
	GenerateByteSize(io::Printer* printer) const {
		printer->Print(variables_,
			"total_size += $tag_size$ * this->$name$_size();\n"
			"for (int i = 0; i < this->$name$_size(); i++) {\n"
			"  total_size +=\n"
			"    ::google::protobuf::internal::WireFormatLite::$declared_type$SizeNoVirtual(\n"
			"      this->$name$(i));\n"
			"}\n");
}

}  // namespace 
}  // namespace compiler
}  // namespace protobuf
}  // namespace google
