#pragma once

#include "TrioApiRequisites.h"

#include "IServiceProvider.h"
#include <string>
#include <map>

namespace BrunoEngine
{
	class IServiceBase;

	class BRUNO_API_EXPORT GameServiceContainer : public IServiceProvider
	{
	public:
		GameServiceContainer();
		~GameServiceContainer();

		void AddService(std::string csTypeName, void* object);
		IServiceBase* GetService(std::string csTypeName);
		void RemoveService(std::string csTypeName);

	private:
		std::map<std::string, void*> m_services;
	};

}
