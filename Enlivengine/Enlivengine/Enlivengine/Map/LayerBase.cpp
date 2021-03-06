#include <Enlivengine/Map/LayerBase.hpp>

#include <Enlivengine/Map/Map.hpp>

namespace en
{
namespace tmx
{

LayerBase::LayerBase(Map& map)
	: mMap(map)
	, mID(0)
	, mName("")
	, mOffset(0.0f, 0.0f)
	, mOpacity(1.0f)
	, mVisible(true)
	, mLocked(false)
{
}

Map& LayerBase::GetMap()
{
	return mMap;
}

const Map& LayerBase::GetMap() const
{
	return mMap;
}

U32 LayerBase::GetID() const
{
	return mID;
}

const std::string& LayerBase::GetName() const
{
	return mName;
}

const Vector2f& LayerBase::GetOffset() const
{
	return mOffset;
}

F32 LayerBase::GetOpacity() const
{
	return mOpacity;
}

bool LayerBase::IsVisible() const
{
	return mVisible;
}

bool LayerBase::IsLocked() const
{
	return mLocked;
}

void LayerBase::Render(sf::RenderTarget& target) const
{
    ENLIVE_UNUSED(target);
}

bool LayerBase::Parse(ParserXml& parser)
{
	parser.getAttribute("id", mID);

	parser.getAttribute("name", mName);

	parser.getAttribute("offsetx", mOffset.x);
	parser.getAttribute("offsety", mOffset.y);

	parser.getAttribute("opacity", mOpacity);

	I32 visibleInt = 1;
	parser.getAttribute("visible", visibleInt);
	mVisible = static_cast<bool>(visibleInt);

	I32 lockedInt = 0;
	parser.getAttribute("locked", lockedInt);
	mLocked = static_cast<bool>(lockedInt);

	return true;
}

} // namespace tmx
} // namespace en