#pragma once

#include <Enlivengine/System/PrimitiveTypes.hpp>
#include <Enlivengine/System/Hash.hpp>
#include <Enlivengine/System/ParserXml.hpp>
#include <Enlivengine/Graphics/Color.hpp>

#include <unordered_map>

namespace en
{
namespace tmx
{

class PropertyHolder
{
protected:
	PropertyHolder() = default;

	bool Parse(ParserXml& parser);

public:
	enum class PropertyType
	{
		Boolean,
		Color,
		File,
		Float,
		Int,
		String
	};

	bool HasProperty(const std::string& name) const;
	PropertyType GetPropertyType(const std::string& name) const;

	bool GetBooleanProperty(const std::string& name) const;
	const Color& GetColorProperty(const std::string& name) const;
	F32 GetFloatProperty(const std::string& name) const;
	const std::string& GetFileProperty(const std::string& name) const;
	I32 GetIntProperty(const std::string& name) const;
	const std::string& GetStringProperty(const std::string& name) const;

private:
	std::unordered_map<U32, PropertyType> mTypeMap;

	std::unordered_map<U32, bool> mBooleans;
	std::unordered_map<U32, Color> mColors;
	std::unordered_map<U32, F32> mFloats;
	std::unordered_map<U32, std::string> mFiles;
	std::unordered_map<U32, I32> mInts;
	std::unordered_map<U32, std::string> mStrings;
};

} // namespace tmx
} // namespace en