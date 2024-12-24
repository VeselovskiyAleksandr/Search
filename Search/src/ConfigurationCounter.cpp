#include "ConfigurationCounter.h"

template <class T>
struct ConfigurationCounter
{
	ConfigurationCounter()
	{
		++Counter;
	}
	static int getConfigurationCounter()
	{
		return Counter;
	}

    private:
	static int Counter;
};