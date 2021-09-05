/// @file examples.hpp
#pragma once

#include "utils.hpp"
#include "common/types.hpp"
#include "common/functions.hpp"
#include "common/vec_output.hpp"
#include "Buffer2D.hpp"
#include "samplers/Sampler.hpp"
#include "samplers/Regular.hpp"
#include "samplers/PureRandom.hpp"
#include "samplers/Jittered.hpp"
#include "samplers/NRooks.hpp"
#include "samplers/MultiJittered.hpp"
#include "samplers/Hammersley.hpp"
#include "Ray.hpp"
#include "cameras/Camera.hpp"
#include "cameras/Parallel.hpp"
#include "cameras/Pinhole.hpp"
#include <iostream>
#include <string>

using ::std::string;
using ::std::cout;
using ::std::cerr;
using ::std::endl;

string const output_dir = "outputs/";


namespace nyas
{
    /// test for fast inverse square root.
    void test_inersesqrt()
    {
        cout << "Example: test_inersesqrt" << endl;

        /* some number in a dvec4 */
        /* also use float32 for low precision floating-point */
        vec<4, float64> exam(2., 3., 4., 5.);
        auto default_res = glm::inversesqrt(exam);
        auto fast_res = nyas::inversesqrt(exam);        // fast inversesqrt
        cout << "default result: " << default_res << endl;
        cout << "fast result: " << fast_res << endl;
        cout << "relative error: " << ((fast_res - default_res) / exam) << endl;

        cout << endl;
    }


    /// example for graphics buffer, rendering process and output an image
    void example_rendering_output()
    {
        cout << "Example: example_rendering_output" << endl;

        /* set and create output directory */
        if (!makedir(output_dir)) {
            cerr << "Cannot create directory: '" << output_dir << '\'' << endl;
            return;
        }

        /* set image size */
        length_t constexpr width = 400;
        length_t constexpr height = 400;
        /* alloc buffers */
        // gbuff uses float32 type to store color data, value should in range [0.f, 1.f]
        GraphicsBuffer gbuff(width, height);
        /* if buffer is not alloc correctly, then return */
        if (!gbuff.valid())
            return;
        /* use pixel index rendering color into gbuff */
        gbuff.for_each_index(
            [] (Length2D const& index, RGBColor & pixel) {      // rendering method
                pixel.r = float(index.x) / (width - 1);
                pixel.g = float(index.y) / (height - 1);
                pixel.b = 0.2f;
            }
        );
        /* map floating-point color to display color in display output buffer */
        // ibuff is display output buffer, it uses uint8 type to store color data, value should in range [0, 255].
        ImageBuffer ibuff = map_to_image(gbuff);
        // save output bffer into bmp image
        save_bmp(output_dir + "gradient_color.bmp", ibuff);

        cout << endl;
    }


    /// example for samplers
    void example_samplers()
    {
        cout << "Example: example_samplers" << endl;

        /* set and create output directory */
        string const sampler_output_dir = output_dir + "sampler/";
        if (!makedir(output_dir)) {
            cerr << "Cannot create directory: '" << output_dir << '\'' << endl;
            return;
        }
        if (!makedir(sampler_output_dir)) {
            cerr << "Cannot create directory: '" << sampler_output_dir << '\'' << endl;
            return;
        }

        ImageRGBColor constexpr BLACK(0);
        ImageRGBColor constexpr WHITE(255);
        auto clean_to_black = [&BLACK] (ImageRGBColor & pixel) { pixel = BLACK; };

        auto plot_point = [&WHITE] (ImageBuffer & buff, Sampler & sampler) {
            for (Point2D const& sample : sampler.samples()) {
                buff(Length2D(Point2D(buff.size()) * sample)) = WHITE;
            }
        };

        // display output buffer
        ImageBuffer buff(256, 256);

        // regular sampling
        buff.for_each(clean_to_black);
        samplers::Regular samp1(8);
        plot_point(buff, samp1);
        save_bmp(sampler_output_dir + "regular.bmp", buff);

        // pure random sampling
        buff.for_each(clean_to_black);
        samplers::PureRandom samp2(8);
        plot_point(buff, samp2);
        save_bmp(sampler_output_dir + "pureRandom.bmp", buff);

        // jittered sampling
        buff.for_each(clean_to_black);
        samplers::Jittered samp3(8);
        plot_point(buff, samp3);
        save_bmp(sampler_output_dir + "jittered.bmp", buff);

        // n-Rooks sampling
        buff.for_each(clean_to_black);
        samplers::NRooks samp4(8);
        plot_point(buff, samp4);
        save_bmp(sampler_output_dir + "n-Rooks.bmp", buff);

        // multi-jittered sampling
        buff.for_each(clean_to_black);
        samplers::MultiJittered samp5(8);
        plot_point(buff, samp5);
        save_bmp(sampler_output_dir + "multiJittered.bmp", buff);

        // hammersley sampling
        buff.for_each(clean_to_black);
        samplers::Hammersley samp6(8);
        plot_point(buff, samp6);
        save_bmp(sampler_output_dir + "hammersley.bmp", buff);

        cout << endl;
    }


