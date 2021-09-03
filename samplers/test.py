# usig ft(Fourier Transform) to test sampler's correctness


import numpy as np
from numpy import pi, sin, cos, sqrt
from numpy.random import random
from matplotlib import pyplot as plt

Point2D = np.dtype([('x', "float64"), ('y', "float64")])


##################################  Basic Sampler  ############################


class Sampler:
    name = "Sampler"

    def __init__(self, side: int) -> None:
        self._side_length = side
        self._num_samples = side * side
        self.samples: np.ndarray = np.ndarray([self._num_samples], Point2D)
        if side == 1:
            self.samples[0]['x'] = 0.5
            self.samples[0]['y'] = 0.5
        else:
            self._generate_samples()

    @staticmethod
    def map_to_circle(px: float, py: float) -> tuple[float, float]:
        px = px * 2. - 1.
        py = py * 2. - 1.
        if px > -py:
            if px > py:
                r = px;
                phi = py / px;
            else:
                r = py;
                phi = 2. - px / py;
        else:
            if px < py:
                r = -px;
                phi = 4. + py / px;
            else:
                r = -py;
                phi = 6. - px / py;
        phi *= pi / 4.;
        return r * cos(phi), r * sin(phi);

    @staticmethod
    def map_to_hemisphere(px: float, py: float, e: float) -> tuple[float, float, float]:
        cos_phi = cos(2. * np.pi * px);
        sin_phi = sin(2. * np.pi * px);
        cos_theta = pow(1. - py, 1. / (e + 1.));
        sin_theta = sqrt(1. - cos_theta * cos_theta);
        return sin_theta * cos_phi, sin_theta * sin_phi, cos_theta;

    def num_samples(self) -> int:
        return self._num_samples

    def side_length(self) -> int:
        return self._side_length

    def _generate_samples(self) -> None:
        raise NotImplementedError


##################################  Samplers  #################################


class Regular(Sampler):
    name = "Regular Sampler"

    def __init__(self, side: int) -> None:
        super().__init__(side)

    def _generate_samples(self) -> None:
        coord = np.linspace(0., 1., self._side_length, endpoint=False) + 0.5 / self._side_length
        coord = np.tile(coord[np.newaxis, :], [self._side_length, 1])
        self.samples['x'] = coord.flatten()
        self.samples['y'] = coord.T.flatten()


class PureRandom(Sampler):
    name = "PureRandom Sampler"

    def __init__(self, side: int) -> None:
        super().__init__(side)

    def _generate_samples(self) -> None:
        self.samples['x'] = random([self._num_samples])
        self.samples['y'] = random([self._num_samples])


class Jittered(Sampler):
    name = "Jittered Sampler"

    def __init__(self, side: int) -> None:
        super().__init__(side)

    def _generate_samples(self) -> None:
        cell_size = 1. / self._side_length
        coord = np.linspace(0., 1., self._side_length, endpoint=False)
        coord = np.tile(coord[np.newaxis, :], [self._side_length, 1])
        self.samples['x'] = coord.flatten() + random([self._num_samples]) * cell_size
        self.samples['y'] = coord.T.flatten() + random([self._num_samples]) * cell_size


class NRooks(Sampler):
    name = "NRooks Sampler"

    def __init__(self, side: int) -> None:
        super().__init__(side)

    def _generate_samples(self) -> None:
        cell_size = 1. / self._num_samples
        coord = np.linspace(0., 1., self._num_samples, endpoint=False)
        self.samples['x'] = coord + random([self._num_samples]) * cell_size
        self.samples['y'] = coord + random([self._num_samples]) * cell_size
        self._shuffle_coordinates()

    def _shuffle_coordinates(self) -> None:
        np.random.shuffle(self.samples['x'])
        np.random.shuffle(self.samples['y'])


class MultiJittered(Sampler):
    name = "MultiJittered Sampler"

    def __init__(self, side: int) -> None:
        super().__init__(side)

    def _generate_samples(self) -> None:
        cell_size = 1. / self._side_length
        coord = np.linspace(0., 1., self._side_length, endpoint=False)
        coord = np.tile(coord[np.newaxis, :], [self._side_length, 1])
        self.samples['x'] = coord.flatten() + (coord.T.flatten() + random([self._num_samples]) * cell_size) * cell_size
        self.samples['y'] = coord.T.flatten() + (coord.flatten() + random([self._num_samples]) * cell_size) * cell_size
        self._shuffle_x_coordinates()
        self._shuffle_y_coordinates()

    def _shuffle_x_coordinates(self) -> None:
        for x in range(self._side_length):
            np.random.shuffle(self.samples['x'][x : self._num_samples - self._side_length + x : self._side_length])

    def _shuffle_y_coordinates(self) -> None:
        for y in range(self._side_length):
            np.random.shuffle(self.samples['y'][y * self._side_length : (y + 1) * self._side_length])


