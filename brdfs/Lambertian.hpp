/// @file brdfs/Lambertian.hpp
#pragma once

#include "../common/constants.hpp"
#include "BRDF.hpp"


namespace nyas
{
    namespace BRDFs
    {
        /// Lambertian
        ///
        /// @param diffuse Diffuse reflection coefficient in range [0, 1]
        /// @param color Diffuse reflection color in range [0, 1]^3
        class Lambertian : public BRDF
        {
        public:
            Lambertian()
                : _diffuse(0.)
                , _color(0.)
            {}
            explicit Lambertian(float32 diffuse, RGBColor const& color)
                : _diffuse(diffuse)
                , _color(color)
            {}

            Lambertian inline & set_diffuse(float32 diffuse)
            {
                this->_diffuse = diffuse;
                return *this;
            }
            Lambertian inline & set_color(RGBColor const& color)
            {
                this->_color = color;
                return *this;
            }

            float32 inline diffuse() const
            {
                return this->_diffuse;
            }
            RGBColor inline color() const
            {
                return this->_color;
            }

            RGBColor virtual operator()(Vector3D const& normal, Vector3D const& incident, Vector3D const& outgoing) const override
            {
                float32 constexpr inverse_two_pi = one_over_two_pi<float32>();
                return this->_diffuse * this->_color * inverse_two_pi;
            }

            tuple<Vector3D, RGBColor> virtual scatter(Vector3D const& normal, Vector3D const& incident) const override
            {

            }


        private:
            float32 _diffuse;
            RGBColor _color;
        };

        typedef ::std::shared_ptr<Lambertian> LambertianPtr;
        typedef ::std::shared_ptr<Lambertian const> LambertianConstptr;

    } // namespace BRDFs

} // namespace nyas
