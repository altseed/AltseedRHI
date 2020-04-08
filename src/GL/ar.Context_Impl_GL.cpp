
#include "ar.Context_Impl_GL.h"

#include "ar.Manager_Impl_GL.h"
#include "ar.VertexBuffer_Impl_GL.h"
#include "ar.IndexBuffer_Impl_GL.h"
#include "ar.Shader_Impl_GL.h"

#include "ar.Texture2D_Impl_GL.h"
#include "ar.CubemapTexture_Impl_GL.h"
#include "ar.RenderTexture2D_Impl_GL.h"
#include "ar.DepthTexture_Impl_GL.h"

#include "ar.ConstantBuffer_Impl_GL.h"

namespace ar
{
	void Context_Impl_GL::CreateRenderStates(Manager* manager)
	{

	}

	void Context_Impl_GL::UpdateRenderStates(const DrawParameter& param, bool forced)
	{
		GLCheckError();

		auto& current = previousDrawParam;
		auto& next = param;

		if (current.IsDepthTest != next.IsDepthTest || forced)
		{
			if (next.IsDepthTest)
			{
				glEnable(GL_DEPTH_TEST);
				glDepthFunc(GL_LEQUAL);
			}
			else
			{
				glDisable(GL_DEPTH_TEST);
			}
		}

		if (current.IsDepthWrite != next.IsDepthWrite || forced)
		{
			glDepthMask(next.IsDepthWrite);
		}

		if (current.Culling != next.Culling || forced)
		{
			if (next.Culling == CullingType::Front)
			{
				glEnable(GL_CULL_FACE);
				glCullFace(GL_FRONT);
			}
			else if (next.Culling == CullingType::Back)
			{
				glEnable(GL_CULL_FACE);
				glCullFace(GL_BACK);
			}
			else if (next.Culling == CullingType::Double)
			{
				glDisable(GL_CULL_FACE);
				glCullFace(GL_FRONT_AND_BACK);
			}
		}

		if (current.AlphaBlend != next.AlphaBlend || forced)
		{
			if (next.AlphaBlend == AlphaBlendMode::Opacity)
			{
				glDisable(GL_BLEND);
			}
			else
			{
				glEnable(GL_BLEND);

				if (next.AlphaBlend == AlphaBlendMode::Sub)
				{
					//glBlendEquation(GL_FUNC_REVERSE_SUBTRACT);
					//glBlendFunc(GL_SRC_ALPHA, GL_ONE);
					glBlendEquationSeparate(GL_FUNC_REVERSE_SUBTRACT, GL_FUNC_ADD);
					glBlendFuncSeparate(GL_SRC_ALPHA, GL_ONE, GL_ONE, GL_ONE);
				}
				else
				{
					//glBlendEquation(GL_FUNC_ADD);
					glBlendEquationSeparate(GL_FUNC_ADD, GL_FUNC_ADD);
					if (next.AlphaBlend == AlphaBlendMode::Blend)
					{
						glBlendFuncSeparate(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, GL_ONE, GL_ONE);
					}
					else if (next.AlphaBlend == AlphaBlendMode::Add)
					{
						glBlendFuncSeparate(GL_SRC_ALPHA, GL_ONE, GL_ONE, GL_ONE);
					}
					else if (next.AlphaBlend == AlphaBlendMode::Mul)
					{
						glBlendFuncSeparate(GL_ZERO, GL_SRC_COLOR, GL_ONE, GL_ONE);
					}
					else if (next.AlphaBlend == AlphaBlendMode::AddAll)
					{
						glBlendFuncSeparate(GL_ONE, GL_ONE, GL_ONE, GL_ONE);
					}
					else if (next.AlphaBlend == AlphaBlendMode::OpacityAll)
					{
						glBlendFuncSeparate(GL_ONE, GL_ZERO, GL_ONE, GL_ZERO);
					}
				}
			}
		}

		previousDrawParam = param;
	}

	Context_Impl_GL::Context_Impl_GL()
	{

	}

