/// @file Buffer2D.hpp
#pragma once

#include "common/setup.h"
#include "common/types.hpp"
#include "utils.hpp"
#include <assert.h>
#include <cstring>
#include <functional>
#include <fstream>


namespace nyas
{
    template<typename T>
    class Buffer2D final
    {
    public:
        typedef T Data;
        typedef ::std::function<void(Data &)> ConverFunc;
        typedef ::std::function<void(Length2D const&, Data &)> ConverWithIndexFunc;
        template<typename U> using MapFunc = ::std::function<U(Data const&)>;
        template<typename U> using MapWithIndexFunc = ::std::function<U(Length2D const&, Data const&)>;


        /* Constructors */
        Buffer2D()
            : _size(0, 0)
            , _data(nullptr)
        {}
        explicit Buffer2D(Length2D const& size)
            : _size(size)
            , _data(new Data[size.x * size.y]())
        {
            assert(size.x > 0 && size.y > 0);
        }
        explicit Buffer2D(Length2D const& size, Data * const data_ptr, bool copy = true)
            : _size(size)
        {
            assert(size.x > 0 && size.y > 0);
            if (copy) {
                length_t const total = size.x * size.y;
                this->_data = new Data[total];
                if (this->_data != nullptr && data_ptr != nullptr) {
                    memcpy(this->_data, data_ptr, total * sizeof(Data));
                }
            }
            else {
                this->_data = data_ptr;
            }
        }
        explicit Buffer2D(length_t const& width, length_t const& height)
            : Buffer2D(Length2D(width, height))
        {}
        explicit Buffer2D(length_t const& width, length_t const& height, Data * const data_ptr, bool const& copy = true)
            : Buffer2D(Length2D(width, height), data_ptr, copy)
        {}
        Buffer2D(Buffer2D<Data> const& buff)
            : Buffer2D(buff._size, buff._data, true)
        {}
        Buffer2D(Buffer2D<Data> && buff)
            : Buffer2D()
        {
            ::std::swap(this->_size, buff._size);
            ::std::swap(this->_data, buff._data);
        }

        /* Destructor */
        ~Buffer2D()
        {
            delete[] this->_data;
        }

        bool inline valid() const
        {
            return this->_data != nullptr;
        }

        length_t inline width() const
        {
            return this->_size.x;
        }
        length_t inline height() const
        {
            return this->_size.y;
        }
        Length2D inline size() const
        {
            return this->_size;
        }
        length_t inline total() const
        {
            return this->_size.x * this->_size.y;
        }
        Data inline * data_pointer()
        {
            return this->_data;
        }
        Data inline const* data_pointer() const
        {
            return this->_data;
        }

        /* position conversion */
        Point2D inline at(Length2D const& index) const
        {
            Point2D const static inverse_size = 1. / Point2D(this->_size);
#ifdef REDUCE_POINT_BEYOND_RANGE
            return reduce_over01(Point2D(index) * inverse_size) * 2. - 1.;
#else
            return Point2D(index) * inverse_size * 2. - 1.;
#endif
        }
        Length2D inline at(Point2D const& position) const
        {
            Point2D const static size = Point2D(this->_size);
#ifdef REDUCE_POINT_BEYOND_RANGE
            return Length2D(reduce_over01(position * 0.5 + 0.5) * size);
#else
            return Length2D((position * 0.5 + 0.5) * size);
#endif
        }

        /* access elements */
        Data inline & operator()(Length2D const& index)
        {
            assert(0 <= index.x && index.x < _size.x && 0 <= index.y && index.y < _size.y);
            return *(this->_data + (index.y * this->_size.x + index.x));
        }
        Data inline const& operator()(Length2D const& index) const
        {
            assert(0 <= index.x && index.x < _size.x && 0 <= index.y && index.y < _size.y);
            return *(this->_data + (index.y * this->_size.x + index.x));
        }
        Data inline & operator()(length_t const& x, length_t const& y)
        {
            return this->operator()(Length2D(x, y));
        }
        Data inline const& operator()(length_t const& x, length_t const& y) const
        {
            return this->operator()(Length2D(x, y));
        }

        /* converse elements */
        Buffer2D & for_each(ConverFunc const& func)
        {
            Data * iter = this->_data;
            length_t const total = this->total();
            for (length_t i = 0; i < total; ++i) {
                func(*(iter++));
            }
            return *this;
        }
        Buffer2D & for_each_index(ConverWithIndexFunc const& func)
        {
            Data * iter = this->_data;
            Length2D index;
            for (index.y = 0; index.y < this->_size.y; ++index.y) {
                for(index.x = 0; index.x < this->_size.x; ++index.x) {
                    func(index, *(iter++));
                }
            }
            return *this;
        }
        // TODO: Buffer2D & for_each_multithreads(ConverFunc const& func)
        // TODO: Buffer2D & for_each_index_multithreads(ConverFunc const& func)

