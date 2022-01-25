#include "Config.h"

using namespace Config;

bool Config::Size::operator==(const Size& other) const
{
	return this->width == other.width &&
		this->height == other.height;
}

bool Config::Size::operator!=(const Size& other) const
{
	return this->width != other.width ||
		this->height != other.height;
}