class Hammersley(Sampler):
    name = "Hammersley Sampler"

    def __init__(self, side: int) -> None:
        super().__init__(side)

    @staticmethod
    def phi (x: int) -> float:
        y = 0.
        scaler = 0.5
        while x > 0:
            y += scaler * (x & 1)
            x >>= 1
            scaler *= 0.5
        return y

    def _generate_samples(self) -> None:
        coord = list(range(self._num_samples))
        self.samples['x'] = np.array(coord) * (1. / self._num_samples)
        self.samples['y'] = np.array([Hammersley.phi(x) for x in coord])


###############################  testing functions  ###########################


SIDE = 8
POINT_SIZE = 3.0


def draw_mapping_samples(sampler: Sampler):
    fig = plt.figure(figsize=(9., 6.))
    fig.canvas.manager.set_window_title(sampler.name)

    # draw samples in unit-square
    ax = plt.subplot(2, 3, 1)
    ax.xaxis.set_major_locator(plt.NullLocator())
    ax.yaxis.set_major_locator(plt.NullLocator())
    ax.set_title("in square")
    plt.xlim(0., 1.); plt.ylim(0., 1.)
    plt.scatter(sampler.samples['x'], sampler.samples['y'], s=POINT_SIZE)

    # draw samples in unit-circle
    ax = plt.subplot(2, 3, 4)
    ax.xaxis.set_major_locator(plt.NullLocator())
    ax.yaxis.set_major_locator(plt.NullLocator())
    ax.set_title("in circle")
    plt.xlim(-1., 1.); plt.ylim(-1., 1.)
    samples = np.array([Sampler.map_to_circle(p['x'], p['y']) for p in sampler.samples])
    circle = np.array([(cos(theta), sin(theta)) for theta in np.linspace(0, 2. * pi, 100, endpoint=False)])
    plt.plot(circle[:, 0], circle[:, 1], linewidth=0.5)
    plt.scatter(samples[:,0], samples[:,1], s=POINT_SIZE)

    #draw samples in hemisphere (top, e=1)
    ax = plt.subplot(2, 3, 2)
    ax.xaxis.set_major_locator(plt.NullLocator())
    ax.yaxis.set_major_locator(plt.NullLocator())
    ax.set_title("in hemisphere (e=1, top)")
    plt.xlim(-1., 1.); plt.ylim(-1., 1.)
    samples = np.array([Sampler.map_to_hemisphere(p['x'], p['y'], 1.) for p in sampler.samples])
    plt.plot(circle[:, 0], circle[:, 1], linewidth=0.5)
    plt.scatter(samples[:, 0], samples[:, 1], s=POINT_SIZE)

    #draw samples in hemisphere (side, e=1)
    ax = plt.subplot(2, 3, 5)
    ax.xaxis.set_major_locator(plt.NullLocator())
    ax.yaxis.set_major_locator(plt.NullLocator())
    ax.set_title("in hemisphere (e=1, side)")
    plt.xlim(-1., 1.); plt.ylim(-0.5, 1.5)
    half_circle = np.concatenate([circle[:50, :],
        np.concatenate(
            [np.linspace(-1., 1., 50)[:, np.newaxis],
            np.zeros([50, 1])], axis=1
        )], axis=0)
    plt.plot(half_circle[:, 0], half_circle[:, 1], linewidth=0.5)
    plt.scatter(samples[:, 0], samples[:, 2], s=POINT_SIZE)

    #draw samples in hemisphere (top, e=5)
    ax = plt.subplot(2, 3, 3)
    ax.xaxis.set_major_locator(plt.NullLocator())
    ax.yaxis.set_major_locator(plt.NullLocator())
    ax.set_title("in hemisphere (e=5, top)")
    plt.xlim(-1., 1.); plt.ylim(-1., 1.)
    samples = np.array([Sampler.map_to_hemisphere(p['x'], p['y'], 5.) for p in sampler.samples])
    plt.plot(circle[:, 0], circle[:, 1], linewidth=0.5)
    plt.scatter(samples[:, 0], samples[:, 1], s=POINT_SIZE)

    #draw samples in hemisphere (top, e=25)
    ax = plt.subplot(2, 3, 6)
    ax.xaxis.set_major_locator(plt.NullLocator())
    ax.yaxis.set_major_locator(plt.NullLocator())
    ax.set_title("in hemisphere (e=25, top)")
    plt.xlim(-1., 1.); plt.ylim(-1., 1.)
    samples = np.array([Sampler.map_to_hemisphere(p['x'], p['y'], 25.) for p in sampler.samples])
    plt.plot(circle[:, 0], circle[:, 1], linewidth=0.5)
    plt.scatter(samples[:, 0], samples[:, 1], s=POINT_SIZE)


draw_mapping_samples(Regular(SIDE))
plt.show()

draw_mapping_samples(PureRandom(SIDE))
plt.show()

draw_mapping_samples(Jittered(SIDE))
plt.show()

draw_mapping_samples(NRooks(SIDE))
plt.show()

draw_mapping_samples(MultiJittered(SIDE))
plt.show()

draw_mapping_samples(Hammersley(SIDE))
plt.show()
