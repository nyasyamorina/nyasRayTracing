/// @file skies/Zenith.hpp
#pragma once

#include "Sky.hpp"
#include "../common/types.hpp"
#include "../common/functions.hpp"


namespace nyas
{
    namespace skies
    {
        class Zenith final : public Sky
        {
        public:
            Zenith()
                : _zenith(0.f)
                , _ambient(0.f)
            {}
            explicit Zenith(RGBColor const& zenith, RGBColor const& ambient)
                : _zenith(zenith)
                , _ambient(ambient)
            {}

            Zenith inline & set_zenith(RGBColor const& zenith)
            {
                this->_zenith = zenith;
                return *this;
            }
            Zenith inline & set_ambient(RGBColor const& ambient)
            {
                this->_ambient = _ambient;
                return *this;
            }

            RGBColor inline zenith() const
            {
                return this->_zenith;
            }
            RGBColor inline ambient() const
            {
                return this->_ambient;
            }

            RGBColor virtual get_color(Vector3D const& direction) const override
            {
                return mix(this->_ambient, this->_zenith, static_cast<float32>(normalize(direction).y) * 0.5f + 0.5f);
            }


        private:
            RGBColor _zenith;
            RGBColor _ambient;
        };

        typedef shared_ptr<Zenith> ZenithPtr;
        typedef shared_ptr<Zenith const> ZenithConstptr;

    } // namespace skies

} // namespace nyas
