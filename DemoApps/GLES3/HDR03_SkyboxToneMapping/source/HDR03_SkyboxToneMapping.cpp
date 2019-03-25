/****************************************************************************************************************************************************
 * Copyright 2018 NXP
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 *    * Redistributions of source code must retain the above copyright notice,
 *      this list of conditions and the following disclaimer.
 *
 *    * Redistributions in binary form must reproduce the above copyright notice,
 *      this list of conditions and the following disclaimer in the documentation
 *      and/or other materials provided with the distribution.
 *
 *    * Neither the name of the NXP. nor the names of
 *      its contributors may be used to endorse or promote products derived from
 *      this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
 * IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT,
 * INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
 * BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
 * LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE
 * OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
 * ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 ****************************************************************************************************************************************************/

#include "HDR03_SkyboxToneMapping.hpp"
#include <FslBase/Bits/BitsUtil.hpp>
#include <FslBase/Log/Log.hpp>
#include <FslBase/Math/MathHelper.hpp>
#include <FslDemoService/Graphics/IGraphicsService.hpp>
#include <FslGraphics/Vertices/VertexPositionNormalTexture.hpp>
#include <FslGraphics/Vertices/VertexPositionTexture.hpp>
#include <FslSimpleUI/Base/Control/Background9Slice.hpp>
#include <FslSimpleUI/Base/Layout/StackLayout.hpp>
#include <FslUtil/OpenGLES3/Exceptions.hpp>
#include <FslUtil/OpenGLES3/GLCheck.hpp>
#include <Shared/CubeMapping/API/OpenGLES3/TextureUtil.hpp>
#include <GLES3/gl3.h>
#include <array>

namespace Fsl
{
  using namespace GLES3;
  using namespace UI;

  namespace
  {
    const Vector3 DEFAULT_CAMERA_POSITION(0.0f, 0.0f, 0.0f);
    const Vector3 DEFAULT_CAMERA_TARGET(-4.0f, 0.0f, 0.0f);


    const char* GetTonemapperShaderName(const Tonemapper::Enum tonemapper)
    {
      switch (tonemapper)
      {
      case Tonemapper::LinearNoGamma:
        return "TonemapperLinearNoGamma.frag";
      case Tonemapper::Linear:
        return "TonemapperLinear.frag";
      case Tonemapper::ReinhardLum:
        return "TonemapperReinhardLum.frag";
      case Tonemapper::Uncharted2Lum:
        return "TonemapperUncharted2Lum.frag";
      case Tonemapper::Custom:
        return "TonemapperCustom.frag";
      default:
        throw NotSupportedException("Unsupported tonemapper");
      }
    }


    GLES3::GLFrameBuffer CreateHdrFrameBuffer(const Point2& resolution)
    {
      GLTextureParameters params(GL_LINEAR, GL_LINEAR, GL_REPEAT, GL_REPEAT);
      GLTextureImageParameters texImageParams(GL_RGBA16F, GL_RGBA, GL_HALF_FLOAT);
      return GLFrameBuffer(resolution, params, texImageParams, GL_DEPTH_COMPONENT16);
    }
  }


  HDR03_SkyboxToneMapping::HDR03_SkyboxToneMapping(const DemoAppConfig& config)
    : DemoAppGLES3(config)
    , m_menuUI(config, Tonemapper::COUNT)
    , m_keyboard(config.DemoServiceProvider.Get<IKeyboard>())
    , m_mouse(config.DemoServiceProvider.Get<IMouse>())
    , m_demoAppControl(config.DemoServiceProvider.Get<IDemoAppControl>())
    , m_mouseCaptureEnabled(false)
    , m_rotationSpeed(5.4f, 0.0f)
  {
    RegisterExtension(m_menuUI.GetUIDemoAppExtension());

    const auto options = config.GetOptions<OptionParser>();

    m_camera.SetPosition(DEFAULT_CAMERA_POSITION, DEFAULT_CAMERA_TARGET, Vector3::Up());

    const auto contentManager = GetContentManager();

    PrepareScene(contentManager, m_resources.MainScene);
    CreateTonemappers(m_resources.ProgramTonemap, contentManager);

    m_resources.HdrFrameBuffer = CreateHdrFrameBuffer(GetScreenResolution());
    m_resources.MeshQuad = CreateQuadVertexArray(m_resources.ProgramTonemap.front().Program);
  }


