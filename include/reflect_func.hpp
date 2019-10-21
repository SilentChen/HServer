#ifndef REFLECT_FUNC_H
#define REFLECT_FUNC_H

#define FUNC_REGISTER(func_name) ReflectFuncFactory::registerFunc(#func_name, &func_name)
#define FUNC_INVOKE(func_name) ReflectFuncFactory::invoke(func_name)

#include <string>
#include <map>

typedef void* (*ReflectFunc)();

class ReflectFuncFactory
{
	public:
		static void registerFunc(std::string funcName, ReflectFunc reflectFunc)
		{
			reflectFuncs()[funcName] = reflectFunc;
		}

		static void* invoke(const std::string& funcName)
		{
			ReflectFunc reflectFunc = NULL;

			if(reflectFuncs().find(funcName) != reflectFuncs().end())
				reflectFunc = reflectFuncs().find(funcName)->second;

			if ( reflectFunc == NULL )
				return NULL;

			return (*reflectFunc)();
		}

	private:
		inline static std::map<std::string, ReflectFunc>& reflectFuncs()
		{
			static std::map<std::string, ReflectFunc> instance;
			return instance;
		}
};

#endif
