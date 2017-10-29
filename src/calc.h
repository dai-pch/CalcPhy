#pragma once
#ifndef CALC_H__
#define CALC_H__

#include <cmath>
#include "common.h"

inline vec3d force2_Van(vec3d dest, vec3d source, double sigma, double epsilon) {
	auto r = dest - source;
	auto r_mod = r.abs();
	auto r_2 = r_mod * r_mod;
	auto r_6 = pow(r_mod, 6);
	auto r_8 = r_6 * r_2;
	auto sigma_6 = pow(sigma, 6);

	auto sur = 24 * epsilon * sigma_6;
	vec3d force;
	for (int i = 0; i < 3; ++i)
		force[i] = sur * (2 * sigma_6 / r_6 - 1) * r[i] / r_8;

	return force;
}

inline vec3d force_Van(std::shared_ptr<std::vector<vec3d>> atoms, size_t desId, double sigma, double epsilon) {
	vec3d force = { 0.0, 0.0, 0.0 };
	for (size_t ii = 0; ii < atoms->size(); ++ii)
		if (ii != desId)
			force = force + force2_Van(atoms->operator[](desId), atoms->operator[](ii), sigma, epsilon);
	return force;
}

inline double potential_Van(std::shared_ptr<std::vector<vec3d>> atoms, double sigma, double epsilon) {
	double potential = 0;
	for (int jj = 0; jj < atoms->size(); ++jj)
		for (int ii = 0; ii < jj; ++ii) {
			auto r = atoms->operator[](ii) - atoms->operator[](jj);
			auto r_mod = r.abs();
			auto sigma_div_r = sigma / r_mod;
			auto sdr_6 = pow(sigma_div_r, 6);
			auto sdr_12 = sdr_6 * sdr_6;
			potential += sdr_12 - sdr_6;
		}
	potential *= 4 * epsilon;
	return potential;
}

#endif // CALC_H__