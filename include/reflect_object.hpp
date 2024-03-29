#ifndef REFLECT_OBJECT_H
#define REFLECT_OBJECT_H

#define REGISTER_CLASS(class_name) \
	class class_name##Helper { \
		public: \
				class_name##Helper() \
		{ \
			CObjectFactory::registerClass(#class_name, class_name##Helper::creatObjFunc); \
		} \
		static void* creatObjFunc() \
		{ \
			return new class_name; \
		} \
	}; \
class_name##Helper class_name##helper;

#include <string>
#include <map>

typedef void* (*Constructor)();

class CObjectFactory
{
	public:
		static void registerClass(std::string className, Constructor constructor)
		{
			constructors()[className] = constructor;
		}

		static void* createObject(const std::string& className)
		{
			Constructor constructor = NULL;

			if(constructors().find(className) != constructors().end())
				constructor = constructors().find(className)->second;

			if ( constructor == NULL )
				return NULL;

			return (*constructor)();
		}

	private:
		inline static std::map<std::string, Constructor>& constructors()
		{
			static std::map<std::string, Constructor> instance;
			return instance;
		}
};

#endif
