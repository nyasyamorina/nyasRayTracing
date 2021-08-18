/// @file Camera.hpp
#pragma once

#include "common/types.hpp"
#include "RenderPlane.hpp"
#include <memory>


namespace nyas
{
    class Camera
    {
    public:
        Camera()
            : _view_point(0., 0., 0.)
            , _render_plane(nullptr)
        {}

        bool inline valid() const
        {
            return _render_plane != nullptr && _render_plane->valid();
        }

        void inline set_view_point(Point3D const& p)
        {
            _view_point = p;
        }
        /// ! This method will generate a new figure
        void inline set_figure_size(length_t width, length_t height)
        {
            set_figure_size(Length2D(width, height));
        }
        /// ! This method will generate a new figure
        void set_figure_size(Length2D const& size)
        {
            if (_render_plane == nullptr) {
                _render_plane = std::make_shared<RenderPlane>(size);
            }
            else {
                RenderPlanePtr _old_render_plane = _render_plane;
                _render_plane = std::make_shared<RenderPlane>(size);
                _render_plane->set_center(_old_render_plane->center());
                _render_plane->set_directions(_old_render_plane->horizontal_direction(), _old_render_plane->vertical_direction());
            }
        }
        void inline set_figure(RenderPlanePtr render_plane)
        {
            _render_plane = render_plane;
        }

        Point3D inline view_point() const
        {
            return _view_point;
        }
        RenderPlanePtr inline render_plane()
        {
            return _render_plane;
        }
        RenderPlaneConstptr inline render_plane() const
        {
            return RenderPlaneConstptr(_render_plane);
        }


    private:
        Point3D _view_point;
        RenderPlanePtr _render_plane;
    };

    typedef std::shared_ptr<Camera> CameraPtr;
    typedef std::shared_ptr<Camera const> CameraConstptr;


    /// get a default camera
    ///
    /// @param view_point poosition of camera
    /// @param view_direction direction where camera look at
    /// @param view_length distance from camera to render plane
    /// @param up_direction roughly upward direction in figure
    /// @param FOV_rad field of view in radian
    /// @param figure_size size of render plane
    CameraPtr default_camera(
        Point3D const& view_point, Vector3D const& view_direction,
        double view_length, Vector3D const& up_direction, double FOV_rad,
        Length2D const& figure_size
    )
    {
        CameraPtr camera = std::make_shared<Camera>();
        camera->set_view_point(view_point);
        camera->set_figure_size(figure_size);
        RenderPlanePtr render_plane = camera->render_plane();
        if (render_plane != nullptr) {
            Vector3D const u = normalize(cross(view_direction, up_direction));
            Vector3D const v = normalize(cross(u, view_direction));
            float64 const fov_scaler = tan(FOV_rad * 0.5);
            render_plane->set_center(view_length * normalize(view_direction));
            render_plane->set_directions(fov_scaler * u, (fov_scaler * figure_size.y / figure_size.x) * v);
        }
        return camera;
    }

} // namespace nyas