  HDR03_SkyboxToneMapping::~HDR03_SkyboxToneMapping() = default;


  void HDR03_SkyboxToneMapping::OnKeyEvent(const KeyEvent& event)
  {
    m_menuUI.OnKeyEvent(event);
  }


  void HDR03_SkyboxToneMapping::OnMouseButtonEvent(const MouseButtonEvent& event)
  {
    if (event.IsHandled())
    {
      return;
    }

    switch (event.GetButton())
    {
    case VirtualMouseButton::Right:
    {
      m_rightMouseDown = event.IsPressed();
      if (m_demoAppControl->TryEnableMouseCaptureMode(m_rightMouseDown))
      {
        m_mouseCaptureEnabled = m_rightMouseDown;
      }
      else
      {
        m_mouseCaptureEnabled = false;
      }
      event.Handled();
      break;
    }
    case VirtualMouseButton::Middle:
      if (event.IsPressed())
      {
        m_camera.SetPosition(DEFAULT_CAMERA_POSITION, DEFAULT_CAMERA_TARGET, Vector3::Up());
        event.Handled();
      }
      break;
    default:
      break;
    }
  }


  void HDR03_SkyboxToneMapping::Update(const DemoTime& demoTime)
  {
    UpdateInput(demoTime);
    m_menuUI.Update(demoTime);

    const auto screenResolution = GetScreenResolution();
    m_vertexUboData.MatModel = Matrix::GetIdentity();
    m_vertexUboData.MatView = m_camera.GetViewMatrix();
    float aspect = static_cast<float>(screenResolution.X) / screenResolution.Y;    // ok since we divide both by two when we show four screens
    m_vertexUboData.MatProj = Matrix::CreatePerspectiveFieldOfView(MathHelper::ToRadians(45.0f), aspect, 0.1f, 100.0f);
  }


  void HDR03_SkyboxToneMapping::Draw(const DemoTime& demoTime)
  {
    const auto screenResolution = GetScreenResolution();

    glDisable(GL_BLEND);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);

    // Render into HDR framebuffer
    glBindFramebuffer(GL_FRAMEBUFFER, m_resources.HdrFrameBuffer.Get());

    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    DrawScene(m_resources.MainScene, m_vertexUboData);

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glEnable(GL_SCISSOR_TEST);

    auto rRenderRecords = m_menuUI.GetRenderRecords();
    assert(rRenderRecords.size() == m_resources.ProgramTonemap.size());
    GLint startX = 0;
    for (std::size_t i = 0; i < rRenderRecords.size(); ++i)
    {
      auto endX = static_cast<GLint>(rRenderRecords[i].SplitX.GetValue());
      glScissor(startX, 0, endX - startX, screenResolution.Y);
      startX = endX;
      DrawTonemappedScene(m_resources.ProgramTonemap[i], m_resources.HdrFrameBuffer);
    }

    glDisable(GL_SCISSOR_TEST);

