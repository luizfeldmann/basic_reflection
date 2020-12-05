#include "reflection.hpp"

using namespace reflection;

// FIELDS
// ==============================================
FieldInfo* TypeInfo::getField(const char* fname)
{
    for (auto t = this->fields.cbegin(); t !=  this->fields.cend(); ++t)
    {
        FieldInfo* finfo = (FieldInfo*)(*t);
        if (strcmp(fname, finfo->name) == 0)
            return finfo;
    }

    return NULL;
}

FieldInfo* TypeInfo::addFieldF(const char* fname, size_t offset, const TypeInfo* type)
{
    if (fname == NULL || type == NULL)
        return NULL;

    FieldInfo* finfo = new FieldInfo(fname, offset, type);
    this->fields.push_back(finfo);

    return finfo;
}

TypeInfo* TypeInfo::addFieldT(const char* fname, size_t offset, const TypeInfo* type)
{
    addFieldF(fname, offset, type);
    return this;
}

TypeInfo* TypeInfo::copyFieldsT(const TypeInfo* base)
{
    if (base != NULL)
    {
        for (auto t = base->fields.cbegin(); t !=  base->fields.cend(); ++t)
        {
            FieldInfo* finfo = (FieldInfo*)(*t);
            this->fields.push_back(finfo);
        }
    }

    return this;
}

void TypeInfo::foreachField(void (*callback)(FieldInfo*))
{
    for (auto t = this->fields.cbegin(); t !=  this->fields.cend(); ++t)
    {
        FieldInfo* finfo = (FieldInfo*)(*t);
        callback(finfo);
    }
}

void reflection::printField(FieldInfo* finfo)
{
    std::cout << "\t" << finfo->type->name << " (" << finfo->offset << ") " << finfo->name << std::endl;
}

// TYPES
// ==============================================
// TYPE TABLE
std::vector<TypeInfo*> TypeTable::types;
TypeInfo* TypeTable::getType(const char* tname)
{
    for (auto t = TypeTable::types.cbegin(); t != TypeTable::types.cend(); ++t)
    {
        TypeInfo* tinfo = (TypeInfo*)(*t);
        if (strcmp(tname, tinfo->name) == 0)
            return tinfo;
    }

    return NULL;
}

TypeInfo* TypeTable::addType(const char* tname)
{
    TypeInfo* tinfo = new TypeInfo(tname);
    TypeTable::types.push_back(tinfo);

    return tinfo;
}

TypeInfo* TypeTable::GetOrAdd(const char* tname)
{
    TypeInfo* tinfo = NULL;

    if ((tinfo = getType(tname)) == NULL)
        tinfo = addType(tname);

    return tinfo;
}

size_t TypeTable::countTypes()
{
    return TypeTable::types.size();
}

void TypeTable::foreachType(void (*callback)(TypeInfo*))
{
    for (auto t = TypeTable::types.cbegin(); t != TypeTable::types.cend(); ++t)
    {
        TypeInfo* tinfo = (TypeInfo*)(*t);
        callback(tinfo);
    }
}

void reflection::printType(TypeInfo* tinfo)
{
    std::cout << tinfo->name << std::endl;
    tinfo->foreachField(reflection::printField);
}