    /// example for cameras()
    void example_cameras()
    {
        cout << "Example: example_cameras" << endl;

        /* set and create output directory */
        string const camera_output_dir = output_dir + "camera/";
        if (!makedir(output_dir)) {
            cerr << "Cannot create directory: '" << output_dir << '\'' << endl;
            return;
        }
        if (!makedir(camera_output_dir)) {
            cerr << "Cannot create directory: '" << camera_output_dir << '\'' << endl;
            return;
        }

        /* render scenes function
               an infinity checkerboard on z=0
               a 'normal' ball (center = [0.5,0.5,1.0], radius=1)
        */
        Point3D const ball_center(0.5, 0.5, 1.0);
        float64 const ball_radius = 1.;
        auto render_ray = [&ball_center, &ball_radius] (Ray const& ray) -> RGBColor {
            float64 const static square_radius = ball_radius * ball_radius;
            float64 t_floor, t_ball;
            // time for ray hitting floor
            t_floor = -ray.origin.z / ray.direction.z;
            // time for ray hitting ball
            Vector3D const c2o = ray.origin - ball_center;
            float64 a = length2(ray.direction);
            float64 half_b = dot(ray.direction, c2o);
            float64 discriminant = half_b * half_b - a * (length2(c2o) - square_radius);
            if (discriminant < 0.) {
                t_ball = -1.;
            }
            else {
                float64 sqrt_d = sqrt(discriminant);
                t_ball = (-half_b - sqrt_d) / a;
                if (t_ball < 0.) {
                    t_ball = (-half_b + sqrt_d) / a;
                }
            }

            Point3D hitting_point;
            // if ray hit ball
            if (t_ball >= 0. && (t_ball < t_floor || t_floor < 0.)) {
                // return normal color on ball
                hitting_point = ray.at(t_ball);
                return RGBColor((hitting_point - ball_center) / ball_radius * 0.5 + 0.5);
            }
            // if ray hit floor
            else if (t_floor >= 0. && (t_floor < t_ball || t_ball < 0.)) {
                hitting_point = ray.at(t_floor);
                if (int(mod(floor(hitting_point.x) + floor(hitting_point.y), 2.)) == 1) {
                    return RGBColor(0.1f);       // checkerboard color 1
                }
                else {
                    return RGBColor(0.9f);       // checkerboard color 2
                }
            }
            // if ray does not hit anything
            else {
                return RGBColor(0.);
            }
        };

        /* render ray for each pixel and output image */
        auto render_and_output = [&render_ray] (Camera & camera, string const& output_path) {
            camera.figure().for_each_index(
                [&render_ray, &camera] (Length2D const& indexes, RGBColor & pixel) {
                    pixel = render_ray(camera.get_ray(indexes));
                }
            );
            save_bmp(output_path, map_to_image(camera.figure()));
        };

        /* set output image size */
        Length2D const figure_size(360, 240);

        /* parallel projection */
        cameras::ParallelPtr camera1 = cameras::default_parallel(
            figure_size, 6., Point3D(3.5, 3.5, 4.0), Vector3D(-1.)
        );
        render_and_output(*camera1, camera_output_dir + "parallele1.bmp");

        /* basic perspective projection */
        cameras::PinholePtr camera2 = cameras::default_pinhole(
            figure_size, Point3D(3.5, 3.5, 4.0), Vector3D(-1.), 60._deg
        );
        render_and_output(*camera2, camera_output_dir + "pinhole1.bmp");

        /* perspective projection but zoomed in a bit */
        cameras::PinholePtr camera3 = cameras::default_pinhole(
            figure_size, Point3D(3.5, 3.5, 4.0), Vector3D(-1.), 30._deg
        );
        render_and_output(*camera3, camera_output_dir + "pinhole2.bmp");

        /* perspective projection but rotated camera by some angle */
        auto [view_direction, view_up] = correct_view(225._deg, 125._deg, 30._deg);
        cameras::PinholePtr camera4 = cameras::default_pinhole(
            figure_size, Point3D(3.5, 3.5, 4.0), view_direction, 60._deg, view_up
        );
        render_and_output(*camera4, camera_output_dir + "pinhole3.bmp");

        cout << endl;
    }

} // namespace nyas
