#include "Config.h"


namespace Config
{
	Boolean EnableSleepWait[8] = {
		{ "InAir" },
		{ "Trespassing" },
		{ "AskedToLeave" },
		{ "GuardsPursuing" },
		{ "EnemiesNearby" },
		{ "TakingHealthDamage" },
		{ "Owned" },
		{ "InUse" },
	};


	void Load()
	{
		auto main = COMPILE_PROXY("YouCanSleepRemake.toml"sv);

		for (auto index = 0; index < std::extent_v<decltype(EnableSleepWait)>; ++index) {
			main.Bind(EnableSleepWait[index], true);
		}

		main.Load();

		INFO("Config Loaded"sv);
	}
}

// SKSE-Hotfixes: Explicit template instantiations for DKUtil AData (declared extern template in DKUtil/Impl/Config/Data.hpp)
template class DKUtil::Config::detail::AData<bool>;
template class DKUtil::Config::detail::AData<std::int64_t>;
template class DKUtil::Config::detail::AData<double>;
template class DKUtil::Config::detail::AData<std::basic_string<char>>;