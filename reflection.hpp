#ifndef _REFLECTION_HPP_
#define _REFLECTION_HPP_

#include <cstddef>
#include <vector>
#include <typeinfo>
#include <cxxabi.h>
#include <iostream>
#include "string.h"

namespace reflection {
// TOOLS
// ==============================================
template<typename T, typename U> constexpr size_t OffsetOfMember(U T::*member)
{
    return (char*)&((T*)nullptr->*member) - (char*)nullptr;
}

template<typename T, typename U> constexpr auto MemberType(U T::*member)
{
    return ((T*)nullptr->*member);
}

#define type_string(variable) abi::__cxa_demangle(typeid(variable).name(), 0, 0, NULL)

// TYPES
// ==============================================
class FieldInfo;
class TypeInfo
{
public:
    const char* name;

    TypeInfo(const char* tname) : name(tname)
    {

    }

    void foreachField(void (*callback)(FieldInfo*));
    FieldInfo* getField(const char* fname);

    FieldInfo* addFieldF(const char* fname, size_t offset, const TypeInfo* type);
    TypeInfo* addFieldT(const char* fname, size_t offset, const TypeInfo* type);
    TypeInfo* copyFieldsT(const TypeInfo* base);

private:
    std::vector<FieldInfo*> fields = std::vector<FieldInfo*>();

};

class TypeTable
{
public:
    static TypeInfo* getType(const char* tname);
    static TypeInfo* addType(const char* tname);
    static TypeInfo* GetOrAdd(const char* tname);

    static size_t countTypes();
    static void foreachType(void (*callback)(TypeInfo*));

private:
    static std::vector<TypeInfo*> types;
};

void printType(TypeInfo* tinfo);

// FIELDS
// ==============================================
class FieldInfo
{
public:
    const char* name;
    const size_t offset;
    const TypeInfo* type;

    FieldInfo(const char* _name, size_t _offset, const TypeInfo* _type) : name(_name), offset(_offset), type(_type)
    {

    }

    template <typename Tobject, typename Tfield>
    bool Read(Tobject& instance, Tfield& output)
    {
        if (!AssertType(output))
            return false;

        output = *((Tfield*)((size_t)&instance + this->offset));

        return true;
    }

    template <typename Tobject, typename Tfield>
    bool Write(Tobject& instance, Tfield& input)
    {
        if (!AssertType(input))
            return false;

        *((Tfield*)((size_t)&instance + this->offset)) = input;

        return true;
    }

private:
    template <typename Tfield>
    bool AssertType(Tfield& variable)
    {
        char *outputtypename = type_string(variable);

        if (strcmp(outputtypename, type->name) != 0)
        {
            std::cerr << "Attempt read/write value of type " << outputtypename << "  to field '" << name << "' of type " << type->name << std::endl;
            return false;
        }

        return true;
    }
};

void printField(FieldInfo* finfo);

// REFLECTOR
// ==============================================
template <typename Tobject>
TypeInfo* reflect(Tobject& instance)
{
    char *instanceTypeName = type_string(instance);
    return TypeTable::getType(instanceTypeName);
}

template <typename Tobject>
TypeInfo* dynamic_reflect(Tobject* instancePtr)
{
    return TypeTable::getType(instancePtr->className());
}

// REGISTRATION
// ===============================================================
#define BEGIN_REFLECTION(classname) const static reflection::TypeInfo* info_##classname = reflection::TypeTable::addType(#classname)
#define IMPLEMENT_DYNAMIC_REFLECTION(classname) const char* classname::className() {return #classname; }
#define DECLARE_DYNAMIC_REFLECTION() virtual const char* className();
#define REGISTER_FIELD(classname, fieldname) ->addFieldT(#fieldname, reflection::OffsetOfMember(&classname::fieldname), reflection::TypeTable::GetOrAdd( type_string( reflection::MemberType(&classname::fieldname) ) ) )
#define INHERIT_BASE(baseclass) ->copyFieldsT(reflection::TypeTable::getType(#baseclass))
#define END_REFLECTION() ;

}
#endif // _REFLECTION_HPP_
