#pragma once

#include "TrioApiDecl.h"
#include <vector>
#include <string>

namespace TrioEngine
{
	class Object
	{
	public:
		Object();

		virtual ~Object()
		{
		}

		const std::string& GetName() const
		{
			return m_name;
		}
		
		void SetName(const std::string& name)
		{
			m_name = name;
		}

		int GetId() const
		{
			return m_id;
		}

	private:
		std::string m_name;
		int m_id;

		static int g_id;
	};
}
