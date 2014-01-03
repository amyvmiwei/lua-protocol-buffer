#include "lua_field.h"
#include "lua_helpers.h"
#include "lua_primitive_field.h"
#include "lua_string_field.h"
#include "lua_enum_field.h"
#include "lua_message_field.h"
#include <google/protobuf/descriptor.pb.h>
#include <google/protobuf/wire_format.h>
#include <google/protobuf/io/printer.h>
#include <google/protobuf/stubs/common.h>
#include <google/protobuf/stubs/strutil.h>


namespace google {
	namespace protobuf {
		namespace compiler {
			namespace mwpb_lua {


				using internal::WireFormat;

				void SetCommonFieldVariables(const FieldDescriptor* descriptor,
					map<string, string>* variables,
					const Options& options) {
						(*variables)["name"] = FieldName(descriptor);
						(*variables)["index"] = SimpleItoa(descriptor->index());
						(*variables)["number"] = SimpleItoa(descriptor->number());
						(*variables)["classname"] = ClassName(FieldScope(descriptor), false);
						(*variables)["declared_type"] = DeclaredTypeMethodName(descriptor->type());

						(*variables)["tag_size"] = SimpleItoa(
							WireFormat::TagSize(descriptor->number(), descriptor->type()));
						(*variables)["deprecation"] = descriptor->options().deprecated()
							? " PROTOBUF_DEPRECATED" : "";

						(*variables)["cppget"] = "Get";
				}

				FieldGenerator::~FieldGenerator() {}

				void FieldGenerator::
					GenerateMergeFromCodedStreamWithPacking(io::Printer* printer) const {
						// Reaching here indicates a bug. Cases are:
						//   - This FieldGenerator should support packing, but this method should be
						//     overridden.
						//   - This FieldGenerator doesn't support packing, and this method should
						//     never have been called.
						GOOGLE_LOG(FATAL) << "GenerateMergeFromCodedStreamWithPacking() "
							<< "called on field generator that does not support packing.";

				}

				FieldGeneratorMap::FieldGeneratorMap(const Descriptor* descriptor,
					const Options& options)
					: descriptor_(descriptor),
					field_generators_(new scoped_ptr<FieldGenerator>[descriptor->field_count()]) {
						// Construct all the FieldGenerators.
						for (int i = 0; i < descriptor->field_count(); i++) {
							field_generators_[i].reset(MakeGenerator(descriptor->field(i), options));
						}
				}

				FieldGenerator* FieldGeneratorMap::MakeGenerator(const FieldDescriptor* field,
					const Options& options) {
						if (field->is_repeated()) {
							switch (field->cpp_type()) {
							case FieldDescriptor::CPPTYPE_MESSAGE:
								return new RepeatedMessageFieldGenerator(field, options);
							case FieldDescriptor::CPPTYPE_STRING:
								switch (field->options().ctype()) {
								default:  // RepeatedStringFieldGenerator handles unknown ctypes.
								case FieldOptions::STRING:
									return new RepeatedStringFieldGenerator(field, options);
								}
							case FieldDescriptor::CPPTYPE_ENUM:
								return new RepeatedEnumFieldGenerator(field, options);
							default:
								return new RepeatedPrimitiveFieldGenerator(field, options);
							}
						} else {
							switch (field->cpp_type()) {
							case FieldDescriptor::CPPTYPE_MESSAGE:
								return new MessageFieldGenerator(field, options);
							case FieldDescriptor::CPPTYPE_STRING:
								switch (field->options().ctype()) {
								default:  // StringFieldGenerator handles unknown ctypes.
								case FieldOptions::STRING:
									return new StringFieldGenerator(field, options);
								}
							case FieldDescriptor::CPPTYPE_ENUM:
								return new EnumFieldGenerator(field, options);
							default:
								return new PrimitiveFieldGenerator(field, options);
							}
						}
				}

				FieldGeneratorMap::~FieldGeneratorMap() {}

				const FieldGenerator& FieldGeneratorMap::get(
					const FieldDescriptor* field) const {
						GOOGLE_CHECK_EQ(field->containing_type(), descriptor_);
						return *field_generators_[field->index()];
				}

			}}}}