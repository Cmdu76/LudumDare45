#include <Enlivengine/Map/TileLayer.hpp>

#include <Enlivengine/System/Compression.hpp>
#include <Enlivengine/System/String.hpp>

#include <sstream>

namespace en
{
namespace tmx
{

TileLayer::TileLayer(Map& map)
	: LayerBase(map)
	, mSize(0, 0)
	, mEncoding(EncodingType::Base64)
	, mCompression(CompressionType::Zlib)
	, mTiles()
	, mVertexArrays()
{
}

const Vector2u& TileLayer::GetSize() const
{
	return mSize;
}

TileLayer::EncodingType TileLayer::GetEncoding() const
{
	return mEncoding;
}

TileLayer::CompressionType TileLayer::GetCompression() const
{
	return mCompression;
}

void TileLayer::SetTile(const Vector2u& tileCoords, U32 tileID)
{
	assert(tileCoords.x < mSize.x);
	assert(tileCoords.y < mSize.y);
	const U32 index = tileCoords.y * mSize.x + tileCoords.x;
	assert(index < static_cast<U32>(mTiles.size()));

	const U32 vertexIndex = (tileCoords.x + tileCoords.y * mSize.x) * 4;

	const U32 previousTileID = mTiles[index];
	const U32 previousTilesetIndex = mMap.GetTilesetIndexFromGID(previousTileID);
	const U32 tilesetIndex = mMap.GetTilesetIndexFromGID(tileID);
	
	// Reset previous tile if needed
	if ((tilesetIndex != previousTilesetIndex || tileID == 0) && previousTileID > 0 && previousTilesetIndex < mMap.GetTilesetCount())
	{
		sf::Vertex* quad = &mVertexArrays[previousTilesetIndex][vertexIndex];
		quad[0].texCoords = sf::Vector2f(0.0f, 0.0f);
		quad[1].texCoords = sf::Vector2f(0.0f, 0.0f);
		quad[2].texCoords = sf::Vector2f(0.0f, 0.0f);
		quad[3].texCoords = sf::Vector2f(0.0f, 0.0f);
	}

	// Update new tile if needed
	mTiles[index] = tileID;
	if (tileID > 0 && tilesetIndex < mMap.GetTilesetCount())
	{
		TilesetPtr tilesetPtr = mMap.GetTileset(tilesetIndex);
		if (tilesetPtr.IsValid())
		{
			const Tileset& tileset = tilesetPtr.Get();

			const Vector2u& tilesetTileSize = tileset.GetTileSize();
			const Vector2f pos(tileset.ToPos(tileID));
			const Vector2f texSize(static_cast<F32>(tilesetTileSize.x), static_cast<F32>(tilesetTileSize.y));

			sf::Vertex* quad = &mVertexArrays[tilesetIndex][vertexIndex];
			quad[0].texCoords = sf::Vector2f(pos.x, pos.y);
			quad[1].texCoords = sf::Vector2f(pos.x + texSize.x, pos.y);
			quad[2].texCoords = sf::Vector2f(pos.x + texSize.x, pos.y + texSize.y);
			quad[3].texCoords = sf::Vector2f(pos.x, pos.y + texSize.y);
		}
	}
}

U32 TileLayer::GetTile(const Vector2u& tileCoords) const
{
	assert(tileCoords.x < mSize.x);
	assert(tileCoords.y < mSize.y);
	const U32 index = tileCoords.y * mSize.x + tileCoords.x;
	assert(index < static_cast<U32>(mTiles.size()));
	return mTiles[index];
}

bool TileLayer::Parse(ParserXml& parser)
{
	if (!LayerBase::Parse(parser))
	{
		return false;
	}

	parser.getAttribute("width", mSize.x);
	parser.getAttribute("height", mSize.y);

	Update();

	if (parser.readNode("data"))
	{
		std::string attribStr = "base64";
		parser.getAttribute("encoding", attribStr);
		if (attribStr == "base64")
		{
			mEncoding = EncodingType::Base64;
		}
		else if (attribStr == "csv")
		{
			mEncoding = EncodingType::CSV;
		}
		else if (attribStr == "xml")
		{
			mEncoding = EncodingType::XML;
		}
		else
		{
			LogError(en::LogChannel::Graphics, 7, "Unknown encoding %s", attribStr.c_str());
			return false;
		}

		attribStr = "";
		parser.getAttribute("compression", attribStr);
		if (attribStr == "zlib")
		{
			mCompression = CompressionType::Zlib;
		}
		else if (attribStr == "gzip")
		{
			mCompression = CompressionType::Gzip;
		}
		else if (attribStr == "")
		{
			mCompression = CompressionType::None;
		}
		else
		{
			LogError(en::LogChannel::Graphics, 7, "Unknown compression %s", attribStr.c_str());
			return false;
		}

		if (mEncoding == EncodingType::Base64)
		{
			ParseBase64(parser);
		}
		else if (mEncoding == EncodingType::CSV)
		{
			ParseCsv(parser);
		}
		else if (mEncoding == EncodingType::XML)
		{
			ParseXml(parser);
		}
		else
		{
			assert(false);
		}

		parser.closeNode();
	}

	if (parser.readNode("properties"))
	{
		PropertyHolder::Parse(parser);
		parser.closeNode();
	}

	return true;
}

bool TileLayer::ParseBase64(ParserXml& parser)
{
	std::string data;
	parser.getValue(data);
	trim(data);

	if (!Compression::Decode64(data))
	{
		LogError(en::LogChannel::Graphics, 8, "Can't decode Base64");
		return false;
	}

	bool decompression = false;
	switch (mCompression)
	{
	case CompressionType::Zlib: decompression = Compression::Decompress(data); break;
	case CompressionType::Gzip: decompression = false; break; // TODO : Gzip decompression
	case CompressionType::None: decompression = true; break;
	default: decompression = false; break;
	}
	if (!decompression)
	{
		LogError(en::LogChannel::Graphics, 8, "Can't decompress %d", mCompression);
		return false;
	}

	std::vector<U8> byteVector;
	const U32 byteVectorSize = mSize.x * mSize.y * 4;
	byteVector.reserve(byteVectorSize);
	for (std::string::iterator i = data.begin(); i != data.end(); ++i)
	{
		byteVector.push_back(*i);
	}

	Vector2u coords(0, 0);
	for (std::size_t i = 0; i < byteVector.size() - 3; i += 4)
	{
		U32 gid = byteVector[i] | byteVector[i + 1] << 8 | byteVector[i + 2] << 16 | byteVector[i + 3] << 24;
		// TODO : Read Flip Flag
		SetTile(coords, gid);
		coords.x = (coords.x + 1) % mSize.x;
		if (coords.x == 0)
		{
			coords.y++;
		}
	}

	return true;
}

bool TileLayer::ParseCsv(ParserXml& parser)
{
	std::string data;
	parser.getValue(data);

	std::stringstream ss(data);

	U32 gid;
	Vector2u coords(0, 0);
	while (ss >> gid)
	{
		if (ss.peek() == ',')
		{
			ss.ignore();
		}
		// TODO : Read Flip Flag
		SetTile(coords, gid);
		coords.x = (coords.x + 1) % mSize.x;
		if (coords.x == 0)
		{
			coords.y++;
		}
	}

	return true;
}

bool TileLayer::ParseXml(ParserXml& parser)
{
	if (parser.readNode("tile"))
	{
		Vector2u coords(0, 0);
		do
		{
			U32 gid = 0;
			parser.getAttribute("gid", gid);
			// TODO : Read Flip Flag
			SetTile(coords, gid);
			coords.x = (coords.x + 1) % mSize.x;
			if (coords.x == 0)
			{
				coords.y++;
			}

		} while (parser.nextSibling("tile"));
		parser.closeNode();

		return true;
	}
	else
	{
		return false;
	}
}

void TileLayer::Update()
{
	const U32 tileCount = mSize.x * mSize.y;
	mTiles.resize(tileCount, 0);
	mVertexArrays.resize(mMap.GetTilesetCount());
	for (sf::VertexArray& vertexArray : mVertexArrays)
	{
		const U32 vertexCount = tileCount * 4;
		vertexArray.setPrimitiveType(sf::Quads);
		vertexArray.resize(vertexCount);
	}

	const sf::Color color = sf::Color(255, 255, 255);
	const Vector2u& mapTileSize = mMap.GetTileSize();
	const U32 vertexArrayCount = static_cast<U32>(mVertexArrays.size());
	for (U32 vertexArrayIndex = 0; vertexArrayIndex < vertexArrayCount; ++vertexArrayIndex)
	{
		TilesetPtr tilesetPtr = mMap.GetTileset(vertexArrayIndex);
		const Vector2u& tilesetTileSize = (tilesetPtr.IsValid()) ? tilesetPtr.Get().GetTileSize() : mapTileSize;
		const F32 deltaY = static_cast<F32>(tilesetTileSize.y - mapTileSize.y);

		Vector2u tileCoords;
		for (tileCoords.x = 0; tileCoords.x < mSize.x; ++tileCoords.x)
		{
			for (tileCoords.y = 0; tileCoords.y < mSize.y; ++tileCoords.y)
			{
				const U32 vertexIndex = (tileCoords.x + tileCoords.y * mSize.x) * 4;
				const Vector2f pos = mMap.CoordsToWorld(tileCoords);

				sf::Vertex* quad = &mVertexArrays[vertexArrayIndex][vertexIndex];
				quad[0].position = sf::Vector2f(pos.x, pos.y - deltaY);
				quad[1].position = sf::Vector2f(pos.x + static_cast<F32>(mapTileSize.x), pos.y - deltaY);
				quad[2].position = sf::Vector2f(pos.x + static_cast<F32>(mapTileSize.x), pos.y + static_cast<F32>(mapTileSize.y));
				quad[3].position = sf::Vector2f(pos.x, pos.y + static_cast<F32>(mapTileSize.y));
				for (U32 i = 0; i < 4; ++i)
				{
					quad[i].color = color;
				}
			}
		}
	}
}

} // namespace tmx
} // namespace en