	Context_Impl_GL::~Context_Impl_GL()
	{
		glDeleteSamplers(MaxTextureCount, samplers.data());
	}

	bool Context_Impl_GL::Initialize(Manager* manager)
	{
		samplers.fill(0);
		glGenSamplers(MaxTextureCount, samplers.data());

		this->manager = manager;

		return true;
	}

	void Context_Impl_GL::Begin()
	{
		UpdateRenderStates(DrawParameter(), true);
	}

	void Context_Impl_GL::End()
	{

	}


	void Context_Impl_GL::Draw(const DrawParameter& param)
	{
		GLCheckError();

		auto vb = (VertexBuffer_Impl_GL*)param.VertexBufferPtr;
		auto ib = (IndexBuffer_Impl_GL*)param.IndexBufferPtr;
		auto shader = (Shader_Impl_GL*)param.ShaderPtr;

		if (vb->GetBuffer() == 0) return;
		if (ib->GetBuffer() == 0) return;
		if (shader->GetShader() == 0) return;

		auto m = (Manager_Impl_GL*)manager;
		m->ApplyVAO();

		UpdateRenderStates(param, false);

		glBindBuffer(GL_ARRAY_BUFFER, vb->GetBuffer());
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ib->GetBuffer());

		glUseProgram(shader->GetShader());
		GLCheckError();

		const void* vertices = NULL;

		for (auto& layout : shader->GetLayouts())
		{
			if (layout.attribute < 0) continue;

			glEnableVertexAttribArray(layout.attribute);
			glVertexAttribPointer(layout.attribute, layout.count, layout.type, layout.normalized, vb->GetVertexSize(), (uint8_t*)vertices + layout.offset);
		}
		GLCheckError();

		// set textures
		for (auto& l : shader->GetPixelTextureLayouts())
		{
			glUniform1i(l.second.ID, l.second.Index);
		}

		for (int32_t i = 0; i < shader->GetPixelTextureCount(); i++)
		{
			if (param.PixelShaderTextures[i] == nullptr)
			{
				continue;
			}

			static const GLint glfilter[] = { GL_NEAREST, GL_LINEAR };
			static const GLint glfilter_mip[] = { GL_NEAREST_MIPMAP_LINEAR, GL_LINEAR_MIPMAP_LINEAR };
			static const GLint glwrap[] = { GL_REPEAT, GL_CLAMP_TO_EDGE };

			glActiveTexture(GL_TEXTURE0 + i);

			GLint textureID = 0;

			if (param.PixelShaderTextures[i]->GetType() == TextureType::Texture2D)
			{
				auto texture = (Texture2D_Impl_GL*)param.PixelShaderTextures[i];
				textureID = texture->GetTexture();
			}
			else if (param.PixelShaderTextures[i]->GetType() == TextureType::RenderTexture2D)
			{
				auto texture = (RenderTexture2D_Impl_GL*)param.PixelShaderTextures[i];
				textureID = texture->GetTexture();
			}
			else if (param.PixelShaderTextures[i]->GetType() == TextureType::CubemapTexture)
			{
				auto texture = (CubemapTexture_Impl_GL*)param.PixelShaderTextures[i];
				textureID = texture->GetTexture();
			}
			else if (param.PixelShaderTextures[i]->GetType() == TextureType::DepthTexture)
			{
				auto texture = (DepthTexture_Impl_GL*)param.PixelShaderTextures[i];
				textureID = texture->GetTexture();
			}

			glBindTexture(GL_TEXTURE_2D, textureID);
			
			// Sampler
			glBindSampler(i, samplers[i]);

			int32_t filter_ = (int32_t)param.PixelShaderTextureFilers[i];
			
			if (param.PixelShaderTextures[i]->GetType() == TextureType::CubemapTexture)
			{
				glSamplerParameteri(samplers[i], GL_TEXTURE_MAG_FILTER, glfilter[filter_]);
				glSamplerParameteri(samplers[i], GL_TEXTURE_MIN_FILTER, glfilter_mip[filter_]);
			}
			else
			{
				glSamplerParameteri(samplers[i], GL_TEXTURE_MAG_FILTER, glfilter[filter_]);
				glSamplerParameteri(samplers[i], GL_TEXTURE_MIN_FILTER, glfilter[filter_]);
			}

			int32_t wrap_ = (int32_t)param.PixelShaderTextureWraps[i];
			glSamplerParameteri(samplers[i], GL_TEXTURE_WRAP_S, glwrap[wrap_]);
			glSamplerParameteri(samplers[i], GL_TEXTURE_WRAP_T, glwrap[wrap_]);
		}
		GLCheckError();