        /* mapping */
        template<typename U>
        Buffer2D<U> map(MapFunc<U> const& func) const
        {
            Buffer2D<U> buff(this->_size);
            if (buff.data_pointer() != nullptr) {
                mapping_data(func, this->_data, buff.data_pointer(), this->total());
            }
            return buff;
        }
        template<typename U>
        Buffer2D<U> & map(MapFunc<U> const& func, Buffer2D<U> & buff) const
        {
            assert(buff.size() == this->_size);
            length_t const total = this->total();
            if (buff.data_pointer() != nullptr && buff.total() >= total) {
                mapping_data(func, this->_data, buff.data_pointer(), total);
            }
            return buff;
        }


    private:
        Length2D _size;
        Data * _data;
    };

    template<typename T> using Buffer2DPtr = shared_ptr<Buffer2D<T>>;
    template<typename T> using Buffer2DConstptr = shared_ptr<Buffer2D<T> const>;

    typedef Buffer2D<RGBColor> GraphicsBuffer;
    typedef Buffer2D<ImageRGBColor> ImageBuffer;


    /* GraphicsBuffer to ImageBuffer */

    ImageRGBColor inline RGBcolor_to_imageRGBcolor(RGBColor const& color)
    {
        return ImageRGBColor(clamp(256.f * color, 0.f, 255.f));
    }

    ImageBuffer inline map_to_image(GraphicsBuffer const& gbuff)
    {
        return gbuff.map<ImageRGBColor>(RGBcolor_to_imageRGBcolor);
    }

    ImageBuffer inline & map_to_image(GraphicsBuffer const& gbuff, ImageBuffer & ibuff)
    {
        return gbuff.map<ImageRGBColor>(RGBcolor_to_imageRGBcolor, ibuff);
    }


    /* gamma correction */
    float32 constexpr gamma_correction_value = 1.f / 2.2f;
    RGBColor inline gamma_correction_color(RGBColor const& color)
    {
        return pow(color, gamma_correction_value);
    }
    void inline gamma_correction_color_builtin(RGBColor & color)
    {
        color = pow(color, gamma_correction_value);
    }
    GraphicsBuffer inline & gamma_correction(GraphicsBuffer & gbuff)
    {
        return gbuff.for_each(gamma_correction_color_builtin);
    }


    /* save to image */

    void save_bmp(char const* file_name, ImageBuffer const& buff)
    {
        length_t width = buff.width(), height = buff.height();

        char align_buff[3] {0, 0, 0};
        length_t const align = width % 4;
        length_t buffer_size = (3 * width + align) * height;
        length_t file_size = buffer_size + 54;

        uint8 header[54] = {
            66, 77, 88, 88, 88, 88,  0,  0,  0,  0, 54,  0,  0,  0, 40,  0,
            0,   0, 88, 88, 88, 88, 88, 88, 88, 88,  1,  0, 24,  0,  0,  0,
            0,   0, 88, 88, 88, 88,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
            0,   0,  0,  0,  0,  0
        };
        uint8 * ptr = header + 2;
        *(ptr++) = file_size & 0xFF; *(ptr++) = (file_size >>= 8) & 0xFF; *(ptr++) = (file_size >>= 8) & 0xFF; *ptr = (file_size >>= 8) & 0xFF;
        ptr += 13;
        *(ptr++) = width & 0xFF; *(ptr++) = (width >>= 8) & 0xFF; *(ptr++) = (width >>= 8) & 0xFF; *(ptr++) = (width >>= 8) & 0xFF;
        *(ptr++) = height & 0xFF; *(ptr++) = (height >>= 8) & 0xFF; *(ptr++) = (height >>= 8) & 0xFF; *ptr = (height >>= 8) & 0xFF;
        ptr += 9;
        *(ptr++) = buffer_size & 0xFF; *(ptr++) = (buffer_size >>= 8) & 0xFF; *(ptr++) = (buffer_size >>= 8) & 0xFF; *ptr = (buffer_size >>= 8) & 0xFF;


        ::std::ofstream outfile;
        outfile.open(file_name, ::std::ios::out | ::std::ios::binary | ::std::ios::trunc);

        outfile.write(reinterpret_cast<char const*>(header), 54);
        ImageRGBColor const* data_ptr = buff.data_pointer();

        for (length_t h = 0; h < buff.height(); ++h) {
            for (length_t w = 0; w < buff.width(); ++w) {
                outfile.write(reinterpret_cast<char const*>(&data_ptr->b), 1);
                outfile.write(reinterpret_cast<char const*>(&data_ptr->g), 1);
                outfile.write(reinterpret_cast<char const*>(&data_ptr->r), 1);
                ++data_ptr;
            }
            outfile.write(align_buff, align);
        }
        outfile.close();
    }
    void inline save_bmp(string const& str, ImageBuffer const& buff)
    {
        save_bmp(str.c_str(), buff);
    }

} // namespace nyas
