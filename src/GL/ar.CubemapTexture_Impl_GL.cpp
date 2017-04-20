#include "ar.CubemapTexture_Impl_GL.h"

#include "ar.Manager_Impl_GL.h"

#include "../ar.ImageHelper.h"

#include "../3rdParty/nv_dds/nv_dds.h"

#include <sstream>
#include <iostream>

namespace ar
{

	CubemapTexture_Impl_GL::CubemapTexture_Impl_GL()
	{

	}

	CubemapTexture_Impl_GL::~CubemapTexture_Impl_GL()
	{
		glDeleteTextures(1, &texture);
	}

	bool CubemapTexture_Impl_GL::Initialize(Manager* manager, void* data, int32_t size)
	{
		if (!ImageHelper::IsDDS(data, size)) return false;

		std::vector<uint8_t> temp;
		temp.resize(size);
		memcpy(temp.data(), data, size);

		nv_dds::CDDSImage image;
		std::istringstream stream(std::string(temp.begin(), temp.end()));
		image.load(stream);

		auto m = (Manager_Impl_GL*)manager;

		glGenTextures(1, &texture);
		glBindTexture(GL_TEXTURE_CUBE_MAP, texture);

		for (int n = 0; n < 6; n++)
		{
		    auto target = GL_TEXTURE_CUBE_MAP_POSITIVE_X+n;
			auto& face = image.get_cubemap_face(n);

		    glTexImage2D(target, 0, image.get_components(), face.get_width(),
		        face.get_height(), 0, image.get_format(), GL_UNSIGNED_BYTE,
		        face);
		
			for (int i = 0; i < face.get_num_mipmaps(); i++)
		    {
		        glTexImage2D(target, i+1, image.get_components(),
		            face.get_mipmap(i).get_width(),
		            face.get_mipmap(i).get_height(), 0,
					image.get_format(), GL_UNSIGNED_BYTE, face.get_mipmap(i));
		    }
		}


		glBindTexture(GL_TEXTURE_CUBE_MAP, 0);

		// TODO : correct
		this->format = TextureFormat::R8G8B8A8_UNORM;
		
		this->width = image.get_width();
		this->height = image.get_height();
		this->mipmapCount = image.get_num_mipmaps();
		return false;
	}
}