		glActiveTexture(GL_TEXTURE0);

		// constant buffers
		if (param.PixelConstantBufferPtr != nullptr)
		{
			auto cb = (ConstantBuffer_Impl_GL*)param.PixelConstantBufferPtr;
			auto buf = cb->GetBuffer();

			for (auto& l_ : shader->GetPixelConstantLayouts())
			{
				auto& l = l_.second;

				if (l.Type == ConstantBufferFormat::Matrix44)
				{
					uint8_t* data = (uint8_t*)buf;
					data += l.Offset;
					glUniformMatrix4fv(
						l.Index,
						1,
						GL_TRUE,
						(const GLfloat*)data);
				}
				else if (l.Type == ConstantBufferFormat::Matrix44_ARRAY)
				{
					uint8_t* data = (uint8_t*)buf;
					data += l.Offset;
					glUniformMatrix4fv(
						l.Index,
						l.Count,
						GL_TRUE,
						(const GLfloat*)data);
				}
				else if (l.Type == ConstantBufferFormat::Float4)
				{
					uint8_t* data = (uint8_t*)buf;
					data += l.Offset;
					glUniform4fv(
						l.Index,
						1,
						(const GLfloat*)data);
				}
				else if (l.Type == ConstantBufferFormat::Float4_ARRAY)
				{
					uint8_t* data = (uint8_t*)buf;
					data += l.Offset;
					glUniform4fv(
						l.Index,
						l.Count,
						(const GLfloat*)data);
				}
				else if (l.Type == ConstantBufferFormat::Float1)
				{
					uint8_t* data = (uint8_t*)buf;
					data += l.Offset;
					glUniform1fv(
						l.Index,
						1,
						(const GLfloat*)data);
				}
				else if (l.Type == ConstantBufferFormat::Float2)
				{
					uint8_t* data = (uint8_t*)buf;
					data += l.Offset;
					glUniform2fv(
						l.Index,
						1,
						(const GLfloat*)data);
				}
				else if (l.Type == ConstantBufferFormat::Float3)
				{
					uint8_t* data = (uint8_t*)buf;
					data += l.Offset;
					glUniform3fv(
						l.Index,
						1,
						(const GLfloat*)data);
				}
			}
		}
		GLCheckError();

		int32_t indexType = ib->GetIs32Bit() ? GL_UNSIGNED_INT : GL_UNSIGNED_SHORT;
		int32_t isize = ib->GetIs32Bit() ? 4 : 2;

		int32_t topology = GL_TRIANGLES;
		if (param.Topology == TopologyMode::Triangles)
		{
			topology = GL_TRIANGLES;
		}
		if (param.Topology == TopologyMode::Lines)
		{
			topology = GL_LINES;
		}

		if (param.InstanceCount == 1)
		{
			glDrawElements(
				topology,
				param.IndexCount == 0 ? ib->GetIndexCount() : param.IndexCount,
				indexType, 
				(void*)(param.IndexOffset * isize));
		}
		else
		{
			glDrawElementsInstanced(
				topology,
				param.IndexCount == 0 ? ib->GetIndexCount() : param.IndexCount,
				indexType,
				(void*)(param.IndexOffset * isize),
				param.InstanceCount);
		}
		GLCheckError();

		for (auto& layout : shader->GetLayouts())
		{
			if (layout.attribute >= 0)
			{
				glDisableVertexAttribArray(layout.attribute);
			}
		}
		GLCheckError();

		glUseProgram(0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);

		GLCheckError();
	}
}
