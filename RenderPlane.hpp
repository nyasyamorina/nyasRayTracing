/// @file RenderPlane.hpp
#pragma once

#include "common/types.hpp"
#include "common/functions.hpp"
#include "GraphicsBuffer.hpp"
#include <memory>


namespace nyas
{
    class RenderPlane
    {
    public:
        typedef GraphicsBuffer<float32> GBuff;


        explicit RenderPlane(length_t figture_width, length_t figture_height)
            : RenderPlane(Length2D(figture_width, figture_height))
        {}
        explicit RenderPlane(Length2D const& size)
            : _gbuff(size)
            , _center(0.)
            , _horizontal(0.)
            , _vertical(0.)
        {}
        RenderPlane(RenderPlane const&) = default;

        RenderPlane & operator=(RenderPlane const&) = default;

        bool inline valid() const
        {
            /* RenderPlane's directions should not close to zero vector, it will make some numerical problem. */
            return _gbuff.valid() && !(near_to_zero(_horizontal) || near_to_zero(_vertical));
        }

        void inline set_center(Point3D const& c)
        {
            _center = c;
        }
        void inline set_horizontal_direction(Vector3D const& h)
        {
            _horizontal = h;
        }
        void inline set_vertical_direction(Vector3D const& v)
        {
            _vertical = v;
        }
        void inline set_directions(Vector3D const& h, Vector3D const& v)
        {
            _horizontal = h;
            _vertical = v;
        }

        length_t inline width() const
        {
            return _gbuff.width();
        }
        length_t inline height() const
        {
            return _gbuff.height();
        }
        Length2D inline const& size() const
        {
            return _gbuff.size();
        }
        Point3D inline const& center() const
        {
            return _center;
        }
        Vector3D inline const& horizontal_direction() const
        {
            return _horizontal;
        }
        Vector3D inline const& vertical_direction() const
        {
            return _vertical;
        }
        GBuff inline & buffer()
        {
            return _gbuff;
        }
        GBuff inline const& buffer() const
        {
            return _gbuff;
        }

        /// Return a point in space on render plane
        ///
        /// @param p point on plane, the range should be [-1, 1]
        Point3D inline at(Point2D const& p) const
        {
            return _center + p.x * _horizontal + p.y * _vertical;
        }

        /// Return a point in space on render plane
        ///
        /// @param pix pixel index on plane
        Point3D inline at(Length2D const& pix) const
        {
            Point2D static const half_pixel_offset = 0.5 / Point2D(_gbuff.size());
            return at(Point2D(pix) * 2. - 1. + half_pixel_offset);
        }


    protected:
        Point3D _center;
        Vector3D _horizontal, _vertical;
        GBuff _gbuff;
    };

    typedef std::shared_ptr<RenderPlane> RenderPlanePtr;
    typedef std::shared_ptr<RenderPlane const> RenderPlaneConstptr;

} // namespace nyas
