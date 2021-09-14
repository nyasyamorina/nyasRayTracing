/// @file brdfs/Lambertian.hpp
#pragma once

#include "BRDF.hpp"
#include "../common/constants.hpp"


namespace nyas
{
    namespace BRDFs
    {
        /// Lambertian
        ///
        /// @param diffuse Diffuse reflection coefficient in range [0, 1]
        class Lambertian : public BRDF
        {
        public:
            Lambertian()
                : _diffuse(0.)
            {}
            explicit Lambertian(float32 const& diffuse)
                : _diffuse(diffuse)
            {}

            Lambertian inline & set_diffuse(float32 const& diffuse)
            {
                this->_diffuse = diffuse;
                return *this;
            }

            float32 inline diffuse() const
            {
                return this->_diffuse;
            }

            float32 virtual operator()(Vector3D const& normal, Vector3D const& incident, Vector3D const& outgoing) const override
            {
                return this->_diffuse;
            }


        private:
            float32 _diffuse;
        };

        typedef shared_ptr<Lambertian> LambertianPtr;
        typedef shared_ptr<Lambertian const> LambertianConstptr;

    } // namespace BRDFs

} // namespace nyas
