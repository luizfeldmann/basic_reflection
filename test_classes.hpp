#ifndef _CLASSTEST_H_
#define _CLASSTEST_H_

#include "reflection.hpp"

class object
{
    public:
    DECLARE_DYNAMIC_REFLECTION()
};

class shape : public object
{
public:
    double x;
    double y;
    DECLARE_DYNAMIC_REFLECTION()
};

class square : public shape
{
public:
    int width;
    int height;
    DECLARE_DYNAMIC_REFLECTION()
};

class circle : public shape
{
public:
    float radius;
    DECLARE_DYNAMIC_REFLECTION()
};

class animal : public object
{
public:
    float height;
    float weight;
    char bloodType;
    DECLARE_DYNAMIC_REFLECTION()
};

class dog : public animal
{
public:
    char race;
    char color;
    DECLARE_DYNAMIC_REFLECTION()
};

class cat : public animal
{
public:
    char* species;
    long numberFurrballs;
    DECLARE_DYNAMIC_REFLECTION()
};

#endif