    m_menuUI.Draw(demoTime);
  }


  void HDR03_SkyboxToneMapping::UpdateInput(const DemoTime& demoTime)
  {
    if (!m_rightMouseDown)
    {
      m_camera.RotateByRadians(m_rotationSpeed * demoTime.DeltaTime);
    }

    const auto keyboardState = m_keyboard->GetState();
    UpdateCameraControlInput(demoTime, keyboardState);
    m_menuUI.UpdateExposureInput(demoTime, keyboardState);
  }


  void HDR03_SkyboxToneMapping::UpdateCameraControlInput(const DemoTime& demoTime, const KeyboardState& keyboardState)
  {
    {    // Mouse camera rotation
      const auto mouseState = m_mouse->GetState();

      if (!m_rightMouseDown)
      {
        const bool rotateCamera = mouseState.IsRightButtonPressed();
        m_camera.RotateViaPosition(rotateCamera, mouseState.Position);
      }
      else
      {
        if (mouseState.IsRightButtonPressed())
        {
          const auto rawPosition = Vector2(mouseState.RawPosition.X, -mouseState.RawPosition.Y);
          m_camera.Rotate(rawPosition);
        }
      }
    }

    // Keyboard camera movement
    const float movementSpeed = 2.0f * demoTime.DeltaTime;
    if (keyboardState.IsKeyDown(VirtualKey::W))
    {
      m_camera.MoveForward(movementSpeed);
    }
    if (keyboardState.IsKeyDown(VirtualKey::S))
    {
      m_camera.MoveBackwards(movementSpeed);
    }
    if (keyboardState.IsKeyDown(VirtualKey::A))
    {
      m_camera.MoveLeft(movementSpeed);
    }
    if (keyboardState.IsKeyDown(VirtualKey::D))
    {
      m_camera.MoveRight(movementSpeed);
    }
  }


  void HDR03_SkyboxToneMapping::DrawScene(const Scene& scene, const VertexUBOData& vertexUboData)
  {
    const auto& program = scene.Program.Program;
    const auto& location = scene.Program.Location;
    const auto& mesh = scene.Mesh;
    const auto& texture = scene.CubemapTexture;

    // Set the shader program
    glUseProgram(program.Get());

    // Load the matrices
    assert(location.ViewMatrix != GLValues::INVALID_LOCATION);
    assert(location.ProjMatrix != GLValues::INVALID_LOCATION);
    assert(location.SkyboxSampler != GLValues::INVALID_LOCATION);

    glUniformMatrix4fv(location.ViewMatrix, 1, 0, vertexUboData.MatView.DirectAccess());
    glUniformMatrix4fv(location.ProjMatrix, 1, 0, vertexUboData.MatProj.DirectAccess());

    // Bind the vertex array
    mesh.VertexArray.Bind();

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_CUBE_MAP, texture.Get());
    glDrawArrays(GL_TRIANGLES, 0, mesh.VertexBuffer.GetCapacity());
    // glDrawArrays(GL_TRIANGLES, 0, 6);

    mesh.VertexArray.Unbind();

    glUseProgram(0);
  }


  void HDR03_SkyboxToneMapping::DrawTonemappedScene(const TonemapProgramInfo& programInfo, const GLTextureInfo& hdrFramebufferTexInfo)
  {
    const auto& program = programInfo.Program;
    const auto& location = programInfo.Location;

    // Set the shader program
    glUseProgram(program.Get());

    // The LDR shader dont use exposure
    if (location.Exposure != GLValues::INVALID_LOCATION)
    {
      glUniform1f(location.Exposure, m_menuUI.GetExposure());
    }

    // Bind the vertex array
    m_resources.MeshQuad.VertexArray.Bind();

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, hdrFramebufferTexInfo.Handle);
    glDrawArrays(GL_TRIANGLES, 0, m_resources.MeshQuad.VertexBuffer.GetCapacity());

    m_resources.MeshQuad.VertexArray.Unbind();
  }


  void HDR03_SkyboxToneMapping::PrepareScene(const std::shared_ptr<IContentManager>& contentManager, Scene& rScene)
  {
    FSLLOG("Preparing scene");
    FSLLOG("- loading cubemaps")
    GLTextureParameters3 texParams(GL_NEAREST, GL_NEAREST, GL_CLAMP_TO_EDGE, GL_CLAMP_TO_EDGE, GL_CLAMP_TO_EDGE);

    std::string texture = "test";
    // rScene.CubemapTexture = TextureUtil::CreateCubemapTextureFromSix(contentManager, "floral_tent/1024", PixelFormat::R16G16B16A16_SFLOAT);
    rScene.CubemapTexture =
      TextureUtil::CreateCubemapTextureFromSix(contentManager, "Textures/Cubemap/HDR_Lookout/1024", PixelFormat::R16G16B16A16_SFLOAT);

    FSLLOG("- loading shaders")
    rScene.Program.Reset(contentManager->ReadAllText("skybox.vert"), contentManager->ReadAllText("skybox.frag"));
    rScene.Mesh.Reset(rScene.Program.Program);
  }


  void HDR03_SkyboxToneMapping::CreateTonemappers(std::vector<TonemapProgramInfo>& rVector, const std::shared_ptr<IContentManager>& contentManager)
  {
    const std::size_t count = Tonemapper::COUNT;
    rVector.resize(count);
    for (std::size_t i = 0; i < count; ++i)
    {
      rVector[i] = CreateTonemapShader(contentManager, static_cast<Tonemapper::Enum>(i));
    }
  }


  HDR03_SkyboxToneMapping::TonemapProgramInfo HDR03_SkyboxToneMapping::CreateTonemapShader(const std::shared_ptr<IContentManager>& contentManager,
                                                                                           const Tonemapper::Enum tonemapper)
  {
    TonemapProgramInfo info;
    const auto fragmentShaderName = GetTonemapperShaderName(tonemapper);

    info.Program.Reset(contentManager->ReadAllText("Tonemapper.vert"), contentManager->ReadAllText(fragmentShaderName));

    info.Location.Exposure = info.Program.TryGetUniformLocation("g_exposure");
    return info;
  }


  HDR03_SkyboxToneMapping::SimpleMesh HDR03_SkyboxToneMapping::CreateQuadVertexArray(const GLES3::GLProgram& program)
  {
    // B D
    // |\|
    // A C
    // A = 1.0

    const float size = 1.0f;
    const float x0 = -size;
    const float x1 = size;
    const float y0 = -size;
    const float y1 = size;
    const float zPos = 0.0f;

    const float u0 = 0.0f;
    const float u1 = 1.0f;
    const float v0 = 0.0f;
    const float v1 = 1.0f;

    std::array<VertexPositionTexture, 6> vertices = {
      // Floor
      VertexPositionTexture(Vector3(x0, y1, zPos), Vector2(u0, v1)), VertexPositionTexture(Vector3(x0, y0, zPos), Vector2(u0, v0)),
      VertexPositionTexture(Vector3(x1, y0, zPos), Vector2(u1, v0)),

      VertexPositionTexture(Vector3(x0, y1, zPos), Vector2(u0, v1)), VertexPositionTexture(Vector3(x1, y0, zPos), Vector2(u1, v0)),
      VertexPositionTexture(Vector3(x1, y1, zPos), Vector2(y1, v1)),
    };

    auto vertexDecl = VertexPositionTexture::GetVertexDeclaration();
    std::vector<GLES3::GLVertexAttribLink> attribLink(2);
    attribLink[0] =
      GLVertexAttribLink(program.GetAttribLocation("VertexPosition"), vertexDecl.VertexElementGetIndexOf(VertexElementUsage::Position, 0));
    attribLink[1] =
      GLVertexAttribLink(program.GetAttribLocation("VertexTexCoord"), vertexDecl.VertexElementGetIndexOf(VertexElementUsage::TextureCoordinate, 0));

    GLVertexBuffer vertexBuffer(vertices.data(), vertices.size(), GL_STATIC_DRAW);

    // Prepare the vertex arrays
    GLVertexArray vertexArray(true);
    vertexArray.Bind();
    {
      // Set up VBO Vertex Attribute information
      GL_CHECK(glBindBuffer(vertexBuffer.GetTarget(), vertexBuffer.Get()));
      vertexBuffer.EnableAttribArrays(attribLink);
    }
    vertexArray.Unbind();
    return SimpleMesh(std::move(vertexBuffer), std::move(vertexArray));
  }
}
