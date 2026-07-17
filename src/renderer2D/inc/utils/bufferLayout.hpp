#pragma once

#include <vector>
#include "renderer2d/inc/utils/gl_commons.hpp"

namespace R2D
{
	struct BufferAttribute
	{
		uint32_t       count;
		uint32_t       stride;
		ATTRIBUTE_TYPE type;
		bool           normalised;
	};

	struct BufferLayout
	{
		std::vector<BufferAttribute> attributes{ 0 };
		uint32_t                     byteStride{ 0 };
	};

	struct AttributeArg
	{
		ATTRIBUTE_TYPE type;
		uint32_t       count;
		bool           normalised{ false };
	};

	struct BufferLayoutBuilder
	{
		static BufferLayout buildLayout(std::initializer_list<AttributeArg> attributesIn)
		{
			std::vector<BufferAttribute> attributes;
			attributes.reserve(attributesIn.size());

			uint32_t totalStride = 0;

			for (auto const& attr : attributesIn)
			{
				uint32_t stride{ 0 };
				switch (attr.type)
				{
				case ATTRIBUTE_TYPE::BYTE:
				case ATTRIBUTE_TYPE::UBYTE:
					stride = sizeof(char);
					break;
				case ATTRIBUTE_TYPE::SHORT:
				case ATTRIBUTE_TYPE::USHORT:
					stride = sizeof(short);
					break;
				case ATTRIBUTE_TYPE::INT:
				case ATTRIBUTE_TYPE::UINT:
					stride = sizeof(int);
					break;
				case ATTRIBUTE_TYPE::FLOAT:
					stride = sizeof(float);
					break;
				}

				stride *= attr.count;

				attributes.push_back({ attr.count, stride, attr.type, attr.normalised });
				totalStride += stride;
			}


			return BufferLayout{ attributes, totalStride };
		}

		static BufferLayout concatLayouts(std::initializer_list<BufferLayout const*> layoutsIn)
		{
			std::vector<BufferAttribute> attributes;

			uint32_t totalStride = 0;

			for (auto const& layout : layoutsIn)
			{
				attributes.insert(attributes.end(), layout->attributes.begin(), layout->attributes.end());
				totalStride += layout->byteStride;
			}

			return BufferLayout{ attributes, totalStride };
		}
	};

	// Common layouts
	namespace LAYOUTS
	{
		static const BufferLayout VEC2LAYOUT
		{
			{
				{2, sizeof(float) * 2, ATTRIBUTE_TYPE::FLOAT, false}
			},
			sizeof(float) * 2
		};

		static const BufferLayout VEC3LAYOUT
		{
			{
				{3, sizeof(float) * 3, ATTRIBUTE_TYPE::FLOAT, false}
			},
			sizeof(float) * 3
		};

		static const BufferLayout VEC4LAYOUT
		{
			{
				{4, sizeof(float) * 4, ATTRIBUTE_TYPE::FLOAT, false}
			},
			sizeof(float) * 4
		};

		static const BufferLayout MAT3LAYOUT
		{
			{
				{3, sizeof(float) * 3, ATTRIBUTE_TYPE::FLOAT, false},
				{3, sizeof(float) * 3, ATTRIBUTE_TYPE::FLOAT, false},
				{3, sizeof(float) * 3, ATTRIBUTE_TYPE::FLOAT, false}
			},
			sizeof(float) * 9
		};

		static const BufferLayout MAT4LAYOUT
		{
			{
				{4, sizeof(float) * 4, ATTRIBUTE_TYPE::FLOAT, false},
				{4, sizeof(float) * 4, ATTRIBUTE_TYPE::FLOAT, false},
				{4, sizeof(float) * 4, ATTRIBUTE_TYPE::FLOAT, false},
				{4, sizeof(float) * 4, ATTRIBUTE_TYPE::FLOAT, false}
			},
			sizeof(float) * 16
		};

		static const BufferLayout UINTLAYOUT
		{
			{
				{1, sizeof(uint32_t), ATTRIBUTE_TYPE::UINT, false}
			},
			sizeof(uint32_t)
		};

		static const BufferLayout FLOATLAYOUT
		{
			{
				{1, sizeof(float), ATTRIBUTE_TYPE::FLOAT, false}
			},
			sizeof(float)
		};

		static const BufferLayout INTLAYOUT
		{
			{
				{1, sizeof(uint32_t), ATTRIBUTE_TYPE::INT, false}
			},
			sizeof(uint32_t)
		};

		static const BufferLayout IVEC2LAYOUT
		{
			{
				{2, sizeof(uint32_t) * 2, ATTRIBUTE_TYPE::INT, false}
			},
			sizeof(uint32_t) * 2
		};

		static const BufferLayout IVEC3LAYOUT
		{
			{
				{3, sizeof(uint32_t) * 3, ATTRIBUTE_TYPE::INT, false}
			},
			sizeof(uint32_t) * 3
		};
	}
}
