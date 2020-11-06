// ----------------------------------------------------------------------------
// -                        Open3D: www.open3d.org                            -
// ----------------------------------------------------------------------------
// The MIT License (MIT)
//
// Copyright (c) 2020 www.open3d.org
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
// FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
// IN THE SOFTWARE.
// ----------------------------------------------------------------------------

#pragma once

#include <memory>
#include <set>
#include <string>

#include "open3d/visualization/gui/Window.h"
#include "open3d/visualization/rendering/Material.h"

namespace open3d {

namespace geometry {
class Geometry3D;
}  // namespace geometry

namespace t {
namespace geometry {
class Geometry;
}  // namespace geometry
}  // namespace t

namespace visualization {

namespace rendering {
class Open3DScene;
}  // namespace rendering

namespace visualizer {

class DrawVisualizer : public gui::Window {
    using Super = gui::Window;

public:
    enum class Shader { STANDARD, NORMALS, DEPTH };

    struct DrawObject {
        std::string name;
        std::shared_ptr<geometry::Geometry3D> geometry;
        std::shared_ptr<t::geometry::Geometry> tgeometry;
        rendering::Material material;
        std::string group;
        double time = 0.0;
        bool is_visible = true;

        // internal
        bool is_color_default = true;
    };

    struct UIState {
        Shader scene_shader = Shader::STANDARD;
        bool show_settings = false;
        bool show_skybox = false;
        bool show_axes = false;
        bool is_animating = false;
        std::set<std::string> enabled_groups;

        Eigen::Vector4f bg_color = {1.0f, 1.0f, 1.0f, 1.0f};

        bool use_ibl = false;
        bool use_sun = true;
        std::string ibl_path = "";  // "" is default path
        int ibl_intensity = 0;
        int sun_intensity = 100000;
        Eigen::Vector3f sun_dir = {0.577f, -0.577f, -0.577f};
        Eigen::Vector3f sun_color = {1.0f, 1.0f, 1.0f};

        double current_time = 0.0;   // seconds
        double time_step = 1.0;      // seconds
        double frame_delay = 0.100;  // seconds
    };

    DrawVisualizer(const std::string& title, int width, int height);
    virtual ~DrawVisualizer();

    void AddAction(const std::string& name,
                   std::function<void(DrawVisualizer&)> callback);
    void AddMenuAction(const std::string& name,
                       std::function<void(DrawVisualizer&)> callback);

    void SetBackgroundColor(const Eigen::Vector4f& bg_color);

    void SetShader(Shader shader);

    void AddGeometry(const std::string& name,
                     std::shared_ptr<geometry::Geometry3D> geom,
                     rendering::Material* material = nullptr,
                     const std::string& group = "",
                     double time = 0.0,
                     bool is_visible = true);

    void AddGeometry(const std::string& name,
                     std::shared_ptr<t::geometry::Geometry> tgeom,
                     rendering::Material* material = nullptr,
                     const std::string& group = "",
                     double time = 0.0,
                     bool is_visible = true);

    void RemoveGeometry(const std::string& name);

    void ShowGeometry(const std::string& name, bool show);

    DrawObject GetGeometry(const std::string& name) const;

    void ResetCamera();

    void ShowSettings(bool show);
    void ShowSkybox(bool show);
    void ShowAxes(bool show);
    void EnableGroup(const std::string& group, bool enable);

    double GetAnimationFrameDelay() const;
    void SetAnimationFrameDelay(double secs);

    double GetAnimationTimeStep() const;
    void SetAnimationTimeStep(double time_step);

    double GetCurrentTime() const;
    void SetCurrentTime(double t);

    bool GetIsAnimating() const;
    void SetAnimating(bool is_animating);

    void ExportCurrentImage(const std::string& path);

    UIState GetUIState() const;
    rendering::Open3DScene* GetScene() const;

protected:
    void Layout(const gui::Theme& theme);

private:
    struct Impl;
    std::unique_ptr<Impl> impl_;
};

}  // namespace visualizer
}  // namespace visualization
}  // namespace open3d