#include <GH3/CRC32.hpp>
#include <string>

uint32_t gh3::crc32::CRC32TableLength = 256;
uint32_t* gh3::crc32::CRC32Table = reinterpret_cast<uint32_t*>(0x0095CD28);

gh3::GH3Key gh3::crc32::CreateKeyFromStringOut(GH3Key* outKey, const char* srcString)
{
	if (srcString == nullptr)
	{
		if (outKey != nullptr)
			*outKey = 0;
		return 0;
	}

	GH3Key finalKey = 0xFFFFFFFF;
	char currChar = *srcString;

	auto string = std::string_view(srcString);

	uint32_t lastChar = 0;

	for (uint32_t i = 0; i < string.length(); i++)
	{
		char c = string[i];

		if ((c + 0xBFU) < 0x1A)
			c += 32;

		lastChar = c;

		if (c == '/')
			lastChar = '\\';

		finalKey = finalKey >> 8 ^ CRC32Table[(lastChar ^ finalKey) & 0xFF];
	}

	if (outKey != nullptr)
		*outKey = finalKey;

	return finalKey;
}

gh3::GH3Key gh3::crc32::CreateKeyFromString(const char* srcString)
{
	return CreateKeyFromStringOut(nullptr, srcString);
}


void gh3::crc32::CreateKeyNameAssociation(GH3Key key, char* name)
{
	reinterpret_cast<void(__cdecl*)(GH3Key, char*)>(key, name);
}